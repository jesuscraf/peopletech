//==============================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             property.cpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             22/07/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    This module defines a property
//
//    
//
//==============================================


#include "property.h"
#include "changelistener.hpp"


m4return_t	ClGenericProperty::_Set( const m4VariantType& ai_rcvValue, m4bool_t ai_bCheck )
{
	if( m_vValue != ai_rcvValue )
	{
		if( ai_bCheck == M4_TRUE && GetFlags( M4_PROP_WRITE ) == M4_FALSE )
		{
			// No puedo asignar a una propiedad de no escritura
			DUMP_CHLOG_WARNING( M4_CH_DM_NO_WRITE_PROP ) ;
			return( M4_ERROR ) ;
		}

		m_vValue.Set( ai_rcvValue, M4_TRUE ) ;

		if( m_poChangeListener != NULL )
		{
			m_poChangeListener->SetChange() ;
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t ClGenericProperty::_Serialize( ClGenericIODriver& IOD )
{

	m4return_t	iResult ;
	m4uint8_t	iInteger ;


	switch( m_vValue.Type )
	{
		case M4CL_CPP_TYPE_NUMBER :

			if( m_vValue.Data.DoubleData == -1 )
			{
				iInteger = M4_DMPROP_MINUS_ONE ;
			}
			else if( m_vValue.Data.DoubleData == 0 )
			{
				iInteger = M4_DMPROP_CERO ;
			}
			else if( m_vValue.Data.DoubleData == 1 )
			{
				iInteger = M4_DMPROP_ONE ;
			}
			else if( m_vValue.Data.DoubleData == 2 )
			{
				iInteger = M4_DMPROP_TWO ;
			}
			else if( m_vValue.Data.DoubleData == 3 )
			{
				iInteger = M4_DMPROP_THREE ;
			}
			else
			{
				iInteger = M4_DMPROP_NUMBER ;
			}

			break ;

		case M4CL_CPP_TYPE_DATE :

			if( m_vValue.Data.DoubleData == M4CL_MINUS_INFINITE_IN_JULIAN )
			{
				iInteger = M4_DMPROP_MINUS_INF ;
			}
			else if( m_vValue.Data.DoubleData == M4CL_PLUS_INFINITE_IN_JULIAN )
			{
				iInteger = M4_DMPROP_PLUS_INF ;
			}
			else
			{
				iInteger = M4_DMPROP_DATE ;
			}

			break ;

		case M4CL_CPP_TYPE_STRING_VAR :

			if( m_vValue.Data.PointerChar == NULL )
			{
				iInteger = M4_DMPROP_NULL_STRING ;
			}
			else
			{
				iInteger = M4_DMPROP_STRING ;
			}
			break ;

		case M4CL_CPP_TYPE_NULL :
		default :
				iInteger = M4_DMPROP_NULL ;
			break ;
	}


	// Ponemos el tipo de  contenido
	iResult = IOD.Write( m4uint8_t( iInteger ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	switch( iInteger )
	{
		case M4_DMPROP_NUMBER :
		case M4_DMPROP_DATE :

			iResult = IOD.Write( m_vValue.Data.DoubleData ) ;
			break ;

		case M4_DMPROP_STRING :

			iResult = m_vValue.Serialize( IOD ) ;
			break ;

		default :
			break ;
	}

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClGenericProperty::_DeSerialize( ClGenericIODriver& IOD )
{

	m4return_t		iResult ;
	m4uint8_t		iInteger ;
	m4VariantType	vValue ;


	iResult = IOD.Read( iInteger ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	switch( iInteger )
	{
		case M4_DMPROP_STRING :

			/* Bug 0102652
			Hay que asignar el tipo del variant, no sólo el valor
			*/
			vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
			iResult = vValue.DeSerialize( IOD ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			break ;

		case M4_DMPROP_NULL_STRING :

			vValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
			vValue.Data.PointerChar = NULL ;
			break ;

		case M4_DMPROP_NUMBER :

			/* Bug 0102652
			Hay que asignar el tipo del variant, no sólo el valor
			*/
			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			iResult = IOD.Read( vValue.Data.DoubleData ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			break ;

		case M4_DMPROP_DATE :

			/* Bug 0102652
			Hay que asignar el tipo del variant, no sólo el valor
			*/
			vValue.Type = M4CL_CPP_TYPE_DATE ;
			iResult = IOD.Read( vValue.Data.DoubleData ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			break ;

		case M4_DMPROP_MINUS_INF :

			vValue.Type = M4CL_CPP_TYPE_DATE ;
			vValue.Data.DoubleData = M4CL_MINUS_INFINITE_IN_JULIAN ;
			break ;

		case M4_DMPROP_PLUS_INF :

			vValue.Type = M4CL_CPP_TYPE_DATE ;
			vValue.Data.DoubleData = M4CL_PLUS_INFINITE_IN_JULIAN ;
			break ;

		case M4_DMPROP_MINUS_ONE :

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = -1.0 ;
			break ;

		case M4_DMPROP_CERO :

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = 0.0 ;
			break ;

		case M4_DMPROP_ONE :

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = 1.0 ;
			break ;

		case M4_DMPROP_TWO :

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = 2.0 ;
			break ;

		case M4_DMPROP_THREE :

			vValue.Type = M4CL_CPP_TYPE_NUMBER ;
			vValue.Data.DoubleData = 3.0 ;
			break ;

		case M4CL_CPP_TYPE_NULL :
		default :
			vValue.Type = M4CL_CPP_TYPE_NULL ;
		break ;
	}

	return( _Set( vValue, M4_FALSE ) ) ;
}
