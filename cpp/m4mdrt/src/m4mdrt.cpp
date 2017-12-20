
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrt.cpp    
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
//    Este modulo define las funciones del metacanal compilado
//
//
//==============================================================================

#include "m4mdrt.hpp"
#include "m4mdrtre.hpp"

#include "m4objglb.hpp"
#include "cllstr.hpp"
#include "m4logsys.hpp"
#include "mmmanager.hpp"


/* de momento no porque el API siempre debe ser inline
#ifndef	_M4_USING_INLINE
#include "m4mdrt.inl"
#endif
*/



// Máxima longitud del mensage de código generado y de los números
#define	M4MDRT_MAX_DECODE_BUFFER_SIZE						255



//=================================================================================
// Funciones de búsqueda
//=================================================================================

int M4ClMCRCompareIndexesById( m4pcvoid_t ai_pcvIndex1, m4pcvoid_t ai_pcvIndex2 )
{
    return( *( ( m4uint32_t* ) ai_pcvIndex1 ) - *( ( m4uint32_t* ) ai_pcvIndex2 ) ) ;
}


int M4ClMCRCompareOneStringById( m4pcvoid_t ai_pcvString1, m4pcvoid_t ai_pcvString2 )
{

	m4pcchar_t	pccString1 = ( m4pchar_t ) ai_pcvString1 + *( ( m4uint32_t* ) ai_pcvString1 ) - 1 ;
	m4pcchar_t	pccString2 = ( m4pchar_t ) ai_pcvString2 + *( ( m4uint32_t* ) ai_pcvString2 ) - 1 ;


	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}

    m4uchar_t uiSize1 = *pccString1 ;
	return( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ) ;
}


int M4ClMCRComparePlugsByIds( m4pcvoid_t ai_pcvPlug1, m4pcvoid_t ai_pcvPlug2 )
{

    int iResult ;


	m4pcchar_t	pccString1 = ( m4pchar_t ) ai_pcvPlug1 + *( ( m4uint32_t* ) ai_pcvPlug1 ) - 1 ;
	m4pcchar_t	pccString2 = ( m4pchar_t ) ai_pcvPlug2 + *( ( m4uint32_t* ) ai_pcvPlug2 ) - 1 ;

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

	pccString1 = ( m4pchar_t ) ai_pcvPlug1 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvPlug1 + M4CL_MCR_NODE_PLUG_NODE_ID ) ) + M4CL_MCR_NODE_PLUG_NODE_ID - 1 ;
	pccString2 = ( m4pchar_t ) ai_pcvPlug2 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvPlug2 + M4CL_MCR_NODE_PLUG_NODE_ID ) ) + M4CL_MCR_NODE_PLUG_NODE_ID - 1 ;

	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}

	uiSize1 = *pccString1 ;
	return( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ) ;
}


int M4ClMCRCompareDmdsById( m4pcvoid_t ai_pcvDmd1, m4pcvoid_t ai_pcvDmd2 )
{

	int			iResult ;


	m4pcchar_t	pccString1 = ( m4pchar_t ) ai_pcvDmd1 + *( ( m4uint32_t* ) ai_pcvDmd1 ) - 1 ;
	m4pcchar_t	pccString2 = ( m4pchar_t ) ai_pcvDmd2 + *( ( m4uint32_t* ) ai_pcvDmd2 ) - 1 ;

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

	pccString1 = ( m4pchar_t ) ai_pcvDmd1 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvDmd1 + M4CL_MCR_NODE_DMDS_DMD_FIELD ) ) + M4CL_MCR_NODE_DMDS_DMD_FIELD - 1 ;
	pccString2 = ( m4pchar_t ) ai_pcvDmd2 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvDmd2 + M4CL_MCR_NODE_DMDS_DMD_FIELD ) ) + M4CL_MCR_NODE_DMDS_DMD_FIELD - 1 ;

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

	pccString1 = ( m4pchar_t ) ai_pcvDmd1 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvDmd1 + M4CL_MCR_NODE_DMDS_DMD ) ) + M4CL_MCR_NODE_DMDS_DMD - 1 ;
	pccString2 = ( m4pchar_t ) ai_pcvDmd2 + *( ( m4uint32_t* ) ( ( m4pcchar_t ) ai_pcvDmd2 + M4CL_MCR_NODE_DMDS_DMD ) ) + M4CL_MCR_NODE_DMDS_DMD - 1 ;

	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}

	uiSize1 = *pccString1 ;
	return( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) ) ;
}




//=================================================================================
//
// ClLaneMCR
//
// Clase para la información del carril en el metacanal compilado
//
//=================================================================================

ClLaneMCR :: ClLaneMCR( void )
{
	m_iSonNodeHandle = 0 ;
	m_iNodeItems = 0 ;
	m_iBlockItems = 0 ;
	m_iRegisterItems = 0 ;

	*m_acFatherChannelId = '\0' ;
	*m_acFatherNodeId = '\0' ;
	*m_acSonNodeId = '\0' ;

	m_pcLaneInfo = NULL ;
}


ClLaneMCR :: ClLaneMCR( const ClLaneMCR &ai_roLane, ClCompiledMCR* ai_pMCR )
{

	m4uint16_t	iLength ;


	m_iSonNodeHandle = ai_roLane.m_iSonNodeHandle ;
	m_iNodeItems = ai_roLane.m_iNodeItems ;
	m_iBlockItems = ai_roLane.m_iBlockItems ;
	m_iRegisterItems = ai_roLane.m_iRegisterItems ;

	strcpy( m_acFatherChannelId, ai_roLane.m_acFatherChannelId ) ;
	strcpy( m_acFatherNodeId, ai_roLane.m_acFatherNodeId ) ;
	strcpy( m_acSonNodeId, ai_roLane.m_acSonNodeId ) ;

	m_pcLaneInfo = NULL ;

	if( ai_roLane.m_pcLaneInfo != NULL && m_iSonNodeHandle != 0 )
	{
		iLength = ai_pMCR->GetNodeNumberOfItems( m_iSonNodeHandle ) ;

		m_pcLaneInfo = ( m4pchar_t ) new m4uint32_t[ iLength * 2 ] ;

		if( m_pcLaneInfo == NULL )
		{
			DUMP_CHLOG_ERRORF( M4_MDRT_NO_MEMORY, m4uint32_t( iLength * M4CL_MCR_LANE_INFO_SIZE ) << __LINE__ << __FILE__ ) ;
		}

		memcpy( m_pcLaneInfo, ai_roLane.m_pcLaneInfo, iLength * M4CL_MCR_LANE_INFO_SIZE ) ;
	}
}


ClLaneMCR :: ~ClLaneMCR( void )
{
	Delete() ;
}


void	ClLaneMCR :: Delete( void )
{
	m_iSonNodeHandle = 0 ;
	m_iNodeItems = 0 ;
	m_iBlockItems = 0 ;
	m_iRegisterItems = 0 ;

	*m_acFatherChannelId = '\0' ;
	*m_acFatherNodeId = '\0' ;
	*m_acSonNodeId = '\0' ;

	if( m_pcLaneInfo != NULL )
	{
		delete [] m_pcLaneInfo ;
		m_pcLaneInfo= NULL ;
	}
}



//=================================================================================
//
// ClMCRDependentsBase
//
// Clase para la información de los dependientes en el metacanal compilado
//
//=================================================================================

ClMCRDependentsBase :: ClMCRDependentsBase( void )
{
	m_iItemHandle = 0 ;
	m_iSize = 0 ;
	m_iLength = 0 ;
	m_piDependents = NULL ;
}

ClMCRDependentsBase :: ~ClMCRDependentsBase( void )
{
	m_iItemHandle = 0 ;
	m_iSize = 0 ;
	m_iLength = 0 ;

	if( m_piDependents != NULL )
	{
		delete( m_piDependents ) ;
		m_piDependents = NULL ;
	}
}


m4uint32_t	ClMCRDependentsBase :: GetDependent( m4uint16_t ai_iPosition ) const
{
	CHECK_CHLOG_ERRORF( ai_iPosition >= m_iLength, 0, M4_MDRT_INTERNAL_ERROR_WITH_INFO, ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;

	return( m_piDependents[ ai_iPosition ] ) ;
}


m4return_t	ClMCRDependentsBase :: _Increment( void )
{

	m4uint16_t	iNewSize ;
	m4uint32_t	*piTmp ;


	iNewSize = m_iSize + 10 ;

	piTmp = new m4uint32_t[ iNewSize ] ;
	CHECK_CHLOG_ERRORF( piTmp == NULL, M4_ERROR, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4uint32_t ) * m_iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = iNewSize ;

	if( m_piDependents != NULL )
	{
		if( m_iLength > 0 )
		{
			memcpy( piTmp, m_piDependents, sizeof( m4pvoid_t ) * m_iLength ) ;
		}

		delete [] m_piDependents ;
	}

	m_piDependents = piTmp ;
		
	return( M4_SUCCESS ) ;
}



m4return_t	ClMCRDependentsBase :: _AddDependent( m4uint32_t ai_iDependent )
{

    m4return_t  iResult ;


	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

    m_piDependents[ m_iLength++ ] = ai_iDependent ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClMCRDependentsBase :: Calculate( m4uint32_t ai_iItemHandle, const ClCompiledMCR * ai_pMCR )
{

	m4return_t	iResult ;
	m4uint16_t	i, j, k ;
	m4uint16_t	iNodes ;
	m4uint16_t	iItems ;
	m4uint16_t	iConditionants ;
	m4uint32_t	iNode ;
	m4uint32_t	iItem ;
	m4uint32_t	iConditionant ;


	if( ai_iItemHandle == m_iItemHandle )
	{
		return( M4_SUCCESS ) ;
	}

	m_iItemHandle = 0 ;
	m_iLength = 0 ;

	iNodes = ai_pMCR->GetChannelNumberOfNodes() ;

	for( i = 0 ; i < iNodes ; i++ )
	{
		iNode = ai_pMCR->GetChannelNodeHandleByPosition( i ) ;

		iItems = ai_pMCR->GetNodeNumberOfItems( iNode ) ;

		for( j = 0 ; j < iItems ; j++ )
		{
			iItem = ai_pMCR->GetNodeItemHandleByPosition( iNode, j ) ;

			iConditionants = _GetNumberOfElements( iItem, ai_pMCR ) ;

			for( k = 0 ; k < iConditionants ; k++ )
			{
				iConditionant = _GetElementHandleByPosition( iItem, k, ai_pMCR ) ;

				if( iConditionant == ai_iItemHandle )
				{
					iResult = _AddDependent( iItem ) ;

					if( iResult != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}

	m_iItemHandle = ai_iItemHandle ;

	return( M4_SUCCESS ) ;
}

//=================================================================================
//
// ClMCRDependents
//
// Clase para la información de los dependientes correspondientes a los condicionantes
// en el metacanal compilado
//
//=================================================================================

m4uint16_t	ClMCRDependents :: _GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemNumberOfConditionants( ai_iItemHandle ) ;
}

m4uint32_t ClMCRDependents :: _GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemConditionantHandleByPosition( ai_iItemHandle, ai_iPosition ) ;
}


//=================================================================================
//
// ClMCRAssignedDependents
//
// Clase para la información de los dependientes correspondientes a los asignados
// en el metacanal compilado
//
//=================================================================================

m4uint16_t	ClMCRAssignedDependents :: _GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemNumberOfAssigned( ai_iItemHandle ) ;
}

m4uint32_t	ClMCRAssignedDependents :: _GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemAssignedHandleByPosition( ai_iItemHandle, ai_iPosition ) ;
}


//=================================================================================
//
// ClMCRExecutedDependents
//
// Clase para la información de los dependientes correspondientes a los ejecutados
// en el metacanal compilado
//
//=================================================================================

m4uint16_t	ClMCRExecutedDependents :: _GetNumberOfElements( m4uint32_t ai_iItemHandle, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemNumberOfExecuted( ai_iItemHandle ) ;
}

m4uint32_t	ClMCRExecutedDependents :: _GetElementHandleByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iPosition, const ClCompiledMCR *ai_pMCR ) const
{
	return ai_pMCR->GetItemExecutedHandleByPosition( ai_iItemHandle, ai_iPosition ) ;
}





//=================================================================================
//
// ClCMCRWrapper
//
// Clase para el wrapper de metadatos
//
//=================================================================================

ClCMCRWrapper :: ClCMCRWrapper( void )

:	ClPersistiableObject ( 0 )

{
    m_pcBase = NULL ;
	m_poMMManager = NULL ;
	m_pCO = NULL ;
}


ClCMCRWrapper :: ClCMCRWrapper( ClCMCRWrapper * ai_pCMCRWRapper )

:	ClPersistiableObject ( 0 )

{

	m4uint32_t	iSize ;
	m4pchar_t	pcCMCR ;


    m_pcBase = NULL ;
	m_poMMManager = NULL ;
	m_pCO = NULL ;

	if( ai_pCMCRWRapper != NULL )
	{
		if( ai_pCMCRWRapper->m_pcBase != NULL )
		{
			iSize = M4ClUnPackInt32( ai_pCMCRWRapper->m_pcBase, M4CL_MCR_CHANNEL_SIZE ) ;

			pcCMCR = CreateBuffer( iSize ) ;

			if( pcCMCR != NULL )
			{
				memcpy( pcCMCR, ai_pCMCRWRapper->m_pcBase, iSize ) ;
			}
		}
	}
}


ClCMCRWrapper :: ~ClCMCRWrapper( void )
{
	_Reset() ;
}


void	ClCMCRWrapper :: _Reset( void )
{
	if( m_poMMManager != NULL )
	{
		delete( m_poMMManager ) ;
		m_poMMManager = NULL ;
	}
	else if( m_pcBase != NULL )
	{
		delete [] m_pcBase ;
	}
    m_pcBase = NULL ;
}


void ClCMCRWrapper :: Destroy( m4bool_t ai_bRemoveFromCache )
{
	// Si esta en la cache (m_pCO != NULL) quita una referencia al objecto,
	// ademas si ai_bRemoveFromCache es TRUE, se elimina el objeto de la cache y se le pasa a la cola de borrado.
	// Si no esta en la cache, lo destruye directamente.

    if( m_pCO != NULL )
    {
		if( ai_bRemoveFromCache == M4_TRUE )
		{
			( (ClCMCRContainer*) m_pCO )->RemoveObjectFromCache() ;
		}

		( (ClCMCRContainer*) m_pCO )->FreeObjectFromCache() ;
    }
	else
	{
		delete( this ) ;
	}
}


void ClCMCRWrapper :: RemoveFromCache( void )
{
    if( m_pCO != NULL )
    {
		( (ClCMCRContainer*) m_pCO )->RemoveObjectFromCache() ;
	}
}


m4pchar_t	ClCMCRWrapper :: CreateBuffer( m4uint32_t ai_iSize )
{
    
	m4uint32_t	iDoubleSize ;


	_Reset() ;

	// alineamiento a 8
	iDoubleSize = ( ( ai_iSize - 1 ) >> 0x03 ) + 1 ;

	if( iDoubleSize > 0x20000 )
	{
		// Si es más de 1MB se hace con memoria de disco
		m_poMMManager = new ClMappedMemoryManager( iDoubleSize * sizeof( m4double_t ) ) ;
		CHECK_CHLOG_ERRORF( m_poMMManager == NULL, NULL, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( ClMappedMemoryManager ) ) << __LINE__ << __FILE__ ) ;

		m_pcBase = m_poMMManager->GetMemory() ;
	}
	else
	{
		m_pcBase = (m4pchar_t) new m4double_t[ iDoubleSize ] ;
	}

	CHECK_CHLOG_ERRORF( m_pcBase == NULL, NULL, M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4double_t ) * iDoubleSize ) << __LINE__ << __FILE__ ) ;
	memset( m_pcBase, 0, iDoubleSize * sizeof( m4double_t ) ) ;
	return( m_pcBase ) ;
}


m4uint32_t	ClCMCRWrapper :: GetSize( void ) const
{
	if( m_pcBase != NULL )
	{
		return( M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SIZE ) ) ;
	}

    return( 0 ) ;
}




//=================================================================================
//
// ClCMCRWrapper
//
// Clase para el wrapper de seguridad
//
//=================================================================================


ClCSCRWrapper :: ClCSCRWrapper( void ) : ClPersistiableObject ( 0 )
{
    m_pcSecurity = NULL ;
	m_pCO = NULL ;
}


ClCSCRWrapper :: ClCSCRWrapper( ClCSCRWrapper * ai_pCSCRWRapper )

:	ClPersistiableObject ( 0 )

{

	m4uint32_t	iSize  ;


    m_pcSecurity = NULL ;
	m_pCO = NULL ;

	if( ai_pCSCRWRapper )
	{
		if( ai_pCSCRWRapper->m_pcSecurity )
		{
			iSize = M4ClUnPackInt32( ai_pCSCRWRapper->m_pcSecurity, M4CL_SCR_SECURITY_SIZE ) ;
			m_pcSecurity = new m4char_t[ iSize ] ;

			if( m_pcSecurity == NULL )
			{
				DUMP_CHLOG_ERRORF( M4_MDRT_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iSize ) << __LINE__ << __FILE__ ) ;
			}

			memcpy( m_pcSecurity, ai_pCSCRWRapper->m_pcSecurity, iSize ) ;
		}
	}
}


ClCSCRWrapper :: ~ClCSCRWrapper( void )
{
    if( m_pcSecurity != NULL )
    {
        delete [] m_pcSecurity ;
        m_pcSecurity = NULL ;
    }
}


void ClCSCRWrapper :: Destroy( m4bool_t ai_bRemoveFromCache )
{
	// Si esta en la cache (m_pCO != NULL) quita una referencia al objecto,
	// ademas si ai_bRemoveFromCache es TRUE, se elimina el objeto de la cache y se le pasa a la cola de borrado.
	// Si no esta en la cache, lo destruye directamente.

    if( m_pCO != NULL )
    {
		if( ai_bRemoveFromCache == M4_TRUE )
		{
			( (ClCSCRContainer*) m_pCO )->RemoveObjectFromCache() ;
		}

		( (ClCSCRContainer*) m_pCO )->FreeObjectFromCache() ;
    }
	else
	{
		delete( this ) ;
	}
}


void ClCSCRWrapper :: RemoveFromCache( void )
{
    if( m_pCO != NULL )
    {
		( (ClCSCRContainer*) m_pCO )->RemoveObjectFromCache() ;
	}
}


m4uint32_t	ClCSCRWrapper :: GetSize( void ) const
{
	if( m_pcSecurity != NULL )
	{
		return( M4ClUnPackInt32( m_pcSecurity, M4CL_SCR_SECURITY_SIZE ) ) ;
	}

    return( 0 ) ;
}


m4date_t  ClCSCRWrapper :: GetVersion( void ) const
{

	m4date_t	dResult ;


	if( m_pcSecurity == NULL )
	{
		dResult = 0 ;
	}
	else
	{
		dResult = M4ClUnPackDate( m_pcSecurity, M4CL_SCR_SECURITY_OWN_VERSION ) ;
	}

    return( dResult ) ;
}


m4date_t  ClCSCRWrapper :: GetT3Version( void ) const
{

	m4date_t	dResult ;


	if( m_pcSecurity == NULL )
	{
		dResult = 0 ;
	}
	else
	{
		dResult = M4ClUnPackDate( m_pcSecurity, M4CL_SCR_SECURITY_T3_VERSION ) ;
	}

    return( dResult ) ;
}




//=================================================================================
//
// ClCompiledMCR
//
// Clase con todo el metacanal compilado
//
//=================================================================================

ClCompiledMCR :: ClCompiledMCR( void )
{
    m_pcBase = NULL ;
	m_pcSecurity = NULL ;

	m_poCMCRWrapper =  NULL ;
	m_poCSCRWrapper = NULL ;

	m_iCacheCsType = M4CL_CSTYPE_UNKNOWN ;
}


ClCompiledMCR :: ClCompiledMCR( ClCompiledMCR * ai_pMCR )

: m_oLane( ai_pMCR->m_oLane, ai_pMCR )

{
    m_pcBase = NULL ;
	m_pcSecurity = NULL ;

	m_poCMCRWrapper =  NULL ;
	m_poCSCRWrapper = NULL ;

	if (ai_pMCR)
	{
		if (ai_pMCR->m_poCMCRWrapper)
		{
			m_poCMCRWrapper = new ClCMCRWrapper( ai_pMCR->m_poCMCRWrapper) ;

			if( m_poCMCRWrapper == NULL )
			{
				DUMP_CHLOG_ERRORF( M4_MDRT_NO_MEMORY, m4uint32_t( m_poCMCRWrapper ) << __LINE__ << __FILE__ ) ;
			}

			m_pcBase = m_poCMCRWrapper->GetBuffer() ;
		}

		if (ai_pMCR->m_poCSCRWrapper)
		{
			m_poCSCRWrapper = new ClCSCRWrapper( ai_pMCR->m_poCSCRWrapper ) ;

			if( m_poCSCRWrapper == NULL )
			{
				DUMP_CHLOG_ERRORF( M4_MDRT_NO_MEMORY, m4uint32_t( m_poCMCRWrapper ) << __LINE__ << __FILE__ ) ;
			}

			m_pcSecurity = m_poCSCRWrapper->GetBuffer() ;
		}
	}

	m_iCacheCsType = M4CL_CSTYPE_UNKNOWN ;
}

    
ClCompiledMCR :: ~ClCompiledMCR( void )
{
	m_pcBase = NULL ;
	m_pcSecurity = NULL ;
}


void ClCompiledMCR :: Destroy( m4bool_t ai_bRemoveFromCache )
{
	// LLama al destroy de los wrappers (que son los dueños de los buffers) y luego se destruye.

    if( m_poCMCRWrapper )
    {
		m_poCMCRWrapper->Destroy( ai_bRemoveFromCache ) ;

		m_poCMCRWrapper = NULL ;
    }

    if( m_poCSCRWrapper )
    {
		m_poCSCRWrapper->Destroy( ai_bRemoveFromCache ) ;

		m_poCSCRWrapper = NULL ;
    }

	delete( this ) ;
}


void ClCompiledMCR :: RemoveFromCache( void )
{
	if( m_poCMCRWrapper )
    {
		m_poCMCRWrapper->RemoveFromCache() ;
    }

    if( m_poCSCRWrapper )
    {
		m_poCSCRWrapper->RemoveFromCache() ;
    }
}


m4return_t	ClCompiledMCR :: SetCMCRWrapper( ClCMCRWrapper *ai_poCMCRWrapper )
{
	if( m_poCMCRWrapper != NULL )
	{
		return( M4_ERROR ) ;
	}

    m_poCMCRWrapper = ai_poCMCRWrapper ;
    m_pcBase = m_poCMCRWrapper->GetBuffer() ;

    return( M4_SUCCESS ) ;
}


m4return_t	ClCompiledMCR :: SetCSCRWrapper( ClCSCRWrapper *ai_poCSCRWrapper )
{
	if( m_poCSCRWrapper != NULL )
	{
		return( M4_ERROR ) ;
	}

    m_poCSCRWrapper = ai_poCSCRWrapper ;
    m_pcSecurity = m_poCSCRWrapper->GetBuffer() ;

    return( M4_SUCCESS ) ;
}


m4bool_t ClCompiledMCR :: IsEqual ( const ClCompiledMCR * ai_poCMCR ) const
{
	if( this == ai_poCMCR )
	{
		return( M4_TRUE ) ;
	}

	if( strcmpi( GetChannelId(), ai_poCMCR->GetChannelId() ) == 0 )
	{
		if( GetCacheCsType() == ai_poCMCR->GetCacheCsType() )
		{
			if( GetChannelLanguage() == ai_poCMCR->GetChannelLanguage() )
			{
				if( strcmpi( GetSecurityMaskId(), ai_poCMCR->GetSecurityMaskId() ) == 0 )
				{
					if( GetChannelVersion() == ai_poCMCR->GetChannelVersion() )
					{
						return( M4_TRUE ) ;
					}
				}
			}
		}
	}

	return( M4_FALSE ) ;
}


m4pcchar_t	ClCompiledMCR :: GetLaneFatherChannelId( void ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 )
	{
		return( m_oLane.m_acFatherChannelId ) ;
	}

	return( NULL ) ;
}


m4pcchar_t	ClCompiledMCR :: GetLaneFatherNodeId( void ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 )
	{
		return( m_oLane.m_acFatherNodeId ) ;
	}

	return( NULL ) ;
}


m4pcchar_t	ClCompiledMCR :: GetLaneSonNodeId( void ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 )
	{
		return( m_oLane.m_acSonNodeId ) ;
	}

	return( NULL ) ;
}


m4return_t  ClCompiledMCR :: ConnectNodeToNode( m4pcchar_t ai_pccNode, ClCompiledMCR *ai_poFatherCMCR, m4pcchar_t ai_pccFatherChannel, m4pcchar_t ai_pccFatherNode, m4uint32_t &ao_riPlugHandle )
{

	m4return_t	iResult ;
	m4uint32_t	iNodeHandle ;
	m4uint32_t	iFatherNodeHandle ;


    ao_riPlugHandle = 0 ;

	CHECK_CHLOG_DEBUGF( ai_pccNode == NULL || ai_poFatherCMCR == NULL || ai_pccFatherNode == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iNodeHandle = GetChannelNodeHandleById( ai_pccNode ) ;
	CHECK_CHLOG_ERRORF( iNodeHandle == 0, M4_ERROR, M4_MDRT_NO_SUCH_NODE_IN_LANE, ai_pccNode << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << ai_pccFatherNode << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;

	iFatherNodeHandle = ai_poFatherCMCR->GetChannelNodeHandleById( ai_pccFatherNode ) ;
	CHECK_CHLOG_ERRORF( iFatherNodeHandle == 0, M4_ERROR, M4_MDRT_NO_SUCH_NODE_IN_LANE, LOG_CAT_AND( iNodeN( iNodeHandle ), nNodeN( iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << ai_pccFatherNode << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;

	iResult = ConnectNodeToNode( iNodeHandle, ai_poFatherCMCR, ai_pccFatherChannel, iFatherNodeHandle, ao_riPlugHandle ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_BAD_CONNECT_LANE, LOG_CAT_AND( ai_poFatherCMCR->iNodeN( iFatherNodeHandle ), ai_poFatherCMCR->nNodeN( iFatherNodeHandle ) ) << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) << LOG_CAT_AND( iNodeN( iNodeHandle ), nNodeN( iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;

    return( M4_SUCCESS ) ;
}


m4return_t  ClCompiledMCR :: ConnectNodeToNode( m4uint32_t ai_iNodeHandle, ClCompiledMCR *ai_poFatherCMCR, m4pcchar_t ai_pccFatherChannel, m4uint32_t ai_iFatherNodeHandle, m4uint32_t &ao_riPlugHandle )
{

    m4uint8_t   iType ;
    m4uint8_t   iFatherScope ;
    m4uint8_t   iScope ;
    m4uint8_t   iIsRealRoot ;
    m4uint8_t   iCsExeType ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
    m4uint32_t	iItemHandle ;
    m4uint32_t  iPlugItem ;
    m4uint32_t  iFatherItemHandle ;
    m4uint32_t  iItemRefHandle ;
    m4uint32_t  iInverseConnector ;
    m4uint32_t  iOffset ;
    m4pcchar_t  pccFatherNodeId ;
    m4pcchar_t  pccFatherItemId ;
    m4pcchar_t  pccSonNodeId ;


    ao_riPlugHandle = 0 ;

	CHECK_CHLOG_DEBUGF( ai_iNodeHandle == 0 || ai_poFatherCMCR == NULL || ai_iFatherNodeHandle == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

// Chequeamos que sea nodo raiz
	iIsRealRoot = GetNodeIsRealRoot( ai_iNodeHandle ) ;
	CHECK_CHLOG_ERRORF( iIsRealRoot == 0, M4_ERROR, M4_MDRT_BAD_LANE_NODE_IN_RT, LOG_CAT_AND( iNodeN( ai_iNodeHandle ), nNodeN( ai_iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( ai_poFatherCMCR->iNodeN( ai_iFatherNodeHandle ), ai_poFatherCMCR->nNodeN( ai_iFatherNodeHandle ) ) << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;

// Chequeamos que no sea proxy
	iCsExeType = GetChannelCsExeType() ;
	CHECK_CHLOG_ERRORF( iCsExeType == M4CL_CSEXETYPE_PROXY, M4_ERROR, M4_MDRT_BAD_LANE_M4OBJ_IN_RT, LOG_CAT_AND( iNodeN( ai_iNodeHandle ), nNodeN( ai_iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( ai_poFatherCMCR->iNodeN( ai_iFatherNodeHandle ), ai_poFatherCMCR->nNodeN( ai_iFatherNodeHandle ) ) << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;


    pccFatherNodeId = ai_poFatherCMCR->GetNodeId( ai_iFatherNodeHandle ) ;
	pccSonNodeId = GetNodeId( ai_iNodeHandle ) ;

    ao_riPlugHandle = GetNodePlugHandleByIds( ai_iNodeHandle, ai_pccFatherChannel, pccFatherNodeId ) ;
	CHECK_CHLOG_ERRORF( ao_riPlugHandle == 0, M4_ERROR, M4_MDRT_NO_SUCH_PLUG_IN_RT, LOG_CAT_AND( iNodeN( ai_iNodeHandle ), nNodeN( ai_iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( ai_poFatherCMCR->iNodeN( ai_iFatherNodeHandle ), ai_poFatherCMCR->nNodeN( ai_iFatherNodeHandle ) ) << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;


// a poner las partes fijas ============

	m_oLane.m_iSonNodeHandle = ai_iNodeHandle ;

	strcpy( m_oLane.m_acFatherChannelId, ai_pccFatherChannel ) ;
	strcpy( m_oLane.m_acFatherNodeId, pccFatherNodeId ) ;
	strcpy( m_oLane.m_acSonNodeId, pccSonNodeId ) ;

	m_oLane.m_iNodeItems = 0 ;
	m_oLane.m_iBlockItems = 0 ;
	m_oLane.m_iRegisterItems = 0 ;


// a poner scopes y conectores inversos ===========

	if( m_oLane.m_pcLaneInfo != NULL )
	{
		delete [] m_oLane.m_pcLaneInfo ;
		m_oLane.m_pcLaneInfo = NULL ;
	}

    iLength = GetNodeNumberOfItems( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		m_oLane.m_pcLaneInfo = ( m4pchar_t ) new m4uint32_t[ iLength * 2 ] ;
		CHECK_CHLOG_ERRORF( m_oLane.m_pcLaneInfo == NULL, M4_ERROR, M4_MDRT_NO_MEMORY, m4uint32_t( iLength * M4CL_MCR_LANE_INFO_SIZE ) << __LINE__ << __FILE__ ) ;

		memset( m_oLane.m_pcLaneInfo, 0, iLength * M4CL_MCR_LANE_INFO_SIZE ) ;


		for( i = 0 ; i < iLength ; i++ )
		{
			iItemHandle = GetNodeItemHandleByPosition( ai_iNodeHandle, i ) ;
			iItemRefHandle = M4ClUnPackInt32( m_pcBase, iItemHandle + M4CL_MCR_NITEM_RITEM ) ;

// metemos el nuevo scope y conector inverso

		    iScope = M4ClUnPackInt8( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_SCOPE ) ;
		    iInverseConnector = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_INVERSE_CONNECTOR ) ;
			// Esto nunca puede pasar porque se controla antes
			CHECK_CHLOG_ERRORF( iScope > M4CL_ITEM_MAX_SCOPE || iScope < 1, M4_ERROR, M4_MDRT_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << iScope ) ;


			iPlugItem = GetPlugItemHandleByItemHandle( ao_riPlugHandle, iItemHandle ) ;

			if( iPlugItem != 0 )
			{
				iType = GetPlugType( ao_riPlugHandle ) ;

				pccFatherItemId = GetPlugItemFatherItemId( iPlugItem ) ;
				iFatherItemHandle = ai_poFatherCMCR->GetNodeItemHandleById( ai_iFatherNodeHandle, pccFatherItemId ) ;
				CHECK_CHLOG_ERRORF( iFatherItemHandle == 0, M4_ERROR, M4_MDRT_NO_SUCH_ITEM_IN_LANE, pccFatherItemId << LOG_CAT_AND( iNodeN( ai_iNodeHandle ), nNodeN( ai_iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << LOG_CAT_AND( ai_poFatherCMCR->iNodeN( ai_iFatherNodeHandle ), ai_poFatherCMCR->nNodeN( ai_iFatherNodeHandle ) ) << LOG_CAT_AND( ai_poFatherCMCR->iM4Obj(), ai_poFatherCMCR->nM4Obj() ) ) ;

				iFatherScope = ai_poFatherCMCR->GetItemScope( iFatherItemHandle ) ;
				// Esto nunca puede pasdar porque se controla antes
				CHECK_CHLOG_ERRORF( iFatherScope > M4CL_ITEM_MAX_SCOPE || iFatherScope < 1, M4_ERROR, M4_MDRT_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << iFatherScope ) ;


				iScope = g_aiScopeArray[ iType - 1 ][ iFatherScope - 1 ][ iScope - 1 ] ;
				iInverseConnector = -1 ;
			}

			iOffset = i * M4CL_MCR_LANE_INFO_SIZE ;
			M4ClPackInt8( m_oLane.m_pcLaneInfo, iOffset, iScope ) ;

			iOffset = M4_ROUND_TO_4( iOffset ) ;
			M4ClPackInt32( m_oLane.m_pcLaneInfo, iOffset, iInverseConnector ) ;


// metemos el numero de items

			switch( iScope )
			{
				case M4CL_ITEM_SCOPE_NODE :

					m_oLane.m_iNodeItems++ ;
					break ;

				case M4CL_ITEM_SCOPE_BLOCK :

					m_oLane.m_iBlockItems++ ;
					break ;

				case M4CL_ITEM_SCOPE_REGISTER :

					m_oLane.m_iRegisterItems++ ;
					break ;
        
				default :

					// Esto nunca puede pasdar porque se controla antes
					DUMP_CHLOG_ERRORF( M4_MDRT_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << iScope ) ;
					return( M4_ERROR ) ;
			}
		}
    }


    return( M4_SUCCESS ) ;
}


m4return_t  ClCompiledMCR :: DisconnectLane( void )
{
	m_oLane.Delete() ;
    return( M4_SUCCESS ) ;
}



// Funciones de Log ===============================

m4pcchar_t  ClCompiledMCR :: iM4Obj( void ) const
{
	return( GetChannelId() ) ;
}


m4pcchar_t  ClCompiledMCR :: nM4Obj( void ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetChannelName() ;

	if( *pccResult == '\0' )
	{
		pccResult = GetChannelId() ;
	}

	return( pccResult ) ;
}


m4pcchar_t  ClCompiledMCR :: iNodeN( m4uint32_t ai_iNodeHandle ) const
{
	return( GetNodeId( ai_iNodeHandle ) ) ;
}


m4pcchar_t  ClCompiledMCR :: nNodeN( m4uint32_t ai_iNodeHandle ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetNodeName( ai_iNodeHandle ) ;

	if( *pccResult == '\0' )
	{
		pccResult = GetNodeId( ai_iNodeHandle ) ;
	}

	return( pccResult ) ;
}


m4pcchar_t  ClCompiledMCR :: iItemI( m4uint32_t ai_iItemHandle ) const
{
	return( GetItemId( ai_iItemHandle ) ) ;
}


m4pcchar_t  ClCompiledMCR :: nItemI( m4uint32_t ai_iItemHandle ) const
{

	m4pcchar_t	pccResult ;


	pccResult = GetItemName( ai_iItemHandle ) ;

	if( *pccResult == '\0' )
	{
		pccResult = GetItemId( ai_iItemHandle ) ;
	}

	return( pccResult ) ;
}


m4pcchar_t  ClCompiledMCR :: iNodeI( m4uint32_t ai_iItemHandle ) const
{
	m4uint32_t	iNodeHandle = GetItemNodeHandle( ai_iItemHandle ) ;

	return( GetNodeId( iNodeHandle ) ) ;
}


m4pcchar_t  ClCompiledMCR :: nNodeI( m4uint32_t ai_iItemHandle ) const
{

	m4pcchar_t	pccResult ;
	m4uint32_t	iNodeHandle = GetItemNodeHandle( ai_iItemHandle ) ;


	pccResult = GetNodeName( iNodeHandle ) ;

	if( *pccResult == '\0' )
	{
		pccResult = GetNodeId( iNodeHandle ) ;
	}

	return( pccResult ) ;
}




// funciones de devolución de datos ====================================



// para el carril

m4uint16_t  ClCompiledMCR :: GetNodeNumberOfNodeItems( m4uint32_t ai_iNodeHandle ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 && m_oLane.m_iSonNodeHandle == ai_iNodeHandle )
	{
		return( m_oLane.m_iNodeItems ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_NODE_NITEM_NUMBER ) ) ;
}


m4uint16_t  ClCompiledMCR :: GetNodeNumberOfBlockItems( m4uint32_t ai_iNodeHandle ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 && m_oLane.m_iSonNodeHandle == ai_iNodeHandle )
	{
		return( m_oLane.m_iBlockItems ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_BLOCK_NITEM_NUMBER ) ) ;
}


m4uint16_t  ClCompiledMCR :: GetNodeNumberOfRegisterItems( m4uint32_t ai_iNodeHandle ) const
{
	if( m_oLane.m_iSonNodeHandle != 0 && m_oLane.m_iSonNodeHandle == ai_iNodeHandle )
	{
		return( m_oLane.m_iRegisterItems ) ;
	}

    return( M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_REGISTER_NITEM_NUMBER ) ) ;
}


m4uint8_t	ClCompiledMCR :: GetItemScope( m4uint32_t ai_iItemHandle ) const
{

    m4uint16_t  iIndex ;
    m4uint32_t  iOffset ;
    m4uint32_t  iNodeHandle ;
    m4uint32_t  iItemRefHandle ;


	if( m_oLane.m_iSonNodeHandle != 0 )
	{
		iNodeHandle = GetItemNodeHandle( ai_iItemHandle ) ;

		if( m_oLane.m_iSonNodeHandle == iNodeHandle )
		{
			iIndex = GetItemIndex( ai_iItemHandle ) ;
			iOffset = iIndex * M4CL_MCR_LANE_INFO_SIZE ;
			return( M4ClUnPackInt8( m_oLane.m_pcLaneInfo, iOffset ) ) ;
		}
	}

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt8( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_SCOPE ) ) ;
}


m4int16_t	ClCompiledMCR :: GetItemArgumentPositionById( m4uint32_t ai_iItemHandle, m4pcchar_t ai_pccArgumentId ) const
{

	m4int16_t	iResult ;
	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4pcchar_t	pccArgumentId ;


	if( ai_pccArgumentId == NULL )
	{
		return( -1 ) ;
	}

	iLength = GetItemNumberOfArguments( ai_iItemHandle ) ;

	iResult = -1 ;
	for( i = 0 ; i < iLength && iResult == -1 ; i++ )
	{
		pccArgumentId = GetItemArgumentIdByPosition( ai_iItemHandle, i ) ;

		if( strcmpi( ai_pccArgumentId, pccArgumentId ) == 0 )
		{
			iResult = i ;
		}
	}

	return( iResult ) ;
}


m4uint32_t	ClCompiledMCR :: GetItemInverseConnector( m4uint32_t ai_iItemHandle ) const
{

    m4uint16_t  iIndex ;
    m4uint32_t  iOffset ;
    m4uint32_t  iNodeHandle ;
    m4uint32_t  iItemRefHandle ;


	if( m_oLane.m_iSonNodeHandle != 0 )
	{
		iNodeHandle = GetItemNodeHandle( ai_iItemHandle ) ;

		if( m_oLane.m_iSonNodeHandle == iNodeHandle )
		{
			iIndex = GetItemIndex( ai_iItemHandle ) ;
			iOffset = iIndex * M4CL_MCR_LANE_INFO_SIZE ;
			return( M4ClUnPackInt32( m_oLane.m_pcLaneInfo, iOffset + M4CL_MCR_LANE_INFO_INVERSE_CONNECTOR ) ) ;
		}
	}

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    return( M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_INVERSE_CONNECTOR ) ) ;
}



// unas funciones auxiliares

m4uint16_t	ClCompiledMCR :: _SetWorkingArea( m4puint32_t ai_piWorkingArea, m4pcchar_t ai_pccString, m4uint16_t ai_iMaxLength, m4bool_t ai_iUpperCase ) const
{

	m4uint8_t	iTruncated = 0 ;
    m4uint16_t  iLength ;
	m4pchar_t	pcStringArea = ( ( m4pchar_t ) ai_piWorkingArea ) + M4CL_MCR_CHANNEL_STRING_AREA ;


    iLength = strlen( ai_pccString ) ;

    if( iLength > ai_iMaxLength )
    {
        iLength = ai_iMaxLength ;
		iTruncated = 1 ;
    }

    *ai_piWorkingArea = M4CL_MCR_CHANNEL_STRING_AREA ;

	*( pcStringArea - 1 ) = m4char_t( iLength ) ;
    memcpy( pcStringArea, ai_pccString, sizeof( m4char_t ) * iLength ) ;
    *( pcStringArea + iLength ) = '\0' ;

	if( ai_iUpperCase == M4_FALSE )
	{
		strupr( pcStringArea ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccString << ai_iMaxLength << pcStringArea ) ;
	}


	return( iLength ) ;
}



// Funciones de decodificación ================================================

ClDecodeInstruction*	ClCompiledMCR :: _GetInstruction( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, m4uint16_t &ai_riFunction ) const
{

	ClDecodeInstruction	*poResult ;
	m4uint32_t			iInstructions ;


	iInstructions = ai_roFormula.GetNumberOfInstructions() ;
	CHECK_CHLOG_ERRORF( ai_iPosition < ai_iStart || ai_iPosition >= iInstructions, NULL, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poResult = ai_roFormula[ ai_iPosition ] ;
	ai_riFunction = poResult->GetExeFunc() ;

	return( poResult ) ;
}



m4return_t	ClCompiledMCR :: _ProcessAssign( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t			iResult ;
	m4uint16_t			iFunction ;
	m4uint32_t			i ;
	m4uint32_t			iRefered ;
	m4uint32_t			iArguments ;
	m4uint32_t			i32Number ;
	m4int32_t			iVariable ;
	m4double_t			dValue ;
	m4pcchar_t			pccString ;
	ClDecodeInstruction	*poInstruction ;

    m4char_t			acIndent[ 11 ] ;


	acIndent[ 0 ] = '\r' ;
	acIndent[ 1 ] = '\n' ;

	for( i = 0 ; i < ai_iIndent ; i++ )
	{
		acIndent[ i + 2 ] = '\t' ;
	}

	acIndent[ i + 2 ] = '\0' ;


// Se comprueba que sea una asignación A0 =====================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_ASSIGNVAR && iFunction != M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iVariable = poInstruction->GetNumVar() ;
	// Si no es 0 es error
	CHECK_CHLOG_ERRORF( iVariable != 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "x = ", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	ai_iPosition-- ;

// Pasamos las instrucciones de los argumentos por referencia A-i (si los hay)

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iFunction == M4CL_VM_FT_ASSIGNVAR || iFunction == M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM )
	{
		i = 0 ;

		iRefered = m4uint32_t( GetItemNumberOfReferedArguments( ai_iItemHandle ) ) ;

		for( i = 0 ; i < iRefered ; i++ )
		{
			poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - i, ai_iStart, iFunction ) ;
			CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			// Si no es una A es error
			CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_ASSIGNVAR && iFunction != M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iVariable = poInstruction->GetNumVar() ;
			// Si no es negativo es error
			CHECK_CHLOG_ERRORF( iVariable >= 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ai_iPosition -= iRefered ;
	}


// Hay que ver lo que viene después ===========================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( iFunction == M4CL_VM_FT_EXECUTEITEM )
	{
		// Lo siguiente es la ejecución de item

		poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		// Si no es una ejecución es error
		CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_EXECUTEITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		i32Number = poInstruction->GetNumItem() ;
		CHECK_CHLOG_ERRORF( i32Number == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccString = GetItemId( i32Number ) ;
		CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		
		iResult = ao_roString.Concat( pccString, ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ao_roString.Concat( "(", ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		iArguments = m4uint32_t( GetItemNumberOfArguments( ai_iItemHandle ) ) ;

		if( iArguments > 0 )
		{
			iResult = ao_roString.Concat( " ", ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		for( i = 0 ; i < iArguments; i++ )
		{
			if( i != 0 )
			{
				iResult = ao_roString.Concat( ", ", ai_iMode ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			}

			pccString = GetItemArgumentIdByPosition( ai_iItemHandle, m4uint16_t( i ) ) ;
			CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( pccString, ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		if( iArguments > 0 )
		{
			iResult = ao_roString.Concat( " ", ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		iResult = ao_roString.Concat( ")..", ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		i32Number = poInstruction->GetNumRegla() ;

		pccString = GetItemFormIdByPosition( ai_iItemHandle, m4uint16_t( i32Number ) ) ;
		CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		
		iResult = ao_roString.Concat( pccString, ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		// Pasamos las instrucciones de los argumentos asignados R-i ==================

		i = 0 ;

		for( i = 0 ; i < iArguments ; i++ )
		{
			poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - ( i + 1 ), ai_iStart, iFunction ) ;
			CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			// Si no es una R es error
			CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_READVAR && iFunction != M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iVariable = poInstruction->GetNumVar() ;
			// Si no es el argumento correcto es error
			CHECK_CHLOG_ERRORF( iVariable != - m4int32_t( i + 1 ), M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}

		ai_iPosition -= iArguments ;
	}
	else if( iFunction == M4CL_VM_FT_PUSHNUM )
	{
		// Lo siguiente es una inicialización de la variable local

		dValue = poInstruction->GetVariantDouble() ;
		// Si no es 0 es error
		CHECK_CHLOG_ERRORF( dValue != 0.0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ao_roString.Concat( "0", ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}
	else
	{
		// Lo siguiente tiene que ser una suma de total

		// La primera instrucción es la lectura de la variable local
		poInstruction = _GetInstruction( ai_roFormula, ai_iStart, ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Si no es una R0 es error
		CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_READVAR && iFunction != M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iVariable = poInstruction->GetNumVar() ;
		// Si no es el la variable 0 es error
		CHECK_CHLOG_ERRORF( iVariable != 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ao_roString.Concat( "x", ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


		// Lo siguiente es la lectura del item que totaliza
		poInstruction = _GetInstruction( ai_roFormula, ai_iStart + 2, ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		CHECK_CHLOG_ERRORF( iFunction != 9 && iFunction != 10, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( iFunction == 9 )
		{
			iResult = ao_roString.Concat( " + ", ai_iMode ) ;
		}
		else
		{
			iResult = ao_roString.Concat( " - ", ai_iMode ) ;
		}

		poInstruction = _GetInstruction( ai_roFormula, ai_iStart + 1, ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// El anterior tiene que ser una lectura de item o ejecución
		CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		i32Number = poInstruction->GetNumItem() ;
		CHECK_CHLOG_ERRORF( i32Number == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		pccString = GetItemId( i32Number ) ;
		CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ao_roString.Concat( pccString, ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		ai_iStart += 2 ;

		// Lo siguiente puede ser o no ser el payback
		if( ai_iStart < ai_iPosition )
		{
			poInstruction = _GetInstruction( ai_roFormula, ai_iStart + 3, ai_iStart, iFunction ) ;
			CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			CHECK_CHLOG_ERRORF( iFunction != 9 && iFunction != 10, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			if( iFunction == 9 )
			{
				iResult = ao_roString.Concat( " + ", ai_iMode ) ;
			}
			else
			{
				iResult = ao_roString.Concat( " - ", ai_iMode ) ;
			}

			poInstruction = _GetInstruction( ai_roFormula, ai_iStart + 2, ai_iStart, iFunction ) ;
			CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_EXECUTEITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			i32Number = poInstruction->GetNumItem() ;
			CHECK_CHLOG_ERRORF( i32Number == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			pccString = GetItemId( i32Number ) ;
			CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( pccString, ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


			poInstruction = _GetInstruction( ai_roFormula, ai_iStart + 1, ai_iStart, iFunction ) ;
			CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
			// El anterior tiene que ser una cadena
			CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHSTRING, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			i32Number = poInstruction->GetVariantInt() ;

			pccString = m_pcBase + i32Number ;
			CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( "( \"", ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( pccString, ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( "\" )", ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			ai_iStart += 3 ;
		}
	}


	// Ver si se han procesado todas las instrucciones
	CHECK_CHLOG_ERRORF( ai_iStart != ai_iPosition, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	return( M4_SUCCESS ) ;
}



m4return_t	ClCompiledMCR :: _ProcessIf( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t			iResult ;
	m4bool_t			bFinish ;
	m4uint8_t			iInsideOr ;
	m4uint8_t			iInsideAnd ;
	m4uint8_t			iStackLength ;
	m4uint16_t			iLength ;
	m4uint16_t			iFunction ;
	m4uint16_t			iAuxFunction ;
	m4uint32_t			i ;
	m4uint32_t			iPosition ;
	m4uint32_t			i32Number ;
	m4uint32_t			iThisNode ;
	m4pcchar_t			pccString ;
	m4double_t			dValue ;
    m4char_t			acString[ M4MDRT_MAX_DECODE_BUFFER_SIZE + 1 ] ;
	ClDecodeInstruction	*poInstruction ;
	ClDecodeInstruction	*poAuxInstruction ;

    m4char_t			acIndent[ 11 ] ;


	acIndent[ 0 ] = '\r' ;
	acIndent[ 1 ] = '\n' ;

	for( i = 0 ; i < ai_iIndent ; i++ )
	{
		acIndent[ i + 2 ] = '\t' ;
	}

	acIndent[ i + 2 ] = '\0' ;

	iThisNode = GetItemNodeHandle( ai_iItemHandle ) ;


// Se comprueba que sean un if ================================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_CONDITIONALJUMP, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "if( ", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iInsideOr = 0 ;
	iInsideAnd = 0 ;
	iStackLength = 0 ;

	iPosition = ai_iPosition ;

	for( i = ai_iStart ; i < iPosition; i++ )
	{
		ai_iPosition-- ;

		// Instrucción actual
		poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		// Si es un AND
		if( iFunction == 0 )
		{
			iInsideAnd++ ;
		}
		else if( iFunction == 1 )
		{
			// Si es un OR
			iInsideOr++ ;
		}
		else
		{
			switch( iFunction )
			{
				// Comparación
				case	3 :
				case	4 :
				case	5 :
				case	6 :
				case	7 :
				case	8 :

					// Si hay algo en la pila y hay or por procesar se pone el OR
					if( iStackLength > 0 && iInsideOr > 0 )
					{
						ao_roString.Concat( " OR ", ai_iMode ) ;
						CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						iInsideOr-- ;
					}
					else
					{
						iStackLength++ ;
					}


					// El segundo anterior
					poAuxInstruction = _GetInstruction( ai_roFormula, ai_iPosition - 2, ai_iStart, iAuxFunction ) ;
					CHECK_CHLOG_ERRORF( poAuxInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					// Tiene que ser lectura de item
					CHECK_CHLOG_ERRORF( iAuxFunction != M4CL_VM_FT_PUSHITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					i32Number = poAuxInstruction->GetNumNodo() ;
					CHECK_CHLOG_ERRORF( i32Number == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					if( i32Number != iThisNode )
					{
						pccString = GetNodeId( i32Number ) ;
						CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						ao_roString.Concat( pccString, ai_iMode ) ;
						CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						ao_roString.Concat( ".", ai_iMode ) ;
						CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					}

					i32Number = poAuxInstruction->GetNumItem() ;
					CHECK_CHLOG_ERRORF( i32Number == 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					pccString = GetItemId( i32Number ) ;
					CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					ao_roString.Concat( pccString, ai_iMode ) ;
					CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


					// Ponemos la condición
					switch( iFunction )
					{
						case	3 :

							ao_roString.Concat( " = ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;

						case	4 :

							ao_roString.Concat( " <> ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;

						case	5 :

							ao_roString.Concat( " > ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;

						case	6 :

							ao_roString.Concat( " >= ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;

						case	7 :

							ao_roString.Concat( " < ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;

						case	8 :

							ao_roString.Concat( " <= ", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							break ;
					}

					
					// El anterior
					poAuxInstruction = _GetInstruction( ai_roFormula, ai_iPosition - 1, ai_iStart, iAuxFunction ) ;
					CHECK_CHLOG_ERRORF( poAuxInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					// Tiene que ser una constante
					CHECK_CHLOG_ERRORF( iAuxFunction != M4CL_VM_FT_PUSHNUM && iAuxFunction != M4CL_VM_FT_PUSHSTRING && iAuxFunction != M4CL_VM_FT_PUSHDATE, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

					switch( iAuxFunction )
					{
						case	M4CL_VM_FT_PUSHNUM :

							dValue = poAuxInstruction->GetVariantDouble() ;

							sprintf( acString, "%.8f", dValue ) ;

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
										acString[ iLength ] = '\0' ;
									}
									bFinish = M4_TRUE ;
								}
							}

							iResult = ao_roString.Concat( acString, ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							break ;

						case	M4CL_VM_FT_PUSHSTRING :

							i32Number = poAuxInstruction->GetVariantInt() ;

							pccString = m_pcBase + i32Number ;
							CHECK_CHLOG_ERRORF( pccString == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							iResult = ao_roString.Concat( "\"", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							iResult = ao_roString.Concat( pccString, ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							iResult = ao_roString.Concat( "\"", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							break ;

						case	M4CL_VM_FT_PUSHDATE :

							dValue = poAuxInstruction->GetVariantDouble() ;

							if( dValue - m4uint32_t( dValue ) != 0 )
							{
								i32Number = M4_SECOND_END ;
							}
							else
							{
								i32Number = M4_DAY_END ;
							}
							ClDateToString( dValue, acString, i32Number + 1 ) ;

							iResult = ao_roString.Concat( "{", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							iResult = ao_roString.Concat( acString, ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							iResult = ao_roString.Concat( "}", ai_iMode ) ;
							CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

							break ;

						default :

							DUMP_CHLOG_ERRORF( M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
							return( M4_ERROR ) ;
							break ;
					}


					// Si hay algo en la pila y hay and por procesar sin ors se pone el AND
					if( iStackLength > 0 && iInsideAnd > 0 && iInsideOr == 0 )
					{
						ao_roString.Concat( " ) AND ( ", ai_iMode ) ;
						CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

						iInsideAnd-- ;
						iStackLength-- ;
					}

					ai_iPosition -= 2 ;
					i += 2 ;
					break ;

				default :

					DUMP_CHLOG_ERRORF( M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
					return( M4_ERROR ) ;
					break ;
			}
		}
	}


	iResult = ao_roString.Concat( " ) then", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	// Ver si se han procesado todas las instrucciones
	CHECK_CHLOG_ERRORF( iInsideAnd != 0 || iInsideOr != 0 || iStackLength != 1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( ai_iStart != ai_iPosition, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
	return( M4_SUCCESS ) ;
}



m4return_t	ClCompiledMCR :: _ProcessCall( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t			iResult ;
	m4uint16_t			iFunction ;
	m4uint32_t			i ;
	m4int32_t			iValue ;
	m4double_t			dValue ;
	m4pcchar_t			pccValue ;
	ClDecodeInstruction	*poInstruction ;

    m4char_t			acIndent[ 11 ] ;


	acIndent[ 0 ] = '\r' ;
	acIndent[ 1 ] = '\n' ;

	for( i = 0 ; i < ai_iIndent ; i++ )
	{
		acIndent[ i + 2 ] = '\t' ;
	}

	acIndent[ i + 2 ] = '\0' ;


// Se comprueba que sea un call ===============================================

	/* Bug 0123677
	Cambia la función a la que se llama
	*/
	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != 372, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "Call( \"", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Tienen que quedar 3 por procesar ============================================

	CHECK_CHLOG_ERRORF( ai_iPosition != ai_iStart + 3, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
// La anterior tiene que ser un 2 por el número de argumentos ==================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - 1, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHNUM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	dValue = poInstruction->GetVariantDouble() ;
	CHECK_CHLOG_ERRORF( dValue != 2.0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// 3 antes tiene que ser el nombre del nodo ===================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - 3, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHSTRING, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iValue = poInstruction->GetVariantInt() ;

	pccValue = m_pcBase + iValue ;
	CHECK_CHLOG_ERRORF( pccValue == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( pccValue, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "\", \"", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	
// 2 antes tiene que ser el nombre del item ===================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - 2, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHSTRING, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iValue = poInstruction->GetVariantInt() ;

	pccValue = m_pcBase + iValue ;
	CHECK_CHLOG_ERRORF( pccValue == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( pccValue, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "\" )", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	return( M4_SUCCESS ) ;
}



m4return_t	ClCompiledMCR :: _ProcessEnd( ClDecodeFormula &ai_roFormula, m4uint32_t ai_iPosition, m4uint32_t ai_iStart, ClLongString &ao_roString, m4uint32_t ai_iItemHandle, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t			iResult ;
	m4uint16_t			iFunction ;
	m4uint32_t			i ;
	m4uint32_t			iRefered ;
	m4int32_t			iVariable ;
	m4double_t			dValue ;
	ClDecodeInstruction	*poInstruction ;

    m4char_t			acIndent[ 11 ] ;


	acIndent[ 0 ] = '\r' ;
	acIndent[ 1 ] = '\n' ;

	for( i = 0 ; i < ai_iIndent ; i++ )
	{
		acIndent[ i + 2 ] = '\t' ;
	}

	acIndent[ i + 2 ] = '\0' ;


// Se comprueba que sean un final =============================================

	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_READCOMMENT, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Pasamos las instrucciones de los argumentos por referencia R-i =============

	i = 0 ;

	iRefered = m4uint32_t( GetItemNumberOfReferedArguments( ai_iItemHandle ) ) ;

	for( i = 0 ; i < iRefered ; i++ )
	{
		poInstruction = _GetInstruction( ai_roFormula, ai_iPosition - ( i + 1 ), ai_iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		// Si no es una R es error
		CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_READVAR && iFunction != M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_INVERSE_ERROR, "" << LOG_CAT_AND( iItemI( ai_iItemHandle ), nItemI( ai_iItemHandle ) ) << LOG_CAT_AND( iNodeI( ai_iItemHandle ), nNodeI( ai_iItemHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << M4CL_MINUS_INFINITE_IN_JULIAN << M4CL_PLUS_INFINITE_IN_JULIAN << __LINE__ << __FILE__ ) ;

		iVariable = poInstruction->GetNumVar() ;
		// Si no es negativo es error
		CHECK_CHLOG_ERRORF( iVariable >= 0, M4_ERROR, M4_MDRT_INTERNAL_INVERSE_ERROR, "" << LOG_CAT_AND( iItemI( ai_iItemHandle ), nItemI( ai_iItemHandle ) ) << LOG_CAT_AND( iNodeI( ai_iItemHandle ), nNodeI( ai_iItemHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << M4CL_MINUS_INFINITE_IN_JULIAN << M4CL_PLUS_INFINITE_IN_JULIAN << __LINE__ << __FILE__ ) ;
	}

	ai_iPosition -= iRefered + 1 ;


	poInstruction = _GetInstruction( ai_roFormula, ai_iPosition, ai_iStart, iFunction ) ;
	CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Si es un return( 0 ) es de un call =========================================

	if( iFunction == M4CL_VM_FT_PUSHNUM )
	{
		dValue = poInstruction->GetVariantDouble() ;
		// Si no es 0 es error
		CHECK_CHLOG_ERRORF( dValue != 0.0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		iResult = ao_roString.Concat( "return( 0 )", ai_iMode ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		return( M4_SUCCESS ) ;
	}


// Si es un return( x ) es de metaregla o total, tendrá un R0 y acabamos ======

	CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_READVAR && iFunction != M4CL_VM_FT_READFIXEDARGUMENTINVARARGSITEM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iVariable = poInstruction->GetNumVar() ;

	// Si no es 0 es error
	CHECK_CHLOG_ERRORF( iVariable != 0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "return( x )", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



m4return_t	ClCompiledMCR :: _GetCodeSource( m4pcchar_t ai_pccCode, m4uint32_t ai_iItemHandle, ClLongString &ao_roString, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t			iResult ;
	m4bool_t			bAutoGenerated ;
	m4bool_t			bInsideCode ;
	m4bool_t			bProcessFormula ;
	m4uint8_t			iTab ;
	m4uint8_t			iLanguage ;
	m4uint8_t			iType ;
	m4uint16_t			iFunction ;
	m4uint32_t			i ;
	m4uint32_t			iLength ;
	m4uint32_t			iStart ;
	m4uint32_t			iSize ;
	m4int32_t			iVariable ;
	m4int32_t			iEndIf ;
	m4int32_t			iCommentLine ;
	m4int32_t			iLastCommentLine ;
	m4double_t			dValue ;
	m4char_t			acMessage[ M4MDRT_MAX_DECODE_BUFFER_SIZE + 1 ] ;
	ClDecodeFormula		oForm ;
	ClDecodeInstruction	*poInstruction ;

    m4char_t			acIndent[ 11 ] ;


	acIndent[ 0 ] = '\r' ;
	acIndent[ 1 ] = '\n' ;

	for( i = 0 ; i < ai_iIndent ; i++ )
	{
		acIndent[ i + 2 ] = '\t' ;
	}

	acIndent[ i + 2 ] = '\0' ;


	// Si no tiene fórmula hay que retornar error
	CHECK_CHLOG_ERRORF( ai_pccCode == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	oForm.Init( ai_pccCode ) ;

	bAutoGenerated = oForm.GetIsAutogenerated() ;

	iType = oForm.GetType() ;

	// Si no es autogenerada
	CHECK_CHLOG_ERRORF( bAutoGenerated == M4_FALSE, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( acIndent + 2, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "//==================================================================================", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "// ", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iLanguage = GetChannelLanguage() ;

	iResult = FormatErrorCode( M4_MDRT_AUTOGENEATED_CODE, NULL, 1, acMessage, M4MDRT_MAX_DECODE_BUFFER_SIZE, &iSize, iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		iResult = ao_roString.Concat( "This code has been created automatically by the application.", ai_iMode ) ;
	}
	else
	{
		iResult = ao_roString.Concat( acMessage, ai_iMode ) ;
	}
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "//==================================================================================", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	iLength = oForm.GetNumberOfInstructions() ;

	iEndIf = -1 ;
	iStart = 0 ;
	iLastCommentLine = 1 ;
	bInsideCode = M4_FALSE ;

	for( i = 0 ; i < iLength ; i++ )
	{
		// Si es la linea de final de if la ponemos y reseteamos el if
		if( iEndIf == m4int32_t( i ) )
		{
			iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_roString.Concat( "endif", ai_iMode ) ;
			CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iEndIf = -1 ;
		}

		poInstruction = _GetInstruction( oForm, i, iStart, iFunction ) ;
		CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( bInsideCode == M4_TRUE )
		{
			bProcessFormula = M4_TRUE ;
		}
		else
		{
			bProcessFormula = M4_FALSE ;
		}

		// Si viene un comentario hay que ver si son los que marcan
		// el comienzo y el final de la fórmula
		if( iFunction == M4CL_VM_FT_READCOMMENT )
		{
			iCommentLine = poInstruction->GetNumVar() ;

			if( iCommentLine == iLastCommentLine + 4 )
			{
				// Si se saltan 4 líneas es un delimitador de fórmula
				if( bInsideCode == M4_TRUE )
				{
					// Si estamos dentro se sale
					bInsideCode = M4_FALSE ;
				}
				else
				{
					// Si estamos fuera se entra y se continua
					bInsideCode = M4_TRUE ;
				}
			}
			else
			{
				// Cualquier otro comentario se ignora
				bProcessFormula = M4_FALSE ;
			}

			iLastCommentLine = iCommentLine ;
		}

		if( bProcessFormula == M4_FALSE )
		{
			// Si so ne procesa se continua
			iStart++ ;
			continue ;
		}

		switch( iFunction )
		{
			case	M4CL_VM_FT_ASSIGNVAR :
			case	M4CL_VM_FT_ASSIGNFIXEDARGUMENTINVARARGSITEM :

				iVariable = poInstruction->GetNumVar() ;

				if( iVariable != 0 )
				{
					continue ;
				}

				if( iEndIf != -1 )
				{
					iTab = 1 ;
				}
				else
				{
					iTab = 0 ;
				}

				iResult = _ProcessAssign( oForm, i , iStart, ao_roString, ai_iItemHandle, ai_iMode, ai_iIndent + iTab ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				iStart = i + 1 ;
				break ;

			case	M4CL_VM_FT_CONDITIONALJUMP :

				// No se puede estar dentro de un if
				CHECK_CHLOG_ERRORF( iEndIf != -1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iEndIf = poInstruction->GetNumVar() + i ;

				iResult = _ProcessIf( oForm, i, iStart, ao_roString, ai_iItemHandle, ai_iMode, ai_iIndent ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				iStart = i + 1 ;
				break ;

			// Viene un BeginTransaction
			case	89 :

				// No se puede estar dentro de un if
				CHECK_CHLOG_ERRORF( iEndIf != -1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Tiene que quedar 0 por procesar
				CHECK_CHLOG_ERRORF( i != iStart, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ao_roString.Concat( "BeginTransaction()", ai_iMode ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iStart = i + 1 ;
				break ;

			// Viene un EndTransaction
			case	90 :

				// No se puede estar dentro de un if
				CHECK_CHLOG_ERRORF( iEndIf != -1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// Tiene que quedar 1 por procesar
				CHECK_CHLOG_ERRORF( i != iStart + 1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				// La anterior tiene que ser un 1 por el commit
				poInstruction = _GetInstruction( oForm, i - 1, iStart, iFunction ) ;
				CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PUSHNUM, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				dValue = poInstruction->GetVariantDouble() ;
				CHECK_CHLOG_ERRORF( dValue != 1.0, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = ao_roString.Concat( "EndTransaction( M4_COMMIT )", ai_iMode ) ;
				CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iStart = i + 1 ;
				break ;

			// Viene un Call
			case	372 :

				// No se puede estar dentro de un if
				CHECK_CHLOG_ERRORF( iEndIf != -1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = _ProcessCall( oForm, i, iStart, ao_roString, ai_iItemHandle, ai_iMode, ai_iIndent ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				// La siguiente tiene que ser el purge
				i++ ;
				poInstruction = _GetInstruction( oForm, i, iStart, iFunction ) ;
				CHECK_CHLOG_ERRORF( poInstruction == NULL, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
				CHECK_CHLOG_ERRORF( iFunction != M4CL_VM_FT_PURGESTACK, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iStart = i + 1 ;
				break ;

			// Este case representa el final del código
			case	M4CL_VM_FT_READCOMMENT :

				// No se puede estar dentro de un if
				CHECK_CHLOG_ERRORF( iEndIf != -1, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

				iResult = _ProcessEnd( oForm, i, iStart, ao_roString, ai_iItemHandle, ai_iMode, ai_iIndent ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				iStart = i + 1 ;
				break ;

			default :
				break ;
		}
	}

	// Si no hemos procesado todo damos error
	CHECK_CHLOG_ERRORF( iStart != i, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Los espacios del final
	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( acIndent, ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ao_roString.Concat( "\r\n", ai_iMode ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


// las demás ====================================================================


m4uint32_t  ClCompiledMCR :: GetChannelNodeHandleById( m4pcchar_t ai_pccNodeId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccNodeId, M4CL_MAX_NODE_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfNodes() ;

	if( iLength > 0 )
	{
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + M4CL_MCR_CHANNEL_NODES, iLength, M4CL_MCR_CHANNEL_NODE_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_NODE_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetChannelRootHandleById( m4pcchar_t ai_pccRootId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iRootsHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccRootId, M4CL_MAX_NODE_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfRoots() ;

	if( iLength > 0 )
	{
		iRootsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ROOTS_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iRootsHandle + M4CL_MCR_CHANNEL_ROOTS, iLength, M4CL_MCR_CHANNEL_ROOT_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_ROOT_HANDLE ) ) ;
}


m4uint32_t  ClCompiledMCR :: GetChannelSentenceHandleById( m4pcchar_t ai_pccSentenceId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iSentencesHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccSentenceId, M4CL_MAX_SENTENCE_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfSentences() ;

	if( iLength > 0 )
	{
		iSentencesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_SENTENCES_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iSentencesHandle + M4CL_MCR_CHANNEL_SENTENCES, iLength, M4CL_MCR_CHANNEL_SENTENCE_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_SENTENCE_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetChannelInstanceHandleById( m4pcchar_t ai_pccInstanceId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iInstancesHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccInstanceId, M4CL_MAX_T3_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfInstances() ;

	if( iLength > 0 )
	{
		iInstancesHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_INSTANCES_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iInstancesHandle + M4CL_MCR_CHANNEL_INSTANCES, iLength, M4CL_MCR_CHANNEL_INSTANCE_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_INSTANCE_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetChannelItemHandleById( m4pcchar_t ai_pccItemId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iItemsHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;


	_SetWorkingArea( aiWorkingArea, ai_pccItemId, M4CL_MAX_ITEM_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfItems() ;

	if( iLength > 0 )
	{
		iItemsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_ITEMS_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iItemsHandle + M4CL_MCR_CHANNEL_ITEMS, iLength, M4CL_MCR_CHANNEL_ITEM_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_ITEM_HANDLE ) ) ;
}




m4uint32_t	ClCompiledMCR :: GetChannelTiHandleById( m4pcchar_t ai_pccTiId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iTisHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;


	_SetWorkingArea( aiWorkingArea, ai_pccTiId, M4CL_MAX_TI_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfTis() ;

	if( iLength > 0 )
	{
		iTisHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_TIS_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iTisHandle + M4CL_MCR_CHANNEL_TIS, iLength, M4CL_MCR_CHANNEL_TI_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_TI_HANDLE ) ) ;
}




m4pcchar_t  ClCompiledMCR :: GetChannelRSMMaskById( m4pcchar_t ai_pccRSMId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iRSMsHandle ;
    m4uint32_t  iMaskHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccRSMId, M4CL_MAX_RSM_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfRSMs() ;

	if( iLength > 0 )
	{
		iRSMsHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_RSMS_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iRSMsHandle + M4CL_MCR_CHANNEL_RSMS, iLength, M4CL_MCR_CHANNEL_RSMS_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( "" ) ;
		}
	}
	else
    {
        return( "" ) ;
    }

	iMaskHandle = M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_RSMS_MASK_ID ) ;

    return( m_pcBase + iMaskHandle ) ;
}


m4uint32_t  ClCompiledMCR :: GetChannelBusinessMethodHandleById( m4pcchar_t ai_pccBusinessMethodId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iBusinessMethodHandle ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;


	_SetWorkingArea( aiWorkingArea, ai_pccBusinessMethodId, M4CL_MAX_BMETHOD_ID, ai_iUpperCase ) ;


    iLength = GetChannelNumberOfBusinessMethods() ;

	if( iLength > 0 )
	{
		iBusinessMethodHandle = M4ClUnPackInt32( m_pcBase, M4CL_MCR_CHANNEL_BUSINESS_METHOD_START ) ;
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iBusinessMethodHandle + M4CL_MCR_CHANNEL_BUSINESS_METHODS, iLength, M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_CHANNEL_BUSINESS_METHOD_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeItemHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccItemId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccItemId, M4CL_MAX_ITEM_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfItems( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + ai_iNodeHandle + M4CL_MCR_NODE_NITEMS, iLength, M4CL_MCR_NODE_NITEM_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_NITEM_HANDLE ) ) ;
}




m4uint32_t  ClCompiledMCR :: GetNodeGroupHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccGroupId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iGroupsHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccGroupId, M4CL_MAX_GROUP_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfGroups( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iGroupsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_GROUPS_START ) ;

		if( iGroupsHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iGroupsHandle + M4CL_MCR_NODE_GROUPS, iLength, M4CL_MCR_NODE_GROUP_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_GROUP_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodePlugHandleByIds( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFatherChannelId, m4pcchar_t ai_pccFatherNodeId, m4bool_t ai_iUpperCase ) const
{

	m4uint8_t	iTruncated = 0 ;
    m4uint16_t  iLength ;
    m4uint16_t  iAuxLength ;
    m4uint32_t  iPlugsHandle ;
    m4pchar_t   pcObject ;
    m4pchar_t   pcStringArea ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA + M4CL_MCR_INT_32_STRING_AREA ] ;


    iLength = strlen( ai_pccFatherChannelId ) ;

    if( iLength > M4CL_MAX_T3_ID )
    {
        iLength = M4CL_MAX_T3_ID ;
		iTruncated = 1 ;
    }

    *aiWorkingArea = M4CL_MCR_CHANNEL_HANDLE_AREA_SIZE + 1 ;
    pcStringArea = ( ( m4pchar_t ) aiWorkingArea ) + M4CL_MCR_CHANNEL_STRING_AREA ;

	*pcStringArea = m4char_t( iLength ) ;
    memcpy( pcStringArea + 1, ai_pccFatherChannelId, sizeof( m4char_t ) * iLength ) ;
    *( pcStringArea + 1 + iLength ) = '\0' ;

	if( ai_iUpperCase != M4_TRUE )
	{
	    strupr( pcStringArea + 1 ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccFatherChannelId << M4CL_MAX_T3_ID << pcStringArea + 1 ) ;
	}


    iLength += 2 ;
	iTruncated = 0 ;
    iAuxLength = strlen( ai_pccFatherNodeId ) ;

    if( iAuxLength > M4CL_MAX_NODE_ID )
    {
        iLength = M4CL_MAX_NODE_ID ;
		iTruncated = 1 ;
    }

	*( pcStringArea + iLength ) = m4char_t( iAuxLength ) ;
    memcpy( pcStringArea + iLength + 1, ai_pccFatherNodeId, sizeof( m4char_t ) * iAuxLength ) ;
    *( pcStringArea + iLength + 1 + iAuxLength ) = '\0' ;

	if( ai_iUpperCase != M4_TRUE )
	{
		strupr( pcStringArea + iLength + 1 ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccFatherNodeId << M4CL_MAX_NODE_ID << pcStringArea + iLength + 1 ) ;
	}


// segundo apuntador relativo. Lo ponemos fuera para no molestar al caso normal
    *( aiWorkingArea + 1 ) = M4CL_MCR_CHANNEL_HANDLE_AREA_SIZE - M4_SIZE_OF_INT32 + iLength + 1 ;


    iLength = GetNodeNumberOfPlugs( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iPlugsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_PLUGS_START ) ;

		if( iPlugsHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iPlugsHandle + M4CL_MCR_NODE_PLUGS, iLength, M4CL_MCR_NODE_PLUG_SIZE, M4ClMCRComparePlugsByIds ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_PLUG_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeIndexHandleById( m4uint32_t ai_iNodeHandle, m4uint32_t ai_iIndexId ) const
{

    m4uint16_t  iLength ;
    m4pchar_t   pcObject ;
    m4uint32_t  iIndexesHandle ;

	
    iIndexesHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_INDEXES_START ) ;

	if( iIndexesHandle == 0 )
	{
		return( 0 ) ;
	}

    iLength = M4ClUnPackInt16( m_pcBase, iIndexesHandle + M4CL_MCR_NODE_INDEX_NUMBER ) ;

	if( iLength > 0 )
	{
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) &ai_iIndexId, m_pcBase + iIndexesHandle + M4CL_MCR_NODE_INDEXES, iLength, M4CL_MCR_NODE_INDEX_SIZE, M4ClMCRCompareIndexesById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_INDEX_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeAliasNodeHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAliasId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iAliasHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccAliasId, M4CL_MAX_NODE_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfAlias( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iAliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_ALIAS_START ) ;

		if( iAliasHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iAliasHandle + M4CL_MCR_NODE_ALIAS, iLength, M4CL_MCR_NODE_ALIAS_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_ALIAS_NODE_HANDLE ) ) ;
}



m4uint32_t	ClCompiledMCR :: GetNodeAliasNodeHandleByAlias( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccAlias, m4uint8_t ai_iLevel, m4bool_t ai_iUpperCase ) const
{

	m4uint8_t	i ;
	m4uint8_t	iInherits ;
	m4uint8_t	iTruncated = 0 ;
	m4uint16_t	iLength ;
	m4uint32_t	iReferences ;
	m4uint32_t	iNewReferences ;
	m4uint32_t	iTiHandle ;
	m4uint32_t	iNodeHandle ;
	m4uint32_t	iResult ;
	m4char_t	acAlias[ M4CL_MAX_TI_ID + 1 ] ;


	CHECK_CHLOG_DEBUGF( ai_pccAlias == NULL, 0, M4_MDRT_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iLength = strlen( ai_pccAlias ) ;

    if( iLength > M4CL_MAX_TI_ID )
    {
        iLength = M4CL_MAX_TI_ID ;
		iTruncated = 1 ;
    }

    memcpy( acAlias, ai_pccAlias, sizeof( m4char_t ) * iLength ) ;
    acAlias[ iLength ] = '\0' ;

	if( ai_iUpperCase != M4_TRUE )
	{
		strupr( acAlias ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccAlias << M4CL_MAX_TI_ID << acAlias ) ;
	}


// Lo buscamos por alias ======================================================

	iResult = GetNodeAliasNodeHandleById( ai_iNodeHandle, acAlias, M4_TRUE ) ;

	if( iResult != 0 )
	{
		return( iResult ) ;
	}


// Lo buscamos por ti =========================================================

	iTiHandle = GetChannelTiHandleById( acAlias, M4_TRUE ) ;

	if( iTiHandle != 0 )
	{
		iInherits = GetChannelInheritLevel() ;

		// Por si acaso, pero no debería pasar

		/* Bug 0074628
		El nivel no puede ser igual al número de herencias
		*/
		if( ai_iLevel >= iInherits )
		{
			if( iInherits > 0 )
			{
				ai_iLevel = iInherits - 1 ;
			}
			else
			{
				ai_iLevel = 0 ;
			}
		}

		// Buscamos en nuestro m4obj ==========================================

		iReferences = GetTiReferenceNumberByPosition( iTiHandle, ai_iLevel ) ;

		// Si hay referencias la hemos encontrado en nuestro nivel
		if( iReferences != 0 )
		{
			iResult = GetTiReferenceNodeHandleByPosition( iTiHandle, ai_iLevel ) ;
		}
		else
		{
			// Buscamos en los m4objs padre ===================================

			iReferences = 0 ;
			iResult = 0 ;

			iInherits = GetChannelInheritLevel() ;

			for( i = ai_iLevel + 1; i < iInherits ; i++ )
			{
				iNewReferences = GetTiReferenceNumberByPosition( iTiHandle, i ) ;

				if( iNewReferences != 0 )
				{
					if( iResult == 0 )
					{
						iResult = GetTiReferenceNodeHandleByPosition( iTiHandle, i ) ;
					}
					iReferences += iNewReferences ;
				}
			}

			if( iResult == 0 )
			{
				iResult = GetChannelNodeHandleById( acAlias, M4_TRUE ) ;
			}
		}

		if( iReferences > 1 )
		{
			iNodeHandle = GetChannelNodeHandleById( acAlias, M4_TRUE ) ;

			if( iNodeHandle != 0 )
			{
				iResult = iNodeHandle ;
			}
			else
			{
				DUMP_CHLOG_ERRORF( M4_MDRT_AMBIGUOUS_TI_CALL_IN_RUN_TIME, ai_pccAlias << iReferences << LOG_CAT_AND( iNodeN( ai_iNodeHandle ), nNodeN( ai_iNodeHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) ) ;
			}
		}

		if( iResult != 0 )
		{
			return( iResult ) ;
		}

		// Lo intentamos en el nivel más bajo

		if( ai_iLevel > 0 )
		{
			iResult = GetNodeAliasNodeHandleByAlias( ai_iNodeHandle, ai_pccAlias, 0, ai_iUpperCase ) ;
		}

		return( iResult ) ;
	}


// Lo buscamos por nodo =======================================================

	iResult = GetChannelNodeHandleById( acAlias, M4_TRUE ) ;

	if( iResult != 0 )
	{
		return( iResult ) ;
	}

	return( iResult ) ; 
}



// lo del NULL tiene que ser así, no ""
m4pcchar_t	ClCompiledMCR :: GetNodeT3AliasInstanceById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3AliasId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iT3AliasHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccT3AliasId, M4CL_MAX_T3_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfT3Alias( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

		if( iT3AliasHandle == 0 )
		{
			return( NULL ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iT3AliasHandle + M4CL_MCR_NODE_T3_ALIAS, iLength, M4CL_MCR_NODE_T3_ALIAS_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( NULL ) ;
		}
	}
	else
    {
        return( NULL ) ;
    }

    return( m_pcBase + M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeT3AliasInstanceHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3AliasId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iT3AliasHandle ;
    m4uint32_t  iInstanceHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccT3AliasId, M4CL_MAX_T3_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfT3Alias( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iT3AliasHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_T3_ALIAS_START ) ;

		if( iT3AliasHandle == 0 )
		{
			return( 0 ) ;
		}
		
		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iT3AliasHandle + M4CL_MCR_NODE_T3_ALIAS, iLength, M4CL_MCR_NODE_T3_ALIAS_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    iInstanceHandle = GetChannelInstanceHandleById( m_pcBase + M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID ) ) ;

    return( iInstanceHandle ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeT3AliasInstanceHandleByAlias( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3Alias, m4bool_t ai_iUpperCase ) const
{

    m4pcchar_t  pccInstanceId ;
    m4uint32_t  iInstanceHandle ;


    pccInstanceId = GetNodeT3AliasInstanceById( ai_iNodeHandle, ai_pccT3Alias, ai_iUpperCase ) ;

    if( pccInstanceId == NULL )
    {
        pccInstanceId = ai_pccT3Alias ;
    }

    iInstanceHandle = GetChannelInstanceHandleById( pccInstanceId, ai_iUpperCase ) ;

    return( iInstanceHandle ) ;
}


m4pcchar_t	ClCompiledMCR :: GetNodeT3AliasInstanceIdByAlias( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccT3Alias, m4bool_t ai_iUpperCase ) const
{

    m4pcchar_t  pccInstanceId ;


    pccInstanceId = GetNodeT3AliasInstanceById( ai_iNodeHandle, ai_pccT3Alias, ai_iUpperCase ) ;

    if( pccInstanceId == NULL )
    {
        pccInstanceId = ai_pccT3Alias ;
    }

    return( pccInstanceId ) ;
}




m4uint32_t  ClCompiledMCR :: GetNodeDmdHandleByIds( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccDmdId, m4pcchar_t ai_pccDmdComponentId, m4pcchar_t ai_pccDmdFieldId, m4bool_t ai_iUpperCase ) const
{


	m4uint8_t	iTruncated = 0 ;
    m4uint16_t  iLength ;
    m4uint32_t  iDmdsHandle ;
    m4pchar_t   pcObject ;
    m4pchar_t   pcStringArea ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA + M4CL_MCR_INT_32_STRING_AREA + M4CL_MCR_INT_32_STRING_AREA ] ;


// esto está un poco sucio, pero no lo tocamos por que así debe ser más óptimo

	iLength = M4_SIZE_OF_INT32 << 0x02 ;
    pcStringArea = ( ( m4pchar_t ) aiWorkingArea ) + iLength ;

// primer apuntador relativo
    *aiWorkingArea = iLength + 1 ;

    iLength = strlen( ai_pccDmdComponentId ) ;

    if( iLength > M4CL_MAX_DMD_COMPONENT_ID )
    {
        iLength = M4CL_MAX_DMD_COMPONENT_ID ;
		iTruncated = 1 ;
    }

	*pcStringArea = m4char_t( iLength ) ;
    memcpy( pcStringArea + 1, ai_pccDmdComponentId, sizeof( m4char_t ) * iLength ) ;
    *( pcStringArea + 1 + iLength ) = '\0' ;

	if( ai_iUpperCase == M4_FALSE )
	{
		strupr( pcStringArea + 1 ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccDmdComponentId << M4CL_MAX_DMD_COMPONENT_ID << pcStringArea + 1 ) ;
	}


    pcStringArea += ( iLength + 2 ) ;

// segundo apuntador relativo
    *( aiWorkingArea + 1 ) = pcStringArea - ( m4pchar_t ) aiWorkingArea - M4_SIZE_OF_INT32 + 1 ;

    iLength = strlen( ai_pccDmdFieldId ) ;
	iTruncated = 0 ;

    if( iLength > M4CL_MAX_DMD_FIELD_ID )
    {
        iLength = M4CL_MAX_DMD_FIELD_ID ;
		iTruncated = 1 ;
    }

	*pcStringArea = m4char_t( iLength ) ;
    memcpy( pcStringArea + 1, ai_pccDmdFieldId, sizeof( m4char_t ) * iLength ) ;
    *( pcStringArea + 1 + iLength ) = '\0' ;

	if( ai_iUpperCase == M4_FALSE )
	{
		strupr( pcStringArea + 1 ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccDmdFieldId << M4CL_MAX_DMD_FIELD_ID << pcStringArea + 1 ) ;
	}


    pcStringArea += ( iLength + 2 ) ;

// tercer apuntador relativo
    *( aiWorkingArea + 2 ) = pcStringArea - ( m4pchar_t ) aiWorkingArea - ( M4_SIZE_OF_INT32 << 0x01 ) + 1 ;

    iLength = strlen( ai_pccDmdId ) ;
	iTruncated = 0 ;

    if( iLength > M4CL_MAX_DMD_ID )
    {
        iLength = M4CL_MAX_DMD_ID ;
		iTruncated = 1 ;
    }

	*pcStringArea = m4char_t( iLength ) ;
    memcpy( pcStringArea + 1, ai_pccDmdId, sizeof( m4char_t ) * iLength ) ;
    *( pcStringArea + 1 + iLength ) = '\0' ;

	if( ai_iUpperCase == M4_FALSE )
	{
	    strupr( pcStringArea + 1 ) ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDRT_DATA_TRUNCATED, ai_pccDmdId << M4CL_MAX_DMD_ID << pcStringArea + 1 ) ;
	}


    iLength = GetNodeNumberOfDmds( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iDmdsHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_DMDS_START ) ;

		if( iDmdsHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iDmdsHandle + M4CL_MCR_NODE_DMDS, iLength, M4CL_MCR_NODE_DMD_SIZE, M4ClMCRCompareDmdsById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_DMDS_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeQBFFilterHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFilterId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iFiltersHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccFilterId, M4CL_MAX_FILTER_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfQBFFilters( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_QBF_FILTERS_START ) ;

		if( iFiltersHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iFiltersHandle + M4CL_MCR_NODE_FILTERS, iLength, M4CL_MCR_NODE_FILTER_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_FILTER_HANDLE ) ) ;
}



m4uint32_t  ClCompiledMCR :: GetNodeFindFilterHandleById( m4uint32_t ai_iNodeHandle, m4pcchar_t ai_pccFilterId, m4bool_t ai_iUpperCase ) const
{

    m4uint16_t  iLength ;
    m4uint32_t  iFiltersHandle ;
    m4pchar_t   pcObject ;
    m4uint32_t	aiWorkingArea[ M4CL_MCR_INT_32_STRING_AREA ] ;

	
	_SetWorkingArea( aiWorkingArea, ai_pccFilterId, M4CL_MAX_FILTER_ID, ai_iUpperCase ) ;


    iLength = GetNodeNumberOfFindFilters( ai_iNodeHandle ) ;

	if( iLength > 0 )
	{
		iFiltersHandle = M4ClUnPackInt32( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_FIND_FILTERS_START ) ;

		if( iFiltersHandle == 0 )
		{
			return( 0 ) ;
		}

		pcObject = ( m4pchar_t ) M4VMBSearch( ( m4pchar_t ) aiWorkingArea, m_pcBase + iFiltersHandle + M4CL_MCR_NODE_FILTERS, iLength, M4CL_MCR_NODE_FILTER_SIZE, M4ClMCRCompareOneStringById ) ;

		if( pcObject == NULL )
		{
			return( 0 ) ;
		}
	}
	else
    {
        return( 0 ) ;
    }

    return( M4ClUnPackInt32( pcObject, M4CL_MCR_NODE_FILTER_HANDLE ) ) ;
}



m4int16_t	ClCompiledMCR :: GetItemFormOrderByFormId( m4uint32_t ai_iItemHandle, m4pcchar_t ai_iFormId ) const
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t  iItemRefHandle ;
    m4uint32_t  iFormIdsHandle ;
    m4uint32_t  iHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormIdsHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORM_IDS_START ) ;

	if( iFormIdsHandle == 0 )
	{
		return( -1 ) ;
	}

	iLength = M4ClUnPackInt16( m_pcBase, iFormIdsHandle ) ;

	i = 0 ;
	iHandle = iFormIdsHandle + M4CL_MCR_RITEM_FORM_IDS ;

	while( i < iLength )
	{
		if( strcmpi( ai_iFormId, m_pcBase + M4ClUnPackInt32( m_pcBase, iHandle ) ) == 0 )
		{
			return( i ) ;
		}
		iHandle += M4CL_MCR_RITEM_FORM_IDS_SIZE ;
		i++ ;
	}

	return( -1 ) ;
}



m4pcchar_t  ClCompiledMCR :: GetItemCodeByDate( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4date_t ai_dSliceDate, m4uint16_t &ao_riSlicePosition ) const
{

    m4uint8_t   iFound ;
    m4uint8_t   iTimeUnit ;
    m4uint16_t  i ;
    m4uint16_t  iNumberOfFormulas ;
    m4uint16_t  iNumberOfSlices ;
	m4uint32_t  iItemRefHandle ;
    m4uint32_t  iSliceHandle ;
    m4uint32_t  iCodeHandle ;
    m4uint32_t  iFormulas ;
    m4uint32_t  iObjectSize ;
    m4uint32_t  iFormulaHandle ;
    m4date_t    dStartDate ;
    m4date_t    dEndDate ;
	m4double_t	dTimeUnit ;


	ao_riSlicePosition = 0 ;

    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
        return( NULL ) ;
	}

    iNumberOfFormulas = M4ClUnPackInt16( m_pcBase, iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_NUMBER ) ;

    if( ai_iRulePosition >= iNumberOfFormulas )
    {
        return( NULL ) ;
    }

	iTimeUnit = M4ClUnPackInt8( m_pcBase, M4CL_MCR_CHANNEL_TIME_UNIT ) ;

	switch( iTimeUnit )
	{
		case M4_UNIT_ONE_DAY :

			dTimeUnit = M4_ONE_DAY ;
			break ;

		case M4_UNIT_ONE_HOUR :

			dTimeUnit = M4_ONE_HOUR ;
			break ;

		case M4_UNIT_ONE_MINUTE :

			dTimeUnit = M4_ONE_MINUTE ;
			break ;

		case M4_UNIT_ONE_SECOND :

			dTimeUnit = M4_ONE_SECOND ;
			break ;

		default :

			dTimeUnit = M4_ONE_DAY ;
			break ;
	}

    iNumberOfSlices = M4ClUnPackInt16( m_pcBase, iFormulaHandle ) ;

    iFormulas = M4CL_MCR_FORMULA_FORMULA_START + iFormulaHandle ;
    iObjectSize = M4_ROUND_TO_8( 2 * M4_SIZE_OF_DATE + iNumberOfFormulas * M4_SIZE_OF_INT32 ) ;

    iFound = 0 ;
    for( i = 0 ; i < iNumberOfSlices && iFound == 0 ; i++ )
    {
        iSliceHandle = iFormulas + i * iObjectSize ;
        dStartDate = M4ClUnPackDate( m_pcBase, iSliceHandle ) ;
        dEndDate = M4ClUnPackDate( m_pcBase, iSliceHandle + M4CL_MCR_FORMULA_END_DATE ) ;

// para la corrección del espacio discontinuo de fechas
//      if( ai_dSliceDate >= dStartDate && ai_dSliceDate <= dEndDate )
        if( ai_dSliceDate >= dStartDate && ( ai_dSliceDate < dEndDate + dTimeUnit ) )
        {
			ao_riSlicePosition = i ;
            iFound = 1 ;
        }
    }

    if( iFound == 0 )
    {
        return( NULL ) ;
    }

    iCodeHandle = M4ClUnPackInt32( m_pcBase, iSliceHandle + M4CL_MCR_FORMULA_CODE_START + ai_iRulePosition * M4_SIZE_OF_INT32 ) ;

    if( iCodeHandle == 0 )
    {
        return( NULL ) ;
    }

    return( m_pcBase + iCodeHandle ) ;
}



m4pcchar_t  ClCompiledMCR :: GetItemCodeByPosition( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4uint16_t ai_iSlicePosition ) const
{

    m4uint16_t  iNumberOfFormulas ;
	m4uint32_t  iItemRefHandle ;
    m4uint32_t  iFormulaHandle ;
    m4uint32_t  iSliceHandle ;
    m4uint32_t  iCodeHandle ;


    iItemRefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_RITEM ) ;
    iFormulaHandle = M4ClUnPackInt32( m_pcBase, iItemRefHandle + M4CL_MCR_RITEM_FORMULAS_START ) ;

	if( iFormulaHandle == 0 )
	{
        return( NULL ) ;
	}

    iNumberOfFormulas = M4ClUnPackInt16( m_pcBase, iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_NUMBER ) ;

    if( ai_iRulePosition >= iNumberOfFormulas )
    {
        return( NULL ) ;
    }

    iSliceHandle = iFormulaHandle + M4CL_MCR_FORMULA_FORMULA_START + ai_iSlicePosition * M4_ROUND_TO_8( 2 * M4_SIZE_OF_DATE + iNumberOfFormulas * M4_SIZE_OF_INT32 ) ;
    iCodeHandle = M4ClUnPackInt32( m_pcBase, iSliceHandle + M4CL_MCR_FORMULA_CODE_START + ai_iRulePosition * M4_SIZE_OF_INT32 ) ;

    if( iCodeHandle == 0 )
    {
        return( NULL ) ;
    }

    return( m_pcBase + iCodeHandle ) ;
}



m4pcchar_t  ClCompiledMCR :: GetItemCodeByRuleDates( m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, m4date_t ai_dStartDate, m4date_t ai_dEndDate ) const
{

	m4uint16_t	i ;
	m4uint16_t	iSlices ;
	m4date_t	dStartDate ;
	m4date_t	dEndDate ;
	m4pcchar_t	pccCode ;


	iSlices = GetItemNumberOfSlices( ai_iItemHandle ) ;

	for( i = 0 ; i < iSlices ; i++ )
	{
		pccCode = GetItemCodeByPosition( ai_iItemHandle, ai_iRulePosition, i ) ;

		if( pccCode != NULL )
		{
			dStartDate = GetCodeStartDate( pccCode ) ;

			if( dStartDate == ai_dStartDate )
			{
				dEndDate = GetCodeEndDate( pccCode ) ;

				if( dEndDate == ai_dEndDate )
				{
					return( pccCode ) ;
				}
			}
		}
	}

	return( NULL ) ;
}



m4pcchar_t	ClCompiledMCR :: GetItemNextCode( m4uint32_t ai_iItemHandle, m4int16_t &ai_rioNext ) const
{

	m4bool_t	bFound ;
	m4int16_t	i ;
	m4int16_t	iPosition ;
	m4int16_t	iSlice ;
	m4int16_t	iFormula ;
	m4int16_t	iSlices ;
	m4int16_t	iFormulas ;
	m4int16_t	iTotal ;
	m4pcchar_t	pccCode ;
	m4pcchar_t	pccNextCode ;


	iSlices = GetItemNumberOfSlices( ai_iItemHandle ) ;
	iFormulas = GetItemNumberOfFormulas( ai_iItemHandle ) ;

	iTotal = iSlices * iFormulas ;


	// Si estamos fuera de límites volvemos a empezar
	if( ai_rioNext < -1 || ai_rioNext >= iTotal )
	{
		ai_rioNext = -1 ;
	}

	while( ++ai_rioNext < iTotal )
	{
		iFormula = ai_rioNext / iSlices ;
		iSlice = ai_rioNext % iSlices ;
		pccNextCode = GetItemCodeByPosition( ai_iItemHandle, iFormula, iSlice ) ;

		if( pccNextCode == NULL )
		{
			continue ;
		}

		bFound = M4_FALSE ;
		for( i = 0 ; i < ai_rioNext && bFound == M4_FALSE ; i++ )
		{
			iPosition = ai_rioNext - i - 1 ;
			iFormula = iPosition / iSlices ;
			iSlice = iPosition % iSlices ;
			pccCode = GetItemCodeByPosition( ai_iItemHandle, iFormula, iSlice ) ;

			if( pccCode == pccNextCode )
			{
				bFound = M4_TRUE ;
			}
		}

		if( bFound == M4_FALSE )
		{
			return( pccNextCode ) ;
		}
	}

	return( NULL ) ;
}



// Funciones de acceso al código fuente =======================================

m4return_t	ClCompiledMCR :: GetCodeSource( m4pcchar_t ai_pccCode, m4uint32_t ai_iItemHandle, m4uint16_t ai_iRulePosition, ClLongString &ao_roString, m4uint8_t ai_iMode, m4uint8_t ai_iIndent ) const
{

	m4return_t		iResult ;
	m4date_t		dStartDate ;
	m4date_t		dEndDate ;
	m4pcchar_t		pccRuleId ;
    m4char_t		acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t		acEndDate  [ M4_SECOND_END + 1 ] ;
	ClDecodeFormula	oFormula ;


	iResult = _GetCodeSource( ai_pccCode, ai_iItemHandle, ao_roString, ai_iMode, ai_iIndent ) ;

	if( iResult != M4_SUCCESS )
	{
		pccRuleId = GetItemFormIdByPosition( ai_iItemHandle, ai_iRulePosition ) ;

		if( pccRuleId == NULL )
		{
			pccRuleId = "" ;
		}

		if( ai_pccCode != NULL )
		{
			oFormula.Init( ai_pccCode ) ;

			dStartDate = oFormula.GetStartDate() ;
			dEndDate = oFormula.GetEndDate() ;
		}
		else
		{
			dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
			dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
		}

		ClDateToString( dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDRT_INTERNAL_INVERSE_ERROR, pccRuleId << LOG_CAT_AND( iItemI( ai_iItemHandle ), nItemI( ai_iItemHandle ) ) << LOG_CAT_AND( iNodeI( ai_iItemHandle ), nNodeI( ai_iItemHandle ) ) << LOG_CAT_AND( iM4Obj(), nM4Obj() ) << acStartDate << acEndDate << __LINE__ << __FILE__ ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// ============================================================================
// Función que busca la siguiente línea de código ejecutable a partir de una dada
// Sirve para poder colocar puntos de ruptura en líneas válidas
// Se recorre todas las instrucciones y va computando la linea actual hasta que
// es mayor que la que nos pasan
// Sólo no se encuentra una linea válida si se acaba la fórmula
// ============================================================================

m4int32_t	ClCompiledMCR :: GetNextCodeLine( m4pcchar_t ai_pccCode, m4int32_t ai_iLine ) const
{

	m4int32_t			i ;
	m4int32_t			iLength ;
	m4int32_t			iLine ;
	ClDecodeFormula		oForm ;
	ClDecodeInstruction	*poInstruction ;


	oForm.Init( ai_pccCode ) ;

	iLine = 0 ;
	iLength = oForm.GetNumberOfInstructions() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poInstruction = oForm[ i ] ;

		if( poInstruction == NULL )
		{
			return( -1 ) ;
		}

		if( poInstruction->GetExeFunc() == M4CL_VM_FT_READCOMMENT )
		{
			iLine = poInstruction->GetNumVar() ;
			continue ;
		}

		if( iLine >= ai_iLine )
		{
			return( iLine ) ;
		}
	}

	return( -1 ) ;
}



// ============================================================================
// Función que busca la siguiente línea de código ejecutable a partir de una dada
// Sirve para poder colocar puntos de ruptura en líneas válidas
// Los argumentos son los que definen un punto de ruptura
// Simplemente se posiciona en el código y llama a la función anterior
// ============================================================================

m4int32_t	ClCompiledMCR :: GetNextCodeLine( m4pcchar_t ai_pccNodeId, m4pcchar_t ai_pccItemId, m4pcchar_t ai_pccRuleId, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4int32_t ai_iLine ) const
{

	m4int32_t	iResult ;
	m4int16_t	iRuleOrder ;
	m4uint32_t	iItem ;
	m4uint32_t	iNode ;
	m4pcchar_t	pccCode ;


	iNode = GetChannelNodeHandleById( ai_pccNodeId ) ;

	if( iNode == 0 )
	{
		return( -1 ) ;
	}

	iItem = GetNodeItemHandleById( iNode, ai_pccItemId ) ;

	if( iItem == 0 )
	{
		return( -1 ) ;
	}

	iRuleOrder = GetItemFormOrderByFormId( iItem, ai_pccRuleId ) ;

	if( iRuleOrder == -1 )
	{
		return( -1 ) ;
	}

	/* Bug 0250746
	Se busca por fechas de definición de las reglas
	*/
	pccCode = GetItemCodeByRuleDates( iItem, iRuleOrder, ai_dStartDate, ai_dEndDate ) ;

	if( pccCode == NULL )
	{
		return( -1 ) ;
	}

	iResult = GetNextCodeLine( pccCode, ai_iLine ) ;

	return( iResult ) ;
}




void    ClCompiledMCR :: GetItemDefaultValue( m4uint32_t ai_iItemHandle, m4VariantType &ao_rvValue ) const
{

    m4uint8_t   iType ;
    m4uint8_t   iM4Type ;
    m4uint8_t   iFlag ;
    m4uint32_t  iItemDefHandle ;
    m4uint32_t  iObjectHandle ;


    iItemDefHandle = M4ClUnPackInt32( m_pcBase, ai_iItemHandle ) ;

    iFlag = M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DEFAULT_VALUE_FLAG ) ;
	iM4Type = M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_M4_TYPE ) ;

    if( iFlag == 0 )
    {
        ao_rvValue.Type = M4CL_CPP_TYPE_NULL ;
    }
    else
    {
        iType = M4ClUnPackInt8( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_CPP_TYPE ) ;

        switch( iType )
        {
            case M4CL_CPP_TYPE_NUMBER :
                ao_rvValue.Type = M4CL_CPP_TYPE_NUMBER ;
                ao_rvValue.Data.DoubleData = M4ClUnPackDouble( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DEFAULT_VALUE ) ;
                break ;

            case M4CL_CPP_TYPE_STRING :
                ao_rvValue.Type = M4CL_CPP_TYPE_STRING_VAR ;
                iObjectHandle = M4ClUnPackInt32( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DEFAULT_VALUE ) ;
                ao_rvValue.Data.PointerChar = m_pcBase + iObjectHandle ;
                break ;

            case M4CL_CPP_TYPE_DATE :
                ao_rvValue.Type = M4CL_CPP_TYPE_DATE ;
                ao_rvValue.Data.DoubleData = M4ClUnPackDouble( m_pcBase, iItemDefHandle + M4CL_MCR_ITEM_DEFAULT_VALUE ) ;
                break ;

            default :
                ao_rvValue.Type = M4CL_CPP_TYPE_NULL ;
                break ;
        }
    }
}




m4uint32_t  ClCompiledMCR :: GetPlugItemHandleByItemHandle( m4uint32_t ai_iPlugHandle, m4uint32_t ai_iItemHandle ) const
{

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint32_t	iPlugItemHandle ;
	m4uint32_t	iItemHandle ;


	iLength = GetPlugNumberOfItems( ai_iPlugHandle ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iPlugItemHandle = GetPlugItemHandleByPosition( ai_iPlugHandle, i ) ;
		iItemHandle = GetPlugItemSonItemHandle( iPlugItemHandle ) ;

		if( iItemHandle == ai_iItemHandle )
		{
			return( iPlugItemHandle ) ;
		}
	}

	return( 0 ) ;
}


// seguridad ==================================================================


m4uint32_t  ClCompiledMCR :: GetSecuritySize( void ) const
{

	m4uint32_t	iResult ;


	if( m_pcSecurity == NULL )
	{
		iResult = 0 ;
	}
	else
	{
		iResult = M4ClUnPackInt32( m_pcSecurity, M4CL_SCR_SECURITY_SIZE ) ;
	}

    return( iResult ) ;
}


m4uint16_t  ClCompiledMCR :: GetSecurityBuild( void ) const
{

	m4uint16_t	iResult ;


	if( m_pcSecurity == NULL )
	{
		iResult = 0 ;
	}
	else
	{
		iResult = M4ClUnPackInt16( m_pcSecurity, M4CL_SCR_SECURITY_BUILD ) ;
	}

    return( iResult ) ;
}


m4uint8_t	ClCompiledMCR :: GetSecurityCsType( void ) const
{

	m4uint8_t	iResult ;


	if( m_pcSecurity == NULL )
	{
		iResult = 0 ;
	}
	else
	{
		iResult = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_SECURITY_CS_TYPE ) ;
	}

    return( iResult ) ;
}


m4pcchar_t  ClCompiledMCR :: GetSecurityMaskId( void ) const
{

	m4pcchar_t	pccResult ;
    m4uint32_t  iHandle ;


	if( m_pcSecurity == NULL )
	{
		pccResult = "" ;
	}
	else
	{
		iHandle = M4ClUnPackInt32( m_pcSecurity, M4CL_SCR_SECURITY_MASK_ID ) ;
		pccResult = m_pcSecurity + iHandle ;
	}

    return( pccResult ) ;
}


m4pcchar_t  ClCompiledMCR :: GetSecurityChannelId( void ) const
{

	m4pcchar_t	pccResult ;
    m4uint32_t  iHandle ;


	if( m_pcSecurity == NULL )
	{
		pccResult = "" ;
	}
	else
	{
		iHandle = M4ClUnPackInt32( m_pcSecurity, M4CL_SCR_SECURITY_T3_ID ) ;
		pccResult = m_pcSecurity + iHandle ;
	}

    return( pccResult ) ;
}


m4date_t  ClCompiledMCR :: GetSecurityT3Version( void ) const
{

	m4date_t	dResult ;


	if( m_pcSecurity == NULL )
	{
		dResult = 0 ;
	}
	else
	{
		dResult = M4ClUnPackDate( m_pcSecurity, M4CL_SCR_SECURITY_T3_VERSION ) ;
	}

    return( dResult ) ;
}


m4date_t  ClCompiledMCR :: GetSecurityOwnVersion( void ) const
{

	m4date_t	dResult ;


	if( m_pcSecurity == NULL )
	{
		dResult = 0 ;
	}
	else
	{
		dResult = M4ClUnPackDate( m_pcSecurity, M4CL_SCR_SECURITY_OWN_VERSION ) ;
	}

    return( dResult ) ;
}


m4uint16_t  ClCompiledMCR :: GetSecurityNumberOfNodes( void ) const
{

	m4uint16_t	iResult ;


	if( m_pcSecurity == NULL )
	{
		iResult = 0 ;
	}
	else
	{
		iResult = M4ClUnPackInt16( m_pcSecurity, M4CL_SCR_SECURITY_NODE_NUMBER ) ;
	}

    return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetChannelSecRead( void ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iMask = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_T3_MASK_START ) ;
		iResult = ( M4CL_SCR_READ & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetChannelSecWrite( void ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iMask = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_T3_MASK_START ) ;
		iResult = ( M4CL_SCR_WRITE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetChannelSecDelete( void ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iMask = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_T3_MASK_START ) ;
		iResult = ( M4CL_SCR_DELETE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetChannelSecUpdate( void ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iMask = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_T3_MASK_START ) ;
		iResult = ( M4CL_SCR_UPDATE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetChannelSecExecute( void ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iMask = M4ClUnPackInt8( m_pcSecurity, M4CL_SCR_T3_MASK_START ) ;
		iResult = ( M4CL_SCR_EXECUTE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecRead( m4uint32_t ai_iNodeHandle ) const
{
	
	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_READ & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecWrite( m4uint32_t ai_iNodeHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_WRITE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecDelete( m4uint32_t ai_iNodeHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_DELETE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecUpdate( m4uint32_t ai_iNodeHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_UPDATE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecExecute( m4uint32_t ai_iNodeHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_EXECUTE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t   ClCompiledMCR :: GetNodeSecDynFilter( m4uint32_t ai_iNodeHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_DYN_FILTER & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4pcchar_t  ClCompiledMCR :: GetNodeSecFilter( m4uint32_t ai_iNodeHandle ) const
{

	m4pcchar_t	pccResult ;
    m4uint32_t  iHandle ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		pccResult = NULL ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iNodeHandle + M4CL_MCR_NODE_SCR_START ) ;
		iHandle = M4ClUnPackInt32( m_pcSecurity, iSecurity + M4CL_SCR_NODE_SEC_FILTER ) ;

		if( iHandle == 0 )
		{
			pccResult = NULL ;
		}
		else
		{
			pccResult = m_pcSecurity + iHandle ;
		}
	}

    return( pccResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecRead( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_READ & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecWrite( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_WRITE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecDelete( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_DELETE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecUpdate( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_UPDATE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecExecute( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_EXECUTE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetItemSecContinue( m4uint32_t ai_iItemHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iItemHandle + M4CL_MCR_NITEM_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_CONTINUE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t     ClCompiledMCR :: GetFilterIsApplicable( m4uint32_t ai_iFilterHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iFilterHandle + M4CL_MCR_FILTER_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_IS_APPLICABLE & iMask ) != 0 ;
	}

	return( iResult ) ;
}


m4uint8_t	ClCompiledMCR :: GetPlugIsApplicable( m4uint32_t ai_iPlugHandle ) const
{

	m4uint8_t	iResult ;
	m4uint8_t	iMask ;
	m4uint16_t	iSecurity ;


	if( m_pcSecurity == NULL )
	{
		iResult = M4CL_SEC_ENABLE ;
	}
	else
	{
		iSecurity = M4ClUnPackInt16( m_pcBase, ai_iPlugHandle + M4CL_MCR_PLUG_SCR_START ) ;
		iMask = M4ClUnPackInt8( m_pcSecurity, iSecurity ) ;
		iResult = ( M4CL_SCR_IS_APPLICABLE & iMask ) != 0 ;
	}

	return( iResult ) ;
}

