
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facsec.hpp  
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                02-04-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions of the class of the factory of
//    compiled metachannels related to the security
//
//
//==============================================================================


#include "m4mdfaci.hpp"
#include "m4facdef.hpp"
#include "m4facres.hpp"

#include "m4mdrt.hpp"
#include "m4objglb.hpp"
#include "m4buildversion.hpp"
#include "m4mdadaptor.hpp"
#include "chlog.hpp"
#include "node.hpp"
#include "node_knl.hpp"
#include "sttwrap.hpp"
#include "sttlib.hpp"
#include "m4cstrace.hpp"
#include "cllgadap.hpp"
#include "m4objreg.hpp"



// Lectura de valores conociendo el mcr ===================================

m4return_t	M4ReadNumber( ClCompiledMCR *ai_poCMCR, m4double_t &ao_rdValue, m4double_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItemId << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadNumber( ClCompiledMCR *ai_poCMCR, m4double_t &ao_rdValue, m4double_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister, m4double_t ai_dMinimum, m4double_t ai_dMaximum )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItemId << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_NUMBER )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_NUMBER ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;

// sólo si hemos leido valor

		if( ao_rdValue < ai_dMinimum )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MIM_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MIM_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << ai_dMinimum << ai_dDefault << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}

		if( ao_rdValue > ai_dMaximum )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MAX_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << ai_dMaximum << ai_dDefault << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadString( ClCompiledMCR *ai_poCMCR, m4pcchar_t &ao_rpccValue, m4pcchar_t ai_pccDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rpccValue = ai_pccDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItemId << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_STRING_VAR )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rpccValue = vValue.Data.PointerChar ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4ReadDate( ClCompiledMCR *ai_poCMCR, m4date_t &ao_rdValue, m4date_t ai_dDefault, ClItemIndex ai_iIndex, m4pcchar_t ai_pccItemId, ClRegister *ai_poRegister )
{

	m4return_t		iResult ;
	m4uint32_t		iHandle ;
    m4VariantType	vValue ;
	ClCompiledMCR	*poCMCR ;


	ao_rdValue = ai_dDefault ;

    iResult = ai_poRegister->Item[ ai_iIndex ].Value.Get( vValue ) ;

    if( iResult != M4_SUCCESS )
    {
		poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
		iHandle = ai_poRegister->GetpNode_Data()->GetpNodeDef()->GetHandle() ;

		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_GET_ITEM_VALUE, ai_pccItemId << LOG_CAT_AND( poCMCR->iNodeN( iHandle ), poCMCR->nNodeN( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
    }

	if( vValue.Type != M4CL_CPP_TYPE_NULL )
	{
		if( vValue.Type != M4CL_CPP_TYPE_DATE )
		{
			poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
			iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

			DUMP_CHLOG_ERRORF( M4_MDFAC_ITEM_TYPE_MISMATCH, LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4ClCppType( M4CL_CPP_TYPE_DATE ) << M4ClCppType( vValue.Type ) << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			return( M4_ERROR ) ;
		}

		ao_rdValue = vValue.Data.DoubleData ;

// sólo si hemos leido valor

		if( ao_rdValue < M4CL_MINUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MIM_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MIM_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_MINUS_INFINITE_IN_JULIAN << ai_dDefault << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}

		if( ao_rdValue > M4CL_PLUS_INFINITE_IN_JULIAN )
		{
			if( M4ChLogIsErrorIn( M4_MDFAC_MAX_VALUE ) == 0 )
			{
				poCMCR = ai_poRegister->Item.ItemDef.GetpCMCR() ;
				iHandle = ai_poRegister->Item.ItemDef.GetHandle() ;

				DUMP_CHLOG_WARNINGF( M4_MDFAC_MAX_VALUE, ao_rdValue << LOG_CAT_AND( poCMCR->iItemI( iHandle ), poCMCR->nItemI( iHandle ) ) << LOG_CAT_AND( poCMCR->iNodeI( iHandle ), poCMCR->nNodeI( iHandle ) ) << LOG_CAT_AND( poCMCR->iM4Obj(), poCMCR->nM4Obj() ) << M4CL_PLUS_INFINITE_IN_JULIAN << ai_dDefault << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) ) ;
			}

			ao_rdValue = ai_dDefault ;
		}
	}

	return( M4_SUCCESS ) ;
}



// ====================================================================

m4return_t  ClFactoryImp :: _CreateSecurityAccess( void )
{

    m4return_t      iResult ;


    if( m_iIsSecurityBuilt == 1 )
    {
		return( M4_SUCCESS ) ;
    }

    iResult = _BuildSystemChannel( M4MDFAC_FILE_M4SEC_M2, m_poSecurityM4Obj ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }


    m_poSecurityAccess = m_poSecurityM4Obj->CreateAccess() ;
	CHECK_CHLOG_DEBUGF( m_poSecurityAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_iIsSecurityBuilt = 1 ;

	return( M4_SUCCESS ) ;
}



// lectura de seguridad ===============================================

m4return_t  ClFactoryImp :: _ReadOneT3SecFromAccess( m4pchar_t ai_pcMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR, m4pchar_t ai_pcSecurity )
{

    m4return_t		iResult ;
	m4uint32_t		iOffset ;
	m4date_t		dT3Version ;

	m4double_t		fValue ;
	m4date_t		dValue ;



	iResult = M4ReadNumber( ai_poCMCR, fValue, 0, M4MDFAC_T3_MASK_T3_SEC, M4MDFAC_ITEM_SEC, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	*ai_pcMask = m4char_t( fValue ) ;


	dT3Version = M4ClUnPackDate( ai_pcSecurity, M4CL_SCR_SECURITY_T3_VERSION ) ;

	iResult = M4ReadDate( ai_poCMCR, dValue, dT3Version, M4MDFAC_T3_MASK_VERSION, M4MDFAC_ITEM_VERSION, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

    iOffset = M4CL_SCR_SECURITY_OWN_VERSION ;
	M4ClPackDate( ai_pcSecurity, iOffset, dValue ) ;


	return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadOneNodeSecFromAccess( m4pchar_t ai_pcMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t		iResult ;
	m4double_t		fValue ;



	iResult = M4ReadNumber( ai_poCMCR, fValue, 0, M4MDFAC_NODE_MASK_T3_SEC, M4MDFAC_ITEM_SEC, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	*ai_pcMask = m4char_t( fValue ) ;
	

    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadOneItemSecFromAccess( m4pchar_t ai_pcMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t		iResult ;
	m4double_t		fValue ;



	iResult = M4ReadNumber( ai_poCMCR, fValue, 0, M4MDFAC_ITEM_MASK_T3_SEC, M4MDFAC_ITEM_SEC, ai_poRegister ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	*ai_pcMask = m4char_t( fValue ) ;


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadOneFilterSecFromAccess( m4pchar_t ai_pcMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t		iResult ;
	m4double_t		fValue ;



	iResult = M4ReadNumber( ai_poCMCR, fValue, 0, M4MDFAC_FILTER_MASK_IS_APPLICABLE, M4MDFAC_ITEM_IS_APPLICABLE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	*ai_pcMask = m4char_t( fValue ) ;


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadOneT3ConSecFromAccess( m4pchar_t ai_pcMask, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t		iResult ;
	m4double_t		fValue ;



	iResult = M4ReadNumber( ai_poCMCR, fValue, 0, M4MDFAC_T3_CON_MASK_IS_APPLICABLE, M4MDFAC_ITEM_IS_APPLICABLE, ai_poRegister, 0, 1 ) ;

    if( iResult != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	*ai_pcMask = m4char_t( fValue ) ;


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadNodesSecFromAccess( ClAccess* ai_poAccess, ClCompiledMCR *ai_poCMCR )
{

    m4return_t          iResult ;
    m4uint8_t           iFound ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    m4uint16_t          iNode ;
    m4uint16_t          iNodeNumber ;
	m4uint32_t			iSize ;
    m4uint32_t          iHandle ;
    m4uint32_t          iStart ;
    m4uint32_t          hNode ;
    m4uint32_t          iStringOffset ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccStoredNodeId ;
	m4pchar_t			pcSecurity ;
	m4pchar_t			pcMask ;
	m4pcchar_t			pccFilterId ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
    ClNode              *poDNode ;
	ClCompiledMCR		*poSecurityCMCR ;


    poSecurityCMCR = m_poSecurityAccess->GetpCMCR() ;

    iHandle = poSecurityCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_NODE_MASK, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_NODE_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Node Mask" ) ;

    poDNode = &( m_poSecurityAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_NODE_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Node Mask" ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


	pcSecurity = ai_poCMCR->GetSecurity() ;

	iStringOffset = ai_poCMCR->GetChannelSecurity() ;				// parte fija
	iStringOffset += strlen( pcSecurity + iStringOffset ) + 1 ;		// id_mask
	iStringOffset += strlen( pcSecurity + iStringOffset ) + 1 ;		// id_channel
    
    iLength = m4uint16_t( poDRecordSet->Count() ) ;
	iNodeNumber = ai_poCMCR->GetChannelNumberOfNodes() ;


    iNode = 0 ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;

		iResult = M4ReadString( ai_poCMCR, pccNodeId, "", M4MDFAC_NODE_MASK_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hNode = 0 ;

        while( iFound == 0 && iNode < iNodeNumber )
        {
            hNode = ai_poCMCR->GetChannelNodeHandleByPosition( iNode ) ;

            if( hNode != 0 )
            {
                pccStoredNodeId = ai_poCMCR->GetNodeId( hNode ) ;
				CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( strcmpi( pccNodeId, pccStoredNodeId ) == 0 )
                {
                    iFound = 1 ;
                }
                else
                {
                    iNode++ ;
                }
            }

        } ;

        if( iFound == 0 )
	    {
            hNode = ai_poCMCR->GetChannelNodeHandleById( pccNodeId ) ;

            if( hNode == 0 )
            {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iStart = ai_poCMCR->GetNodeSCRStart( hNode ) ;
		pcMask = pcSecurity + iStart ;

		iResult = _ReadOneNodeSecFromAccess( pcMask, poRegister, ai_poCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

// los filtros de seguridad

		iResult = M4ReadString( ai_poCMCR, pccFilterId, "", M4MDFAC_NODE_MASK_SEC_FILTER, M4MDFAC_ITEM_SEC_FILTER, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		if( pccFilterId != NULL && *pccFilterId != '\0' )
		{
			iSize = strlen( pccFilterId ) + 1 ;

			iStart += M4CL_SCR_NODE_SEC_FILTER ;
			M4ClPackInt32( pcSecurity, iStart, iStringOffset ) ;
			memcpy( pcSecurity + iStringOffset, pccFilterId, iSize ) ;

			iStringOffset += iSize ;
		}
    }


    return( M4_SUCCESS ) ;
}


m4return_t  ClFactoryImp :: _ReadItemsSecFromAccess( ClAccess* ai_poAccess, ClCompiledMCR *ai_poCMCR )
{

    m4return_t          iResult ;
    m4uint8_t           iFound ;
    m4uint8_t           iInternalType ;
    m4uint16_t          i, j ;
    m4uint16_t          iLength ;
    m4uint16_t          iNode ;
    m4uint16_t          iItem ;
    m4uint16_t          iNodeNumber ;
    m4uint16_t          iItemNumber ;
    m4uint32_t          iHandle ;
    m4uint32_t          iStart ;
    m4uint32_t          hNode ;
    m4uint32_t          hItem ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccItemId ;
    m4pcchar_t          pccStoredNodeId ;
    m4pcchar_t          pccStoredItemId ;
	m4pchar_t			pcSecurity ;
	m4pchar_t			pcMask ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
    ClNode              *poDNode ;
	ClCompiledMCR		*poSecurityCMCR ;


    poSecurityCMCR = m_poSecurityAccess->GetpCMCR() ;

    iHandle = poSecurityCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_ITEM_MASK, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_ITEM_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Item Mask" ) ;

    poDNode = &( m_poSecurityAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_ITEM_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Item Mask" ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


	pcSecurity = ai_poCMCR->GetSecurity() ;
    
    iLength = m4uint16_t( poDRecordSet->Count() ) ;
	iNodeNumber = ai_poCMCR->GetChannelNumberOfNodes() ;


    iNode = 0 ;
    iItem = 0 ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;

		iResult = M4ReadString( ai_poCMCR, pccNodeId, "", M4MDFAC_ITEM_MASK_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hNode = 0 ;

        while( iFound == 0 && iNode < iNodeNumber )
        {
            hNode = ai_poCMCR->GetChannelNodeHandleByPosition( iNode ) ;

            if( hNode != 0 )
            {
                pccStoredNodeId = ai_poCMCR->GetNodeId( hNode ) ;
				CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( strcmpi( pccNodeId, pccStoredNodeId ) == 0 )
                {
                    iFound = 1 ;
                }
                else
                {
                    iNode++ ;
                    iItem = 0 ;
                }
            }

        } ;

        if( iFound == 0 )
	    {
            hNode = ai_poCMCR->GetChannelNodeHandleById( pccNodeId ) ;

            if( hNode == 0 )
            {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iResult = M4ReadString( ai_poCMCR, pccItemId, "", M4MDFAC_ITEM_MASK_ID_ITEM, M4MDFAC_ITEM_ID_ITEM, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hItem = 0 ;
        iItemNumber = ai_poCMCR->GetNodeNumberOfItems( hNode ) ;

        while( iFound == 0 && iItem < iItemNumber ) 
        {
            hItem = ai_poCMCR->GetNodeItemHandleByPosition( hNode, iItem ) ;

            if( hItem != 0 )
            {
                pccStoredItemId = ai_poCMCR->GetItemId( hItem ) ;
				CHECK_CHLOG_DEBUGF( pccStoredItemId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( strcmpi( pccItemId, pccStoredItemId ) == 0 )
                {
                    iFound = 1 ;
                }
                else
                {
                    iItem++ ;
                }
            }

        } ;

        if( iFound == 0 )   // por si no esta ordenado
        {
            hItem = ai_poCMCR->GetNodeItemHandleById( hNode, pccItemId ) ;

            if( hItem == 0 )
	        {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iStart = ai_poCMCR->GetItemSCRStart( hItem ) ;
		pcMask = pcSecurity + iStart ;

		iResult = _ReadOneItemSecFromAccess( pcMask, poRegister, ai_poCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


	/* Bug 0108765
	Hay que dar permisos a los items que se generan automáticamente
	*/
    for( i = 0 ; i < iNodeNumber ; i++ )
    {
        hNode = ai_poCMCR->GetChannelNodeHandleByPosition( i ) ;

        if( hNode == 0 )
        {
			continue ;
		}

        iItemNumber = ai_poCMCR->GetNodeNumberOfItems( hNode ) ;

		for( j = 0 ; j < iItemNumber ; j++ )
		{
            hItem = ai_poCMCR->GetNodeItemHandleByPosition( hNode, j ) ;

            if( hItem == 0 )
            {
				continue ;
			}

            iInternalType = ai_poCMCR->GetItemInternalType( hItem ) ;

			if( iInternalType == M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE || iInternalType == M4CL_INTERNAL_TYPE_SYSDB_ITEM )
			{
				iStart = ai_poCMCR->GetItemSCRStart( hItem ) ;
				pcSecurity[ iStart ] = 1 + 2 + 4 + 8 + 16 + 32 ;	// (1*T3_SEC_READ)+(2*T3_SEC_WRITE)+(4*T3_SEC_DELETE)+(8*T3_SEC_UPDATE)+(16*T3_SEC_EXE)+(32*T3_SEC_CONTINUE)
			}
		}
	}


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadFiltersSecFromAccess( ClAccess* ai_poAccess, ClCompiledMCR *ai_poCMCR )
{

    m4return_t          iResult ;
    m4uint8_t           iFound ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    m4uint16_t          iNode ;
    m4uint16_t          iNodeNumber ;
    m4uint32_t          iHandle ;
    m4uint32_t          iStart ;
    m4uint32_t          hNode ;
    m4uint32_t          hFilter ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccFilterId ;
    m4pcchar_t          pccStoredNodeId ;
	m4pchar_t			pcSecurity ;
	m4pchar_t			pcMask ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
    ClNode              *poDNode ;
	ClCompiledMCR		*poSecurityCMCR ;


    poSecurityCMCR = m_poSecurityAccess->GetpCMCR() ;

    iHandle = poSecurityCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_FILTER_MASK, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_FILTER_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Filter Mask" ) ;

    poDNode = &( m_poSecurityAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_FILTER_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Filter Mask" ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


	pcSecurity = ai_poCMCR->GetSecurity() ;
    
    iLength = m4uint16_t( poDRecordSet->Count() ) ;
	iNodeNumber = ai_poCMCR->GetChannelNumberOfNodes() ;


    iNode = 0 ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;

		iResult = M4ReadString( ai_poCMCR, pccNodeId, "", M4MDFAC_FILTER_MASK_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hNode = 0 ;

        while( iFound == 0 && iNode < iNodeNumber )
        {
            hNode = ai_poCMCR->GetChannelNodeHandleByPosition( iNode ) ;

            if( hNode != 0 )
            {
                pccStoredNodeId = ai_poCMCR->GetNodeId( hNode ) ;
				CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( strcmpi( pccNodeId, pccStoredNodeId ) == 0 )
                {
                    iFound = 1 ;
                }
                else
                {
                    iNode++ ;
                }
            }

        } ;

        if( iFound == 0 )
	    {
            hNode = ai_poCMCR->GetChannelNodeHandleById( pccNodeId ) ;

            if( hNode == 0 )
            {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iResult = M4ReadString( ai_poCMCR, pccFilterId, "", M4MDFAC_FILTER_MASK_ID_FILTER, M4MDFAC_ITEM_ID_FILTER, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		hFilter = ai_poCMCR->GetNodeQBFFilterHandleById( hNode, pccFilterId ) ;

		if( hFilter == 0 )
		{
			hFilter = ai_poCMCR->GetNodeFindFilterHandleById( hNode, pccFilterId ) ;

			if( hFilter == 0 )
			{
				continue ;
			}
		}


		iStart = ai_poCMCR->GetFilterSCRStart( hFilter ) ;
		pcMask = pcSecurity + iStart ;

		iResult = _ReadOneFilterSecFromAccess( pcMask, poRegister, ai_poCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


    return( M4_SUCCESS ) ;
}



m4return_t  ClFactoryImp :: _ReadT3ConsSecFromAccess( ClAccess* ai_poAccess, ClCompiledMCR *ai_poCMCR )
{

    m4return_t          iResult ;
    m4uint8_t           iFound ;
    m4uint16_t          i ;
    m4uint16_t          iLength ;
    m4uint16_t          iNode ;
    m4uint16_t          iPlug ;
    m4uint16_t          iNodeNumber ;
    m4uint16_t          iPlugNumber ;
    m4uint32_t          iHandle ;
    m4uint32_t          iStart ;
    m4uint32_t          hNode ;
    m4uint32_t          hPlug ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccT3UsedId ;
    m4pcchar_t          pccNodeIdUsed ;
    m4pcchar_t          pccStoredNodeId ;
    m4pcchar_t          pccStoredT3UsedId ;
    m4pcchar_t          pccStoredNodeIdUsed ;
	m4pchar_t			pcSecurity ;
	m4pchar_t			pcMask ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
    ClNode              *poDNode ;
	ClCompiledMCR		*poSecurityCMCR ;


    poSecurityCMCR = m_poSecurityAccess->GetpCMCR() ;

    iHandle = poSecurityCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_T3_CONNECTOR_MASK, M4_TRUE ) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_T3_CONNECTOR_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Meta4Object Connector Mask" ) ;

    poDNode = &( m_poSecurityAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_T3_CONNECTOR_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Meta4Object Connector Mask" ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


	pcSecurity = ai_poCMCR->GetSecurity() ;
    
    iLength = m4uint16_t( poDRecordSet->Count() ) ;
	iNodeNumber = ai_poCMCR->GetChannelNumberOfNodes() ;


    iNode = 0 ;
    iPlug = 0 ;

    for( i = 0 ; i < iLength ; i++ )
    {
        poRegister = &( poDRecordSet->Register[ i ] ) ;

		iResult = M4ReadString( ai_poCMCR, pccNodeId, "", M4MDFAC_T3_CON_MASK_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hNode = 0 ;

        while( iFound == 0 && iNode < iNodeNumber )
        {
            hNode = ai_poCMCR->GetChannelNodeHandleByPosition( iNode ) ;

            if( hNode != 0 )
            {
                pccStoredNodeId = ai_poCMCR->GetNodeId( hNode ) ;
				CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( strcmpi( pccNodeId, pccStoredNodeId ) == 0 )
                {
                    iFound = 1 ;
                }
                else
                {
                    iNode++ ;
                    iPlug = 0 ;
                }
            }

        } ;

        if( iFound == 0 )
	    {
            hNode = ai_poCMCR->GetChannelNodeHandleById( pccNodeId ) ;

            if( hNode == 0 )
            {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iResult = M4ReadString( ai_poCMCR, pccT3UsedId, "", M4MDFAC_T3_CON_MASK_ID_T3_USED, M4MDFAC_ITEM_ID_T3_USED, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		iResult = M4ReadString( ai_poCMCR, pccNodeIdUsed, "", M4MDFAC_T3_CON_MASK_ID_NODE_USED, M4MDFAC_ITEM_ID_NODE_USED, poRegister ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

        iFound = 0 ;
        hPlug = 0 ;
        iPlugNumber = ai_poCMCR->GetNodeNumberOfPlugs( hNode ) ;

        while( iFound == 0 && iPlug < iPlugNumber ) 
        {
            hPlug = ai_poCMCR->GetNodePlugHandleByPosition( hNode, iPlug ) ;

            if( hPlug != 0 )
            {
                pccStoredT3UsedId = ai_poCMCR->GetPlugFatherChanneId( hPlug ) ;
				CHECK_CHLOG_DEBUGF( pccStoredT3UsedId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                pccStoredNodeIdUsed = ai_poCMCR->GetPlugFatherNodeId( hPlug ) ;
				CHECK_CHLOG_DEBUGF( pccStoredNodeIdUsed == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                if( ( strcmpi( pccT3UsedId, pccStoredT3UsedId ) == 0 ) && ( strcmpi( pccNodeIdUsed, pccStoredNodeIdUsed ) == 0 ) )
                {
                    iFound = 1 ;
                }
                else
                {
                    iPlug++ ;
                }
            }

        } ;

        if( iFound == 0 )   // por si no esta ordenado
        {
            hPlug = ai_poCMCR->GetNodePlugHandleByIds( hNode, pccT3UsedId, pccNodeIdUsed ) ;

            if( hPlug == 0 )
	        {
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
				continue ;
	        }
        }


		iStart = ai_poCMCR->GetPlugSCRStart( hPlug ) ;
		pcMask = pcSecurity + iStart ;

		iResult = _ReadOneT3ConSecFromAccess( pcMask, poRegister, ai_poCMCR ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
    }


    return( M4_SUCCESS ) ;
}





m4return_t  ClFactoryImp :: _ReadSecurityFromAccess( ClAccess* ai_poAccess, ClRegister *ai_poRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t  iResult ;
	m4pchar_t	pcSecurity ;
	m4pchar_t	pcMask ;


// t3s =====================================

	pcSecurity = ai_poCMCR->GetSecurity() ;
	pcMask = pcSecurity + M4CL_SCR_T3_MASK_START ;

    iResult = _ReadOneT3SecFromAccess( pcMask, ai_poRegister, ai_poCMCR, pcSecurity ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// nodes =====================================

    iResult = _ReadNodesSecFromAccess( ai_poAccess, ai_poCMCR ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// items =====================================

    iResult = _ReadItemsSecFromAccess( ai_poAccess, ai_poCMCR ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// filters =====================================

    iResult = _ReadFiltersSecFromAccess( ai_poAccess, ai_poCMCR ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


// plugs =====================================

    iResult = _ReadT3ConsSecFromAccess( ai_poAccess, ai_poCMCR ) ;

    if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _CreateSecFromAccess( m4pcchar_t ai_pccMask, ClAccess* ai_poAccess, m4uint8_t ai_iRegister, ClCompiledMCR *ai_poCMCR )
{

    m4return_t          iResult ;
	m4uint8_t			iDebug ;
    m4uint16_t          iLength ;
    m4uint32_t          iHandle ;
	m4pcchar_t			pccPath ;
    ClNode              *poDNode ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
	ClAccess			*poSecurityAccess ;
    ClCompiledMCR       *poCMCR ;

    
	CHECK_CHLOG_DEBUGF( ai_poAccess == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poCMCR = ai_poAccess->GetpCMCR() ;
	CHECK_CHLOG_DEBUGF( poCMCR == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iHandle = poCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_T3_MASKS , M4_TRUE) ;
	CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_T3_MASKS << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Meta4Object Mask" ) ;

    poDNode = &( ai_poAccess->Node[ ClHandle( iHandle ) ] ) ;
	CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_T3_MASKS << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Meta4Object Mask" ) ;

    poDRecordSet = &( poDNode->RecordSet ) ;


    iLength = m4uint16_t( poDRecordSet->Count() ) ;

    if( iLength <= ai_iRegister )
    {
		poSecurityAccess = NULL ;
	}
	else
	{
		poDRecordSet->Current.MoveTo( ai_iRegister ) ;
		poSecurityAccess = ai_poAccess ;
	}
	

	iResult = _CreateSecBuffer( ai_pccMask, ai_poCMCR, poSecurityAccess ) ;

    if( iResult != M4_SUCCESS )
    {
        return( iResult ) ;
    }


    if( iLength <= ai_iRegister )
    {
// no hay seguridad a aplicar. se siente, se te queda todo bloqueado
		return( M4_SUCCESS ) ;
    }
    

    poRegister = &( poDRecordSet->Register[ ai_iRegister ] ) ;

    iResult = _ReadSecurityFromAccess( ai_poAccess, poRegister, ai_poCMCR ) ;

    if( iResult != M4_SUCCESS )
    {
        return( iResult ) ;
    }


// Imprimir la metaseguridad ==================================================

	if( m_poEnvironment != NULL )
	{
		iDebug = ClVMDebugEnv::getInstance()->getDebugLevel() ;
		pccPath = m_poEnvironment->GetDebugDirectory() ;

		ai_poCMCR->PrintSecurity( iDebug, pccPath ) ;
	}


    return( M4_SUCCESS ) ;
}



m4return_t	ClFactoryImp :: _CreateSecBuffer( m4pcchar_t ai_pccMask, ClCompiledMCR *ai_poCMCR, ClAccess* ai_poSecurityAccess ) const
{

    m4return_t          iResult ;

	m4uint8_t			iCsType ;
    m4uint8_t           iFound ;

    m4uint16_t          i ;
    m4uint16_t          iNode ;
    m4uint16_t          iNodeNumber ;
	m4uint16_t			iLength ;
    m4uint16_t			iBuild ;

	m4uint32_t			iFixed ;
    m4uint32_t          iHandle ;
	m4uint32_t			iMask ;
	m4uint32_t			iChannel ;
	m4uint32_t			iOffset ;
	m4uint32_t			iPosition ;
	m4uint32_t			iSize ;
	m4uint32_t			iDoubleSize ;
    m4uint32_t          hNode ;

	m4date_t			dVersion ;

	m4pchar_t			pcSecurity ;
	m4pcchar_t			pccFilterId ;
    m4pcchar_t          pccNodeId ;
    m4pcchar_t          pccStoredNodeId ;
	m4pcchar_t			pccM4Obj ;

	ClCSCRWrapper		*poCSCRWrapper ;
    ClRegister          *poRegister ;
    ClAccessRecordSet   *poDRecordSet ;
    ClNode              *poDNode ;
	ClCompiledMCR		*poSecurityCMCR ;


	CHECK_CHLOG_DEBUGF( ai_pccMask == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	pccM4Obj = ai_poCMCR->GetChannelId() ;

// a ver si calculamos el tamaño =========================================

	iFixed = ai_poCMCR->GetChannelSecurity() ;
	iMask = strlen( ai_pccMask ) + 1 ;
	iChannel = strlen( pccM4Obj ) + 1 ;

	iSize = iFixed + iMask + iChannel ;

	iNodeNumber = ai_poCMCR->GetChannelNumberOfNodes() ;

// los filtros de seguridad ==============================================

    if( ai_poSecurityAccess != NULL )
    {
		poSecurityCMCR = ai_poSecurityAccess->GetpCMCR() ;

		iHandle = poSecurityCMCR->GetChannelNodeHandleById( M4MDFAC_NODE_SYS_NODE_MASK, M4_TRUE ) ;
		CHECK_CHLOG_DEBUGF( iHandle == 0, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_NODE_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Node Mask" ) ;

		poDNode = &( ai_poSecurityAccess->Node[ ClHandle( iHandle ) ] ) ;
		CHECK_CHLOG_DEBUGF( poDNode == NULL, M4_ERROR, M4_MDFAC_NO_SUCH_NODE, M4MDFAC_NODE_SYS_NODE_MASK << LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << "Node Mask" ) ;

		poDRecordSet = &( poDNode->RecordSet ) ;


		pcSecurity = ai_poCMCR->GetSecurity() ;
    
		iLength = m4uint16_t( poDRecordSet->Count() ) ;

		iNode = 0 ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( poDRecordSet->Register[ i ] ) ;

			iResult = M4ReadString( ai_poCMCR, pccNodeId, "", M4MDFAC_NODE_MASK_ID_NODE, M4MDFAC_ITEM_ID_NODE, poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iFound = 0 ;
			hNode = 0 ;

			while( iFound == 0 && iNode < iNodeNumber )
			{
				hNode = ai_poCMCR->GetChannelNodeHandleByPosition( iNode ) ;

				if( hNode != 0 )
				{
					pccStoredNodeId = ai_poCMCR->GetNodeId( hNode ) ;
					CHECK_CHLOG_DEBUGF( pccStoredNodeId == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					if( strcmpi( pccNodeId, pccStoredNodeId ) == 0 )
					{
						iFound = 1 ;
					}
					else
					{
						iNode++ ;
					}
				}

			} ;

			if( iFound == 0 )
			{
				hNode = ai_poCMCR->GetChannelNodeHandleById( pccNodeId ) ;

				if( hNode == 0 )
				{
// bueno, no pasa nada, puede que no esté en la vista
//		        return( M4_ERROR ) ;
					continue ;
				}
			}


			iResult = M4ReadString( ai_poCMCR, pccFilterId, "", M4MDFAC_NODE_MASK_SEC_FILTER, M4MDFAC_ITEM_SEC_FILTER, poRegister ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			if( pccFilterId != NULL && *pccFilterId != '\0' )
			{
				iSize += strlen( pccFilterId ) + 1 ;
			}
		}
	}


// alineamiento a 8 ======================================================

	iDoubleSize = ( ( iSize - 1 ) >> 0x03 ) + 1 ;
	iSize = iDoubleSize * sizeof( m4double_t ) ;

    pcSecurity = (m4pchar_t) new m4double_t[ iDoubleSize ] ;
	CHECK_CHLOG_ERRORF( pcSecurity == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( iSize ) << __LINE__ << __FILE__ ) ;

	memset( pcSecurity, 0, iSize ) ;

	iOffset = 0 ;
	M4ClPackInt32( pcSecurity, iOffset, iSize ) ;

	iBuild = GetM2VersionNumber() ;
	M4ClPackInt16( pcSecurity, iOffset, iBuild ) ;

	iCsType = ai_poCMCR->GetChannelCsType() ;
	M4ClPackInt8 ( pcSecurity, iOffset, iCsType ) ;
		
    iOffset = M4_ROUND_TO_4( iOffset ) ;

	dVersion = ai_poCMCR->GetChannelVersion() ;
	M4ClPackDate( pcSecurity, iOffset, dVersion ) ;
	M4ClPackDate( pcSecurity, iOffset, dVersion ) ;

	M4ClPackInt16( pcSecurity, iOffset, iNodeNumber ) ;
		
    iOffset = M4_ROUND_TO_4( iOffset ) ;

	iPosition = iFixed ;
	M4ClPackInt32( pcSecurity, iOffset, iPosition ) ;
	memcpy( pcSecurity + iPosition, ai_pccMask, iMask ) ;

	iPosition += iMask;
	M4ClPackInt32( pcSecurity, iOffset, iPosition ) ;
	memcpy( pcSecurity + iPosition, pccM4Obj, iChannel ) ;


	poCSCRWrapper = new ClCSCRWrapper ;
	CHECK_CHLOG_ERRORF( poCSCRWrapper == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClCSCRWrapper ) ) << __LINE__ << __FILE__ ) ;

	poCSCRWrapper->SetBuffer( pcSecurity ) ;
    ai_poCMCR->SetCSCRWrapper( poCSCRWrapper ) ;


    return( M4_SUCCESS ) ;
}




m4return_t	ClFactoryImp :: _CreateSecFromId( m4pcchar_t ai_pccRSM, m4uint8_t ai_iConvertRSM, ClCompiledMCR *ai_poCMCR )
{

    m4return_t      iResult ;
	m4pcchar_t		pccMask ;
	m4uint8_t		iCsType ;
	m4bool_t		bSttEnabled ;
	m4pchar_t		pcRole ;
	m4pcchar_t		pccM4Obj ;
	ClVMStatistics	*poStatistics ;
	ClCSCRWrapper	*poSecWrap = NULL ;
	ClCSCRWrapper	*poSecWrapAux = NULL ;


	poStatistics = m_poStatisticsWrap->GetStatistics() ;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer	oSttStateTimer( poStatistics, m_poStatisticsWrap->GetStatisticsLevel(), M4_VM_STT_SPECIAL_ITEM_CREATE_SCR ) ;


	if( ai_pccRSM == NULL )
	{
// para no tener lios
		ai_pccRSM = "" ;
	}

// pues a calcular la máscara, que para eso nos lo hemos currado

	if( ai_iConvertRSM == 1 )
	{
		pccMask = ai_poCMCR->GetChannelRSMMaskById( ai_pccRSM ) ;
	}
	else
	{
		pccMask = ai_pccRSM ;
	}

	CHECK_CHLOG_ERRORF( *pccMask == '\0', M4_ERROR, M4_MDFAC_EMPTY_SECURITY_MASK, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccRSM ) ;


	pccM4Obj = ai_poCMCR->GetChannelId() ;

	bSttEnabled = oSttStateTimer.GetSttEnabled() ;

	if( m_poSecCache != NULL && ai_poCMCR->GetChannelIsMDCacheable() != M4CL_CACHEABLE_FALSE )
	{
		iCsType = ai_poCMCR->GetChannelCsType() ;
		iResult = m_poSecCache->GetObject( iCsType, pccM4Obj, pccMask, poSecWrap ) ;

		if( bSttEnabled == M4_TRUE )
		{
			//Ponemos el cachehit a las Stt

			if( iResult == M4_SUCCESS )
			{
				poStatistics->sttAddCacheHit( M4_TRUE ) ;
			}
			else
			{
				poStatistics->sttAddCacheHit( M4_FALSE ) ;
			}
		}
	}
	else
	{
		iResult = M4_ERROR ;
	}

	if( bSttEnabled == M4_TRUE )
	{
		if( poStatistics->sttGetInitState() & M4_VM_STATISTICS_ENABLED_PARAMS )
		{
			poStatistics->sttAddExecParam( (m4pchar_t) pccM4Obj ) ;
			poStatistics->sttAddExecParam( (m4pchar_t) pccMask ) ;
			poStatistics->sttAddExecParam( (m4double_t) ai_poCMCR->GetChannelCsType() ) ;
		}
	}


	if( iResult == M4_SUCCESS )
	{
		//Si está en la cache.
		if( m_poCsTrace->IsEnabled() == M4_TRUE )
		{
			pcRole = m_poLogonAdaptor->GetRole() ;

			m_poCsTrace->PreSecurityTrace() ;
			m_poCsTrace->PostSecurityTrace( 0, 0, (m4pchar_t) pccM4Obj, iCsType, pcRole, M4_TRUE ) ;
		}

		//Comprobamos las fechas de mascara y metacanal 
		if( poSecWrap->GetT3Version() < ai_poCMCR->GetChannelVersion() )
		{
			// La mascara cacheada esta desfasada: 
			// Pasar a la Cola de Borrado, Liberar referencia y Repetir el Build (que ya no encontrara la mask en la cache).
			poSecWrap->Destroy( M4_TRUE ) ;
			return( _CreateSecFromId( ai_pccRSM, ai_iConvertRSM, ai_poCMCR ) ) ;
		}

		ai_poCMCR->SetCSCRWrapper( poSecWrap ) ;

		return( M4_SUCCESS ) ;
	}


	// Llamar a remoto si hace falta. Función polimórfica
	iResult = p_DelegateSecFromId( pccMask, ai_poCMCR, poSecWrap ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	if( m_poSecCache != NULL )
	{
		poSecWrap = ai_poCMCR->GetCSCRWrapper() ;

		//Borramos de la caché todos los SCRs con el mismo ID y versión anterior.
		m_poSecCache->RemoveObjectByIdAndVersion( (m4pchar_t) pccM4Obj, poSecWrap->GetVersion(), ClBaseCacheInterface::MAIN ) ;

		if( ai_poCMCR->GetChannelIsMDCacheable() != M4CL_CACHEABLE_FALSE )
		{
			//Hacemos el Put de la caché si es que no está
			iResult = m_poSecCache->GetObject( iCsType, pccM4Obj, pccMask, poSecWrapAux ) ;

			if( iResult == M4_SUCCESS )
			{
				if( poSecWrapAux != NULL )
				{
					poSecWrapAux->Destroy() ;
					poSecWrapAux = NULL ;
				}
			}
			else
			{
				iResult = m_poSecCache->PutObject( iCsType, pccM4Obj, pccMask, poSecWrap ) ;

				if( iResult == M4_ERROR )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_CACHE_PUT_CSCR, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
				}
			}
		}
	}

    return( M4_SUCCESS ) ;
}





//=================================================================================
//
// ClFactoryClient
//
// Clase que implementa la factoría cliente base de metadatos y seguridad
//
//=================================================================================

m4return_t	ClFactoryClient :: p_DelegateSecFromId( m4pcchar_t ai_pccMask, ClCompiledMCR *ai_poCMCR, ClCSCRWrapper* &ai_rpoSecWrap )
{

    m4return_t	iResult ;


	iResult = m_poMDAdaptor->RemoteGetCSCR( ai_poCMCR->GetChannelCsType(), ai_poCMCR->GetChannelId(), ai_pccMask, ai_rpoSecWrap, m_poStatisticsWrap ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ai_poCMCR->SetCSCRWrapper( ai_rpoSecWrap ) ;

    return( M4_SUCCESS ) ;
}



//=================================================================================
//
// ClFactoryServer
//
// Clase que implementa la factoría server base de metadatos y seguridad
//
//=================================================================================

m4return_t	ClFactoryServer :: p_DelegateSecFromId( m4pcchar_t ai_pccMask, ClCompiledMCR *ai_poCMCR, ClCSCRWrapper* &ai_rpoSecWrap )
{

    m4return_t      iResult ;
	m4pcchar_t		pccM4Obj ;
	m4VariantType   vValue ;
    ClNode          *poDNode ;

	
	if( m_iIsSecurityBuilt == 0 )
	{
		iResult = _CreateSecurityAccess() ;

		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
			return( M4_ERROR ) ;
		}
	}

	poDNode = &( m_poSecurityAccess->Node[ M4MDFAC_NODE_SYS_T3_MASKS ] ) ;


	iResult = poDNode->RecordSet.Item[ M4MDFAC_ITEM_ID_MASK ].Value.Set( ai_pccMask, strlen( ai_pccMask ) + 1 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		return( M4_ERROR ) ;
	}

	pccM4Obj = ai_poCMCR->GetChannelId() ;

	iResult = poDNode->RecordSet.Item[ M4MDFAC_ITEM_ID_T3 ].Value.Set( pccM4Obj, strlen( pccM4Obj ) + 1 ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		return( M4_ERROR ) ;
	}


	poDNode->RecordSet.Item[ M4CL_ITEM_LOAD ] ;

	if( poDNode->RecordSet.Item.GetIndex() == M4DM_EOF_INDEX )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		return( M4_ERROR ) ;
	}

	iResult = poDNode->RecordSet.Item.Call() ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		m_poSecurityM4Obj->ReleaseAll() ;
		return( M4_ERROR ) ;
	}


	iResult = poDNode->RecordSet.Item.StackPop( vValue ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		m_poSecurityM4Obj->ReleaseAll() ;
		return( M4_ERROR ) ;
	}

	if( vValue.Type != M4CL_CPP_TYPE_NUMBER || vValue.Data.DoubleData != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4_MDFAC_NO_CREATE_SECURITY, LOG_CAT_AND( ai_poCMCR->iM4Obj(), ai_poCMCR->nM4Obj() ) << ai_pccMask << M4ClCsType( ai_poCMCR->GetChannelCsType() ) ) ;
		m_poSecurityM4Obj->ReleaseAll() ;
		return( M4_ERROR ) ;
	}

	iResult = _CreateSecFromAccess( ai_pccMask, m_poSecurityAccess, 0, ai_poCMCR ) ;

	// limpia todo el contenido del metacanal, para la siguiente ejecución:

	m_poSecurityM4Obj->ReleaseAll() ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ai_rpoSecWrap = ai_poCMCR->GetCSCRWrapper() ;

	return( M4_SUCCESS ) ;
}

