
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltiform.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                12-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary formula of an item
//
//
//==============================================================================

#include <stddef.h>
#include "cltiform.hpp"
#include "cltnform.hpp"




//=============================================================================
//
// ClTempItemFormula
//
// Clase que implementa las fórmulas de item temporales
//
//=============================================================================

// Funciones de inicialización ================================================

ClTempItemFormula :: ClTempItemFormula( void )
{
	m_acFormulaId[ 0 ] = '\0' ;
	m_acFormulaId[ 1 ] = '\0' ;

    m_iType = 0 ;
    m_iPriority = M4CL_RULE_MAX_PRIORITY ;
	m_iIsMetarule = 0 ;
	m_iIsRetroactivity = 0 ;
	m_iBatchType = 0 ;
	m_iIsBatchMetarule = 0 ;
	m_iIsEvent = 0 ;
	m_iIsCorrect = 1 ;
	m_iDBOrder = 0 ;
    m_pcPolish = NULL ;
    m_pccObject = NULL ;
    m_pccMethod = NULL ;

	m_dStartDate = 0 ;
	m_dEndDate = 0 ;

	m_poNitem = NULL ;

	m_acTiId  [ 0 ] = '\0' ;
	m_acTiId  [ 1 ] = '\0' ;
	m_acItemId[ 0 ] = '\0' ;
	m_acItemId[ 1 ] = '\0' ;

	m_bIsDatabase = M4_FALSE ;
}




// Funciones de escritura de parte fija =======================================

void	ClTempItemFormula :: SetPolish( m4pchar_t ai_pcPolish, IFuncTableDesc *ai_poTable )
{

/*
	m4return_t	iResult ;
	m4bool_t	bIsDatabase ;
	m4int8_t	iAuxChar ;
	m4uint16_t	iAuxFunction ;
*/

    m_pcPolish = ai_pcPolish ;

	/* Desactivación automatismos
	if( ai_poTable != NULL && m_iType == M4CL_INTERN_OBJECT_LN4 )
	{
		while( ai_pcPolish != NULL && m_bIsDatabase == M4_FALSE )
		{
			switch( *ai_pcPolish )
			{
				case 'F':
				case 'H':

					iAuxFunction = ClTempNitemFormula::_ReadPositive( ai_pcPolish + 1, '\n' ) ;

					iResult = ai_poTable->Exists( iAuxFunction, iAuxChar );

					if( iResult == M4_SUCCESS )
					{
						bIsDatabase = ai_poTable->GetIsDatabase( iAuxFunction ) ;

						if( bIsDatabase == M4_TRUE )
						{
							m_bIsDatabase = M4_TRUE ;
						}
					}
					break;
			}
		
			ai_pcPolish = (m4pchar_t) ClTempNitemFormula::GetNextLine( ai_pcPolish ) ;
		}
	}
	*/
}




void	ClTempItemFormula :: SetMethod( m4pcchar_t ai_pccMethod, IFuncTableDesc *ai_poTable )
{

/*
	m4return_t	iResult ;
	m4bool_t	bIsDatabase ;
	m4uint16_t	iAuxFunction ;
	m4char_t	acName[ M4CL_MAX_NOMBRE_FUNC + 1 ] ;
*/

    m_pccMethod = ai_pccMethod ;

	/* Desactivación automatismos
	if( ai_poTable != NULL && m_iType == M4CL_INTERN_OBJECT_CPP && ai_pccMethod != NULL )
	{
		strcpy( acName, ai_pccMethod ) ;
		strupr( acName ) ;

		iResult = ai_poTable->GetId( acName, iAuxFunction );

		if( iResult == M4_SUCCESS )
		{
			bIsDatabase = ai_poTable->GetIsDatabase( iAuxFunction ) ;

			if( bIsDatabase == M4_TRUE )
			{
				m_bIsDatabase = M4_TRUE ;
			}
		}
	}
	*/
}


