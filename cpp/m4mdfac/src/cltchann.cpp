//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltmcr.cpp   
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
//    This module defines the functions of the temporary metachanel in RAM
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"

#include "cltchann.hpp"
#include "cltrsm.hpp"
#include "m4mdrt.hpp"
#include "cldates.hpp"
#include "cltconn.hpp"
#include "cltnicon.hpp"
#include "cltindit.hpp"
#include "cltiplug.hpp"
#include "cltarg.hpp"
#include "cltfilt.hpp"
#include "cltt3inh.hpp"
#include "clttiinh.hpp"
#include "cltndinh.hpp"
#include "clttrig.hpp"
#include "cltiform.hpp"
#include "clttotal.hpp"
#include "clttotaltag.hpp"
#include "clttag.hpp"
#include "cltti.hpp"
#include "clttiref.hpp"
#include "cltsent.hpp"
#include "cltinst.hpp"
#include "cltnode.hpp"
#include "cltbarg.hpp"
#include "cltbmeth.hpp"
#include "cltitinh.hpp"
#include "cltcpt.hpp"
#include "m4objglb.hpp"
#include "m4lgadapdef.hpp"
#include "chlog.hpp"
#include "cllgadap.hpp"
#include "m4objreg.hpp"
#include "recorset.hpp"
#include "cllstr.hpp"
#include "m4hsparser.hpp"
#include "nodedef.hpp"
#include "m4unicode.hpp"


extern	m4int8_t   g_aiReducedNodeCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] ;
extern	m4int8_t   g_aiReducedItemCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] ;



// ============================================================================


// Array de items de base de datos para apaño indecente
// al que toque el orden lo mato

m4pchar_t	g_apcRootItems[ 10 ] =
{
	M4CL_ITEM_ROOT_LOAD,
	M4CL_ITEM_PERSIST_TREE,
	M4CL_ITEM_LOAD_PRG,
	M4CL_ITEM_INSERT_PRG,
	M4CL_ITEM_DELETE_PRG,
	M4CL_ITEM_UPDATE_PRG,
	M4CL_ITEM_LOAD_BLK,
	M4CL_ITEM_INSERT_BLK,
	M4CL_ITEM_DELETE_BLK,
	M4CL_ITEM_UPDATE_BLK
} ;


// array de validación de cs type de items en nodos
m4uint8_t	g_aiValidItemsCsTypesArray[ 4 ][ 4 ] =
{
// Item cs type:			  F  Ba Bo M
/* Node cs type FRONT  */	{ 1, 0, 1, 0 },
/* Node cs type BACK   */	{ 0, 1, 1, 1 },
/* Node cs type BOTH   */	{ 1, 1, 1, 1 },
/* Node cs type MAPPED */	{ 1, 1, 1, 1 }
} ;


// ============================================================================



//=============================================================================
// Funciones de comparacion
//=============================================================================

int	M4ClCompareT3InheritsByLevel( m4pcvoid_t ai_pcvT3Inherit1, m4pcvoid_t ai_pcvT3Inherit2 )
{
	return( ( *( ClTempT3Inherit** )ai_pcvT3Inherit1 )->GetLevel() - ( *( ClTempT3Inherit** )ai_pcvT3Inherit2 )->GetLevel() ) ;
}



int	M4ClCompareTiInheritsByIdAndLevel( m4pcvoid_t ai_pcvTiInherit1, m4pcvoid_t ai_pcvTiInherit2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempTiInherit** )ai_pcvTiInherit1 )->GetTiId(), ( *( ClTempTiInherit** )ai_pcvTiInherit2 )->GetTiId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempTiInherit** )ai_pcvTiInherit1 )->GetLevel() - ( *( ClTempTiInherit** )ai_pcvTiInherit2 )->GetLevel() ) ;
}



int	M4ClCompareArgumentsByTiItemAndOrder( m4pcvoid_t ai_pcvArg1, m4pcvoid_t ai_pcvArg2 )
{

	int	iResult = M4MdFacStrcmp( ( *( ClTempArgument** )ai_pcvArg1 )->GetTiId(), ( *( ClTempArgument** )ai_pcvArg2 )->GetTiId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }
    
	iResult = M4MdFacStrcmp( ( *( ClTempArgument** )ai_pcvArg1 )->GetItemId(), ( *( ClTempArgument** )ai_pcvArg2 )->GetItemId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempArgument** )ai_pcvArg1 )->GetOrder() - ( *( ClTempArgument** )ai_pcvArg2 )->GetOrder() ) ;
}



int	M4ClCompareFormulasByTiItemRuleAndDate( m4pcvoid_t ai_pcvFormula1, m4pcvoid_t ai_pcvFormula2 )
{

	int	iResult = M4MdFacStrcmp( ( *( ClTempItemFormula** )ai_pcvFormula1 )->GetTiId(), ( *( ClTempItemFormula** )ai_pcvFormula2 )->GetTiId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }
    
	iResult = M4MdFacStrcmp( ( *( ClTempItemFormula** )ai_pcvFormula1 )->GetItemId(), ( *( ClTempItemFormula** )ai_pcvFormula2 )->GetItemId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempItemFormula** )ai_pcvFormula1 )->GetFormulaId(), ( *( ClTempItemFormula** )ai_pcvFormula2 )->GetFormulaId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempItemFormula** )ai_pcvFormula1 )->GetStartDate() - ( *( ClTempItemFormula** )ai_pcvFormula2 )->GetStartDate() ) ;
}



int	M4ClCompareTotalsByTiId( m4pcvoid_t ai_pcvTotal1, m4pcvoid_t ai_pcvTotal2 )
{
	return( M4MdFacStrcmp( ( *( ClTempTotal** )ai_pcvTotal1 )->GetTiId(), ( *( ClTempTotal** )ai_pcvTotal2 )->GetTiId() ) ) ;
}


int	M4ClCompareTotalTagsByTiId( m4pcvoid_t ai_pcvTotalTag1, m4pcvoid_t ai_pcvTotalTag2 )
{
	return( M4MdFacStrcmp( ( *( ClTempTotalTag** )ai_pcvTotalTag1 )->GetTiId(), ( *( ClTempTotalTag** )ai_pcvTotalTag2 )->GetTiId() ) ) ;
}


int	M4ClCompareTagsByTiId( m4pcvoid_t ai_pcvTag1, m4pcvoid_t ai_pcvTag2 )
{
	return( M4MdFacStrcmp( ( *( ClTempTag** )ai_pcvTag1 )->GetTiId(), ( *( ClTempTag** )ai_pcvTag2 )->GetTiId() ) ) ;
}



int	M4ClCompareNodeInheritsByNodeAndLevel( m4pcvoid_t ai_pcvNodeInherit1, m4pcvoid_t ai_pcvNodeInherit2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempNodeInherit** )ai_pcvNodeInherit1 )->GetNodeId(), ( *( ClTempNodeInherit** )ai_pcvNodeInherit2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempNodeInherit** )ai_pcvNodeInherit1 )->GetLevel() - ( *( ClTempNodeInherit** )ai_pcvNodeInherit2 )->GetLevel() ) ;
}


int	M4ClCompareConnectorsByFatherTypeAndSon( m4pcvoid_t ai_pcvConnector1, m4pcvoid_t ai_pcvConnector2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempConnector** )ai_pcvConnector1 )->GetFatherNodeId(), ( *( ClTempConnector** )ai_pcvConnector2 )->GetFatherNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

    iResult = ( *( ClTempConnector** )ai_pcvConnector1 )->GetType() - ( *( ClTempConnector** )ai_pcvConnector2 )->GetType() ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( ( *( ClTempConnector** )ai_pcvConnector1 )->GetSonNode()->GetNodeId(), ( *( ClTempConnector** )ai_pcvConnector2 )->GetSonNode()->GetNodeId() ) ) ;
}


int	M4ClCompareConnectorsBySonTypeAndFather( m4pcvoid_t ai_pcvConnector1, m4pcvoid_t ai_pcvConnector2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempConnector** )ai_pcvConnector1 )->GetSonNode()->GetNodeId(), ( *( ClTempConnector** )ai_pcvConnector2 )->GetSonNode()->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

    iResult = ( *( ClTempConnector** )ai_pcvConnector1 )->GetType() - ( *( ClTempConnector** )ai_pcvConnector2 )->GetType() ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( ( *( ClTempConnector** )ai_pcvConnector1 )->GetFatherNodeId(), ( *( ClTempConnector** )ai_pcvConnector2 )->GetFatherNodeId() ) ) ;
}


int	M4ClCompareNitemConnsByNodeItemAndOrder( m4pcvoid_t ai_pcvNitenConn1, m4pcvoid_t ai_pcvNitenConn2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempNitemConn** )ai_pcvNitenConn1 )->GetFatherNodeId(), ( *( ClTempNitemConn** )ai_pcvNitenConn2 )->GetFatherNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempNitemConn** )ai_pcvNitenConn1 )->GetFatherItemId(), ( *( ClTempNitemConn** )ai_pcvNitenConn2 )->GetFatherItemId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempNitemConn** )ai_pcvNitenConn1 )->GetCallOrder() - ( *( ClTempNitemConn** )ai_pcvNitenConn2 )->GetCallOrder() ) ;
}


int	M4ClCompareIndexesByNodeAndIndex( m4pcvoid_t ai_pcvIndex1, m4pcvoid_t ai_pcvIndex2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempIndex** )ai_pcvIndex1 )->GetNodeId(), ( *( ClTempIndex** )ai_pcvIndex2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempIndex** )ai_pcvIndex1 )->GetIndexId() - ( *( ClTempIndex** )ai_pcvIndex2 )->GetIndexId() ) ;
}


int	M4ClCompareIndexItemsByNodeIndexAndOrder( m4pcvoid_t ai_pcvIndexItem1, m4pcvoid_t ai_pcvIndexItem2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempIndexItem** )ai_pcvIndexItem1 )->GetNodeId(), ( *( ClTempIndexItem** )ai_pcvIndexItem2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = ( *( ClTempIndexItem** )ai_pcvIndexItem1 )->GetIndexId() - ( *( ClTempIndexItem** )ai_pcvIndexItem2 )->GetIndexId() ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempIndexItem** )ai_pcvIndexItem1 )->GetOrder() - ( *( ClTempIndexItem** )ai_pcvIndexItem2 )->GetOrder() ) ;
}


int	M4ClComparePlugsByNodeUsedT3AndUsedNode( m4pcvoid_t ai_pcvPlug1, m4pcvoid_t ai_pcvPlug2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempPlug** )ai_pcvPlug1 )->GetNodeId(), ( *( ClTempPlug** )ai_pcvPlug2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempPlug** )ai_pcvPlug1 )->GetUsedT3Id(), ( *( ClTempPlug** )ai_pcvPlug2 )->GetUsedT3Id() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( ( *( ClTempPlug** )ai_pcvPlug1 )->GetUsedNodeId(), ( *( ClTempPlug** )ai_pcvPlug2 )->GetUsedNodeId() ) ) ;
}


int	M4ClComparePlugItemsByNodeUsedT3UsedNodeAndItem( m4pcvoid_t ai_pcvPlugItem1, m4pcvoid_t ai_pcvPlugItem2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempPlugItem** )ai_pcvPlugItem1 )->GetNodeId(), ( *( ClTempPlugItem** )ai_pcvPlugItem2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempPlugItem** )ai_pcvPlugItem1 )->GetUsedT3Id(), ( *( ClTempPlugItem** )ai_pcvPlugItem2 )->GetUsedT3Id() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempPlugItem** )ai_pcvPlugItem1 )->GetUsedNodeId(), ( *( ClTempPlugItem** )ai_pcvPlugItem2 )->GetUsedNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( ( *( ClTempPlugItem** )ai_pcvPlugItem1 )->GetItemId(), ( *( ClTempPlugItem** )ai_pcvPlugItem2 )->GetItemId() ) ) ;
}


int	M4ClCompareT3AliasByNodeAliasAndLevel( m4pcvoid_t ai_pcvT3Alias1, m4pcvoid_t ai_pcvT3Alias2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempT3Alias** )ai_pcvT3Alias1 )->GetNodeId(), ( *( ClTempT3Alias** )ai_pcvT3Alias2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	iResult = M4MdFacStrcmp( ( *( ClTempT3Alias** )ai_pcvT3Alias1 )->GetT3AliasId(), ( *( ClTempT3Alias** )ai_pcvT3Alias2 )->GetT3AliasId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( ( *( ClTempT3Alias** )ai_pcvT3Alias1 )->GetLevel() - ( *( ClTempT3Alias** )ai_pcvT3Alias2 )->GetLevel() ) ;
}


/*
Se ha pedido que los filtros se ordenen por identificador
El nodo sigue como antes y el filtro deja de ir por tamaño
*/
int	M4ClCompareFiltersByNodeAndFilter( m4pcvoid_t ai_pcvObject1, m4pcvoid_t ai_pcvObject2 )
{
	int	iResult = M4MdFacStrcmp( ( *( ClTempFilter** )ai_pcvObject1 )->GetNodeId(), ( *( ClTempFilter** )ai_pcvObject2 )->GetNodeId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( strcmp( ( *( ClTempFilter** )ai_pcvObject1 )->GetFilterId(), ( *( ClTempFilter** )ai_pcvObject2 )->GetFilterId() ) ) ;
}


int	M4ClCompareRowsByRevTotalAndDBOrder( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	ClSliceRow	*poRow1 = *( ClSliceRow** ) ai_pcvRow1 ;
	ClSliceRow	*poRow2 = *( ClSliceRow** ) ai_pcvRow2 ;

	ClTempNitemFormula	*poFormula1 = ( ClTempNitemFormula* ) poRow1->GetLeader() ;
	ClTempNitemFormula	*poFormula2 = ( ClTempNitemFormula* ) poRow2->GetLeader() ;


	// Si no son sin revisión van primero
	m4uint8_t	iValue1 = poFormula1->GetIsRetroactivity() ;
	m4uint8_t	iValue2 = poFormula2->GetIsRetroactivity() ;

	if( iValue1 != iValue2 )
	{
		return( iValue1 - iValue2 ) ;
	}

	iValue1 = poFormula1->GetBatchType() ;
	iValue2 = poFormula2->GetBatchType() ;

	/* Bug 0123410
	Por aquí han pasado además los bugs 0089848, 0094379, 0122637
	Ahora la cosa queda:
		Los totales (tipo 2) van primero (como antes) sin rule order
		Después los de tipo 5 con rule order
		Después los de tipo 4 con rule order
		Después el resto ordenadas por rule order
	*/
	if( iValue1 == 2 )
	{
		if( iValue2 != 2 )
		{
			// El primero es 2 y el segundo no
			return( -1 ) ;
		}
		else
		{
			// Los dos son 2
			return( 0 ) ;
		}
	}
	else
	{
		if( iValue2 == 2 )
		{
			// El primero no es 2 y el segundo si
			return( 1 ) ;
		}
		else
		{
			// Ninguno es 2, van por el 5
			if( iValue1 == 5 )
			{
				if( iValue2 != 5 )
				{
					// El primero es 5 y el segundo no
					return( -1 ) ;
				}
				else
				{
					// Los dos son 5, van por rule order
				}
			}
			else
			{
				if( iValue2 == 5 )
				{
					// El primero no es 5 y el segundo si
					return( 1 ) ;
				}
				else
				{
					// Ninguno es 5, van por el 4
					if( iValue1 == 4 )
					{
						if( iValue2 != 4 )
						{
							// El primero es 4 y el segundo no
							return( -1 ) ;
						}
						else
						{
							// Los dos son 4, van por rule order
						}
					}
					else
					{
						if( iValue2 == 4 )
						{
							// El primero no es 4 y el segundo si
							return( 1 ) ;
						}
						else
						{
							// Ninguno es 4, van por rule order
						}
					}
				}
			}
		}
	}

	m4int32_t	iDBOrder1 = poFormula1->GetDBOrder() ;
	m4int32_t	iDBOrder2 = poFormula2->GetDBOrder() ;

	return( iDBOrder1 - iDBOrder2 ) ;
}


m4bool_t	M4ClCheckRowIsTotal( m4pcvoid_t ai_pcvRow )
{

	ClSliceRow	*poRow = *( ClSliceRow** ) ai_pcvRow ;
	ClTempNitemFormula	*poFormula = ( ClTempNitemFormula* ) poRow->GetLeader() ;
	m4uint8_t iValue = poFormula->GetBatchType() ;

	if( iValue == 2 )
	{
		return( M4_TRUE ) ;
	}

	return( M4_FALSE ) ;
}



//=================================================================================
//
// ClTempChannel
//
// Clase que implementa la definición de los M4Objs
//
//=================================================================================
	
ClTempChannel :: ClTempChannel( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

    m_iSize = 0 ;
    m_iBuild = 0 ;
    m_iTimeUnit = 0 ;
	m_iCsType = 0 ;
	m_iLanguage = M4CL_LANGUAGE_DEFAULT ;
	m_dVersion = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	m_dCorStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dCorEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	m_dCreationDate = ClActualDate() ;
	m_dCacheMaxPeriod = 0 ;
    m_iChannelId = 0 ;
	m_iName = 0 ;
	m_iPool = 0 ;
	m_iOleClassId = 0 ;
	m_iServiceId = 0 ;
	m_iServiceAltId = 0 ;
	m_iEnvironmentVars = 0 ;
	m_iOwnerFlag = 0 ;
    m_iRootsStart = 0 ;
    m_iSentencesStart = 0 ;
    m_iInstancesStart = 0 ;
    m_iSystemItemsStart = 0 ;
	m_iTisStart = 0 ;
	m_iParametersStart = 0 ;
	m_iRSMsStart = 0 ;
	m_iBusinessMethodsStart = 0 ;

	m_iSecurity = 0 ;
	m_iVMCapacityReq = 0 ;

    m_iType = 0 ;
	m_iIsSeparable = 1 ;
	m_iCsExeType = M4CL_CSEXETYPE_OLTP ;
	m_iIsExternal = 0 ;
	m_iHasSecurity = 0 ;
	m_iIsCacheable = 0 ;
	m_iIsMDCacheable = 0 ;
	m_iCheckConcurrency = 0 ;
	m_iOrganizationType = M4CL_ORGANIZATION_TYPE_NONE ;
	m_iDataLanguage = M4CL_LANGUAGE_TYPE_NONE ;
	m_iMetaLanguage = M4CL_LANGUAGE_TYPE_NONE ;
	m_iCreationType = M4CL_CHANNEL_CREATION_TYPE_NONE ;
	m_iStyleUse = 0 ;
	m_iWSSSecurity = 0 ;

	m_poSysLoad = NULL ;
	m_poSysPersist = NULL ;

	m_poClientSysLoad = NULL ;
	m_poClientSysPersist = NULL ;
	m_poServerSysLoad = NULL ;
	m_poServerSysPersist = NULL ;

	m_iDefaultLanguage = M4CL_LANGUAGE_DEFAULT ;

	m_dUserStartDate = 0 ;
	m_dUserEndDate = 0 ;

    m_poSystemTi = NULL ;
	m_poSystemNode = NULL ;
    m_poFactory = NULL ;

    m_poCompT3Inherit = &m_oCompT3Inherit ;
    m_poComparisonTi = &m_oComparisonTi ;
    m_poComparisonNode = &m_oComparisonNode ;
    m_poComparisonItem = &m_oComparisonItem ;
    m_poComparisonSentence = &m_oComparisonSentence ;
    m_poComparisonInstance = &m_oComparisonInstance ;

	m_iNodesToCompile = 0 ;
	m_iTisToCompile = 0 ;
	m_iSentencesToCompile = 0 ;
	m_iParametersToCompile = 0 ;

	m_iNitemConnsSize = 0 ;

	m_poT3InheritsArray = NULL ;
	m_poSentencesArray = NULL ;
	m_poRSMsArray = NULL ;
	m_poTisArray = NULL ;
	m_poTiInheritsArray = NULL ;
	m_poTiReferencesArray = NULL ;
	m_poItemsArray = NULL ;
	m_poItemInheritsArray = NULL ;
	m_poConceptsArray = NULL ;
	m_poArgumentsArray = NULL ;
	m_poItemFormulasArray = NULL ;
	m_poTriggersArray = NULL ;
	m_poTotalsArray = NULL ;
	m_poTotalTagsArray = NULL ;
	m_poTagsArray = NULL ;
	m_poNodesArray = NULL ;
	m_poNodeInheritsArray = NULL ;
	m_poNitemsArray = NULL ;
	m_poConnectorsArray = NULL ;
	m_poNitemConnsArray = NULL ;
	m_poIndexesArray = NULL ;
	m_poIndexItemsArray = NULL ;
	m_poPlugsArray = NULL ;
	m_poPlugItemsArray = NULL ;
	m_poAliasArray = NULL ;
	m_poT3AliasArray = NULL ;
	m_poFiltersArray = NULL ;
	m_poInstancesArray = NULL ;
	m_poBusinessMethodsArray = NULL;
	m_poBusinessArgsArray = NULL ;

	m_poClientSysLoadItem = NULL ;
	m_poClientSysPersistItem = NULL ;
	m_poServerSysLoadItem = NULL ;
	m_poServerSysPersistItem = NULL ;
	m_poExecuteReportItem = NULL ;
	m_poDynFilterItem = NULL ;

	m_poSystemArgumentsArray = NULL ;

	m_poHsParser = new M4HsParser ;

	m_iPrint = 0 ;

	m_iNoQBFRoots = 0 ;
}


ClTempChannel :: ~ClTempChannel( void )
{
    Delete() ;
}


m4return_t	ClTempChannel :: Delete( void )
{

// Memoria alojada

	if( m_poT3InheritsArray != NULL )
	{
		delete [] m_poT3InheritsArray ;
		m_poT3InheritsArray = NULL ;
	}

	if( m_poSentencesArray != NULL )
	{
		delete [] m_poSentencesArray ;
		m_poSentencesArray = NULL ;
	}

	if( m_poRSMsArray != NULL )
	{
		delete [] m_poRSMsArray ;
		m_poRSMsArray = NULL ;
	}

	if( m_poTisArray != NULL )
	{
		delete [] m_poTisArray ;
		m_poTisArray = NULL ;
	}

	if( m_poTiInheritsArray != NULL )
	{
		delete [] m_poTiInheritsArray ;
		m_poTiInheritsArray = NULL ;
	}

	if( m_poTiReferencesArray != NULL )
	{
		delete [] m_poTiReferencesArray ;
		m_poTiReferencesArray = NULL ;
	}

	if( m_poItemsArray != NULL )
	{
		delete [] m_poItemsArray ;
		m_poItemsArray = NULL ;
	}

	if( m_poItemInheritsArray != NULL )
	{
		delete [] m_poItemInheritsArray ;
		m_poItemInheritsArray = NULL ;
	}
	
	if( m_poConceptsArray != NULL )
	{
		delete [] m_poConceptsArray ;
		m_poConceptsArray = NULL ;
	}
	
	if( m_poArgumentsArray != NULL )
	{
		delete [] m_poArgumentsArray ;
		m_poArgumentsArray = NULL ;
	}

	if( m_poItemFormulasArray != NULL )
	{
		delete [] m_poItemFormulasArray ;
		m_poItemFormulasArray = NULL ;
	}

	if( m_poTriggersArray != NULL )
	{
		delete [] m_poTriggersArray ;
		m_poTriggersArray = NULL ;
	}

	if( m_poTotalsArray != NULL )
	{
		delete [] m_poTotalsArray ;
		m_poTotalsArray = NULL ;
	}

	if( m_poTotalTagsArray != NULL )
	{
		delete [] m_poTotalTagsArray ;
		m_poTotalTagsArray = NULL ;
	}

	if( m_poTagsArray != NULL )
	{
		delete [] m_poTagsArray ;
		m_poTagsArray = NULL ;
	}

	if( m_poNodesArray != NULL )
	{
		delete [] m_poNodesArray ;
		m_poNodesArray = NULL ;
	}

	if( m_poNodeInheritsArray != NULL )
	{
		delete [] m_poNodeInheritsArray ;
		m_poNodeInheritsArray = NULL ;
	}

	if( m_poNitemsArray != NULL )
	{
		delete [] m_poNitemsArray ;
		m_poNitemsArray = NULL ;
	}
	
	if( m_poConnectorsArray != NULL )
	{
		delete [] m_poConnectorsArray ;
		m_poConnectorsArray = NULL ;
	}

	if( m_poNitemConnsArray != NULL )
	{
		delete [] m_poNitemConnsArray ;
		m_poNitemConnsArray = NULL ;
	}

	if( m_poIndexesArray != NULL )
	{
		delete [] m_poIndexesArray ;
		m_poIndexesArray = NULL ;
	}

	if( m_poIndexItemsArray != NULL )
	{
		delete [] m_poIndexItemsArray ;
		m_poIndexItemsArray = NULL ;
	}

	if( m_poPlugsArray != NULL )
	{
		delete [] m_poPlugsArray ;
		m_poPlugsArray = NULL ;
	}

	if( m_poPlugItemsArray != NULL )
	{
		delete [] m_poPlugItemsArray ;
		m_poPlugItemsArray = NULL ;
	}

	if( m_poAliasArray != NULL )
	{
		delete [] m_poAliasArray ;
		m_poAliasArray = NULL ;
	}

	if( m_poT3AliasArray != NULL )
	{
		delete [] m_poT3AliasArray ;
		m_poT3AliasArray = NULL ;
	}

	if( m_poFiltersArray != NULL )
	{
		delete [] m_poFiltersArray ;
		m_poFiltersArray = NULL ;
	}

	if( m_poInstancesArray != NULL )
	{
		delete [] m_poInstancesArray ;
		m_poInstancesArray = NULL ;
	}

	if( m_poBusinessMethodsArray != NULL )
	{
		delete [] m_poBusinessMethodsArray ;
		m_poBusinessMethodsArray = NULL ;
	}

	if( m_poBusinessArgsArray != NULL )
	{
		delete [] m_poBusinessArgsArray ;
		m_poBusinessArgsArray = NULL ;
	}

	if( m_poClientSysLoadItem != NULL )
	{
		delete( m_poClientSysLoadItem ) ;
		m_poClientSysLoadItem = NULL ;
	}

	if( m_poClientSysPersistItem != NULL )
	{
		delete( m_poClientSysPersistItem ) ;
		m_poClientSysPersistItem = NULL ;
	}

	if( m_poServerSysLoadItem != NULL )
	{
		delete( m_poServerSysLoadItem ) ;
		m_poServerSysLoadItem = NULL ;
	}

	if( m_poServerSysPersistItem != NULL )
	{
		delete( m_poServerSysPersistItem ) ;
		m_poServerSysPersistItem = NULL ;
	}

	if( m_poExecuteReportItem != NULL )
	{
		delete( m_poExecuteReportItem ) ;
		m_poExecuteReportItem = NULL ;
	}

	if( m_poDynFilterItem != NULL )
	{
		delete( m_poDynFilterItem ) ;
		m_poDynFilterItem = NULL ;
	}

	if( m_poSystemArgumentsArray != NULL )
	{
		delete []m_poSystemArgumentsArray ;
		m_poSystemArgumentsArray = NULL ;
	}

	if( m_poHsParser != NULL )
	{
		delete( m_poHsParser ) ;
		m_poHsParser = NULL ;
	}

    return( M4_SUCCESS ) ;
}





// Otras funciones de lectura ================================================

ClTempT3Inherit* ClTempChannel :: GetT3InheritByLevel( m4uint16_t ai_iLevel )
{

    ClTempT3Inherit		**ppoResult ;
	m4uint8_t			iIndex ;
	m4uint16_t			iLength ;
    ClTempT3Inherit		*poT3Inherit ;

	
	// Lo normal es que el nivel y el indice coincidan
	iLength = GetNumberOfT3Inherits() ;

	if( ai_iLevel < iLength )
	{
		poT3Inherit = GetT3InheritByPosition( ai_iLevel ) ;

		if( poT3Inherit != NULL )
		{
			iIndex = poT3Inherit->GetIndex() ;

			if( ai_iLevel == iIndex )
			{
				return( poT3Inherit ) ;
			}
		}

	}

	m_oCompT3Inherit.SetIdentifier( ai_iLevel ) ;

	ppoResult = ( ClTempT3Inherit** ) m_oT3Inherits.Find( M4ClCompareObjectsByNumber, &m_poCompT3Inherit ) ;

	if( ppoResult == NULL )
	{
		return( NULL ) ;
	}

    return( *ppoResult ) ;
}



ClTempT3Inherit*	ClTempChannel :: _GetT3InheritById( m4pcchar_t ai_pccT3Id )
{

    ClTempT3Inherit		*poResult ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pcchar_t			pccT3Id ;


	CHECK_CHLOG_DEBUGF( ai_pccT3Id == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	
	// Lo normal es que el nivel y el indice coincidan
	iLength = GetNumberOfT3Inherits() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poResult = GetT3InheritByPosition( i ) ;

		if( poResult != NULL )
		{
			pccT3Id = poResult->GetBaseT3Id() ;

			if( pccT3Id != NULL )
			{
				if( M4MdFacStrcmp( pccT3Id, ai_pccT3Id ) == 0 )
				{
					return( poResult ) ;
				}
			}
		}
	}

	return( NULL ) ;
}



ClTempItem*	ClTempChannel :: _CreateSystemItem( m4pcchar_t ai_pccItemId, m4uint8_t ai_iType, m4uint8_t ai_iScope, m4uint8_t ai_iM4Type, m4uint32_t ai_iDBPrecision, m4uint8_t ai_iScale, m4uint8_t ai_iInternalType, m4uint8_t ai_iCsType, m4uint8_t ai_iTransactionMode )
{

	m4return_t	iResult	;
	ClTempItem	*poItem ;


	poItem = new ClTempItem() ;
	CHECK_CHLOG_ERRORF( poItem == NULL, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempItem ) ) << __LINE__ << __FILE__ ) ;

	poItem->SetUpperPool( m_poUpperPool ) ;
	poItem->SetNoUpperPool( m_poNoUpperPool ) ;

	poItem->SetTi( m_poSystemTi ) ;


	iResult = poItem->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetItemId( ai_pccItemId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->SetSynonym( 0 ) ;

	iResult = poItem->SetName( ai_pccItemId) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->SetType( ai_iType ) ;
	poItem->SetScope( ai_iScope ) ;
	poItem->SetM4Type( ai_iM4Type ) ;
	poItem->SetDBPrecision( ai_iDBPrecision ) ;
	poItem->SetScale( ai_iScale ) ;

	iResult = poItem->SetCppType( this ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetDefaultValue( NULL, this ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetDmd( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetDmdComponent( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetDmdCrosstab( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetDmdField( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->SetSliceBhrv( M4CL_ITEM_SLICE_BHRV_BASE_VALUE ) ;
	poItem->SetSliceSplit( M4CL_ITEM_SLICE_SPLIT_LINEAR ) ;
	poItem->SetIdSliceTotType( M4CL_ITEM_TOTALIZE_FIRST ) ;
	poItem->SetIdTotalize( M4CL_ITEM_TOTALIZE_MAX ) ;
	poItem->SetTransactionMode( ai_iTransactionMode ) ;
	poItem->SetMethodLevel( 2 ) ;

	iResult = poItem->SetGroup( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetSliceItem( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetAuxItem( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetAuxItemProrat( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetReadObjectAlias( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetWriteObjectAlias( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetReadFieldId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetWriteFieldId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_AFFECTS_DB ) ;

	iResult = poItem->SetReadObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poItem->SetWriteObjectId( "" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->SetSentTotType( M4CL_ITEM_TOTALIZE_NONE ) ;
	poItem->SetOrder( 0 ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VISIBLE ) ;
	poItem->SetModifyData( M4CL_ITEM_MODIFY_NONE ) ;
	poItem->SetInternalType( ai_iInternalType ) ;
	poItem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_PUBLIC ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_PARAMETER ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_CLIENT_NOT_NULL ) ;

	iResult = poItem->SetT3Id( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_NODAL_ITEM ) ;

	iResult = poItem->SetNodeId( "", M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poItem->SetIsEvent( 0 ) ;
	poItem->SetConvertFunction( M4CL_CONV_FUNCT_NONE ) ;
	poItem->UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_PK ) ;
	poItem->SetCsType( ai_iCsType ) ;
	poItem->SetSyncType( M4_SYNC_NULL ) ;

	poItem->SetIsToCompile( 0 ) ;

	return( poItem ) ;
}


m4return_t	ClTempChannel :: _AddSystemItem( ClTempItem *ai_poSystemItem, ClTempNitem *ai_poSystemNitem, ClTempNode *ai_poNode, m4bool_t ai_bAddFormula, m4int8_t ai_iType, m4pcchar_t ai_pccObject, m4pcchar_t ai_pccMethod, m4uint16_t ai_iArgumentStart, m4uint16_t ai_iArgumentNumber, m4uint8_t ai_iCreationType )
{

	m4return_t			iResult ;
	m4uint16_t			i ;
	ClTempRitem			*poRitem ;
	ClTempNitemFormula	*poFormula ;
    ClTempArgument		**ppoArgument ;


	// Añadimos el item a su nitem
	iResult = ai_poSystemNitem->SetItem( this, ai_poNode, ai_poSystemItem, NULL ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Lo ponemos en el array global
	iResult = m_oNitems.AddElement( ai_poSystemNitem ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( ai_bAddFormula == M4_TRUE )
	{
		poRitem = ai_poSystemNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Creamos una nueva fórmula
		poFormula = new ClTempNitemFormula ;
		CHECK_CHLOG_ERRORF( poFormula == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula ) ) << __LINE__ << __FILE__ ) ;

		poFormula->SetUpperPool( m_poUpperPool ) ;
		poFormula->SetNoUpperPool( m_poNoUpperPool ) ;

		// Inicializamos la fórmula
		iResult = poFormula->Init( ai_poSystemNitem, ai_poNode, ai_iType, "", ai_pccObject, ai_pccMethod, M4CL_MINUS_INFINITE_IN_JULIAN, M4CL_PLUS_INFINITE_IN_JULIAN, ai_iCreationType, 0 ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// La añadimos al array
		iResult = poRitem->AddFormula( poFormula ) ;

		if( iResult != M4_SUCCESS )
		{
			delete( poFormula ) ;
		}

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Se asignan los argumentos
		if( ai_iArgumentNumber > 0 )
		{
			for( i = 0 ; i < ai_iArgumentNumber ; i++ )
			{
				ppoArgument = GetSystemArgumentPointerByPosition( ai_iArgumentStart + i ) ;

				if( i == 0 )
				{
					ai_poSystemItem->InitArguments( ppoArgument ) ;
				}

				ai_poSystemItem->AddArgument( *ppoArgument ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: _CreateSystemArguments( void )
{

    m4return_t		iResult ;
    ClTempArgument	*poArgument ;


	m_poSystemArgumentsArray = new ClTempArgument[ M4_MDFAC_SYSTEM_ARGUMENTS_NUMBER ] ;
	CHECK_CHLOG_ERRORF( m_poSystemArgumentsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempArgument ) * M4_MDFAC_SYSTEM_ARGUMENTS_NUMBER ) << __LINE__ << __FILE__ ) ;

	SetSystemArgumentsDelta( M4_MDFAC_SYSTEM_ARGUMENTS_NUMBER ) ;

	// Argumentos de ExecuteReport

	// OutputParam

	poArgument = &( m_poSystemArgumentsArray[ 0 ] ) ;

	poArgument->SetUpperPool( m_poUpperPool ) ;
	poArgument->SetNoUpperPool( m_poNoUpperPool ) ;

	iResult = poArgument->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetItemId( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetArgumentId( "OUTPUTPARAM" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetName( "Output Parameter" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poArgument->SetType( M4CL_ITEM_ARGUMENT_TYPE_INPUT ) ;
	poArgument->SetM4Type( M4CL_M4_TYPE_VAR_STRING ) ;
	poArgument->SetPrecision( 300 ) ;
	poArgument->SetOrder( 1 ) ;
	poArgument->SetRealOrder( 0 ) ;

    iResult = AddSystemArgument( poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// ManagerMode
	poArgument = &( m_poSystemArgumentsArray[ 1 ] ) ;

	poArgument->SetUpperPool( m_poUpperPool ) ;
	poArgument->SetNoUpperPool( m_poNoUpperPool ) ;

	iResult = poArgument->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetItemId( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetArgumentId( "MANAGERMODE" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetName( "Manager Mode" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poArgument->SetType( M4CL_ITEM_ARGUMENT_TYPE_INPUT ) ;
	poArgument->SetM4Type( M4CL_M4_TYPE_NUMBER ) ;
	poArgument->SetPrecision( 10 ) ;
	poArgument->SetOrder( 2 ) ;
	poArgument->SetRealOrder( 1 ) ;

    iResult = AddSystemArgument( poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// DesignerMode
	poArgument = &( m_poSystemArgumentsArray[ 2 ] ) ;

	poArgument->SetUpperPool( m_poUpperPool ) ;
	poArgument->SetNoUpperPool( m_poNoUpperPool ) ;

	iResult = poArgument->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetItemId( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetArgumentId( "DESIGNERMODE" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetName( "Designer Mode" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poArgument->SetType( M4CL_ITEM_ARGUMENT_TYPE_INPUT ) ;
	poArgument->SetM4Type( M4CL_M4_TYPE_NUMBER ) ;
	poArgument->SetPrecision( 10 ) ;
	poArgument->SetOrder( 3 ) ;
	poArgument->SetRealOrder( 2 ) ;

    iResult = AddSystemArgument( poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// DataMode
	poArgument = &( m_poSystemArgumentsArray[ 3 ] ) ;

	poArgument->SetUpperPool( m_poUpperPool ) ;
	poArgument->SetNoUpperPool( m_poNoUpperPool ) ;

	iResult = poArgument->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetItemId( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetArgumentId( "DATAMODE" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetName( "Data Mode" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poArgument->SetType( M4CL_ITEM_ARGUMENT_TYPE_INPUT ) ;
	poArgument->SetM4Type( M4CL_M4_TYPE_NUMBER ) ;
	poArgument->SetPrecision( 10 ) ;
	poArgument->SetOrder( 4 ) ;
	poArgument->SetRealOrder( 3 ) ;

    iResult = AddSystemArgument( poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// HandleReport
	poArgument = &( m_poSystemArgumentsArray[ 4 ] ) ;

	poArgument->SetUpperPool( m_poUpperPool ) ;
	poArgument->SetNoUpperPool( m_poNoUpperPool ) ;

	iResult = poArgument->SetTiId( M4MDFAC_TI_TI_SYSTEM, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetItemId( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetArgumentId( "HANDLEREPORT" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = poArgument->SetName( "Handle Report" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poArgument->SetType( M4CL_ITEM_ARGUMENT_TYPE_INPUT ) ;
	poArgument->SetM4Type( M4CL_M4_TYPE_VARIANT ) ;
	poArgument->SetPrecision( 30 ) ;
	poArgument->SetOrder( 5 ) ;
	poArgument->SetRealOrder( 4 ) ;

    iResult = AddSystemArgument( poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	
	return( M4_SUCCESS ) ;
}



ClTempSentence* ClTempChannel :: GetSentenceById( m4pcchar_t ai_pccSentenceId, m4bool_t ai_bConvert )
{

    ClTempSentence  **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccSentenceId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonSentence.SetIdentifier( ai_pccSentenceId, ai_bConvert ) ;

	ppoResult = ( ClTempSentence** ) m_oSentences.Find( M4ClCompareObjectsById, &m_poComparisonSentence ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}


ClTempTi*	ClTempChannel :: GetTiById( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert )
{

    ClTempTi   **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccTiId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonTi.SetIdentifier( ai_pccTiId, ai_bConvert ) ;

	ppoResult = ( ClTempTi** ) m_oTis.Find( M4ClCompareObjectsById, &m_poComparisonTi ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}


ClTempItem*	ClTempChannel :: GetItemByTiAndId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvertTi, m4pcchar_t ai_pccItemId, m4bool_t ai_bConvertItem )
{

    ClTempItem **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccTiId == NULL || ai_pccItemId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonItem.SetParentIdentifier( ai_pccTiId, ai_bConvertTi ) ;
    m_oComparisonItem.SetIdentifier( ai_pccItemId, ai_bConvertItem ) ;

	ppoResult = ( ClTempItem** ) m_oItems.Find( M4ClCompareObjectsByTwoIds, &m_poComparisonItem ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}


ClTempNode*	ClTempChannel :: GetNodeById( m4pcchar_t ai_pccNodeIdId, m4bool_t ai_bConvert )
{

    ClTempNode **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccNodeIdId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonNode.SetIdentifier( ai_pccNodeIdId, ai_bConvert ) ;

	ppoResult = ( ClTempNode** ) m_oNodes.Find( M4ClCompareObjectsById, &m_poComparisonNode ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}



ClTempNode* ClTempChannel :: GetNodeByTiId( m4pcchar_t ai_pccTiId, m4bool_t ai_bConvert, m4uint8_t ai_iLevel, ClTempNitem* ai_poNitem )
{

	m4uint8_t	i ;
	m4uint8_t	iInherits ;
	m4uint32_t	iReferences ;
	m4uint32_t	iNewReferences ;
    ClTempTi	*poTi ;
    ClTempNode	*poNode ;


	CHECK_CHLOG_DEBUGF( ai_pccTiId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poTi = GetTiById( ai_pccTiId, ai_bConvert ) ;

	if( poTi == NULL )
	{
        return( NULL ) ;
	}


// Buscamos en nuestro m4obj ==================================================

	iReferences = poTi->GetReferences( ai_iLevel ) ;

	// Si hay referencias la hemos encontrado en nuestro nivel
	if( iReferences != 0 )
	{
		poNode = poTi->GetNode( ai_iLevel ) ;
	}
	else
	{
// Buscamos en los m4objs padre ===============================================

		iReferences = 0 ;
		poNode = NULL ;

		iInherits = m4uint8_t( GetNumberOfT3Inherits() ) ;

		for( i = ai_iLevel + 1; i < iInherits ; i++ )
		{
			iNewReferences = poTi->GetReferences( i ) ;

			if( iNewReferences != 0 )
			{
				if( poNode == NULL )
				{
					poNode = poTi->GetNode( i ) ;
				}
				iReferences += iNewReferences ;
			}
		}
	}


	if( iReferences > 1 )
	{
		if( M4ChLogIsErrorIn( M4_MDFAC_AMBIGUOUS_TI_CALL_INSTALLING ) == 0 )
		{
			if( ai_poNitem != NULL )
			{
				DUMP_CHLOG_ERRORF( M4_MDFAC_AMBIGUOUS_TI_CALL_INSTALLING, ai_pccTiId << iReferences << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
			else
			{
				DUMP_CHLOG_ERRORF( M4_MDFAC_AMBIGUOUS_TI_CALL_INSTALLING, ai_pccTiId << iReferences << "ITEM_SYSTEM" << "NODE_SYSTEM" << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}
	}


    return( poNode ) ;
}



ClTempInstance*	ClTempChannel :: GetInstanceById( m4pcchar_t ai_pccInstanceId, m4bool_t ai_bConvert )
{

    ClTempInstance  **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccInstanceId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonInstance.SetIdentifier( ai_pccInstanceId, ai_bConvert ) ;

	ppoResult = ( ClTempInstance** ) m_oInstances.Find( M4ClCompareObjectsById, &m_poComparisonInstance ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}




ClTempT3Inherit*	ClTempChannel :: GetT3InheritByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempT3Inherit	*poResult ;


	iResult = m_oT3Inherits.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempT3Inherit* ) pvElement ;

	return( poResult ) ;
}


ClTempTi*   ClTempChannel :: GetTiByPosition( m4uint16_t ai_iPosition )
{

    m4return_t  iResult ;
    m4pvoid_t   pvElement ;
    ClTempTi    *poResult ;


	iResult = m_oTis.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTi* ) pvElement ;

	return( poResult ) ;
}



ClTempItem*	ClTempChannel :: GetItemByPosition( m4uint16_t ai_iPosition )
{

    m4return_t	iResult ;
    m4pvoid_t	pvElement ;
    ClTempItem	*poResult ;


	iResult = m_oItems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempItem* ) pvElement ;

	return( poResult ) ;
}



ClTempNode*	ClTempChannel :: GetNodeByPosition( m4uint16_t ai_iPosition )
{

    m4return_t  iResult ;
    m4pvoid_t   pvElement ;
    ClTempNode  *poResult ;


	iResult = m_oNodes.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNode* ) pvElement ;

	return( poResult ) ;
}



ClTempNitem*	ClTempChannel :: GetNitemByPosition( m4uint16_t ai_iPosition )
{

    m4return_t  iResult ;
    m4pvoid_t   pvElement ;
    ClTempNitem	*poResult ;


	iResult = m_oNitems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}


ClTempIndex*	ClTempChannel :: GetIndexByPosition( m4uint16_t ai_iPosition )
{

    m4return_t  iResult ;
    m4pvoid_t   pvElement ;
    ClTempIndex	*poResult ;


	iResult = m_oIndexes.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempIndex* ) pvElement ;

	return( poResult ) ;
}



ClTempPlug*	ClTempChannel :: GetPlugByPosition( m4uint16_t ai_iPosition )
{

    m4return_t  iResult ;
    m4pvoid_t   pvElement ;
    ClTempPlug	*poResult ;


	iResult = m_oPlugs.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempPlug* ) pvElement ;

	return( poResult ) ;
}



ClTempBusinessMethod*	ClTempChannel :: GetBusinessMethodByPosition( m4uint16_t ai_iPosition )
{

    m4return_t				iResult ;
    m4pvoid_t				pvElement ;
    ClTempBusinessMethod	*poResult ;


	iResult = m_oBusinessMethods.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempBusinessMethod* ) pvElement ;

	return( poResult ) ;
}



ClTempNodeInherit**	ClTempChannel :: GetNodeInheritPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			*ppvReference ;
    ClTempNodeInherit	**ppoResult ;


	iResult = m_oNodeInherits.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempNodeInherit** ) ppvReference ;

	return( ppoResult ) ;
}


ClTempTiInherit**	ClTempChannel :: GetTiInheritPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempTiInherit	**ppoResult ;


	iResult = m_oTiInherits.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempTiInherit** ) ppvReference ;

	return( ppoResult ) ;
}


ClTempItem**	ClTempChannel :: GetItemPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t	iResult ;
    m4pvoid_t	*ppvReference ;
    ClTempItem	**ppoResult ;


	iResult = m_oItems.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempItem** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempConcept**	ClTempChannel :: GetConceptPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempConcept	**ppoResult ;


	iResult = m_oConcepts.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempConcept** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempItemInherit**	ClTempChannel :: GetItemInheritPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			*ppvReference ;
    ClTempItemInherit	**ppoResult ;


	iResult = m_oItemInherits.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempItemInherit** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempArgument**   ClTempChannel :: GetArgumentPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempArgument	**ppoResult ;


	iResult = m_oArguments.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempArgument** ) ppvReference ;

	return( ppoResult ) ;
}


ClTempItemFormula**   ClTempChannel :: GetItemFormulaPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			*ppvReference ;
    ClTempItemFormula	**ppoResult ;


	iResult = m_oItemFormulas.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempItemFormula** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempTrigger**   ClTempChannel :: GetTriggerPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempTrigger	**ppoResult ;


	iResult = m_oTriggers.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempTrigger** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempTotal**   ClTempChannel :: GetTotalPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempTotal		**ppoResult ;


	iResult = m_oTotals.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempTotal** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempTotalTag**   ClTempChannel :: GetTotalTagPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempTotalTag	**ppoResult ;


	iResult = m_oTotalTags.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempTotalTag** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempTag**   ClTempChannel :: GetTagPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempTag		**ppoResult ;


	iResult = m_oTags.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempTag** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempNitem**   ClTempChannel :: GetNitemPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempNitem		**ppoResult ;


	iResult = m_oNitems.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempNitem** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempConnector**   ClTempChannel :: GetConnectorPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempConnector	**ppoResult ;


	iResult = m_oConnectors.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempConnector** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempConnector**   ClTempChannel :: GetInvConnectorPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempConnector	**ppoResult ;


	iResult = m_oInvConnectors.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempConnector** ) ppvReference ;

	return( ppoResult ) ;
}


ClTempNitemConn**   ClTempChannel :: GetNitemConnPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempNitemConn	**ppoResult ;


	iResult = m_oNitemConns.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempNitemConn** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempIndex**   ClTempChannel :: GetIndexPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempIndex		**ppoResult ;


	iResult = m_oIndexes.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempIndex** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempIndexItem**   ClTempChannel :: GetIndexItemPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			*ppvReference ;
    ClTempIndexItem		**ppoResult ;


	iResult = m_oIndexItems.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempIndexItem** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempPlug**   ClTempChannel :: GetPlugPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t	iResult ;
    m4pvoid_t	*ppvReference ;
    ClTempPlug	**ppoResult ;


	iResult = m_oPlugs.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempPlug** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempPlugItem**   ClTempChannel :: GetPlugItemPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempPlugItem	**ppoResult ;


	iResult = m_oPlugItems.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempPlugItem** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempAlias**   ClTempChannel :: GetAliasPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempAlias		**ppoResult ;


	iResult = m_oAlias.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempAlias** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempT3Alias**   ClTempChannel :: GetT3AliasPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempT3Alias	**ppoResult ;


	iResult = m_oT3Alias.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempT3Alias** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempFilter**   ClTempChannel :: GetFilterPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempFilter	**ppoResult ;


	iResult = m_oFilters.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempFilter** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempBusinessArg**   ClTempChannel :: GetBusinessArgPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			*ppvReference ;
    ClTempBusinessArg	**ppoResult ;


	iResult = m_oBusinessArgs.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempBusinessArg** ) ppvReference ;

	return( ppoResult ) ;
}



ClTempArgument**   ClTempChannel :: GetSystemArgumentPointerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		*ppvReference ;
    ClTempArgument	**ppoResult ;


	iResult = m_oSystemArguments.GetElementPointer( ai_iPosition, ppvReference ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ppoResult = ( ClTempArgument** ) ppvReference ;

	return( ppoResult ) ;
}




// Otras funciones de escritura ===============================================

m4return_t	ClTempChannel :: AddT3Inherit( ClTempT3Inherit *ai_poT3Inherit )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poT3Inherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oT3Inherits.AddElement( ai_poT3Inherit ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddSentence( ClTempSentence *ai_poSentence )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poSentence == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oSentences.AddElement( ai_poSentence ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddRSM( ClTempRSM *ai_poRSM )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poRSM == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oRSMs.AddElement( ai_poRSM ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddTi( ClTempTi *ai_poTi )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTis.AddElement( ai_poTi ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddTiInherit( ClTempTiInherit *ai_poTiInherit )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTiInherits.AddElement( ai_poTiInherit ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddItem( ClTempItem *ai_poItem )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oItems.AddElement( ai_poItem ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddItemInherit( ClTempItemInherit *ai_poItemInherit )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poItemInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oItemInherits.AddElement( ai_poItemInherit ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddConcept( ClTempConcept *ai_poConcept )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poConcept == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oConcepts.AddElement( ai_poConcept ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddArgument( ClTempArgument *ai_poArgument )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oArguments.AddElement( ai_poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddItemFormula( ClTempItemFormula *ai_poItemFormula )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oItemFormulas.AddElement( ai_poItemFormula ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddTrigger( ClTempTrigger *ai_poTrigger )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTrigger == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTriggers.AddElement( ai_poTrigger ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddTotal( ClTempTotal *ai_poTotal )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTotals.AddElement( ai_poTotal ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddTotalTag( ClTempTotalTag *ai_poTotalTag )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTotalTags.AddElement( ai_poTotalTag ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddTag( ClTempTag *ai_poTag )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTags.AddElement( ai_poTag ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddNode( ClTempNode *ai_poNode )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oNodes.AddElement( ai_poNode ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddNodeInherit( ClTempNodeInherit *ai_poNodeInherit )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poNodeInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oNodeInherits.AddElement( ai_poNodeInherit ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddConnector( ClTempConnector *ai_poConnector )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oConnectors.AddElement( ai_poConnector ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddInvConnector( ClTempConnector *ai_poConnector )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oInvConnectors.AddElement( ai_poConnector ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddNitemConn( ClTempNitemConn *ai_poNitemConn )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oNitemConns.AddElement( ai_poNitemConn ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddIndex( ClTempIndex *ai_poIndex )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poIndex == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oIndexes.AddElement( ai_poIndex ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: AddIndexItem( ClTempIndexItem *ai_poIndexItem )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poIndexItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oIndexItems.AddElement( ai_poIndexItem ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddPlug( ClTempPlug *ai_poPlug )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poPlug == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oPlugs.AddElement( ai_poPlug ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddPlugItem( ClTempPlugItem *ai_poPlugItem )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poPlugItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oPlugItems.AddElement( ai_poPlugItem ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddAlias( ClTempAlias *ai_poAlias )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poAlias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oAlias.AddElement( ai_poAlias ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddT3Alias( ClTempT3Alias *ai_poT3Alias )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poT3Alias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oT3Alias.AddElement( ai_poT3Alias ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddFilter( ClTempFilter *ai_poFilter )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poFilter == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oFilters.AddElement( ai_poFilter ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddInstance( ClTempInstance *ai_poInstance )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poInstance == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oInstances.AddElement( ai_poInstance ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: AddInstance( ClTempInstance *ao_poInstance, m4pcchar_t ai_pccInstanceId, m4pcchar_t ai_pccT3Id, m4pcchar_t ai_pccRoleId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iScope, m4uint8_t ai_iOpenMode, m4uint8_t ai_iCsAccess, m4uint8_t ai_iUseRole, m4uint8_t ai_iOrgType )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ao_poInstance == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	ao_poInstance->SetUpperPool( m_poUpperPool ) ;


	iResult = ao_poInstance->SetInstanceId( ai_pccInstanceId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = ao_poInstance->SetT3Id( ai_pccT3Id ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = ao_poInstance->SetRoleId( ai_pccRoleId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = ao_poInstance->SetOrgId( ai_pccOrgId ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
    ao_poInstance->SetScope( ai_iScope ) ;
    ao_poInstance->SetOpenMode( ai_iOpenMode ) ;
    ao_poInstance->SetCsAccess( ai_iCsAccess ) ;
    ao_poInstance->SetUseRole( ai_iUseRole ) ;
    ao_poInstance->SetOrgType( ai_iOrgType ) ;


    iResult = AddInstance( ao_poInstance ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddBusinessMethod( ClTempBusinessMethod *ai_poBusinessMethod )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poBusinessMethod == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oBusinessMethods.AddElement( ai_poBusinessMethod ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddBusinessArg( ClTempBusinessArg *ai_poBusinessArg )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poBusinessArg == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oBusinessArgs.AddElement( ai_poBusinessArg ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: AddSystemArgument( ClTempArgument *ai_poArgument )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oSystemArguments.AddElement( ai_poArgument ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillTisWithItems( void )
{

    m4bool_t			bFound ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    ClTempTi			*poTi ;
    ClTempTi			*poLastTi ;
    ClTempItem			*poItem ;
    ClTempItem			**ppoItem ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oItems.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al item dentro del array de punteros
		ppoItem = GetItemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = *ppoItem ;
		CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poItem->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Items" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitItems( ppoItem ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de items que tiene esta Ti
		poTi->AddItem() ;

		// Guardamos la TI en el item para el identificador de TI
		poItem->SetTi( poTi ) ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillTisWithItemInherits( void )
{

    m4bool_t			bFound ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    ClTempTi			*poTi ;
    ClTempTi			*poLastTi ;
    ClTempItemInherit	*poItemInherit ;
    ClTempItemInherit	**ppoItemInherit ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oItemInherits.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al Item Inherit dentro del array de punteros
		ppoItemInherit = GetItemInheritPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoItemInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItemInherit = *ppoItemInherit ;
		CHECK_CHLOG_DEBUGF( poItemInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poItemInherit->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "ItemInherits" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitItemInherits( ppoItemInherit ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de Item Inherits que tiene esta Ti
		poTi->AddItemInherit() ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillTisWithConcepts( void )
{

    m4bool_t			bFound ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    ClTempTi			*poTi ;
    ClTempTi			*poLastTi ;
    ClTempConcept		*poConcept ;
    ClTempConcept		**ppoConcept ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oConcepts.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al cancepto dentro del array de punteros
		ppoConcept = GetConceptPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoConcept == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConcept = *ppoConcept ;
		CHECK_CHLOG_DEBUGF( poConcept == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poConcept->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Concepts" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitConcepts( ppoConcept ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de conceptos que tiene esta Ti
		poTi->AddConcept() ;
	}

    return( M4_SUCCESS ) ;
}





m4return_t	ClTempChannel :: FillTisWithInherits( void )
{

    m4return_t			iResult ;
    m4bool_t			bFound ;
	m4int8_t			iSentenceNumber ;
    m4uint16_t			i, j ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4uint16_t			iInherits ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    m4pcchar_t			pccObject ;
    ClTempTi			*poTi ;
    ClTempTi			*poBaseTi ;
    ClTempTi			*poNewBaseTi ;
    ClTempTi			*poLastTi ;
	ClTempSentence		*poSentence ;
    ClTempTiInherit		*poTiInherit ;
    ClTempTiInherit		*poBaseTiInherit ;
    ClTempTiInherit		**ppoTiInherit ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oTiInherits.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero a la Ti inherit dentro del array de punteros
		ppoTiInherit = GetTiInheritPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTiInherit = *ppoTiInherit ;
		CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poTiInherit->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Ti Inherits" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitInherits( ppoTiInherit ) ;
			poLastTi = poTi ;
		}

		// Comprobamos que no esté ya la Ti, porque sino hay ciclos
		poNewBaseTi = poTiInherit->GetBaseTi() ;

		iInherits = poTi->GetNumberOfInherits() ;

		for( j = 0 ; j < iInherits ; j++ )
		{
			poBaseTiInherit = poTi->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poBaseTi = poBaseTiInherit->GetBaseTi() ;
			CHECK_CHLOG_ERRORF( poBaseTi == poNewBaseTi, M4_ERROR, M4_MDFAC_TI_INHERIT_CYCLE, poNewBaseTi->GetTiId() << poTi->GetTiId() << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}

		// Incrementamos el número de herencias que tiene esta Ti
		poTi->AddInherit() ;


		/* Bug 0069889
		Aquí se calcula el número de sentencias que va a tener cada TI
			-1 si es dinámica
			0 si no tiene sentencia
			n si tiene 1 ó más sentencias
		Si ya es dinámica siempre será dinámica independientemente de sus padres.
		Si la que viene es dinámica y todavía no tiene sentencia se queda dinámica.
		Si la que viene tiene sentencia de base de datos se incrementa el número de ellas.
		*/
		iSentenceNumber = poTi->GetSentenceNumber() ;

		if( iSentenceNumber != -1 )
		{
			if( poNewBaseTi->GetGenerateSql() == 1 )
			{
				if( iSentenceNumber == 0 )
				{
					poTi->SetSentenceNumber( -1 ) ;
				}
			}
			else
			{
				if( poNewBaseTi->GetReadSentence() != NULL )
				{
					poTi->SetSentenceNumber( iSentenceNumber + 1 ) ;
				}
			}
		}

		pccObject = poTi->GetReadObjectId() ;

		if( *pccObject == '\0' )
		{
			pccObject = poNewBaseTi->GetReadObjectId() ;

			if( *pccObject != '\0' )
			{
				iResult = poTi->SetReadObjectId( pccObject ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		poSentence = poTi->GetWriteSentence() ;

		if( poSentence == NULL )
		{
			poSentence = poNewBaseTi->GetWriteSentence() ;

			if( poSentence != NULL )
			{
				poTi->SetWriteSentence( poSentence ) ;
			}
		}

		pccObject = poTi->GetWriteObjectId() ;

		if( *pccObject == '\0' )
		{
			pccObject = poNewBaseTi->GetWriteObjectId() ;

			if( *pccObject != '\0' )
			{
				iResult = poTi->SetWriteObjectId( pccObject ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillItemsWithArguments( void )
{

    m4bool_t			bFound ;
	m4uint8_t			iRealOrder ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iItem ;
    m4uint16_t			iItemNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccItem ;
    m4pcchar_t			pccStoredTi ;
    m4pcchar_t			pccStoredItem ;
    ClTempItem			*poItem ;
    ClTempItem			*poLastItem ;
    ClTempArgument		*poArgument ;
    ClTempArgument		**ppoArgument ;

    
	iRealOrder = 0 ;
	iItem = 0 ;
	poLastItem = NULL ;

    iLength = m_oArguments.GetLength() ;
    iItemNumber = m_oItems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al item dentro del array de punteros
		ppoArgument = GetArgumentPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poArgument = *ppoArgument ;
		CHECK_CHLOG_DEBUGF( poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poArgument->GetTiId() ;
		pccItem = poArgument->GetItemId() ;

        bFound = M4_FALSE ;
        poItem = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iItem < iItemNumber )
		{
			poItem = GetItemByPosition( iItem ) ;
			CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poItem->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredItem = poItem->GetItemId() ;
			CHECK_CHLOG_DEBUGF( pccStoredItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 && M4MdFacStrcmp( pccItem, pccStoredItem ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iItem++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		// Es porque antes no lo controlabamos por el tema de los items nodales
		// Ahora lo controlamos pero damos warning, para no ser severos
		// Eso si, reseteo los items para que empiece otra vez la búsqueda
		if( bFound == M4_FALSE )
		{
			DUMP_CHLOG_WARNINGF( M4_MDFAC_NO_SUCH_ITEM_IN_TI, pccItem << pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Item Arguments" ) ;
			iItem = 0 ;
			continue ;
		}


		// Establecemos las referencias si es un nuevo item
		// Si es un nuevo item reseteamos el orden real
		if( poItem != poLastItem )
		{
			iRealOrder = 0 ;
			poItem->InitArguments( ppoArgument ) ;
			poLastItem = poItem ;
		}

		// Incrementamos el número de argumentos que tiene este item
		poItem->AddArgument( poArgument ) ;

		// Establecemos el orden real
		poArgument->SetRealOrder( iRealOrder ) ;

		iRealOrder++ ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillTisWithItemFormulas( void )
{

    m4bool_t			bFound ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    ClTempTi			*poTi ;
    ClTempTi			*poLastTi ;
    ClTempItemFormula	*poItemFormula ;
    ClTempItemFormula	**ppoItemFormula ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oItemFormulas.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero a la fórmula dentro del array de punteros
		ppoItemFormula = GetItemFormulaPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItemFormula = *ppoItemFormula ;
		CHECK_CHLOG_DEBUGF( poItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poItemFormula->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Item Formulas" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitItemFormulas( ppoItemFormula ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de formulas que tiene esta Ti
		poTi->AddItemFormula() ;

		// Asignamos la TI de la fórmula
		poItemFormula->SetTi( poTi ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempChannel :: FillTisWithTriggers( void )
{

    m4bool_t			bFound ;
    m4uint16_t			i ;
    m4uint16_t			iLength ;
    m4uint16_t			iTi ;
    m4uint16_t			iTiNumber ;
    m4pcchar_t			pccTi ;
    m4pcchar_t			pccStoredTi ;
    ClTempTi			*poTi ;
    ClTempTi			*poLastTi ;
    ClTempTrigger		*poTrigger ;
    ClTempTrigger		**ppoTrigger ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oTriggers.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al trigger dentro del array de punteros
		ppoTrigger = GetTriggerPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoTrigger == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTrigger = *ppoTrigger ;
		CHECK_CHLOG_DEBUGF( poTrigger == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poTrigger->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Triggers" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitTriggers( ppoTrigger ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de triggers que tiene esta Ti
		poTi->AddTrigger() ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillTisWithTotals( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iTi ;
    m4uint16_t		iTiNumber ;
    m4pcchar_t		pccTi ;
    m4pcchar_t		pccStoredTi ;
    ClTempTi		*poTi ;
    ClTempTi		*poLastTi ;
    ClTempTotal		*poTotal ;
    ClTempTotal		**ppoTotal ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oTotals.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al total dentro del array de punteros
		ppoTotal = GetTotalPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTotal = *ppoTotal ;
		CHECK_CHLOG_DEBUGF( poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poTotal->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Totals" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitTotals( ppoTotal ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de totales que tiene esta Ti
		poTi->AddTotal() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillTisWithTotalTags( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iTi ;
    m4uint16_t		iTiNumber ;
    m4pcchar_t		pccTi ;
    m4pcchar_t		pccStoredTi ;
    ClTempTi		*poTi ;
    ClTempTi		*poLastTi ;
    ClTempTotalTag	*poTotalTag ;
    ClTempTotalTag	**ppoTotalTag ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oTotalTags.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero a la condiciones de total dentro del array de punteros
		ppoTotalTag = GetTotalTagPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTotalTag = *ppoTotalTag ;
		CHECK_CHLOG_DEBUGF( poTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poTotalTag->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Total Tags" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitTotalTags( ppoTotalTag ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de condiciones de total que tiene esta Ti
		poTi->AddTotalTag() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillTisWithTags( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iTi ;
    m4uint16_t		iTiNumber ;
    m4pcchar_t		pccTi ;
    m4pcchar_t		pccStoredTi ;
    ClTempTi		*poTi ;
    ClTempTi		*poLastTi ;
    ClTempTag		*poTag ;
    ClTempTag		**ppoTag ;

    
	iTi = 0 ;
	poLastTi = NULL ;

    iLength = m_oTags.GetLength() ;
    iTiNumber = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al tag dentro del array de punteros
		ppoTag = GetTagPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTag = *ppoTag ;
		CHECK_CHLOG_DEBUGF( poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccTi = poTag->GetTiId() ;

		bFound = M4_FALSE ;
		poTi = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iTi < iTiNumber )
		{
			poTi = GetTiByPosition( iTi ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredTi = poTi->GetTiId() ;
			CHECK_CHLOG_DEBUGF( pccStoredTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccTi, pccStoredTi ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iTi++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_TI, pccTi << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Tags" ) ;


		// Establecemos las referencias si es una nueva Ti
		if( poTi != poLastTi )
		{
			poTi->InitTags( ppoTag ) ;
			poLastTi = poTi ;
		}

		// Incrementamos el número de tags que tiene esta Ti
		poTi->AddTag() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillNodesWithInherits( void )
{

    m4bool_t			bFound ;
    m4uint8_t			iLevel ;
    m4uint8_t			iNodeLevel ;
    m4uint8_t			iStoredLevel ;
    m4uint16_t			i, j ;
    m4uint16_t			iLength ;
    m4uint16_t			iNode ;
    m4uint16_t			iNodeNumber ;
    m4uint16_t			iInherits ;
    m4pcchar_t			pccM4ObjId ;
    m4pcchar_t			pccNodeM4ObjId ;
    m4pcchar_t			pccNodeId ;
    m4pcchar_t			pccStoredNodeId ;
    ClTempNode			*poNode ;
    ClTempNode			*poLastNode ;
	ClTempT3Inherit		*poT3Inherit ;
    ClTempNodeInherit	*poNodeInherit ;
    ClTempNodeInherit	*poStoredNodeInherit ;
    ClTempNodeInherit	**ppoNodeInherit ;

    
	iNode = 0 ;
	poLastNode = NULL ;

    iLength = m_oNodeInherits.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al node inherit dentro del array de punteros
		ppoNodeInherit = GetNodeInheritPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNodeInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNodeInherit = *ppoNodeInherit ;
		CHECK_CHLOG_DEBUGF( poNodeInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poNodeInherit->GetNodeId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Node Inherits" ) ;


		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitInherits( ppoNodeInherit ) ;
			poLastNode = poNode ;

			// Asignamos la nueva Ti sobreescrita (o no)
			poNode->SetTi( poNodeInherit->GetTi() ) ;
		}


		// Se comprueba que no haya ningun nivel mayor que el del nodo

		iLevel = poNodeInherit->GetLevel() ;
		iNodeLevel = poNode->GetLevel() ;

		if( iLevel > iNodeLevel )
		{
			poT3Inherit = GetT3InheritByLevel( iLevel ) ;
			pccM4ObjId = poT3Inherit->GetBaseT3Id() ;
			poT3Inherit = GetT3InheritByLevel( iNodeLevel ) ;
			pccNodeM4ObjId = poT3Inherit->GetBaseT3Id() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_NODE_OVERWRITE, pccM4ObjId << pccNodeM4ObjId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}


		// Se comprueba que no haya ninguna sobreescritura repetida

		iInherits = poNode->GetNumberOfInherits() ;

		for( j = 0 ; j < iInherits ; j++ )
		{
			poStoredNodeInherit = poNode->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poStoredNodeInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iStoredLevel = poStoredNodeInherit->GetLevel() ;

			if( iStoredLevel == iLevel )
			{
				poT3Inherit = GetT3InheritByLevel( iStoredLevel ) ;
				pccM4ObjId = poT3Inherit->GetBaseT3Id() ;

				DUMP_CHLOG_ERRORF( M4_MDFAC_REPEATED_NODE_INHERIT, pccM4ObjId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
				return( M4_ERROR ) ;
			}
		}


		// Incrementamos el número de node inherits que tiene este nodo
		poNode->AddInherit() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: FillNodesWithItems( void )
{

    m4return_t          iResult ;
    m4bool_t			bFound ;
    m4bool_t			bAddSysDBItems ;
    m4bool_t			bAddExecuteReportItem ;
    m4bool_t			bAddDynFilterItem ;
    m4bool_t			bClientSysLoadAdded ;
    m4bool_t			bClientSysPersistAdded ;
    m4bool_t			bServerSysLoadAdded ;
    m4bool_t			bServerSysPersistAdded ;
    m4bool_t			bExecuteReportAdded ;
    m4bool_t			bHasClientSysLoad ;
    m4bool_t			bHasClientSysPersist ;
    m4bool_t			bHasServerSysLoad ;
    m4bool_t			bHasServerSysPersist ;
    m4bool_t			bHasExecuteReport ;
    m4bool_t			bHasDynFilter ;
	m4uint8_t			iIsRoot ;
	m4uint8_t			iDynFilter ;
    m4uint8_t           iLevel ;
    m4uint8_t           iType ;
    m4uint8_t           iScope ;
    m4uint8_t           iCsType ;
    m4uint8_t           iIsParameter ;
	m4uint8_t			iInheritType ;
	m4uint8_t			iSystemType ;
    m4uint8_t			iInternalType ;
    m4uint16_t          i, j, k ;
    m4uint16_t			iLength ;
    m4uint16_t			iNode ;
    m4uint16_t			iNodeNumber ;
    m4uint16_t          iItems ;
    m4uint16_t          iSystemItems ;
    m4uint16_t          iParameters ;
    m4uint16_t          iItemNumber ;
    m4uint16_t          iTiInherits ;
    m4uint16_t          iNodeInherits ;
    m4uint16_t          iNextInherit ;
	m4pcchar_t          pccItemId ;
	m4pcchar_t          pccLastItemId ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccStoredNodeId ;
	m4pcchar_t          pccThisT3Id ;
    m4pcchar_t          pccThisNodeId ;
    m4pcchar_t          pccItemT3Id ;
    m4pcchar_t          pccItemNodeId ;
    m4pvoid_t           pvPointer ;
	ClTempRitem			*poRitem ;
    ClTempItem          *poItem ;
    ClTempItem          *poSystemItem ;
    ClTempNitem         *poNitem ;
    ClTempNitem         **ppoNitem ;
    ClTempNitem         *poSystemNitem ;
    ClTempNitem         *poNitemsArray ;
    ClTempTi            *poTi ;
    ClTempTi            *poBaseTi ;
    ClTempTi            *poNextTi ;
    ClTempTiInherit     *poTiInherit ;
    ClTempNodeInherit	*poNodeInherit ;
    ClTempNode          *poNode ;
    ClTempNode          *poLastNode ;


	if( m_poSystemNode != NULL )
	{
		iSystemItems = m_poSystemNode->GetNumberOfNitems() ;
	}
	else
	{
		iSystemItems = 0 ;
	}


// Recorremos los nodos en una primera pasada para calcular el tamaño =========

	iItemNumber = 0 ;
	iParameters = 0 ;

	bClientSysLoadAdded = M4_FALSE ;
	bClientSysPersistAdded = M4_FALSE ;
	bServerSysLoadAdded = M4_FALSE ;
	bServerSysPersistAdded = M4_FALSE ;
	bExecuteReportAdded = M4_FALSE ;

    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iNodeNumber ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        poTi = poNode->GetTi() ;

		// En el primer nodo raiz mapped se meten los items de carga y persistencia de canal
		iIsRoot = poNode->GetIsRoot() ;
		iCsType = poNode->GetCsType() ;

		if( m_poSystemTi != NULL && iIsRoot == 1 && iCsType == M4CL_CSTYPE_MAPPED )
		{
			bAddSysDBItems = M4_TRUE ;
		}
		else
		{
			bAddSysDBItems = M4_FALSE ;
		}

		// En el primer nodo raiz se mete el item ExecuteReport
		if( m_poSystemTi != NULL && iIsRoot == 1 )
		{
			bAddExecuteReportItem = M4_TRUE ;
		}
		else
		{
			bAddExecuteReportItem = M4_FALSE ;
		}

		// En los nodos con filtro dinámico se mete el item DynFilter
		iDynFilter = poNode->GetDynFilter() ;

		if( m_poSystemTi != NULL && iDynFilter == 1 )
		{
			bAddDynFilterItem = M4_TRUE ;
		}
		else
		{
			bAddDynFilterItem = M4_FALSE ;
		}

		bHasClientSysLoad = M4_FALSE ;
		bHasClientSysPersist = M4_FALSE ;
		bHasServerSysLoad = M4_FALSE ;
		bHasServerSysPersist = M4_FALSE ;
		bHasExecuteReport = M4_FALSE ;
		bHasDynFilter = M4_FALSE ;

		m4pcchar_t pTiId = poNode->GetTiId();

		// Leemos la herencia normal
		iTiInherits = poTi->GetNumberOfInherits() ;

		for( j = 0 ; j < iTiInherits ; j++ )
		{
			poTiInherit = poTi->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poBaseTi = poTiInherit->GetBaseTi() ;
			CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iItems = poBaseTi->GetNumberOfItems() ;

			iItemNumber += poBaseTi->GetNumberOfItems() ;

			for( k = 0 ; k < iItems ; k++ )
			{
				poItem = poBaseTi->GetItemByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Si es un item nodal de otro nodo no lo metemos
				pccItemT3Id = poItem->GetT3Id() ;

				if( pccItemT3Id != NULL && *pccItemT3Id != '\0' )
				{
					// Va aquí porque es del pool
					pccThisT3Id = GetChannelId() ;

					// Para que los items nodales se hereden
					if( _GetT3InheritById( pccItemT3Id ) == NULL )
					{
						iItemNumber-- ;
						continue ;
					}
					else
					{
						pccItemNodeId = poItem->GetNodeId() ;

						if( pccItemNodeId != NULL && *pccItemNodeId != '\0' )
						{
							// Va aquí porque es del pool
							pccThisNodeId = poNode->GetNodeId() ;
		
							if( M4MdFacStrcmp( pccItemNodeId, pccThisNodeId ) != 0 )
							{
								iItemNumber-- ;
								continue ;
							}
						}
					}
				}
				
// Si es uno del sistema sobreescrito no lo descontamos, porque no es lo normal

				// Contamos los parámetros
				iIsParameter = poItem->GetIsParameter() ;

				if( iIsParameter == 1 )
				{
					iType = poItem->GetType() ;
					iScope = poItem->GetScope() ;
					CHECK_CHLOG_ERRORF( iScope != M4CL_ITEM_SCOPE_NODE || iType == M4CL_ITEM_TYPE_METHOD, M4_ERROR, M4_MDFAC_BAD_PARAMETER, LOG_CAT_AND( poItem->iItem(), poItem->nItem() ) << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << iType << iScope ) ;

					iParameters++ ;
				}

				if( bAddSysDBItems == M4_TRUE )
				{
					pccItemId = poItem->GetIdentifier() ;

					// Se mira si hay que añadir el item SYS_LOAD_CLIENT
					if( bClientSysLoadAdded == M4_FALSE && bHasClientSysLoad == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_CLIENT ) == 0 )
						{
							bHasClientSysLoad = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_PERSIST_CLIENT
					if( bClientSysPersistAdded == M4_FALSE && bHasClientSysPersist == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_CLIENT ) == 0 )
						{
							bHasClientSysPersist = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_LOAD_SERVER
					if( bServerSysLoadAdded == M4_FALSE && bHasServerSysLoad == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_SERVER ) == 0 )
						{
							bHasServerSysLoad = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_PERSIST_SERVER
					if( bServerSysPersistAdded == M4_FALSE && bHasServerSysPersist == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_SERVER ) == 0 )
						{
							bHasServerSysPersist = M4_TRUE ;
						}
					}
				}

					// Se mira si hay que añadir el item EXECUTEREPORT
				if( bAddExecuteReportItem == M4_TRUE && bExecuteReportAdded == M4_FALSE && bHasExecuteReport == M4_FALSE )
				{
					pccItemId = poItem->GetIdentifier() ;

					if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT ) == 0 )
					{
						bHasExecuteReport = M4_TRUE ;
					}
				}

				// Se mira si hay que añadir el item DYN_FILTER
				if( bAddDynFilterItem == M4_TRUE && bHasDynFilter == M4_FALSE )
				{
					pccItemId = poItem->GetIdentifier() ;
					iInternalType = poItem->GetInternalType() ;

					if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_DYN_FILTER ) == 0 || iInternalType == M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE )
					{
						bHasDynFilter = M4_TRUE ;
					}
				}
			}
        }


		// Leemos la herencia del sistema
		iInheritType = poTi->GetInheritanceType() ;

		for( k = 0 ; k < iSystemItems ; k++ )
		{
			poSystemNitem = m_poSystemNode->GetNitemByPosition( k ) ;
			CHECK_CHLOG_DEBUGF( poSystemNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poSystemItem = poSystemNitem->GetItem() ;

			iInternalType = poSystemItem->GetInternalType() ;
			iSystemType = 1 << ( iInternalType - M4CL_INTERNAL_TYPE_SYSTEM_START ) ;

			if( iSystemType & iInheritType )
			{
				iItemNumber++ ;
			}
		}

		if( bAddSysDBItems == M4_TRUE )
		{
			// Se cuenta el item SYS_LOAD_CLIENT
			if( bClientSysLoadAdded == M4_FALSE && bHasClientSysLoad == M4_FALSE )
			{
				bClientSysLoadAdded = M4_TRUE ;
				iItemNumber++ ;
			}

			// Se cuenta el item SYS_PERSIST_CLIENT
			if( bClientSysPersistAdded == M4_FALSE && bHasClientSysPersist == M4_FALSE )
			{
				bClientSysPersistAdded = M4_TRUE ;
				iItemNumber++ ;
			}

			// Se cuenta el item SYS_LOAD_SERVER
			if( bServerSysLoadAdded == M4_FALSE && bHasServerSysLoad == M4_FALSE )
			{
				bServerSysLoadAdded = M4_TRUE ;
				iItemNumber++ ;
			}

			// Se cuenta el item SYS_PERSIST_SERVER
			if( bServerSysPersistAdded == M4_FALSE && bHasServerSysPersist == M4_FALSE )
			{
				bServerSysPersistAdded = M4_TRUE ;
				iItemNumber++ ;
			}
		}

		if( bAddExecuteReportItem == M4_TRUE && bExecuteReportAdded == M4_FALSE && bHasExecuteReport == M4_FALSE )
		{
			bExecuteReportAdded = M4_TRUE ;
			iItemNumber++ ;
		}

		// Se cuenta el item DYN_FILTER
		if( bAddDynFilterItem == M4_TRUE && bHasDynFilter == M4_FALSE )
		{
			iItemNumber++ ;
		}
	}


// Alojamos la memoria necesaria ==============================================

	if( iItemNumber > 0 )
	{
		poNitemsArray = new ClTempNitem[ iItemNumber ] ;
		CHECK_CHLOG_ERRORF( poNitemsArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitem ) * iItemNumber ) << __LINE__ << __FILE__ ) ;

		SetNitemsArray( poNitemsArray ) ;
		SetNitemsDelta( iItemNumber ) ;
	}

	m_oParameters.SetDelta( iParameters ) ;


// Recorremos los nodos en una segunda pasada asignando =======================

	iItemNumber = 0 ;

	bClientSysLoadAdded = M4_FALSE ;
	bClientSysPersistAdded = M4_FALSE ;
	bServerSysLoadAdded = M4_FALSE ;
	bServerSysPersistAdded = M4_FALSE ;
	bExecuteReportAdded = M4_FALSE ;

    for( i = 0 ; i < iNodeNumber ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		poTi = poNode->GetTi() ;

		// En el primer nodo raiz mapped se meten los items de carga y persistencia de canal
		iIsRoot = poNode->GetIsRoot() ;
		iCsType = poNode->GetCsType() ;

		if( m_poSystemTi != NULL && iIsRoot == 1 && iCsType == M4CL_CSTYPE_MAPPED )
		{
			bAddSysDBItems = M4_TRUE ;
		}
		else
		{
			bAddSysDBItems = M4_FALSE ;
		}

		// En el primer nodo raiz se mete el item ExecuteReport
		if( m_poSystemTi != NULL && iIsRoot == 1 )
		{
			bAddExecuteReportItem = M4_TRUE ;
		}
		else
		{
			bAddExecuteReportItem = M4_FALSE ;
		}

		// En los nodos con filtro dinámico se mete el item DynFilter
		iDynFilter = poNode->GetDynFilter() ;

		if( m_poSystemTi != NULL && iDynFilter == 1 )
		{
			bAddDynFilterItem = M4_TRUE ;
		}
		else
		{
			bAddDynFilterItem = M4_FALSE ;
		}

		bHasClientSysLoad = M4_FALSE ;
		bHasClientSysPersist = M4_FALSE ;
		bHasServerSysLoad = M4_FALSE ;
		bHasServerSysPersist = M4_FALSE ;
		bHasExecuteReport = M4_FALSE ;
		bHasDynFilter = M4_FALSE ;

		// Reseteamos los items del sistema que entran
		for( k = 0 ; k < iSystemItems ; k++ )
		{
			poSystemNitem = m_poSystemNode->GetNitemByPosition( k ) ;
			CHECK_CHLOG_DEBUGF( poSystemNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poSystemItem = poSystemNitem->GetItem() ;

			poSystemItem->SetIsToInclude( 1 ) ;
		}

		
		// Inicalizamos los niveles de herencia de nodos
		iNodeInherits = poNode->GetNumberOfInherits() ;
		CHECK_CHLOG_DEBUGF( iNodeInherits == 0, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iNextInherit = 0 ;
		poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
		poNextTi = poNodeInherit->GetTi() ;
		iLevel = poNodeInherit->GetLevel() ;


		// Leemos la herencia normal
		iTiInherits = poTi->GetNumberOfInherits() ;

		for( j = 0 ; j < iTiInherits ; j++ )
		{
			poTiInherit = poTi->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poBaseTi = poTiInherit->GetBaseTi() ;
			CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// Calculamos el nivel de los items de esta TI
			if( iNextInherit < iNodeInherits )
			{
				if( poNextTi == poBaseTi )
				{
					poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
					iLevel = poNodeInherit->GetLevel() ;
					iNextInherit++ ;

					if( iNextInherit < iNodeInherits )
					{
						poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
						poNextTi = poNodeInherit->GetTi() ;
					}
				}
			}

			// Asignamos el nodo y las referencias a la TI
			poBaseTi->SetNode( poNode, iLevel ) ;


			iItems = poBaseTi->GetNumberOfItems() ;

			for( k = 0 ; k < iItems ; k++ )
			{
				poItem = poBaseTi->GetItemByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Si es un item nodal de otro nodo no lo metemos
				pccItemT3Id = poItem->GetT3Id() ;

				if( pccItemT3Id != NULL && *pccItemT3Id != '\0' )
				{
					// Va aquí porque es del pool
					pccThisT3Id = GetChannelId() ;

					// Para que los items nodales se hereden
					if( _GetT3InheritById( pccItemT3Id ) == NULL )
					{
						continue ;
					}
					else
					{
						pccItemNodeId = poItem->GetNodeId() ;

						if( pccItemNodeId != NULL && *pccItemNodeId != '\0' )
						{
							// Va aquí porque es del pool
							pccThisNodeId = poNode->GetNodeId() ;
		
							if( M4MdFacStrcmp( pccItemNodeId, pccThisNodeId ) != 0 )
							{
								continue ;
							}
						}
					}
				}


				// Vamos a ver que no sea uno del sistema sobreescrito
				// Si lo es quitamos el del sistema
				if( m_poSystemNode != NULL )
				{
					pccItemId = poItem->GetItemId() ;
					poSystemNitem = m_poSystemNode->GetNitemById( pccItemId, M4_FALSE ) ;

					if( poSystemNitem != NULL )
					{
						poSystemItem = poSystemNitem->GetItem() ;

						if( poSystemItem->GetIsToInclude() == 1 )
						{
							poSystemItem->SetIsToInclude( 0 ) ;
						}
					}
				}

				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				// Ponemos el nivel al item
				poNitem->SetLevel( iLevel ) ;

				iResult = poNitem->SetItem( this, poNode, poItem, NULL ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Lo ponemos en el array global
			    iResult = m_oNitems.AddElement( poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;


				// Añadimos los parámetros
				iIsParameter = poItem->GetIsParameter() ;

				if( iIsParameter == 1 )
				{
					iResult = m_oParameters.AddElement( poNitem ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}

				if( bAddSysDBItems == M4_TRUE )
				{
					pccItemId = poItem->GetIdentifier() ;

					// Se mira si hay que añadir el item SYS_LOAD_CLIENT
					if( bClientSysLoadAdded == M4_FALSE && bHasClientSysLoad == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_CLIENT ) == 0 )
						{
							bHasClientSysLoad = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_PERSIST_CLIENT
					if( bClientSysPersistAdded == M4_FALSE && bHasClientSysPersist == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_CLIENT ) == 0 )
						{
							bHasClientSysPersist = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_LOAD_SERVER
					if( bServerSysLoadAdded == M4_FALSE && bHasServerSysLoad == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_SERVER ) == 0 )
						{
							bHasServerSysLoad = M4_TRUE ;
						}
					}

					// Se mira si hay que añadir el item SYS_PERSIST_SERVER
					if( bServerSysPersistAdded == M4_FALSE && bHasServerSysPersist == M4_FALSE )
					{
						if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_SERVER ) == 0 )
						{
							bHasServerSysPersist = M4_TRUE ;
						}
					}
				}

				// Se mira si hay que añadir el item EXECUTEREPORT
				if( bAddExecuteReportItem == M4_TRUE && bExecuteReportAdded == M4_FALSE && bHasExecuteReport == M4_FALSE )
				{
					pccItemId = poItem->GetIdentifier() ;

					if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT ) == 0 )
					{
						bHasExecuteReport = M4_TRUE ;
					}
				}

				// Se mira si hay que añadir el item DYN_FILTER
				if( bAddDynFilterItem == M4_TRUE && bHasDynFilter == M4_FALSE )
				{
					pccItemId = poItem->GetIdentifier() ;
					iInternalType = poItem->GetInternalType() ;

					if( strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_DYN_FILTER ) == 0 || iInternalType == M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE )
					{
						bHasDynFilter = M4_TRUE ;
					}
				}
			}
		}

		// Comprobamos si algun nodo sobreescrito no tira de una Ti heredada
		CHECK_CHLOG_ERRORF( iNextInherit < iNodeInherits, M4_ERROR, M4_MDFAC_BAD_NODE_TI_OVERWRITE, poTi->GetTiId() << poNextTi->GetTiId() << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;


		// Leemos la herencia del sistema
		iInheritType = poTi->GetInheritanceType() ;

		for( k = 0 ; k < iSystemItems ; k++ )
		{
			poSystemNitem = m_poSystemNode->GetNitemByPosition( k ) ;

			/* Bug 0095808
			El control de errores esta sobre la variable equivocada
			*/
			CHECK_CHLOG_DEBUGF( poSystemNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poSystemItem = poSystemNitem->GetItem() ;

			// Si no hay que meterlo se salta
			if( poSystemItem->GetIsToInclude() == 0 )
			{
				continue ;
			}

			iInternalType = poSystemItem->GetInternalType() ;
			iSystemType = 1 << ( iInternalType - M4CL_INTERNAL_TYPE_SYSTEM_START ) ;

			if( iSystemType & iInheritType )
			{
				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				poRitem = poSystemNitem->GetRitem() ;

				iResult = poNitem->SetItem( this, poNode, poSystemItem, poRitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Lo ponemos en el array global
				iResult = m_oNitems.AddElement( poNitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;
			}
		}

		if( bAddSysDBItems == M4_TRUE )
		{
			// Se añade el item SYS_LOAD_CLIENT
			if( bClientSysLoadAdded == M4_FALSE && bHasClientSysLoad == M4_FALSE )
			{
				bClientSysLoadAdded = M4_TRUE ;

				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				if( m_poClientSysLoadItem == NULL )
				{
					m_poClientSysLoadItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_SYS_LOAD_CLIENT, M4CL_ITEM_TYPE_METHOD, M4CL_ITEM_SCOPE_BLOCK, M4CL_M4_TYPE_NUMBER, 6, 0, M4CL_INTERNAL_TYPE_SYSDB_ITEM, M4CL_CSTYPE_MAPPED, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ) ;
				}

				iResult = _AddSystemItem( m_poClientSysLoadItem, poNitem, poNode, M4_TRUE, M4CL_LENGUAGE_LN4, "", "", 0, 0, M4_MDFAC_RULE_CREATION_SYS_LOAD ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;
			}

			// Se añade el item SYS_PERSIST_CLIENT
			if( bClientSysPersistAdded == M4_FALSE && bHasClientSysPersist == M4_FALSE )
			{
				bClientSysPersistAdded = M4_TRUE ;

				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				if( m_poClientSysPersistItem == NULL )
				{
					m_poClientSysPersistItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_CLIENT, M4CL_ITEM_TYPE_METHOD, M4CL_ITEM_SCOPE_BLOCK, M4CL_M4_TYPE_NUMBER, 6, 0, M4CL_INTERNAL_TYPE_SYSDB_ITEM, M4CL_CSTYPE_MAPPED, M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION ) ;
				}

				iResult = _AddSystemItem( m_poClientSysPersistItem, poNitem, poNode, M4_TRUE, M4CL_LENGUAGE_LN4, "", "", 0, 0, M4_MDFAC_RULE_CREATION_SYS_PERSIST ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;
			}

			// Se añade el item SYS_LOAD_SERVER
			if( bServerSysLoadAdded == M4_FALSE && bHasServerSysLoad == M4_FALSE )
			{
				bServerSysLoadAdded = M4_TRUE ;

				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				if( m_poServerSysLoadItem == NULL )
				{
					m_poServerSysLoadItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_SYS_LOAD_SERVER, M4CL_ITEM_TYPE_METHOD, M4CL_ITEM_SCOPE_BLOCK, M4CL_M4_TYPE_NUMBER, 6, 0, M4CL_INTERNAL_TYPE_SYSDB_ITEM, M4CL_CSTYPE_BACK, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ) ;
				}

				iResult = _AddSystemItem( m_poServerSysLoadItem, poNitem, poNode, M4_TRUE, M4CL_LENGUAGE_LN4, "", "", 0, 0, M4_MDFAC_RULE_CREATION_SYS_LOAD ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;
			}

			// Se añade el item SYS_PERSIST_SERVER
			if( bServerSysPersistAdded == M4_FALSE && bHasServerSysPersist == M4_FALSE )
			{
				bServerSysPersistAdded = M4_TRUE ;

				poNitem = &( poNitemsArray[ iItemNumber ] ) ;

				poNitem->SetUpperPool( m_poUpperPool ) ;
				poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

				if( m_poServerSysPersistItem == NULL )
				{
					m_poServerSysPersistItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_SERVER, M4CL_ITEM_TYPE_METHOD, M4CL_ITEM_SCOPE_BLOCK, M4CL_M4_TYPE_NUMBER, 6, 0, M4CL_INTERNAL_TYPE_SYSDB_ITEM, M4CL_CSTYPE_BACK, M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION ) ;
				}

				iResult = _AddSystemItem( m_poServerSysPersistItem, poNitem, poNode, M4_TRUE, M4CL_LENGUAGE_LN4, "", "", 0, 0, M4_MDFAC_RULE_CREATION_SYS_PERSIST ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iItemNumber++ ;
			}
		}

		// Se añade el item EXECUTEREPORT
		if( bAddExecuteReportItem == M4_TRUE && bExecuteReportAdded == M4_FALSE && bHasExecuteReport == M4_FALSE )
		{
			bExecuteReportAdded = M4_TRUE ;

			poNitem = &( poNitemsArray[ iItemNumber ] ) ;

			poNitem->SetUpperPool( m_poUpperPool ) ;
			poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

			if( m_poExecuteReportItem == NULL )
			{
				m_poExecuteReportItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT, M4CL_ITEM_TYPE_CONCEPT, M4CL_ITEM_SCOPE_NODE, M4CL_M4_TYPE_LONG, M4CL_MAX_LONG_PRECISION, 0, M4CL_INTERNAL_TYPE_EXECUTE_REPORT, M4CL_CSTYPE_BOTH, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ) ;
			}

			if( m_poSystemArgumentsArray == NULL )
			{
				iResult = _CreateSystemArguments() ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}

			iResult = _AddSystemItem( m_poExecuteReportItem, poNitem, poNode, M4_TRUE, M4CL_INTERN_OBJECT_DLL, "engindll", "ExecuteReport", M4_MDFAC_SYSTEM_ARGUMENTS_EXECUTEREPORT, M4_MDFAC_SYSTEM_ARGUMENTS_EXECUTEREPORT_NUMBER, M4_MDFAC_RULE_CREATION_EXECUTEREPORT ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iItemNumber++ ;
		}

		// Se añade el item DYN_FILTER
		if( bAddDynFilterItem == M4_TRUE && bHasDynFilter == M4_FALSE )
		{
			poNitem = &( poNitemsArray[ iItemNumber ] ) ;

			poNitem->SetUpperPool( m_poUpperPool ) ;
			poNitem->SetNoUpperPool( m_poNoUpperPool ) ;

			if( m_poDynFilterItem == NULL )
			{
				m_poDynFilterItem = _CreateSystemItem( M4MDFAC_SYSTEM_ITEM_DYN_FILTER, M4CL_ITEM_TYPE_PROPERTY, M4CL_ITEM_SCOPE_NODE, M4CL_M4_TYPE_LONG, M4CL_MAX_LONG_PRECISION, 0, M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE, M4CL_CSTYPE_MAPPED, M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED ) ;
			}

			iResult = _AddSystemItem( m_poDynFilterItem, poNitem, poNode, M4_FALSE, M4CL_LENGUAGE_LN4, "", "", 0, 0, M4_MDFAC_RULE_CREATION_NORMAL ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iItemNumber++ ;
		}
	}

	m_oNitems.Sort( M4ClCompareObjectsByTwoIds ) ;


// Añadimos los nitems a los nodos ============================================

	iNode = 0 ;
	poLastNode = NULL ;
	pccLastItemId = NULL ;

    iLength = m_oNitems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al nitem dentro del array de punteros
		ppoNitem = GetNitemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = *ppoNitem ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poNitem->GetNodeId() ;
		pccItemId = poNitem->GetNitemId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Nitems" ) ;


		// Establecemos las referencias si es una nueva Node
		if( poNode != poLastNode )
		{
			poNode->InitNitems( ppoNitem ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya items repetidos si no es el primero del nodo
			if( pccLastItemId != NULL && pccItemId != NULL )
			{
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccLastItemId, pccItemId ) == 0, M4_ERROR, M4_MDFAC_REPEATED_ITEM, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << poNitem->GetItem()->GetTiId() << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}

		pccLastItemId = pccItemId ;


		// Incrementamos el número de items que tiene este Node
		poNode->AddNitem( poNitem ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: FillItemsWithInheritance( void )
{

    m4return_t				iResult ;
	m4uint8_t				iInheritType ;
	m4uint8_t				iIsOverWritten ;
	m4uint8_t				iIsNameOverWritten ;
	m4uint8_t				iIsMyRitem ;
    m4uint8_t				iLevel ;
    m4uint16_t				i, j, k ;
    m4uint16_t				iNitems ;
    m4uint16_t				iNodes ;
    m4uint16_t				iInherits ;
    m4uint16_t				iTriggers ;
    m4uint16_t				iFormulas ;
    m4uint16_t				iTotals ;
    m4uint16_t				iTotalTags ;
    m4uint16_t				iTags ;
    m4uint16_t				iConcepts ;
    m4uint16_t				iItemInherits ;
    m4uint16_t				iNitenConns ;
    m4uint16_t				iNodeInherits ;
    m4uint16_t				iNextInherit ;
	m4pcchar_t				pccItemId ;
	m4pcchar_t				pccItemName ;
	m4pvoid_t				pvPointer ;
    ClTempTi				*poTi ;
    ClTempTi				*poBaseTi ;
    ClTempTiInherit			*poTiInherit ;
    ClTempNode				*poNode ;
    ClTempNitem				*poNitem ;
    ClTempNitem				*poSonNitem ;
    ClTempRitem				*poRitem ;
    ClTempRitem				*poSonRitem ;
    ClTempTrigger			*poTrigger ;
    ClTempItemFormula		*poItemFormula ;
    ClTempNitemFormula		*poNitemFormula ;
	ClTempNitemConn			*poNitemConn ;
	ClTempConnector			*poConnector ;
    ClTempConcept			*poConcept ;
    ClTempItemInherit		*poItemInherit ;
    ClTempTotal				*poTotal ;
    ClTempTotalTag			*poTotalTag ;
    ClTempTag				*poTag ;
    ClTempNitem				**ppoNitem ;
    ClTempTi				*poNextTi ;
    ClTempNodeInherit		*poNodeInherit ;



    iNodes = m_oNodes.GetLength() ;
    iNitenConns = m_oNitemConns.GetLength() ;

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		poTi = poNode->GetTi() ;

		iInheritType = poTi->GetInheritanceType() ;

		iInherits = poTi->GetNumberOfInherits() ;


// Recorremos la herencia para contar =========================================

		for( j = 0 ; j < iInherits ; j++ )
		{
			poTiInherit = poTi->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poBaseTi = poTiInherit->GetBaseTi() ;
			CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			
// Contamos los triggers en los nitems ========================================

			iTriggers = poBaseTi->GetNumberOfTriggers() ;

			for( k = 0 ; k < iTriggers ; k++ )
			{
				poTrigger = poBaseTi->GetTriggerByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTrigger == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Miramos el item padre
				pccItemId = poTrigger->GetFatherItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sea de un item nodal
					poTrigger->SetFatherNitem( NULL ) ;
					poTrigger->SetSonNitem( NULL ) ;
					continue ;
				}

				// Miramos el item hijo
				pccItemId = poTrigger->GetSonItemId() ;
				poSonNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poSonNitem == NULL )
				{
					// Puede que sea de un item nodal
					poTrigger->SetFatherNitem( NULL ) ;
					poTrigger->SetSonNitem( NULL ) ;
					continue ;
				}

				// Chequeamos que no esté conectado a si mismo
				CHECK_CHLOG_ERRORF( poNitem == poSonNitem, M4_ERROR, M4_MDFAC_AUTOCONNECTED_NODE_ITEM, LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;

				// Este puede cambiar por nodo, así que ojo
				poTrigger->SetFatherNitem( poNitem ) ;
				poTrigger->SetSonNitem( poSonNitem ) ;


				// Si son del sistema los desconectamos
				iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

				if( iIsMyRitem == 0 )
				{
					iResult = poNitem->UnreferNitem( m_poSystemTi ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}

				iIsMyRitem = poSonNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

				if( iIsMyRitem == 0 )
				{
					iResult = poSonNitem->UnreferNitem( m_poSystemTi ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poSonRitem = poSonNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poSonRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem->AddConnectorsDelta( 1 ) ;
				poSonRitem->AddInvConnectorsDelta( 1 ) ;
			}


// Contamos las fórmulas en los nitems ========================================

			iFormulas = poBaseTi->GetNumberOfItemFormulas() ;

			for( k = 0 ; k < iFormulas ; k++ )
			{
				poItemFormula = poBaseTi->GetItemFormulaByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poItemFormula->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sea de un item nodal
					poItemFormula->SetNitem( NULL ) ;
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem->AddFormulasDelta( 1 ) ;

				// Este puede cambiar por nodo, así que ojo
				poItemFormula->SetNitem( poNitem ) ;
			}


// Contamos los totales en los nitems =========================================

			iTotals = poBaseTi->GetNumberOfTotals() ;

			for( k = 0 ; k < iTotals ; k++ )
			{
				poTotal = poBaseTi->GetTotalByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poTotal->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sean de un item nodal
					poTotal->SetNitem( NULL ) ;
					continue ;
				}

				if( poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 0 )
				{
					// Los del sistema no pueden tener totales
					poTotal->SetNitem( NULL ) ;
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem->AddTotalsDelta( 1 ) ;

				// Este puede cambiar por nodo, así que ojo
				poTotal->SetNitem( poNitem ) ;
			}

// Contamos las condiciones de total en los nitems ============================

			iTotalTags = poBaseTi->GetNumberOfTotalTags() ;

			for( k = 0 ; k < iTotalTags ; k++ )
			{
				poTotalTag = poBaseTi->GetTotalTagByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poTotalTag->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sean de un item nodal
					poTotalTag->SetNitem( NULL ) ;
					continue ;
				}

				if( poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 0 )
				{
					// Los del sistema no pueden tener totales
					poTotalTag->SetNitem( NULL ) ;
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem->AddTotalTagsDelta( 1 ) ;

				// Este puede cambiar por nodo, así que ojo
				poTotalTag->SetNitem( poNitem ) ;
			}

// Contamos los tags en los nitems =========================================

			iTags = poBaseTi->GetNumberOfTags() ;

			for( k = 0 ; k < iTags ; k++ )
			{
				poTag = poBaseTi->GetTagByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poTag->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sean de un item nodal
					poTag->SetNitem( NULL ) ;
					continue ;
				}

				if( poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 0 )
				{
					// Los del sistema no pueden tener tags
					poTag->SetNitem( NULL ) ;
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem->AddTagsDelta( 1 ) ;

				// Este puede cambiar por nodo, así que ojo
				poTag->SetNitem( poNitem ) ;
			}
		}


// Recorremos la herencia para asignar ========================================
// Hay que hacerlo dentro del mismo nodo en el que estamos, porque formulas
// y totales pueden ser compartidos

		// Inicalizamos los niveles de herencia de nodos
		iNodeInherits = poNode->GetNumberOfInherits() ;
		CHECK_CHLOG_DEBUGF( iNodeInherits == 0, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iNextInherit = 0 ;
		poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
		poNextTi = poNodeInherit->GetTi() ;
		iLevel = poNodeInherit->GetLevel() ;


		iInherits = poTi->GetNumberOfInherits() ;

		for( j = 0 ; j < iInherits ; j++ )
		{
			poTiInherit = poTi->GetInheritByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poBaseTi = poTiInherit->GetBaseTi() ;
			CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// Calculamos el nivel de las fórmulas de esta TI
			if( iNextInherit < iNodeInherits )
			{
				if( poNextTi == poBaseTi )
				{
					poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
					iLevel = poNodeInherit->GetLevel() ;
					iNextInherit++ ;

					if( iNextInherit < iNodeInherits )
					{
						poNodeInherit = poNode->GetInheritByPosition( iNextInherit ) ;
						poNextTi = poNodeInherit->GetTi() ;
					}
				}
			}



// Insertamos los conceptos en los nitems =====================================

			iConcepts = poBaseTi->GetNumberOfConcepts() ;

			for( k = 0 ; k < iConcepts ; k++ )
			{
				poConcept = poBaseTi->GetConceptByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poConcept == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poConcept->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sea de un item nodal
					continue ;
				}

				iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

				if( iIsMyRitem == 0 )
				{
					// Los del sistema no se sobreescriben
					// Si lo dejamos hay que hacer un Unrefer
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				//Begin 0181356 Si se sobrescribe el nombre hay que seguir leyendo la información del concepto
				// Si está sobreescrito lo saltamos
				iIsOverWritten = poNitem->GetIsOverWritten() ;

				if( iIsOverWritten == 1 )
				{
					continue ;
				}

				poNitem->SetIsOverWritten( 1 ) ;

				poNitem->SetHasBackPay( poConcept->GetHasBackPay() ) ;
				//End 0181356

				// Si está sobreescrito el nombre también lo está el resto
				iIsNameOverWritten = poNitem->GetIsNameOverWritten() ;

				if( iIsNameOverWritten == 1 )
				{
					continue ;
				}

				pccItemName = poConcept->GetItemName() ;

				if( *pccItemName != '\0' )
				{
					poRitem->SetName( pccItemName ) ;
					poNitem->SetIsNameOverWritten( 1 ) ;
				}

			}


// Insertamos la herencia de items en los nitems =====================================

			iItemInherits = poBaseTi->GetNumberOfItemInherits() ;

			for( k = 0 ; k < iItemInherits ; k++ )
			{
				poItemInherit = poBaseTi->GetItemInheritByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poItemInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				pccItemId = poItemInherit->GetItemId() ;
				poNitem = poNode->GetNitemById( pccItemId, M4_FALSE ) ;

				if( poNitem == NULL )
				{
					// Puede que sea de un item nodal
					continue ;
				}

				iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

				if( iIsMyRitem == 0 )
				{
					// Los del sistema no se sobreescriben
					// Si lo dejamos hay que hacer un Unrefer
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Si está sobreescrito el nombre también lo está el resto
				iIsNameOverWritten = poNitem->GetIsNameOverWritten() ;

				if( iIsNameOverWritten == 1 )
				{
					continue ;
				}

				pccItemName = poItemInherit->GetItemName() ;

				if( *pccItemName != '\0' )
				{
					poRitem->SetName( pccItemName ) ;
					poNitem->SetIsNameOverWritten( 1 ) ;
				}
			}


// Insertamos los triggers en los nitems ======================================

			iTriggers = poBaseTi->GetNumberOfTriggers() ;

			for( k = 0 ; k < iTriggers ; k++ )
			{
				poTrigger = poBaseTi->GetTriggerByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTrigger == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitem = poTrigger->GetFatherNitem() ;
				poSonNitem = poTrigger->GetSonNitem() ;

				if( poNitem == NULL || poSonNitem == NULL )
				{
					// Puede que sea de un item nodal
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitemConn = poRitem->GetConnectorBySonNitem( poSonNitem ) ;

				if( poNitemConn != NULL )
				{
					// Es que ya estaba metido

					// Comprobar que no sea un trigger repetido
					iIsOverWritten = poNitemConn->GetIsOverWritten() ;

					if( iIsOverWritten == 1 )
					{
						DUMP_CHLOG_ERRORF( M4_MDFAC_REPEATED_TRIGGER, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
						return( M4_ERROR ) ;
					}

					continue ;
				}

				
				// Buscamos el conector a si mismo
				poConnector = poNode->GetConnectorBySonNode( poNode ) ;
				CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Conseguimos un conector vacío
				if( iNitenConns < m_iNitemConnsSize )
				{
					poNitemConn = &m_poNitemConnsArray[ iNitenConns++ ] ;
				}
				else
				{
					// Si no lo creamos y le ponemos como owner
					poNitemConn = new ClTempNitemConn ;
					CHECK_CHLOG_ERRORF( poNitemConn == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemConn ) ) << __LINE__ << __FILE__ ) ;

					poNitemConn->SetIsOwn( 1 ) ;
				}

				poNitemConn->SetUpperPool( m_poUpperPool ) ;


				poNitemConn->SetConnector( poConnector ) ;
				poNitemConn->SetFatherNitem( poNitem ) ;
				poNitemConn->SetSonNitem( poSonNitem ) ;

				poNitemConn->FillConnectorWithTrigger( poTrigger ) ;

				iResult = poRitem->AddConnector( poNitemConn ) ;

				if( iResult != M4_SUCCESS )
				{
					if( poNitemConn->GetIsOwn() == 1 )
					{
						delete( poNitemConn ) ;
					}
				}

				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				poSonRitem = poSonNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poSonRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poSonRitem->AddInvConnector( poNitemConn ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitemConn->SetIsOverWritten( 1 ) ;
			}


// Insertamos las fórmulas en los nitems ======================================

			iFormulas = poBaseTi->GetNumberOfItemFormulas() ;

			for( k = 0 ; k < iFormulas ; k++ )
			{
				poItemFormula = poBaseTi->GetItemFormulaByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitem = poItemFormula->GetNitem() ;

				if( poNitem == NULL )
				{
					// Puede que sea de un item nodal
					continue ;
				}

				/* Bug 0099169
				El control hay que hacerlo después de controlar los items nodales
				*/
				CHECK_CHLOG_ERRORF( poItemFormula->GetIsCorrect() != 1, M4_ERROR, M4_MDFAC_INCORRECT_RULE, poItemFormula->GetFormulaId() << poItemFormula->GetItemId() << poItemFormula->GetTiId() << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;


				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Creamos una nueva fórmula
				poNitemFormula = new ClTempNitemFormula ;
				CHECK_CHLOG_ERRORF( poNitemFormula == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula ) ) << __LINE__ << __FILE__ ) ;

				poNitemFormula->SetUpperPool( m_poUpperPool ) ;
				poNitemFormula->SetNoUpperPool( m_poNoUpperPool ) ;


				// Inicializamos la fórmula
				iResult = poNitemFormula->Init( poNitem, poItemFormula, iLevel ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// La añadimos al array
				iResult = poRitem->AddFormula( poNitemFormula ) ;

				if( iResult != M4_SUCCESS )
				{
					delete( poNitemFormula ) ;
				}

				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}



// Insertamos los totales en los nitems =======================================

			iTotals = poBaseTi->GetNumberOfTotals() ;

			for( k = 0 ; k < iTotals ; k++ )
			{
				poTotal = poBaseTi->GetTotalByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitem = poTotal->GetNitem() ;

				if( poNitem == NULL )
				{
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Lo añadimos al array
				iResult = poRitem->AddTotal( poTotal ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}


// Insertamos las condiciones de total en los nitems ==========================

			iTotalTags = poBaseTi->GetNumberOfTotalTags() ;

			for( k = 0 ; k < iTotalTags ; k++ )
			{
				poTotalTag = poBaseTi->GetTotalTagByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitem = poTotalTag->GetNitem() ;

				if( poNitem == NULL )
				{
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Lo añadimos al array
				iResult = poRitem->AddTotalTag( poTotalTag ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}


// Insertamos los tags en los nitems =======================================

			iTags = poBaseTi->GetNumberOfTags() ;

			for( k = 0 ; k < iTags ; k++ )
			{
				poTag = poBaseTi->GetTagByPosition( k ) ;
				CHECK_CHLOG_DEBUGF( poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poNitem = poTag->GetNitem() ;

				if( poNitem == NULL )
				{
					continue ;
				}

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Lo añadimos al array
				iResult = poRitem->AddTag( poTag ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}
	}


// Volvemos a ordenar los conectores de item por si se han metido triggers ====

    iNitems = m_oNitems.GetLength() ;

    for( i = 0 ; i < iNitems ; i++ )
    {
		// Conseguimos la dirección del puntero al nitem dentro del array de punteros
		ppoNitem = GetNitemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = *ppoNitem ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

		// Si no soy propietario no hago nada
		if( iIsMyRitem == 0 )
		{
			continue ;
		}

		poRitem = poNitem->GetRitem() ;
		poRitem->OrderConnectors() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillNodesWithConnectors( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i, j ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
    m4uint16_t		iConnectors ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poSonNode ;
    ClTempNode		*poStoredSonNode ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempConnector	*poStoredConnector ;
    ClTempConnector	*poConnector ;
    ClTempConnector	**ppoConnector ;


// Primero los conectores directos ============================================
    
	iNode = 0 ;
	poLastNode = NULL ;

    iLength = m_oConnectors.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al conector dentro del array de punteros
		ppoConnector = GetConnectorPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = *ppoConnector ;
		CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poConnector->GetFatherNodeId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Connectors" ) ;

		// Asignamos el padre del conector
		poConnector->SetFatherNode( poNode ) ;

		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitConnectors( ppoConnector ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya conectores repetidos si no es el primero del nodo
			poSonNode = poConnector->GetSonNode() ;

			iConnectors =  poNode->GetNumberOfConnectors() ;

			for( j = 0 ; j < iConnectors ; j++ )
			{
				poStoredConnector = poNode->GetConnectorByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poStoredConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poStoredSonNode = poStoredConnector->GetSonNode() ;
				CHECK_CHLOG_ERRORF( poStoredSonNode == poSonNode, M4_ERROR, M4_MDFAC_REPEATED_CONNECTOR, LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}


		// Incrementamos el número de conectores que tiene este nodo
		poNode->AddConnector() ;
	}


// Y ahora los inversos =======================================================

	iNode = 0 ;
	poLastNode = NULL ;

    iLength = m_oInvConnectors.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al conector dentro del array de punteros
		ppoConnector = GetInvConnectorPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poConnector = *ppoConnector ;
		CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poConnector->GetSonNode()->GetNodeId() ;

		
		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Connectors" ) ;


		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitInvConnectors( ppoConnector ) ;
			poLastNode = poNode ;
		}

		// Incrementamos el número de conectores que tiene este nodo
		poNode->AddInvConnector() ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillItemsWithConnectors( void )
{

	m4return_t			iResult ;
    m4bool_t			bFound ;
    m4uint8_t			iIsMyRitem ;
    m4uint16_t			i, j ;
    m4uint16_t			iLength ;
    m4uint16_t			iNitem ;
    m4uint16_t			iNitemNumber ;
    m4uint16_t			iConnectors ;
    m4pcchar_t			pccNodeId ;
    m4pcchar_t			pccNitemId ;
    m4pcchar_t			pccStoredNodeId ;
    m4pcchar_t			pccStoredNitemId ;
    ClTempNode			*poFatherNode ;
    ClTempNode			*poSonNode ;
    ClTempRitem			*poRitem ;
    ClTempRitem			*poSonRitem ;
    ClTempConnector		*poConnector ;
    ClTempNitem			*poNitem ;
    ClTempNitem			*poSonNitem ;
    ClTempNitem			*poStoredSonNitem ;
    ClTempNitem			*poLastNitem ;
    ClTempNitemConn		*poStoredNitemConn ;
    ClTempNitemConn		*poNitemConn ;
    ClTempNitemConn		**ppoNitemConn ;


// Primero calculamos el tamaño ===============================================

	iNitem = 0 ;
	poLastNitem = NULL ;

    iLength = m_oNitemConns.GetLength() ;
    iNitemNumber = m_oNitems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al item dentro del array de punteros
		ppoNitemConn = GetNitemConnPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitemConn = *ppoNitemConn ;
		CHECK_CHLOG_DEBUGF( poNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poNitemConn->GetFatherNodeId() ;
		pccNitemId = poNitemConn->GetFatherItemId() ;

        bFound = M4_FALSE ;
        poNitem = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNitem < iNitemNumber )
		{
			poNitem = GetNitemByPosition( iNitem ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNitem->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNitemId = poNitem->GetNitemId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNitemId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 && M4MdFacStrcmp( pccNitemId, pccStoredNitemId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNitem++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccNitemId << pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Item Connectors" ) ;


		// si es nuevo inicializamos el anterior
		if( poNitem != poLastNitem )
		{
			poLastNitem = poNitem ;
		}


		// Si es del sistema le desconectamos
		iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

		if( iIsMyRitem == 0 )
		{
			iResult = poNitem->UnreferNitem( m_poSystemTi ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


		// Inicializamos el conector ==========================================

		poFatherNode = poNitem->GetNode() ;
		CHECK_CHLOG_DEBUGF( poFatherNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		poSonNitem = poNitemConn->GetSonNitem() ;
		CHECK_CHLOG_DEBUGF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Si son el mismo se da error
		CHECK_CHLOG_ERRORF( poNitem == poSonNitem, M4_ERROR, M4_MDFAC_AUTOCONNECTED_NODE_ITEM, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;

	
		poSonNode = poSonNitem->GetNode() ;
		CHECK_CHLOG_DEBUGF( poSonNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		poNitemConn->SetFatherNitem( poNitem ) ;

		poConnector = poFatherNode->GetConnectorBySonNode( poSonNode ) ;
		CHECK_CHLOG_ERRORF( poConnector == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE_CON, LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) << LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) ) ;

		poNitemConn->SetConnector( poConnector ) ;


		// Si es del sistema le desconectamos
		iIsMyRitem = poSonNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

		if( iIsMyRitem == 0 )
		{
			iResult = poSonNitem->UnreferNitem( m_poSystemTi ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		poRitem = poNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSonRitem = poSonNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poSonRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRitem->AddConnectorsDelta( 1 ) ;
		poSonRitem->AddInvConnectorsDelta( 1 ) ;
	}


// Añadimos el conector =======================================================

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al item dentro del array de punteros
		ppoNitemConn = GetNitemConnPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitemConn = *ppoNitemConn ;
		CHECK_CHLOG_DEBUGF( poNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		poNitem = poNitemConn->GetFatherNitem() ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRitem = poNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSonNitem = poNitemConn->GetSonNitem() ;
		CHECK_CHLOG_DEBUGF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSonRitem = poSonNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poSonRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Controlamos que no haya conectores repetidos

		iConnectors =  poRitem->GetNumberOfConnectors() ;

		for( j = 0 ; j < iConnectors ; j++ )
		{
			poStoredNitemConn = poRitem->GetConnectorByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poStoredNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poStoredSonNitem = poStoredNitemConn->GetSonNitem() ;
			CHECK_CHLOG_ERRORF( poStoredSonNitem == poSonNitem, M4_ERROR, M4_MDFAC_REPEATED_ITEM_CONNECTOR, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iNode(), poSonNitem->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}


		iResult = poRitem->AddConnector( poNitemConn ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSonRitem->AddInvConnector( poNitemConn ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Reseteamos para los triggers
		poRitem->SetConnectorsDelta( 0 ) ;
		poSonRitem->SetInvConnectorsDelta( 0 ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillNodesWithIndexes( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
	m4uint32_t		iIndex ;
	m4uint32_t		iLastIndex ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempIndex		*poIndex ;
    ClTempIndex		**ppoIndex ;

    
	iNode = 0 ;
	poLastNode = NULL ;
	iLastIndex = 0 ;

    iLength = m_oIndexes.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al indice dentro del array de punteros
		ppoIndex = GetIndexPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoIndex == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poIndex = *ppoIndex ;
		CHECK_CHLOG_DEBUGF( poIndex == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poIndex->GetNodeId() ;
		iIndex = poIndex->GetIndexId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Indexes" ) ;


		// Asignamos el nodo al indice
		poIndex->SetNode( poNode ) ;

		// Establecemos las referencias si es una nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitIndexes( ppoIndex ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya indices repetidos si no es el primero del nodo
			CHECK_CHLOG_ERRORF( iLastIndex == iIndex, M4_ERROR, M4_MDFAC_REPEATED_INDEX, iIndex << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}

		iLastIndex = iIndex ;


		// Incrementamos el número de totales que tiene esta Node
		poNode->AddIndex( poIndex ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillIndexesWithItems( void )
{

    m4bool_t			bFound ;
	m4uint8_t			iFunction ;
    m4uint16_t			i, j ;
    m4uint16_t			iLength ;
    m4uint16_t			iIndex ;
    m4uint16_t			iIndexNumber ;
    m4uint16_t			iItems ;
    m4uint32_t			iIndexId ;
    m4uint32_t			iStoredIndexId ;
    m4pcchar_t			pccItem ;
    m4pcchar_t			pccNodeId ;
    m4pcchar_t			pccStoredNodeId ;
    ClTempNitem			*poNitem ;
    ClTempNitem			*poStoredNitem ;
    ClTempNitem			*poNotEqualNitem ;
    ClTempNode			*poNode ;
    ClTempIndex			*poIndex ;
    ClTempIndex			*poLastIndex ;
    ClTempIndexItem		*poStoredIndexItem ;
    ClTempIndexItem		*poIndexItem ;
    ClTempIndexItem		**ppoIndexItem ;

    
	iIndex = 0 ;
	poLastIndex = NULL ;
	poNotEqualNitem = NULL ;

    iLength = m_oIndexItems.GetLength() ;
    iIndexNumber = m_oIndexes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al indice dentro del array de punteros
		ppoIndexItem = GetIndexItemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoIndexItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poIndexItem = *ppoIndexItem ;
		CHECK_CHLOG_DEBUGF( poIndexItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poIndexItem->GetNodeId() ;
		iIndexId = poIndexItem->GetIndexId() ;

        bFound = M4_FALSE ;
        poIndex = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iIndex < iIndexNumber )
		{
			poIndex = GetIndexByPosition( iIndex ) ;
			CHECK_CHLOG_DEBUGF( poIndex == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poIndex->GetNodeId() ;
			iStoredIndexId = poIndex->GetIndexId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 && iIndexId == iStoredIndexId )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iIndex++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_INDEX, poIndexItem->GetItemId() << iIndexId << pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;


		// Asignamos el item del item de indice
		poNode = poIndex->GetNode() ;
		CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccItem = poIndexItem->GetItemId() ;
		poNitem = poNode->GetNitemById( pccItem, M4_FALSE ) ;
		CHECK_CHLOG_ERRORF( poNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccItem << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Index Items" ) ;

		poIndexItem->SetNitem( poNitem ) ;


		// Establecemos las referencias si es un nuevo indice
		if( poIndex != poLastIndex )
		{
			poIndex->InitItems( ppoIndexItem ) ;
			poLastIndex = poIndex ;
			poNotEqualNitem = NULL ;
		}

		// Controlamos que no haya ya un item con función distinta de equal
		CHECK_CHLOG_ERRORF( poNotEqualNitem != NULL, M4_ERROR, M4_MDFAC_BAD_FUNCTION_INDEX_ITEM, LOG_CAT_AND( poNotEqualNitem->iItem(), poNotEqualNitem->nItem() ) << iIndex << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;

		iFunction = poIndexItem->GetFunction() ;

		if( iFunction != M4CL_EQUAL_FUNCTION )
		{
			poNotEqualNitem = poNitem ;
		}

		// Controlamos que no haya items repetidos
		iItems =  poIndex->GetNumberOfItems() ;

		for( j = 0 ; j < iItems ; j++ )
		{
			poStoredIndexItem = poIndex->GetItemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poStoredIndexItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poStoredNitem = poStoredIndexItem->GetNitem() ;
			CHECK_CHLOG_ERRORF( poStoredNitem == poNitem, M4_ERROR, M4_MDFAC_REPEATED_INDEX_ITEM, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << iIndex << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}


		// Incrementamos el número de argumentos que tiene este item
		poIndex->AddItem() ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillNodesWithPlugs( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
    m4pcchar_t		pccUsedT3Id ;
    m4pcchar_t		pccLastUsedT3Id ;
    m4pcchar_t		pccUsedNodeId ;
    m4pcchar_t		pccLastUsedNodeId ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempPlug		*poPlug ;
    ClTempPlug		**ppoPlug ;

    
	iNode = 0 ;
	poLastNode = NULL ;
	pccLastUsedT3Id = NULL ;
	pccLastUsedNodeId = NULL ;

    iLength = m_oPlugs.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al plug dentro del array de punteros
		ppoPlug = GetPlugPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoPlug == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poPlug = *ppoPlug ;
		CHECK_CHLOG_DEBUGF( poPlug == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poPlug->GetNodeId() ;
		pccUsedT3Id = poPlug->GetUsedT3Id() ;
		pccUsedNodeId = poPlug->GetUsedNodeId() ;


		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Lists" ) ;


		// Asignamos el nodo
		poPlug->SetNode( poNode ) ;

		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitPlugs( ppoPlug ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya listas repetidas si no es el primero del nodo
			if( pccLastUsedT3Id != NULL && pccUsedT3Id != NULL && pccLastUsedNodeId != NULL && pccUsedNodeId != NULL )
			{
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccLastUsedT3Id, pccUsedT3Id ) == 0 && M4MdFacStrcmp( pccLastUsedNodeId, pccUsedNodeId ) == 0, M4_ERROR, M4_MDFAC_REPEATED_LIST, pccUsedNodeId << pccUsedT3Id << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}

		pccLastUsedT3Id = pccUsedT3Id ;
		pccLastUsedNodeId = pccUsedNodeId ;


		// Incrementamos el número de plugs que tiene este nodo
		poNode->AddPlug() ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillPlugsWithItems( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iPlug ;
    m4uint16_t		iPlugNumber ;
    m4pcchar_t		pccItem ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccUsedT3Id ;
    m4pcchar_t		pccUsedNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    m4pcchar_t		pccStoredUsedT3Id ;
    m4pcchar_t		pccStoredUsedNodeId ;
    ClTempNitem		*poNitem ;
    ClTempNitem		*poLastNitem ;
    ClTempNode		*poNode ;
    ClTempPlug		*poPlug ;
    ClTempPlug		*poLastPlug ;
    ClTempPlugItem	*poPlugItem ;
    ClTempPlugItem	**ppoPlugItem ;

    
	iPlug = 0 ;
	poLastPlug = NULL ;
	poLastNitem = NULL ;

    iLength = m_oPlugItems.GetLength() ;
    iPlugNumber = m_oPlugs.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al itemplug dentro del array de punteros
		ppoPlugItem = GetPlugItemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoPlugItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poPlugItem = *ppoPlugItem ;
		CHECK_CHLOG_DEBUGF( poPlugItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poPlugItem->GetNodeId() ;
		pccUsedT3Id = poPlugItem->GetUsedT3Id() ;
		pccUsedNodeId = poPlugItem->GetUsedNodeId() ;

		bFound = M4_FALSE ;
		poPlug = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iPlug < iPlugNumber )
		{
			poPlug = GetPlugByPosition( iPlug ) ;
			CHECK_CHLOG_DEBUGF( poPlug == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poPlug->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredUsedT3Id = poPlug->GetUsedT3Id() ;
			CHECK_CHLOG_DEBUGF( pccStoredUsedT3Id == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredUsedNodeId = poPlug->GetUsedNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredUsedNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 && M4MdFacStrcmp( pccUsedT3Id, pccStoredUsedT3Id ) == 0 && M4MdFacStrcmp( pccUsedNodeId, pccStoredUsedNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iPlug++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_PLUG_INSTALLING, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << pccUsedNodeId << pccUsedT3Id ) ;


		// Asignamos el item
		poNode = poPlug->GetNode() ;
		CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccItem = poPlugItem->GetItemId() ;
		poNitem = poNode->GetNitemById( pccItem, M4_FALSE ) ;
		CHECK_CHLOG_ERRORF( poNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccItem << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "List Items" ) ;

		poPlugItem->SetSonNitem( poNitem ) ;


		// Establecemos las referencias si es un nuevo plug
		if( poPlug != poLastPlug )
		{
			poPlug->InitItems( ppoPlugItem ) ;
			poLastPlug = poPlug ;
		}
		else
		{
			// Controlamos que no haya conectores repetidos si no es el primero del plug
			CHECK_CHLOG_ERRORF( poLastNitem == poNitem, M4_ERROR, M4_MDFAC_REPEATED_ITEM_LIST, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << pccUsedNodeId << pccUsedT3Id << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		}

		poLastNitem = poNitem ;


		// Incrementamos el número de items que tiene este plug
		poPlug->AddItem() ;
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillNodesWithAlias( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
    m4pcchar_t		pccAliasId ;
    m4pcchar_t		pccLastAliasId ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempAlias		*poAlias ;
    ClTempAlias		**ppoAlias ;

    
	iNode = 0 ;
	poLastNode = NULL ;
	pccLastAliasId = NULL ;

    iLength = m_oAlias.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al alias dentro del array de punteros
		ppoAlias = GetAliasPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoAlias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poAlias = *ppoAlias ;
		CHECK_CHLOG_DEBUGF( poAlias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poAlias->GetNodeId() ;
		pccAliasId = poAlias->GetAliasId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Alias" ) ;


		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitAlias( ppoAlias ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya alias repetidos si no es el primero del nodo
			if( pccLastAliasId != NULL && pccAliasId != NULL )
			{
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccLastAliasId, pccAliasId ) == 0, M4_ERROR, M4_MDFAC_REPEATED_ALIAS, pccAliasId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}

		pccLastAliasId = pccAliasId ;

		// Incrementamos el número de alias que tiene este nodo
		poNode->AddAlias() ;
	}


    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillNodesWithT3Alias( void )
{

	m4return_t		iResult ;
    m4bool_t		bFound ;
    m4bool_t		bInsert ;
    m4uint16_t		i, j ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
    m4pcchar_t		pccT3AliasId ;
    m4pcchar_t		pccLastT3AliasId ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempT3Alias	*poT3Alias ;
    ClTempT3Alias	**ppoT3Alias ;

    
	j = 0 ;
	iNode = 0 ;
	poLastNode = NULL ;
	pccLastT3AliasId = NULL ;

    iLength = m_oT3Alias.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al alias de T3 dentro del array de punteros
		ppoT3Alias = GetT3AliasPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoT3Alias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poT3Alias = *ppoT3Alias ;
		CHECK_CHLOG_DEBUGF( poT3Alias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poT3Alias->GetNodeId() ;
		pccT3AliasId = poT3Alias->GetT3AliasId() ;

		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "M4Obj Alias" ) ;


		bInsert = M4_TRUE ;

		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			// Reajustamos el comienzo del array si hemos reducido alias
			poNode->InitT3Alias( ppoT3Alias - i + j ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Si es repetido es que está sobreescrito y hay que ignorarlo
			if( pccLastT3AliasId != NULL && pccT3AliasId != NULL )
			{
				if( M4MdFacStrcmp( pccLastT3AliasId, pccT3AliasId ) == 0 )
				{
					bInsert = M4_FALSE ;
				}
			}
		}

		pccLastT3AliasId = pccT3AliasId ;

		// Incrementamos el número de alias de T3 que tiene este nodo si no está sobreescrito
		if( bInsert == M4_TRUE )
		{
			// Si nos hemos desplazado realocamos el hueco
			if( j != i )
			{
				iResult = SetT3AliasByPosition( j, poT3Alias ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}

			poNode->AddT3Alias() ;
			j++ ;
		}
	}

	// Reajustamos el tamaño del array si hemos reducido alias
	if( j != iLength )
	{
		SetT3AliasLength( j ) ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempChannel :: FillNodesWithFilters( void )
{

    m4bool_t		bFound ;
    m4uint16_t		i ;
    m4uint16_t		iLength ;
    m4uint16_t		iNode ;
    m4uint16_t		iNodeNumber ;
    m4pcchar_t		pccFilterId ;
    m4pcchar_t		pccLastFilterId ;
    m4pcchar_t		pccNodeId ;
    m4pcchar_t		pccStoredNodeId ;
    ClTempNode		*poNode ;
    ClTempNode		*poLastNode ;
    ClTempFilter	*poFilter ;
    ClTempFilter	**ppoFilter ;

    
	iNode = 0 ;
	poLastNode = NULL ;
	pccLastFilterId = NULL ;

    iLength = m_oFilters.GetLength() ;
    iNodeNumber = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al filtro dentro del array de punteros
		ppoFilter = GetFilterPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoFilter == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFilter = *ppoFilter ;
		CHECK_CHLOG_DEBUGF( poFilter == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccNodeId = poFilter->GetNodeId() ;
		pccFilterId = poFilter->GetFilterId() ;


		bFound = M4_FALSE ;
		poNode = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iNode < iNodeNumber )
		{
			poNode = GetNodeByPosition( iNode ) ;
			CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredNodeId = poNode->GetNodeId() ;
			CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccNodeId, pccStoredNodeId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNode++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, pccNodeId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Filters" ) ;


		// Establecemos las referencias si es un nuevo nodo
		if( poNode != poLastNode )
		{
			poNode->InitFilters( ppoFilter ) ;
			poLastNode = poNode ;
		}
		else
		{
			// Controlamos que no haya alias repetidos si no es el primero del nodo
			if( pccLastFilterId != NULL && pccFilterId != NULL )
			{
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccLastFilterId, pccFilterId ) == 0, M4_ERROR, M4_MDFAC_REPEATED_FILTER, pccFilterId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}

		pccLastFilterId = pccFilterId ;


		// Incrementamos el número de filtros que tiene este nodo
		poNode->AddFilter( poFilter ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: FillChannelWithInstances( void )
{

    m4return_t          iResult ;
    m4uint8_t           iScope, iScopeAux ;
    m4uint8_t           iOpenMode, iOpenModeAux ;
    m4uint8_t           iCsAccess, iCsAccessAux ;
    m4uint8_t           iOrgType, iOrgTypeAux ;
    m4uint16_t          i, j ;
    m4uint16_t			iLength ;
    m4uint16_t          iNodes ;
    m4uint16_t          iT3Alias ;
    m4uint32_t          iItem ;
    m4uint32_t          iNode ;
    m4uint32_t          iInstance ;
    m4pcchar_t			pccInstanceId ;
    m4pcchar_t			pccT3Id, pccT3IdAux ;
    m4pcchar_t			pccRoleId, pccRoleIdAux ;
    m4pcchar_t			pccOrg, pccOrgAux ;
	m4pcchar_t			pccThisT3Id ;
	m4pcchar_t			pccExchangeRates ;
	m4char_t			acInstance[ M4CL_MAX_T3_ID + 2 ] ;
    m4pvoid_t           pvPointer ;
    m4VariantType		vValue ;
    ClTempNode          *poNode ;
    ClTempT3Alias       *poT3Alias ;
    ClTempInstance		*poInstance ;
	ClLogonAdaptor		*poLogonAdaptor ;
	ClAccessRecordSet	*poRecordSet ;
	ClCompiledMCR		*poCMCR ;



// Creamos las instancias. Como mucho hay alias + 2 ===========================

	iLength = m_oT3Alias.GetLength() + 2 ;
	m_poInstancesArray = new ClTempInstance[ iLength ] ;
	CHECK_CHLOG_ERRORF( m_poInstancesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempInstance ) * iLength ) << __LINE__ << __FILE__ ) ;

	m_oInstances.SetDelta( iLength ) ;


// Rellenamos con los alias ===================================================

	iInstance = 0 ;

    iNodes = m_oNodes.GetLength() ;

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

        iT3Alias = poNode->GetNumberOfT3Alias() ;

        for( j = 0 ; j < iT3Alias ; j++ )
        {
		    poT3Alias = poNode->GetT3AliasByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poT3Alias == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

            pccInstanceId = poT3Alias->GetInstanceId() ;
            poInstance = GetInstanceById( pccInstanceId, M4_FALSE ) ;

            if( poInstance != NULL )
            {
				// Si existe comprobar que tienen los mismos datos
                pccT3Id = poInstance->GetT3Id() ;
                pccT3IdAux = poT3Alias->GetT3Id() ;
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccT3Id, pccT3IdAux ) != 0, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Meta4Object" << pccT3Id << pccT3IdAux ) ;

                pccRoleId = poInstance->GetRoleId() ;
                pccRoleIdAux = poT3Alias->GetRoleId() ;
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccRoleId, pccRoleIdAux ) != 0, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Role" << pccRoleId << pccRoleIdAux ) ;

                pccOrg = poInstance->GetOrgId() ;
                pccOrgAux = poT3Alias->GetOrgId() ;
				CHECK_CHLOG_ERRORF( M4MdFacStrcmp( pccOrg, pccOrgAux ) != 0, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Organization" << pccOrg << pccOrgAux ) ;

                iScope = poInstance->GetScope() ;
                iScopeAux = poT3Alias->GetScope() ;
				CHECK_CHLOG_ERRORF( iScope != iScopeAux, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Scope" << iScope << iScopeAux ) ;

                iOpenMode = poInstance->GetOpenMode() ;
                iOpenModeAux = poT3Alias->GetOpenMode() ;
				CHECK_CHLOG_ERRORF( iOpenMode != iOpenModeAux, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Open Mode" << iOpenMode << iOpenModeAux ) ;

                iCsAccess = poInstance->GetCsAccess() ;
                iCsAccessAux = poT3Alias->GetCsAccess() ;
				CHECK_CHLOG_ERRORF( iCsAccess != iCsAccessAux, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "CS Access" << iCsAccess << iCsAccessAux ) ;

                iOrgType = poInstance->GetOrgType() ;
                iOrgTypeAux = poT3Alias->GetOrgType() ;
				CHECK_CHLOG_ERRORF( iOrgType != iOrgTypeAux, M4_ERROR, M4_MDFAC_AMBIGUOUS_ALIAS_INSTANCE, pccInstanceId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Organization Type" << iOrgType << iOrgTypeAux ) ;
            }
            else
            {
				// Si no existe se crea con el alias
				// Ojo, por lo de los punteros al pool, que se realocan

				iResult = M4MdFacSetLocalId( pccInstanceId, acInstance, M4CL_MAX_T3_ID, M4_FALSE ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


                pccT3Id   = poT3Alias->GetT3Id() ;
                pccRoleId = poT3Alias->GetRoleId() ;
                pccOrg    = poT3Alias->GetOrgId() ;
                iScope    = poT3Alias->GetScope() ;
                iOpenMode = poT3Alias->GetOpenMode() ;
                iCsAccess = poT3Alias->GetCsAccess() ;
                iOrgType  = poT3Alias->GetOrgType() ;


				// Añadimos la instancia
				poInstance = &( m_poInstancesArray[ iInstance++ ] ) ;

                iResult = AddInstance( poInstance, acInstance + 1, pccT3Id, pccRoleId, pccOrg, iScope, iOpenMode, iCsAccess, 1, iOrgType ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
            }
        }
    }


	poLogonAdaptor = m_poFactory->_GetLogonAdaptor() ;


// Hacemos el apaño de la instancia del canal sesión
// Para el propio canal sesión también hay que hacerlo

	iLength = m_oInstances.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oInstances.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poInstance = ( ClTempInstance* ) pvPointer ;

		pccT3Id = poInstance->GetT3Id() ;

		if( strcmp( pccT3Id, M4_SESSION_CHANNEL_ID ) == 0 )
		{
			poInstance->SetScope( M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED ) ;
			poInstance->SetCsAccess( M4CL_CSTYPE_L2_SITE ) ;

			// OJO con esta cosa !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// En SM es Both de verdad. En el resto según el sitio
			// Si es el canal sesión en SM le ponemos que cree uno nuevo basurilla

			if( m_poFactory->p_GetContext() == eM4MdFacClientEmul || m_poFactory->p_GetContext() == eM4MdFacServerEmul )
			{
				poInstance->SetCsAccess( M4CL_CSTYPE_L2_BOTH ) ;

				// Va aquí porque es del pool
				pccThisT3Id = GetChannelId() ;

				if( pccThisT3Id != NULL && strcmp( pccThisT3Id, M4_SESSION_CHANNEL_ID ) == 0 )
				{
					poInstance->SetScope( M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED ) ;
				}
			}

			poInstance->SetUseRole( 0 ) ;
			poInstance->SetOrgType( M4CL_ORGANIZATION_L2_TYPE_ANY ) ; // Este no hace falta con el otro flag
		}
	}



// Se rellenan las instancias fijas ===========================================

    poInstance = GetInstanceById( M4_INSTANCE_EXCHANGE_RATES, M4_FALSE ) ;

    if( poInstance == NULL )
	{
		// Nos conseguimos el nombre del canal moneda del canal sesión si no soy el canal sesión

		pccExchangeRates = M4_T3_EXCHANGE_RATES ;

		// Va aquí porque es del pool
		pccThisT3Id = GetChannelId() ;

		// para que en el canal sesión en server no sea recursivo
		if( pccThisT3Id != NULL && strcmp( pccThisT3Id, M4_SESSION_CHANNEL_ID ) != 0 )
		{
			if( poLogonAdaptor != NULL )
			{
				poRecordSet = poLogonAdaptor->GetSessionRecordSet() ;

				if( poRecordSet != NULL )
				{
					poCMCR = poRecordSet->GetpNodeDef()->GetpCMCR() ;
					iNode = poRecordSet->GetpNodeDef()->GetHandle() ;
					
					iItem = poCMCR->GetNodeItemHandleById( iNode, M4_SCH_SESSION_T3_EXCHANGE_RATES ) ;

					if( iItem != 0 )
					{
						iResult = poRecordSet->Current.Item[ ClHandle( iItem ) ].Value.Get( vValue ) ;

						if( iResult == M4_SUCCESS )
						{
							if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
							{
								pccExchangeRates = vValue.Data.PointerChar ;
							}
						}
					}
				}
			}
		}

		// Añadimos la instancia
		poInstance = &( m_poInstancesArray[ iInstance++ ] ) ;
		
		//JCM 12-2011 Añadimos la instancia del canal de monedas, para que lo levante MONO 
		//(antes estaba ANY) para poder soportar la tabla de monedas multi. 
		iResult = AddInstance( poInstance ,M4_INSTANCE_EXCHANGE_RATES, pccExchangeRates, "", "", M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED, M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_BLOCK, M4CL_CSTYPE_L2_SITE, 1, M4CL_ORGANIZATION_L2_TYPE_MONO ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}
	else
	{
		// En cualquier caso, que no se use sociedad y role
		poInstance->SetUseRole( 0 ) ;
	}
	

    poInstance = GetInstanceById( M4_INSTANCE_M4_ERRORS, M4_FALSE ) ;

    if( poInstance == NULL )
	{
		// Añadimos la instancia
		poInstance = &( m_poInstancesArray[ iInstance++ ] ) ;

        iResult = AddInstance( poInstance, M4_INSTANCE_M4_ERRORS, M4_T3_M4_ERRORS, "", "", M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED, M4CL_T3ALIAS_OPEN_MODE_NO_LOAD, M4CL_CSTYPE_L2_SITE, 0, M4CL_ORGANIZATION_L2_TYPE_ANY ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}
	else
	{
		// En cualquier caso, que no se use sociedad y role
		poInstance->SetUseRole( 0 ) ;
	}

	
	m_oInstances.Sort( M4ClCompareObjectsById ) ;


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: FillBusinessMethodsWithArgs( void )
{

    m4bool_t				bFound ;
    m4uint16_t				i ;
    m4uint16_t				iLength ;
    m4uint16_t				iMethod ;
    m4uint16_t				iMethodNumber ;
    m4pcchar_t				pccMethodId ;
    m4pcchar_t				pccStoredMethodId ;
    ClTempBusinessMethod	*poMethod ;
    ClTempBusinessMethod	*poLastMethod ;
    ClTempBusinessArg		*poArg ;
    ClTempBusinessArg		**ppoArg ;

    
	iMethod = 0 ;
	poLastMethod = NULL ;

    iLength = m_oBusinessArgs.GetLength() ;
    iMethodNumber = m_oBusinessMethods.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al argumento dentro del array de punteros
		ppoArg = GetBusinessArgPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoArg == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poArg = *ppoArg ;
		CHECK_CHLOG_DEBUGF( poArg == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccMethodId = poArg->GetMethodId() ;


		bFound = M4_FALSE ;
		poMethod = NULL ;

		// Buscamos de manera secuencial porque está ordenado
		while( bFound == M4_FALSE && iMethod < iMethodNumber )
		{
			poMethod = GetBusinessMethodByPosition( iMethod ) ;
			CHECK_CHLOG_DEBUGF( poMethod == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccStoredMethodId = poMethod->GetMethodId() ;
			CHECK_CHLOG_DEBUGF( pccStoredMethodId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( M4MdFacStrcmp( pccMethodId, pccStoredMethodId ) == 0 )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iMethod++ ;
			}
		}

		// Es imposible que no lo haya encontrado
		CHECK_CHLOG_ERRORF( bFound == M4_FALSE, M4_ERROR, M4_MDFAC_NO_SUCH_METHOD, pccMethodId << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << "Business Arg" ) ;


		// Establecemos las referencias si es un nuevo nodo
		if( poMethod != poLastMethod )
		{
			poMethod->InitArguments( ppoArg ) ;
			poLastMethod = poMethod ;
		}


		// Incrementamos el número de argumentos que tiene este método de negocio
		poMethod->AddArgument() ;
	}


    return( M4_SUCCESS ) ;
}




// Funciones de impresión =====================================================

m4pcchar_t	ClTempChannel :: iM4Obj( void ) const
{
	return( GetChannelId() ) ;
}


m4pcchar_t	ClTempChannel :: nM4Obj( void ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetName() ;

	if( pccResult == NULL || *pccResult == '\0' )
	{
		pccResult = GetChannelId() ;
	}

	return( pccResult ) ;
}



// Funciones de serialización =================================================

m4return_t	ClTempChannel :: PreGetSize( void )
{

    m4return_t	    iResult ;
	m4int8_t		iSentenceNumber ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4pvoid_t	    pvPointer ;
	ClTempNode	    *poNode ;
	ClTempTi	    *poTi ;
	ClTempNitem     *poNitem ;
	ClTempSentence	*poSentence ;


    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNode->PreGetSize() ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			if( poNode->GetItemsInGroups() > 0 )
			{
				iResult = poNode->CalculateGroupOrder() ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}

// Calculamos las sentencias para todas las TIs que pertenezcan a un nodo

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			poTi = poNode->GetTi() ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iIsToCompile = poTi->GetIsToCompile() ;
			CHECK_CHLOG_DEBUGF( iIsToCompile == 0, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poSentence = poTi->GetSentence() ;

			if( poSentence != NULL )
			{
				// Si la TI ya está procesada hay que saltarla
				continue ;
			}

			iSentenceNumber = poTi->GetSentenceNumber() ;

			if( iSentenceNumber == -1 )
			{
				iResult = poTi->CalculateDynamicSentence() ;
			}
			else
			{
				iResult = poTi->CalculateInheritedSentence( m_poHsParser ) ;
			}

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


	if( m_poSystemNode != NULL )
	{
		iLength = m_poSystemNode->GetNumberOfNitems() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poNitem = m_poSystemNode->GetNitemByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = poNitem->PreGetSize() ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

	m4return_t				iResult ;
	m4uint8_t				iIsToCompile ;
	m4uint16_t				i ;
	m4uint16_t				iLength ;
	m4uint16_t				iInherits ;
	m4pvoid_t				pvPointer ;
	ClTempNode				*poNode ;
	ClTempTi				*poTi ;
	ClTempSentence			*poSentence ;
	ClTempInstance			*poInstance ;
	ClTempNitem				*poNitem ;
	ClTempBusinessMethod	*poBusinessMethod ;


// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_8( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_CHANNEL_FIXED_SIZE ;


// Variables ==================================================================

//	iLength = m_oNodes.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iNodesToCompile * ( M4CL_MCR_CHANNEL_NODE_SIZE ) ;


	iLength = m_oRoots.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iRootsStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_ROOT_SIZE ) ;


	iLength = m_oSentences.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iSentencesStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_SENTENCE_SIZE ) ;


	iLength = m_oInstances.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iInstancesStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_INSTANCE_SIZE ) ;


	if( m_poSystemNode != NULL )
	{
		iLength = m_poSystemNode->GetNumberOfNitems() ;
	}
	else
	{
		iLength = 0 ;
	}

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iSystemItemsStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_ITEM_SIZE ) ;


//    iLength = m_oTis.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iTisStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iTisToCompile * ( M4CL_MCR_CHANNEL_TI_SIZE ) ;


//    iLength = m_oParameters.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iParametersStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iParametersToCompile * ( M4CL_MCR_CHANNEL_PARAMETER_SIZE ) ;


    iLength = m_oRSMs.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iRSMsStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_RSMS_SIZE ) ;


    iLength = m_oBusinessMethods.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
    m_iBusinessMethodsStart = ao_iSize ;
	ao_iSize += M4_SIZE_OF_INT32 ;  // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE ) ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_M4OBJ, ao_iSize - m_iHandle ) ;
	}


// Componentes ================================================================

	iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNode->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Ti del sistema
    if( m_poSystemTi != NULL )
    {
		iResult = m_poSystemTi->GetSize( ao_iSize, 0, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


// SysLoad Client
    if( m_poClientSysLoadItem != NULL && m_poClientSysLoadItem->GetIsToCompile() == 1 )
    {
		iResult = m_poClientSysLoadItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysPersist Client
    if( m_poClientSysPersistItem != NULL && m_poClientSysPersistItem->GetIsToCompile() == 1 )
    {
		iResult = m_poClientSysPersistItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysLoad Server
    if( m_poServerSysLoadItem != NULL && m_poServerSysLoadItem->GetIsToCompile() == 1 )
    {
		iResult = m_poServerSysLoadItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysPersist Server
    if( m_poServerSysPersistItem != NULL && m_poServerSysPersistItem->GetIsToCompile() == 1 )
    {
		iResult = m_poServerSysPersistItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// ExecuterReport
    if( m_poExecuteReportItem != NULL && m_poExecuteReportItem->GetIsToCompile() == 1 )
    {
		iResult = m_poExecuteReportItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// Dyn Filter
    if( m_poDynFilterItem != NULL && m_poDynFilterItem->GetIsToCompile() == 1 )
    {
		iResult = m_poDynFilterItem->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


	iInherits = m_oT3Inherits.GetLength() ;

    iLength = m_oTis.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oTis.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTi = ( ClTempTi* ) pvPointer ;
		iIsToCompile = poTi->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poTi->GetSize( ao_iSize, iInherits, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    iLength = m_oSentences.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oSentences.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSentence = ( ClTempSentence* ) pvPointer ;
		iResult = poSentence->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    iLength = m_oInstances.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oInstances.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poInstance = ( ClTempInstance* ) pvPointer ;
		iResult = poInstance->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    
    iLength = m_oBusinessMethods.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oBusinessMethods.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBusinessMethod = ( ClTempBusinessMethod* ) pvPointer ;
		iResult = poBusinessMethod->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    
	if( m_poSystemNode != NULL )
	{
		iLength = m_poSystemNode->GetNumberOfNitems() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poNitem = m_poSystemNode->GetNitemByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = poNitem->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
            }
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempChannel :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

	m4return_t				iResult ;
	m4uint8_t				iIsToCompile ;
	m4uint16_t				i ;
	m4uint16_t				iLength ;
	m4uint16_t				iInherits ;
	m4uint32_t				iOffset ;
	m4uint32_t				iHandle ;
	m4uint32_t				iId ;
	m4uint32_t				iRSMId ;
	m4uint32_t				iMaskId ;
	m4pvoid_t				pvPointer ;
	ClTempTi				*poTi ;
	ClTempNode				*poNode ;
	ClTempSentence			*poSentence ;
	ClTempInstance			*poInstance ;
	ClTempNitem				*poNitem ;
	ClTempRSM				*poRSM ;
	ClTempBusinessMethod	*poBusinessMethod ;
 

	iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSize ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iBuild ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iTimeUnit ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iCsType ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iLanguage ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, 0 ) ;

    M4ClPackDate ( ai_pcBuffer, iOffset, m_dStartDate ) ;
	M4ClPackDate ( ai_pcBuffer, iOffset, m_dEndDate ) ;
    M4ClPackDate ( ai_pcBuffer, iOffset, m_dCorStartDate ) ;
	M4ClPackDate ( ai_pcBuffer, iOffset, m_dCorEndDate ) ;
    M4ClPackDate ( ai_pcBuffer, iOffset, m_dVersion ) ;
	M4ClPackDate ( ai_pcBuffer, iOffset, m_dCreationDate ) ;
	M4ClPackDate ( ai_pcBuffer, iOffset, m_dCacheMaxPeriod ) ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iChannelId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iName + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, 0 ) ;	// Licencia. Deprecado
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iPool ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iOleClassId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iServiceId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iServiceAltId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iEnvironmentVars ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iOwnerFlag ) ;

    if( m_poSysLoad != NULL )
    {
        iHandle = m_poSysLoad->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poSysPersist != NULL )
    {
        iHandle = m_poSysPersist->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iRootsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSentencesStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iInstancesStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSystemItemsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iTisStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iParametersStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iRSMsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iBusinessMethodsStart ) ;

    if( m_poSystemTi != NULL )
    {
		iHandle = m_poSystemTi->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poClientSysLoadItem != NULL && m_poClientSysLoadItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poClientSysLoadItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poClientSysPersistItem != NULL && m_poClientSysPersistItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poClientSysPersistItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poServerSysLoadItem != NULL && m_poServerSysLoadItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poServerSysLoadItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poServerSysPersistItem != NULL && m_poServerSysPersistItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poServerSysPersistItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poExecuteReportItem != NULL && m_poExecuteReportItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poExecuteReportItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poDynFilterItem != NULL && m_poDynFilterItem->GetIsToCompile() == 1 )
    {
		iHandle = m_poDynFilterItem->GetHandle() ;
	}
	else
	{
		iHandle = 0 ;
	}

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iSecurity ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iVMCapacityReq ) ;

	M4ClPackInt8( ai_pcBuffer, iOffset, m_iType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsSeparable ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCsExeType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsExternal ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iHasSecurity ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsCacheable ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsMDCacheable ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCheckConcurrency ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iOrganizationType ) ;

	iInherits = m_oT3Inherits.GetLength() ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m4uint8_t( iInherits ) ) ;
	
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iDataLanguage ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iMetaLanguage ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCreationType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iStyleUse ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iWSSSecurity ) ;


	iLength = m_oNodes.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iNodesToCompile ) ;

// variables

	iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iId = poNode->GetNodeIdHandle() ;
			iHandle = poNode->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ; // - iOffset para el offset relativo
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


// roots

	iLength = m_oRoots.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oRoots.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

		iId = poNode->GetNodeIdHandle() ;
		iHandle = poNode->GetHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ; // - iOffset para el offset relativo
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


// sentences

	iLength = m_oSentences.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oSentences.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSentence = ( ClTempSentence* ) pvPointer ;
        iId = poSentence->GetSentenceIdHandle() ;
		iHandle = poSentence->GetHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


// instances

	iLength = m_oInstances.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oInstances.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poInstance = ( ClTempInstance* ) pvPointer ;
        iId = poInstance->GetInstanceIdHandle() ;
		iHandle = poInstance->GetHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


// items

	if( m_poSystemNode != NULL )
	{
		iLength = m_poSystemNode->GetNumberOfNitems() ;
	}
	else
	{
		iLength = 0 ;
	}

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		poNitem = m_poSystemNode->GetNitemByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        iId = poNitem->GetNitemIdHandle() ;
		iHandle = poNitem->GetHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


// tis

	iLength = m_oTis.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iTisToCompile ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oTis.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTi = ( ClTempTi* ) pvPointer ;
		iIsToCompile = poTi->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iId = poTi->GetTiIdHandle() ;
			iHandle = poTi->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


// parameters

	iLength = m_oParameters.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iParametersToCompile ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oParameters.GetElement( i, pvPointer ) ;
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


// RSMs

	iLength = m_oRSMs.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oRSMs.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRSM = ( ClTempRSM* ) pvPointer ;

		iRSMId = poRSM->GetRSMIdHandle() ;
		iMaskId = poRSM->GetMaskIdHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iRSMId - iOffset + ai_iBufferSize ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iMaskId + ai_iBufferSize ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}



// Business Method

	iLength = m_oBusinessMethods.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oBusinessMethods.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBusinessMethod = ( ClTempBusinessMethod* ) pvPointer ;
        iId = poBusinessMethod->GetMethodIdHandle() ;
		iHandle = poBusinessMethod->GetHandle() ;

		M4ClPackInt32( ai_pcBuffer, iOffset, iId - iOffset + ai_iBufferSize ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


// componentes ===============

	iLength = m_oNodes.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNode->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


// Ti del sistema
    if( m_poSystemTi != NULL )
    {
		iResult = m_poSystemTi->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize, 0 ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


// SysLoad Client
    if( m_poClientSysLoadItem != NULL && m_poClientSysLoadItem->GetIsToCompile() == 1 )
    {
		iResult = m_poClientSysLoadItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysPersist Client
    if( m_poClientSysPersistItem != NULL && m_poClientSysPersistItem->GetIsToCompile() == 1 )
    {
		iResult = m_poClientSysPersistItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysLoad Server
    if( m_poServerSysLoadItem != NULL && m_poServerSysLoadItem->GetIsToCompile() == 1 )
    {
		iResult = m_poServerSysLoadItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// SysPersist Server
    if( m_poServerSysPersistItem != NULL && m_poServerSysPersistItem->GetIsToCompile() == 1 )
    {
		iResult = m_poServerSysPersistItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// ExecuterReport
    if( m_poExecuteReportItem != NULL && m_poExecuteReportItem->GetIsToCompile() == 1 )
    {
		iResult = m_poExecuteReportItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }

// Dyn Filter
    if( m_poDynFilterItem != NULL && m_poDynFilterItem->GetIsToCompile() == 1 )
    {
		iResult = m_poDynFilterItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


// tis

	iInherits = m_oT3Inherits.GetLength() ;

    iLength = m_oTis.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oTis.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTi = ( ClTempTi* ) pvPointer ;
		iIsToCompile = poTi->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poTi->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize, iInherits ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    iLength = m_oSentences.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oSentences.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSentence = ( ClTempSentence* ) pvPointer ;
		iResult = poSentence->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    iLength = m_oInstances.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oInstances.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poInstance = ( ClTempInstance* ) pvPointer ;
		iResult = poInstance->Compile( ai_pcBuffer, ai_iBufferSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    
    iLength = m_oBusinessMethods.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oBusinessMethods.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBusinessMethod = ( ClTempBusinessMethod* ) pvPointer ;
		iResult = poBusinessMethod->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    
	if( m_poSystemNode != NULL )
	{
		iLength = m_poSystemNode->GetNumberOfNitems() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poNitem = m_poSystemNode->GetNitemByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = poNitem->Compile( this, ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

    return( M4_SUCCESS ) ;
}




// Funciones de client server =================================================

m4return_t	ClTempChannel :: ComputeCS( m4uint8_t ai_iCsType )
{

	m4return_t			iResult ;
	m4bool_t			bIsRootLoad ;
	m4bool_t			bIsPersistTree ;
	m4bool_t			bAddSysLoad ;
	m4bool_t			bAddSysPersist ;
	m4uint8_t			iIsRealRoot ;
	m4uint8_t			iType ;
	m4uint8_t			iM4Type ;
	m4uint8_t			iInternalType ;
	m4uint8_t			iScope ;
	m4uint8_t			iItemCsType ;
	m4uint8_t			iNodeCsType ;
	m4uint8_t			iSonNodeCsType ;
	m4uint8_t			iNitemConCsType ;
	m4uint8_t			iItemReducedCsType ;
	m4uint8_t			iNodeReducedCsType ;
	m4uint8_t			iFatherIsToCompile ;
	m4uint8_t			iSonIsToCompile ;
	m4uint8_t			iIsToCompile ;
	m4uint16_t			i, j, k ;
	m4uint16_t			iLength ;
	m4uint16_t			iConnectors ;
	m4uint16_t			iItems ;
	m4uint16_t			iNitems ;
	m4uint16_t			iIndexes ;
	m4uint16_t			iIndexItems ;
	m4uint16_t			iPlugs ;
	m4uint16_t			iPlugItems ;
	m4uint16_t			iFilters ;
	m4uint16_t			iSysSentences ;
	m4uint16_t			iSysHints ;
	m4uint16_t			iAlias ;
	m4uint16_t			iArguments ;
	m4uint16_t			iItemsToCompile ;
	m4uint16_t			iNitemsToCompile ;
	m4uint16_t			iConnectorsToCompile ;
	m4uint16_t			iSysSentencesToCompile ;
	m4uint16_t			iSysHintsToCompile ;
	m4uint16_t			iAliasToCompile ;
	m4uint16_t			iInherits ;
	m4pcchar_t          pccItemId ;
	m4pvoid_t			pvPointer ;
	ClTempTi			*poTi ;
	ClTempTi			*poBaseTi ;
	ClTempTiInherit		*poTiInherit ;
	ClTempNode			*poNode ;
	ClTempNode			*poSonNode ;
	ClTempNode			*poAlias ;
	ClTempConnector		*poConnector ;
	ClTempItem			*poItem ;
	ClTempRitem			*poRitem ;
	ClTempNitem			*poNitem ;
	ClTempNitem			*poSonNitem ;
	ClTempNitemConn		*poNitemConn ;
	ClTempIndex			*poIndex ;
	ClTempIndexItem		*poIndexItem ;
	ClTempPlug			*poPlug ;
	ClTempPlugItem		*poPlugItem ;
	ClTempFilter		*poFilter ;


// También hacemos el cálculo del ofsset de seguridad

	m_iSecurity = M4CL_SCR_SECURITY_FIXED_SIZE ;

// Nodos que entran ==========

    iLength = m_oNodes.GetLength() ;

	m_iNodesToCompile = 0 ;
    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

        iNodeCsType = poNode->GetCsType() ;

		if( ( iNodeCsType & ai_iCsType ) == 0 )
		{
			poNode->SetIsToCompile( 0 ) ;
		}
		else
		{
			poNode->SetIsToCompile( 1 ) ;
			poNode->SetIndex( m_iNodesToCompile ) ;
			m_iNodesToCompile++ ;

			poNode->SetSCRStart( m_iSecurity ) ;
			m_iSecurity += M4CL_SCR_NODE_MASK_SIZE ;
		}
	}

// Se calculan los nodos raiz

    iResult = ComputeRoots() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// Connectores que entran ==========

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iFatherIsToCompile = poNode->GetIsToCompile() ;

		iConnectors = poNode->GetNumberOfConnectors() ;

		iConnectorsToCompile = 0 ;
		for( j = 0 ; j < iConnectors ; j++ )
		{
			poConnector = poNode->GetConnectorByPosition( j ) ;

			poSonNode = poConnector->GetSonNode() ;
			iSonIsToCompile = poSonNode->GetIsToCompile() ;

			if( iFatherIsToCompile == 0 )
			{
				poConnector->SetIsToCompile( 0 ) ;

				if( iSonIsToCompile == 1 && poSonNode != poNode )
				{
					if( m_iIsSeparable == 1 && m_iCsType != M4CL_CSTYPE_BOTH )
					{
						poSonNode = poConnector->GetSonNode() ;

						iSonNodeCsType = poSonNode->GetCsType() ;
						iNodeCsType = poNode->GetCsType() ;

						DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_NODE_CS_TYPE, LOG_CAT_AND( poSonNode->iNode(), poSonNode->nNode() ) << M4ClCsType( iSonNodeCsType ) << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << M4ClCsType( iNodeCsType ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
						return( M4_ERROR ) ;
					}
				}
			}
			else
			{
				if( iSonIsToCompile == 0 )
				{
					poConnector->SetIsToCompile( 0 ) ;
				}
				else
				{
					poConnector->SetIsToCompile( 1 ) ;
					iConnectorsToCompile++ ;
				}
			}
		}

		poNode->SetConnectorsToCompile( iConnectorsToCompile ) ;
	}


// Nitems, syssentences, syshints y alias que entran ==========
// También controlamos los cs type de los items respecto al nodo

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

		iNodeCsType = poNode->GetCsType() ;
		// esto nunca puede pasar porque se controla antes
		CHECK_CHLOG_DEBUGF( iNodeCsType > M4CL_CSTYPE_MAX_NUMBER || iNodeCsType < M4CL_CSTYPE_MIN_NUMBER, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iNodeReducedCsType = g_aiReducedNodeCsTypesArray[ iNodeCsType - 1 ] ;


		iFatherIsToCompile = poNode->GetIsToCompile() ;

		// Items
		iNitems = poNode->GetNumberOfNitems() ;

		iNitemsToCompile = 0 ;
		for( j = 0 ; j < iNitems ; j++ )
		{
			poNitem = poNode->GetNitemByPosition( j ) ;

			if( iFatherIsToCompile == 0 )
			{
				poNitem->SetIsToCompile( 0 ) ;
			}
			else
			{
				iItemCsType = poNitem->GetCsType() ;

				if( ( iItemCsType & ai_iCsType ) == 0 )
				{
					poNitem->SetIsToCompile( 0 ) ;
				}
				else
				{
					poNitem->SetIsToCompile( 1 ) ;
					poNitem->SetIndex( iNitemsToCompile ) ;
					iNitemsToCompile++ ;

					poNitem->SetSCRStart( m_iSecurity ) ;
					m_iSecurity += M4CL_SCR_ITEM_MASK_SIZE ;


					// Esto nunca puede pasar porque se controla antes
					CHECK_CHLOG_DEBUGF( iItemCsType > M4CL_CSTYPE_MAX_NUMBER || iItemCsType < M4CL_CSTYPE_MIN_NUMBER, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					// Control de cs types por nodo
					// Sólo para canales separables y cstype both
					if( m_iIsSeparable == 1 && m_iCsType != M4CL_CSTYPE_BOTH )
					{
						iItemReducedCsType = g_aiReducedItemCsTypesArray[ iItemCsType - 1 ] ;

						if( g_aiValidItemsCsTypesArray[ iNodeReducedCsType ][ iItemReducedCsType ] == 0 )
						{
							if( M4ChLogIsErrorIn( M4_MDFAC_BAD_ITEM_CS_TYPE ) == 0 )
							{
								DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_ITEM_CS_TYPE, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << M4ClCsType( iItemCsType ) << M4ClCsType( iNodeCsType ) ) ;
							}
						}
					}

					// Se asignan los items de BD del canal
					poItem = poNitem->GetItem() ;
					CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					iInternalType = poItem->GetInternalType() ;

					if( iInternalType == M4CL_INTERNAL_TYPE_SYSDB_ITEM )
					{
						pccItemId = poItem->GetIdentifier() ;
						CHECK_CHLOG_DEBUGF( pccItemId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						if( m_poClientSysLoad == NULL && strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_CLIENT ) == 0 )
						{
							m_poClientSysLoad = poNitem ;
						}
						else if( m_poClientSysPersist == NULL && strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_CLIENT ) == 0 )
						{
							m_poClientSysPersist = poNitem ;
						}
						else if( m_poServerSysLoad == NULL && strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_LOAD_SERVER ) == 0 )
						{
							m_poServerSysLoad = poNitem ;
						}
						else if( m_poServerSysPersist == NULL && strcmp( pccItemId, M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_SERVER ) == 0 )
						{
							m_poServerSysPersist = poNitem ;
						}
					}
				}
			}
		}

		poNode->SetItemsToCompile( iNitemsToCompile ) ;


		// SysSentences
		iSysSentences = poNode->GetNumberOfSysSentences() ;

		iSysSentencesToCompile = 0 ;
		for( j = 0 ; j < iSysSentences ; j++ )
		{
			poNitem = poNode->GetSysSentenceByPosition( j ) ;

			iIsToCompile = poNitem->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iSysSentencesToCompile++ ;
			}
		}

		poNode->SetSysSentencesToCompile( iSysSentencesToCompile ) ;


		// SysHints
		iSysHints = poNode->GetNumberOfSysHints() ;

		iSysHintsToCompile = 0 ;
		for( j = 0 ; j < iSysHints ; j++ )
		{
			poNitem = poNode->GetSysHintByPosition( j ) ;

			iIsToCompile = poNitem->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iSysHintsToCompile++ ;
			}
		}

		poNode->SetSysHintsToCompile( iSysHintsToCompile ) ;


		// Alias
		iAlias = poNode->GetNumberOfAlias() ;

		iAliasToCompile = 0 ;
		for( j = 0 ; j < iAlias ; j++ )
		{
			poAlias = poNode->GetAliasByPosition( j ) ;

			iIsToCompile = poAlias->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iAliasToCompile++ ;
			}
		}

		poNode->SetAliasToCompile( iAliasToCompile ) ;
	}


// Conectores de nitems que entran ==========

	bAddSysLoad = M4_TRUE ;
	bAddSysPersist = M4_TRUE ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsRealRoot = poNode->GetIsRealRoot() ;

		iNitems = poNode->GetNumberOfNitems() ;

		for( j = 0 ; j < iNitems ; j++ )
		{
			bIsRootLoad = M4_FALSE ;
			bIsPersistTree = M4_FALSE ;

			poNitem = poNode->GetNitemByPosition( j ) ;
			poRitem = poNitem->GetRitem() ;
			poItem = poNitem->GetItem() ;

			pccItemId = poItem->GetIdentifier() ;
			CHECK_CHLOG_DEBUGF( pccItemId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iItemCsType = poNitem->GetCsType() ;

			if( strcmp( pccItemId, M4CL_ITEM_ROOT_LOAD ) == 0 )
			{
				bIsRootLoad = M4_TRUE ;

				/* Bug 0106868
				Si el item no es mapped no se puede aplicar la optimización
				*/
				if( iIsRealRoot == 1 && iItemCsType != M4CL_CSTYPE_MAPPED )
				{
					bAddSysLoad = M4_FALSE ;
				}
			}
			else if( strcmp( pccItemId, M4CL_ITEM_PERSIST_TREE ) == 0 )
			{
				bIsPersistTree = M4_TRUE ;

				/* Bug 0106868
				Si el item no es mapped no se puede aplicar la optimización
				*/
				if( iIsRealRoot == 1 && iItemCsType != M4CL_CSTYPE_MAPPED )
				{
					bAddSysPersist = M4_FALSE ;
				}
			}

			iM4Type = poItem->GetType() ;
			iFatherIsToCompile = poNitem->GetIsToCompile() ;

			iConnectors = poRitem->GetNumberOfConnectors() ;

			iConnectorsToCompile = 0 ;
			for( k = 0 ; k < iConnectors ; k++ )
			{
				poNitemConn = poRitem->GetConnectorByPosition( k ) ;

				if( iFatherIsToCompile == 0 )
				{
					poNitemConn->SetIsToCompile( 0 ) ;
				}
				else
				{
					poSonNitem = poNitemConn->GetSonNitem() ;
					iSonIsToCompile = poSonNitem->GetIsToCompile() ;

					if( iSonIsToCompile == 0 )
					{
						poNitemConn->SetIsToCompile( 0 ) ;
					}
					else
					{
						iNitemConCsType = poNitemConn->GetCsType() ;

						if( ( iNitemConCsType & ai_iCsType ) == 0 )
						{
							poNitemConn->SetIsToCompile( 0 ) ;
						}
						else
						{
							poNitemConn->SetIsToCompile( 1 ) ;
							iConnectorsToCompile++ ;

							// Si la ROOTLOAD o PERSIST_TREE de un nodo raiz en cliente
							// tienen conectores que no sean insite o con argumentos,
							// o son de scope register no se aplican los items de BD
							if( ai_iCsType == M4CL_CSTYPE_FRONT && iIsRealRoot == 1 && ( bIsRootLoad == M4_TRUE || bIsPersistTree == M4_TRUE ) )
							{
								poItem = poSonNitem->GetItem() ;
								iType = poItem->GetType() ;
								iScope = poItem->GetScope() ;
								iArguments = poItem->GetNumberOfArguments() ;

								if	(
										iNitemConCsType != M4CL_CSTYPE_MAPPED	// No es insite
									||	iScope == M4CL_ITEM_SCOPE_REGISTER		// Es de registro
									||	iArguments > 0							// Tiene argumentos
									||	iType == M4CL_ITEM_TYPE_PROPERTY		// Es propiedad o campo
									||	iType == M4CL_ITEM_TYPE_FIELD
									)
								{
									if( bIsRootLoad == M4_TRUE )
									{
										bAddSysLoad = M4_FALSE ;
									}
									else if( bIsPersistTree == M4_TRUE )
									{
										bAddSysPersist = M4_FALSE ;
									}
								}
							}
						}
					}
				}
			}

			poRitem->SetConnectorsToCompile( iConnectorsToCompile ) ;
		}
	}


	// Si no hay más de un nodo raiz no QBF no hace falta incluir el SYS_LOAD y SYS_PERSIST
	if( m_iNoQBFRoots < 2 )
	{
		bAddSysLoad = M4_FALSE ;
		bAddSysPersist = M4_FALSE ;
	}

	// Se añaden los items de carga y persistencia
	if( ai_iCsType == M4CL_CSTYPE_FRONT )
	{
		if( bAddSysLoad == M4_TRUE )
		{
			m_poSysLoad = m_poClientSysLoad ;
		}

		if( bAddSysPersist == M4_TRUE )
		{
			/* Bug 0108084
			Se desactiva el item sys_persist
			m_poSysPersist = m_poClientSysPersist ;
			*/
		}
	}
	else	// Back o Both
	{
		if( bAddSysLoad == M4_TRUE )
		{
			m_poSysLoad = m_poServerSysLoad ;
		}

		if( bAddSysPersist == M4_TRUE )
		{
			/* Bug 0108084
			Se desactiva el item sys_persist
			m_poSysPersist = m_poServerSysPersist ;
			*/
		}
	}


// Tis que entran ==========

    iLength = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oTis.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTi = ( ClTempTi* ) pvPointer ;
		poTi->SetIsToCompile( 0 ) ;

		iItems = poTi->GetNumberOfItems() ;

		for( j = 0 ; j < iItems ; j++ )
		{
			poItem = poTi->GetItemByPosition( j ) ;
			poItem->SetIsToCompile( 0 ) ;
		}
	}


	m_iTisToCompile = 0 ;
    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			poTi = poNode->GetTi() ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iInherits = poTi->GetNumberOfInherits() ;

			for( j = 0 ; j < iInherits ; j++ )
			{
				poTiInherit = poTi->GetInheritByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poBaseTi = poTiInherit->GetBaseTi() ;
				CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iIsToCompile = poBaseTi->GetIsToCompile() ;

				if( iIsToCompile == 0 )
				{
					poBaseTi->SetIsToCompile( 1 ) ;
					m_iTisToCompile++ ;
				}
			}
        }
	}


// Parameters que entran ==========

    iLength = m_oParameters.GetLength() ;

	m_iParametersToCompile = 0 ;
    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oParameters.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = ( ClTempNitem* ) pvPointer ;
		iIsToCompile = poNitem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			m_iParametersToCompile++ ;
		}
	}


// Items que entran ==========

    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iNitems = poNode->GetNumberOfNitems() ;

			for( j = 0 ; j < iNitems ; j++ )
			{
				poNitem = poNode->GetNitemByPosition( j ) ;
				iIsToCompile = poNitem->GetIsToCompile() ;

				if( iIsToCompile == 1 )
				{
					poItem = poNitem->GetItem() ;
					poItem->SetIsToCompile( 1 ) ;
				}
			}
		}
	}

    iLength = m_oTis.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oTis.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poTi = ( ClTempTi* ) pvPointer ;
		iItems = poTi->GetNumberOfItems() ;

		iItemsToCompile = 0 ;
		for( j = 0 ; j < iItems ; j++ )
		{
			poItem = poTi->GetItemByPosition( j ) ;
			iIsToCompile = poItem->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iItemsToCompile++ ;
			}
		}

		poTi->SetItemsToCompile( iItemsToCompile ) ;
	}


// Indexes y plugs que entran

    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iFatherIsToCompile = poNode->GetIsToCompile() ;

		// Indexes
		iIndexes = poNode->GetNumberOfIndexes() ;

		for( j = 0 ; j < iIndexes ; j++ )
		{
			poIndex = poNode->GetIndexByPosition( j ) ;

			iIndexItems = poIndex->GetNumberOfItems() ;

			iItemsToCompile = 0 ;
			for( k = 0 ; k < iIndexItems ; k++ )
			{
				poIndexItem = poIndex->GetItemByPosition( k ) ;

				if( iFatherIsToCompile == 0 )
				{
					poIndexItem->SetIsToCompile( 0 ) ;
				}
				else
				{
					poNitem = poIndexItem->GetNitem() ;
					iIsToCompile = poNitem->GetIsToCompile() ;

					if( iIsToCompile == 0 )
					{
						poIndexItem->SetIsToCompile( 0 ) ;
					}
					else
					{
						poIndexItem->SetIsToCompile( 1 ) ;
						iItemsToCompile++ ;
					}
				}
			}

			poIndex->SetItemsToCompile( iItemsToCompile ) ;
		}

		// Plug items
		iPlugs = poNode->GetNumberOfPlugs() ;

		for( j = 0 ; j < iPlugs ; j++ )
		{
			poPlug = poNode->GetPlugByPosition( j ) ;

			iPlugItems = poPlug->GetNumberOfItems() ;

			iItemsToCompile = 0 ;
			for( k = 0 ; k < iPlugItems ; k++ )
			{
				poPlugItem = poPlug->GetItemByPosition( k ) ;

				if( iFatherIsToCompile == 0 )
				{
					poPlugItem->SetIsToCompile( 0 ) ;
				}
				else
				{
					poNitem = poPlugItem->GetSonNitem() ;
					iIsToCompile = poNitem->GetIsToCompile() ;

					if( iIsToCompile == 0 )
					{
						poPlugItem->SetIsToCompile( 0 ) ;
					}
					else
					{
						poPlugItem->SetIsToCompile( 1 ) ;
						iItemsToCompile++ ;
					}
				}
			}

			poPlug->SetItemsToCompile( iItemsToCompile ) ;
		}
	}
			

// Seguridad de filters ==========

    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iFilters = poNode->GetNumberOfFilters() ;

		for( j = 0 ; j < iFilters ; j++ )
		{
			poFilter = poNode->GetFilterByPosition( j ) ;
			poFilter->SetSCRStart( m_iSecurity ) ;
			m_iSecurity += M4CL_SCR_FILTER_MASK_SIZE ;
		}
	}


// Seguridad de t3 connectors  ==========

    iLength = m_oNodes.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iPlugs = poNode->GetNumberOfPlugs() ;

		for( j = 0 ; j < iPlugs ; j++ )
		{
			poPlug = poNode->GetPlugByPosition( j ) ;
			poPlug->SetSCRStart( m_iSecurity ) ;
			m_iSecurity += M4CL_SCR_T3_CON_MASK_SIZE ;
		}
	}


	return( M4_SUCCESS ) ;
}



// Funciones varias ===========================================================

void	ClTempChannel :: OrderT3Inherits( void )
{
	m_oT3Inherits.Sort( M4ClCompareT3InheritsByLevel ) ;
}

void	ClTempChannel :: OrderTiInherits( void )
{
    m_oTiInherits.Sort( M4ClCompareTiInheritsByIdAndLevel ) ;
}

void	ClTempChannel :: OrderArguments( void )
{
    m_oArguments.Sort( M4ClCompareArgumentsByTiItemAndOrder ) ;
}

void	ClTempChannel :: OrderItemFormulas( void )
{
	/* Bug 0194294
	Se ordenan las reglas por la pk completa
	*/
	m_oItemFormulas.Sort( M4ClCompareFormulasByTiItemRuleAndDate ) ;
}

void	ClTempChannel :: OrderTotals( void )
{
	m_oTotals.Sort( M4ClCompareTotalsByTiId ) ;
}

void	ClTempChannel :: OrderTotalTags( void )
{
	m_oTotalTags.Sort( M4ClCompareTotalTagsByTiId ) ;
}

void	ClTempChannel :: OrderTags( void )
{
	m_oTags.Sort( M4ClCompareTagsByTiId ) ;
}

void	ClTempChannel :: OrderNodeInherits( void )
{
	m_oNodeInherits.Sort( M4ClCompareNodeInheritsByNodeAndLevel ) ;
}

void	ClTempChannel :: OrderConnectors( void )
{
	m_oConnectors.Sort( M4ClCompareConnectorsByFatherTypeAndSon ) ;
}

void	ClTempChannel :: OrderInvConnectors( void )
{
	m_oInvConnectors.Sort( M4ClCompareConnectorsBySonTypeAndFather ) ;
}

void	ClTempChannel :: OrderNitemConns( void )
{
	m_oNitemConns.Sort( M4ClCompareNitemConnsByNodeItemAndOrder ) ;
}

void	ClTempChannel :: OrderIndexes( void )
{
	m_oIndexes.Sort( M4ClCompareIndexesByNodeAndIndex ) ;
}

void	ClTempChannel :: OrderIndexItems( void )
{
	m_oIndexItems.Sort( M4ClCompareIndexItemsByNodeIndexAndOrder ) ;
}

void	ClTempChannel :: OrderPlugs( void )
{
	m_oPlugs.Sort( M4ClComparePlugsByNodeUsedT3AndUsedNode ) ;
}

void	ClTempChannel :: OrderPlugItems( void )
{
	m_oPlugItems.Sort( M4ClComparePlugItemsByNodeUsedT3UsedNodeAndItem ) ;
}

void	ClTempChannel :: OrderT3Alias( void )
{
	m_oT3Alias.Sort( M4ClCompareT3AliasByNodeAliasAndLevel ) ;
}

void	ClTempChannel :: OrderFilters( void )
{
	m_oFilters.Sort( M4ClCompareFiltersByNodeAndFilter ) ;
}



m4return_t	ClTempChannel :: OrderItemConnMappings( void )
{

    m4return_t	iResult ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4pvoid_t	pvPointer ;
	ClTempNode	*poNode ;

    
	iLength = m_oNodes.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        iResult = poNode->OrderItemConnMappings() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: CreateTiReferences( void )
{

	m4uint16_t	i ;
	m4uint16_t	iTis ;
	m4uint16_t	iT3sInherit ;
	m4uint16_t	iSize ;
	ClTempTi	*poTi ;


    iTis = m_oTis.GetLength() ;
	iT3sInherit = m_oT3Inherits.GetLength() ;

	iSize = iTis * iT3sInherit ;

	if( iSize > 0 )
	{
		m_poTiReferencesArray = new ClTempTiReference[ iSize ] ;
		CHECK_CHLOG_ERRORF( m_poTiReferencesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempTi ) * iSize ) << __LINE__ << __FILE__ ) ;

		for( i = 0 ; i < iTis ; i++ )
		{
			poTi = GetTiByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poTi->SetReferences( &m_poTiReferencesArray[ i * iT3sInherit ] ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeFormulas( void )
{

    m4return_t          iResult ;
	m4bool_t			bGenerate ;
	m4bool_t			bAdded ;
    m4bool_t			bHasMetarules ;
    m4uint8_t           iType ;
    m4uint8_t           iIsMyRitem ;
    m4uint8_t           iVariableArgs ;
    m4uint16_t			iLength ;
    m4uint16_t          i, j ;
    m4uint16_t          iTotals ;
    m4uint16_t          iTotalTags ;
    m4uint16_t          iTags ;
    m4uint16_t          iFormulas ;
    m4uint16_t          iFormulasBefore ;
	m4date_t			dTimeUnit ;
    ClTempNode          *poNode ;
    ClTempNode          *poRevNode ;
    ClTempItem			*poItem ;
    ClTempNitem         *poNitem ;
    ClTempNitem         *poBackPayNitem ;
    ClTempNitem         *poRevNitem ;
    ClTempNitem         **ppoNitem ;
    ClTempRitem         *poRitem ;
    ClTempTotal			*poTotal ;
    ClTempTotalTag		*poTotalTag ;
    ClTempTag			*poTag ;
    ClTempNitemFormula	*poFormula ;
	ClVMBaseEnv			*poEnvironment ;
	ClLongString		oPolish ;
	ClDateStack			oDateStack ;
	ClSliceArray		oSliceArray ;



	// Inicilaizamos las estructuras siempre

	poEnvironment = m_poFactory->_GetEnvironment() ;

	if( poEnvironment != NULL )
	{
		dTimeUnit = poEnvironment->GetDateTimeUnit() ;
	}
	else
	{
		dTimeUnit = M4_ONE_DAY ;
	}

	oPolish.Init( 4000, 1000 ) ;
	oSliceArray.Init( 20, 100, 500, dTimeUnit ) ;
	oDateStack.Init( m_dStartDate, m_dEndDate, 20, 10 ) ;


// Recorremos los items =======================================================

    iLength = m_oNitems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
    {
		// Conseguimos la dirección del puntero al nitem dentro del array de punteros
		ppoNitem = GetNitemPointerByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( ppoNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitem = *ppoNitem ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iIsMyRitem = poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;

		// Si no soy propietario no hago nada
		if( iIsMyRitem == 0 )
		{
			continue ;
		}

		poRitem = poNitem->GetRitem() ;
		CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Ponemos el nombre del item =========================================
		iResult = poRitem->SetNameInPool() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		/* Bug 0081800
		La exclusión hay que hacerla después de asignar el nombre del item
		*/
		poItem = poNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iType = poItem->GetType() ;

		// Si no es método o concepto no se hace nada
		if( iType != M4CL_ITEM_TYPE_METHOD && iType != M4CL_ITEM_TYPE_CONCEPT )
		{
			continue ;
		}


		poNode = poNitem->GetNode() ;
		CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		// Calculamos los totales =============================================

		iFormulasBefore = poRitem->GetNumberOfFormulas() ;

		iTotals = poRitem->GetNumberOfTotals() ;

		if( iTotals > 0 )
		{
			poBackPayNitem = poNode->GetBackPayNitem() ;

			oSliceArray.Reset() ;

			// Añadimos los elementos al array
			for( j = 0 ; j < iTotals ; j++ )
			{
				poTotal = poRitem->GetTotalByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				bAdded = M4_FALSE ;
				iResult = oSliceArray.AddElement( poTotal, poNitem, bAdded ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}
			}

			// Pasamos a la segunda ventana
			oSliceArray.MoveToSecondBlock() ;

			iTotalTags = poRitem->GetNumberOfTotalTags() ;

			if( iTotalTags > 0 )
			{
				// Añadimos los elementos de condiciones de total al array
				for( j = 0 ; j < iTotalTags ; j++ )
				{
					poTotalTag = poRitem->GetTotalTagByPosition( j ) ;
					CHECK_CHLOG_DEBUGF( poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					bAdded = M4_FALSE ;
					iResult = oSliceArray.AddElement( poTotalTag, poNitem, bAdded ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}

			// Calculamos las polacas de los totales
			iResult = poRitem->CalculateTotals( poNitem, &oSliceArray, &oPolish, &oDateStack, poBackPayNitem, dTimeUnit ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}


		// Calculamos las reglas ==============================================

		oSliceArray.Reset() ;

		iFormulas = poRitem->GetNumberOfFormulas() ;

		// Añadimos los elementos al array, primero los totales
		for( j = iFormulasBefore ; j < iFormulas ; j++ )
		{
			poFormula = poRitem->GetFormulaByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			bAdded = M4_FALSE ;
			iResult = oSliceArray.AddElement( poFormula, poNitem, bAdded ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}

		/*
		Bug 0090072
		El cálculo de metareglas debe ser dinámico
		Si se añade una que sea metarregla el item tiene metareglas
		*/
		bHasMetarules = M4_FALSE ;

		for( j = 0 ; j < iFormulasBefore ; j++ )
		{
			poFormula = poRitem->GetFormulaByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			bAdded = M4_FALSE ;
			iResult = oSliceArray.AddElement( poFormula, poNitem, bAdded ) ;

			if( bHasMetarules == M4_FALSE && bAdded == M4_TRUE && poFormula->GetIsMetarule() == 1 )
			{
				bHasMetarules = M4_TRUE ;
			}

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}


		// Calculamos las metareglas ==========================================

		iVariableArgs = poItem->GetVariableArguments() ;
		iTags = poRitem->GetNumberOfTags() ;

		if( iVariableArgs == 0 && bHasMetarules == M4_FALSE )
		{
			// Si no es de argumentos variables y no tiene metareglas podemos generar
			iFormulas = oSliceArray.GetLength() ;

			// Sólo hace falta generar si
			// 1- Teniendo reglas tiene tags
			// 2- Teniendo reglas no tiene tags y tiene más de una regla
			// 3- Teniendo reglas no tiene tags y tiene una sola regla que es de revisión

			bGenerate = M4_FALSE ;

			if( iFormulas > 0 )
			{
				// Sólo hace falta generar si tiene alguna regla
				if( iTags > 0 )
				{
					// Si tiene tags se genera
					bGenerate = M4_TRUE ;
				}
				else
				{
					// Si no tiene tags
					if( iFormulas > 1 )
					{
						// Si tiene más de una regla hay que generar
						bGenerate = M4_TRUE ;
					}
					else
					{
						// Si tiene una fórmula hay que ver si es de retoractividad
						poFormula = (ClTempNitemFormula*) oSliceArray.GetLeader( 0 ) ;

						if( poFormula != NULL && poFormula->GetIsRetroactivity() == 1 )
						{
							// Si la que hay es de retoractividad hay que generar
							bGenerate = M4_TRUE ;
						}
					}
				}
			}

			if( bGenerate == M4_TRUE )
			{
				poRevNitem = poNode->GetRevNitem() ;
				poRevNode = poNode->GetRevNode() ;

				// Ordenamos por orden de ejecución
				oSliceArray.Sort( M4ClCompareRowsByRevTotalAndDBOrder ) ;

				/* Bug 0089848
				Se pasan los totales al principio
				*/
				iResult = oSliceArray.MoveUp( M4ClCheckRowIsTotal ) ;
				
				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				// Pasamos a la segunda ventana
				oSliceArray.MoveToSecondBlock() ;

				// Añadimos los tags al array
				iFormulasBefore = poRitem->GetNumberOfFormulas() ;

				iTags = poRitem->GetNumberOfTags() ;

				for( j = 0 ; j < iTags ; j++ )
				{
					poTag = poRitem->GetTagByPosition( j ) ;
					CHECK_CHLOG_DEBUGF( poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					bAdded = M4_FALSE ;
					iResult = oSliceArray.AddElement( poTag, poNitem, bAdded ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}

				// Calculamos las polacas de las metareglas
				iResult = poRitem->CalculateMetaRules( poNitem, &oSliceArray, &oPolish, &oDateStack, poRevNode, poRevNitem, dTimeUnit ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				// Volvemos a la primera ventana
				oSliceArray.MoveToFirstBlock() ;


				iFormulas = poRitem->GetNumberOfFormulas() ;

				for( j = iFormulasBefore ; j < iFormulas ; j++ )
				{
					poFormula = poRitem->GetFormulaByPosition( j ) ;
					CHECK_CHLOG_DEBUGF( poFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					bAdded = M4_FALSE ;
					iResult = oSliceArray.AddElement( poFormula, poNitem, bAdded ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
		else
		{
			// Si es de argumentos variables o ya tiene fórmulas no podemos generar metareglas
			// Si además tenía tags hay que dar un warning
			if( iTags > 0 )
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_TAGS_IGNORED, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}


		// Cónsolidamos el array de fórmulas
		iResult = poRitem->CalculateFormulas( poNitem, &oSliceArray, &oPolish, &oDateStack ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeOneDBFormula( ClTempNitem *ai_poNitem, ClLongString *ai_poPolish, m4pcchar_t ai_pccItemId, m4bool_t ai_bAddTransaction, m4bool_t ai_bExcludeBack )
{

    m4return_t			iResult ;
	m4bool_t			bFirst ;
	m4uint8_t			iIsToCompile ;
	m4uint8_t			iQBF ;
	m4uint8_t			iCsType ;
	m4uint8_t			iCheckNotNull ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pcchar_t			pccNodeId ;
	m4pchar_t			pcPolish ;
	m4pvoid_t			pvPointer ;
	ClTempNode			*poNode ;
	ClTempItem			*poItem ;
	ClTempItem			*poMethodItem ;
	ClTempRitem			*poRitem ;
	ClTempNitem			*poMethodNitem ;
	ClTempNitemFormula	*poFormula ;


	ai_poPolish->Reset() ;

	iResult = ai_poPolish->Concat( "C4\n" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	/* Bug 0106148
	De momento no se hace nada transaccional por el impacto
	*/
	ai_bAddTransaction = M4_FALSE ;

	if( ai_bAddTransaction == M4_TRUE )
	{
		iResult = ai_poPolish->Concat( "F89\nC1\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	bFirst = M4_TRUE ;

	iLength = m_oRoots.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oRoots.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;

		iQBF = poNode->GetIsQBFFilter() ;

		if( iQBF == 1 )
		{
			continue ;
		}

		if( ai_bExcludeBack == M4_TRUE )
		{
			iCsType = poNode->GetCsType() ;

			if( iCsType == M4CL_CSTYPE_BACK )
			{
				continue ;
			}
		}

		poMethodNitem = poNode->GetNitemById( ai_pccItemId, M4_TRUE ) ;

		if( poMethodNitem == NULL )
		{
			continue ;
		}

		/* Bug 0106868
		Si el item no se encuentra en esta vista no se puede añadir
		*/
		iIsToCompile = poMethodNitem->GetIsToCompile() ;

		if( iIsToCompile == 0 )
		{
			continue ;
		}

		poMethodItem = poMethodNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poMethodItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iCheckNotNull = poMethodItem->GetFlag( M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ;

		if( iCheckNotNull == 1 )
		{
			poItem = ai_poNitem->GetItem() ;
			CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poItem->SetFlag( M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL ) ;
		}


		if( bFirst == M4_FALSE )
		{
			iResult = ai_poPolish->Concat( "C1\n" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		bFirst = M4_FALSE ;

		pccNodeId = poNode->GetNodeId() ;

		iResult = ai_poPolish->Concat( "S" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ai_poPolish->Concat( pccNodeId ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ai_poPolish->Concat( "\nS" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ai_poPolish->Concat( ai_pccItemId ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		/* Bug 0123677
		Cambia la función a la que se llama
		*/
		iResult = ai_poPolish->Concat( "\nN2.0\nF372\nP1\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	if( ai_bAddTransaction == M4_TRUE )
	{
		iResult = ai_poPolish->Concat( "C1\nN1.0\nF90\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	if( bFirst == M4_FALSE )
	{
		iResult = ai_poPolish->Concat( "C1\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	iResult = ai_poPolish->Concat( "N0.0\n" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// El final
	// El C4 marca el final de la fórmula en ingeniería inversa
	iResult = ai_poPolish->Concat( "C4\n" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ai_poPolish->Concat( "E" ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	poRitem = ai_poNitem->GetRitem() ;
	CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Se coje la única fórmula que hay
	poFormula = poRitem->GetFormulaByPosition( 0 ) ;
	CHECK_CHLOG_DEBUGF( poFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( poFormula != NULL )
	{
		pcPolish = ai_poPolish->Clone() ;
		CHECK_CHLOG_DEBUGF( pcPolish == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula->SetPolish( pcPolish ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeDBFormulas( void )
{

    m4return_t		iResult ;
	ClLongString	oPolish;


	oPolish.Init( 1000, 500 ) ;

	if( m_poClientSysLoad != NULL )
	{
		iResult = ComputeOneDBFormula( m_poClientSysLoad, &oPolish, M4CL_ITEM_ROOT_LOAD, M4_FALSE, M4_TRUE ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	if( m_poClientSysPersist != NULL )
	{
		iResult = ComputeOneDBFormula( m_poClientSysPersist, &oPolish, M4CL_ITEM_PERSIST_TREE, M4_TRUE, M4_TRUE ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	if( m_poServerSysLoad != NULL )
	{
		iResult = ComputeOneDBFormula( m_poServerSysLoad, &oPolish, M4CL_ITEM_ROOT_LOAD, M4_FALSE, M4_FALSE ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	if( m_poServerSysPersist != NULL )
	{
		iResult = ComputeOneDBFormula( m_poServerSysPersist, &oPolish, M4CL_ITEM_PERSIST_TREE, M4_TRUE, M4_FALSE ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeGraphOrder( void )
{

    m4return_t		iResult ;
    m4uint8_t       iType ;
    m4int16_t       iDegree ;
	m4uint16_t		i, j ;
	m4uint16_t		iNodes ;
	m4uint16_t		iFathers ;
	m4uint16_t		iSons ;
	m4uint16_t		iInside ;
	m4pvoid_t		pvPointer ;
	ClTempNode	    *poFather ;
	ClTempNode	    *poNode ;
    ClTempConnector *poConnector ;
    ClPointerStack  oOrderedStack ;
    ClPointerStack  oTempStack ;
    


    iNodes = m_oNodes.GetLength() ;

// Si no hay nodos al carajo
	CHECK_CHLOG_ERRORF( iNodes == 0, M4_ERROR, M4_MDFAC_NO_NODES, LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;


// Se establece el grado de conexión

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        iSons = poNode->GetNumberOfConnectors() ;

        for( j = 0 ; j < iSons ; j++ )
        {
		    poConnector = poNode->GetConnectorByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

            iType = poConnector->GetType() ;

            if( iType != M4CL_NODE_CONNECTOR_TYPE_FREE && iType != M4CL_NODE_CONNECTOR_TYPE_AUTO )
            {
                poNode->IncrementConnectedDegree() ;
            }
		}
	}


// introducimos en la pila los de grado cero

	oOrderedStack.SetDelta( iNodes ) ;
    oTempStack.SetDelta( iNodes ) ;

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        iDegree = poNode->GetConnectedDegree() ;

        if( iDegree == 0 )
        {
            iResult = oTempStack.AddElement( poNode ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ << i ) ;
        }
	}


// hacemos el algoritmo del orden

    while( oTempStack.GetLength() != 0 )
    {
        iResult = oTempStack.Pop( pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        poNode = ( ClTempNode* ) pvPointer ;
        poNode->SetConnectedDegree( -1 ) ;
    
        iResult = oOrderedStack.AddElement( poNode ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    
        iFathers = poNode->GetNumberOfInvConnectors() ;

        for( i = 0 ; i < iFathers ; i++ )
        {
		    poConnector = poNode->GetInvConnectorByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ << i ) ;

            iType = poConnector->GetType() ;

            if( iType != M4CL_NODE_CONNECTOR_TYPE_FREE )
            {
                poFather = poConnector->GetFatherNode() ;
				CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                poFather->DecrementConnectedDegree() ;

                iDegree = poFather->GetConnectedDegree() ;

                if( iDegree == 0 )
                {
                    iResult = oTempStack.AddElement( poFather ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
                }
            }
        }
    }


// miramos si hay ciclos

    iInside = 0 ;

	for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        iDegree = poNode->GetConnectedDegree() ;

        if( iDegree != -1 )
        {
            iInside++ ;
	    }
	}

    if( iInside > 0 )
    {
// hay que detectar el ciclo
		DUMP_CHLOG_ERRORF( M4_MDFAC_NODE_CYCLE, LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }


    iNodes = oOrderedStack.GetLength() ;

	for( i = iNodes ; i > 0 ; i-- )
	{
		iResult = oOrderedStack.GetElement( i - 1, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
        iResult = poNode->ComputeConnectedNitemsScope() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
        }
	}

    return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeConnectorOrder( void )
{

    m4return_t		iResult ;
    m4int16_t       iDegree ;
	m4uint16_t		i, j ;
	m4uint16_t		iLength ;
	m4uint16_t		iNodes ;
	m4uint16_t		iNitems ;
	m4uint16_t		iFathers ;
	m4uint16_t		iConnectors ;
	m4uint16_t		iInside ;
	m4uint16_t		iSize ;
	m4pchar_t		pcCycleIds ;
	m4pchar_t		pcCycleNames ;
	m4pvoid_t		pvPointer ;
	ClTempNode	    *poNode ;
	ClTempNitem	    *poFatherNitem ;
	ClTempNitem	    *poNitem ;
	ClTempRitem	    *poFatherRitem ;
	ClTempRitem	    *poRitem ;
	ClTempNitemConn	*poFatherNitemConn ;
    ClPointerStack  oTempStack ;
    


	iLength = 0 ;
    iNodes = m_oNodes.GetLength() ;

// Se establece el grado de conexión y se calcula el tamaño

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iNitems = poNode->GetNumberOfNitems() ;

		iLength += iNitems ;

		for( j = 0 ; j < iNitems ; j++ )
        {
		    poNitem = poNode->GetNitemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poRitem = poNitem->GetRitem() ;
			CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iConnectors = poRitem->GetNumberOfConnectors() ;

			poRitem->SetConnectedDegree( iConnectors ) ;
		}
	}


// introducimos en la pila los de grado cero

    oTempStack.SetDelta( iLength ) ;

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iNitems = poNode->GetNumberOfNitems() ;

        for( j = 0 ; j < iNitems ; j++ )
        {
		    poNitem = poNode->GetNitemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poRitem = poNitem->GetRitem() ;
			CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iDegree = poRitem->GetConnectedDegree() ;

			if( iDegree == 0 )
			{
				iResult = oTempStack.AddElement( poRitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ << i << LogCat << " - " << LogCat << j ) ;
			}
		}
	}


// hacemos el algoritmo del orden

    while( oTempStack.GetLength() != 0 )
    {
        iResult = oTempStack.Pop( pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        poRitem = ( ClTempRitem* ) pvPointer ;
        poRitem->SetConnectedDegree( -1 ) ;
    
        iFathers = poRitem->GetNumberOfInvConnectors() ;

        for( i = 0 ; i < iFathers ; i++ )
        {
		    poFatherNitemConn = poRitem->GetInvConnectorByPosition( i ) ;
			CHECK_CHLOG_DEBUGF( poFatherNitemConn == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ << i ) ;

			poFatherNitem = poFatherNitemConn->GetFatherNitem() ;
			CHECK_CHLOG_DEBUGF( poFatherNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poFatherRitem = poFatherNitem->GetRitem() ;
			CHECK_CHLOG_DEBUGF( poFatherRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poFatherRitem->DecrementConnectedDegree() ;

			iDegree = poFatherRitem->GetConnectedDegree() ;

			if( iDegree == 0 )
			{
				iResult = oTempStack.AddElement( poFatherRitem ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}
    }

    
// miramos si hay ciclos

    iInside = 0 ;

	for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iNitems = poNode->GetNumberOfNitems() ;

        for( j = 0 ; j < iNitems ; j++ )
        {
		    poNitem = poNode->GetNitemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poRitem = poNitem->GetRitem() ;
			CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iDegree = poRitem->GetConnectedDegree() ;

			if( iDegree != -1 )
			{
				iInside++ ;
			}
		}
	}

    if( iInside > 0 )
    {
// hay que detectar el ciclo

		iSize = ( M4CL_MAX_NODE_ID + M4CL_MAX_ITEM_ID + 3 ) * iInside ;

		pcCycleIds = new m4char_t[ iSize + 1 ] ;
		CHECK_CHLOG_ERRORF( pcCycleIds == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iSize ) << __LINE__ << __FILE__ ) ;

		pcCycleNames = new m4char_t[ iSize+ 1 ] ;
		CHECK_CHLOG_ERRORF( pcCycleNames == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iSize ) << __LINE__ << __FILE__ ) ;

		*pcCycleIds = '\0' ;
		*pcCycleNames = '\0' ;

		for( i = 0 ; i < iNodes ; i++ )
		{
			iResult = m_oNodes.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNode = ( ClTempNode* ) pvPointer ;
			iNitems = poNode->GetNumberOfNitems() ;

			for( j = 0 ; j < iNitems ; j++ )
			{
				poNitem = poNode->GetNitemByPosition( j ) ;
				CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				poRitem = poNitem->GetRitem() ;
				CHECK_CHLOG_DEBUGF( poRitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iDegree = poRitem->GetConnectedDegree() ;

				if( iDegree != -1 )
				{
					strcat( pcCycleIds, poNode->GetNodeId() ) ;
					strcat( pcCycleIds, "." ) ;
					strcat( pcCycleIds, poNitem->GetNitemId() ) ;
					strcat( pcCycleIds, ", " ) ;

					strcat( pcCycleNames, poNode->GetName() ) ;
					strcat( pcCycleNames, "." ) ;
					strcat( pcCycleNames, poNitem->GetItem()->GetName() ) ;
					strcat( pcCycleNames, ", " ) ;
				}
			}
		}

		pcCycleIds[ strlen( pcCycleIds ) - 2 ] = '\0' ;
		pcCycleNames[ strlen( pcCycleNames ) - 2 ] = '\0' ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_CONNECTOR_ITEM_CYCLE, LOG_CAT_AND( pcCycleIds, pcCycleNames ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	
	return( M4_SUCCESS ) ;
}



m4return_t  ClTempChannel :: ComputeRoots( void )
{

    m4return_t			iResult ;
	m4uint8_t			iIsToCompile ;
	m4uint8_t			iIsRoot ;
	m4uint8_t			iIsQBF ;
	m4uint8_t			iType ;
	m4uint16_t			i, j, k, m ;
	m4uint16_t			iNodes ;
	m4uint16_t			iFathers ;
	m4uint16_t			iConnectors ;
	m4uint16_t			iPlugs ;
	m4pcchar_t			pccItem ;
	m4pvoid_t			pvPointer ;
	ClTempNitem			*poNitem ;
	ClTempRitem			*poRitem ;
	ClTempNode			*poNode ;
	ClTempNode			*poFatherNode ;
	ClTempNode			*poSecondFatherNode ;
	ClTempNitemConn		*poNitemConn ;
	ClTempConnector		*poConnector ;

    
    iNodes = m_oNodes.GetLength() ;

	// Como mucho el número de nodos
    m_oRoots.SetDelta( iNodes ) ;

    for( i = 0 ; i < iNodes ; i++ )
	{
		iResult = m_oNodes.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNode = ( ClTempNode* ) pvPointer ;
		iIsToCompile = poNode->GetIsToCompile() ;

		if( iIsToCompile == 0 )
		{
			continue ;
		}

		iFathers = poNode->GetNumberOfInvConnectors() ;

		iIsRoot = 1 ;
		poFatherNode = NULL ;

		for( j = 0 ; j < iFathers && iIsRoot == 1 ; j++ )
		{
			poConnector = poNode->GetInvConnectorByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iType = poConnector->GetType() ;

			if( iType != M4CL_NODE_CONNECTOR_TYPE_FREE )
			{
				iIsRoot = 0 ;

// control de varios padres
				if( poFatherNode != NULL )
				{
					poSecondFatherNode = poConnector->GetFatherNode() ;

					DUMP_CHLOG_ERRORF( M4_MDFAC_TOO_MANY_PARENT_NODES, LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( poFatherNode->iNode(), poFatherNode->nNode() ) << LOG_CAT_AND( poSecondFatherNode->iNode(), poSecondFatherNode->nNode() ) ) ;
					return( M4_ERROR ) ;
				}
				else
				{
					poFatherNode = poConnector->GetFatherNode() ;
				}
			}
		}

		if( iIsRoot == 1 && iFathers != 0 )
		{
// vamos todavía a ver si estan conectados los items de base de datos
			for( k = 0 ; k < 10 && iIsRoot == 1 ; k++ )
			{
				poNitem = poNode->GetNitemById( g_apcRootItems[ k ], M4_FALSE ) ;

				if( poNitem != NULL )
				{
					poRitem = poNitem->GetRitem() ;
					poNitemConn = poRitem->GetInverseConnector() ;

					if( poNitemConn != NULL )
					{
						poConnector = poNitemConn->GetConnector() ;
						poFatherNode = poConnector->GetFatherNode() ;

						if( poFatherNode != poNode )
						{
							if( k == 0 )
							{
// si es rootload hay que ver que esté conectada a la prg
								iConnectors = poRitem->GetNumberOfConnectors() ;

								for( m = 0 ; m < iConnectors && iIsRoot == 1 ; m++ )
								{
									poNitemConn = poRitem->GetConnectorByPosition( m ) ;
									poNitem = poNitemConn->GetSonNitem() ;
									pccItem = poNitem->GetNitemId() ;

									if( strcmp( pccItem, M4CL_ITEM_LOAD_PRG ) == 0 )
									{
										iIsRoot = 0 ;
									}
								}
							}
							else
							{
								iIsRoot = 0 ;
							}
						}
					}
				}
			}
		}

		if( iIsRoot == 1 )
		{
			poNode->SetIsRealRoot( 1 ) ;
			iResult = m_oRoots.AddElement( poNode ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			// Si no es QBF se cuenta para el SYS_LOAD y SYS_PERSIST
			iIsQBF = poNode->GetIsQBFFilter() ;

			if( iIsQBF == 0 )
			{
				m_iNoQBFRoots++ ;
			}
		}
		else
		{
// Chequeo de los carriles a nodos no root
			iPlugs = poNode->GetNumberOfPlugs() ;

			if( iPlugs > 0 )
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_BAD_LANE_NODE, LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << iPlugs ) ;
			}
		}
	}


    m_oRoots.Sort( M4ClCompareObjectsById ) ;


    return( M4_SUCCESS ) ;
}




void	ClTempChannel :: ClearFormulas( void )
{

	m4return_t			iResult ;
	m4uint16_t			i, j ;
	m4uint16_t			iNitems ;
	m4uint16_t			iNitemFormulas ;
	m4pvoid_t	        pvPointer ;
	ClTempNitem			*poNitem ;
	ClTempRitem			*poRitem ;
	ClTempNitemFormula	*poNitemFormula ;


    iNitems = m_oNitems.GetLength() ;

    for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;

		if( iResult != M4_SUCCESS || pvPointer == NULL )
		{
			continue ;
		}

		poNitem = ( ClTempNitem* ) pvPointer ;
		poRitem = poNitem->GetRitem() ;


		iNitemFormulas = poRitem->GetNumberOfFormulas() ;

		for( j = 0 ; j < iNitemFormulas ; j++ )
		{
			poNitemFormula = poRitem->GetFormulaByPosition( j ) ;

			if( poNitemFormula == NULL )
			{
				continue ;
			}

			poNitemFormula->Clear() ;
		}
	}
}



m4return_t  ClTempChannel :: _Print( FILE* ai_pfFile, m4uint8_t ai_iIndent )
{

 	m4return_t			iResult ;
	m4uint16_t			i, j ;
	m4uint16_t			iNitems ;
	m4uint16_t			iNitemFormulas ;
    m4date_t			dDate ;
    m4char_t			acDate[ M4_SECOND_END + 1 ] ;
	m4pvoid_t	        pvPointer ;
	ClTempNitem			*poNitem ;
	ClTempRitem			*poRitem ;
	ClTempNitemFormula	*poNitemFormula ;

    m4char_t			acIndent[ 10 ] ;


    for( i = 0 ; i < ai_iIndent ; i++ )
    {
        acIndent[ i ] = '\t' ;
    }

    acIndent[ i ] = '\0' ;


    fprintf( ai_pfFile, acIndent ) ;
    fprintf( ai_pfFile, "============ M4Object ====================================================\n" ) ;


    fprintf( ai_pfFile, acIndent ) ;
    fprintf( ai_pfFile, "M4Object: <%s>\n", iM4Obj() ) ;


    iNitems = m_oNitems.GetLength() ;

    for( i = 0 ; i < iNitems ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;

		if( iResult != M4_SUCCESS || pvPointer == NULL )
		{
			continue ;
		}

		poNitem = ( ClTempNitem* ) pvPointer ;

		if( poNitem->GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 0 )
		{
			continue ;
		}

		poRitem = poNitem->GetRitem() ;

		iNitemFormulas = poRitem->GetNumberOfFormulas() ;

		for( j = 0 ; j < iNitemFormulas ; j++ )
		{
			poNitemFormula = poRitem->GetFormulaByPosition( j ) ;

			if( poNitemFormula == NULL || poNitemFormula->GetIsToCompile() == 0 )
			{
				continue ;
			}

			if( poNitemFormula->GetCreationType() == M4_MDFAC_RULE_CREATION_SYSTEM_METARULE || poNitemFormula->GetCreationType() == M4_MDFAC_RULE_CREATION_TOTAL )
			{
			    fprintf( ai_pfFile, "\n============ Rule ============================================\n" ) ;

				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "Node: <%s>\n", poNitem->GetNodeId() ) ;

				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "Item: <%s>\n", poNitem->GetNitemId() ) ;

				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "Rule: <%s>\n", poNitemFormula->GetFormulaId() + 2 ) ;

				dDate = poNitemFormula->GetStartDate() ;
				ClDateToString( dDate, acDate, M4_SECOND_END + 1 ) ;
				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "<%s>\n", acDate ) ;

				dDate = poNitemFormula->GetEndDate() ;
				ClDateToString( dDate, acDate, M4_SECOND_END + 1 ) ;
				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "<%s>\n\n", acDate ) ;

				fprintf( ai_pfFile, acIndent ) ;
				fprintf( ai_pfFile, "%s\n", poNitemFormula->GetPolish() ) ;

			    fprintf( ai_pfFile, "============ Rule End ========================================\n" ) ;
			}
		}
	}


	fprintf( ai_pfFile, acIndent ) ;
    fprintf( ai_pfFile, "============ End M4Object ================================================\n" ) ;

	return( M4_SUCCESS ) ;
}


m4return_t  ClTempChannel :: Print( m4uint8_t ai_iSystemDebug, m4pcchar_t ai_pccPath )
{

	m4return_t	iResult ;
    m4char_t	acFile[ M4CH_MAX_FILE + 1 ] ;
    FILE        *pfFile ;


	if( ai_iSystemDebug < 3 )
	{
		return( M4_SUCCESS ) ;
	}

	if( m_iPrint == 0 )
	{
		return( M4_SUCCESS ) ;
	}
	
    
    if( ai_pccPath != 0 && *ai_pccPath != '\0' )
    {
        sprintf( acFile, "%s%s%s&%d.tt", ai_pccPath, M4_BACK_SLASH, iM4Obj(), GetCsType() ) ;
    }
    else
    {
        sprintf( acFile, "%s&%d.tt", iM4Obj(), GetCsType() ) ;
    }

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

    pfFile = M4Fopen( acFile, M4UniWrite, eEncoding ) ;

    if( pfFile != NULL )
    {
        iResult = _Print( pfFile ) ;

        if( iResult != M4_SUCCESS )
        {
            fprintf( pfFile, "Error Printing Metachannel\n" ) ;
        }
        fclose( pfFile)  ;
    }

    return( iResult ) ;

}


