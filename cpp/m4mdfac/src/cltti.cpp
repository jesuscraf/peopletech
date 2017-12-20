
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cltti.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                20-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the funcions of the temporary ti
//
//
//==============================================================================


#include "m4facres.hpp"

#include "m4stl.hpp"
#include "cltti.hpp"
#include "cltitem.hpp"
#include "cltnode.hpp"
#include "clttiinh.hpp"
#include "chlog.hpp"
#include "cltsent.hpp"
#include "clttrig.hpp"
#include "clttiref.hpp"
#include "m4mdrt.hpp"
#include "m4hsparser.hpp"


//=================================================================================
//
// ClTempTi
//
// Clase que implementa la definición de las Tis
//
//=================================================================================


// Funciones de inicialización ================================================

ClTempTi :: ClTempTi( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iTiId = 0 ;
    m_iReadObjectId = 0 ;
    m_iWriteObjectId = 0 ;
	m_iReferencesStart = 0 ;

    m_iTemporaryId = 0 ;
    m_iComplete = 0 ;
    m_iCorrected = 0 ;
	m_iOwnSentence = 0 ;
	m_iGenerateSql = 0 ;

	m_poTiReferences = NULL ;

    m_poReadSentence = NULL ;
    m_poWriteSentence = NULL ;
    m_poSentence = NULL ;

	m_iSentenceNumber = 0 ;
    m_iInheritanceType = 0 ;

    m_poComparisonItem = &m_oComparisonItem ;

	m_iIsToCompile = 1 ;
	m_iItemsToCompile = 0 ;
}



ClTempTi :: ~ClTempTi( void )
{
    Delete() ;
}



m4return_t	ClTempTi :: Delete( void )
{
	if( m_iOwnSentence == 1 && m_poSentence != NULL )
	{
		delete( m_poSentence ) ;
		m_poSentence = NULL ;
	}
    
	return( M4_SUCCESS ) ;
}



//  Otras funciones de lectura ================================================

ClTempNode*	ClTempTi :: GetNode( m4uint8_t ai_iLevel ) const
{
	return( m_poTiReferences[ ai_iLevel ].GetNode() ) ;
}


m4uint32_t	ClTempTi :: GetReferences( m4uint8_t ai_iLevel ) const
{
	return( m_poTiReferences[ ai_iLevel ].GetReferences() ) ;
}


ClTempTiInherit*	ClTempTi :: GetInheritByPosition( m4uint16_t ai_iPosition )
{

	m4return_t		iResult ;
	m4pvoid_t		pvElement ;
	ClTempTiInherit	*poResult ;


	iResult = m_oInherits.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poResult = ( ClTempTiInherit* ) pvElement ;

	return( poResult ) ;
}


ClTempItem*	ClTempTi :: GetItemByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempItem      *poResult ;


	iResult = m_oItems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempItem* ) pvElement ;

	return( poResult ) ;
}



ClTempItemInherit*	ClTempTi :: GetItemInheritByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			pvElement ;
    ClTempItemInherit	*poResult ;


	iResult = m_oItemInherits.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempItemInherit* ) pvElement ;

	return( poResult ) ;
}



ClTempConcept*	ClTempTi :: GetConceptByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempConcept   *poResult ;


	iResult = m_oConcepts.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempConcept* ) pvElement ;

	return( poResult ) ;
}



ClTempTrigger*  ClTempTi :: GetTriggerByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTrigger	*poResult ;


	iResult = m_oTriggers.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTrigger* ) pvElement ;

	return( poResult ) ;
}


ClTempTotal*  ClTempTi :: GetTotalByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTotal		*poResult ;


	iResult = m_oTotals.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTotal* ) pvElement ;

	return( poResult ) ;
}



ClTempTotalTag*  ClTempTi :: GetTotalTagByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTotalTag	*poResult ;


	iResult = m_oTotalTags.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTotalTag* ) pvElement ;

	return( poResult ) ;
}



ClTempTag*  ClTempTi :: GetTagByPosition( m4uint16_t ai_iPosition )
{

    m4return_t		iResult ;
    m4pvoid_t		pvElement ;
    ClTempTag		*poResult ;


	iResult = m_oTags.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempTag* ) pvElement ;

	return( poResult ) ;
}



ClTempItemFormula*  ClTempTi :: GetItemFormulaByPosition( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
    m4pvoid_t			pvElement ;
    ClTempItemFormula	*poResult ;


	iResult = m_oItemFormulas.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempItemFormula* ) pvElement ;

	return( poResult ) ;
}



ClTempItem*    ClTempTi :: GetItemById( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
{

    ClTempItem **ppoResult ;


	CHECK_CHLOG_DEBUGF( ai_pccItemId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    m_oComparisonItem.SetIdentifier( ai_pccItemId, ai_bConvert ) ;

	ppoResult = ( ClTempItem** ) m_oItems.Find( M4ClCompareObjectsById, &m_poComparisonItem ) ;

    if( ppoResult == NULL )
    {
        return( NULL ) ;
    }

    return( *ppoResult ) ;
}



// Otras funciones de escritura ===============================================

void	ClTempTi :: SetNode( ClTempNode* ai_poNode, m4uint8_t ai_iLevel )
{
	m_poTiReferences[ ai_iLevel ].SetNode( ai_poNode ) ;
}


// Funciones de serialización =================================================

m4return_t	ClTempTi :: GetSize( m4uint32_t &ao_iSize, m4uint16_t ai_iInheritLevel, ClMdStatistics *ai_poStatistics )
{

	m4return_t	    iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4pvoid_t       pvPointer ;
	ClTempItem      *poItem ;


// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_TI_FIXED_SIZE ;


// Variables ==================================================================

//	iLength = m_oItems.GetLength() ;

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iItemsToCompile * ( M4CL_MCR_TI_ITEM_SIZE ) ;


	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iReferencesStart = ao_iSize ;
	ao_iSize += ai_iInheritLevel * M4CL_MCR_REFERENCE_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_TI, ao_iSize - m_iHandle ) ;
	}

// Componentes ================================================================

	iLength = m_oItems.GetLength() ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poItem->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	if( m_iOwnSentence == 1 && m_poSentence != NULL )
	{
		iResult = m_poSentence->GetSize( ao_iSize, ai_poStatistics ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	return( M4_SUCCESS ) ;
}



m4return_t	ClTempTi :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize, m4uint16_t ai_iInheritLevel )
{

	m4return_t	    iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4uint32_t	    iOffset ;
	m4uint32_t	    iHandle ;
	m4uint32_t	    iReferences ;
	m4pvoid_t       pvPointer ;
	ClTempItem      *poItem ;
	ClTempNode      *poNode ;


    iOffset = m_iHandle ;

// fijos

    M4ClPackInt32( ai_pcBuffer, iOffset, m_iTiId + ai_iBufferSize ) ;

    if( m_poSentence != NULL )
    {
        iHandle = m_poSentence->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poWriteSentence != NULL )
    {
        iHandle = m_poWriteSentence->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
	
    M4ClPackInt32( ai_pcBuffer, iOffset, m_iReadObjectId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iWriteObjectId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iReferencesStart ) ;

	M4ClPackInt8( ai_pcBuffer, iOffset, m_iTemporaryId ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iComplete ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCorrected ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iOwnSentence ) ;


	iLength = m_oItems.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iItemsToCompile ) ;


// variables

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poItem->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


	iOffset = M4_ROUND_TO_4( iOffset ) ;
	
	for( i = 0 ; i < ai_iInheritLevel ; i++ )
	{
		poNode = m_poTiReferences[ i ].GetNode() ;

		if( poNode != NULL )
		{
			iIsToCompile = poNode->GetIsToCompile() ;

			if( iIsToCompile == 1 )
			{
				iHandle = poNode->GetHandle() ;
			}
			else
			{
				iHandle = 0 ;
			}
		}
		else
		{
			iHandle = 0 ;
		}

		if( iHandle != 0 )
		{
			iReferences = m_poTiReferences[ i ].GetReferences() ;
		}
		else
		{
			iReferences = 0 ;
		}

		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iReferences ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


//  componentes

    iLength = m_oItems.GetLength() ;
	
	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poItem->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	if( m_poSentence != NULL && m_poSentence != m_poReadSentence )
	{
		iResult = m_poSentence->Compile( ai_pcBuffer, ai_iBufferSize, ai_iPoolSize ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}


	return( M4_SUCCESS ) ;
}



// Funciones varias ===========================================================


m4return_t	ClTempTi :: CalculateDynamicSentence( void )
{

	m4return_t		iResult ;
	m4uint8_t	    iFirst ;
	m4uint8_t	    iType ;
	m4uint8_t	    iScope ;
	m4uint8_t	    iIsToCompile ;
	m4uint16_t	    i, j ;
	m4uint16_t	    iItems ;
	m4uint16_t	    iInherits ;
	m4uint32_t		iSize ;
	m4pchar_t		pcSentence ;
	m4pcchar_t		pccTiId ;
	m4pcchar_t		pccItemId ;
	m4pcchar_t		pccFieldId ;
	m4pcchar_t		pccTiObjectId ;
	ClTempItem      *poItem ;
	ClTempTi		*poBaseTi ;
	ClTempTiInherit	*poTiInherit ;


	iSize = 15	; // parte fija "SELECT  FROM &\0"

	pccTiId = m_poUpperPool->GetString( m_iTiId ) ;
	pccTiObjectId = m_poUpperPool->GetString( m_iReadObjectId ) ;

	if( *pccTiObjectId == '\0' )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_SENTENCE_WITHOUT_OBJECT, pccTiId ) ;
		return( M4_SUCCESS ) ;
	}

// Primera pasada para el tamaño ==============================================

	iFirst = 1 ;

	iInherits = GetNumberOfInherits() ;

	for( i = 0 ; i < iInherits ; i++ )
	{
		poTiInherit = GetInheritByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBaseTi = poTiInherit->GetBaseTi() ;
		CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iItems = poBaseTi->GetNumberOfItems() ;

		for( j = 0 ; j < iItems ; j++ )
		{
			poItem = poBaseTi->GetItemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iIsToCompile = poItem->GetIsToCompile() ;

			if( iIsToCompile == 0 )
			{
				continue ;
			}

			iType = poItem->GetType() ;
			iScope = poItem->GetScope() ;
			pccFieldId = poItem->GetReadFieldId() ;

			// Si es campo, de registro, y tiene campo se trata
			if( iType == M4CL_ITEM_TYPE_FIELD && iScope == M4CL_ITEM_SCOPE_REGISTER && *pccFieldId != '\0' )
			{
				pccItemId = poItem->GetItemId() ;

				iSize += strlen( pccItemId ) + strlen( pccFieldId ) + 6 ;	// "@ = , "

				if( iFirst == 1 )
				{
					iFirst = 0 ;
				}
			}
		}
	}

	// check for no items
	if( iFirst == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_SENTENCE_WITHOUT_FIELDS, pccTiId ) ;
		return( M4_SUCCESS ) ;
	}

	iSize += strlen( pccTiObjectId ) ;

	pcSentence = new m4char_t[ iSize ] ;
	CHECK_CHLOG_ERRORF( pcSentence == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iSize ) << __LINE__ << __FILE__ ) ;

	sprintf( pcSentence, "SELECT " ) ;

// segunda pasada para generar

	iFirst = 1 ;
	iInherits = GetNumberOfInherits() ;

	for( i = 0 ; i < iInherits ; i++ )
	{
		poTiInherit = GetInheritByPosition( i ) ;
		CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBaseTi = poTiInherit->GetBaseTi() ;
		CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iItems = poBaseTi->GetNumberOfItems() ;

		for( j = 0 ; j < iItems ; j++ )
		{
			poItem = poBaseTi->GetItemByPosition( j ) ;
			CHECK_CHLOG_DEBUGF( poItem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iIsToCompile = poItem->GetIsToCompile() ;

			if( iIsToCompile == 0 )
			{
				continue ;
			}

			iType = poItem->GetType() ;
			iScope = poItem->GetScope() ;
			pccFieldId = poItem->GetReadFieldId() ;

			// Si es campo, de registro, y tiene campo se trata
			if( iType == M4CL_ITEM_TYPE_FIELD && iScope == M4CL_ITEM_SCOPE_REGISTER && *pccFieldId != '\0' )
			{
				pccItemId = poItem->GetItemId() ;

				if( iFirst == 0 )
				{
					strcat( pcSentence, ", " ) ;
				}
				else
				{
					iFirst = 0 ;
				}

				strcat( pcSentence, "@" ) ;
				strcat( pcSentence, pccItemId ) ;
				strcat( pcSentence, " = " ) ;
				strcat( pcSentence, pccFieldId ) ;
			}
		}
	}

	strcat( pcSentence, " FROM &" ) ;
	strcat( pcSentence, pccTiObjectId ) ;

	m_poSentence = new ClTempSentence ;
	m_iOwnSentence = 1 ;
	CHECK_CHLOG_ERRORF( m_poSentence == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempSentence ) ) << __LINE__ << __FILE__ ) ;
	
	m_poSentence->SetUpperPool( m_poUpperPool ) ;
	m_poSentence->SetNoUpperPool( m_poNoUpperPool ) ;

    iResult = m_poSentence->SetApiSql1( pcSentence ) ;
	delete [] pcSentence ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	m_poSentence->SetSentenceId( m_iTiId ) ;

	return( M4_SUCCESS ) ;
}


// Herencia de sentencias
m4return_t	ClTempTi :: CalculateInheritedSentence( M4HsParser *ai_poHsParser )
{

	m4return_t		iResult ;
	m4uint16_t		i ;
	m4uint16_t		iInherits ;
	m4pcchar_t		pccBaseGroupObjects;
	m4pcchar_t		pccGroupObjects ;
	m4pcchar_t		pccBaseSentence ;
	m4pcchar_t		pccSentence ;
	m4pcchar_t		pccNewSentence ;
	ClTempTi		*poBaseTi ;
	ClTempTiInherit	*poTiInherit ;
	ClTempSentence	*poSentence ;

	
	iInherits = GetNumberOfInherits() ;

	for( i = 0 ; i < iInherits ; i++ )
	{

		// fix bug 0089762
		// we have to walk from upper to lower level
		poTiInherit = GetInheritByPosition( iInherits - i - 1 ) ;
		CHECK_CHLOG_DEBUGF( poTiInherit == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poBaseTi = poTiInherit->GetBaseTi() ;
		CHECK_CHLOG_DEBUGF( poBaseTi == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poSentence = poBaseTi->GetReadSentence() ;

		if( poSentence == NULL )
		{
			continue ;
		}

		pccSentence = poSentence->GetApiSql1() ;

		if( pccSentence == NULL || *pccSentence == '\0' )
		{
			continue ;
		}

		if( m_poSentence == NULL )
		{
			// Si es la primera se asigna y ya está
			m_poSentence = poSentence ;
		}
		else
		{
			// Hay que hacer merge
			pccBaseSentence = m_poSentence->GetApiSql1() ;

			/* Bug 0092542
			El orden de las sentencias está mal, donde la hija estaba la padre
			*/
			istrstream	iBaseStream( (char*) pccBaseSentence ) ;
			istrstream	iStream( (char*) pccSentence ) ;
			ostrstream	oStream ;

			iResult = ai_poHsParser->SentenceMerge( iStream, iBaseStream, oStream ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_BAD_SENTENCE_MERGE, GetTiId() << pccBaseSentence << pccSentence ) ;

			pccBaseGroupObjects = m_poSentence->GetGroupObjects() ;

			if( m_iOwnSentence == 1 )
			{
				delete( m_poSentence ) ;
				m_poSentence = NULL ;
				m_iOwnSentence = 0 ;
			}

			m_poSentence = new ClTempSentence ;
			m_iOwnSentence = 1 ;
			CHECK_CHLOG_ERRORF( m_poSentence == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClTempSentence ) ) << __LINE__ << __FILE__ ) ;
			
			m_poSentence->SetUpperPool( m_poUpperPool ) ;
			m_poSentence->SetNoUpperPool( m_poNoUpperPool ) ;

			pccNewSentence = oStream.str() ;
			iResult = m_poSentence->SetApiSql1( pccNewSentence ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			m_poSentence->SetSentenceId( m_iTiId ) ;

			// Bug 0281717 Se asigna el GroupObjects de la hija o en su defecto el de la padre
			pccGroupObjects = poSentence->GetGroupObjects() ;

			if( pccGroupObjects != NULL && *pccGroupObjects != '\0' )
			{
				m_poSentence->SetGroupObjects( pccGroupObjects ) ;
			}
			else if( pccBaseGroupObjects != NULL && *pccBaseGroupObjects != '\0' )
			{
				m_poSentence->SetGroupObjects( pccBaseGroupObjects ) ;
			}
		}
	}


	return( M4_SUCCESS ) ;
}
