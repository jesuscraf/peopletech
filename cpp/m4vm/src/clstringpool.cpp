
//==============================================================================
//
// (c) Copyright  1991-2012 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4variant.dll
// File:                clstringpool.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                17-10-2012
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a dynamic string pool.
//
//
//==============================================================================

#include <string.h>

#include "clstringpool.hpp"
#include "chlog.hpp"
#include "vmres.hpp"

#ifdef _M4_TRACE_POOL
#include "vmachine.h"
#endif



//=============================================================================
// Defines
//=============================================================================

#define	M4SP_POOL_START							24
#define	M4SP_POOL_MASK							0xFF000000
#define	M4SP_OFFSET_MASK						0x00FFFFFF

#define	M4SP_STRING_SIZE						4


#ifdef	_M4_TRACE_POOL
static FILE*	s_pfFile = NULL ;

void PoolTrace( m4pcchar_t ai_pccLabel, m4pcchar_t ai_pccString1, m4pcchar_t ai_pccString2, m4pcchar_t ai_pccString3, m4int32_t ai_iNumber1, m4int32_t ai_iNumber2 )
{
	if( s_pfFile == NULL )
	{
		s_pfFile = fopen( "c:\\temp\\m4pool.log", "w" ) ;
	}

	if( s_pfFile != NULL )
	{
		fprintf( s_pfFile, "%s:", ai_pccLabel ) ;

		if( ai_pccString1 != NULL )
		{
			fprintf( s_pfFile, " [%s]", ai_pccString1 ) ;
		}

		if( ai_pccString2 != NULL )
		{
			fprintf( s_pfFile, " [%s]", ai_pccString2 ) ;
		}

		if( ai_pccString3 != NULL )
		{
			fprintf( s_pfFile, " [%s]", ai_pccString3 ) ;
		}

		if( ai_iNumber1 != -1)
		{
			fprintf( s_pfFile, " [%d]", ai_iNumber1 ) ;
		}

		if( ai_iNumber2 != -1)
		{
			fprintf( s_pfFile, " [%d]", ai_iNumber2 ) ;
		}

		fprintf( s_pfFile, "\n" ) ;
		fflush( s_pfFile ) ; 
	}
}
#endif


//=================================================================================
//
// ClOnePool
//
// Clase que implementa un pool unitario de cadenas
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClOnePool :: ClOnePool( void )
{
	m_pcStart = NULL ;
	m_pcCurrent = NULL ;
	m_pcEnd = NULL ;
}


ClOnePool :: ~ClOnePool( void )
{
	Destroy() ;
}


void ClOnePool :: Reset( void )
{
	m_pcCurrent = m_pcStart ;
}


m4return_t	ClOnePool :: Create( m4uint32_t ai_iSize )
{
	Destroy() ;

	m_pcStart = new m4char_t[ ai_iSize ] ;
	CHECK_CHLOG_ERRORF( m_pcStart == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( m4char_t ) * ai_iSize ) << __LINE__ << __FILE__ ) ;
	m_pcCurrent = m_pcStart ;
	m_pcEnd = m_pcStart + ai_iSize ;

	return( M4_SUCCESS ) ;
}


m4pchar_t	ClOnePool :: Destroy( m4bool_t ai_bFree )
{

	m4pchar_t	pcResult = NULL ;


	if( m_pcStart != NULL )
	{
		if( ai_bFree == M4_TRUE )
		{
			delete( m_pcStart ) ;
		}
		else
		{
			pcResult = m_pcStart ;
		}

		m_pcStart = NULL ;
		m_pcCurrent = NULL ;
		m_pcEnd = NULL ;
	}

	return( pcResult ) ;
}


void	ClOnePool :: Move( ClOnePool& ai_roPool )
{
	ai_roPool.Destroy() ;

	ai_roPool.m_pcStart = m_pcStart ;
	ai_roPool.m_pcCurrent = m_pcCurrent ;
	ai_roPool.m_pcEnd = m_pcEnd ;

	m_pcStart = NULL ;
	m_pcCurrent = NULL ;
	m_pcEnd = NULL ;
}



// Funciones de lectura =======================================================

m4pchar_t	ClOnePool :: GetString( m4uint32_t ai_iOffset ) const
{

	m4pchar_t	pcString = m_pcStart + ai_iOffset ;

	if( pcString >= m_pcEnd )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_POOL_HANDLE, ai_iOffset << m4uint32_t( m_pcCurrent - m_pcStart ) ) ;
		return( "" ) ;
	}

	return( pcString ) ;
}


m4return_t	ClOnePool :: Dump( FILE *ai_poFile ) const
{

	m4uint32_t	iLength = 0 ;

	if( ai_poFile == NULL )
	{
		return( M4_SUCCESS ) ;
	}

	iLength = GetLength() ;
	fprintf( ai_poFile, "Length <%d> Size <%d> Start <0x%x> Current <0x%x> End <0x%x>\n[", iLength, GetSize(), m_pcStart, m_pcCurrent, m_pcEnd ) ;

	if( iLength > 0 )
	{
		fwrite( m_pcStart, sizeof( m4char_t ), GetLength(), ai_poFile ) ;
	}
	fprintf( ai_poFile, "]\n" ) ;

	return( M4_SUCCESS ) ;
}



// Funciones auxiliares =======================================================

m4return_t ClOnePool :: NewString( m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset )
{

	ao_rpcOutString = NULL ;
	ao_riOutOffset = -1 ;

	if( m_pcEnd - m_pcCurrent < m4int32_t( M4SP_STRING_SIZE + ai_iInSize + M4SP_STRING_SIZE ) )
	{
		// No hay hueco
		return( M4_ERROR ) ;
	}

	if( m_pcCurrent - m_pcStart > m4int32_t( M4SP_OFFSET_MASK - M4SP_STRING_SIZE ) )
	{
		/* Bug 0257080
		El offset no se puede direccionar
		*/
		return( M4_ERROR ) ;
	}

	// Si hay hueco se devuelve la cadena y su offset y se escriben las cabeceras
	( *( (m4uint32_t*)( m_pcCurrent ) ) ) = ai_iInSize ;
	ao_rpcOutString = m_pcCurrent + M4SP_STRING_SIZE ;
	ao_riOutOffset = ao_rpcOutString - m_pcStart ;

	if( ai_pccInString != NULL && ai_iInLength > 0 && ai_pccInString != ao_rpcOutString )
	{
		memcpy( ao_rpcOutString, ai_pccInString, sizeof( m4char_t ) * ai_iInLength ) ;
	}
	m_pcCurrent = ao_rpcOutString + ai_iInSize + M4SP_STRING_SIZE ;
	( *( (m4uint32_t*)( m_pcCurrent - M4SP_STRING_SIZE ) ) ) = ai_iInSize ;

#ifdef	_M4_TRACE_POOL
	PoolTrace( "Add", ai_pccInString, NULL, NULL, ao_riOutOffset, -1 ) ;
#endif

	return( M4_SUCCESS ) ;
}


m4return_t	ClOnePool :: DeleteString( m4uint32_t ai_iOffset )
{

	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcString = NULL ;


	pcString = m_pcStart + ai_iOffset - M4SP_STRING_SIZE ;

	if( pcString >= m_pcEnd )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_POOL_HANDLE, ai_iOffset << m4uint32_t( m_pcCurrent - m_pcStart ) ) ;
		return( M4_ERROR ) ;
	}

#ifdef	_M4_TRACE_POOL
	PoolTrace( "Delete", pcString + M4SP_STRING_SIZE, NULL, NULL, ai_iOffset, -1 ) ;
#endif

	iSize = *( (m4uint32_t*)pcString ) ;
	*( (m4uint32_t*)pcString ) = 0 ;

	if( pcString + M4SP_STRING_SIZE + iSize + M4SP_STRING_SIZE == m_pcCurrent )
	{
		// Si es el último se reduce
		m_pcCurrent = pcString ;

		while( m_pcCurrent > m_pcStart )
		{
			pcString = m_pcCurrent - M4SP_STRING_SIZE - *( (m4uint32_t*)( m_pcCurrent - M4SP_STRING_SIZE ) ) - M4SP_STRING_SIZE ;

			if( *( (m4uint32_t*)pcString ) != 0 )
			{	
				break ;
			}
			m_pcCurrent = pcString ;
		}
	}

	return( M4_SUCCESS ) ;
}






//=================================================================================
//
// ClStringPool
//
// Clase que implementa el pool dinámico de cadenas
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClStringPool :: ClStringPool( m4uint32_t ai_PoolSize )
{
	m_iPoolSize = ai_PoolSize * 1024 ;

	if( m_iPoolSize > M4SP_OFFSET_MASK )
	{
		// Máximo offset que se puede direccionar
		m_iPoolSize = M4SP_OFFSET_MASK ;
	}

	m_iLength = 0 ;
	m_iSize = 0 ;
	m_poPoolArray = NULL ;

#ifdef _M4_TRACE_POOL
	m_iAllocation = 0 ;
	m_poVM = NULL ;
#endif
}
		

ClStringPool :: ~ClStringPool( void )
{
	if( m_poPoolArray != NULL )
	{
		delete [] m_poPoolArray ;
	}
}


void ClStringPool :: Reset( void )
{

	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;


	iLength = m_iLength ;
	m_iLength = 0 ;

	for( i = 0 ; i < iLength ; i++ )
	{
		if( m_poPoolArray[ i ].GetSize() > m_iPoolSize )
		{
			m_poPoolArray[ i ].Destroy() ;
		}
		else
		{
			m_poPoolArray[ i ].Reset() ;

			if( m_iLength < i )
			{
				m_poPoolArray[ i ].Move( m_poPoolArray[ m_iLength ] ) ;
			}
			m_iLength++ ;
		}
	}
}



// Funciones de lectura =======================================================

m4uint32_t	ClStringPool :: GetSize( void ) const
{

	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;
	m4uint32_t	iSize = 0 ;


	for( i = 0 ; i < iLength ; i++ )
	{
		iSize += m_poPoolArray[ i ].GetSize() ;
	}

	iSize += sizeof( ClStringPool ) ;
	iSize += m_iSize * sizeof( ClOnePool ) ;

	return( iSize ) ;
}


m4pchar_t	ClStringPool :: GetString( m4uint32_t ai_iOffset ) const
{

	m4uint16_t	iPool = 0 ;


	if( ai_iOffset == -1 )
	{
		// La cadena vacía va en el offset -1
		return( "" ) ;
	}

	iPool = m4uint16_t( ai_iOffset >> M4SP_POOL_START ) ;

	if( iPool >= m_iLength )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_POOL_HANDLE, ai_iOffset << m_iLength ) ;
		return( "" ) ;
	}

	return m_poPoolArray[ iPool ].GetString( ai_iOffset & M4SP_OFFSET_MASK ) ;
}


m4uint32_t	ClStringPool :: GetStringLength( m4uint32_t ai_iOffset ) const
{
	return( strlen( GetString( ai_iOffset ) ) ) ;
}


m4return_t	ClStringPool :: Dump( FILE *ai_poFile ) const
{

	m4uint16_t	i = 0 ;


	if( ai_poFile == NULL )
	{
		return( M4_SUCCESS ) ;
	}
	
	fprintf( ai_poFile, "Length <%d> Size <%d> Initial Size <%d>\n", m_iLength, m_iSize, m_iPoolSize ) ;

	for( i = 0 ; i < m_iLength ; i++ )
	{
		fprintf( ai_poFile, "Pool <%d>\n", i ) ;
		m_poPoolArray[ i ].Dump( ai_poFile ) ;
	}

#ifdef _M4_TRACE_POOL
	map<m4int32_t, StStringAllocation, less<m4int32_t> >::iterator it = m_oAllocatedStrings.begin() ;

	fprintf( ai_poFile, "Leaks\n" ) ;
	while( it != m_oAllocatedStrings.end() )
	{
//		fprintf( ai_poFile, "%d: %s!%s.%s(%d)\n", it->second.m_iAllocation, , it->second.m_acChannel, it->second.m_acNode, it->second.m_acItem, it->second.m_iLine ) ;
		fprintf( ai_poFile, "%d: %d(%d)\n", it->second.m_iAllocation, it->second.m_iHandle, it->second.m_iLine ) ;
		it++;
	}
#endif

	fflush( ai_poFile ) ;

	return( M4_SUCCESS ) ;
}



// Funciones de escritura =====================================================

m4return_t ClStringPool :: AddString( m4pcchar_t ai_pccInString, m4uint32_t &ao_riOutOffset )
{

	m4uint32_t	iLength = 0 ;
	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcString = NULL ;

	
	ao_riOutOffset = -1 ;

	if( ai_pccInString == NULL || *ai_pccInString == '\0' )
	{
		// La cadena nula es vacía y va en el offset -1
		return( M4_SUCCESS ) ;
	}

	iLength = strlen( ai_pccInString ) ;
	iSize = M4_ROUND_TO_4( iLength + 1 ) ;

	if( AllocString( iSize, ai_pccInString, iLength + 1, pcString, ao_riOutOffset ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClStringPool :: AddStringConcat( m4uint32_t ai_iInOffset1, m4uint32_t ai_iInOffset2 , m4uint32_t &ao_riOutOffset )
{

	m4uint32_t	iLength1 = 0 ;
	m4uint32_t	iLength2 = 0 ;
	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcInString1 = NULL ;
	m4pchar_t	pcInString2 = NULL ;
	m4pchar_t	pcOutString = NULL ;


	ao_riOutOffset = -1 ;

	pcInString1 = GetString( ai_iInOffset1 ) ;
	pcInString2 = GetString( ai_iInOffset2 ) ;

	iLength1 = strlen( pcInString1 ) ;
	iLength2 = strlen( pcInString2 ) ;
	iSize = M4_ROUND_TO_4( iLength1 + iLength2 + 1 ) ;

	if( AllocString( iSize, pcInString1, iLength1, pcOutString, ao_riOutOffset ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	memcpy( pcOutString + iLength1, pcInString2, sizeof( m4char_t ) * ( iLength2 + 1 ) ) ;

	DeleteString( ai_iInOffset1 ) ;
	DeleteString( ai_iInOffset2 ) ;

	// Se intenta reallocar para no saturar el pool
	DeleteString( ao_riOutOffset ) ;
	return( AddString( pcOutString, ao_riOutOffset ) ) ;
}


m4return_t	ClStringPool :: SetString( m4uint32_t ai_iInOffset, m4uint32_t &aio_riOutOffset )
{

	m4uint32_t	iLength = 0 ;
	m4uint32_t	iSize = 0 ;
	m4pchar_t	pcInString = NULL ;
	m4pchar_t	pcOutString = NULL ;

	
	if( aio_riOutOffset != -1 )
	{
		pcOutString = GetString( aio_riOutOffset ) ;
		pcInString = GetString( ai_iInOffset ) ;

		iLength = strlen( pcInString ) ;
		iSize = M4_ROUND_TO_4( iLength + 1 ) ;

		if( iSize <= *( (m4uint32_t*)( pcOutString - M4SP_STRING_SIZE ) ) )
		{
			// La nueva cadena cabe donde la vieja
			memcpy( pcOutString, pcInString, sizeof( m4char_t ) * ( iLength + 1 ) ) ;
			DeleteString( ai_iInOffset ) ;
			return( M4_SUCCESS ) ;
		}
	}
	
	DeleteString( aio_riOutOffset ) ;
	aio_riOutOffset = ai_iInOffset ;

	return( M4_SUCCESS ) ;
}


m4return_t	ClStringPool :: DeleteString( m4uint32_t ai_iOffset )
{

	m4uint16_t	iPool = 0 ;


	if( ai_iOffset == -1 )
	{
		// La cadena vacía no se libera
		return( M4_SUCCESS ) ;
	}

	iPool = m4uint16_t( ai_iOffset >> M4SP_POOL_START ) ;

	if( iPool >= m_iLength )
	{
		DUMP_CHLOG_WARNINGF( M4_CH_VM_NO_POOL_HANDLE, ai_iOffset << m_iLength ) ;
		return( M4_ERROR ) ;
	}

	if( m_poPoolArray[ iPool ].DeleteString( ai_iOffset & M4SP_OFFSET_MASK ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

#ifdef _M4_TRACE_POOL
	map<m4int32_t, StStringAllocation, less<m4int32_t> >::iterator	it ;

	it = m_oAllocatedStrings.find( ai_iOffset ) ;

	if( it != m_oAllocatedStrings.end() )
	{
		m_oAllocatedStrings.erase( it ) ;
	}
#endif

	return( M4_SUCCESS ) ;
}



// Funciones auxiliares =======================================================

m4return_t	ClStringPool :: NewString( m4uint16_t ai_iPool, m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset )
{

	ao_rpcOutString = NULL ;
	ao_riOutOffset = -1 ;


	if( m_poPoolArray[ ai_iPool ].NewString( ai_iInSize, ai_pccInString, ai_iInLength, ao_rpcOutString, ao_riOutOffset ) != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Si hay hueco se ajusta el oofset
	ao_riOutOffset |= ( ai_iPool << M4SP_POOL_START ) ;

#ifdef _M4_TRACE_POOL
	if( m_poVM != NULL )
	{
		StStringAllocation	stStringAllocation ;
//		ClCompiledMCR	*poCMCR = m_poVM->m_oState.m_poAccess->GetpCMCR() ;
//		strcpy( stStringAllocation.m_acChannel, poCMCR->GetChannelId() ) ;
//		strcpy( stStringAllocation.m_acNode, poCMCR->GetNodeId( poCMCR->GetItemNodeHandle( m_poVM->m_oState.m_hItem ) ) ) ;
//		strcpy( stStringAllocation.m_acItem, poCMCR->GetItemId( m_poVM->m_oState.m_hItem ) ) ;
		stStringAllocation.m_iAllocation = m_iAllocation++ ;
		stStringAllocation.m_iHandle = m_poVM->m_oState.m_hItem ;
		stStringAllocation.m_iLine = m_poVM->m_oState.m_iSourceLine ;
		m_oAllocatedStrings[ ao_riOutOffset ] = stStringAllocation ;
	}
#endif

	return( M4_SUCCESS ) ;
}


m4return_t	ClStringPool :: AllocString( m4uint32_t ai_iInSize, m4pcchar_t ai_pccInString, m4uint32_t ai_iInLength, m4pchar_t &ao_rpcOutString, m4uint32_t &ao_riOutOffset )
{

	m4return_t	iResult = M4_ERROR ;
	m4int16_t	iFree = -1 ;
	m4int16_t	i = 0 ;
	m4uint32_t	iNewPoolSize = 0 ;
	m4uint16_t	iOldSize = 0 ;
	m4uint16_t	iNewSize = 0 ;
	m4pchar_t	pcStart = NULL ;
	ClOnePool*	poPoolArray ;


	ao_rpcOutString = NULL ;
	ao_riOutOffset = -1 ;

	// Se busca un hueco libre
	iFree = -1 ;
	for( i = 0 ; i < m_iLength ; i++ )
	{
		if( NewString( i, ai_iInSize, ai_pccInString, ai_iInLength, ao_rpcOutString, ao_riOutOffset ) == M4_SUCCESS )
		{
			return( M4_SUCCESS ) ;
		}

		if( m_poPoolArray[ i ].GetLength() == 0 )
		{
			iFree = i ;
		}
	}

	// Si no hay pool disponible hay que crearlo
	if( m_iLength < m_iSize )
	{
		iFree = m_iLength ;
	}
	else
	{
		// Se intenta borrar un pool vacío
		if( iFree != -1 )
		{
			pcStart = m_poPoolArray[ iFree ].Destroy( M4_FALSE ) ;
		}
		else
		{
			// Si no hay pools hay que crearlos
			iOldSize = m_iSize ;

			if( iOldSize > 0 )
			{
				iNewSize = iOldSize * 2 ;
				CHECK_CHLOG_ERRORF( iNewSize > ( M4SP_POOL_MASK >> M4SP_POOL_START ), M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, iNewSize << __LINE__ << __FILE__ ) ;
			}
			else
			{
				iNewSize = 8 ;
			}

			poPoolArray = new ClOnePool[ iNewSize ] ;
			CHECK_CHLOG_ERRORF( poPoolArray == NULL, M4_ERROR, M4_CH_VM_OUT_OF_MEMORY, m4uint32_t( sizeof( ClOnePool ) * iNewSize ) << __LINE__ << __FILE__ ) ;

			if( m_poPoolArray != NULL )
			{
				// Se copia a pelo y se borra sin llamar a los destructores
				memcpy( poPoolArray, m_poPoolArray, iOldSize * sizeof( ClOnePool ) ) ;
				memset( m_poPoolArray, 0, iOldSize * sizeof( ClOnePool ) ) ;
				delete [] m_poPoolArray ;
			}

			m_poPoolArray = poPoolArray ;
			m_iSize = iNewSize ;
			iFree = iOldSize ;
		}
	}

	iNewPoolSize = m_iPoolSize ;

	// Si no hay hueco hay que crearlo
	if( iNewPoolSize < M4SP_STRING_SIZE + ai_iInSize + M4SP_STRING_SIZE )
	{
		// Hay que crear un cacho mayor que el defecto
		iNewPoolSize += ai_iInSize ;
	}

	// Se hace hueco en el libre
	iResult = m_poPoolArray[ iFree ].Create( iNewPoolSize ) ;

	if( iResult != M4_SUCCESS )
	{
		if( pcStart != NULL )
		{
			delete( pcStart ) ;
		}
		return( M4_ERROR ) ;
	}

	if( iFree == m_iLength )
	{
		m_iLength++ ;
	}

	iResult = NewString( iFree, ai_iInSize, ai_pccInString, ai_iInLength, ao_rpcOutString, ao_riOutOffset ) ;

	if( pcStart != NULL )
	{
		delete( pcStart ) ;
	}

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


