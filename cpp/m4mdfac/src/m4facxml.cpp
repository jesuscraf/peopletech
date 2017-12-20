
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facxml.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene las funciones de xml de la factoría
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"
#include "m4facdef.hpp"

#include "m4mdrt.hpp"
#include "cldates.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "m4mdadaptor.hpp"
#include "m4objglb.hpp"
#include "m4lang.hpp"
#include "cldbgutil.hpp"
#include "m4unicode.hpp"
#include "replaceliterals.hpp"




//=============================================================================
// Importaciones
//=============================================================================
extern	m4return_t	M4ReadDate	( ClCompiledMCR *ai_poCMCR, m4date_t &ao_rdValue, m4date_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister ) ;
extern	m4return_t	M4ReadString( ClCompiledMCR *ai_poCMCR, m4pcchar_t &ao_rpccValue, m4pcchar_t ai_pccDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister ) ;
extern	m4return_t	M4ReadNumber( ClCompiledMCR *ai_poCMCR, m4double_t &ao_rdValue, m4double_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister, m4double_t ai_dMinimum, m4double_t ai_dMaximum ) ;

extern	m4uint8_t   g_aiReducedLanguagesArray[ M4CL_LANGUAGE_NUMBER ] ;


//=============================================================================
// Defines
//=============================================================================
#define	M4MDFAC_XML_BUFFER_SIZE								1000



//=============================================================================
// Macros de volcado básicas con control de errores
//=============================================================================

#define	M4MDFAC_PRINT_START( indent, object )		if( M4MdFacPrintStart( indent, object, ao_roXML, ai_pcBuffer ) != M4_SUCCESS ) return( M4_ERROR )
#define	M4MDFAC_PRINT_CONTINUE						if( M4MdFacPrintContinue( ao_roXML ) != M4_SUCCESS ) return( M4_ERROR )
#define	M4MDFAC_PRINT_TAG( tag, value, mask )		if( M4MdFacPrintTag( tag, value, ai_iMask, mask, ao_roXML, ai_pcBuffer ) != M4_SUCCESS ) return( M4_ERROR )
#define	M4MDFAC_PRINT_SIMPLE_END					if( M4MdFacPrintSimpleEnd( ao_roXML ) != M4_SUCCESS ) return( M4_ERROR )
#define	M4MDFAC_PRINT_END( indent, object )			if( M4MdFacPrintEnd( indent, object, ao_roXML, ai_pcBuffer ) != M4_SUCCESS ) return( M4_ERROR )




//=============================================================================
// Funciones de volcado básicas
//=============================================================================

m4return_t	M4MdFacPrintStart( m4uint8_t ai_iIndent, m4pcchar_t ai_pccObject, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer )
{

    m4uint16_t  i ;
    m4char_t    acIndent[ 10 ] ;


    for( i = 0 ; i < ai_iIndent ; i++ )
    {
        acIndent[ i ] = '\t' ;
    }

    acIndent[ i ] = '\0' ;


	sprintf( ai_pcBuffer, "%s<%s", acIndent, ai_pccObject ) ;
	return( ao_roXML.Concat( ai_pcBuffer ) ) ;
}


m4return_t	M4MdFacPrintContinue( ClLongString &ao_roXML )
{
	return( ao_roXML.Concat( ">\n" ) ) ;
}


m4return_t	M4MdFacPrintTag( m4pcchar_t ai_pccTag, m4int32_t ai_iValue, m4uint32_t ai_iMask, m4uint32_t ai_iElementMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer )
{
	if( ( ai_iMask & ai_iElementMask ) == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	sprintf( ai_pcBuffer, " %s=\"%d\"", ai_pccTag, ai_iValue ) ;
	return( ao_roXML.Concat( ai_pcBuffer ) ) ;
}


m4return_t	M4MdFacPrintTag( m4pcchar_t ai_pccTag, m4pcchar_t ai_pccValue, m4uint32_t ai_iMask, m4uint32_t ai_iElementMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer )
{
	
	m4return_t	iResult ;

	
	if( ( ai_iMask & ai_iElementMask ) == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	sprintf( ai_pcBuffer, " %s=\"", ai_pccTag ) ;

	iResult = ao_roXML.Concat( ai_pcBuffer ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ai_pccValue != NULL )
	{
		iResult = ao_roXML.Concat( ai_pccValue, m4uint8_t( M4MDRT_CODE_MODE_XML ) ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( ao_roXML.Concat( "\"" ) ) ;
}


m4return_t	M4MdFacPrintTag( m4pcchar_t ai_pccTag, m4VariantType &ai_rvValue, m4uint32_t ai_iMask, m4uint32_t ai_iElementMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer )
{

	m4return_t	iResult ;

	
	if( ( ai_iMask & ai_iElementMask ) == 0 )
	{
		return( M4_SUCCESS ) ;
	}

	sprintf( ai_pcBuffer, " %s=\"", ai_pccTag ) ;

	iResult = ao_roXML.Concat( ai_pcBuffer ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	switch( ai_rvValue.Type )
	{
		case	M4CL_CPP_TYPE_NUMBER :

			sprintf( ai_pcBuffer, "%f", ai_rvValue.Data.DoubleData ) ;
			iResult = ao_roXML.Concat( ai_pcBuffer ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			break ;

		case	M4CL_CPP_TYPE_STRING_VAR :

			if( ai_rvValue.Data.PointerChar != NULL )
			{
				iResult = ao_roXML.Concat( ai_rvValue.Data.PointerChar, m4uint8_t( M4MDRT_CODE_MODE_XML ) ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
			break ;

		case	M4CL_CPP_TYPE_DATE :

			ClDateToString( ai_rvValue.Data.DoubleData, ai_pcBuffer, M4_SECOND_END + 1 ) ;
			iResult = ao_roXML.Concat( ai_pcBuffer ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
			break ;

		default :
			break ;
	}

	return( ao_roXML.Concat( "\"" ) ) ;
}


m4return_t	M4MdFacPrintSimpleEnd( ClLongString &ao_roXML )
{
	return( ao_roXML.Concat( "/>\n" ) ) ;
}


m4return_t	M4MdFacPrintEnd( m4uint8_t ai_iIndent, m4pcchar_t ai_pccObject, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer )
{

    m4uint16_t  i ;
    m4char_t    acIndent[ 10 ] ;


    for( i = 0 ; i < ai_iIndent ; i++ )
    {
        acIndent[ i ] = '\t' ;
    }

    acIndent[ i ] = '\0' ;

	sprintf( ai_pcBuffer, "%s</%s>\n", acIndent, ai_pccObject ) ;
	return( ao_roXML.Concat( ai_pcBuffer ) ) ;
}


m4pcchar_t	M4MdFacDate( m4pchar_t ai_pcBuffer, m4date_t ai_dDate )
{
    ClDateToString( ai_dDate, ai_pcBuffer, M4_SECOND_END + 1 ) ;
	return( ai_pcBuffer ) ;
}


m4pcchar_t	M4MdFacItem( ClCompiledMCR *ai_poCMCR, m4uint32_t ai_iHandle )
{
	if( ai_iHandle == 0 )
	{
		return( NULL ) ;
	}

	return( ai_poCMCR->GetItemId( ai_iHandle ) ) ;
}


m4pcchar_t	M4MdFacParentNodeId( ClCompiledMCR *ai_poCMCR, m4uint32_t ai_iHandle, m4uint8_t *ao_piType )
{

	m4pcchar_t	pccParentNodeId ;
	m4uint8_t	iType ;
	m4uint16_t	i ;
	m4uint16_t	iParents ;
	m4uint32_t	iConnector ;
	m4uint32_t	iHandle ;


	if( ao_piType != NULL )
	{
		*ao_piType = 0 ;
	}

	iParents = ai_poCMCR->GetNodeNumberOfInvConnectors( ai_iHandle ) ;

	for( i = 0 ; i < iParents ; i++ )
	{
		iConnector = ai_poCMCR->GetNodeInvConnectorHandleByPosition( ai_iHandle, i ) ;
		iType = ai_poCMCR->GetNodeConnectorType( iConnector ) ;

		if( iType == M4CL_NODE_CONNECTOR_TYPE_RB || iType == M4CL_NODE_CONNECTOR_TYPE_BB )
		{
			if( ao_piType != NULL )
			{
				*ao_piType = iType ;
			}

			iHandle = ai_poCMCR->GetNodeConnectorFatherNodeHandle( iConnector ) ;
			pccParentNodeId = ai_poCMCR->GetNodeId( iHandle ) ;
			return( pccParentNodeId ) ;
		}
	}

	return( "" ) ;
}



// ============================================================================
// Funciones de generación
// ============================================================================

m4return_t	ClFactoryImp :: _PrintXmlArg( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition  )
{

	m4uint8_t	iType ;
	m4uint8_t	iM4Type ;
	m4uint16_t	iPrecision ;
	m4pcchar_t	pccArgId ;
	m4pcchar_t	pccArgName ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "arg" ) ;


// Propiedades ================================================================

	pccArgId =		ai_poCMCR->GetItemArgumentIdByPosition			( ai_iItemHandle, ai_iPosition ) ;
	pccArgName =	ai_poCMCR->GetItemArgumentNameByPosition		( ai_iItemHandle, ai_iPosition ) ;
	iType =			ai_poCMCR->GetItemArgumentTypeByPosition		( ai_iItemHandle, ai_iPosition ) ;
	iM4Type =		ai_poCMCR->GetItemArgumentM4TypeByPosition		( ai_iItemHandle, ai_iPosition ) ;
	iPrecision =	ai_poCMCR->GetItemArgumentPrecisionByPosition	( ai_iItemHandle, ai_iPosition ) ;

	M4MDFAC_PRINT_TAG( "id",	pccArgId,							M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "n",		pccArgName,							M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "tp",	iType,								M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "mtp",	iM4Type,							M4XMASK_ALL | M4XMASK_NET  | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "ctp",	ConvertM4TypeToCppType( iM4Type ),	M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "pr",	iPrecision,							M4XMASK_ALL ) ;

// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlConnector( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4pcchar_t ai_pccTag )
{

	m4uint32_t	iConnector ;
	m4uint32_t	iHandle ;
	m4pcchar_t	pccNode ;
	m4pcchar_t	pccItem ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, ai_pccTag ) ;


// Propiedades ================================================================

	iConnector = ai_poCMCR->GetItemConnectorNodeConnectorHandle( ai_iHandle ) ;
	iHandle = ai_poCMCR->GetNodeConnectorSonNodeHandle( iConnector ) ;
	pccNode = ai_poCMCR->GetNodeId( iHandle ) ;
	iHandle = ai_poCMCR->GetItemConnectorSonItemHandle( ai_iHandle ) ;
	pccItem = ai_poCMCR->GetItemId( iHandle ) ;

	M4MDFAC_PRINT_TAG( "nid",	pccNode,	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "id",	pccItem,	M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "pc",	ai_poCMCR->GetItemConnectorPrecedence( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "sp",	ai_poCMCR->GetItemConnectorSpin( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "rt",	ai_poCMCR->GetItemConnectorRelation( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ct",	ai_poCMCR->GetItemConnectorContext( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cs",	ai_poCMCR->GetItemConnectorCsType( ai_iHandle ),		M4XMASK_ALL ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlItem( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iIndex, m4pcchar_t ai_pccOrganization )
{

	m4return_t		iResult ;
	m4bool_t		bBody ;
	m4bool_t		bModified ;
	m4uint8_t		iType ;
	m4uint8_t		iSonType ;
	m4uint8_t		iIsExecutable ;
	m4uint8_t		iPrecedence ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4uint32_t		iConnector ;
	m4uint32_t		iHandle ;
	m4pchar_t		pcOutput ;
	m4pcchar_t		pccName ;
	m4VariantType	vValue ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "item" ) ;


// Propiedades ================================================================

	pccName = ai_poCMCR->GetItemName( ai_iHandle ) ;

	if( pccName == NULL )
	{
		pccName = ai_poCMCR->GetItemId( ai_iHandle ) ;
	}

	iResult = M4ReplaceLiterals( m_poChannelManager, pccName, pcOutput, ai_pccOrganization, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		pccName = pcOutput ;
	}

	iConnector = ai_poCMCR->GetItemInverseConnector( ai_iHandle ) ;

	if( iConnector != 0 && iConnector != m4uint32_t( -1 ) )
	{
		iHandle = ai_poCMCR->GetItemConnectorFatherItemHandle( iConnector ) ; ;
	}
	else
	{
		iHandle = 0 ;
	}

	iType = ai_poCMCR->GetItemType( ai_iHandle ) ;

	if( iType == M4CL_ITEM_TYPE_METHOD || iType == M4CL_ITEM_TYPE_CONCEPT )
	{
		iIsExecutable = 1 ;
	}
	else
	{
		iIsExecutable = 0 ;

	}

	ai_poCMCR->GetItemDefaultValue( ai_iHandle, vValue ) ;

	
	M4MDFAC_PRINT_TAG( "id",	ai_poCMCR->GetItemId( ai_iHandle ),					M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "pi",	M4MdFacItem( ai_poCMCR, iHandle ),					M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "n",		pccName,											M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "s",		ai_poCMCR->GetItemSynonymId( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ti",	ai_poCMCR->GetItemTiId( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "tsc",	ai_poCMCR->GetItemTheoreticScope( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "sp",	ai_poCMCR->GetItemScope( ai_iHandle ),				M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "tp",	iType,												M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "ie",	iIsExecutable,										M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "ctp",	ai_poCMCR->GetItemCppType( ai_iHandle ),			M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "mtp",	ai_poCMCR->GetItemM4Type( ai_iHandle ),				M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "dpr",	ai_poCMCR->GetItemDBPrecision( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pr",	ai_poCMCR->GetItemPrecision( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "sc",	ai_poCMCR->GetItemScale( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "in",	ai_iIndex,											M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "or",	ai_poCMCR->GetItemOrder( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "dv",	vValue,												M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "dmd",	ai_poCMCR->GetItemDmd( ai_iHandle ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dc",	ai_poCMCR->GetItemDmdComponent( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dct",	ai_poCMCR->GetItemDmdCrosstab( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "df",	ai_poCMCR->GetItemDmdField( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ms",	ai_poCMCR->GetItemMaxSize( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "roa",	ai_poCMCR->GetItemReadObjectAlias( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "woa",	ai_poCMCR->GetItemWriteObjectAlias( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "rf",	ai_poCMCR->GetItemReadFieldId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "wf",	ai_poCMCR->GetItemWriteFieldId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ro",	ai_poCMCR->GetItemReadObjectId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "wo",	ai_poCMCR->GetItemWriteObjectId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "cs",	ai_poCMCR->GetItemCsType( ai_iHandle ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "st",	ai_poCMCR->GetItemSyncType( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "sb",	ai_poCMCR->GetItemSliceBhrv( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ss",	ai_poCMCR->GetItemSliceSplit( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "to",	ai_poCMCR->GetItemIdTotalize( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "sto",	ai_poCMCR->GetItemSliceTotalize( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "tm",	ai_poCMCR->GetItemTransactionMode( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ml",	ai_poCMCR->GetItemMethodLevel( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "vl",	ai_poCMCR->GetItemIsVariableLength( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "stt",	ai_poCMCR->GetItemSentTotType( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "va",	ai_poCMCR->GetItemVariableArguments( ai_iHandle ),	M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "vi",	ai_poCMCR->GetItemIsVisible( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "md",	ai_poCMCR->GetItemModifyData( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "dvf",	ai_poCMCR->GetItemDefaultValueFlag( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "it",	ai_poCMCR->GetItemInternalType( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "cf",	ai_poCMCR->GetItemConvertFunction( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ih",	ai_poCMCR->GetItemIsInherited( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pb",	ai_poCMCR->GetItemIsPublic( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pm",	ai_poCMCR->GetItemIsParameter( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ckn",	ai_poCMCR->GetItemCheckNotNull( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "cln",	ai_poCMCR->GetItemClientNotNull( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "nn",	ai_poCMCR->GetItemNotNull( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "ab",	ai_poCMCR->GetItemAffectsDB( ai_iHandle ),			M4XMASK_ALL | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "gs",	ai_poCMCR->GetItemGenerateSlices( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pk",	ai_poCMCR->GetItemIsPK( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "nd",	ai_poCMCR->GetItemIsNodal( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ev",	ai_poCMCR->GetItemIsEvent( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "lv",	ai_poCMCR->GetItemLevel( ai_iHandle ),				M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "si",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetItemSliceItem( ai_iHandle ) ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ai",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetItemAuxItem( ai_iHandle ) ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;

	M4MDFAC_PRINT_TAG( "jad",	"",													M4XMASK_ALL | M4XMASK_SOAP) ;
	M4MDFAC_PRINT_TAG( "jac",	"",													M4XMASK_ALL | M4XMASK_SOAP) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),					M4XMASK_ALL | M4XMASK_SOAP ) ;
	
	if( pcOutput != NULL )
	{
		delete [] pcOutput ;
	}

	bBody = M4_FALSE ;


// Argumentos =================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		iLength = ai_poCMCR->GetItemNumberOfArguments( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlArg( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, ai_iHandle, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Conectores =================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iType = ai_poCMCR->GetItemType( ai_iHandle ) ;
		iLength = ai_poCMCR->GetItemNumberOfConnectedItems( ai_iHandle ) ;

		// Si es método o concepto los before y after
		if( iType == M4CL_ITEM_TYPE_METHOD || iType == M4CL_ITEM_TYPE_CONCEPT )
		{
			// Los before
			for( i = 0 ; i < iLength ; i++ )
			{
				iConnector = ai_poCMCR->GetItemConnectorHandleByPosition( ai_iHandle, i ) ;
				iHandle = ai_poCMCR->GetItemConnectorSonItemHandle( iConnector ) ;
				iPrecedence = ai_poCMCR->GetItemConnectorPrecedence( iConnector ) ;
				iSonType = ai_poCMCR->GetItemType( iHandle ) ;

				if( iPrecedence == M4CL_ITEM_CONNECTOR_BEFORE && ( iSonType == M4CL_ITEM_TYPE_METHOD || iSonType == M4CL_ITEM_TYPE_CONCEPT ) )
				{
					if( iSonType != M4CL_ITEM_TYPE_CONCEPT || iType != M4CL_ITEM_TYPE_CONCEPT )
					{
						if( bBody == M4_FALSE )
						{
							M4MDFAC_PRINT_CONTINUE ;
							bBody = M4_TRUE ;
						}

						iResult = _PrintXmlConnector( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iConnector, "bconn" ) ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}
					}
				}
			}

			// Los after
			for( i = 0 ; i < iLength ; i++ )
			{
				iConnector = ai_poCMCR->GetItemConnectorHandleByPosition( ai_iHandle, i ) ;
				iHandle = ai_poCMCR->GetItemConnectorSonItemHandle( iConnector ) ;
				iPrecedence = ai_poCMCR->GetItemConnectorPrecedence( iConnector ) ;
				iSonType = ai_poCMCR->GetItemType( iHandle ) ;

				if( iPrecedence == M4CL_ITEM_CONNECTOR_AFTER && ( iSonType == M4CL_ITEM_TYPE_METHOD || iSonType == M4CL_ITEM_TYPE_CONCEPT ) )
				{
					if( iSonType != M4CL_ITEM_TYPE_CONCEPT || iType != M4CL_ITEM_TYPE_CONCEPT )
					{
						if( bBody == M4_FALSE )
						{
							M4MDFAC_PRINT_CONTINUE ;
							bBody = M4_TRUE ;
						}

						iResult = _PrintXmlConnector( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iConnector, "aconn" ) ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}
					}
				}
			}
		}

		// Si es celda, los conectores de valor
		if( iType != M4CL_ITEM_TYPE_METHOD )
		{
			for( i = 0 ; i < iLength ; i++ )
			{
				iConnector = ai_poCMCR->GetItemConnectorHandleByPosition( ai_iHandle, i ) ;
				iHandle = ai_poCMCR->GetItemConnectorSonItemHandle( iConnector ) ;

				iSonType = ai_poCMCR->GetItemType( iHandle ) ;

				if( iSonType != M4CL_ITEM_TYPE_METHOD )
				{
					if( bBody == M4_FALSE )
					{
						M4MDFAC_PRINT_CONTINUE ;
						bBody = M4_TRUE ;
					}

					iResult = _PrintXmlConnector( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iConnector, "vconn" ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "item" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlIndexItem( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iPosition )
{

// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "indexitem" ) ;


// Propiedades ================================================================

	M4MDFAC_PRINT_TAG( "id",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetIndexItemHandleByPosition( ai_iHandle, ai_iPosition ) ) ,	M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "f",		ai_poCMCR->GetIndexFunctionByPosition( ai_iHandle, ai_iPosition ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "w",		ai_poCMCR->GetIndexWayByPosition( ai_iHandle, ai_iPosition ),		M4XMASK_ALL ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlIndex( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4return_t	iResult ;
	m4bool_t	bBody ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "index" ) ;


// Propiedades ================================================================

	M4MDFAC_PRINT_TAG( "id",	ai_poCMCR->GetIndexId( ai_iHandle ),	M4XMASK_ALL ) ;

	bBody = M4_FALSE ;


// Items ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iLength = ai_poCMCR->GetIndexNumberOfItems( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlIndexItem( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, ai_iHandle, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "index" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlListColumn( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4uint32_t	iChildItem ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "listcol" ) ;


// Propiedades ================================================================

	iChildItem = ai_poCMCR->GetPlugItemSonItemHandle( ai_iHandle ) ;

	M4MDFAC_PRINT_TAG( "pc",	ai_poCMCR->GetPlugItemFatherItemId( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "cc",	ai_poCMCR->GetItemId( iChildItem ),					M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlList( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccOrganization )
{

	m4return_t	iResult ;
	m4bool_t	bBody ;
	m4bool_t	bModified ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iPlugItemHandle ;
	m4pchar_t	pcOutput ;
	m4pcchar_t	pccListId ;
	m4pcchar_t	pccName ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "list" ) ;


// Propiedades ================================================================

	pccListId = ai_poCMCR->GetPlugFatherChanneId( ai_iHandle ) ;
	pccName = ai_poCMCR->GetPlugFatherChanneName( ai_iHandle ) ;

	if( pccName == NULL )
	{
		pccName = pccListId ;
	}

	iResult = M4ReplaceLiterals( m_poChannelManager, pccName, pcOutput, ai_pccOrganization, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		pccName = pcOutput ;
	}

	M4MDFAC_PRINT_TAG( "id",	pccListId,											M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "n",		pccName,											M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "p",		ai_poCMCR->GetPlugFatherNodeId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "c",		ai_poCMCR->GetNodeId( ai_iNodeHandle ),				M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "tp",	ai_poCMCR->GetPlugRelationTypeId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "m",		ai_poCMCR->GetPlugIsMultiselection( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;

	if( pcOutput != NULL )
	{
		delete [] pcOutput ;
	}

	bBody = M4_FALSE ;


// Items ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		iLength = ai_poCMCR->GetPlugNumberOfItems( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iPlugItemHandle = ai_poCMCR->GetPlugItemHandleByPosition( ai_iHandle, i ) ;

			iResult = _PrintXmlListColumn( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iPlugItemHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "list" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlFilter( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint8_t ai_iType, m4uint32_t ai_iNodeHandle )
{

// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "filter" ) ;


// Propiedades ================================================================

	M4MDFAC_PRINT_TAG( "id",	ai_poCMCR->GetFilterId( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "m4o",	ai_poCMCR->GetFilterT3Id( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "p",		ai_poCMCR->GetFilterNodeId( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "c",		ai_poCMCR->GetNodeId( ai_iNodeHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "tp",	ai_iType,											M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ss",	ai_poCMCR->GetFilterSysSentenceItem( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "sp",	ai_poCMCR->GetFilterSysParamItem( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "b",		ai_poCMCR->GetFilterBehaviourType( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "o",		ai_poCMCR->GetFilterIsOwnNode( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlAlias( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iNodeHandle, m4uint16_t ai_iPosition )
{

	m4uint32_t	iHandle ;
	m4pcchar_t	pccAlias ;
	m4pcchar_t	pccNode ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "alias" ) ;


// Propiedades ================================================================

	pccAlias = ai_poCMCR->GetNodeAliasIdByPosition( ai_iNodeHandle, ai_iPosition ) ;
	iHandle = ai_poCMCR->GetNodeAliasNodeHandleByPosition( ai_iNodeHandle, ai_iPosition ) ;
	pccNode = ai_poCMCR->GetNodeId( iHandle ) ;

	M4MDFAC_PRINT_TAG( "id",	pccAlias,	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "nd",	pccNode,	M4XMASK_ALL ) ;

// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlNode( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4pcchar_t ai_pccParentNode, m4pcchar_t ai_pccOrganization )
{

	m4return_t	iResult ;
	m4bool_t	bBody ;
	m4bool_t	bModified ;
	m4uint8_t	iType ;
	m4uint8_t	iScope ;
	m4uint8_t	iConnectorType ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint16_t	iIndex ;
	m4uint16_t	iFirst ;
	m4uint32_t	iHandle ;
	m4uint32_t	iSentence ;
	m4uint32_t	iConnector ;
	m4uint32_t	iExecuteReport ;
	m4pchar_t	pcOutput ;
	m4pcchar_t	pccNodeId ;
	m4pcchar_t	pccName ;
	m4pcchar_t	pccSentenceId ;
	m4pcchar_t	pccObjectGroup ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "node" ) ;


// Propiedades ================================================================

	pccNodeId = ai_poCMCR->GetNodeId( ai_iHandle ) ;
	pccName = ai_poCMCR->GetNodeName( ai_iHandle ) ;

	if( pccName == NULL )
	{
		pccName = pccNodeId ;
	}

	iResult = M4ReplaceLiterals( m_poChannelManager, pccName, pcOutput, ai_pccOrganization, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		pccName = pcOutput ;
	}

	iSentence = ai_poCMCR->GetNodeReadSentenceHandle( ai_iHandle ) ;

	if( iSentence != 0 )
	{
		pccSentenceId = ai_poCMCR->GetSentenceApiSql( iSentence ) ;
		pccObjectGroup = ai_poCMCR->GetSentenceGroupObjects( iSentence ) ;
	}
	else
	{
		pccSentenceId = "" ;
		pccObjectGroup = "" ;
	}

	M4MdFacParentNodeId( ai_poCMCR, ai_iHandle, &iConnectorType ) ;

	M4MDFAC_PRINT_TAG( "id",	pccNodeId,											M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "pn",	ai_pccParentNode,									M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "pct",	iConnectorType,										M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "n",		pccName,											M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "ns",	ai_poCMCR->GetNodeTiId( ai_iHandle ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "mid",	ai_poCMCR->GetNodeM4ObjId( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "rs",	pccSentenceId,										M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "og",	pccObjectGroup,										M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "tp",	ai_poCMCR->GetNodeType( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "in",	ai_poCMCR->GetNodeIndex( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "or",	ai_poCMCR->GetNodeOrder( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "rsi",	ai_poCMCR->GetNodeReadSentenceId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "wsi",	ai_poCMCR->GetNodeWriteSentenceId( ai_iHandle ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ro",	ai_poCMCR->GetNodeReadObjectId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "wo",	ai_poCMCR->GetNodeWriteObjectId( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "tm",	ai_poCMCR->GetNodeTemporality( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "cm",	ai_poCMCR->GetNodeCompleteness( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "pc",	ai_poCMCR->GetNodePeriodCorrected( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "os",	ai_poCMCR->GetNodeOwnSentence( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "di",	ai_poCMCR->GetNodeDefaultIndex( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "nr",	ai_poCMCR->GetNodeNumRows( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "nrd",	ai_poCMCR->GetNodeNumRowsDB( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "nkr",	ai_poCMCR->GetNodeNumKeepRows( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "mnb",	ai_poCMCR->GetNodeMaxNumBlocks( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "mnr",	ai_poCMCR->GetNodeMaxNumRecords( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cs",	ai_poCMCR->GetNodeCsType( ai_iHandle ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "st",	ai_poCMCR->GetNodeSyncType( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "odi",	ai_poCMCR->GetNodeOleDispId( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "oni",	ai_poCMCR->GetNodeOleNodeInterId( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cp",	ai_poCMCR->GetNodeCapacity( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "rt",	ai_poCMCR->GetNodeIsRoot( ai_iHandle ),				M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "rr",	ai_poCMCR->GetNodeIsRealRoot( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "at",	ai_poCMCR->GetNodeAutoLoad( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "io",	ai_poCMCR->GetNodeIsOrdered( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "vi",	ai_poCMCR->GetNodeIsVisible( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "df",	ai_poCMCR->GetNodeDynFilter( ai_iHandle ),			M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "af",	ai_poCMCR->GetNodeIdAutoFilter( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ex",	ai_poCMCR->GetNodeIsExternal( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "qbf",	ai_poCMCR->GetNodeIsQBFFilter( ai_iHandle ),		M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ab",	ai_poCMCR->GetNodeAffectsDB( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dr",	ai_poCMCR->GetNodeDBReload( ai_iHandle ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "il",	ai_poCMCR->GetNodeTiInheritLevel( ai_iHandle ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "go",	ai_poCMCR->GetNodeGroupObjects( ai_iHandle ),		M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "sdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeStartDateItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "edi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeEndDateItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "scdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeStartCorDateItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ecdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeEndCorDateItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "vfi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeVirtualFlagItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "fsdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeFilterStartDateItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "fedi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeFilterEndDateItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "fscdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeFilterStartCorDateItem( ai_iHandle ) ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "fecdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeFilterEndCorDateItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cti",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeCurrencyTypeItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cedi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeCurrencyExchDateItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cri",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeChangeReasonItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dci",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeDmdComponentItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dvi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeDmdValueItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "lsi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeLoadSQLItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodePriorityItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "idi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeImputeDateItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodePayDateItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pti",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodePayTypeItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "pfi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodePayFrequencyItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ipti",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeImputePayTypeItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ipfi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeImputePayFrequencyItem( ai_iHandle ) ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cdi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeClosingDateItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "spi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeSysParamsItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "bci",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeBDLChecksItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "roi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeRAMOrderByItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "oii",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeOrganizationIdItem( ai_iHandle ) ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "lbi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeLoadBlkItem( ai_iHandle ) ),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dbi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeDeleteBlkItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ubi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeUpdateBlkItem( ai_iHandle ) ),			M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ibi",	M4MdFacItem( ai_poCMCR, ai_poCMCR->GetNodeInsertBlkItem( ai_iHandle ) ),			M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),													M4XMASK_ALL | M4XMASK_SOAP ) ;

	if( pcOutput != NULL )
	{
		delete [] pcOutput ;
	}

	bBody = M4_FALSE ;


// Items ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		iIndex = 0 ;
		iLength = ai_poCMCR->GetNodeNumberOfItems( ai_iHandle ) ;
		iExecuteReport = ai_poCMCR->GetChannelExecuteReportHandle() ;

		// Primero los que no son métodos y son de ámbito nodo
		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetNodeItemHandleByPosition( ai_iHandle, i ) ;
			iType = ai_poCMCR->GetItemType( iHandle ) ;
			iScope = ai_poCMCR->GetItemScope( iHandle ) ;

			if( iType == M4CL_ITEM_TYPE_METHOD || iScope != M4CL_ITEM_SCOPE_NODE )
			{
				continue ;
			}

			if( iExecuteReport != 0 && ( ai_iMask & M4XMASK_SOAP ) != 0 && iExecuteReport == ai_poCMCR->GetItemDefinitionHandle( iHandle ) )
			{
				// El execute report generado no se devuelve en el caso de soap
				continue ;
			}

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlItem( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, iIndex++, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		// Segundo los que no son métodos y son de ámbito bloque
		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetNodeItemHandleByPosition( ai_iHandle, i ) ;
			iType = ai_poCMCR->GetItemType( iHandle ) ;
			iScope = ai_poCMCR->GetItemScope( iHandle ) ;

			if( iType == M4CL_ITEM_TYPE_METHOD || iScope != M4CL_ITEM_SCOPE_BLOCK )
			{
				continue ;
			}

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlItem( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, iIndex++, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		// Tercero los que no son métodos y son de ámbito registro
		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetNodeItemHandleByPosition( ai_iHandle, i ) ;
			iType = ai_poCMCR->GetItemType( iHandle ) ;
			iScope = ai_poCMCR->GetItemScope( iHandle ) ;

			if( iType == M4CL_ITEM_TYPE_METHOD || iScope != M4CL_ITEM_SCOPE_REGISTER )
			{
				continue ;
			}

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlItem( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, iIndex++, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		// Por último los métodos los que son métodos
		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetNodeItemHandleByPosition( ai_iHandle, i ) ;
			iType = ai_poCMCR->GetItemType( iHandle ) ;

			if( iType != M4CL_ITEM_TYPE_METHOD )
			{
				continue ;
			}

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlItem( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, iIndex++, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Alias ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iLength = ai_poCMCR->GetNodeNumberOfAlias( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlAlias( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, ai_iHandle, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Indices ====================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iLength = ai_poCMCR->GetNodeNumberOfIndexes( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iHandle = ai_poCMCR->GetNodeIndexHandleByPosition( ai_iHandle, i ) ;

			iResult = _PrintXmlIndex( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Hijos ======================================================================

	// Por defecto se sacan los nodos anidados
	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		// Se sacan RB y BB
		iLength = ai_poCMCR->GetNodeNumberOfRBConnectors( ai_iHandle ) ;
		iLength += ai_poCMCR->GetNodeNumberOfBBConnectors( ai_iHandle ) ;
		iFirst = ai_poCMCR->GetNodeRBFirstConnector( ai_iHandle ) ;

		for( i = iFirst ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iConnector = ai_poCMCR->GetNodeConnectorHandleByPosition( ai_iHandle, i ) ;
			iHandle = ai_poCMCR->GetNodeConnectorSonNodeHandle( iConnector ) ;

			iResult = _PrintXmlNode( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, pccNodeId, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "node" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}


// Si es de SOAP o NET se sacan los nodos planos =============

	if( ( ai_iMask & ( M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		// Se sacan RB y BB
		iLength = ai_poCMCR->GetNodeNumberOfRBConnectors( ai_iHandle ) ;
		iLength += ai_poCMCR->GetNodeNumberOfBBConnectors( ai_iHandle ) ;
		iFirst = ai_poCMCR->GetNodeRBFirstConnector( ai_iHandle ) ;

		for( i = iFirst ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iConnector = ai_poCMCR->GetNodeConnectorHandleByPosition( ai_iHandle, i ) ;
			iHandle = ai_poCMCR->GetNodeConnectorSonNodeHandle( iConnector ) ;

			iResult = _PrintXmlNode( ai_poCMCR, ai_iIndent, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, pccNodeId, ai_pccOrganization ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlBasicBusArg( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle, m4uint16_t ai_iPosition )
{

	m4uint8_t	iType ;
	m4uint8_t	iM4Type ;
	m4pcchar_t	pccArgId ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "barg" ) ;


// Propiedades ================================================================

	pccArgId = ai_poCMCR->GetItemArgumentIdByPosition( ai_iHandle, ai_iPosition ) ;
	iType = ai_poCMCR->GetItemArgumentTypeByPosition( ai_iHandle, ai_iPosition ) ;
	iM4Type = ai_poCMCR->GetItemArgumentM4TypeByPosition( ai_iHandle, ai_iPosition ) ;

	M4MDFAC_PRINT_TAG( "id",	pccArgId,							M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "pn",	"",									M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "gr",	1,									M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "tp",	iType,								M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "ctp",	ConvertM4TypeToCppType( iM4Type ),	M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),	M4XMASK_ALL | M4XMASK_SOAP ) ;



// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlObjectBusArg( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iNodeHandle )
{

	m4pcchar_t	pccArgNode ;
	m4pcchar_t	pccParentNodeId ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "barg" ) ;


// Propiedades ================================================================

	pccArgNode = ai_poCMCR->GetNodeId( ai_iNodeHandle ) ;
	pccParentNodeId = M4MdFacParentNodeId( ai_poCMCR, ai_iNodeHandle, NULL ) ;

	M4MDFAC_PRINT_TAG( "id",	pccArgNode,						 M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "pn",	pccParentNodeId,				 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "gr",	2,								 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "tp",	2,								 M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ctp",	2,								 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(), M4XMASK_ALL | M4XMASK_SOAP ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlObjectBusArg( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBusinessMehtodHandle, m4uint16_t ai_iPosition )
{

	m4uint8_t	iType ;
	m4uint32_t	iNode ;
	m4pcchar_t	pccArgNode ;
	m4pcchar_t	pccParentNodeId ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "barg" ) ;


// Propiedades ================================================================

	iNode = ai_poCMCR->GetBusinessMethodArgumentNodeHandleByPosition( ai_iBusinessMehtodHandle, ai_iPosition ) ;
	iType = ai_poCMCR->GetBusinessMethodArgumentTypeByPosition( ai_iBusinessMehtodHandle, ai_iPosition ) ;

	pccArgNode = ai_poCMCR->GetNodeId( iNode ) ;
	pccParentNodeId = M4MdFacParentNodeId( ai_poCMCR, iNode, NULL ) ;

	M4MDFAC_PRINT_TAG( "id",	pccArgNode,						 M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "pn",	pccParentNodeId,				 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "gr",	2,								 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "tp",	iType,							 M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ctp",	2,								 M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(), M4XMASK_ALL | M4XMASK_SOAP ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlBusinessMethod( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4pcchar_t ai_pccMethodId, m4uint32_t ai_iItemHandle )
{

	m4return_t			iResult ;
	m4bool_t			bBody ;
	m4uint8_t			iM4Type ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4uint32_t			iNode ;
	m4uint32_t			iItem ;
	m4pcchar_t			pccMethodId ;
	m4pcchar_t			pccMethodName ;
	m4pcchar_t			pccNodeId ;
	m4pcchar_t			pccItemId ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "bmethod" ) ;


// Propiedades ================================================================

	iItem = ai_iItemHandle ;
	iNode = ai_poCMCR->GetItemNodeHandle( iItem ) ;

	pccMethodId = ai_pccMethodId ;
	pccMethodName = ai_pccMethodId ;
	pccNodeId = ai_poCMCR->GetNodeId( iNode ) ;
	pccItemId = ai_poCMCR->GetItemId( iItem ) ;
	iM4Type = ai_poCMCR->GetItemM4Type( iItem ) ;

	M4MDFAC_PRINT_TAG( "id",	pccMethodId,								M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "nm",	pccMethodName,								M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "n",		pccNodeId,									M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "i",		pccItemId,									M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ctp",	ConvertM4TypeToCppType( iM4Type ),			M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "d",		pccMethodName,								M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),			M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "it",	ai_poCMCR->GetItemInternalType( iItem ),	M4XMASK_ALL | M4XMASK_SOAP ) ;

	bBody = M4_FALSE ;


// Argumentos =================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP ) ) != 0 )
	{
		// Los argumentos básicos

		iLength = ai_poCMCR->GetItemNumberOfArguments( iItem ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlBasicBusArg( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iItem, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ) != 0 )
	{
		// Los argumentos de tipo objeto

		iLength = ai_poCMCR->GetChannelNumberOfRoots() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iNode = ai_poCMCR->GetChannelRootHandleByPosition( i ) ;

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlObjectBusArg( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iNode ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "bmethod" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _PrintXmlBusinessMethod( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4return_t			iResult ;
	m4bool_t			bBody ;
	m4uint8_t			iM4Type ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4uint32_t			iNode ;
	m4uint32_t			iItem ;
	m4pcchar_t			pccMethodId ;
	m4pcchar_t			pccMethodName ;
	m4pcchar_t			pccNodeId ;
	m4pcchar_t			pccItemId ;
	m4pcchar_t			pccDescription ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "bmethod" ) ;


// Propiedades ================================================================

	pccMethodId = ai_poCMCR->GetBusinessMethodId( ai_iHandle ) ;
	pccMethodName = ai_poCMCR->GetBusinessMethodName( ai_iHandle ) ;
	pccDescription = ai_poCMCR->GetBusinessMethodDescription( ai_iHandle ) ;

	iItem = ai_poCMCR->GetBusinessMethodItemHandle( ai_iHandle ) ;
	iNode = ai_poCMCR->GetItemNodeHandle( iItem ) ;

	pccNodeId = ai_poCMCR->GetNodeId( iNode ) ;
	pccItemId = ai_poCMCR->GetItemId( iItem ) ;
	iM4Type = ai_poCMCR->GetItemM4Type( iItem ) ;

	M4MDFAC_PRINT_TAG( "id",	pccMethodId,								M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "nm",	pccMethodName,								M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "n",		pccNodeId,									M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "i",		pccItemId,									M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ctp",	ConvertM4TypeToCppType( iM4Type ),			M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "d",		pccDescription,								M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),			M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "it",	ai_poCMCR->GetItemInternalType( iItem ),	M4XMASK_ALL | M4XMASK_SOAP ) ;

	bBody = M4_FALSE ;


// Argumentos =================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP ) ) != 0 )
	{
		// Los argumentos básicos

		iLength = ai_poCMCR->GetItemNumberOfArguments( iItem ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlBasicBusArg( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iItem, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ) != 0 )
	{
		// Los argumentos de tipo objeto

		iLength = ai_poCMCR->GetBusinessMethodNumberOfArguments( ai_iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlObjectBusArg( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, ai_iHandle, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "bmethod" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlRelationColumn( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4uint32_t	iParentItem ;
	m4uint32_t	iChildItem ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "col" ) ;


// Propiedades ================================================================

	iParentItem = ai_poCMCR->GetItemConnectorFatherItemHandle( ai_iHandle ) ;
	iChildItem = ai_poCMCR->GetItemConnectorSonItemHandle( ai_iHandle ) ;

	M4MDFAC_PRINT_TAG( "pc", ai_poCMCR->GetItemId( iParentItem ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "cc", ai_poCMCR->GetItemId( iChildItem ),	M4XMASK_ALL | M4XMASK_NET ) ;


// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlRelation( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4return_t	iResult ;
	m4uint8_t	iType ;
	m4uint8_t	iRelation ;
	m4uint16_t	i ;
	m4uint16_t	j ;
	m4uint16_t	iLength ;
	m4uint16_t	iFirst ;
	m4uint16_t	iLast ;
	m4uint32_t	iHandle ;
	m4uint32_t	iParentNode ;
	m4uint32_t	iItem ;
	m4uint32_t	iParentItem ;
	m4uint32_t	iItemConnectorHandle ;
	m4uint32_t	iConnector ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "relation" ) ;


// Propiedades ================================================================

	iHandle = ai_poCMCR->GetNodeConnectorSonNodeHandle( ai_iHandle ) ;
	iParentNode = ai_poCMCR->GetNodeConnectorFatherNodeHandle( ai_iHandle ) ;

	M4MDFAC_PRINT_TAG( "id", ai_poCMCR->GetNodeId( iHandle ),						M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "pn", ai_poCMCR->GetNodeId( iParentNode ),					M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "ed", ai_poCMCR->GetNodeConnectorExpandDelete( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "eu", ai_poCMCR->GetNodeConnectorExpandUndo( ai_iHandle ),	M4XMASK_ALL | M4XMASK_NET ) ;

	M4MDFAC_PRINT_CONTINUE ;


// Columnas ===================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET ) ) != 0 )
	{
		iLength = ai_poCMCR->GetNodeNumberOfItems( iHandle ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iItem = ai_poCMCR->GetNodeItemHandleByPosition( iHandle, i ) ;
			iType = ai_poCMCR->GetItemType( iItem ) ;

			// Si no es campo o propiedad no es columna de la relación
			if( iType != M4CL_ITEM_TYPE_FIELD && iType != M4CL_ITEM_TYPE_PROPERTY )
			{
				continue ;
			}

			iItemConnectorHandle = ai_poCMCR->GetItemInverseConnector( iItem ) ;

			// Si no tiene padre no es columna de la relación
			if( iItemConnectorHandle == 0 || iItemConnectorHandle == m4uint32_t( -1 ) )
			{
				continue ;
			}

			iConnector = ai_poCMCR->GetItemConnectorNodeConnectorHandle( iItemConnectorHandle ) ;

			// Si no es del conector no es columna de la relación
			if( iConnector != ai_iHandle )
			{
				continue ;
			}

			iRelation = ai_poCMCR->GetItemConnectorRelation( iItemConnectorHandle ) ;

			// Si no es por igualdad no es columna de la relación
			if( iRelation != M4CL_ITEM_CONNECTOR_EQUALITY )
			{
				continue ;
			}

			iParentItem = ai_poCMCR->GetItemConnectorFatherItemHandle( iItemConnectorHandle ) ;
			iType = ai_poCMCR->GetItemType( iParentItem ) ;

			// Si es método no es columna de la relación
			if( iType == M4CL_ITEM_TYPE_METHOD )
			{
				continue ;
			}

			iResult = _PrintXmlRelationColumn( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iItemConnectorHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	M4MDFAC_PRINT_END( ai_iIndent, "relation" ) ;


// Hijas ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET ) ) != 0 )
	{
		iLast = ai_poCMCR->GetNodeNumberOfRBConnectors( iHandle ) ;
		iLast += ai_poCMCR->GetNodeNumberOfBBConnectors( iHandle ) ;
		iFirst = ai_poCMCR->GetNodeRBFirstConnector( iHandle ) ;

		for( j = iFirst ; j < iLast ; j++ )
		{
			iConnector = ai_poCMCR->GetNodeConnectorHandleByPosition( iHandle, j ) ;

			iResult = _PrintXmlRelation( ai_poCMCR, ai_iIndent, ai_iMask, ao_roXML, ai_pcBuffer, iConnector ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlTiRef( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iTiHandle, m4uint16_t ai_iPosition )
{

	m4uint32_t	iReferences ;
	m4uint32_t	iHandle ;
	m4pcchar_t	pccNode ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "tiref" ) ;


// Propiedades ================================================================

	iReferences = ai_poCMCR->GetTiReferenceNumberByPosition( ai_iTiHandle, ai_iPosition ) ;
	iHandle = ai_poCMCR->GetTiReferenceNodeHandleByPosition( ai_iTiHandle, ai_iPosition ) ;

	if( iHandle != 0 )
	{
		pccNode = ai_poCMCR->GetNodeId( iHandle ) ;
	}
	else
	{
		pccNode = "";
	}

	M4MDFAC_PRINT_TAG( "id",	ai_iPosition,	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ref",	iReferences,	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "nd",	pccNode,		M4XMASK_ALL ) ;

// Final ======================================================================

	M4MDFAC_PRINT_SIMPLE_END ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlTi( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4uint32_t ai_iHandle )
{

	m4return_t	iResult ;
	m4bool_t	bBody ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "ti" ) ;


// Propiedades ================================================================

	M4MDFAC_PRINT_TAG( "id", ai_poCMCR->GetTiId( ai_iHandle ), M4XMASK_ALL ) ;

	bBody = M4_FALSE ;


// Referencias ================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelInheritLevel() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlTiRef( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, ai_iHandle, i ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "ti" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlM4Obj( ClCompiledMCR *ai_poCMCR, m4uint8_t ai_iIndent, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4pchar_t ai_pcBuffer, m4date_t ai_dVersion, m4pcchar_t ai_pccOrganization )
{

	m4return_t	iResult ;
	m4bool_t	bBody ;
	m4bool_t	bModified ;
	m4uint8_t	iIsQBF ;
	m4uint16_t	i ;
	m4uint16_t	j ;
	m4uint16_t	iLength ;
	m4uint16_t	iFirst ;
	m4uint16_t	iLast ;
	m4uint16_t	iPlugs ;
	m4uint16_t	iFilters ;
	m4uint32_t	iHandle ;
	m4uint32_t	iNode ;
	m4uint32_t	iConnector ;
	m4uint32_t	iPlug ;
	m4uint32_t	iFilter ;
	m4date_t	dVersion ;
	m4pchar_t	pcOutput ;
	m4pcchar_t	pccName ;
	m4pcchar_t	pccParentNodeId ;
    m4char_t	acDate[ M4_SECOND_END + 1 ] ;


// Comienzo ===================================================================

	M4MDFAC_PRINT_START( ai_iIndent, "m4obj" ) ;


// Propiedades ================================================================

	pccName = ai_poCMCR->GetChannelName() ;

	if( pccName == NULL )
	{
		pccName = ai_poCMCR->GetChannelId() ;
	}

	iResult = M4ReplaceLiterals( m_poChannelManager, pccName, pcOutput, ai_pccOrganization, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		pccName = pcOutput ;
	}

	dVersion = ai_poCMCR->GetChannelVersion() ;
	
	if( ai_dVersion > dVersion )
	{
		dVersion = ai_dVersion ;
	}

	M4MDFAC_PRINT_TAG( "mk",	ai_iMask,							M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_VERSION | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "id",	ai_poCMCR->GetChannelId(),			M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_VERSION | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "n",		pccName,							M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "lg",	ai_poCMCR->GetChannelLanguage(),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "b",		ai_poCMCR->GetChannelBuild(),		M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "cs",	ai_poCMCR->GetChannelCsType(),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "tu",	ai_poCMCR->GetChannelTimeUnit(),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "v",		M4MdFacDate( acDate, dVersion ),	M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_VERSION | M4XMASK_JSAPI ) ;
	M4MDFAC_PRINT_TAG( "cd",	M4MdFacDate( acDate, ai_poCMCR->GetChannelCreationDate() ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "sd",	M4MdFacDate( acDate, ai_poCMCR->GetChannelStartDate() ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ed",	M4MdFacDate( acDate, ai_poCMCR->GetChannelEndDate() ),		M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cds",	M4MdFacDate( acDate, ai_poCMCR->GetChannelCorStartDate() ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ced",	M4MdFacDate( acDate, ai_poCMCR->GetChannelCorEndDate() ),	M4XMASK_ALL ) ;

	M4MDFAC_PRINT_TAG( "tp",	ai_poCMCR->GetChannelType(),							M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "hs",	ai_poCMCR->GetChannelHasSecurity(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "sp",	ai_poCMCR->GetChannelIsSeparable(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ex",	ai_poCMCR->GetChannelIsExternal(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ch",	ai_poCMCR->GetChannelIsCacheable(),						M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "mch",	ai_poCMCR->GetChannelIsMDCacheable(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "chp",	m4uint32_t( ai_poCMCR->GetChannelCacheMaxPeriod() ),	M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cc",	ai_poCMCR->GetChannelCheckConcurrency(),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ot",	ai_poCMCR->GetChannelOrganizationType(),				M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "il",	ai_poCMCR->GetChannelInheritLevel(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "dl",	ai_poCMCR->GetChannelDataLanguage(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ml",	ai_poCMCR->GetChannelMetaLanguage(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "cet",	ai_poCMCR->GetChannelCsExeType(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "mcp",	ai_poCMCR->GetChannelVMCapacityReq(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "oci",	ai_poCMCR->GetChannelOleClassId(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "si",	ai_poCMCR->GetChannelServiceId(),						M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "sai",	ai_poCMCR->GetChannelServiceAltId(),					M4XMASK_ALL ) ;
	M4MDFAC_PRINT_TAG( "ev",	ai_poCMCR->GetChannelEnvironmentVars(),					M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "of",	ai_poCMCR->GetChannelOwnerFlag(),						M4XMASK_ALL | M4XMASK_NET ) ;
	M4MDFAC_PRINT_TAG( "stl",	ai_poCMCR->GetChannelStyleUse(),						M4XMASK_ALL | M4XMASK_SOAP ) ;
	M4MDFAC_PRINT_TAG( "wss",	ai_poCMCR->GetChannelWSSSecurity(),						M4XMASK_ALL | M4XMASK_SOAP ) ;

	if( pcOutput != NULL )
	{
		delete [] pcOutput ;
	}

	bBody = M4_FALSE ;


// Nodos ======================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfNodes() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelNodeHandleByPosition( i ) ;

			pccParentNodeId = M4MdFacParentNodeId( ai_poCMCR, iHandle, NULL ) ;

			// Si no tiene padres RB o BB lo imprimimos
			if( *pccParentNodeId == '\0' )
			{
				if( bBody == M4_FALSE )
				{
					M4MDFAC_PRINT_CONTINUE ;
					bBody = M4_TRUE ;
				}

				iResult = _PrintXmlNode( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle, "", ai_pccOrganization ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}


// Tis ========================================================================

	if( ( ai_iMask & ( M4XMASK_ALL ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfTis() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelTiHandleByPosition( i ) ;

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlTi( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Relaciones =================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfNodes() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelNodeHandleByPosition( i ) ;

			pccParentNodeId = M4MdFacParentNodeId( ai_poCMCR, iHandle, NULL ) ;

			// Si no tiene padres RB o BB lo imprimimos
			if( *pccParentNodeId == '\0' )
			{
				iLast = ai_poCMCR->GetNodeNumberOfRBConnectors( iHandle ) ;
				iLast += ai_poCMCR->GetNodeNumberOfBBConnectors( iHandle ) ;
				iFirst = ai_poCMCR->GetNodeRBFirstConnector( iHandle ) ;

				for( j = iFirst ; j < iLast ; j++ )
				{
					iConnector = ai_poCMCR->GetNodeConnectorHandleByPosition( iHandle, j ) ;

					if( bBody == M4_FALSE )
					{
						M4MDFAC_PRINT_CONTINUE ;
						bBody = M4_TRUE ;
					}

					iResult = _PrintXmlRelation( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iConnector ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}


// Métodos de negocio =========================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_SOAP | M4XMASK_NET ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfBusinessMethods() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelBusinessMethodHandleByPosition( i ) ;

			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlBusinessMethod( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		// Método ROOTLOAD o SYS_LOAD

		// Primero se busca el SYS_LOAD
		iHandle = ai_poCMCR->GetChannelSysLoadItem() ;

		// Si no existe se pone el rootload del primer nodo raiz que no sea qbf
		if( iHandle == 0 )
		{
			iLength = ai_poCMCR->GetChannelNumberOfRoots() ;

			for( i = 0 ; i < iLength && iHandle == 0 ; i++ )
			{
				iNode = ai_poCMCR->GetChannelRootHandleByPosition( i ) ;
				iIsQBF = ai_poCMCR->GetNodeIsQBFFilter( iNode ) ;

				if( iIsQBF == 0 )
				{
					iHandle = ai_poCMCR->GetNodeItemHandleById( iNode, M4CL_ITEM_ROOT_LOAD ) ;
				}
			}
		}

		if( iHandle != 0 )
		{
			if( bBody == M4_FALSE )
			{
				M4MDFAC_PRINT_CONTINUE ;
				bBody = M4_TRUE ;
			}

			iResult = _PrintXmlBusinessMethod( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, "M4LoadObject", iHandle ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Listas =====================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET | M4XMASK_JSAPI ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfNodes() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelNodeHandleByPosition( i ) ;

			pccParentNodeId = M4MdFacParentNodeId( ai_poCMCR, iHandle, NULL ) ;

			// Si no tiene padres RB o BB lo imprimimos
			if( *pccParentNodeId == '\0' )
			{
				iPlugs = ai_poCMCR->GetNodeNumberOfPlugs( iHandle ) ;

				for( j = 0 ; j < iPlugs ; j++ )
				{
					iPlug = ai_poCMCR->GetNodePlugHandleByPosition( iHandle, j ) ;

					if( bBody == M4_FALSE )
					{
						M4MDFAC_PRINT_CONTINUE ;
						bBody = M4_TRUE ;
					}

					iResult = _PrintXmlList( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iPlug, iHandle, ai_pccOrganization ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}


// Filtros =====================================================================

	if( ( ai_iMask & ( M4XMASK_ALL | M4XMASK_NET ) ) != 0 )
	{
		iLength = ai_poCMCR->GetChannelNumberOfNodes() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iHandle = ai_poCMCR->GetChannelNodeHandleByPosition( i ) ;
			iFilters = ai_poCMCR->GetNodeNumberOfQBFFilters( iHandle ) ;

			for( j = 0 ; j < iFilters ; j++ )
			{
				iFilter = ai_poCMCR->GetNodeQBFFilterHandleByPosition( iHandle, j ) ;

				if( bBody == M4_FALSE )
				{
					M4MDFAC_PRINT_CONTINUE ;
					bBody = M4_TRUE ;
				}

				iResult = _PrintXmlFilter( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iFilter, M4CL_FILTER_TYPE_QBF, iHandle ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}

			iFilters = ai_poCMCR->GetNodeNumberOfFindFilters( iHandle ) ;

			for( j = 0 ; j < iFilters ; j++ )
			{
				iFilter = ai_poCMCR->GetNodeFindFilterHandleByPosition( iHandle, j ) ;

				if( bBody == M4_FALSE )
				{
					M4MDFAC_PRINT_CONTINUE ;
					bBody = M4_TRUE ;
				}

				iResult = _PrintXmlFilter( ai_poCMCR, ai_iIndent + 1, ai_iMask, ao_roXML, ai_pcBuffer, iFilter, M4CL_FILTER_TYPE_FIND, iHandle ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}


// Final ======================================================================

	if( bBody == M4_TRUE )
	{
		M4MDFAC_PRINT_END( ai_iIndent, "m4obj" ) ;
	}
	else
	{
		M4MDFAC_PRINT_SIMPLE_END ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClFactoryImp :: _PrintXmlHeader( ClCompiledMCR *ai_poCMCR, ClLongString &ao_roXML )
{

    m4return_t	iResult ;


	// UNICODE XML
	iResult = ao_roXML.Concat( M4XMLHeader() ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_XML, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;

	return( M4_SUCCESS ) ;
}




//=================================================================================
// Genera un metacanal compilado en formato xml a partir de su modelo en base de datos
//=================================================================================
m4return_t  ClFactoryImp :: MDFac_BuildXML( ClCompiledMCR *ai_poCMCR, m4pcchar_t ai_pccOrganization, m4uint32_t ai_iMask, ClLongString &ao_roXML, m4bool_t ai_bGenHeader )
{
	
	m4return_t	iResult ;
	m4char_t	acBuffer[ M4MDFAC_XML_BUFFER_SIZE ] ;
	m4date_t	dVersion ;


// Cabecera ===================================================================

	if( ai_bGenHeader == M4_TRUE )
	{
		iResult = _PrintXmlHeader( ai_poCMCR, ao_roXML ) ;
		
		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


// M4Obj ======================================================================

	dVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;

	iResult = _PrintXmlM4Obj( ai_poCMCR, 0, ai_iMask, ao_roXML, acBuffer, dVersion, ai_pccOrganization ) ;

	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_CREATE_XML, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;


    return( M4_SUCCESS ) ;
}

