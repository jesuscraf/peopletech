
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnitem.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary item of a node
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facres.hpp"

#include "cltnitem.hpp"
#include "cltnicon.hpp"
#include "cltiform.hpp"
#include "cltchann.hpp"
#include "cltnode.hpp"
#include "cltconn.hpp"
#include "clttotal.hpp"
#include "clttotaltag.hpp"
#include "clttag.hpp"
#include "cltti.hpp"
#include "cltarg.hpp"
#include "chlog.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "m4objreg.hpp"
#include "sliarray.hpp"
#include "cllstr.hpp"




// Array de reduccion de tipos de items
// Si tiene celda es un 1 y si no un 0
// El array empieza en 1, que es method y acaba en concept
m4uint8_t   g_aiReducedItemTypesArray[ M4CL_ITEM_TYPE_CONCEPT ] =
{
	0, 1, 1, 1
} ;


// Array de reduccion de tipos CS de items
// Si no es válido un -1, y el resto de 0 a 3 (front, back, both, mapped)
// El array empieza en 1, que es front y acaba en mapped
m4int8_t   g_aiReducedItemCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] =
{
	0, 1, 2, -1, -1, -1, 3
} ;


// Array de reduccion de tipos CS de nodos
// Si no es válido un -1, y el resto de 0 a 3 (front, back, both, mapped)
// El array empieza en 1, que es front y acaba en mapped
m4int8_t   g_aiReducedNodeCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] =
{
	0, 1, 2, -1, -1, -1, 3
} ;


// Array de reduccion de tipos CS de conectores
// Si no es válido un -1, y el resto de 0 a 3 (front, back, both, mapped)
// El array empieza en 1, que es front y acaba en mapped
m4int8_t   g_aiReducedConnCsTypesArray[ M4CL_CSTYPE_MAX_NUMBER ] =
{
	0, 1, 2, -1, -1, -1, 3
} ;


// array de validación de items conectados por scope
m4uint8_t   g_aiValidScopeArray[ M4CL_NODE_CONNECTOR_MAX_TYPE ][ M4CL_ITEM_MAX_SCOPE ][ M4CL_ITEM_MAX_SCOPE ] =
{
// RB
    {
//						hijo nodo	hijo bloque		hijo registro
/*padre nodo*/		{ 1, 1, 1 },
/*padre bloque*/	{ 0, 1, 1 },
/*padre registro*/	{ 0, 1, 1 }
    },

// BB
    {
//						hijo nodo	hijo bloque		hijo registro
/*padre nodo*/		{ 1, 1, 1 },
/*padre bloque*/	{ 0, 1, 1 },
/*padre registro*/	{ 0, 0, 0 }
    },

// FREE
    {
//						hijo nodo	hijo bloque		hijo registro
/*padre nodo*/		{ 1, 1, 1 },
/*padre bloque*/	{ 0, 1, 1 },	// debería ser 0,0,0
/*padre registro*/	{ 0, 0, 0 }
    },

// AUTO
    {
//						hijo nodo	hijo bloque		hijo registro
/*padre nodo*/		{ 1, 1, 1 },
/*padre bloque*/	{ 0, 1, 1 },
/*padre registro*/	{ 0, 0, 1 }
    }

} ;


// array de validación de items método conectados por cs type
m4uint8_t	g_aiValidMethodCsTypesArray [ 4 ][ 4 ][ 4 ] =
{
// Valid for FRONT connectors
	{
// Son method cs type:	    		  F  Ba Bo M
/* Father method cs type FRONT	*/	{ 1, 0, 1, 1 },
/* Father method cs type BACK	*/	{ 0, 0, 0, 0 },
/* Father method cs type BOTH	*/	{ 1, 0, 1, 1 },
/* Father method cs type MAPPED */	{ 1, 0, 1, 0 }
	},
// Valid for BACK connectors
	{
// Son method cs type:	    		  F  Ba Bo M
/* Father method cs type FRONT	*/	{ 0, 0, 0, 0 },
/* Father method cs type BACK	*/	{ 0, 1, 1, 1 },
/* Father method cs type BOTH	*/	{ 0, 1, 1, 1 },
/* Father method cs type MAPPED */	{ 0, 1, 1, 1 } // Obligo a que cuando se conecta un metodo mapped con otro mapped, el conector tiene que estar en el lado del server
	},
// Valid for BOTH connectors
	{
// Son method cs type:	    		  F  Ba Bo M
/* Father method cs type FRONT	*/	{ 0, 0, 0, 0 },
/* Father method cs type BACK	*/	{ 0, 0, 0, 0 },
/* Father method cs type BOTH	*/	{ 0, 0, 1, 1 },
/* Father method cs type MAPPED */	{ 0, 0, 1, 0 }
	},
// Valid for INSITE connectors
	{
// Son method cs type:	    		  F  Ba Bo M
/* Father method cs type FRONT	*/	{ 0, 0, 0, 0 },
/* Father method cs type BACK	*/	{ 0, 0, 0, 0 },
/* Father method cs type BOTH	*/	{ 0, 0, 1, 1 },
/* Father method cs type MAPPED */	{ 0, 0, 1, 0 }
	}
} ;



// array de validación de items método conectados por cs type
m4uint8_t	g_aiValidDataCsTypesArray[ 4 ][ 4 ][ 4 ] =
{
// Valid for FRONT connectors
	{
// Son item cs type:	    		  F  Ba Bo M
/* Father item cs type FRONT  */	{ 1, 0, 1, 1 },
/* Father item cs type BACK	  */	{ 0, 0, 0, 0 },
/* Father item cs type BOTH	  */	{ 1, 0, 1, 0 },
/* Father item cs type MAPPED */	{ 1, 0, 0, 1 }
	},
// Valid for BACK connectors
	{
// Son item cs type:	    		  F  Ba Bo M
/* Father item cs type FRONT  */	{ 0, 0, 0, 0 },
/* Father item cs type BACK	  */	{ 0, 1, 1, 1 },
/* Father item cs type BOTH	  */	{ 0, 1, 1, 1 },
/* Father item cs type MAPPED */	{ 0, 1, 1, 1 }
	},
// Valid for BOTH connectors
	{
// Son item cs type:	    		  F  Ba Bo M
/* Father item cs type FRONT  */	{ 0, 0, 0, 0 },
/* Father item cs type BACK	  */	{ 0, 0, 0, 0 },
/* Father item cs type BOTH	  */	{ 0, 0, 1, 1 },
/* Father item cs type MAPPED */	{ 0, 0, 1, 1 }
	},
// Valid for INSITE connectors
	{
// Son item cs type:	    		  F  Ba Bo M
/* Father item cs type FRONT  */	{ 0, 0, 0, 0 },
/* Father item cs type BACK	  */	{ 0, 0, 0, 0 },
/* Father item cs type BOTH	  */	{ 0, 0, 1, 1 },
/* Father item cs type MAPPED */	{ 0, 0, 1, 1 }
	}
} ;



int	M4ClCompareNitemConnsByOrder( m4pcvoid_t ai_pcvNitemConn1, m4pcvoid_t ai_pcvNitemConn2 )
{
	return( ( *( ClTempNitemConn** )ai_pcvNitemConn1 )->GetCallOrder() - ( *( ClTempNitemConn** )ai_pcvNitemConn2 )->GetCallOrder() ) ;
}


int	M4ClCompareRowsByMetaruleAndId( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	ClSliceRow	*poRow1 = *( ClSliceRow** ) ai_pcvRow1 ;
	ClSliceRow	*poRow2 = *( ClSliceRow** ) ai_pcvRow2 ;

	ClTempNitemFormula	*poFormula1 = ( ClTempNitemFormula* ) poRow1->GetLeader() ;
	ClTempNitemFormula	*poFormula2 = ( ClTempNitemFormula* ) poRow2->GetLeader() ;


	// Si son metareglas van primero
	m4uint8_t	iIsMetarule1 = poFormula1->GetIsMetarule() ;
	m4uint8_t	iIsMetarule2 = poFormula2->GetIsMetarule() ;

	if( iIsMetarule1 != iIsMetarule2 )
	{
		return( iIsMetarule2 - iIsMetarule1 ) ;
	}

	// Si las dos son metareglas son iguales y no importa el identificador
	if( iIsMetarule1 == 1 )
	{
		return( 0 ) ;
	}

	return( M4MdFacStrcmp( poFormula1->GetFormulaId(), poFormula2->GetFormulaId() ) ) ;
}



int	M4ClCompareRowsByItemUsedAndDimId( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	ClSliceRow	*poRow1 = *( ClSliceRow** ) ai_pcvRow1 ;
	ClSliceRow	*poRow2 = *( ClSliceRow** ) ai_pcvRow2 ;

	ClTempTotalTag	*poTotalTag1 = ( ClTempTotalTag* ) poRow1->GetLeader() ;
	ClTempTotalTag	*poTotalTag2 = ( ClTempTotalTag* ) poRow2->GetLeader() ;


	int	iResult = M4MdFacStrcmp( poTotalTag1->GetItemUsedId(), poTotalTag2->GetItemUsedId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( poTotalTag1->GetDimItemId(), poTotalTag2->GetDimItemId() ) ) ;
}



int	M4ClCompareRowsByRuleAndDimId( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	ClSliceRow	*poRow1 = *( ClSliceRow** ) ai_pcvRow1 ;
	ClSliceRow	*poRow2 = *( ClSliceRow** ) ai_pcvRow2 ;

	ClTempTag	*poTag1 = ( ClTempTag* ) poRow1->GetLeader() ;
	ClTempTag	*poTag2 = ( ClTempTag* ) poRow2->GetLeader() ;


	int	iResult = M4MdFacStrcmp( poTag1->GetRuleId(), poTag2->GetRuleId() ) ;

    if( iResult != 0 )
    {
		return( iResult ) ;
    }

	return( M4MdFacStrcmp( poTag1->GetDimItemId(), poTag2->GetDimItemId() ) ) ;
}












// nitems ================================================

ClTempRitem :: ClTempRitem( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iName = 0 ;
    m_iConditionantsStart = 0 ;
    m_iAssignedStart = 0 ;
    m_iExecutedStart = 0 ;
	m_iFormIdsStart = 0 ;
    m_iFormulasStart = 0 ;

    m_iScope = 0 ;

	m_pccName = NULL ;

	m_iFormulaNumber = 0 ;
    m_iSliceNumber = 0 ;

	m_ppoFormulaArray = NULL ;
	m_pdDatesArray = NULL ;

    m_iConditionantDegree = 0 ;
    m_iAfterMethods = 0 ;

	m_iConnectedDegree = 0 ;

	m_bIsDatabase = M4_FALSE ;

	m_iConnectorsToCompile = 0 ;

	m_oConnectors.SetDelta( 0 ) ;
	m_oInvConnectors.SetDelta( 0 ) ;
	m_oFormulas.SetDelta( 0 ) ;
	m_oTotals.SetDelta( 0 ) ;
	m_oTotalTags.SetDelta( 0 ) ;
	m_oTags.SetDelta( 0 ) ;
}



ClTempRitem :: ~ClTempRitem( void )
{
    Delete() ;
}




m4return_t	ClTempRitem :: Delete( void )
{

	m4return_t			iResult ;
	m4uint8_t			iIsOwn ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pvoid_t			pvPointer ;
    ClTempNitemConn		*poNitemConn ;
    ClTempNitemFormula	*poNitemFormula ;

    
    iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitemConn = ( ClTempNitemConn* ) pvPointer ;

		iIsOwn = poNitemConn->GetIsOwn() ;

		if( iIsOwn == 1 )
		{
			delete( poNitemConn ) ;
		}
	}


    iLength = m_oFormulas.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFormulas.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitemFormula = ( ClTempNitemFormula* ) pvPointer ;
        delete( poNitemFormula ) ;
	}

	
	if( m_ppoFormulaArray != NULL )
	{
		delete [] m_ppoFormulaArray ;
		m_ppoFormulaArray = NULL ;
	}

	if( m_pdDatesArray != NULL )
	{
		delete [] m_pdDatesArray ;
		m_pdDatesArray = NULL ;
	}
	

    return( M4_SUCCESS ) ;
}


// Funciones de escritura





m4return_t	ClTempRitem :: AddConnector( ClTempNitemConn *ai_poConnector )
{

	m4return_t	iResult ;
    m4uint8_t   iPrecedence ;
    m4uint8_t   iType ;
    ClTempNitem *poNitem ;
    ClTempItem  *poItem ;


	CHECK_CHLOG_DEBUGF( ai_poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iPrecedence = ai_poConnector->GetPrecedence() ;

    if( iPrecedence == M4CL_ITEM_CONNECTOR_AFTER )
    {
        poNitem = ai_poConnector->GetSonNitem() ;
		CHECK_CHLOG_DEBUGF( poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
    
        poItem = poNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

        iType = poItem->GetType() ;

        if( iType == M4CL_ITEM_TYPE_METHOD || iType == M4CL_ITEM_TYPE_CONCEPT )
        {
            m_iAfterMethods++ ;
        }
    }

    iResult = m_oConnectors.AddElement( ai_poConnector ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: AddInvConnector( ClTempNitemConn *ai_poConnector )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poConnector == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oInvConnectors.AddElement( ai_poConnector ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: AddConditionant( ClTempNitem *ai_poConditionant )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poConditionant == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oConditionants.AddElementIfNotIn( ai_poConditionant ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddAssigned( ClTempNitem *ai_poAssigned )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poAssigned == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oAssigned.AddElementIfNotIn( ai_poAssigned ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddExecuted( ClTempNitem *ai_poExecuted )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poExecuted == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oExecuted.AddElementIfNotIn( ai_poExecuted ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddFormula( ClTempNitemFormula *ai_poFormula )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oFormulas.AddElement( ai_poFormula ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Si alguna de sus fórmulas es de base de datos ya es un item de base de datos
	if( ai_poFormula->GetIsDatabase() == M4_TRUE )
	{
		m_bIsDatabase = M4_TRUE ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddTotal( ClTempTotal *ai_poTotal )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTotal == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTotals.AddElement( ai_poTotal ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddTotalTag( ClTempTotalTag *ai_poTotalTag )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTotalTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTotalTags.AddElement( ai_poTotalTag ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddTag( ClTempTag *ai_poTag )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poTag == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oTags.AddElement( ai_poTag ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: AddDependent( ClTempNitem *ai_poDependent )
{

	m4return_t	        iResult ;


	CHECK_CHLOG_DEBUGF( ai_poDependent == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oDependents.AddElement( ai_poDependent ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



ClTempNitemConn*   ClTempRitem :: GetInverseConnector( void ) const
{

	m4return_t	iResult ;
	m4pvoid_t	pvPointer ;


	if( m_oInvConnectors.GetLength() > 0 )
	{
		iResult = m_oInvConnectors.GetElement( 0, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		return( ( ClTempNitemConn* ) pvPointer ) ;
	}

	return( NULL ) ;
}


m4return_t  ClTempRitem :: GetFormulaOrderById( m4pcchar_t ai_pccRuleId, ClTempNitem *ai_poNitem, ClTempNode *ai_poNode, m4pcchar_t ai_pccThisFormula, ClTempNitem *ai_poThisNitem, m4uint8_t ai_iInheritance, m4uint16_t &ao_riOrder )
{

    m4return_t			    iResult ;
    m4uint8_t               i ;
    m4uint8_t               iFound ;
	m4uint8_t				iTruncated = 0 ;
    m4uint16_t				iLength ;
	m4pcchar_t				pccFormulaId ;
    m4char_t                acFormula[ M4CL_MAX_NITEM_FORMULA_ID + 2 ] ;
	m4pvoid_t               pvPointer ;
	ClTempNitemFormula      *poFormula ;


	CHECK_CHLOG_DEBUGF( ai_pccRuleId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	// Si no tiene nombre es metaregla
	if( *ai_pccRuleId == '\0' )
	{
		ao_riOrder = 0 ;
		return( M4_SUCCESS ) ;
	}


    iLength = strlen( ai_pccRuleId ) ;

    if( iLength > M4CL_MAX_RULE_ID )
    {
        iLength = M4CL_MAX_RULE_ID ;
		iTruncated = 1 ;
    }

    acFormula[ 0 ] = m4uint8_t( iLength ) + 2 ;
    acFormula[ 1 ] = ai_iInheritance ;
    acFormula[ 2 ] = ':' ;
    memcpy( acFormula + 3, ai_pccRuleId, sizeof( m4char_t ) * iLength ) ;
    acFormula[ iLength + 3 ] = '\0' ;
	strupr( acFormula + 3 ) ;

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_DATA_TRUNCATED, ai_pccRuleId << M4CL_MAX_RULE_ID << acFormula + 2 ) ;
	}



	iLength = m_oFormIds.GetLength() ;

    iFound = 0 ;
	for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	{
		iResult = m_oFormIds.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula = ( ClTempNitemFormula* ) pvPointer ;

        pccFormulaId = poFormula->GetFormulaId() - 1 ;

		// Si no tienen la misma longitud no son iguales
		if( pccFormulaId[ 0 ] != acFormula[ 0 ] )
		{
			continue ;
		}

		// Si la herencia es menor no la encontramos
		if( pccFormulaId[ 1 ] < acFormula[ 1 ] )
		{
			continue ;
		}

		if( strcmp( pccFormulaId + 3, acFormula + 3 ) == 0 )
		{
			iFound = 1 ;
		}
	}


// falta pasar el Nitem y el InstallNitem
	CHECK_CHLOG_ERRORF( iFound == 0, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_FORMULA, ai_pccRuleId << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNode->iNode(), ai_poNode->nNode() ) << LOG_CAT_AND( ai_poNode->iM4Obj(), ai_poNode->nM4Obj() ) << ai_pccThisFormula + 3 << LOG_CAT_AND( ai_poThisNitem->iItem(), ai_poThisNitem->nItem() ) << LOG_CAT_AND( ai_poThisNitem->iNode(), ai_poThisNitem->nNode() ) << LOG_CAT_AND( ai_poThisNitem->iM4Obj(), ai_poThisNitem->nM4Obj() ) ) ;

    ao_riOrder = poFormula->GetOrder() ;

    return( M4_SUCCESS ) ;
}


ClTempNitemConn*   ClTempRitem :: GetConnectorBySonNitem( ClTempNitem *ai_poSonNitem )
{

    m4return_t		iResult ;
	m4uint16_t		iFound ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pvoid_t		pvPointer ;
    ClTempNitem		*poSonNitem ;
    ClTempNitemConn	*poNitenConn ;


	CHECK_CHLOG_DEBUGF( ai_poSonNitem == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iLength = m_oConnectors.GetLength() ;

    iFound = 0 ;
	for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;

        poSonNitem = poNitenConn->GetSonNitem() ;
		CHECK_CHLOG_DEBUGF( poSonNitem == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( poSonNitem == ai_poSonNitem )
		{
            iFound = 1 ;
        }
	}

    if( iFound == 0 )
    {
        return( NULL ) ;
    }

	return( poNitenConn ) ;
}


ClTempNitemConn*   ClTempRitem :: GetConnectorByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempNitemConn	*poResult ;


	iResult = m_oConnectors.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitemConn* ) pvElement ;

	return( poResult ) ;
}


ClTempNitemConn*   ClTempRitem :: GetInvConnectorByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempNitemConn	*poResult ;


	iResult = m_oInvConnectors.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitemConn* ) pvElement ;

	return( poResult ) ;
}


ClTempNitem*    ClTempRitem :: GetConditionantByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempNitem     *poResult ;


	iResult = m_oConditionants.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}


ClTempNitemFormula*    ClTempRitem :: GetFormulaByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			pvElement ;
    ClTempNitemFormula  *poResult ;


	iResult = m_oFormulas.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitemFormula* ) pvElement ;

	return( poResult ) ;
}


ClTempTotal*  ClTempRitem :: GetTotalByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTotal		*poResult ;


	iResult = m_oTotals.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTotal* ) pvElement ;

	return( poResult ) ;
}


ClTempTotalTag*  ClTempRitem :: GetTotalTagByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTotalTag	*poResult ;


	iResult = m_oTotalTags.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTotalTag* ) pvElement ;

	return( poResult ) ;
}


ClTempTag*  ClTempRitem :: GetTagByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTag		*poResult ;


	iResult = m_oTags.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTag* ) pvElement ;

	return( poResult ) ;
}



ClTempNitem*    ClTempRitem :: GetDependentByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempNitem     *poResult ;


	iResult = m_oDependents.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempNitem* ) pvElement ;

	return( poResult ) ;
}



// serialización

m4return_t	ClTempRitem :: PreGetSize( void )
{

	m4return_t			iResult ;
	m4uint8_t			iIsToCompile ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pvoid_t			pvPointer ;
	ClTempNitemFormula	*poFormula ;
	ClTempNitemConn		*poNitenConn ;


	iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;
		iIsToCompile = poNitenConn->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNitenConn->PreGetSize() ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


	iLength = m_oFormulas.GetLength() ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFormulas.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula = ( ClTempNitemFormula* ) pvPointer ;

		iIsToCompile = poFormula->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poFormula->PreGetSize() ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}



    return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: GetSize( m4uint32_t &ao_iSize, m4uint8_t ai_iType, ClMdStatistics *ai_poStatistics )
{

    m4return_t			iResult ;
	m4uint8_t			iIsToCompile ;
	m4uint16_t			i ;
    m4uint16_t			iLength ;
	m4pvoid_t			pvPointer ;
	ClTempNitemFormula	*poFormula ;
	ClTempNitemConn		*poNitenConn ;


// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_RITEM_FIXED_SIZE ;


// Variables ==================================================================

//	iLength = m_oNitemConnectors.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iConnectorsToCompile * M4_SIZE_OF_INT32 ;    // array de handles


	iLength = m_oConditionants.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iConditionantsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // Número de dependencias redondeado a 4

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_RITEM_CONDITIONANT_SIZE ;
	}
	else
	{
		m_iConditionantsStart = 0 ;
	}

	iLength = m_oAssigned.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iAssignedStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // Número de asignados redondeado a 4

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_RITEM_ASSIGNED_SIZE ;
	}
	else
	{
		m_iAssignedStart = 0 ;
	}
    
	iLength = m_oExecuted.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iExecutedStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // Número de ejecutados redondeado a 4

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_RITEM_EXECUTED_SIZE ;
	}
	else
	{
		m_iExecutedStart = 0 ;
	}

	iLength = m_oFormIds.GetLength() ;

	if( iLength != 0 )
	{
		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		m_iFormIdsStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;  // Número de form ids redondeado a 4

		ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
		ao_iSize += iLength * M4CL_MCR_RITEM_FORM_IDS_SIZE ;
	}
	else
	{
		m_iFormIdsStart = 0 ;
	}
    
	if( m_iSliceNumber != 0 && m_iFormulaNumber != 0 )
	{
		ao_iSize = M4_ROUND_TO_8( ao_iSize ) ;
		m_iFormulasStart = ao_iSize ;
		ao_iSize += M4_SIZE_OF_INT32 ;	// para el número de slices: medio nop
		ao_iSize += M4_SIZE_OF_INT32 ;	// para el número de fórmulas: medio nop

		ao_iSize += m_iSliceNumber * M4_ROUND_TO_8( 2 * M4_SIZE_OF_DATE + m_iFormulaNumber * M4_SIZE_OF_INT32 ) ;
	}
	else
	{
		m_iFormulasStart = 0 ;
	}


	if( ai_poStatistics != NULL )
	{
		switch( ai_iType )
		{
			case M4CL_ITEM_TYPE_METHOD :
				ai_poStatistics->AddValue( M4MDRT_STT_RITEM_METHOD, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_PROPERTY :
				ai_poStatistics->AddValue( M4MDRT_STT_RITEM_PROPERTY, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_FIELD :
				ai_poStatistics->AddValue( M4MDRT_STT_RITEM_FIELD, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_CONCEPT :
				ai_poStatistics->AddValue( M4MDRT_STT_RITEM_CONCEPT, ao_iSize - m_iHandle ) ;
				break ;

			default :
				break ;
		}
	}

// Componentes ================================================================

	iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;
		iIsToCompile = poNitenConn->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNitenConn->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

    
    iLength = m_oFormulas.GetLength() ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFormulas.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula = ( ClTempNitemFormula* ) pvPointer ;

		iIsToCompile = poFormula->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poFormula->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: Compile( ClTempChannel *ai_poChannel, ClTempNitem *ai_poNitem, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

	m4return_t			iResult ;
	m4uint8_t			iIsToCompile ;
	m4uint8_t			iCppType ;
	m4uint16_t			i, j ;
	m4uint16_t			iLength ;
    m4uint32_t			iOffset ;
    m4uint32_t			iHandle ;
    m4uint16_t			iPosition ;
	m4date_t			dSliceStartDate ;
	m4date_t			dSliceEndDate ;
	m4date_t			dTimeUnit ;
    m4pcchar_t			pccSliceItemId ;
    m4pcchar_t			pccAuxItemId ;
    m4pcchar_t			pccAuxAuxItemId ;
    m4pcchar_t			pccNodeId ;
    m4pvoid_t			pvPointer ;
    ClTempNode			*poNode ;
    ClTempItem			*poItem ;
    ClTempItem			*poAuxItem ;
    ClTempItem			*poAuxAuxItem ;
    ClTempNitem			*poNitem ;
    ClTempNitem			*poAuxNitem ;
    ClTempNitem			*poAuxAuxNitem ;
	ClTempNitemFormula	*poFormula ;
	ClTempNitemConn		*poNitenConn ;
	ClTempNitemConn		*poInverseConnector ;
	ClVMBaseEnv			*poEnvironment ;


	poItem = ai_poNitem->GetItem() ;
	poNode = ai_poNitem->GetNode() ;

	poEnvironment = ai_poChannel->GetFactory()->_GetEnvironment() ;

	if( poEnvironment != NULL )
	{
		dTimeUnit = poEnvironment->GetDateTimeUnit() ;
	}
	else
	{
		dTimeUnit = M4_ONE_DAY ;
	}

    
    iOffset = m_iHandle ;


	M4ClPackInt32( ai_pcBuffer, iOffset, m_iName + ai_iBufferSize + ai_iPoolSize ) ;

	poInverseConnector = GetInverseConnector() ;

    if( poInverseConnector != NULL && poInverseConnector->GetIsToCompile() == 1 )
    {
        iHandle = poInverseConnector->GetHandle() ;
    }
	else
	{
		iHandle = 0 ;
	}
    M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;


    pccSliceItemId = poItem->GetSliceItemId() ;

    if( pccSliceItemId != NULL && *pccSliceItemId != '\0' )
    {
		if( poNode != NULL )
		{
			poNitem = poNode->GetNitemById( pccSliceItemId, M4_FALSE ) ;
		}
		else
		{
			poNitem = NULL ;
		}

        if( poNitem != NULL )
        {
			if( poNitem != ai_poNitem )
			{
				iHandle = poNitem->GetHandle() ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_AUTO_SLICE_ITEM, LOG_CAT_AND( poNitem->iItem(), poNitem->nItem() ) << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
				iHandle = 0 ;
			}
        }
        else
        {
			if( poNode != NULL )
			{
				pccNodeId = poNode->GetNodeId() ;
			}
			else
			{
				pccNodeId = "" ;
			}

			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccSliceItemId << pccNodeId << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) << "Slice Item" ) ;
		    return( M4_ERROR ) ;
        }
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    pccAuxItemId = poItem->GetAuxItemId() ;

    if( pccAuxItemId != NULL && *pccAuxItemId != '\0' )
    {
		if( poNode != NULL )
		{
			poAuxNitem = poNode->GetNitemById( pccAuxItemId, M4_FALSE ) ;
		}
		else
		{
			poAuxNitem = NULL ;
		}

        if( poAuxNitem != NULL )
        {
			if( poAuxNitem != ai_poNitem )
			{
				iHandle = poAuxNitem->GetHandle() ;
			}
			else
			{
				DUMP_CHLOG_WARNINGF( M4_MDFAC_AUTO_AUX_ITEM, LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;
				iHandle = 0 ;
			}
        }
        else
        {
			if( poNode != NULL )
			{
				pccNodeId = poNode->GetNodeId() ;
			}
			else
			{
				pccNodeId = "" ;
			}

			DUMP_CHLOG_WARNINGF( M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccAuxItemId << pccNodeId << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) << "Auxiliar Item" ) ;

// para el bueno de Tom 
//		    return( M4_ERROR ) ;
			iHandle = 0 ;
        }
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    pccAuxItemId = poItem->GetAuxItemProratId() ;

    if( pccAuxItemId != NULL && *pccAuxItemId != '\0' )
    {
		if( poNode != NULL )
		{
			poAuxNitem = poNode->GetNitemById( pccAuxItemId, M4_FALSE ) ;
		}
		else
		{
			poAuxNitem = NULL ;
		}
		CHECK_CHLOG_ERRORF( poAuxNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccAuxItemId << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) << "Auxiliar Prorating Item" ) ;
		CHECK_CHLOG_ERRORF( poAuxNitem == ai_poNitem, M4_ERROR, M4_MDFAC_AUTO_AUX_ITEM, LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;

		// Hay que comprobar que existen los items auxiliares y que sean de tipo fecha
		poAuxItem = poAuxNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poAuxItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iCppType = poAuxItem->GetCppType() ;
		CHECK_CHLOG_ERRORF( iCppType != M4CL_CPP_TYPE_DATE, M4_ERROR, M4_MDFAC_AUX_ITEM_PRORAT_BAD_TYPE, LOG_CAT_AND( poAuxNitem->iItem(), poAuxNitem->nItem() ) << M4ClCppType( iCppType ) << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;

		pccAuxAuxItemId = poAuxItem->GetAuxItemId() ;

		if( pccAuxAuxItemId != NULL && *pccAuxAuxItemId != '\0' )
		{
			if( poNode != NULL )
			{
				poAuxAuxNitem = poNode->GetNitemById( pccAuxAuxItemId, M4_FALSE ) ;
				CHECK_CHLOG_ERRORF( poAuxAuxNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccAuxAuxItemId << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) << "Auxiliar Prorating Item" ) ;
			}
			else
			{
				poAuxAuxNitem = NULL ;
			}
		}
		else
		{
			poAuxAuxNitem = NULL ;
		}
		CHECK_CHLOG_ERRORF( poAuxAuxNitem == NULL, M4_ERROR, M4_MDFAC_AUX_ITEM_PRORAT_NO_AUX_ITEM, LOG_CAT_AND( poAuxNitem->iItem(), poAuxNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;
		CHECK_CHLOG_ERRORF( poAuxAuxNitem == poAuxNitem, M4_ERROR, M4_MDFAC_AUTO_AUX_ITEM, LOG_CAT_AND( poAuxAuxNitem->iItem(), poAuxAuxNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;
		CHECK_CHLOG_ERRORF( poAuxAuxNitem == ai_poNitem, M4_ERROR, M4_MDFAC_AUTO_AUX_ITEM, LOG_CAT_AND( poAuxAuxNitem->iItem(), poAuxAuxNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;

		poAuxAuxItem = poAuxAuxNitem->GetItem() ;
		CHECK_CHLOG_DEBUGF( poAuxAuxItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iCppType = poAuxAuxItem->GetCppType() ;
		CHECK_CHLOG_ERRORF( iCppType != M4CL_CPP_TYPE_DATE, M4_ERROR, M4_MDFAC_AUX_ITEM_PRORAT_BAD_TYPE, LOG_CAT_AND( poAuxAuxNitem->iItem(), poAuxAuxNitem->nItem() ) << M4ClCppType( iCppType ) << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;

		iHandle = poAuxNitem->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt32( ai_pcBuffer, iOffset, m_iConditionantsStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iAssignedStart ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iExecutedStart ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iFormIdsStart ) ;
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iFormulasStart ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iScope ) ;


	iLength = m_oConnectors.GetLength() ;

    iOffset = M4_ROUND_TO_4( iOffset ) ;
    M4ClPackInt16( ai_pcBuffer, iOffset, m_iConnectorsToCompile ) ;

    iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;
		iIsToCompile = poNitenConn->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poNitenConn->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


    iLength = m_oConditionants.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oConditionants.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			iHandle = poNitem->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


    iLength = m_oAssigned.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oAssigned.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			iHandle = poNitem->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

    
    iLength = m_oExecuted.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oExecuted.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poNitem = ( ClTempNitem* ) pvPointer ;
			iHandle = poNitem->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


    iLength = m_oFormIds.GetLength() ;

	if( iLength != 0 )
	{
		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
		iOffset += M4_SIZE_OF_INT16 ;   // nop

		iOffset = M4_ROUND_TO_4( iOffset ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = m_oFormIds.GetElement( i, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poFormula = ( ClTempNitemFormula* ) pvPointer ;
			iHandle = poFormula->GetIdHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


	if( m_iSliceNumber != 0 && m_iFormulaNumber != 0 )
	{
		iOffset = M4_ROUND_TO_8( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iSliceNumber ) ;

		iOffset = M4_ROUND_TO_4( iOffset ) ;
		M4ClPackInt16( ai_pcBuffer, iOffset, m_iFormulaNumber ) ;

		iOffset = M4_ROUND_TO_4( iOffset ) ;


		iPosition = 0 ;

		for( i = 0 ; i < m_iSliceNumber ; i++ )
		{
			iOffset = M4_ROUND_TO_8( iOffset ) ;

			dSliceStartDate = m_pdDatesArray[ i ] ;
			dSliceEndDate = m_pdDatesArray[ i + 1 ] - dTimeUnit ;

			M4ClPackDate( ai_pcBuffer, iOffset, dSliceStartDate ) ;
			M4ClPackDate( ai_pcBuffer, iOffset, dSliceEndDate ) ;

			for( j = 0 ; j < m_iFormulaNumber ; j++ )
			{
				poFormula = ( ClTempNitemFormula* ) m_ppoFormulaArray[ iPosition++ ] ;

				if( poFormula == NULL )
				{
					iHandle = 0 ;
				}
				else
				{
					iHandle = poFormula->GetHandle() ;
				}

				M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			}
		}
	}


// componentes

	iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;
		iIsToCompile = poNitenConn->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poNitenConn->Compile( ai_pcBuffer, ai_iBufferSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    iLength = m_oFormulas.GetLength() ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oFormulas.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula = ( ClTempNitemFormula* ) pvPointer ;

		iIsToCompile = poFormula->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poFormula->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}


void	ClTempRitem :: OrderConnectors( void )
{
	m_oConnectors.Sort( M4ClCompareNitemConnsByOrder ) ;
}

m4return_t	ClTempRitem :: OrderItemConnMappings( void )
{

	m4return_t		iResult ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4pvoid_t		pvPointer ;
	ClTempNitemConn	*poNitenConn ;

    
    iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;
		poNitenConn->OrderArgumentMappings() ;
	}

    return( M4_SUCCESS ) ;
}




m4return_t	ClTempRitem :: ComputeConnectedNitemsScope( ClTempNitem *ai_poFatherNitem )
{

	m4return_t			iResult ;
	m4uint8_t			iValid ;
	m4uint8_t			iIsSeparable ;
	m4uint8_t			iCsType ;
	m4uint8_t			iConnType, iSonType, iFatherType ;
	m4uint8_t			iConnCsType, iSonCsType, iFatherCsType ;
	m4uint8_t			iSonReducedType, iFatherReducedType ;
	m4uint8_t			iConnReducedCsType, iSonReducedCsType, iFatherReducedCsType ;
	m4uint8_t			iSonScope ;
	m4uint8_t			iConnRelation ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pvoid_t           pvPointer ;
	ClTempItem			*poFatherItem ;
	ClTempItem			*poSonItem ;
	ClTempRitem         *poSonRitem ;
	ClTempNitem         *poSonNitem ;
	ClTempNitemConn		*poNitenConn ;
    ClTempConnector     *poConnector ;
	ClTempChannel		*poChannel ;


	poFatherItem = ai_poFatherNitem->GetItem() ;

	iFatherType = poFatherItem->GetType() ;
	iFatherCsType = ai_poFatherNitem->GetCsType() ;

	// esto nunca puede pasar porque se controla antes
	CHECK_CHLOG_DEBUGF( m_iScope > M4CL_ITEM_MAX_SCOPE || m_iScope < 1, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_DEBUGF( iFatherType > M4CL_ITEM_TYPE_CONCEPT || iFatherType < M4CL_ITEM_TYPE_METHOD, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_DEBUGF( iFatherCsType > M4CL_CSTYPE_MAX_NUMBER || iFatherCsType < M4CL_CSTYPE_MIN_NUMBER, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iFatherReducedType = g_aiReducedItemTypesArray[ iFatherType - 1 ] ;
	iFatherReducedCsType = g_aiReducedItemCsTypesArray[ iFatherCsType - 1 ] ;

	CHECK_CHLOG_DEBUGF( iFatherReducedCsType > 3, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iLength = m_oConnectors.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oConnectors.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poNitenConn = ( ClTempNitemConn* ) pvPointer ;

		poConnector = poNitenConn->GetConnector() ;

		if( poConnector != NULL )
		{
            iConnType = poConnector->GetType() ;
		}
        else
        {
            iConnType = M4CL_NODE_CONNECTOR_TYPE_FREE ;
        }

        iConnRelation = poNitenConn->GetRelation() ;
        iConnCsType = poNitenConn->GetCsType() ;

        poSonNitem = poNitenConn->GetSonNitem() ;
		CHECK_CHLOG_DEBUGF( poSonNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		poSonRitem = poSonNitem->GetRitem() ;
		poSonItem = poSonNitem->GetItem() ;

        iSonScope = poSonRitem->GetScope() ;
        iSonType = poSonItem->GetType() ;
        iSonCsType = poSonNitem->GetCsType() ;

        // esto nunca puede pasar porque se controla antes
		CHECK_CHLOG_DEBUGF( iConnType > M4CL_NODE_CONNECTOR_MAX_TYPE || iConnType < 1, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_DEBUGF( iConnCsType > M4CL_CSTYPE_MAX_NUMBER || iConnCsType < M4CL_CSTYPE_MIN_NUMBER, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_DEBUGF( iSonScope > M4CL_ITEM_MAX_SCOPE || iSonScope < 1, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_DEBUGF( iSonType > M4CL_ITEM_TYPE_CONCEPT || iSonType < M4CL_ITEM_TYPE_METHOD, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_DEBUGF( iSonCsType > M4CL_CSTYPE_MAX_NUMBER || iSonCsType < M4CL_CSTYPE_MIN_NUMBER, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iConnReducedCsType = g_aiReducedConnCsTypesArray[ iConnCsType - 1 ] ;
		iSonReducedType = g_aiReducedItemTypesArray[ iSonType - 1 ] ;
		iSonReducedCsType = g_aiReducedItemCsTypesArray[ iSonCsType - 1 ] ;

		CHECK_CHLOG_DEBUGF( iConnReducedCsType > 3, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_DEBUGF( iSonReducedCsType > 3, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		if( iFatherReducedType == 1 && iSonReducedType == 1 )
		{
			// si son celdas reducimos los scopes
			// para cosas que no son por igualdad no comprimimos el tipo
			// ahora sólo para items de registro
			if( iConnRelation == M4CL_ITEM_CONNECTOR_EQUALITY || iSonScope != M4CL_ITEM_SCOPE_REGISTER )
			{
				iSonScope = g_aiScopeArray[ iConnType - 1 ][ m_iScope - 1 ][ iSonScope - 1 ] ;
				poSonRitem->SetScope( iSonScope ) ;
			}

			// chequeo de valided de conexiones por scope para datos

			iValid = g_aiValidScopeArray[ iConnType - 1 ][ m_iScope - 1 ][ iSonScope - 1 ] ;

			if( iValid != 1 )
			{
				DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_CONNECTION_SCOPE, M4ClConnectedNodeType( iConnType ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iNode(), poSonNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iItem(), ai_poFatherNitem->nItem() ) << LOG_CAT_AND( ai_poFatherNitem->iNode(), ai_poFatherNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iM4Obj(), ai_poFatherNitem->nM4Obj() ) << M4ClItemScope( iSonScope ) << M4ClItemScope( m_iScope ) ) ;
				return( M4_ERROR ) ;
			}

			// chequeo de valided de conexiones por CsType para datos
			// sólo para canales separables

			poChannel = ai_poFatherNitem->GetChannel() ;

			if( poChannel != NULL )
			{
				iIsSeparable = poChannel->GetIsSeparable() ;
				iCsType = poChannel->GetCsType() ;

				if( iIsSeparable == 1 && iCsType != M4CL_CSTYPE_BOTH )
				{
					iValid = g_aiValidDataCsTypesArray[ iConnReducedCsType ][ iFatherReducedCsType ][ iSonReducedCsType ] ;

					if( iValid != 1 )
					{
						DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_CONNECTION_CS_TYPE, M4ClConnCsType( iConnCsType ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iNode(), poSonNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iItem(), ai_poFatherNitem->nItem() ) << LOG_CAT_AND( ai_poFatherNitem->iNode(), ai_poFatherNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iM4Obj(), ai_poFatherNitem->nM4Obj() ) << M4ClCsType( iSonCsType ) << M4ClCsType( iFatherCsType ) ) ;
						return( M4_ERROR ) ;
					}
				}
			}
		}
		else if( iFatherReducedType == 0 && iSonReducedType == 0 )
		{
			// chequeo de valided de conexiones por CsType para métodos
			// sólo para canales separables

			poChannel = ai_poFatherNitem->GetChannel() ;

			if( poChannel != NULL )
			{
				iIsSeparable = poChannel->GetIsSeparable() ;
				iCsType = poChannel->GetCsType() ;

				if( iIsSeparable == 1 && iCsType != M4CL_CSTYPE_BOTH )
				{
					iValid = g_aiValidMethodCsTypesArray[ iConnReducedCsType ][ iFatherReducedCsType ][ iSonReducedCsType ] ;

					if( iValid != 1 )
					{
						DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_CONNECTION_CS_TYPE, M4ClConnCsType( iConnCsType ) << LOG_CAT_AND( poSonNitem->iItem(), poSonNitem->nItem() ) << LOG_CAT_AND( poSonNitem->iNode(), poSonNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iItem(), ai_poFatherNitem->nItem() ) << LOG_CAT_AND( ai_poFatherNitem->iNode(), ai_poFatherNitem->nNode() ) << LOG_CAT_AND( ai_poFatherNitem->iM4Obj(), ai_poFatherNitem->nM4Obj() ) << M4ClCsType( iSonCsType ) << M4ClCsType( iFatherCsType ) ) ;
						return( M4_ERROR ) ;
					}
				}
			}
		}
		else
		{
			// aquí no hacemos nada de momento
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: CalculateTotals( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack, ClTempNitem *ai_poBackPayNitem, m4date_t ai_dTimeUnit )
{

    m4return_t          iResult ;
    m4bool_t            bFirstTotal ;
    m4bool_t            bFirstTag ;
    m4bool_t            bFinish ;
	m4uint8_t			iType ;
	m4int8_t			iSumType ;
    m4uint16_t          i, j, k, l ;
    m4uint16_t          iLength ;
    m4uint16_t          iArgs ;
    m4uint16_t          iBackPayArgs ;
    m4uint16_t          iColumns ;
    m4uint16_t          iNotNullColumns ;
    m4uint16_t          iRows ;
    m4uint16_t          iTotals ;
    m4uint16_t          iPosition ;
    m4uint16_t          iOrBlock ;
    m4uint16_t          iAndBlock ;
	m4double_t			fValue ;
	m4date_t			dStartDate ;
	m4date_t			dEndDate ;
	m4pcchar_t			pccValue ;
	m4pcchar_t			pccItemUsedId ;
	m4pcchar_t			pccDimItemId ;
	m4pcchar_t			pccLastDimItemId ;
	m4pchar_t			pcPolish ;
	m4char_t			acOperator[ 5 ] ;
	m4char_t			acString[ 255 ] ;
    ClTempItem			*poItem ;
    ClTempItem			*poDimItem ;
    ClTempNitem			*poDimNitem ;
    ClTempItem			*poBackPayItem ;
    ClTempNitem			*poNitemUsed ;
	ClTempNode			*poNode ;
	ClTempArgument		*poArgument ;
    ClTempTotal			*poTotal ;
	ClTempTotalTag		*poTotalTag ;
    ClTempNitemFormula  *poFormula ;
	ClSlicedElement		**ppoArrayTable ;


	// Ordenamos los tags por total y dimension
	ai_poSliceArray->Sort( M4ClCompareRowsByItemUsedAndDimId ) ;


	// Conseguimos la tabla
	iResult = ai_poSliceArray->GetArrayTable( ppoArrayTable, iColumns, iRows, ai_poDateStack, iNotNullColumns, M4_TRUE ) ;

	// Creamos las fórmulas
	SetFormulasDelta( iNotNullColumns ) ;

	poItem = ai_poNitem->GetItem() ;
	iArgs = poItem->GetNumberOfArguments() ;
	poNode = ai_poNitem->GetNode() ;
	CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iTotals = ai_poSliceArray->GetStart() ;

	iPosition = 0 ;

	for( i = 0 ; i < iColumns ; i++ )
	{
		// Fórmula de un intervalo
		ai_poPolish->Reset() ;

		bFirstTotal = M4_TRUE ;

		for( k = 0 ; k < iTotals ; k++ )
		{
			poTotal = ( ClTempTotal* ) ppoArrayTable[ iPosition + k ] ;

			if( poTotal == NULL )
			{
				continue ;
			}

			// Si el tipo de totalización es 0 no se añade nada (para sobreescritura de totales)
			iSumType = poTotal->GetSumType() ;

			if( iSumType == 0 )
			{
				continue ;
			}

			if( bFirstTotal == M4_TRUE )
			{
				// Los 3 comentarios y la linea en blanco
				// El C4 marca el comienzo de la fórmula en ingeniería inversa
				// La variable local x se inicializa a cero
				iResult = ai_poPolish->Concat( "C4\nN0.0\nA0\nC1\n" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				bFirstTotal = M4_FALSE ;
			}

			pccItemUsedId = poTotal->GetItemUsedId() ;

			iOrBlock = 0 ;
			iAndBlock = 0 ;
			pccLastDimItemId = NULL ;
			bFirstTag = M4_TRUE ;

			for( j = iTotals ; j < iRows ; j++ )
			{
				poTotalTag = ( ClTempTotalTag* ) ppoArrayTable[ iPosition + j ] ;

				if( poTotalTag == NULL || strcmp( pccItemUsedId, poTotalTag->GetItemUsedId() ) != 0 )
				{
					continue ;
				}

				bFirstTag = M4_FALSE ;

				pccDimItemId = poTotalTag->GetDimItemId() ;
				poDimNitem = poNode->GetNitemById( pccDimItemId, M4_FALSE ) ;
				CHECK_CHLOG_ERRORF( poDimNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccDimItemId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poNode->iM4Obj(), poNode->nM4Obj() ) << "Total Tag Dimension Item" ) ;

				poDimItem = poDimNitem->GetItem() ;


				// Ponemos el and si hace falta
				if( iOrBlock == 0 )
				{
					pccLastDimItemId = pccDimItemId ;
				}
				else
				{
					if( M4MdFacStrcmp( pccLastDimItemId, pccDimItemId ) != 0 )
					{
						iOrBlock = 0 ;
						pccLastDimItemId = pccDimItemId ;

						iAndBlock++ ;

						if( iAndBlock > 1 )
						{
							iResult = ai_poPolish->Concat( "F0\n" ) ;
							CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
						}
					}
				}


				// El item dimensión
				iResult = ai_poPolish->Concat( "OR I" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ai_poPolish->Concat( pccDimItemId ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// El valor
				iType = poDimItem->GetCppType() ;
				pccValue = poTotalTag->GetZValue() ;

				if( iType == M4CL_CPP_TYPE_STRING )
				{
					iResult = ai_poPolish->Concat( " 0\nS" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					iResult = ai_poPolish->Concat( pccValue ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
				else if( iType == M4CL_CPP_TYPE_DATE )
				{
					ClStringToDate( pccValue, fValue ) ;
					sprintf( acString, " 0\nD%.8f", fValue ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
				else
				{
					fValue = atof( pccValue ) ;
					sprintf( acString, " 0\nN%.8f", fValue ) ;

					iLength = strlen( acString ) - 1 ;
					bFinish = M4_FALSE ;
					while( bFinish == M4_FALSE )
					{
						if( acString[ iLength ] == '0' )
						{
							acString[ iLength-- ] = '\0' ;
						}
						else
						{
							if( acString[ iLength ] == '.' )
							{
								acString[ iLength + 1 ] = '0' ;
							}
							bFinish = M4_TRUE ;
						}
					}

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}

				// El operador
				sprintf( acOperator, "\nF%c\n", poTotalTag->GetOperator() ) ;

				iResult = ai_poPolish->Concat( acOperator ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Ponemos el or si hace falta
				iOrBlock++ ;

				if( iOrBlock > 1 )
				{
					iResult = ai_poPolish->Concat( "F1\n" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}


			if( bFirstTag == M4_FALSE )
			{
				// Ponemos el and final
				if( iAndBlock > 0 )
				{
					iResult = ai_poPolish->Concat( "F0\n" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}

				// Condición del final del if con un salto 
				sprintf( acString, "J%.0d\nC1\n", k + 1 ) ;

				iResult = ai_poPolish->Concat( acString ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}

			// Sumar la x y el item
			iResult = ai_poPolish->Concat( "R0\nOR I" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ai_poPolish->Concat( poTotal->GetItemUsedId() ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( iSumType == 1 )
			{
				iResult = ai_poPolish->Concat( " 0\nF9\n" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
			else
			{
				iResult = ai_poPolish->Concat( " 0\nF10\n" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}


			poNitemUsed = poNode->GetNitemById( pccItemUsedId, M4_FALSE ) ;
			CHECK_CHLOG_ERRORF( poNitemUsed == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccItemUsedId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poNode->iM4Obj(), poNode->nM4Obj() ) << "Total Item Used" ) ;


			if( poNitemUsed->GetHasBackPay() == 1 && poTotal->GetHasBackPay() == 1 )
			{
				// Poner el back pay

				iResult = ai_poPolish->Concat( "S" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ai_poPolish->Concat( poTotal->GetItemUsedId() ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ai_poPolish->Concat( "\nOF L1 I" ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


				// Se ve si tiene back pay item
				CHECK_CHLOG_ERRORF( ai_poBackPayNitem == NULL, M4_ERROR, M4_NO_BACK_PAY_ITEM, LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;

				// Se ve si el item de back pay tiene un argumento pero no si es de tipo cadena

				poBackPayItem = ai_poBackPayNitem->GetItem() ;
				CHECK_CHLOG_DEBUGF( poBackPayItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iBackPayArgs = poBackPayItem->GetNumberOfArguments() ;
				CHECK_CHLOG_ERRORF( iBackPayArgs != 1, M4_ERROR, M4_NO_BAD_BACK_PAY_ARGUMENTS, LOG_CAT_AND( poBackPayItem->iItem(), poBackPayItem->nItem() ) << iBackPayArgs << LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) << LOG_CAT_AND( ai_poNitem->iNode(), ai_poNitem->nNode() ) << LOG_CAT_AND( ai_poNitem->iM4Obj(), ai_poNitem->nM4Obj() ) ) ;


				iResult = ai_poPolish->Concat( poBackPayItem->GetItemId() ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				if( iSumType == 1 )
				{
					iResult = ai_poPolish->Concat( " 0\nF9\n" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
				else
				{
					iResult = ai_poPolish->Concat( " 0\nF10\n" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}

			// La variable local x se asigna
			iResult = ai_poPolish->Concat( "A0\nC1\n" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			if( bFirstTag == M4_FALSE )
			{
				sprintf( acString, "L%0.d\nC1\n", k + 1 ) ;

				iResult = ai_poPolish->Concat( acString ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}


		if( bFirstTotal == M4_FALSE )
		{
			// Si hay algo que añadir

			// El valor de retorno
			iResult = ai_poPolish->Concat( "R0\n" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			// Ponemos los argumentos por referencia
			for( l = 0 ; l < iArgs ; l++ )
			{
				poArgument = poItem->GetArgumentByPosition( iArgs - l - 1 ) ;
				CHECK_CHLOG_DEBUGF( poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				if( poArgument->GetType() == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE )
				{
					sprintf( acString, "R-%.0d\n", l + 1 ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}

			// El final
			// El C4 marca el final de la fórmula en ingeniería inversa
			iResult = ai_poPolish->Concat( "C4\nE" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// Creamos una nueva fórmula
			poFormula = new ClTempNitemFormula ;
			CHECK_CHLOG_ERRORF( poFormula == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula ) ) << __LINE__ << __FILE__ ) ;

			poFormula->SetUpperPool( m_poUpperPool ) ;
			poFormula->SetNoUpperPool( m_poNoUpperPool ) ;


			// La añadimos al array (da igual que no esté inicializada)
			iResult = AddFormula( poFormula ) ;

			ai_poNitem->GetChannel()->SetPrint( 1 ) ;

			if( iResult != M4_SUCCESS )
			{
				delete( poFormula ) ;
			}

			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			pcPolish = ai_poPolish->Clone() ;
			CHECK_CHLOG_DEBUGF( pcPolish == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ai_poDateStack->GetElement( i, dStartDate ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ai_poDateStack->GetElement( i + 1, dEndDate ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			dEndDate -= ai_dTimeUnit ;


			// Inicializamos la fórmula
			iResult = poFormula->Init( ai_poNitem, poNode, M4CL_LENGUAGE_LN4, pcPolish, "", "", dStartDate, dEndDate, M4_MDFAC_RULE_CREATION_TOTAL, 0 ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		iPosition += iRows ;
	}


	return( M4_SUCCESS ) ;
}



m4return_t	ClTempRitem :: CalculateMetaRules( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack, ClTempNode *ai_poRevNode, ClTempNitem *ai_poRevNitem, m4date_t ai_dTimeUnit )
{

    m4return_t          iResult ;
    m4bool_t            bFirstTag ;
    m4bool_t            bFinish ;
	m4uint8_t			iType ;
	m4uint8_t			iIsRetroactivity ;
    m4uint16_t          i, j, k, l ;
    m4uint16_t          iLength ;
    m4uint16_t          iArgs ;
    m4uint16_t          iColumns ;
    m4uint16_t          iNotNullColumns ;
    m4uint16_t          iRows ;
    m4uint16_t          iFormulas ;
    m4uint16_t          iPosition ;
    m4uint16_t          iOrBlock ;
    m4uint16_t          iAndBlock ;
    m4uint16_t          iRevLabel ;
	m4double_t			fValue ;
	m4date_t			dStartDate ;
	m4date_t			dEndDate ;
	m4pcchar_t			pccFormulaId ;
	m4pcchar_t			pccValue ;
	m4pcchar_t			pccItemId ;
	m4pcchar_t			pccRevItemId ;
	m4pcchar_t			pccDimItemId ;
	m4pcchar_t			pccLastDimItemId ;
	m4pchar_t			pcPolish ;
	m4char_t			acOperator[ 5 ] ;
	m4char_t			acString[ 255 ] ;
    ClTempItem			*poItem ;
    ClTempItem			*poDimItem ;
    ClTempNitem			*poDimNitem ;
	ClTempArgument		*poArgument ;
	ClTempNode			*poNode ;
    ClTempTag			*poTag ;
    ClTempNitemFormula  *poFormula ;
	ClSlicedElement		**ppoArrayTable ;

	
	// Ordenamos los tags por regla y dimension
	ai_poSliceArray->Sort( M4ClCompareRowsByRuleAndDimId ) ;


	// Conseguimos la tabla
	iResult = ai_poSliceArray->GetArrayTable( ppoArrayTable, iColumns, iRows, ai_poDateStack, iNotNullColumns, M4_FALSE ) ;

	// Creamos las fórmulas. Ponemos también las nulas, porque irá la ejecucíon sin más
	SetFormulasDelta( iColumns ) ;

	poItem = ai_poNitem->GetItem() ;
	iArgs = poItem->GetNumberOfArguments() ;
	poNode = ai_poNitem->GetNode() ;
	CHECK_CHLOG_DEBUGF( poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iFormulas = ai_poSliceArray->GetStart() ;

	iPosition = 0 ;

	for( i = 0 ; i < iColumns ; i++ )
	{
		// Fórmula de un intervalo
		ai_poPolish->Reset() ;

		iRevLabel = 0 ;

		// Los 3 comentarios y la linea en blanco
		// El C4 marca el comienzo de la fórmula en ingeniería inversa
		iResult = ai_poPolish->Concat( "C4\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		for( k = 0 ; k < iFormulas ; k++ )
		{
			poFormula = ( ClTempNitemFormula* ) ai_poSliceArray->GetLeaderNoStart( k ) ;
			pccFormulaId = poFormula->GetFormulaId() + 2 ;

			iIsRetroactivity = poFormula->GetIsRetroactivity() ;

			if( iIsRetroactivity == 0 )
			{
				iOrBlock = 0 ;
				iAndBlock = 0 ;
				pccLastDimItemId = NULL ;
				bFirstTag = M4_TRUE ;

				for( j = 0 ; j < iRows ; j++ )
				{
					poTag = ( ClTempTag* ) ppoArrayTable[ iPosition + j ] ;

					if( poTag == NULL || strcmp( pccFormulaId, poTag->GetRuleId() ) != 0 )
					{
						continue ;
					}

					bFirstTag = M4_FALSE ;

					pccDimItemId = poTag->GetDimItemId() ;
					poDimNitem = poNode->GetNitemById( pccDimItemId, M4_FALSE ) ;
					CHECK_CHLOG_ERRORF( poDimNitem == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_ITEM_IN_NODE, pccDimItemId << LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poNode->iM4Obj(), poNode->nM4Obj() ) << "Tag Dimension Item" ) ;

					poDimItem = poDimNitem->GetItem() ;


					// Ponemos el and si hace falta
					if( iOrBlock == 0 )
					{
						pccLastDimItemId = pccDimItemId ;
					}
					else
					{
						if( M4MdFacStrcmp( pccLastDimItemId, pccDimItemId ) != 0 )
						{
							iOrBlock = 0 ;
							pccLastDimItemId = pccDimItemId ;

							iAndBlock++ ;

							if( iAndBlock > 1 )
							{
								iResult = ai_poPolish->Concat( "F0\n" ) ;
								CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							}
						}
					}


					// El item dimensión
					iResult = ai_poPolish->Concat( "OR I" ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					iResult = ai_poPolish->Concat( pccDimItemId ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


					// El valor
					iType = poDimItem->GetCppType() ;
					pccValue = poTag->GetZValue() ;

					if( iType == M4CL_CPP_TYPE_STRING )
					{
						iResult = ai_poPolish->Concat( " 0\nS" ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						iResult = ai_poPolish->Concat( pccValue ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}
					else if( iType == M4CL_CPP_TYPE_DATE )
					{
						ClStringToDate( pccValue, fValue ) ;
						sprintf( acString, " 0\nD%.8f", fValue ) ;

						iResult = ai_poPolish->Concat( acString ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}
					else
					{
						fValue = atof( pccValue ) ;
						sprintf( acString, " 0\nN%.8f", fValue ) ;

						iLength = strlen( acString ) - 1 ;
						bFinish = M4_FALSE ;
						while( bFinish == M4_FALSE )
						{
							if( acString[ iLength ] == '0' )
							{
								acString[ iLength-- ] = '\0' ;
							}
							else
							{
								if( acString[ iLength ] == '.' )
								{
									acString[ iLength + 1 ] = '0' ;
								}
								bFinish = M4_TRUE ;
							}
						}

						iResult = ai_poPolish->Concat( acString ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}

					// El operador
					sprintf( acOperator, "\nF%c\n", poTag->GetOperator() ) ;

					iResult = ai_poPolish->Concat( acOperator ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


					// Ponemos el or si hace falta
					iOrBlock++ ;

					if( iOrBlock > 1 )
					{
						iResult = ai_poPolish->Concat( "F1\n" ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}
				}


				if( bFirstTag == M4_FALSE )
				{
					// Ponemos el and final
					if( iAndBlock > 0 )
					{
						iResult = ai_poPolish->Concat( "F0\n" ) ;
						CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}

					// Condición del final del if con un salto 
					sprintf( acString, "J%.0d\nC1\n", k + 1 ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}
			else
			{
				if( iRevLabel == 0 )
				{
					CHECK_CHLOG_ERRORF( ai_poRevNode == NULL, M4_ERROR, M4_MDFAC_NO_REV_NODE, LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) <<  LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poNode->iM4Obj(), poNode->nM4Obj() ) ) ;

					if( ai_poRevNitem != NULL )
					{
						pccRevItemId = ai_poRevNitem->GetNitemId() ;
					}
					else
					{
						DUMP_CHLOG_ERRORF( M4_MDFAC_NO_REV_ITEM, LOG_CAT_AND( ai_poNitem->iItem(), ai_poNitem->nItem() ) <<  LOG_CAT_AND( poNode->iNode(), poNode->nNode() ) << LOG_CAT_AND( poNode->iM4Obj(), poNode->nM4Obj() ) ) ;						
						return( M4_ERROR ) ;
						// Cableo indecente del nombre del iten de revisión "SCO_REVISION" si es que no viene.
						//pccRevItemId = "SCO_REVISION" ;
					}

					iRevLabel = k + 1 ;

					// El if de revisión con un salto al final
					sprintf( acString, "OR N%s 0 I%s 0\nN1.0\nF3\nJ%.0d\nC1\n", ai_poRevNode->GetTiId(), pccRevItemId, iRevLabel ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}

			
			// Ponemos los argumentos
			for( l = 0 ; l < iArgs ; l++ )
			{
				sprintf( acString, "R-%.0d\n", iArgs - l ) ;

				iResult = ai_poPolish->Concat( acString ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}

			// Esto va aquí por que es del pool
			pccItemId = ai_poNitem->GetNitemId() ;

			// Ponemos el item
			sprintf( acString, "OF L1 I%s 0 R%s\n", pccItemId, pccFormulaId ) ;

			iResult = ai_poPolish->Concat( acString ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			// Sacamos los argumentos por referencia
			for( l = 0 ; l < iArgs ; l++ )
			{
				poArgument = poItem->GetArgumentByPosition( l ) ;
				CHECK_CHLOG_DEBUGF( poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				if( poArgument->GetType() == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE )
				{
					sprintf( acString, "A-%.0d\n", iArgs - l ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}

			// Asignamos la variable local con salto
			iResult = ai_poPolish->Concat( "A0\nC1\n" ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			// Ponemos el final si no es retroactividad y tenía tags (con un salto)
			if( iIsRetroactivity == 0 )
			{
				if( bFirstTag == M4_FALSE )
				{
					sprintf( acString, "L%0.d\nC1\n", k + 1 ) ;

					iResult = ai_poPolish->Concat( acString ) ;
					CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				}
			}
		}


		// Ponemos la última etiqueta si hay revisión (con un salto)
		if( iRevLabel != 0 )
		{
			sprintf( acString, "L%0.d\nC1\n", iRevLabel ) ;

			iResult = ai_poPolish->Concat( acString ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}


		// El valor de retorno
		iResult = ai_poPolish->Concat( "R0\n" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Ponemos los argumentos por referencia
		for( l = 0 ; l < iArgs ; l++ )
		{
			poArgument = poItem->GetArgumentByPosition( iArgs - l - 1 ) ;
			CHECK_CHLOG_DEBUGF( poArgument == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( poArgument->GetType() == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE )
			{
				sprintf( acString, "R-%.0d\n", l + 1 ) ;

				iResult = ai_poPolish->Concat( acString ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}
		}

		// El final
		// El C4 marca el final de la fórmula en ingeniería inversa
		iResult = ai_poPolish->Concat( "C4\nE" ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		// Creamos una nueva fórmula
		poFormula = new ClTempNitemFormula ;
		CHECK_CHLOG_ERRORF( poFormula == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula ) ) << __LINE__ << __FILE__ ) ;

		poFormula->SetUpperPool( m_poUpperPool ) ;
		poFormula->SetNoUpperPool( m_poNoUpperPool ) ;


		// La añadimos al array (da igual que no esté inicializada)
		iResult = AddFormula( poFormula ) ;

		ai_poNitem->GetChannel()->SetPrint( 1 ) ;

		if( iResult != M4_SUCCESS )
		{
			delete( poFormula ) ;
		}

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		pcPolish = ai_poPolish->Clone() ;
		CHECK_CHLOG_DEBUGF( pcPolish == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ai_poDateStack->GetElement( i, dStartDate ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ai_poDateStack->GetElement( i + 1, dEndDate ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		dEndDate -= ai_dTimeUnit ;


		// Inicializamos la fórmula
		iResult = poFormula->Init( ai_poNitem, poNode, M4CL_LENGUAGE_LN4, pcPolish, "", "", dStartDate, dEndDate, M4_MDFAC_RULE_CREATION_SYSTEM_METARULE, 0 ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iPosition += iRows ;
	}


	return( M4_SUCCESS ) ;
}


m4return_t	ClTempRitem :: CalculateFormulas( ClTempNitem *ai_poNitem, ClSliceArray *ai_poSliceArray, ClLongString *ai_poPolish, ClDateStack *ai_poDateStack )
{

    m4return_t          iResult ;
//	m4bool_t			bFound ;
	m4uint8_t			iIsMetarule ;
	m4uint8_t			iIsEvent ;
    m4uint16_t          i, j ;
    m4uint16_t          iNotNullColumns ;
	m4uint32_t		    iStart ;
	m4uint32_t		    iString ;
    m4uint32_t          iSize ;
	m4date_t			*pdDatesArray ;
	m4pcchar_t			pccFormulaId ;
    ClTempItem			*poItem ;
    ClTempNitemFormula  *poFormula ;
	ClSlicedElement		**ppoArrayTable ;


	// Ordenamos por identificador (con la metaregla primero)
	ai_poSliceArray->Sort( M4ClCompareRowsByMetaruleAndId ) ;


	// Conseguimos la tabla
	iResult = ai_poSliceArray->GetArrayTable( ppoArrayTable, m_iSliceNumber, m_iFormulaNumber, ai_poDateStack, iNotNullColumns, M4_FALSE ) ;

	iSize = m_iFormulaNumber * m_iSliceNumber ;


// Quitamos los periodos vacíos ==============================================

 	iStart = 0 ;
/*
	// Si hay un sólo slice seguro que no está vacío
	if( m_iSliceNumber > 1 )
	{
		bFound = M4_FALSE ;
		for( i = 0 ; i < m_iFormulaNumber && bFound == M4_FALSE ; i++ )
		{
			poFormula = ( ClTempNitemFormula* ) ppoArrayTable[ i ] ;

			if( poFormula != NULL )
			{
				bFound = M4_TRUE ;
			}
		}

		if( bFound == M4_FALSE )
		{
			iStart = m_iFormulaNumber ;
			m_iSliceNumber-- ;
		}
	}

	if( m_iSliceNumber > 1 )
	{
		bFound = M4_FALSE ;
		for( i = 0 ; i < m_iFormulaNumber && bFound == M4_FALSE ; i++ )
		{
			poFormula = ( ClTempNitemFormula* ) ppoArrayTable[ iSize - i - 1 ] ;

			if( poFormula != NULL )
			{
				bFound = M4_TRUE ;
			}
		}

		if( bFound == M4_FALSE )
		{
			m_iSliceNumber-- ;
		}
	}


	// Si no hay fórmulas retornamos éxito
	iSize = m_iFormulaNumber * m_iSliceNumber ;
*/
	if( iSize == 0 )
	{
		m_iFormulaNumber = 0 ;
		m_iSliceNumber = 0 ;
		return( M4_SUCCESS ) ;
	}


// metemos el array de ids para el JIT y el debugger ==========================

	m_oFormIds.SetDelta( m_iFormulaNumber ) ;

	for( i = 0 ; i < m_iFormulaNumber ; i++ )
	{
		poFormula = ( ClTempNitemFormula* ) ai_poSliceArray->GetLeader( i ) ;
		CHECK_CHLOG_DEBUGF( poFormula== NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccFormulaId = poFormula->GetFormulaId() + 2 ;

		iResult = m_poUpperPool->AddString( pccFormulaId, iString ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poFormula->SetIdHandle( iString ) ;

		iResult = m_oFormIds.AddElement( poFormula ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


// Conseguimos el array y las fechas ==========================================

	m_ppoFormulaArray = new ClTempNitemFormula* [ iSize ] ;
	CHECK_CHLOG_ERRORF( m_ppoFormulaArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula* ) * iSize ) << __LINE__ << __FILE__ ) ;

	memcpy( m_ppoFormulaArray, ppoArrayTable + iStart, sizeof( ClTempNitemFormula* ) * iSize ) ;


	m_pdDatesArray = new m4date_t[ m_iSliceNumber + 1 ] ;
	CHECK_CHLOG_ERRORF( m_pdDatesArray == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4date_t ) * ( m_iSliceNumber + 1 ) ) << __LINE__ << __FILE__ ) ;

	pdDatesArray = ai_poDateStack->GetElements() ;
	CHECK_CHLOG_DEBUGF( pdDatesArray == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	memcpy( m_pdDatesArray, pdDatesArray + iStart, sizeof( m4date_t ) * ( m_iSliceNumber + 1 ) ) ;


// Establecemos el Order, el MustReturn, el IsToCompile y el IsEvent

	poItem = ai_poNitem->GetItem() ;
	CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iIsEvent = poItem->GetIsEvent() ;


	for( i = 0 ; i < m_iSliceNumber ; i++ )
	{
		for( j = 0 ; j < m_iFormulaNumber ; j++ )
		{
			poFormula = ( ClTempNitemFormula* ) ppoArrayTable[ iStart++ ] ;

			if( poFormula == NULL )
			{
				continue ;
			}

			iIsMetarule = poFormula->GetIsMetarule() ;

			if( iIsMetarule == 1 && m_iFormulaNumber > 1 )
			{
				poFormula->SetMustReturn( 0 ) ;
			}

			poFormula->SetOrder( j ) ;
			poFormula->SetIsToCompile( 1 ) ;

			if( iIsEvent == 1 && poFormula->GetIsEvent() == 1 )
			{
				ai_poNitem->SetFlag( M4CL_MCR_ITEM_FLAG_IS_EVENT ) ;
			}
		}
	}


	return( M4_SUCCESS ) ;
}



// nitems ================================================

ClTempNitem :: ClTempNitem( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iSCRStart = 0 ;
    m_iIndex = 0 ;

	m_iFlags = 0 ;
	m_iCsType = M4CL_CSTYPE_MAPPED ;
	m_iSyncType = M4_SYNC_DOCUMENT ;
	m_iLevel = 0 ;

    m_poItem = NULL ;
    m_poRitem = NULL ;
    m_poNode = NULL ;
	m_poChannel = NULL ;

    m_iIsInGroup = 0 ;
	m_iHasBackPay = 0 ;
	m_iIsOverWritten = 0 ;
	m_iIsNameOverWritten = 0 ;

    m_iIsToCompile = 1 ;
}



ClTempNitem :: ~ClTempNitem( void )
{
	if( GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 1 )
	{
		delete( m_poRitem ) ;
		m_poRitem = NULL ;
		UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;
	}
}




m4return_t  ClTempNitem :: SetItem( ClTempChannel *ai_poChannel, ClTempNode *ai_poNode, ClTempItem *ai_poItem, ClTempRitem *ai_poRitem )
{

	m4uint8_t		iCsType ;
    m4uint8_t       iType ;
	m4uint8_t		iVariableArguments ;
    m4uint16_t      iArguments ;
    m4pcchar_t      pccGroupId ;


	CHECK_CHLOG_DEBUGF( ai_poChannel == NULL || ai_poNode == NULL || ai_poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    m_poChannel = ai_poChannel ;
    m_poNode = ai_poNode ;
    m_poItem = ai_poItem ;


	iCsType = m_poItem->GetCsType() ;

	if( iCsType == M4CL_CSTYPE_UNKNOWN )
	{
		if( m_poNode != NULL )
		{
			iCsType = m_poNode->GetCsType() ;
		}
		else
		{
			iCsType = M4CL_CSTYPE_BOTH ;
		}
	}

	m_iCsType = iCsType ;
	m_iSyncType = m_poItem->GetSyncType() ;


    pccGroupId = m_poItem->GetGroupId() ;

    if( pccGroupId != NULL && *pccGroupId != '\0' )
    {
        iArguments = m_poItem->GetNumberOfArguments() ;
		iVariableArguments = m_poItem->GetVariableArguments() ;
        iType = m_poItem->GetType() ;

        if( iArguments > 0 || iVariableArguments == 1 || iType == M4CL_ITEM_TYPE_PROPERTY || iType == M4CL_ITEM_TYPE_FIELD )
        {
			if( M4ChLogIsErrorIn( M4_MDFAC_BAD_GROUP_ITEM ) == 0 )
			{
				/* Bug 0079330
				Se pasa el mensaje a error
				*/
				DUMP_CHLOG_ERRORF( M4_MDFAC_BAD_GROUP_ITEM, LOG_CAT_AND( iItem(), nItem() ) << LOG_CAT_AND( iNode(), nNode() ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << pccGroupId ) ;
			}
            m_iIsInGroup = 0 ;
        }
        else
        {
            m_iIsInGroup = 1 ;
        }
    }
    else
    {
        m_iIsInGroup = 0 ;
    }


	if( ai_poRitem == NULL )
	{
		ai_poRitem = new ClTempRitem;
		CHECK_CHLOG_ERRORF( ai_poRitem == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempRitem ) ) << __LINE__ << __FILE__ ) ;

		ai_poRitem->SetUpperPool( m_poUpperPool ) ;
		ai_poRitem->SetNoUpperPool( m_poNoUpperPool ) ;

		ai_poRitem->SetScope( ai_poItem->GetScope() ) ;
		ai_poRitem->SetName( ai_poItem->GetName() ) ;

		SetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;
	}

	m_poRitem = ai_poRitem ;


    return( M4_SUCCESS ) ;
}



// funciones de impresión

m4pcchar_t	ClTempNitem :: iItem( void ) const
{
	if( m_poItem == NULL )
	{
		return( "" ) ;
	}

	return( m_poItem->GetItemId() ) ;
}


m4pcchar_t	ClTempNitem :: nItem( void ) const
{

	m4pcchar_t	pccResult ;


	if( m_poItem == NULL )
	{
		return( "" ) ;
	}

	pccResult = m_poItem->GetName() ;

	if( *pccResult == '\0' )
	{
		pccResult = m_poItem->GetItemId() ;
	}

	return( pccResult ) ;
}


m4pcchar_t	ClTempNitem :: iNode( void ) const
{
	if( m_poNode == NULL )
	{
		return( "" ) ;
	}

	return( m_poNode->GetNodeId() ) ;
}


m4pcchar_t	ClTempNitem :: nNode( void ) const
{

	m4pcchar_t	pccResult ;


	if( m_poNode == NULL )
	{
		return( "" ) ;
	}

	pccResult = m_poNode->GetName() ;

	if( *pccResult == '\0' )
	{
		pccResult = m_poNode->GetNodeId() ;
	}

	return( pccResult ) ;
}


m4pcchar_t	ClTempNitem :: iM4Obj( void ) const
{
	if( m_poChannel == NULL )
	{
		return( "" ) ;
	}

	return( m_poChannel->GetChannelId() ) ;
}


m4pcchar_t	ClTempNitem :: nM4Obj( void ) const
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




// Funciones de lectura

m4pcchar_t	ClTempNitem :: GetNodeId( void ) const
{

	m4pcchar_t	pccResult ;


	if( m_poNode != NULL )
	{
		pccResult = m_poNode->GetNodeId() ;
	}
	else
	{
		pccResult = "" ;
	}

    return( pccResult ) ;
}




m4return_t	ClTempNitem :: PreGetSize( void )
{

	m4return_t		iResult ;


	if( GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 1 )
	{
		iResult = m_poRitem->PreGetSize() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitem :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

    m4return_t		iResult ;


// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_NITEM_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_NITEM, ao_iSize - m_iHandle ) ;
	}

// Componentes ================================================================

	if( GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 1 )
	{
		iResult = m_poRitem->GetSize( ao_iSize, m_poItem->GetType(), ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

    
	return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitem :: Compile( ClTempChannel *ai_poChannel, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

    m4return_t		iResult ;
    m4uint32_t		iOffset ;
    m4uint32_t		iHandle ;


    iOffset = m_iHandle ;


    iHandle = m_poItem->GetHandle() ;
    M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    iHandle = m_poRitem->GetHandle() ;
    M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poNode != NULL )
    {
        iHandle = m_poNode->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

    M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt16( ai_pcBuffer, iOffset, m_iSCRStart ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iIndex ) ;

// Se guarda el bit uno si es myritem y el bit 2 si es evento
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iFlags ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iCsType ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iSyncType ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iLevel ) ;


// componentes

	if( GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 1 )
	{
		iResult = m_poRitem->Compile( ai_poChannel, this, ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitem :: UnreferNitem( ClTempTi *ai_poSystemTi )
{

	m4return_t			iResult ;
	m4uint16_t			i ;
	m4uint16_t			iSystemRules ;
	m4pcchar_t			pccItemId ;
	m4pcchar_t			pccStoredItemId ;
    ClTempItemFormula   *poItemFormula ;
    ClTempNitemFormula  *poNitemFormula ;


	if( GetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) == 1 )
	{
		return( M4_SUCCESS ) ;
	}


	m_poRitem = new ClTempRitem ;
	CHECK_CHLOG_ERRORF( m_poRitem == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempRitem ) ) << __LINE__ << __FILE__ ) ;

	m_poRitem->SetUpperPool( m_poUpperPool ) ;
	m_poRitem->SetNoUpperPool( m_poNoUpperPool ) ;

	m_poRitem->SetScope( m_poItem->GetScope() ) ;
	m_poRitem->SetName( m_poItem->GetName() ) ;

	SetFlag( M4CL_MCR_ITEM_FLAG_IS_MY_RITEM ) ;


	if( ai_poSystemTi != NULL )
	{
		iSystemRules = ai_poSystemTi->GetNumberOfItemFormulas() ;
	}
	else
	{
		iSystemRules = 0 ;
	}

	for( i = 0 ; i < iSystemRules ; i++ )
	{
		poItemFormula = ai_poSystemTi->GetItemFormulaByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poItemFormula == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccItemId = poItemFormula->GetItemId() ;

		// Va aquí por que es del pool
		pccStoredItemId = m_poItem->GetItemId() ;

		// Manera cutre de meter sólo las reglas de mi item
		if( M4MdFacStrcmp( pccItemId, pccStoredItemId ) == 0 )
		{
			// Creamos una nueva fórmula
			poNitemFormula = new ClTempNitemFormula ;
			CHECK_CHLOG_ERRORF( poNitemFormula == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempNitemFormula ) ) << __LINE__ << __FILE__ ) ;

			poNitemFormula->SetUpperPool( m_poUpperPool ) ;
			poNitemFormula->SetNoUpperPool( m_poNoUpperPool ) ;

			// Inicializamos la fórmula
			iResult = poNitemFormula->Init( this, poItemFormula, 0 ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			// La añadimos al array
			m_poRitem->SetFormulasDelta( 1 ) ;
			iResult = m_poRitem->AddFormula( poNitemFormula ) ;

			if( iResult != M4_SUCCESS )
			{
				delete( poNitemFormula ) ;
			}

			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}


	CHECK_CHLOG_DEBUGF( m_poNode == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


    return( M4_SUCCESS ) ;
}




