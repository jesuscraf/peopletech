//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                spoolstr.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                13-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Fichero de implementación de la clase de alamcen de cadenas
////
//==============================================================================



#include "spoolstr.h"
#include "clgeniod.hpp"



int	M4ClCompareNoUpperStrings( m4pcvoid_t ai_pcvString1, m4pcvoid_t ai_pcvString2 )
{

	m4pcchar_t	pccString1 = *( m4pchar_t* ) ai_pcvString1 ;
	m4pcchar_t	pccString2 = *( m4pchar_t* ) ai_pcvString2 ;

    
	if( *pccString1 != *pccString2 )
	{
		return( *pccString1 - *pccString2 ) ;
	}

	if( *pccString1 == 0 )
	{
		return( 0 ) ;
	}
	m4uchar_t uiSize1 = (*pccString1) ;
	return( memcmp( pccString1 + 1, pccString2 + 1, uiSize1) ) ;
}




ClStaticPoolStr::ClStaticPoolStr( m4uint32_t ai_InitialKb, m4uint8_t ai_iCaseConvert )
{
	m_iTopOffset = 0 ;
	m_iMaxSize = 0 ;
	m_iInitialSize = ai_InitialKb * 1024 ;
	m_iCaseConvert = ai_iCaseConvert ;
	m_pcBase = NULL ;

	m_oReferences.SetDelta( m4uint16_t( ai_InitialKb ) * 10 ) ;
}


ClStaticPoolStr::~ClStaticPoolStr( void )
{
	m_iTopOffset = 0 ;
	m_iMaxSize = 0 ;

	if( m_pcBase != NULL )
    {
        delete [] m_pcBase ;
        m_pcBase = NULL ;
    }
}


void ClStaticPoolStr::Reset( void )
{
	if( m_pcBase != NULL )
	{
		m_iTopOffset = sizeof( m4char_t ) * 2 ;
	}
	else
	{
		m_iTopOffset = 0 ;	
	}
	m_oReferences.Reset() ;
}


m4uint32_t ClStaticPoolStr::Clone( ClStaticPoolStr* ai_poPool )
{

	m4pchar_t   pccTemp ;


    m_iTopOffset = ai_poPool->m_iTopOffset ;

    if( m_iTopOffset > m_iMaxSize )
    {
        pccTemp = new m4char_t[ m_iTopOffset ] ;
		CHECK_CHLOG_DEBUGF( pccTemp == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * m_iTopOffset ) << __LINE__ << __FILE__ ) ;

        m_iMaxSize = m_iTopOffset ;

	    if( m_pcBase != NULL )
        {
            delete [] m_pcBase ;
        }
	    m_pcBase = pccTemp ;
    }

    memcpy( m_pcBase, ai_poPool->m_pcBase, m_iTopOffset ) ;
	m_oReferences.Reset() ;

    return( M4_SUCCESS ) ;
}


m4uint32_t ClStaticPoolStr::GetTotalSize( m4uint32_t& ao_riStatic, m4uint32_t& ao_riPool, m4uint32_t& ao_riPointers )
{

	m4uint32_t	iStatic ;
	m4uint32_t	iPointers ;


	ao_riStatic = 0 ;
	ao_riPool = 0 ;
	ao_riPointers = 0 ;


	ao_riStatic += sizeof( m4uint32_t ) ;		// m_iTopOffset
	ao_riStatic += sizeof( m4uint32_t ) ;		// m_iMaxSize
	ao_riStatic += sizeof( m4uint32_t ) ;		// m_iInitialSize
	ao_riStatic += sizeof( ClPointerStack ) ;	// m_oReferences
	ao_riStatic += sizeof( m4uint8_t ) ;		// m_iCaseConvert

	ao_riPool = m_iMaxSize ;

	ao_riPointers = m_oReferences.GetTotalSize( iStatic, iPointers ) ;

	return( ao_riStatic + ao_riPool + ao_riPointers ) ;
}


m4return_t ClStaticPoolStr::ResizePool( m4uint32_t ai_iSize )
{

	m4pchar_t	pcTempBase ;
	m4uint32_t	iNewSize  ;


	iNewSize = m_iMaxSize + ai_iSize ;
	
	if( m_pcBase == NULL )
	{
		/* Bug  0131732
		El tamaño del 0 inicial son 2, para tener en cuenta la longitud
 		*/
		iNewSize += sizeof( m4char_t ) * 2 ;
	}

	pcTempBase = new m4char_t[ iNewSize ] ;
	CHECK_CHLOG_DEBUGF( pcTempBase == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iNewSize ) << __LINE__ << __FILE__ ) ;

	if( m_pcBase != NULL )
	{
		memcpy( pcTempBase, m_pcBase, sizeof( m4char_t ) * m_iMaxSize ) ;
        delete [] m_pcBase ;
	}
	else
	{
		/* Bug 0071031
		Hay que poner el tamaño de la cadena vacía por delante
		*/
		pcTempBase[ 0 ] = '\0' ;
		pcTempBase[ 1 ] = '\0' ;
		m_iTopOffset = sizeof( m4char_t ) * 2 ;
	}

	m_iMaxSize = iNewSize ;
	m_pcBase = pcTempBase ;

    return( M4_SUCCESS ) ;
}


//	Bueno, pues me voy a picar de nuevo esta función porque no me gusta
//	Ha tenido tantos cambios que es un cristo
//	El tema principal es optimizar la carga del metacanal evitando
//	llamadas a strcmpi
//	Para ello voy a hacer una historieta. Cuando llege una cadena más pequeña de
//	255 (1byte), voy a hacer una copia en local con el tamaño en el primer
//	caracter y despues la cadena
//	En este caso hago una búsqueda a ver si ya estaba la cadena
//	En la búsqueda se mira primero el tamaño. Si son distintos no se compara
//	Si son iguales y cer tampoco, porque la probabilidad es pequeña
//	Si es mayor no hago la copia y le pongo un cero sin hacer la búsqueda
//	Con esto gasto 1 byte más por cadena, pero bueno
//	Ahora además, la búsqueda me da la posición dende debería estar la cadena si
//	es que no está. COn ello, más tarde la inserto a capón, y no tengo que
//	ordenar de cada vez
m4return_t ClStaticPoolStr::AddString( m4pcchar_t ai_pccString, m4uint32_t &ao_iOffset )
{
	m4return_t	iResult ;
	m4uint16_t	iPosition ;
	m4uint32_t	iLength ;
	m4uint32_t	iSize ;
	m4uint32_t	iIncrement ;
	m4uint32_t	iMissing ;

	m4pchar_t	*ppcElement ;
	m4pchar_t	pccStart ;

	m4pchar_t	pcString ;
	m4char_t	acString[ 0x00FF + 2 ] ;



// para que solo haya una cadena nula

    if( ai_pccString == NULL || *ai_pccString == '\0' )
    {
		/* Bug 0071031
		Hay que poner el tamaño de la cadena vacía por delante
		*/
        ao_iOffset = 1 ;

		if( m_iTopOffset == 0 )
		{
			iResult = ResizePool( m_iInitialSize ) ;	

			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * m_iInitialSize ) << __LINE__ << __FILE__ ) ;
		}
		return( M4_SUCCESS ) ;
    }


// si ya está dentro no hay que añadir nada

	if( ai_pccString >= m_pcBase && ai_pccString < m_pcBase + m_iTopOffset )
	{
        ao_iOffset = ai_pccString - m_pcBase ;
    	return( M4_SUCCESS ) ;
	}


	iLength = strlen( ai_pccString ) ;
	iSize = iLength + 1 ;
	iIncrement = iSize + 1 ;

	if( iLength <= 0x00FF )
	{
		*acString = m4uint8_t( iLength ) ;
		memcpy( acString + 1, ai_pccString, iSize ) ;
		pcString = acString ;

		if( m_iCaseConvert == M4CL_UPPERCASE )
		{
			strupr( acString + 1 ) ;
		}

// si hay alguna igual tampoco

		ppcElement = ( m4pchar_t* ) m_oReferences.Find( M4ClCompareNoUpperStrings, &pcString, iPosition ) ;

		if( ppcElement != NULL )
		{
			ao_iOffset = *ppcElement - m_pcBase + 1 ;
			return( M4_SUCCESS ) ;
		}
	}
	else
	{
		iLength = 0 ;
		iPosition = 0 ;
	}


//Miramos si hay que hacer un ResizePool

	if( m_iTopOffset + iIncrement > m_iMaxSize )
	{
		iMissing = m_iTopOffset + iIncrement - m_iMaxSize ;

		if( iMissing < m_iInitialSize )
		{
			iMissing = m_iInitialSize ;
		}

		// Nos gurdamos el puntero original
		pccStart = m_pcBase ;

		//Hacemos el ResizePool
		iResult = ResizePool( iMissing ) ;	

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iMissing ) << __LINE__ << __FILE__ ) ;

// actualizamos los offsets de las referencias

		m_oReferences.ReAllocatePointers( m_pcBase - pccStart ) ;
	}


	*( m_pcBase + m_iTopOffset ) = m4uint8_t( iLength ) ;
	memcpy( m_pcBase + m_iTopOffset + 1, ai_pccString, sizeof( m4char_t ) * iSize ) ;


    if( m_iCaseConvert == M4CL_UPPERCASE )
    {
        strupr( m_pcBase + m_iTopOffset + 1 ) ;
    }


	m_oReferences.InsertElement( m_pcBase + m_iTopOffset, iPosition ) ;

	ao_iOffset = m_iTopOffset + 1 ;
	m_iTopOffset += iIncrement ; //avanzamos el offset que indica el final de lo ocupado


	return( M4_SUCCESS );
}



m4return_t	ClStaticPoolStr::Serialize( ClGenericIODriver& IOD ) const
{

	m4return_t	iResult = M4_SUCCESS ;


	iResult = IOD.Write( m_iTopOffset ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_iCaseConvert ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Write( m_pcBase, m_iTopOffset ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS );
}


m4return_t	ClStaticPoolStr::DeSerialize( ClGenericIODriver& IOD )
{

	m4return_t	iResult = M4_SUCCESS ;


	iResult = IOD.Read( m_iTopOffset ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( m_iCaseConvert ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	m_iMaxSize = m_iTopOffset + 1 ;

    m_pcBase = new m4char_t[ m_iMaxSize ] ;
	CHECK_CHLOG_DEBUGF( m_pcBase == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * m_iTopOffset ) << __LINE__ << __FILE__ ) ;

	iResult = IOD.Read( m_pcBase, m_iTopOffset ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS );
}


