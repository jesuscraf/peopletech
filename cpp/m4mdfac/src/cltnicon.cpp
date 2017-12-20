
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltnicon.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary connector
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltnicon.hpp"
#include "cltnitem.hpp"
#include "clttrig.hpp"
#include "cltconn.hpp"
#include "cltnode.hpp"
#include "m4mdrt.hpp"





int M4ClCompareArgumentMappingByOrder( m4pcvoid_t ai_pcvArgMapping1, m4pcvoid_t ai_pcvArgMapping2 )
{
	return( ( *( StArgumentMappig_t** )ai_pcvArgMapping1 )->m_iSonRealOrder - ( *( StArgumentMappig_t** )ai_pcvArgMapping2 )->m_iSonRealOrder ) ;
}





//=================================================================================
//
// ClTempNitemConn
//
// Clase que implementa la definición de los conectores de item
//
//=================================================================================

m4return_t  ClTempNitemConn :: _Delete( void )
{

	m4return_t			    iResult ;
	m4uint16_t			    i ;
	m4uint16_t			    iLength ;
	m4pvoid_t			    pvPointer ;
    StArgumentMappig_t      *pstArgumentMapping ;

    
    iLength = m_oArgumentMappings.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oArgumentMappings.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pstArgumentMapping = ( StArgumentMappig_t* ) pvPointer ;
        delete( pstArgumentMapping ) ;
	}

    return( M4_SUCCESS ) ;
}


// Funciones de inicialización ================================================

ClTempNitemConn :: ClTempNitemConn( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

    m_iPrecedence = 0 ;
    m_iSpin = 0 ;
    m_iRelationship = 0 ;
    m_iContext = 0 ;
	m_iCsType = M4CL_CSTYPE_BOTH ;

	m_iIsOwn = 0 ;
    m_iCallOrder = 0 ;
	m_iIsOverWritten = 0 ;
    m_poConnector = NULL ;
    m_poSonNitem = NULL ;
    m_poFatherNitem = NULL ;

	m_iIsToCompile = 1 ;

	m_acFatherNodeId[ 0 ] = '\0' ;
	m_acFatherNodeId[ 1 ] = '\0' ;
	m_acFatherItemId[ 0 ] = '\0' ;
	m_acFatherItemId[ 1 ] = '\0' ;

    m_oArgumentMappings.SetDelta( 1 ) ;
}


ClTempNitemConn :: ~ClTempNitemConn( void )
{
	_Delete() ;
}


// Otras funciones de lectura ================================================

StArgumentMappig_t* ClTempNitemConn :: GetArgMappingByPosition( m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvPointer ;


    iResult = m_oArgumentMappings.GetElement( ai_iPosition, pvPointer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    return( ( StArgumentMappig_t* ) pvPointer ) ;
}



// Otras funciones de escritura ===============================================

m4return_t	ClTempNitemConn :: AddArgMapping( StArgumentMappig_t *ai_pstArgumentMapping )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_pstArgumentMapping == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = m_oArgumentMappings.AddElement( ai_pstArgumentMapping ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}




// Funciones de serialización =================================================

m4return_t	ClTempNitemConn :: PreGetSize( void )
{

	m4uint16_t	iArguments ;
	m4uint16_t	iMappings ;
	ClTempItem	*poSonItem ;


	poSonItem = m_poSonNitem->GetItem() ;
	iArguments = poSonItem->GetNumberOfArguments() ;
	iMappings = m_oArgumentMappings.GetLength() ;
	CHECK_CHLOG_ERRORF( iArguments != iMappings, M4_ERROR, M4_MDFAC_BAD_ARG_MAPPING_NUMBER, LOG_CAT_AND( m_poSonNitem->iItem(), m_poSonNitem->nItem() ) << LOG_CAT_AND( m_poSonNitem->iNode(), m_poSonNitem->nNode() ) << LOG_CAT_AND( m_poSonNitem->iM4Obj(), m_poSonNitem->nM4Obj() ) << iArguments << LOG_CAT_AND( m_poFatherNitem->iItem(), m_poFatherNitem->nItem() ) << LOG_CAT_AND( m_poFatherNitem->iNode(), m_poFatherNitem->nNode() ) << iMappings ) ;

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitemConn :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;
	ao_iSize += M4CL_MCR_NITEM_CONNECTOR_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_ITEM_CON, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClTempNitemConn :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

    m4uint32_t		iOffset ;
	m4uint32_t		iHandle ;

    
    iOffset = m_iHandle ;

// fijos

    if( m_poConnector != NULL )
    {
        iHandle = m_poConnector->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    iHandle = m_poFatherNitem->GetHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    iHandle = m_poSonNitem->GetHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iPrecedence ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iSpin ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iRelationship ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iContext ) ;
	M4ClPackInt8 ( ai_pcBuffer, iOffset, m_iCsType ) ;

    return( M4_SUCCESS ) ;
}



// Varias =====================================================================

void	ClTempNitemConn :: FillConnectorWithTrigger( ClTempTrigger* a_poTrigger )
{
    m_iPrecedence = a_poTrigger->GetPrecedence() ;
    m_iSpin = a_poTrigger->GetSpin() ;
    m_iRelationship = a_poTrigger->GetRelation() ;
    m_iContext = a_poTrigger->GetContext() ;
    m_iCsType = a_poTrigger->GetCsType() ;
    m_iCallOrder = a_poTrigger->GetCallOrder() ;
}



void	ClTempNitemConn :: OrderArgumentMappings( void )
{
	m_oArgumentMappings.Sort( M4ClCompareArgumentMappingByOrder ) ;
}

