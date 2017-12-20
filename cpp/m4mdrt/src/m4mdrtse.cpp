
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrtse.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                03-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este modulo define las funciones de serialización del metacanal compilado
//    
//
//
//==============================================================================

#include "m4stl.hpp"
#include "m4mdrt.hpp"
#include "m4mdrtre.hpp"

#include "m4buildversion.hpp"



#define M4_CH_MAGIC_NUMBER_CMCR							0x6E4303A1
#define M4_CH_MAGIC_NUMBER_CSCR							0xD470DD47




// Funciones de swaping ===================================================


void	SwapInt16( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;
}


void	SwapInt32( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;
}


void	SwapDate( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 7 ) ;
	*( pcNumber + 7 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 5 ) ;
	*( pcNumber + 5 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;
}


void	SwapDouble( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 7 ) ;
	*( pcNumber + 7 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 5 ) ;
	*( pcNumber + 5 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;
}


m4uint16_t	SwapInt16( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	iResult ;
	m4char_t	c ;
	m4pchar_t	pcNumber ;


	if( ai_iReverse == 0 )
	{
		iResult = M4ClUnPackInt16( ai_pcBuffer, ai_iOffset ) ;
	}

	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;

	if( ai_iReverse == 1 )
	{
		iResult = M4ClUnPackInt16( ai_pcBuffer, ai_iOffset ) ;
	}

	return( iResult ) ;
}


m4uint32_t	SwapInt32( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint32_t	iResult ;
	m4char_t	c ;
	m4pchar_t	pcNumber ;


	if( ai_iReverse == 0 )
	{
		iResult = M4ClUnPackInt32( ai_pcBuffer, ai_iOffset ) ;
	}

	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;

	if( ai_iReverse == 1 )
	{
		iResult = M4ClUnPackInt32( ai_pcBuffer, ai_iOffset ) ;
	}

	return( iResult ) ;
}




void	SwapSentence( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_GROUP_OBJECTS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_APISQL_1 ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_APISQL_2 ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_APISQL_3 ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_APISQL_4 ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_SENTENCE_MAX_NUM_COL ) ;
}


void	SwapInstance( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_T3_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_ROLE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_ORGANIZATION ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_SCOPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_OPEN_MODE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_CS_ACCESS ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_USE_ROLE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_INSTANCE_SOCIETY_TYPE ) ;
}


void	SwapBusinessMethod( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;

	
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_BUSINESS_METHOD_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_BUSINESS_METHOD_NAME ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_BUSINESS_METHOD_DESCRIPTION ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_BUSINESS_METHOD_ITEM_HANDLE ) ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_BUSINESS_METHOD_ARGUMENT_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_BUSINESS_METHOD_ARGUMENTS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_BUSINESS_ARG_NODE_ID ) ;
//		SwapInt8 ( ai_pcBuffer, iElement + M4CL_MCR_BUSINESS_ARG_TYPE ) ;
		i++ ;
	}
}


void	SwapItemConnector( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_NODE_CONNECTOR ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_FATHER_NITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_SON_NITEM ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_PRECEDENCE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_SPIN ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_RELATION ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_CONTEXT ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CONNECTOR_CS_TYPE ) ;
}


void	SwapInstruction( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse, m4uint32_t ai_iSize )
{

	m4uint16_t	iFunction ;


	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_FLAGS ) ;
	iFunction = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_EXEFUNC, ai_iReverse ) ;

    switch( ai_iSize )
    {

		case	M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT :
			SwapDouble( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_VARIANT ) ;
			break;

		case    M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT_AS_INT:

			SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_VARIANT_AS_INT ) ;

			break ;

		case	M4CL_MCR_CODE_INSTRUCTION_SIZE_ITEM :

			SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_ITEMVAR ) ;
			SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_RULE ) ;
			SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_NODE ) ;
			SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_CHANNEL ) ;
//			SwapInt8  ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_ATTR ) ;
			break ;

    }
}


void	SwapFormula( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint32_t	i, j ;
	m4uint32_t	iInstruction ;
	m4uint32_t	iLevelAndInstructions ;
	m4uint32_t	iInstructions ;
	m4uint32_t	iNextInstruction ;
	m4uint32_t	iFormulaSize ;
	m4uint32_t	iOffset ;
	m4uint32_t	iElement ;
	m4uint32_t	iGap ;
	m4uint32_t	iSize ;
	m4uint32_t  iDateInt ;
	m4uint8_t   iDateByte ;


	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_VARIABLE_NUMBER ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_PRIOR_ASSIGN ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_TYPE ) ;

	iLevelAndInstructions = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_NUMBER_INSTRUCTIONS, ai_iReverse ) ;
	iInstructions = M4CL_APPLY_MASK( iLevelAndInstructions, M4CL_MCR_CODE_INSTRUCTION_NUMBER_MASK ) ;
	iFormulaSize = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_FORMULA_SIZE, ai_iReverse ) ;

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CODE_RULE_TI ) ;


	iDateByte = *( ai_pcBuffer + ai_iOffset + iFormulaSize - sizeof( m4uint8_t ) ) ;

	iDateInt = 0 ;
	if( M4CL_MCR_INS_HAS_END_DAYS_AND_SECONDS( iDateByte ) )
	{
		iDateInt++ ;
	}
	if( M4CL_MCR_INS_HAS_END_DAYS( iDateByte ) )
	{
		iDateInt++ ;
	}
	if( M4CL_MCR_INS_HAS_START_DAYS_AND_SECONDS( iDateByte ) )
	{
		iDateInt++ ;
	}
	if( M4CL_MCR_INS_HAS_START_DAYS( iDateByte ) )
	{
		iDateInt++ ;
	}

	iNextInstruction = ai_iOffset + iFormulaSize - sizeof( m4uint8_t ) - iDateInt * sizeof( m4uint32_t ) ; //tamaño - byte de fechas - tantos enteros como necesitemos para guardar las fechas
	iOffset = ai_iOffset + M4CL_MCR_CODE_INSTRUCTION_START ;

    for( i = 0 ; i < iInstructions ; i++ )
	{
		j = ( iInstructions - 1 - i ) ;

		iElement = iOffset + ( j * M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE ) ;
		iGap = SwapInt32( ai_pcBuffer, iElement, ai_iReverse ) ;

		if( !M4CL_MCR_INS_EMB_IS_EMBEBBED( iGap ) )
		{
			iInstruction = iElement + M4CL_MCR_INS_EMB_OFFSET( iGap ) ;
			iSize = iNextInstruction - iInstruction ;
			iNextInstruction = iInstruction ;

			SwapInstruction( ai_pcBuffer, iInstruction, ai_iReverse, iSize ) ;
		}
	}

	for( i = 1 ; i <= iDateInt ; i++ )
	{
		SwapInt32( ai_pcBuffer, ai_iOffset + iFormulaSize -  sizeof( m4uint8_t ) - i * sizeof( m4uint32_t ) ) ;
	}
}


void	SwapRitem( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i, j ;
	m4uint16_t	iLength ;
	m4uint16_t	iFormulas ;
	m4uint32_t	iOffset ;
	m4uint32_t	iElement ;
	m4uint32_t	iHandle ;

	set< m4uint32_t, less< m4uint32_t > >	setcode ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_NAME ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_INVERSE_CONNECTOR ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_SLICE_NITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_AUX_NITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_AUX_NITEM_PRORAT ) ;


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_CONDITIONANTS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_RITEM_CONDITIONANT_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_RITEM_CONDITIONANTS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_RITEM_CONDITIONANT_NITEM ) ;

			iElement += M4CL_MCR_RITEM_CONDITIONANT_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_ASSIGNED_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_RITEM_ASSIGNED_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_RITEM_ASSIGNED ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_RITEM_ASSIGNED_NITEM ) ;

			iElement += M4CL_MCR_RITEM_ASSIGNED_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_EXECUTED_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_RITEM_EXECUTED_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_RITEM_EXECUTED ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_RITEM_EXECUTED_NITEM ) ;

			iElement += M4CL_MCR_RITEM_EXECUTED_SIZE ;
			i++ ;
		}
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_FORM_IDS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_RITEM_FORM_IDS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_RITEM_FORM_IDS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_RITEM_FORM_IDS_HANDLE ) ;

			iElement += M4CL_MCR_RITEM_FORM_IDS_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_FORMULAS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_FORMULA_SLICE_NUMBER, ai_iReverse ) ;
		iFormulas = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_FORMULA_FORMULA_NUMBER, ai_iReverse ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iElement = iOffset + M4CL_MCR_FORMULA_FORMULA_START + ( i * M4_ROUND_TO_8( ( 2 * M4_SIZE_OF_DATE ) + ( iFormulas * M4_SIZE_OF_INT32 ) ) ) ;

			SwapDate ( ai_pcBuffer, iElement + M4CL_MCR_FORMULA_START_DATE ) ;
			SwapDate ( ai_pcBuffer, iElement + M4CL_MCR_FORMULA_END_DATE ) ;

			for( j = 0 ; j < iFormulas ; j++ )
			{
				iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_FORMULA_CODE_START + j * M4_SIZE_OF_INT32, ai_iReverse ) ;

				if( ( iHandle != 0 ) &&( setcode.find( iHandle ) == setcode.end() ) )
				{
					SwapFormula( ai_pcBuffer, iHandle, ai_iReverse ) ;
					setcode.insert( iHandle ) ;
				}
			}
		}
	}


//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_SCOPE ) ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_RITEM_CONNECTED_ITEM_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_RITEM_CONNECTED_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		iHandle = SwapInt32( ai_pcBuffer, iElement, ai_iReverse ) ;
		SwapItemConnector( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4_SIZE_OF_INT32 ;
		i++ ;
	}

}


void	SwapItem( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint8_t	iFlag ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;


	iFlag = M4ClUnPackInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DEFAULT_VALUE_FLAG ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_ITEMS_START ) ;

	if( iFlag == 2 )
	{
		SwapInt32 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DEFAULT_VALUE ) ;
	}
	else
	{
		SwapDouble( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DEFAULT_VALUE ) ;
	}


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_TI_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DMD ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DMD_COMPONENT ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DMD_CROSSTAB ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DMD_FIELD ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DB_PRECISION ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_PRECISION ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_MAX_SIZE ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_READ_OBJECT_ALIAS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_WRITE_OBJECT_ALIAS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_READ_FIELD_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_WRITE_FIELD_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_READ_OBJECT_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_WRITE_OBJECT_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SYNONYM_ID ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_ORDER ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_FLAGS ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SCOPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_M4_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_CPP_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SCALE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SLICE_BHRV ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SLICE_SPLIT ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_TOTALIZE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SLICE_TOT_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_TRANSACTION_MODE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_METHOD_LEVEL ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_SENT_TOT_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_DEFAULT_VALUE_FLAG ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_INTERNAL_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_CONVERT_FUNCTION ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_MODIFY_DATA ) ;

	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_ARGUMENT_NUMBER, ai_iReverse ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_ITEM_ARGUMENT_REFERENCES ) ;

	iElement = ai_iOffset + M4CL_MCR_ITEM_ARGUMENTS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_ARGUMENT_ID ) ;
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_ARGUMENT_NAME ) ;
		SwapInt16( ai_pcBuffer, iElement + M4CL_MCR_ARGUMENT_PREC ) ;
//		SwapInt8 ( ai_pcBuffer, iElement + M4CL_MCR_ARGUMENT_TYPE ) ;
//		SwapInt8 ( ai_pcBuffer, iElement + M4CL_MCR_ARGUMENT_M4_TYPE ) ;

		iElement += M4CL_MCR_ITEM_ARGUMENT_SIZE ;
		i++ ;
	}

}


void	SwapNitem( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint8_t	iIsMyRitem ;
	m4uint32_t	iHandle ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_ITEM ) ;
	iHandle = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_RITEM, ai_iReverse ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_NODE ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_SCR_START ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_INDEX ) ;

//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_FLAGS ) ;
	iIsMyRitem = M4ClUnPackInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_FLAGS ) ;

	if( ( iIsMyRitem & M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) != 0 )
	{
		SwapRitem( ai_pcBuffer, iHandle, ai_iReverse ) ;
	}

//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_CS_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_SYNC_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NITEM_LEVEL ) ;
}


void	SwapTi( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint16_t ai_iInheritLevel, m4uint8_t ai_iReverse )
{

	m4uint8_t	iOwnSentence ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;
	m4uint32_t	iHandle ;
	m4uint32_t	iReadSentence ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_ID_TI ) ;
	iReadSentence = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_READ_SENTENCE, ai_iReverse ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_WRITE_SENTENCE ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_READ_OBJECT_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_WRITE_OBJECT_ID ) ;

	iElement = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_REFERENCES_START, ai_iReverse ) ;

	i = 0 ;
	while( i < ai_iInheritLevel )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_REFERENCE_NODE ) ;
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_REFERENCE_NUMBER ) ;

		iElement += M4CL_MCR_REFERENCE_SIZE ;
		i++ ;
	}


//	SwapInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_TEMPORARY_ID ) ;
//	SwapInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_COMPLETE ) ;
//	SwapInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_CORRECTED ) ;
//	SwapInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_OWN_SENTENCE ) ;
	iOwnSentence = M4ClUnPackInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_OWN_SENTENCE ) ;

	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_TI_ITEM_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_TI_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_TI_ITEM_HANDLE, ai_iReverse ) ;
		SwapItem( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_TI_ITEM_SIZE ;
		i++ ;
	}

	if( iOwnSentence == 1 && iReadSentence != 0 )
	{
		SwapSentence( ai_pcBuffer, iReadSentence, ai_iReverse ) ;
	}
}


void	SwapNodeConnector( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_FATHER_NODE ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_SON_NODE ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_SENTENCE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_EXPAND_DELETE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTOR_EXPAND_UNDO ) ;
}


void	SwapGroup( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_GROUP_ITEM_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_GROUP_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement, ai_iReverse ) ;

		iElement += M4_SIZE_OF_INT32 ;
		i++ ;
	}
}


void	SwapPlug( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;
	m4uint32_t	iHandle ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_FATHER_CHANNEL_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_FATHER_CHANNEL_NAME ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_FATHER_NODE_ID ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_SCR_START ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_ID_RELATION_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_IS_MULTISELECTION ) ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_PLUG_ITEMS_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_PLUG_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		iHandle = SwapInt32( ai_pcBuffer, iElement, ai_iReverse ) ;

		SwapInt32( ai_pcBuffer, iHandle + M4CL_MCR_PLUG_FATHER_ITEM_ID ) ;
		SwapInt32( ai_pcBuffer, iHandle + M4CL_MCR_PLUG_SON_ITEM_HANDLE ) ;
//		SwapInt8 ( ai_pcBuffer, iHandle + M4CL_MCR_PLUG_RELATION ) ;

		iElement += M4_SIZE_OF_INT32 ;
		i++ ;
	}

}


void	SwapIndex( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_INDEX_ID ) ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_INDEX_ITEM_NUMBER, ai_iReverse ) ;

	iElement = ai_iOffset + M4CL_MCR_INDEX_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_INDEX_ITEM_ITEM_HANDLE ) ;
//		SwapInt8 ( ai_pcBuffer, iElement + M4CL_MCR_INDEX_ITEM_FUNCTION_ID ) ;
//		SwapInt8 ( ai_pcBuffer, iElement + M4CL_MCR_INDEX_ITEM_WAY ) ;

		iElement += M4CL_MCR_INDEX_ITEM_SIZE ;
		i++ ;
	}
}


void	SwapFilter( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_T3_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_NODE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_SYS_SENTENCE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_SYS_PARAM_ITEM ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_SCR_START ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_BEHAVIOUR_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_FILTER_IS_OWN_NODE ) ;
}


void	SwapNode( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint16_t	iAuto ;
	m4uint32_t	iOffset ;
	m4uint32_t	iElement ;
	m4uint32_t	iHandle ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_TI ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_M4OBJ_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NAME ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_TI_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_GROUP_OBJECTS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_START_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_END_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_START_COR_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_END_COR_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_VIRTUAL_FLAG_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_FILTER_START_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_FILTER_END_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_FILTER_START_COR_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_FILTER_END_COR_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CURRENCY_TYPE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CURRENCY_EXCH_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CHANGE_REASON_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DMD_COMPONENT_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DMD_VALUE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_LOAD_SQL_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_PRIORITY_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IMPUTE_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_PAY_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_PAY_TYPE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_PAY_FREQUENCY_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IMPUTE_PAY_TYPE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IMPUTE_PAY_FREQUENCY_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CLOSING_DATE_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_SYS_PARAMS_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_BDL_CHECKS_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_RAM_ORDER_BY_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_ID_ORGANIZATION ) ;

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_LOAD_BLK_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DELETE_BLK_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_UPDATE_BLK_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_INSERT_BLK_ITEM ) ;

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DEFAULT_INDEX ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NUM_ROWS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NUM_KEEP_ROWS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NUM_ROWS_DB ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_OLE_DISP_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_OLE_NODE_INTER_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_MAX_NUM_BLOCKS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_MAX_NUM_RECORDS ) ;


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CONNECTORS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_CONNECTOR_NUMBER, ai_iReverse ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_RB_CONNECTOR_NUMBER ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_BB_CONNECTOR_NUMBER ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_NR_CONNECTOR_NUMBER ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_RB_FIRST_CONNECTOR ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_BB_FIRST_CONNECTOR ) ;
		SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_NR_FIRST_CONNECTOR ) ;

		iAuto = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_AUTO_CONNECTOR_NUMBER, ai_iReverse ) ;

		iLength += iAuto ;

		iElement = iOffset + M4CL_MCR_NODE_CONNECTORS ;
		i = 0 ;
		while( i < iLength )
		{
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_CONNECTOR_HANDLE, ai_iReverse ) ;
			SwapNodeConnector( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4_SIZE_OF_INT32 ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_INV_CONNECTORS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_INV_CONNECTOR_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_INV_CONNECTORS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_INV_CONNECTOR_HANDLE ) ;

			iElement += M4_SIZE_OF_INT32 ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_GROUPS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_GROUP_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_GROUPS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_GROUP_ID ) ;
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_GROUP_HANDLE, ai_iReverse ) ;

			SwapGroup( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4CL_MCR_NODE_GROUP_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_PLUGS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_PLUG_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_PLUGS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_PLUG_CHANNEL_ID ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_PLUG_NODE_ID ) ;
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_PLUG_HANDLE, ai_iReverse ) ;

			SwapPlug( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4CL_MCR_NODE_PLUG_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_INDEXES_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_INDEX_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_INDEXES ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_INDEX_ID ) ;
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_INDEX_HANDLE, ai_iReverse ) ;

			SwapIndex( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4CL_MCR_NODE_INDEX_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_ALIAS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_ALIAS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_ALIAS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_ALIAS_ID ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_ALIAS_NODE_HANDLE ) ;

			iElement += M4CL_MCR_NODE_ALIAS_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_T3_ALIAS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_T3_ALIAS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_T3_ALIAS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_T3_ALIAS_ID ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID ) ;

			iElement += M4CL_MCR_NODE_T3_ALIAS_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DMDS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_DMDS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_DMDS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_DMDS_DMD_COMPONENT ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_DMDS_DMD_FIELD ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_DMDS_DMD ) ;
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_DMDS_HANDLE ) ;

			iElement += M4CL_MCR_NODE_DMD_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_QBF_FILTERS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_FILTERS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_FILTERS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_FILTER_ID ) ;
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_FILTER_HANDLE, ai_iReverse ) ;

			SwapFilter( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4CL_MCR_NODE_FILTER_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_FIND_FILTERS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_FILTERS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_FILTERS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_FILTER_ID ) ;
			iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_FILTER_HANDLE, ai_iReverse ) ;

			SwapFilter( ai_pcBuffer, iHandle, ai_iReverse ) ;

			iElement += M4CL_MCR_NODE_FILTER_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_SYS_SENTENCES_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_SYS_SENTENCES_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_SYS_SENTENCES ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_SYS_SENTENCE_HANDLE ) ;

			iElement += M4CL_MCR_NODE_SYS_SENTENCE_SIZE ;
			i++ ;
		}
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_SYS_HINTS_START, ai_iReverse ) ;

	if( iOffset != 0 )
	{
		iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_NODE_SYS_HINTS_NUMBER, ai_iReverse ) ;

		iElement = iOffset + M4CL_MCR_NODE_SYS_HINTS ;
		i = 0 ;
		while( i < iLength )
		{
			SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_SYS_HINT_HANDLE ) ;

			iElement += M4CL_MCR_NODE_SYS_HINT_SIZE ;
			i++ ;
		}
	}


	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_SCR_START ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_INDEX ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_TYPE ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_ORDER ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CAPACITY ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_ROOT ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_REAL_ROOT ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_AUTOLOAD ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_ORDERED ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_VISIBLE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DYN_FILTER ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_ID_AUTO_FILTER ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_CS_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_SYNC_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_EXTERNAL ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_QBF_FILTER ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_AFFECTS_DB ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_DB_RELOAD ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_TI_INHERIT_LEVEL ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_IS_OVERWRITTEN ) ;


	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NITEM_NUMBER, ai_iReverse ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_NODE_NITEM_NUMBER ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_BLOCK_NITEM_NUMBER ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_NODE_REGISTER_NITEM_NUMBER ) ;

	iElement = ai_iOffset + M4CL_MCR_NODE_NITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_NITEM_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_NODE_NITEM_HANDLE, ai_iReverse ) ;

		SwapNitem( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_NODE_NITEM_SIZE ;
		i++ ;
	}
}




void	SwapChannel( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint16_t	iInherits ;
	m4uint32_t	iOffset ;
	m4uint32_t	iElement ;
	m4uint32_t	iHandle ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SIZE ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_BUILD ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_TIME_UNIT ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CS_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_LANGUAGE ) ;

	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_START_DATE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_END_DATE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_COR_START_DATE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_COR_END_DATE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_VERSION ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CREATION_DATE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CACHE_MAX_PERIOD ) ;

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_NAME ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_LICENSE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_POOL ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_OLE_CLASS_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SERVICE_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SERVICE_ALT_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_ENVIRONMENT_VARS ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_OWNER_FLAG ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_LOAD_ITEM ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_PERSIST_ITEM ) ;


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_ROOTS_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_ROOT_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_ROOTS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_ROOT_ID ) ;
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_ROOT_HANDLE ) ;

		iElement += M4CL_MCR_CHANNEL_ROOT_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SENTENCES_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_SENTENCE_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_SENTENCES ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_SENTENCE_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_SENTENCE_HANDLE, ai_iReverse ) ;

		SwapSentence( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_SENTENCE_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_INSTANCES_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_INSTANCE_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_INSTANCES ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_INSTANCE_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_INSTANCE_HANDLE, ai_iReverse ) ;

		SwapInstance( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_INSTANCE_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_ITEMS_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_ITEMS_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_ITEMS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_ITEM_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_ITEM_HANDLE, ai_iReverse ) ;

		SwapNitem( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_ITEM_SIZE ;
		i++ ;
	}


	iInherits = M4ClUnPackInt8( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_INHERIT_LEVEL ) ;

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_TIS_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_TIS_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_TIS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_TI_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_TI_HANDLE, ai_iReverse ) ;
		SwapTi( ai_pcBuffer, iHandle, iInherits, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_TI_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_PARAMETERS_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_PARAMETERS_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_PARAMETERS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_PARAMETER_HANDLE ) ;

		iElement += M4CL_MCR_CHANNEL_PARAMETER_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_RSMS_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_RSMS_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_RSMS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_RSMS_ID ) ;
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_RSMS_MASK_ID ) ;

		iElement += M4CL_MCR_CHANNEL_RSMS_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_BUSINESS_METHOD_START, ai_iReverse ) ;
	iLength = SwapInt16( ai_pcBuffer, iOffset + M4CL_MCR_CHANNEL_BUSINESS_METHOD_NUMBER, ai_iReverse ) ;

	iElement = iOffset + M4CL_MCR_CHANNEL_BUSINESS_METHODS ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_BUSINESS_METHOD_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_BUSINESS_METHOD_HANDLE, ai_iReverse ) ;

		SwapBusinessMethod( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE ;
		i++ ;
	}


	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_TI_SYSTEM, ai_iReverse ) ;

	// Puede que no haya Ti del sistema
	if( iOffset != 0 )
	{
		SwapTi( ai_pcBuffer, iOffset, 0, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_LOAD_CLIENT, ai_iReverse ) ;

	// Puede que no haya SysLoad
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_PERSIST_CLIENT, ai_iReverse ) ;

	// Puede que no haya SysPersist
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_LOAD_SERVER, ai_iReverse ) ;

	// Puede que no haya SysLoad
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SYS_PERSIST_SERVER, ai_iReverse ) ;

	// Puede que no haya SysPersist
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_EXECUTEREPORT, ai_iReverse ) ;

	// Puede que no haya ExecuteReport
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	iOffset = SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_DYN_FILTER, ai_iReverse ) ;

	// Puede que no haya Dyn Filter
	if( iOffset != 0 )
	{
		SwapItem( ai_pcBuffer, iOffset, ai_iReverse ) ;
	}

	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_SECURITY ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_VM_CAPACITY_REQ ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_IS_SEPARABLE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CS_EXE_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_IS_EXTERNAL ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_HAS_SECURITY ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_IS_CACHEABLE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_IS_MD_CACHEABLE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CHECK_CONCURRENCY ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_ORGANIZATION_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_INHERIT_LEVEL ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_DATA_LANGUAGE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_META_LANGUAGE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_CREATION_TYPE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_STYLEUSE ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_WSS_SECURITY ) ;

	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_MCR_CHANNEL_NODE_NUMBER, ai_iReverse ) ;


	iElement = ai_iOffset + M4CL_MCR_CHANNEL_NODES ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_NODE_ID ) ;
		iHandle = SwapInt32( ai_pcBuffer, iElement + M4CL_MCR_CHANNEL_NODE_HANDLE, ai_iReverse ) ;

		SwapNode( ai_pcBuffer, iHandle, ai_iReverse ) ;

		iElement += M4CL_MCR_CHANNEL_NODE_SIZE ;
		i++ ;
	}
}


void	SwapSecurity( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset, m4uint8_t ai_iReverse )
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iElement ;


	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_SIZE ) ;
	SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_BUILD ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_CS_TYPE ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_T3_VERSION ) ;
	SwapDate ( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_OWN_VERSION ) ;

	iLength = SwapInt16( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_NODE_NUMBER, ai_iReverse ) ;

	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_MASK_ID ) ;
	SwapInt32( ai_pcBuffer, ai_iOffset + M4CL_SCR_SECURITY_T3_ID ) ;
//	SwapInt8 ( ai_pcBuffer, ai_iOffset + M4CL_SCR_T3_MASK_START ) ;


	iElement = ai_iOffset + M4CL_SCR_SECURITY_FIXED_SIZE ;
	i = 0 ;
	while( i < iLength )
	{
		SwapInt32( ai_pcBuffer, iElement + M4CL_SCR_NODE_SEC_FILTER ) ;

		iElement += M4CL_SCR_NODE_MASK_SIZE ;
		i++ ;
	}
}








//=============================================================================
// Funciones de serialización
//=============================================================================


// clase wrapper del chunk de metadatos =======================================

m4return_t  ClCMCRWrapper :: Serialize( ClGenericIODriver &IOD, m4bool_t ai_bConvert )
{

	m4return_t	iResult ;
	m4uint8_t	*pcAux ;
	m4bool_t	bConvert ;
	m4uint32_t	iSize ;
	m4uint32_t	iDoubleSize ;
    m4pchar_t	pcTemp = NULL ;


// Magic nuber =======

	iResult = IOD.Write( m4uint32_t( M4_CH_MAGIC_NUMBER_CMCR ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

// Tamaño =============

	iSize = GetSize() ;

	iResult = IOD.Write( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize == 0 )
	{
		return( M4_SUCCESS ) ;
	}


	bConvert = IOD.HasToSwap() ;
//	bConvert = M4_FALSE ;

// ahora se puede convertir a la fuerza para los .mcr

	if( bConvert == M4_TRUE || ai_bConvert == M4_TRUE )
	{
		iDoubleSize = ( ( iSize - 1 ) >> 0x03 ) + 1 ;

		pcTemp = ( m4pchar_t ) new m4double_t[ iDoubleSize ] ;

		CHECK_CHLOG_ERRORF( pcTemp == NULL, M4_ERROR, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4double_t ) * iDoubleSize ) << __LINE__ << __FILE__ ) ;

		memcpy( pcTemp, m_pcBase, sizeof( m4char_t ) * iSize ) ;
		SwapChannel( pcTemp, 0, 0 ) ;

		pcAux = ( m4uint8_t * ) pcTemp ;
	}
	else
	{
		pcAux = ( m4uint8_t * ) m_pcBase ;
	}


	iResult = IOD.Write( pcAux, iSize ) ;

    if( pcTemp != NULL )
    {
        delete [] pcTemp ;
        pcTemp = NULL ;
    }

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( IOD.Flush() != M4_TRUE )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t  ClCMCRWrapper :: DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult ;
	m4uint8_t	*pcAux ;
	m4bool_t	bConvert ;
    m4uint16_t	iBuild ;
    m4uint16_t	iRealBuild ;
	m4uint32_t	iSize ;
	m4uint32_t	iMagicNumber ;
	m4pchar_t	pcCMCR ;


// Magic nuber =======

	iResult = IOD.Read( iMagicNumber ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	CHECK_CHLOG_ERRORF( iMagicNumber != M4_CH_MAGIC_NUMBER_CMCR, M4_ERROR, M4_MDRT_BAD_MD_MAGIC_NUMBER, iMagicNumber << m4uint32_t( M4_CH_MAGIC_NUMBER_CMCR ) ) ;

// Tamaño =============

	iResult = IOD.Read( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize == 0 )
	{
		return( M4_ERROR ) ;
	}


    pcCMCR = CreateBuffer( iSize ) ;

	if( pcCMCR == NULL )
	{
		return( M4_ERROR ) ;
	}

	pcAux = ( m4uint8_t * ) m_pcBase ;

	iResult = IOD.Read( pcAux, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		_Reset() ;
		return( M4_ERROR ) ;
	}


	bConvert = IOD.HasToSwap() ;
//	bConvert = M4_FALSE ;


	/* Bug 0072262
	Hay que chequear la versión antes de hacer swap y controlar si
	hay que hacer swap de la propia versión.
	*/
	iBuild = M4ClUnPackInt16( m_pcBase, M4CL_MCR_CHANNEL_BUILD ) ;

	if( bConvert == M4_TRUE )
	{
		SwapInt16( (m4pchar_t) &iBuild, 0 ) ;
	}

	iRealBuild = GetM2VersionNumber() ;

	if( iBuild != iRealBuild )
	{
		_Reset() ;
		DUMP_CHLOG_ERRORF( M4_MDRT_BAD_M4OBJ_VERSION, iBuild << iRealBuild ) ;
		return( M4_ERROR ) ;
	}

	if( bConvert == M4_TRUE )
	{
		SwapChannel( m_pcBase, 0, 1 ) ;
	}

    return( M4_SUCCESS ) ;
}



// clase wrapper del chunk de seguridad =======================================


m4return_t  ClCSCRWrapper :: Serialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult ;
	m4uint8_t	*pcAux ;
	m4bool_t	bConvert ;
	m4uint32_t	iSize ;
	m4uint32_t	iDoubleSize ;
    m4pchar_t	pcTemp = NULL ;


// Magic nuber =======

	iResult = IOD.Write( m4uint32_t( M4_CH_MAGIC_NUMBER_CSCR ) ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

// Tamaño =============

	iSize = GetSize() ;

	iResult = IOD.Write( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize == 0 )
	{
		return( M4_SUCCESS ) ;
	}


	bConvert = IOD.HasToSwap() ;
//	bConvert = M4_FALSE ;

	if( bConvert == M4_TRUE )
	{
		iDoubleSize = ( ( iSize - 1 ) >> 0x03 ) + 1 ;

		pcTemp = ( m4pchar_t ) new m4double_t[ iDoubleSize ] ;

		CHECK_CHLOG_ERRORF( pcTemp == NULL, M4_ERROR, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4double_t ) * iDoubleSize ) << __LINE__ << __FILE__ ) ;

		memcpy( pcTemp, m_pcSecurity, sizeof( m4char_t ) * iSize ) ;
		SwapSecurity( pcTemp, 0, 0 ) ;

		pcAux = ( m4uint8_t * ) pcTemp ;
	}
	else
	{
		pcAux = ( m4uint8_t * ) m_pcSecurity ;
	}


	iResult = IOD.Write( pcAux, iSize ) ;

    if( pcTemp != NULL )
    {
        delete [] pcTemp ;
        pcTemp = NULL ;
    }

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( IOD.Flush() != M4_TRUE )
	{
		return( M4_ERROR ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClCSCRWrapper :: DeSerialize( ClGenericIODriver &IOD )
{

	m4return_t	iResult ;
	m4uint8_t	*pcAux ;
	m4bool_t	bConvert ;
    m4uint16_t	iBuild ;
    m4uint16_t	iRealBuild ;
	m4uint32_t	iSize ;
	m4uint32_t	iMagicNumber ;
	m4uint32_t	iDoubleSize ;


// Magic nuber =======

	iResult = IOD.Read( iMagicNumber ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	CHECK_CHLOG_ERRORF( iMagicNumber != M4_CH_MAGIC_NUMBER_CSCR, M4_ERROR, M4_MDRT_BAD_SEC_MAGIC_NUMBER, iMagicNumber << m4uint32_t( M4_CH_MAGIC_NUMBER_CSCR ) ) ;

// Tamaño =============

	iResult = IOD.Read( iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize == 0 )
	{
		return( M4_ERROR ) ;
	}


// alineamiento a 8

	iDoubleSize = ( ( iSize - 1 ) >> 0x03 ) + 1 ;

    m_pcSecurity = ( m4pchar_t ) new m4double_t[ iDoubleSize ] ;

	CHECK_CHLOG_ERRORF( m_pcSecurity == NULL, M4_ERROR, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4double_t ) * iDoubleSize ) << __LINE__ << __FILE__ ) ;


	pcAux = ( m4uint8_t * ) m_pcSecurity ;

	iResult = IOD.Read( pcAux, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		delete [] m_pcSecurity ;
		m_pcSecurity = NULL ;
		return( M4_ERROR ) ;
	}


	bConvert = IOD.HasToSwap() ;
//	bConvert = M4_FALSE ;


	/* Bug 0072262
	Hay que chequear la versión antes de hacer swap y controlar si
	hay que hacer swap de la propia versión.
	*/
	iBuild = M4ClUnPackInt16( m_pcSecurity, M4CL_SCR_SECURITY_BUILD ) ;

	if( bConvert == M4_TRUE )
	{
		SwapInt16( (m4pchar_t) &iBuild, 0 ) ;
	}

	iRealBuild = GetM2VersionNumber() ;

	if( iBuild != iRealBuild )
	{
		delete [] m_pcSecurity ;
		m_pcSecurity = NULL ;
		DUMP_CHLOG_ERRORF( M4_MDRT_BAD_M4OBJ_VERSION, iBuild << iRealBuild ) ;
		return( M4_ERROR ) ;
	}

	if( bConvert == M4_TRUE )
	{
		SwapSecurity( m_pcSecurity, 0, 1 ) ;
	}

    return( M4_SUCCESS ) ;
}



