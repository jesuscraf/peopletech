
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltitem.cpp   
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
//    This module defines the functions of a temporary item
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltitem.hpp"
#include "cltarg.hpp"
#include "cltiform.hpp"
#include "cltti.hpp"
#include "cltchann.hpp"
#include "chlog.hpp"
#include "cldates.hpp"
#include "m4mdrt.hpp"
#include "m4objglb.hpp"




//=============================================================================
//
// ClTempItem
//
// Clase que implementa la clase de los items temporales
//
//=============================================================================

// Funciones de inicialización ================================================
	
ClTempItem :: ClTempItem( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

    m_dDefaultValue = 0 ;

	m_iItemId = 0 ;
	m_iSynomym = 0 ;
	m_iDmd = 0 ;
	m_iDmdComponent = 0 ;
	m_iDmdCrosstab = 0 ;
	m_iDmdField = 0 ;
    m_iDBPrecision = 0 ;
	m_iPrecision = 0 ;
    m_iMaxSize = 0 ;
    m_iReadObjectAlias = 0;
    m_iWriteObjectAlias = 0 ;
    m_iReadFieldId = 0 ;
    m_iWriteFieldId = 0 ;
    m_iReadObjectId = 0;
    m_iWriteObjectId = 0 ;

    m_iOrder = 0 ;
	m_iFlags = 0 ;

    m_iType = 0 ;
	m_iScope = 0 ;
    m_iM4Type = 0 ;
    m_iCppType = 0 ;
	m_iScale = 0 ;
	m_iSliceBhrv = 0 ;
	m_iSliceSplit = 0 ;
	m_iIdTotalize = 0 ;
	m_iIdSliceTotType = 0 ;
    m_iTransactionMode = 0 ;
    m_iMethodLevel = 0 ;
    m_iSentTotType = 0 ;
    m_iDefaultValueFlag = 0 ;
    m_iInternalType = 0 ;
	m_iConvertFunction = 0 ;
	m_iModifyData = 0 ;

    m_iReferenceNumber = 0 ;

	m_iIsToInclude = 0 ;
	m_iIsEvent = 0 ;

	m_pcName = NULL ;
	m_pcSliceItemId = NULL ;
	m_pcAuxItemId = NULL ;
	m_pcAuxItemProratId = NULL ;
	m_pcGroupId = NULL ;
	m_pcT3Id = NULL ;
	m_pcNodeId = NULL ;

	m_iCsType = M4CL_CSTYPE_MAPPED ;
	m_iSyncType = M4_SYNC_DOCUMENT ;
	m_iIsToCompile = 1 ;
}



ClTempItem :: ~ClTempItem( void )
{
	if( m_pcName != NULL )
	{
		delete( m_pcName ) ;
		m_pcName = NULL ;
	}

	if( m_pcSliceItemId != NULL )
	{
		delete( m_pcSliceItemId ) ;
		m_pcSliceItemId = NULL ;
	}

	if( m_pcAuxItemId != NULL )
	{
		delete( m_pcAuxItemId ) ;
		m_pcAuxItemId = NULL ;
	}

	if( m_pcAuxItemProratId != NULL )
	{
		delete( m_pcAuxItemProratId ) ;
		m_pcAuxItemProratId = NULL ;
	}

	if( m_pcGroupId != NULL )
	{
		delete( m_pcGroupId ) ;
		m_pcGroupId = NULL ;
	}

	if( m_pcT3Id != NULL )
	{
		delete( m_pcT3Id ) ;
		m_pcT3Id = NULL ;
	}

	if( m_pcNodeId != NULL )
	{
		delete( m_pcNodeId ) ;
		m_pcNodeId = NULL ;
	}
}



// Otras funciones de lectura =================================================

ClTempArgument* ClTempItem :: GetArgumentById( m4pcchar_t ai_pccArgumentId )
{

	m4return_t			iResult ;
	m4uint8_t			iFound ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
    m4pcchar_t          pccArgumentId ;
	m4pvoid_t	        pvPointer ;
	ClTempArgument	    *poArgument ;


	CHECK_CHLOG_DEBUGF( ai_pccArgumentId == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iLength = m_oArguments.GetLength() ;

    iFound = 0 ;
	for( i = 0 ; i < iLength && iFound == 0 ; i++ )
	{
		iResult = m_oArguments.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poArgument = ( ClTempArgument* ) pvPointer ;
        pccArgumentId = poArgument->GetArgumentId() ;

		if( strcmpi( pccArgumentId, ai_pccArgumentId ) == 0 )
        {
            iFound = 1 ;
		}
	}

    if( iFound == 0 )
    {
        return( NULL ) ;
    }

	return( poArgument ) ;
}


ClTempArgument* ClTempItem :: GetArgumentByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempArgument  *poResult ;


	iResult = m_oArguments.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempArgument* ) pvElement ;

	return( poResult ) ;
}



// Funciones de escritura de parte fija =======================================

m4return_t	ClTempItem :: SetDefaultValue( m4pcchar_t ai_pccItemDefaultValue, ClTempChannel* ai_poChannel )
{

    m4return_t	iResult ;
    m4uint16_t  iLength ;
    m4uint16_t  iCopy ;
    m4uint32_t  iOffset = 0 ;
	m4pcchar_t	pccString ;
	m4char_t	acString[ M4CL_MAX_VAR_STRING_PRECISION + 1 ] ;


	if( ai_pccItemDefaultValue == NULL )
	{
        m_iDefaultValueFlag = 0 ;
        m_dDefaultValue = 0 ;
	}
    else
    {
        switch( m_iCppType )
        {
            case M4CL_CPP_TYPE_NUMBER :
                m_iDefaultValueFlag = 1 ;
                m_dDefaultValue = atof( ai_pccItemDefaultValue ) ;
                break ;

            case M4CL_CPP_TYPE_STRING :
                m_iDefaultValueFlag = 2 ;

				iLength = strlen( ai_pccItemDefaultValue ) ;
				pccString = ai_pccItemDefaultValue ;

				if( iLength > m_iPrecision )
				{
					if( m_iPrecision > 0 )
					{
						iCopy = m4uint16_t( m_iPrecision ) ;

						if( iCopy > M4CL_MAX_VAR_STRING_PRECISION )
						{
							iCopy = M4CL_MAX_VAR_STRING_PRECISION ;
						}

						memcpy( acString, ai_pccItemDefaultValue, sizeof( m4char_t ) * iCopy ) ;
						*( acString + iCopy ) = '\0' ;
						pccString = acString ;

						if( M4ChLogIsErrorIn( M4_MDFAC_DEFAULT_BIGGER_THAN_PREC ) == 0 )
						{
							DUMP_CHLOG_WARNINGF( M4_MDFAC_DEFAULT_BIGGER_THAN_PREC, m_iPrecision << iLength << ai_pccItemDefaultValue << pccString << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
						}
					}
					else if( m_iPrecision == 0 )
					{
						if( M4ChLogIsErrorIn( M4_MDFAC_DEFAULT_WITH_ZERO_PREC ) == 0 )
						{
							DUMP_CHLOG_WARNINGF( M4_MDFAC_DEFAULT_WITH_ZERO_PREC, iLength << ai_pccItemDefaultValue << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
						}
					}
				}

	            iResult = m_poNoUpperPool->AddString( pccString, iOffset ) ;
				CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

                m_dDefaultValue = iOffset ;
                break ;

            case M4CL_CPP_TYPE_DATE :
                m_iDefaultValueFlag = 1 ;
                ClStringToDate( ai_pccItemDefaultValue, m_dDefaultValue ) ;
                break ;

            default :
                m_iDefaultValueFlag = 0 ;
                m_dDefaultValue = 0 ;
                break ;
        }
    }

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempItem :: SetCppType( ClTempChannel* ai_poChannel )
{

    switch( m_iM4Type )
    {
        case M4CL_M4_TYPE_NULL :

			/* Bug 0072739
			Si es de tipo nulo y no es un método es un error.
			*/
			CHECK_CHLOG_ERRORF( m_iType != M4CL_ITEM_TYPE_METHOD, M4_ERROR, M4_MDFAC_NULL_M4_TYPE, M4ClItemType( m_iType ) << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 ) ;

            m_iCppType = M4CL_CPP_TYPE_NULL ;
            m_iPrecision = 0 ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = 0 ;
            break ;

        case M4CL_M4_TYPE_FIXED_STRING :

			// control de precisión
			if( m4int32_t( m_iDBPrecision ) < 0 )
			{
				if( M4ChLogIsErrorIn( M4_MDFAC_NEGATIVE_DB_PREC ) == 0 )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_NEGATIVE_DB_PREC, m_iDBPrecision << M4ClM4Type( m_iM4Type ) << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
				}

				m_iDBPrecision = 0 ;
			}

            m_iCppType = M4CL_CPP_TYPE_STRING ;
            m_iPrecision = m_iDBPrecision ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = m_iPrecision + 1 ;
            break ;

        case M4CL_M4_TYPE_VAR_STRING :
        case M4CL_M4_TYPE_UNI_VAR_STRING :

			// control de precisión
			if( m4int32_t( m_iDBPrecision ) < 0 )
			{
				if( M4ChLogIsErrorIn( M4_MDFAC_NEGATIVE_DB_PREC ) == 0 )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_NEGATIVE_DB_PREC, m_iDBPrecision << M4ClM4Type( m_iM4Type ) << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
				}

				m_iDBPrecision = 0 ;
			}

			m_iCppType = M4CL_CPP_TYPE_STRING ;
            m_iPrecision = m_iDBPrecision ;

			SetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
			m_iMaxSize = M4_SIZE_OF_DOUBLE ;

/* De momento lo comentamos
// Ojo, que este apaño se hace en SetDefaultValue también
			if( m_iPrecision < 32 ) //lo ponemos a longitud fija
			{
				UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
				m_iMaxSize = m_iPrecision + 1 ;
			}
			else
			{
				SetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
				m_iMaxSize = M4_SIZE_OF_DOUBLE ;
			}
*/
            break ;

        case M4CL_M4_TYPE_LONG :
        case M4CL_M4_TYPE_BINARY_STRING :
        case M4CL_M4_TYPE_UNI_LONG :

            m_iCppType = M4CL_CPP_TYPE_STRING ;
            m_iPrecision = M4CL_MAX_LONG_PRECISION ;
			SetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        case M4CL_M4_TYPE_DATE :

            m_iCppType = M4CL_CPP_TYPE_DATE ;
            m_iPrecision = M4CL_MAX_DATE_PRECISION ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        case M4CL_M4_TYPE_TIMESTAMP :
        case M4CL_M4_TYPE_HOUR :

            m_iCppType = M4CL_CPP_TYPE_DATE ;
            m_iPrecision = M4CL_MAX_TIMESTAMP_PRECISION ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        case M4CL_M4_TYPE_NUMBER :
        case M4CL_M4_TYPE_CURRENCY :
        case M4CL_M4_TYPE_TIME_SPAN :

			// control de precisión
			if( m4int32_t( m_iDBPrecision ) < 0 )
			{
				if( M4ChLogIsErrorIn( M4_MDFAC_NEGATIVE_DB_PREC ) == 0 )
				{
					DUMP_CHLOG_WARNINGF( M4_MDFAC_NEGATIVE_DB_PREC, m_iDBPrecision << M4ClM4Type( m_iM4Type ) << LOG_CAT_AND( iItem(), nItem() ) << m_acTiId + 1 << LOG_CAT_AND( ai_poChannel->iM4Obj(), ai_poChannel->nM4Obj() ) ) ;
				}

				m_iDBPrecision = 0 ;
			}

            m_iCppType = M4CL_CPP_TYPE_NUMBER ;
            m_iPrecision = m_iDBPrecision ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        case M4CL_M4_TYPE_VARIANT :
        case M4CL_M4_TYPE_UNI_VARIANT :

            m_iCppType = M4CL_CPP_TYPE_STRING ;
            m_iPrecision = M4CL_MAX_VARIANT_PRECISION ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4CL_MAX_VARIANT_PRECISION ;
            break ;

        case M4CL_M4_TYPE_NUM_VARIANT :

            m_iCppType = M4CL_CPP_TYPE_NUMBER ;
            m_iPrecision = M4CL_MAX_NUM_VARIANT_PRECISION ;
			UnSetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        case M4CL_M4_TYPE_BLOB :

            m_iCppType = M4CL_CPP_TYPE_STRING ;
            m_iPrecision = M4CL_MAX_BLOB_PRECISION ;
			SetFlag( M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH ) ;
            m_iMaxSize = M4_SIZE_OF_DOUBLE ;
            break ;

        default :
            break ;
    }

    if( m_iType == M4CL_ITEM_TYPE_METHOD )
    {
        m_iMaxSize = 0 ;
    }

	return( M4_SUCCESS ) ;
}


void    ClTempItem :: SetSliceBhrv( m4uint8_t ai_iSliceBhrv )
{
	if( ai_iSliceBhrv == M4CL_ITEM_SLICE_BHRV_INCIDENCE )
	{
		UnSetFlag( M4CL_MCR_ITEM_FLAG_GENERATE_SLICES ) ;
		ai_iSliceBhrv = M4CL_ITEM_SLICE_BHRV_FINAL_VALUE ;
	}
	else
	{
		SetFlag( M4CL_MCR_ITEM_FLAG_GENERATE_SLICES ) ;
	}

	m_iSliceBhrv = ai_iSliceBhrv ;
}



// Funciones de impresión =====================================================

m4pcchar_t	ClTempItem :: iItem( void ) const
{
	return( GetItemId() ) ;
}


m4pcchar_t	ClTempItem :: nItem( void ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetName() ;

	if( *pccResult == '\0' )
	{
		pccResult = GetItemId() ;
	}

	return( pccResult ) ;
}



// Funciones de referencia ====================================================

void	ClTempItem :: AddArgument( ClTempArgument *ai_poItemArgument )
{

	m4uint8_t	iType ;


	m_oArguments.AddElement() ;

	iType = ai_poItemArgument->GetType() ;

	if( iType == M4CL_ITEM_ARGUMENT_TYPE_REFERENCE )
	{
		m_iReferenceNumber++ ;
	}

}



// Funciones de serialización =================================================

m4return_t	ClTempItem :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

	m4uint16_t  iLength ;


// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_8( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_ITEM_FIXED_SIZE ;


// Variables ==================================================================

	iLength = m_oArguments.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * M4CL_MCR_ITEM_ARGUMENT_SIZE ;


	if( ai_poStatistics != NULL )
	{
		switch( m_iType )
		{
			case M4CL_ITEM_TYPE_METHOD :
				ai_poStatistics->AddValue( M4MDRT_STT_ITEM_METHOD, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_PROPERTY :
				ai_poStatistics->AddValue( M4MDRT_STT_ITEM_PROPERTY, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_FIELD :
				ai_poStatistics->AddValue( M4MDRT_STT_ITEM_FIELD, ao_iSize - m_iHandle ) ;
				break ;

			case M4CL_ITEM_TYPE_CONCEPT :
				ai_poStatistics->AddValue( M4MDRT_STT_ITEM_CONCEPT, ao_iSize - m_iHandle ) ;
				break ;

			default :
				break ;
		}
	}

	return( M4_SUCCESS ) ;
}



m4return_t	ClTempItem :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

	m4return_t			iResult ;
	m4uint8_t			iType ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4uint16_t			iPrecision ;
	m4uint32_t			iOffset ;
	m4uint32_t			iHandle ;
	m4pvoid_t		    pvPointer ;
	ClTempArgument	    *poArgument ;


	iOffset = m_iHandle ;

// fijos

    if( m_iDefaultValueFlag == 2 )
    {
        // es una cadena
	    M4ClPackInt32( ai_pcBuffer, iOffset, m4uint32_t( m_dDefaultValue ) + ai_iBufferSize + ai_iPoolSize ) ;
	    M4ClPackInt32( ai_pcBuffer, iOffset, 0 ) ;
    }
    else
    {
	    M4ClPackDouble( ai_pcBuffer, iOffset, m_dDefaultValue ) ;
    }

    M4ClPackInt32( ai_pcBuffer, iOffset, m_iItemId + ai_iBufferSize ) ;

	iHandle = m_poTi->GetTiIdHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDmd + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDmdComponent + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDmdCrosstab + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDmdField + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDBPrecision ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iPrecision ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iMaxSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iReadObjectAlias + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iWriteObjectAlias + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iReadFieldId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iWriteFieldId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iReadObjectId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iWriteObjectId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSynomym ) ;

	M4ClPackInt16( ai_pcBuffer, iOffset, m_iOrder ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iFlags ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iScope ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iM4Type ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iCppType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iScale ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iSliceBhrv ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iSliceSplit ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIdTotalize ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iIdSliceTotType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iTransactionMode ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iMethodLevel ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iSentTotType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iDefaultValueFlag ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iInternalType ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iConvertFunction ) ;
	M4ClPackInt8( ai_pcBuffer, iOffset, m_iModifyData ) ;

    
	iLength = m_oArguments.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iReferenceNumber ) ;

// variables

// argumentos

    iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oArguments.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poArgument = ( ClTempArgument* ) pvPointer ;

        iHandle = poArgument->GetArgumentIdHandle() ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize ) ;

        iHandle = poArgument->GetNameHandle() ;
		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle + ai_iBufferSize + ai_iPoolSize ) ;

        iPrecision = poArgument->GetPrecision() ;
		M4ClPackInt16( ai_pcBuffer, iOffset, iPrecision ) ;

        iType = poArgument->GetType() ; ;
		M4ClPackInt8( ai_pcBuffer, iOffset, iType ) ;

        iType = poArgument->GetM4Type() ; ;
		M4ClPackInt8( ai_pcBuffer, iOffset, iType ) ;

		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}

	return( M4_SUCCESS ) ;
}




