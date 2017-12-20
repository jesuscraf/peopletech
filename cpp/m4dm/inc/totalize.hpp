//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         CVM - DM
// File:           totalize.hpp
// Project:        CVM
// Author:         Meta Software M.S. , S.A
// Date:            27 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//   
////
//==============================================================================

#ifndef __TOTALIZE_HPP__
#define __TOTALIZE_HPP__
#include "m4dm_dll.hpp"
#include "m4var.hpp"
#include "index.hpp"

class _ClRegister_Base;
class _ClValue_Read_Slices;
class ClRgBoundedAc;
class ClRgAc_Base;
class ClRgAc;
class ClVectorVariants;

//GetValue para distintas instancias de totalizadores.
class M4_DECL_M4DM ClGetValue{
public:

	// Totalizador de registro. Totaliza el valor de un item en todos los registros de un bloque.
	m4return_t operator()(_ClRegister_Base * ai_poReg, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue );
	// Totalizador de items. Totaliza el valor de los tramos de un item.
	m4return_t operator()(_ClValue_Read_Slices * ai_poSlices, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue );
	// Totalizador para TIs de acumulado(I). Totaliza varios items en los registros de un bloque que cumplan una condicion determinada.
	m4return_t operator()(ClRgAc_Base * ai_poAcumTot, ClVectorVariants &ao_vTotVal, m4bool_t ai_bWeightedValue );
	// Totalizador para TIs de acumulado(II). Totaliza varios items tomando como valores las entradas de un totalizador del tipo anterior.
	m4return_t operator()(ClRgAc *pRegAcumTot,ClVectorVariants &ao_vTotVal, m4bool_t ai_bWeightedValue );
	// Totalizador para TIs de acumulado(III). Totaliza varios items en un rango de registros que cumplen una condicion determinada( se aplica como iterador para el I)
	m4return_t operator()(ClRgBoundedAc *pRegAcumTot, m4VariantType &ao_vTotVal, m4bool_t ai_bWeightedValue );
};

template <class T, class S, class GetValueT /*= ClGetValue*/ >
class ClTotalize{

public:
	void Init( T * ai_tIterator )
	{
		m_tIterator = ai_tIterator;
	}

	m4return_t Get( S &ao_vTotValOut, m4char_t TotType = -1 );
	m4return_t GetNoConcat( S &ao_vTotValOut, m4char_t TotType = -1 );


	//Pasamos del count por problemas de tipos
	m4return_t Count( S &ao_vTotValOut );
	m4return_t Sum( S &ao_vTotValOut, m4bool_t ai_bConcatStrings );
	m4return_t Avg( S &ao_vTotValOut );
	m4return_t Max( S &ao_vTotValOut );
	m4return_t Min( S &ao_vTotValOut );	
	m4return_t First( S &ao_vTotValOut );
	m4return_t Last( S &ao_vTotValOut );
	m4return_t WeightedSum( S &ao_vTotValOut);

	m4return_t GetSize(ClSize &ao_size);

protected:

	m4return_t _Get( S &ao_vTotValOut, m4char_t TotType, m4bool_t ai_bConcatStrings );

	T * m_tIterator;
	GetValueT	GetValue;	//dedicado a Alex. Deberia poner m_GetValue

};



template <class T,class S,class GetValueT>
m4return_t ClTotalize<T,S,GetValueT>::Get( S &ao_vTotValOut, m4char_t TotType )
{
	m4return_t	iResult = _Get( ao_vTotValOut, TotType, M4_TRUE ) ;
	return( iResult ) ;
}


template <class T,class S,class GetValueT>
m4return_t ClTotalize<T,S,GetValueT>::GetNoConcat( S &ao_vTotValOut, m4char_t TotType )
{
	m4return_t	iResult = _Get( ao_vTotValOut, TotType, M4_FALSE ) ;
	return( iResult ) ;
}


template <class T,class S,class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::Count( S &ao_vTotValOut ) 
{
	ao_vTotValOut.SetDouble( m_tIterator->Count() );
	return M4_SUCCESS;			
}


template <class T, class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::Sum( S &ao_vTotValOut, m4bool_t ai_bConcatStrings ) 
{
	S vTempVal;
	m4return_t ret;
	
	m4uint32_t iIt = m_tIterator->Index;

	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	//En el pie devuelve el Value.footer
	//En el value devuelve el value
	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	while( m_tIterator->Next() )
	{
		ret = GetValue(m_tIterator, vTempVal, M4_FALSE);
		if( ret == M4_ERROR ) 
		{
			m_tIterator->MoveTo( iIt );
			return M4_ERROR;
		}

		ao_vTotValOut.Sum( vTempVal, ai_bConcatStrings );

		vTempVal.FreeData();
	}

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			

}


template <class T, class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::Avg( S &ao_vTotValOut ) 
{
	m4VariantType  vCont;
	S vTempVal;
	m4return_t ret;

	m4uint32_t iIt = m_tIterator->Index;
	
	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	vCont.Type = M4CL_CPP_TYPE_NUMBER;
	vCont.Data.DoubleData = 1;

	while( m_tIterator->Next() )
	{
		ret = GetValue(m_tIterator, vTempVal, M4_FALSE);

		if( ret == M4_ERROR ) 
		{
			m_tIterator->MoveTo( iIt );
			return M4_ERROR;
		}

		/* Bug 0105025
		Se deja a que si concatene para no crear impacto
		*/
		ao_vTotValOut.Sum( vTempVal, M4_TRUE ) ;

		vTempVal.FreeData();

		vCont.Data.DoubleData++;
	}

	ao_vTotValOut.Div( vCont );

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			

}


template <class T,class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::Max( S &ao_vTotValOut ) 
{
	S vTempVal;
	m4return_t ret;

	m4uint32_t iIt = m_tIterator->Index;
	
	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	while( m_tIterator->Next() )
	{
		ret = GetValue(m_tIterator, vTempVal, M4_FALSE);
		if( ret == M4_ERROR ) 
		{
			m_tIterator->MoveTo( iIt );
			return M4_ERROR;
		}

		ao_vTotValOut.Max( vTempVal );

		vTempVal.FreeData();
	}

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			

}


template <class T,class S, class GetValueT >
m4return_t ClTotalize <T,S,GetValueT> :: Min( S &ao_vTotValOut ) 
{
	S vTempVal;
	m4return_t ret;

	m4uint32_t iIt = m_tIterator->Index;
	
	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	while( m_tIterator->Next() )
	{
		ret = GetValue(m_tIterator, vTempVal, M4_FALSE);

		if( ret == M4_ERROR ) 
		{
			m_tIterator->MoveTo( iIt );
			return M4_ERROR;
		}

		ao_vTotValOut.Min( vTempVal );

		vTempVal.FreeData();
	}

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			

}


template <class T, class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::First( S &ao_vTotValOut ) 
{
	m4return_t ret;

	m4uint32_t iIt = m_tIterator->Index;

	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			

}


template <class T,class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::Last( S &ao_vTotValOut ) 
{
	m4return_t ret;

	m4uint32_t iIt = m_tIterator->Index;

	if(! m_tIterator->End() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	ret = GetValue(m_tIterator, ao_vTotValOut, M4_FALSE);
	
	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	m_tIterator->MoveTo( iIt );

	return M4_SUCCESS;			
}


template <class T, class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::WeightedSum( S &ao_vTotValOut ) 
{
	
	S vTempVal;
	m4return_t ret;
	
	
	m4uint32_t iIt = m_tIterator->Index;

	if(! m_tIterator->Begin() )
	{
		ao_vTotValOut.SetNull();
		return M4_SUCCESS;
	}

	ao_vTotValOut.FreeData();

	//En el pie devuelve el Value.footer
	//En el value devuelve el value
	ret = GetValue(m_tIterator, ao_vTotValOut, M4_TRUE);

	if( ret == M4_ERROR ) 
	{
		m_tIterator->MoveTo( iIt );
		return M4_ERROR;
	}

	while( m_tIterator->Next() )
	{
		ret = GetValue(m_tIterator, vTempVal, M4_TRUE);
		if( ret == M4_ERROR ) 
		{
			m_tIterator->MoveTo( iIt );
			return M4_ERROR;
		}

		ao_vTotValOut.Sum( vTempVal, M4_FALSE );

		vTempVal.FreeData();
	}

	m_tIterator->MoveTo( iIt );
	
	return M4_SUCCESS;			

}

template <class T,class S, class GetValueT >
m4return_t ClTotalize<T,S,GetValueT>::GetSize( ClSize &ao_size )
{
	ao_size+=sizeof(ClTotalize<T,S,GetValueT>);
	return M4_SUCCESS;
}




template <class T,class S,class GetValueT>
m4return_t ClTotalize<T,S,GetValueT>::_Get( S &ao_vTotValOut, m4char_t TotType, m4bool_t ai_bConcatStrings )
{

	if( TotType == -1 ) TotType = m_tIterator->GetTotType();

	switch( TotType )
	{
		case M4CL_ITEM_TOTALIZE_COUNT:
			return Count( ao_vTotValOut );
			break;

		case M4CL_ITEM_TOTALIZE_SUM:
			return Sum( ao_vTotValOut, ai_bConcatStrings );
			break;

		case M4CL_ITEM_TOTALIZE_AVG:
			return Avg( ao_vTotValOut );
			break;

		case M4CL_ITEM_TOTALIZE_MAX:
			return Max( ao_vTotValOut );
			break;

		case M4CL_ITEM_TOTALIZE_MIN:
			return Min( ao_vTotValOut );
			break;

		case M4CL_ITEM_TOTALIZE_FIRST:
			return First( ao_vTotValOut );
			break;

		case M4CL_ITEM_TOTALIZE_LAST:
			return Last( ao_vTotValOut );
			break;
		
		case M4CL_ITEM_TOTALIZE_WEIGHTED_SUM:
			return WeightedSum( ao_vTotValOut );
			break;

		default:
			return First( ao_vTotValOut );
			break;
	}
}


#endif

