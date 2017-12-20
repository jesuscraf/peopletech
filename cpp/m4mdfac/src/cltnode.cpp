
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// Module:              m4mdfac.dll
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                11-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions of the temporary node
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltnode.hpp"
#include "cltt3inh.hpp"
#include "cltti.hpp"
#include "cltnitem.hpp"
#include "cltconn.hpp"
#include "cltfilt.hpp"
#include "cltchann.hpp"
#include "clttrig.hpp"
#include "cltnicon.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "chlog.hpp"




//=============================================================================
// Funciones de comparacion
//=============================================================================

int M4ClCompareDmdsById( m4pcvoid_t ai_pcvDmd1, m4pcvoid_t ai_pcvDmd2 )
{

	int			iResult ;
	ClTempItem	*poItem1 = ( *( ClTempNitem** ) ai_pcvDmd1 )->GetItem() ;
	ClTempItem	*poItem2 = ( *( ClTempNitem** ) ai_pcvDmd2 )->GetItem() ;


	m4pcchar_t	pccString1 = poItem1->GetDmdComponentId() - 1 ;
	m4pcchar_t	pccString2 = poItem2->GetDmdComponentId() - 1 ;

	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}
	
	m4uchar_t uiSize1 = *pccString1 ;

    iResult = memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	pccString1 = poItem1->GetDmdFieldId() - 1 ;
	pccString2 = poItem2->GetDmdFieldId() - 1 ;

	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}
	
	uiSize1 = *pccString1 ;
    iResult = memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	pccString1 = poItem1->GetDmdId() - 1 ;
	pccString2 = poItem2->GetDmdId() - 1 ;

	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}

	uiSize1 = *pccString1 ;
	return( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ) ;
}





//=================================================================================
//
// ClTempNode
//
// Clase que implementa la definición de los nodos
//
//=================================================================================

ClTempNode :: ClTempNode( void )
{

    m4uint16_t  i ;


	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iNodeId = 0 ;
    m_iName =  0;
	m_iGroupObjects = 0 ;
    m_iNumRows = 0 ;
	m_iNumKeepRows = 0 ;
	m_iNumRowsDB = 0 ;
	m_iOleDispId = 0 ;
	m_iOleNodeInterId = 0 ;
	m_iMaxNumBlocks = 0;
    m_iMaxNumRecords = 0 ;

    m_iConnectorsStart = 0 ;
    m_iInvConnectorsStart = 0 ;
    m_iGroupsStart = 0 ;
    m_iPlugsStart = 0 ;
    m_iIndexesStart = 0 ;
    m_iAliasStart = 0 ;
	m_iT3AliasStart = 0 ;
	m_iDmdsStart = 0 ;
	m_iQBFFiltersStart = 0 ;
	m_iFindFiltersStart = 0 ;
	m_iSysSentencesStart = 0 ;
	m_iSysHintsStart = 0 ;

	m_iSCRStart = 0 ;
    m_iIndex = 0 ;
	m_iType = 0 ;
	m_iOrder = 0 ;

    m_iCapacity = M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK ;
    m_iIsRoot = 0 ;
	m_iIsRealRoot = 0 ;
    m_iAutoload = 0 ;
    m_iIsOrdered = 0 ;
    m_iIsVisible = 1 ;
    m_iDynFilter = 0 ;
	m_iIdAutoFilter = 0 ;
	m_iCsType = M4CL_CSTYPE_MAPPED ;
	m_iSyncType = M4_SYNC_DOCUMENT ;
	m_iIsExternal = 0 ;
	m_iIsQBFFilter = 0 ;
	m_iAffectsDB = 0 ;
	m_iDBReload = 0 ;

    m_iNodeItems = 0 ;
    m_iBlockItems = 0 ;
    m_iRegisterItems = 0 ;

    m_poStartDate = NULL ;
    m_poEndDate = NULL ;
    m_poCorStartDate = NULL ;
    m_poCorEndDate = NULL ;
    m_poProjection = NULL ;
    m_poFilterStartDate = NULL ;
    m_poFilterEndDate = NULL ;
    m_poFilterCorStartDate = NULL ;
    m_poFilterCorEndDate = NULL ;
    m_poCurrencyType = NULL ;
    m_poCurrencyExchangeDate = NULL ;
    m_poDmdChangeReason = NULL ;
    m_poDmdComponent = NULL ;
    m_poDmdValue = NULL ;
    m_poLoadSQL = NULL ;
    m_poPriority = NULL ;
    m_poImputeDate = NULL ;
    m_poPayDate = NULL ;
    m_poPayType = NULL ;
    m_poPayFrequency = NULL ;
    m_poImputePayType = NULL ;
    m_poImputePayFrequency = NULL ;
	m_poClosingDate = NULL ;
	m_poSysParam = NULL ;
	m_poBDLChecks = NULL ;
	m_poRAMOrderBy = NULL ;
	m_poOrganization = NULL ;

	m_poLoadBlk = NULL ;
	m_poDeleteBlk = NULL ;
	m_poUpdateBlk = NULL ;
	m_poInsertBlk = NULL ;

	m_iLevel = 0 ;
    m_iItemsInGroups = 0 ;
    m_iConnectedDegree = 0 ;

	m_poBackPayNitem = NULL ;
	m_poRevNitem = NULL ;
	m_poRevNode = NULL ;

    m_poDefaultIndex = NULL ;

    m_poChannel = NULL ;
    m_poTi = NULL ;

    for( i = 0 ; i < M4CL_NODE_CONNECTOR_MAX_TYPE ; i++ )
    {
        m_iConnectedNumber[ i ] = 0 ;
        m_iConnectedStart[ i ] = 0 ;
    }

    m_poComparisonNitem = &m_oComparisonNitem ;
    m_poComparisonAlias = &m_oComparisonAlias ;
    m_poComparisonT3Alias = &m_oComparisonT3Alias ;
    m_poComparisonGroup = &m_oComparisonGroup ;

	m_iIsToCompile = 1 ;
	m_iConnectorsToCompile = 0 ;
	m_iInvConnectorsToCompile = 0 ;
	m_iItemsToCompile = 0 ;
	m_iSysSentencesToCompile = 0 ;
	m_iSysHintsToCompile = 0 ;
	m_iAliasToCompile = 0 ;

	m_iQBFFilters = 0 ;
	m_iFindFilters = 0 ;
}



ClTempNode :: ~ClTempNode( void )
{
    Delete() ;
}


m4return_t	ClTempNode :: Delete( void )
{
    
    m4return_t	    iResult ;
	m4uint16_t	    i ;
	m4uint16_t		iLength ;
	m4pvoid_t		pvPointer ;
    ClTempGroup		*poGroup ;


	iLength = m_oGroups.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oGroups.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poGroup = ( ClTempGroup* ) pvPointer ;
		delete( poGroup ) ;
	}
    
    return( M4_SUCCESS ) ;
}




// Otras funciones de lectura ================================================

m4pcchar_t	ClTempNode :: GetTiId( void ) const
{
	return( m_poTi->GetTiId() ) ;
}


m4uint32_t	ClTempNode :: GetTiIdHandle( void ) const
{
	return( m_poTi->GetTiIdHandle() ) ;
}


ClTempNitem*	ClTempNode :: GetRevNitem( void ) const
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	ClTempNode	*poNode ;
	ClTempNitem	*poRevNitem ;


	if( m_poRevNitem != NULL )
	{
		return( m_poRevNitem ) ;
	}

	// Si no tienen item de revisión es el nodo de pagas
	// que no tiene estructura de acumulado y damos el primero
	// que encontramos en el canal

	iLength = m_poChannel->GetNumberOfNodes() ;

	poRevNitem = NULL ;
	for( i = 0 ; i < iLength && poRevNitem == NULL ; i++ )
	{
		poNode = m_poChannel->GetNodeByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poNode == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRevNitem = poNode->m_poRevNitem ;
	}

	return( poRevNitem ) ;
}


ClTempNode*		ClTempNode :: GetRevNode( void ) const
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	ClTempNode	*poNode ;
	ClTempNode	*poRevNode ;


	if( m_poRevNode != NULL )
	{
		return( m_poRevNode ) ;
	}

	// Si no tienen nodo de revisión es el nodo de pagas
	// que no tiene estructura de acumulado y damos el primero
	// que encontramos en el canal

	iLength = m_poChannel->GetNumberOfNodes() ;

	poRevNode = NULL ;
	for( i = 0 ; i < iLength && poRevNode == NULL ; i++ )
	{
		poNode = m_poChannel->GetNodeByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poNode == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRevNode = poNode->m_poRevNode ;
	}

	return( poRevNode ) ;
}



ClTempNodeInherit*   ClTempNode :: GetInheritByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			pvElement ;
    ClTempNodeInherit	*poResult ;


	iResult = m_oInherits.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNodeInherit* ) pvElement ;

	return( poResult ) ;
}


ClTempNitem*   ClTempNode :: GetNitemByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempNitem     *poResult ;


	iResult = m_oNitems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}


ClTempConnector*    ClTempNode :: GetConnectorByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempConnector *poResult ;


	iResult = m_oConnectors.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempConnector* ) pvElement ;

	return( poResult ) ;
}



ClTempConnector*    ClTempNode :: GetInvConnectorByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempConnector *poResult ;


	iResult = m_oInvConnectors.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempConnector* ) pvElement ;

	return( poResult ) ;
}


ClTempIndex*    ClTempNode :: GetIndexByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempIndex     *poResult ;


	iResult = m_oIndexes.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempIndex* ) pvElement ;

	return( poResult ) ;
}



ClTempPlug*	ClTempNode :: GetPlugByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempPlug		*poResult ;


	iResult = m_oPlugs.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempPlug* ) pvElement ;

	return( poResult ) ;
}



ClTempNode*   ClTempNode :: GetAliasByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempAlias		*poResult ;


	iResult = m_oAlias.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempAlias* ) pvElement ;

	return( poResult->GetUsedNode() ) ;
}



ClTempT3Alias*   ClTempNode :: GetT3AliasByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempT3Alias   *poResult ;


	iResult = m_oT3Alias.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempT3Alias* ) pvElement ;

	return( poResult ) ;
}



ClTempFilter*    ClTempNode :: GetFilterByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempFilter    *poResult ;


	iResult = m_oFilters.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempFilter* ) pvElement ;

	return( poResult ) ;
}



ClTempNitem*	ClTempNode :: GetSysSentenceByPosition( m4uint16_t ai_iSysSentencePosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempNitem		*poResult ;


	iResult = m_oSysSentences.GetElement( ai_iSysSentencePosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}



ClTempNitem*	ClTempNode :: GetSysHintByPosition( m4uint16_t ai_iSysHintPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempNitem		*poResult ;


	iResult = m_oSysHints.GetElement( ai_iSysHintPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}



ClTempNitem*   ClTempNode :: GetNitemById( m4pcchar_t ai_pccNitemId, m4bool_t ai_bConvert )
{

    ClTempNitem    **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccNitemId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonNitem.SetIdentifier( ai_pccNitemId, ai_bConvert ) ;

	ppoResult = ( ClTempNitem** ) m_oNitems.Find( M4ClCompareObjectsById, &m_poComparisonNitem ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

	return( *ppoResult ) ;
}



ClTempNode* ClTempNode :: GetAliasById( m4pcchar_t ai_pccAliasId, m4bool_t ai_bConvert )
{

    ClTempAlias **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccAliasId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonAlias.SetIdentifier( ai_pccAliasId, ai_bConvert ) ;

	ppoResult = ( ClTempAlias** ) m_oAlias.Find( M4ClCompareObjectsById, &m_poComparisonAlias ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

	return( ( *ppoResult )->GetUsedNode() ) ;
}



ClTempT3Alias*  ClTempNode :: GetT3AliasById( m4pcchar_t ai_pccT3AliasId, m4bool_t ai_bConvert )
{

    ClTempT3Alias **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccT3AliasId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonT3Alias.SetIdentifier( ai_pccT3AliasId, ai_bConvert ) ;

	ppoResult = ( ClTempT3Alias** ) m_oT3Alias.Find( M4ClCompareObjectsById, &m_poComparisonT3Alias ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

	return( *ppoResult ) ;
}



ClTempGroup*    ClTempNode :: GetGroupById( m4pcchar_t ai_pccGroupId, m4bool_t ai_bConvert )
{

    ClTempGroup **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccGroupId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonGroup.SetIdentifier( ai_pccGroupId, ai_bConvert ) ;

	ppoResult = ( ClTempGroup** ) m_oGroups.Find( M4ClCompareObjectsById, &m_poComparisonGroup ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

	return( *ppoResult ) ;
}



ClTempConnector*    ClTempNode :: GetConnectorBySonNode( ClTempNode *ai_poNode )
{

    m4return_t		iResult ;
	m4uint16_t		iFound ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pvoid_t       pvPointer ;
    ClTempNode      *poSonNode ;
    ClTempConnector *poConnector ;


	CHECK_CHLOG_DEBUGF( ai_poNode == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iLength = m_oConnectors.GetLength() ;

    iFound = 0 ;
	for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;

        poSonNode = poConnector->GetSonNode() ;
		CHECK_CHLOG_DEBUGF( poSonNode == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        if( poSonNode == ai_poNode )
        {
            iFound = 1 ;
        }
	}

    if( iFound == 0 )
    {
        return( NULL ) ;
    }

	return( poConnector ) ;
}





// Otras funciones de escritura ===============================================

void    ClTempNode :: SetTi( ClTempTi *ai_poTi )
{
	if( m_poTi != ai_poTi )
	{
		m_poTi = ai_poTi ;
	}
}


m4return_t	ClTempNode :: AddGroup( ClTempGroup* ai_poGroup )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poGroup == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = m_oGroups.AddElement( ai_poGroup ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



// Funciones de impresión =====================================================

m4pcchar_t	ClTempNode :: iNode( void ) const
{
	return( GetNodeId() ) ;
}


m4pcchar_t	ClTempNode :: nNode( void ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetName() ;

	if( *pccResult == '\0' )
	{
		pccResult = GetNodeId() ;
	}

	return( pccResult ) ;
}


m4pcchar_t	ClTempNode :: iM4Obj( void ) const
{
	if( m_poChannel == NULL )
	{
		return( "" ) ;
	}

	return( m_poChannel->GetChannelId() ) ;
}


m4pcchar_t	ClTempNode :: nM4Obj( void ) const
{

	m4pcchar_t	pccResult ;


	if( m_poChannel == NULL )
	{
		return( "" ) ;
	}

	pccResult = m_poChannel->GetName() ;

	if( *pccResult == '\0' )
	{
		pccResult = m_poChannel->GetChannelId() ;
	}

	return( pccResult ) ;
}




// Funciones de serialización =================================================

m4return_t	ClTempNode :: PreGetSize( void )
{

    m4return_t		iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pcchar_t		pccWriteObject ;
	m4pvoid_t		pvPointer ;
	ClTempRitem		*poRitem ;
	ClTempNitem		*poNitem ;
 

    iResult = CalculateItemTypes() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	
    iResult = CalculateDmds() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	/* Bug 0070821
	Hay que calcular el AffectsDB después del cálculo de herencia.
	*/
	if( m_iAffectsDB == 0 )
	{
		pccWriteObject = m_poTi->GetWriteObjectId() ;

		if( pccWriteObject != NULL && *pccWriteObject != '\0' )
		{
			m_iAffectsDB = 1 ;
		}
	}


	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNitem->PreGetSize() ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
    		}
		}
		
		poRitem = poNitem->GetRitem() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNode :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

	m4return_t		iResult ;
	m4uint8_t		iType ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pvoid_t		pvPointer ;
	ClTempNitem     *poNodeNitem ;
    ClTempGroup		*poGroup ;
    ClTempPlug		*poPlug ;
    ClTempConnector *poConnector ;
    ClTempIndex     *poNodeIndex ;
    ClTempFilter	*poFilter ;


// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_NODE_FIXED_SIZE ;


// Variables ==================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iItemsToCompile * M4CL_MCR_NODE_NITEM_SIZE ;


	// Siempre hay conectores (al menos al auto)
	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iConnectorsStart = ao_iSize ;
	ao_iSize += M4CL_MCR_NODE_NODE_CONNECTORS_FIXED_SIZE ;  // medio nop

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iConnectorsToCompile * M4_SIZE_OF_INT32 ;


	iLength = m_oInvConnectors.GetLength() ;

	m_iInvConnectorsToCompile = 0 ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oInvConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;
		iIsToCompile = poConnector->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			m_iInvConnectorsToCompile++ ;
		}
	}

	if( m_iInvConnectorsToCompile != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iInvConnectorsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iInvConnectorsToCompile * M4_SIZE_OF_INT32 ;
	}
	else
	{
		m_iInvConnectorsStart = 0 ;
	}


    iLength = m_oGroups.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iGroupsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_NODE_GROUP_SIZE ;
	}
	else
	{
		m_iGroupsStart = 0 ;
	}


    iLength = m_oPlugs.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iPlugsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_NODE_PLUG_SIZE ;
	}
	else
	{
		m_iPlugsStart = 0 ;
	}


    iLength = m_oIndexes.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iIndexesStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_NODE_INDEX_SIZE ;
	}
	else
	{
		m_iIndexesStart = 0 ;
	}


	if( m_iAliasToCompile != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iAliasStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iAliasToCompile * M4CL_MCR_NODE_ALIAS_SIZE ;
	}
	else
	{
		m_iAliasStart = 0 ;
	}


    iLength = m_oT3Alias.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iT3AliasStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_NODE_T3_ALIAS_SIZE ;
	}
	else
	{
		m_iT3AliasStart = 0 ;
	}


    iLength = m_oDmds.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iDmdsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_NODE_DMD_SIZE ;
	}
	else
	{
		m_iDmdsStart = 0 ;
	}


	if( m_iQBFFilters != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iQBFFiltersStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iQBFFilters * M4CL_MCR_NODE_FILTER_SIZE ;
	}
	else
	{
		m_iQBFFiltersStart = 0 ;
	}


	if( m_iFindFilters != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iFindFiltersStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iFindFilters * M4CL_MCR_NODE_FILTER_SIZE ;
	}
	else
	{
		m_iFindFiltersStart = 0 ;
	}


	if( m_iSysSentencesToCompile != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iSysSentencesStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iSysSentencesToCompile * M4CL_MCR_NODE_SYS_SENTENCE_SIZE ;
	}
	else
	{
		m_iSysSentencesStart = 0 ;
	}


	if( m_iSysHintsToCompile != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iSysHintsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += m_iSysHintsToCompile * M4CL_MCR_NODE_SYS_HINT_SIZE ;
	}
	else
	{
		m_iSysHintsStart = 0 ;
	}


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_NODE, ao_iSize - m_iHandle ) ;
	}


// Componentes ================================================================

	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNodeNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNodeNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNodeNitem->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
        }
	}


    iLength = m_oGroups.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oGroups.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poGroup = ( ClTempGroup* ) pvPointer ;
		iResult = poGroup->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	iLength = m_oPlugs.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oPlugs.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poPlug = ( ClTempPlug* ) pvPointer ;
        iResult = poPlug->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;

		iIsToCompile = poConnector->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poConnector->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    iLength = m_oIndexes.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oIndexes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNodeIndex = ( ClTempIndex* ) pvPointer ;
        iResult = poNodeIndex->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    iLength = m_oFilters.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFilters.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFilter = ( ClTempFilter* ) pvPointer ;

		iType = poFilter->GetType() ;

		if( iType == M4CL_FILTER_TYPE_QBF )
		{
			iResult = poFilter->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    iLength = m_oFilters.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFilters.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFilter = ( ClTempFilter* ) pvPointer ;

		iType = poFilter->GetType() ;

		if( iType == M4CL_FILTER_TYPE_FIND )
		{
			iResult = poFilter->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}


m4return_t	ClTempNode :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

	m4return_t		iResult ;
	m4uint8_t		iType ;
	m4uint8_t		iIsToCompile ;
	m4uint8_t		iIsOverWritten ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4uint16_t		iInherits ;
	m4uint32_t		iOffset ;
	m4uint32_t		iHandle ;
	m4pvoid_t       pvPointer ;
	ClTempT3Inherit	*poT3Inherit ;
	ClTempItem	    *poItem ;
	ClTempNitem	    *poNitem ;
    ClTempGroup		*poGroup ;
    ClTempPlug		*poPlug ;
    ClTempConnector *poConnector ;
    ClTempIndex     *poNodeIndex ;
    ClTempAlias		*poAlias ;
	ClTempT3Alias	*poT3Alias ;
    ClTempFilter	*poFilter ;



    iOffset = m_iHandle ;

// fijos

    iHandle = m_poTi->GetHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iNodeId + ai_iBufferSize ) ;

	poT3Inherit = m_poChannel->GetT3InheritByPosition( m_iLevel ) ;
	iHandle = poT3Inherit->GetBaseT3IdHandle() ;

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iName + ai_iBufferSize + ai_iPoolSize ) ;

    iHandle = m_poTi->GetTiIdHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iGroupObjects + ai_iBufferSize ) ;


    if( m_poStartDate != NULL )
    {
        iHandle = m_poStartDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
    
    if( m_poEndDate != NULL )
    {
        iHandle = m_poEndDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poCorStartDate != NULL )
    {
        iHandle = m_poCorStartDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
    
    if( m_poCorEndDate != NULL )
    {
        iHandle = m_poCorEndDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poProjection != NULL )
    {
        iHandle = m_poProjection->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poFilterStartDate != NULL )
    {
        iHandle = m_poFilterStartDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poFilterEndDate != NULL )
    {
        iHandle = m_poFilterEndDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poFilterCorStartDate != NULL )
    {
        iHandle = m_poFilterCorStartDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poFilterCorEndDate != NULL )
    {
        iHandle = m_poFilterCorEndDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poCurrencyType != NULL )
    {
        iHandle = m_poCurrencyType->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poCurrencyExchangeDate != NULL )
    {
        iHandle = m_poCurrencyExchangeDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poDmdChangeReason != NULL )
    {
        iHandle = m_poDmdChangeReason->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poDmdComponent != NULL )
    {
        iHandle = m_poDmdComponent->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poDmdValue != NULL )
    {
        iHandle = m_poDmdValue->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poLoadSQL != NULL )
    {
        iHandle = m_poLoadSQL->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poPriority != NULL )
    {
        iHandle = m_poPriority->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poImputeDate != NULL )
    {
        iHandle = m_poImputeDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poPayDate != NULL )
    {
        iHandle = m_poPayDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poPayType != NULL )
    {
        iHandle = m_poPayType->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poPayFrequency != NULL )
    {
        iHandle = m_poPayFrequency->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poImputePayType != NULL )
    {
        iHandle = m_poImputePayType->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poImputePayFrequency != NULL )
    {
        iHandle = m_poImputePayFrequency->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poClosingDate != NULL )
    {
        iHandle = m_poClosingDate->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poSysParam != NULL )
    {
        iHandle = m_poSysParam->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poBDLChecks != NULL )
    {
        iHandle = m_poBDLChecks->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poRAMOrderBy != NULL )
    {
        iHandle = m_poRAMOrderBy->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poOrganization != NULL )
    {
        iHandle = m_poOrganization->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

	

    if( m_poLoadBlk != NULL )
    {
        iHandle = m_poLoadBlk->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poDeleteBlk != NULL )
    {
        iHandle = m_poDeleteBlk->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poUpdateBlk != NULL )
    {
        iHandle = m_poUpdateBlk->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }


	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poInsertBlk != NULL )
    {
        iHandle = m_poInsertBlk->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    if( m_poDefaultIndex != NULL )
    {
        iHandle = m_poDefaultIndex->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

    M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt32( ai_pcBuffer, iOffset, m_iNumRows ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iNumKeepRows ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iNumRowsDB ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iOleDispId ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iOleNodeInterId + ai_iBufferSize ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iMaxNumBlocks ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iMaxNumRecords ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iConnectorsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iInvConnectorsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iGroupsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iPlugsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iIndexesStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iAliasStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iT3AliasStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDmdsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iQBFFiltersStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iFindFiltersStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSysSentencesStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSysHintsStart ) ;

	M4ClPackInt16( ai_pcBuffer, iOffset, m_iSCRStart ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iIndex ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iType ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iOrder ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iCapacity ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsRoot ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsRealRoot ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iAutoload ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsOrdered ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsVisible ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iDynFilter ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIdAutoFilter ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCsType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iSyncType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsExternal ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsQBFFilter ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iAffectsDB ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iDBReload ) ;

	iInherits = m_poTi->GetNumberOfInherits() ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m4uint8_t( iInherits ) ) ;

	if( m_oInherits.GetLength() > 1 )
	{
		iIsOverWritten = 1 ;
	}
	else
	{
		iIsOverWritten = 0 ;
	}
	M4ClPackInt8( ai_pcBuffer, iOffset, iIsOverWritten ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iItemsToCompile ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iNodeItems ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iBlockItems ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iRegisterItems ) ;

// variables

    iOffset = M4_ROUND_TO_4( iOffset ) ;

	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poNitem->GetNitemIdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;
        
			iHandle = poNitem->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


	iLength = m_oConnectors.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectorsToCompile - 1 ) ;	// El 1 es de los autos
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedNumber[ 0 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedNumber[ 1 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedNumber[ 2 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedStart[ 0 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedStart[ 1 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectedStart[ 2 ] ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, 1 ) ; 	// El 1 es de los autos

	iOffset = M4_ROUND_TO_4( iOffset ) ;

	// El auto va al final
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;
		iIsToCompile = poConnector->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poConnector->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

// inverse connectors

	if( m_iInvConnectorsToCompile != 0 )
	{
		iLength = m_oInvConnectors.GetLength() ;

		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iInvConnectorsToCompile ) ;

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oInvConnectors.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poConnector = ( ClTempConnector* ) pvPointer ;
			iIsToCompile = poConnector->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iHandle = poConnector->GetHandle() ;

				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset ) ; // relativo
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}

// groups

	iLength = m_oGroups.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oGroups.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poGroup = ( ClTempGroup* ) pvPointer ;

			iHandle = poGroup->GetGroupIdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poGroup->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

    
// plugs

	iLength = m_oPlugs.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oPlugs.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poPlug = ( ClTempPlug* ) pvPointer ;

			iHandle = poPlug->GetUsedT3IdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poPlug->GetUsedNodeIdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poPlug->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

    
// index

	iLength = m_oIndexes.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oIndexes.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNodeIndex = ( ClTempIndex* ) pvPointer ;

			iHandle = poNodeIndex->GetIndexId() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

			iHandle = poNodeIndex->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

    
// alias

	iLength = m_oAlias.GetLength() ;

	if( m_iAliasToCompile != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iAliasToCompile ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oAlias.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poAlias = ( ClTempAlias* ) pvPointer ;
			iIsToCompile = poAlias->GetUsedNodeIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iHandle = poAlias->GetAliasIdHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

				iHandle = poAlias->GetUsedNodeHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}


// t3 alias

	iLength = m_oT3Alias.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oT3Alias.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poT3Alias = ( ClTempT3Alias* ) pvPointer ;

			iHandle = poT3Alias->GetT3AliasIdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poT3Alias->GetInstanceIdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

// dmds

	iLength = m_oDmds.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oDmds.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			poItem = poNitem->GetItem() ;

			iHandle = poItem->GetDmdComponentHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poItem->GetDmdFieldHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poItem->GetDmdHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

			iHandle = poNitem->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


// filters

	iLength = m_oFilters.GetLength() ;

	if( m_iQBFFilters != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iQBFFilters ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oFilters.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poFilter = ( ClTempFilter* ) pvPointer ;

			iType = poFilter->GetType() ;

			if( iType == M4CL_FILTER_TYPE_QBF )
			{
				iHandle = poFilter->GetFilterIdHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

				iHandle = poFilter->GetHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}


	if( m_iFindFilters != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iFindFilters ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oFilters.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poFilter = ( ClTempFilter* ) pvPointer ;

			iType = poFilter->GetType() ;

			if( iType == M4CL_FILTER_TYPE_FIND )
			{
				iHandle = poFilter->GetFilterIdHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle - iOffset + ai_iBufferSize ) ;

				iHandle = poFilter->GetHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}


// sys sentences

	iLength = m_oSysSentences.GetLength() ;

	if( m_iSysSentencesToCompile != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iSysSentencesToCompile ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oSysSentences.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			iIsToCompile = poNitem->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iHandle = poNitem->GetHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}


// sys hints

	iLength = m_oSysHints.GetLength() ;

	if( m_iSysHintsToCompile != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iSysHintsToCompile ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oSysHints.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			iIsToCompile = poNitem->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iHandle = poNitem->GetHandle() ;
				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
				iOffset = M4_ROUND_TO_4( iOffset ) ;
			}
		}
	}


// componentes

	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNitem->Compile( m_poChannel, ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
    		}
		}
	}


    iLength = m_oGroups.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oGroups.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poGroup = ( ClTempGroup* ) pvPointer ;
		iResult = poGroup->Compile( ai_pcBuffer, ai_iBufferSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	iLength = m_oPlugs.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oPlugs.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poPlug = ( ClTempPlug* ) pvPointer ;
        iResult = poPlug->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
    	}
	}

    
    iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;

		iIsToCompile = poConnector->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poConnector->Compile( ai_pcBuffer, ai_iBufferSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
    		}
		}
	}


    iLength = m_oIndexes.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oIndexes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNodeIndex = ( ClTempIndex* ) pvPointer ;
        iResult = poNodeIndex->Compile( ai_pcBuffer, ai_iBufferSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    iLength = m_oFilters.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFilters.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFilter = ( ClTempFilter* ) pvPointer ;

		iType = poFilter->GetType() ;

		if( iType == M4CL_FILTER_TYPE_QBF )
		{
			iResult = poFilter->Compile( ai_pcBuffer, ai_iBufferSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFilters.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFilter = ( ClTempFilter* ) pvPointer ;

		iType = poFilter->GetType() ;

		if( iType == M4CL_FILTER_TYPE_FIND )
		{
			iResult = poFilter->Compile( ai_pcBuffer, ai_iBufferSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}




// Funciones de referencia ====================================================


m4return_t	ClTempNode :: AddNitem( ClTempNitem* ai_poNitem )
{

	m4return_t	    iResult ;
	m4uint8_t		iFound ;
	m4uint8_t		iType ;
    m4uint8_t       iInternalType ;
	m4pcchar_t		pccItemId ;
	ClTempItem		*poItem ;


	CHECK_CHLOG_DEBUGF( ai_poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem = ai_poNitem->GetItem() ;

    if( ai_poNitem->IsInGroup() == 1 )
    {
    	m_iItemsInGroups++ ;
    }


	pccItemId = poItem->GetItemId() ;
    iType = poItem->GetType() ;


	iFound = 0 ;

	if( iFound == 0 && m_poLoadBlk == NULL && iType == M4CL_ITEM_TYPE_METHOD )
	{
		if( strcmp( pccItemId, M4CL_ITEM_LOAD_BLK ) == 0 )
		{
			if( iType == M4CL_ITEM_TYPE_METHOD )
			{
				m_poLoadBlk = ai_poNitem ;
				iFound = 1 ;
			}
		}
	}

	if( iFound == 0 && m_poDeleteBlk == NULL && iType == M4CL_ITEM_TYPE_METHOD )
	{
		if( strcmp( pccItemId, M4CL_ITEM_DELETE_BLK ) == 0 )
		{
			if( iType == M4CL_ITEM_TYPE_METHOD )
			{
				m_poDeleteBlk = ai_poNitem ;
				iFound = 1 ;
			}
		}
	}

	if( iFound == 0 && m_poUpdateBlk == NULL && iType == M4CL_ITEM_TYPE_METHOD )
	{
		if( strcmp( pccItemId, M4CL_ITEM_UPDATE_BLK ) == 0 )
		{
			if( iType == M4CL_ITEM_TYPE_METHOD )
			{
				m_poUpdateBlk = ai_poNitem ;
				iFound = 1 ;
			}
		}
	}

	if( iFound == 0 && m_poInsertBlk == NULL && iType == M4CL_ITEM_TYPE_METHOD )
	{
		if( strcmp( pccItemId, M4CL_ITEM_INSERT_BLK ) == 0 )
		{
			if( iType == M4CL_ITEM_TYPE_METHOD )
			{
				m_poInsertBlk = ai_poNitem ;
				iFound = 1 ;
			}
		}
	}



    iInternalType = poItem->GetInternalType() ;
    iType = poItem->GetCppType() ;

    switch( iInternalType )
    {
        case M4CL_INTERNAL_TYPE_START_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poStartDate == NULL )
				{
					m_poStartDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "START_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "START_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_END_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poEndDate == NULL )
				{
					m_poEndDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "END_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "END_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_COR_START_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poCorStartDate == NULL )
				{
					m_poCorStartDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "COR_START_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "COR_START_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_COR_END_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poCorEndDate == NULL )
				{
					m_poCorEndDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "COR_END_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "COR_END_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_PROJECTION :

			if( iType == M4CL_CPP_TYPE_NUMBER )
			{
				if( m_poProjection == NULL )
				{
					m_poProjection = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "TYPE_PROJECTION" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "TYPE_PROJECTION" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_FILTER_START_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poFilterStartDate == NULL )
				{
					m_poFilterStartDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "FILTER_START_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "FILTER_START_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_FILTER_END_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poFilterEndDate == NULL )
				{
					m_poFilterEndDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "FILTER_END_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "FILTER_END_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_FILTER_COR_START_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poFilterCorStartDate == NULL )
				{
					m_poFilterCorStartDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "FILTER_COR_START_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "FILTER_COR_START_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_FILTER_COR_END_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poFilterCorEndDate == NULL )
				{
					m_poFilterCorEndDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "FILTER_COR_END_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "FILTER_COR_END_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_CURRENCY_TYPE :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poCurrencyType == NULL )
				{
					m_poCurrencyType = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "CURRENCY_TYPE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "CURRENCY_TYPE" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poCurrencyExchangeDate == NULL )
				{
					m_poCurrencyExchangeDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "CURRENCY_EXCHANGE_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "CURRENCY_EXCHANGE_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_DMD_CHANGE_REASON :

			if( m_poDmdChangeReason == NULL )
			{
				m_poDmdChangeReason = ai_poNitem ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "DMD_CHANGE_REASON" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_DMD_COMPONENT :

			if( m_poDmdComponent == NULL )
			{
				m_poDmdComponent = ai_poNitem ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "DMD_COMPONENT" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_DMD_VALUE :

			if( m_poDmdValue == NULL )
			{
				m_poDmdValue = ai_poNitem ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "DMD_VALUE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_LOAD_SQL :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poLoadSQL == NULL )
				{
					m_poLoadSQL = ai_poNitem ;
				}

				m_oSysSentences.SetDelta( 1 ) ;

				iResult = m_oSysSentences.AddElement( ai_poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "LOAD_SQL" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_SYS_HINT :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				m_oSysHints.SetDelta( 1 ) ;

				iResult = m_oSysHints.AddElement( ai_poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "SYS_HINT" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_PRIORITY :

			if( iType == M4CL_CPP_TYPE_NUMBER )
			{
				if( m_poPriority == NULL )
				{
					m_poPriority = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "PRIORITY" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "PRIORITY" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_IMPUTE_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poImputeDate == NULL )
				{
					m_poImputeDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "IMPUTE_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "IMPUTE_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_PAY_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poPayDate == NULL )
				{
					m_poPayDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "PAY_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "PAY_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_PAY_TYPE :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poPayType == NULL )
				{
					m_poPayType = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "PAY_TYPE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "PAY_TYPE" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_PAY_FREQUENCY :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poPayFrequency == NULL )
				{
					m_poPayFrequency = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "PAY_FREQUENCY" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "PAY_FREQUENCY" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_IMPUTE_PAY_TYPE :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poImputePayType == NULL )
				{
					m_poImputePayType = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "IMPUTE_PAY_TYPE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "IMPUTE_PAY_TYPE" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_IMPUTE_PAY_FREQUENCY :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poImputePayFrequency == NULL )
				{
					m_poImputePayFrequency = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "IMPUTE_PAY_FREQUENCY" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "IMPUTE_PAY_FREQUENCY" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_CLOSING_DATE :

			if( iType == M4CL_CPP_TYPE_DATE )
			{
				if( m_poClosingDate == NULL )
				{
					m_poClosingDate = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "CLOSING_DATE" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "CLOSING_DATE" << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_SYS_PARAM :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poSysParam == NULL )
				{
					m_poSysParam = ai_poNitem ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "SYS_PARAM" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				m_oSysSentences.SetDelta( 1 ) ;

				iResult = m_oSysSentences.AddElement( ai_poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "DYNAMIC_SYS_SENTENCE" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_BDL_CHECKS :

			if( iType == M4CL_CPP_TYPE_NUMBER )
			{
				if( m_poBDLChecks == NULL )
				{
					m_poBDLChecks = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "BDL CHECKS" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "BDL CHECKS" << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_RAM_ORDER_BY :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poRAMOrderBy == NULL )
				{
					m_poRAMOrderBy = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "RAM_ORDER_BY" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "RAM_ORDER_BY" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        case M4CL_INTERNAL_TYPE_ID_ORGANIZATION :

			if( iType == M4CL_CPP_TYPE_STRING )
			{
				if( m_poOrganization == NULL )
				{
					m_poOrganization = ai_poNitem ;
				}
				else
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_REDEFINITION, "ID_ORGANIZATION" << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				}
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_INTERNAL_TYPE_MISMATCH, "ID_ORGANIZATION" << M4ClCppType( M4CL_CPP_TYPE_STRING ) << M4ClCppType( iType ) << LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}

            break ;

        default:
            break ;
    }


	m_oNitems.AddElement() ;

	return( M4_SUCCESS ) ;
}




m4return_t	ClTempNode :: AddIndex( ClTempIndex* ai_poIndex )
{

    m4uint8_t   iIsDefault ;


	CHECK_CHLOG_DEBUGF( ai_poIndex == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iIsDefault = ai_poIndex->GetIsDefault() ;

    if( iIsDefault == 1 )
    {
		if( m_poDefaultIndex == NULL )
		{
			m_poDefaultIndex = ai_poIndex ;
		}
		else
		{
			DUMP_CHLOG_WARNINGF( M4_MDFAC_REPEATED_DEFAULT_INDEX, ai_poIndex->GetIndexId() << m_poDefaultIndex->GetIndexId() << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}
    }

	m_oIndexes.AddElement() ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempNode :: AddFilter( ClTempFilter* ai_poFilter )
{

	m4uint8_t	iType ;
	m4pcchar_t	pccUsedNode ;
	ClTempNode	*poNode ;


	CHECK_CHLOG_DEBUGF( ai_poFilter == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iType = ai_poFilter->GetType() ;

	if( iType == M4CL_FILTER_TYPE_QBF )
	{
		m_iQBFFilters++ ;

		if( ai_poFilter->GetIsOwnNode() == M4_TRUE )
		{
			pccUsedNode = ai_poFilter->GetUsedNodeId() ;
			poNode = m_poChannel->GetNodeById( pccUsedNode, M4_FALSE ) ;

			if( poNode != NULL )
			{
				poNode->SetIsQBFFilter( 1 ) ;
			}
		}
	}
	else if( iType == M4CL_FILTER_TYPE_FIND )
	{
		m_iFindFilters++ ;
	}

	m_oFilters.AddElement() ;

	return( M4_SUCCESS ) ;
}





// Funciones varias ===========================================================

m4return_t	ClTempNode :: OrderItemConnMappings( void )
{

    m4return_t		iResult ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pvoid_t		pvPointer ;
	ClTempRitem		*poRitem ;
	ClTempNitem		*poNitem ;

    
    iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		poRitem = poNitem->GetRitem() ;
        iResult = poRitem->OrderItemConnMappings() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNode :: CalculateGroupOrder( void )
{

    m4return_t			iResult ;
	m4uint8_t			iFound ;
    m4int16_t           iDegree ;
	m4uint16_t			i, j ;
	m4uint16_t			iNitems ;
	m4uint16_t			iConditionants ;
	m4uint16_t			iDependents ;
	m4uint16_t			iItemsInGroups ;
	m4uint16_t			iInside ;
    m4pcchar_t			pccGroupId ;
	m4pvoid_t           pvPointer ;
	ClTempItem	        *poItem ;
	ClTempRitem	        *poRitem ;
	ClTempRitem	        *poConditionantRitem ;
	ClTempRitem	        *poDependentRitem ;
	ClTempNitem	        *poNitem ;
	ClTempNitem	        *poConditionant ;
	ClTempNitem	        *poDependent ;
	ClTempNitem	        *poInsideNitem ;
    ClTempNode          *poNode ;
    ClTempGroup			*poGroup ;
    ClPointerStack      oOrderedStack ;
    ClPointerStack      oTempStack ;



// Si no hay grupos salimos correctamente =====================================

    if( m_iItemsInGroups == 0 )
    {
        return( M4_SUCCESS ) ;
    }


// Ponemos a cero el delta de los condicionantes ==============================

	iItemsInGroups = 0 ;
	iNitems = m_oNitems.GetLength() ;

	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;

		// Si se compila y está en un grupo lo reseteamos
        if( poNitem->GetIsToCompile() == 1 && poNitem->IsInGroup() == 1 )
        {
			poRitem = poNitem->GetRitem() ;
            poRitem->SetDependentDelta( 0 ) ;
			iItemsInGroups++ ;
	    }
	}


// Contamos los tamaños de los arrays de dependientes y el numero real de condicionantes

	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;

        if( poNitem->GetIsToCompile() == 1 && poNitem->IsInGroup() == 1 )
        {
			poRitem = poNitem->GetRitem() ;
            iConditionants = poRitem->GetNumberOfConditionants() ;

            for( j = 0 ; j < iConditionants ; j++ )
            {
                poConditionant = poRitem->GetConditionantByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poConditionant == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( poConditionant->GetIsToCompile() == 1 && poConditionant->IsInGroup() == 1 )
                {
					// Solo si no es el mismo item
					if( poConditionant != poNitem )
					{
						poNode = poConditionant->GetNode() ;

						// Solo si es del mismo nodo
						if( poNode != NULL && poNode == this )
						{
							poConditionantRitem = poConditionant->GetRitem() ;
							poConditionantRitem->AddDependentDelta( 1 ) ;
							poRitem->IncrementConditionantDegree() ;
						}
					}
                }
            }
	    }
	}


// Rellenamos los arrays de dependientes ======================================

	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;

        if( poNitem->GetIsToCompile() == 1 && poNitem->IsInGroup() == 1 )
        {
			poRitem = poNitem->GetRitem() ;
            iConditionants = poRitem->GetNumberOfConditionants() ;

            for( j = 0 ; j < iConditionants ; j++ )
            {
                poConditionant = poRitem->GetConditionantByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poConditionant == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( poConditionant->GetIsToCompile() == 1 && poConditionant->IsInGroup() == 1 )
                {
					// Solo si no es el mismo item
					if( poConditionant != poNitem )
					{
						poNode = poConditionant->GetNode() ;

						// Solo si es del mismo nodo
						if( poNode != NULL && poNode == this )
						{
							poConditionantRitem = poConditionant->GetRitem() ;
                            poConditionantRitem->AddDependent( poNitem ) ;
                        }
                    }
                }
            }
        }
	}


// Rellenamos las pilas de orden y de trabajo =================================

    oOrderedStack.SetDelta( iItemsInGroups ) ;
    oTempStack.SetDelta( iItemsInGroups ) ;

	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;

        if( poNitem->GetIsToCompile() == 1 && poNitem->IsInGroup() == 1 )
        {
			poRitem = poNitem->GetRitem() ;
            iDegree = poRitem->GetConditionantDegree() ;

            if( iDegree == 0 )
            {
                iResult = oTempStack.AddElement( poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
            }
	    }
	}


// Algoritmo de orden =========================================================

    while( oTempStack.GetLength() != 0 )
    {
        iResult = oTempStack.Pop( pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        poNitem = ( ClTempNitem* ) pvPointer ;
		poRitem = poNitem->GetRitem() ;
        poRitem->SetConditionantDegree( -1 ) ;

		iResult = oOrderedStack.AddElement( poNitem ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		
        iDependents = poRitem->GetNumberOfDependents() ;

        for( j = 0 ; j < iDependents ; j++ )
        {
            poDependent = poRitem->GetDependentByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poDependent == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poDependentRitem = poDependent->GetRitem() ;
            poDependentRitem->DecrementConditionantDegree() ;

            iDegree = poDependentRitem->GetConditionantDegree() ;

            if( iDegree == 0 )
            {
                iResult = oTempStack.AddElement( poDependent ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
            }
        }
    }


// Comprobamos los ciclos =====================================================

    iInside = 0 ;

	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;

        if( poNitem->GetIsToCompile() == 1 && poNitem->IsInGroup() == 1 )
        {
			poRitem = poNitem->GetRitem() ;
            iDegree = poRitem->GetConditionantDegree() ;

            if( iDegree != -1 )
            {
                iInside++ ;
                poInsideNitem = poNitem ;
            }
        }
	}


// Se imprime el ciclo si lo hubiera ==========================================

    if( iInside > 0 )
    {
		iDegree = 0 ;
		while( iDegree != -2 )
		{
			poRitem = poInsideNitem->GetRitem() ;
			poItem = poInsideNitem->GetItem() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_CYCLE, LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( poInsideNitem->iNode(), poInsideNitem->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << poItem->GetGroupId() ) ;

			poRitem->SetConditionantDegree( -2 ) ;

			iConditionants = poRitem->GetNumberOfConditionants() ;

			iFound = 0 ;
			for( j = 0 ; j < iConditionants  && !iFound ; j++ )
			{
				poConditionant = poRitem->GetConditionantByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poConditionant == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( poConditionant->GetIsToCompile() == 1 && poConditionant->IsInGroup() == 1 )
                {
					// Solo si no es el mismo item
					if( poConditionant != poInsideNitem )
					{
						poNode = poConditionant->GetNode() ;

						// Solo si es del mismo nodo
						if( poNode != NULL && poNode == this )
						{
							poConditionantRitem = poConditionant->GetRitem() ;
							iDegree = poConditionantRitem->GetConditionantDegree() ;

							if( iDegree != -1 )
							{
								iFound = 1 ;
							}
						}
					}
				}
			}

			if( iFound == 1 )
			{
				poInsideNitem = poConditionant ;
			}
			else
			{
				// Nos salimos para no quedarnos en un ciclo
				iDegree = -2 ;
			}
		}

		return( M4_ERROR ) ;
    }


// Añadimos los grupos ========================================================

    iNitems = oOrderedStack.GetLength() ;
    
	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = oOrderedStack.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		poItem = poNitem->GetItem() ;

        pccGroupId = poItem->GetGroupId() ;
        poGroup = GetGroupById( pccGroupId, M4_FALSE ) ;

        if( poGroup == NULL )
        {
            poGroup = new ClTempGroup ;
			CHECK_CHLOG_ERRORF( poGroup == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempGroup ) ) << __LINE__ << __FILE__ ) ;

			poGroup->SetUpperPool( m_poUpperPool ) ;


			m_oGroups.SetDelta( 1 ) ;

            iResult = AddGroup( poGroup ) ;

		    if( iResult != M4_SUCCESS )
		    {
				DUMP_CHLOG_DEBUGF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				delete( poGroup ) ;
			    return( M4_ERROR ) ;
		    }

            iResult = poGroup->SetGroupId( pccGroupId ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
        }

        poGroup->AddNitemDelta( 1 ) ;
	}


    m_oGroups.Sort( M4ClCompareObjectsById ) ;


	for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = oOrderedStack.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		poItem = poNitem->GetItem() ;

        pccGroupId = poItem->GetGroupId() ;
        poGroup = GetGroupById( pccGroupId, M4_FALSE ) ;
		CHECK_CHLOG_DEBUGF( poGroup == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        iResult = poGroup->AddNitem( poNitem ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    }


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNode :: CalculateItemTypes( void )
{

    m4return_t		iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint8_t		iScope ;
	m4uint8_t		iType ;
	m4uint16_t		i, j ;
	m4uint16_t		iLength ;
	m4pvoid_t       pvPointer ;
	ClTempRitem	    *poRitem ;
	ClTempNitem	    *poNitem ;
	ClTempItem	    *poItem ;
    ClTempConnector *poConnector ;

    
// tipos de items
    
    iLength = m_oNitems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			poItem = poNitem->GetItem() ;
			CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poRitem = poNitem->GetRitem() ;

			iScope = poRitem->GetScope() ;

			switch( iScope )
			{
				case M4CL_ITEM_SCOPE_NODE :

					m_iNodeItems++ ;
					break ;

				case M4CL_ITEM_SCOPE_BLOCK :

					m_iBlockItems++ ;
					break ;

				case M4CL_ITEM_SCOPE_REGISTER :

					m_iRegisterItems++ ;
					break ;
        
				default :
					DUMP_CHLOG_ERRORF( M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					return( M4_ERROR ) ;
			}
		}
	}


// tipos de nodos conectados
    
    iLength = m_oConnectors.GetLength() ;

    iType = 0 ;

	j = 0 ;
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = ( ClTempConnector* ) pvPointer ;
		iIsToCompile = poConnector->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			if( poConnector->GetType() != iType )
			{
				iType = poConnector->GetType() ;
				m_iConnectedStart[ iType - 1 ] = j ;
				m_iConnectedNumber[ iType - 1 ] = 1 ;
			}
			else
			{
				m_iConnectedNumber[ iType - 1 ]++ ;
			}

			j++ ;
		}
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNode :: ComputeConnectedNitemsScope( void )
{

    
    m4return_t			iResult ;
	m4uint8_t			iType ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pvoid_t           pvPointer ;
	ClTempNitem	        *poNitem ;
	ClTempRitem	        *poRitem ;
	ClTempItem	        *poItem ;
 

	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		poRitem = poNitem->GetRitem() ;
		poItem = poNitem->GetItem() ;
		iType = poItem->GetType() ;
        iResult = poRitem->ComputeConnectedNitemsScope( poNitem ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempNode :: CalculateDmds( void )
{

    m4return_t			iResult ;
	m4uint8_t			iIsToCompile ;
	m4uint8_t			iType ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pcchar_t			pccDmdComponent ;
	m4pvoid_t           pvPointer ;
	ClTempItem	        *poItem ;
	ClTempNitem	        *poNitem ;
 

	m_oDmds.SetDelta( 0 ) ;

	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			poItem = poNitem->GetItem() ;

			iType = poItem->GetType() ;

			if( iType != M4CL_ITEM_TYPE_METHOD )
			{
				pccDmdComponent = poItem->GetDmdComponentId() ;

				if( *pccDmdComponent != '\0' )
				{
					m_oDmds.AddDelta( 1 ) ;
				}
			}
		}
	}


	iLength = m_oNitems.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			poItem = poNitem->GetItem() ;

			iType = poItem->GetType() ;

			if( iType != M4CL_ITEM_TYPE_METHOD )
			{
				pccDmdComponent = poItem->GetDmdComponentId() ;

				if( *pccDmdComponent != '\0' )
				{
					iResult = m_oDmds.AddElement( poNitem ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}
		}
	}

	m_oDmds.Sort( M4ClCompareDmdsById ) ;

    return( M4_SUCCESS ) ;
}


