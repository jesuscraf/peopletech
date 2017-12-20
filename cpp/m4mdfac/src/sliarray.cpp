
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                sliarray.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                26-05-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo implementa las clases para arrays de fechas
//
//
//==============================================================================



#include "m4facres.hpp"

#include "sliarray.hpp"
#include "chlog.hpp"
#include "cldstack.hpp"





//=============================================================================
//
// ClLinkedSlicedElement
//
// Clase que implementa los elementos de las filas de los arrays de fechas
//
//=============================================================================

// Funciones de inicialización ================================================
	
ClLinkedSlicedElement :: ClLinkedSlicedElement( void )
{
	m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	m_iNext = -1 ;
	m_iPrevious = -1 ;
	m_poElement = NULL ;
}



//=============================================================================
//
// ClSliceRow
//
// Clase que implementa las filas de los arrays de fechas
//
//=============================================================================


m4return_t	ClSliceRow :: _Increment( void )
{

	m4uint16_t				iSize ;
	ClLinkedSlicedElement   *poTmp ;


	iSize = m_iSize + m_iDelta ;

	if( iSize == 0 )
	{
		iSize = 1 ;
	}

	poTmp = new ClLinkedSlicedElement[ iSize ] ;
	CHECK_CHLOG_ERRORF( poTmp == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClLinkedSlicedElement ) * iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = iSize ;


	if( m_poElements != NULL )
	{
		if( m_iLength > 0 )
		{
			memcpy( poTmp, m_poElements, sizeof( ClLinkedSlicedElement ) * m_iLength ) ;
		}

		delete [] m_poElements ;
	}

	m_poElements = poTmp ;
		
	return( M4_SUCCESS ) ;
}



// Funciones de inicialización ================================================
	
ClSliceRow :: ClSliceRow( void )
{
	m_dUnitDate = M4_ONE_DAY ;
	m_iSize = 0 ;
	m_iLength = 0 ;
    m_iDelta = M4CL_DEFAULT_DELTA ;
	m_iFirst = -1 ;
	m_iLast = -1 ;
	m_poElements = NULL ;
	m_poLeader = NULL ;
}


ClSliceRow :: ~ClSliceRow( void )
{
	if( m_poElements != NULL )
	{
		delete [] m_poElements ;
		m_poElements = NULL ;
	}
}



// Otras funciones de escritura ===============================================

void    ClSliceRow :: InsertElement( m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4int16_t ai_iNext, m4int16_t ai_iPrevious, ClSlicedElement *ai_poElement )
{
	m_poElements[ m_iLength ].m_dStartDate = ai_dStartDate ;
	m_poElements[ m_iLength ].m_dEndDate = ai_dEndDate ;
	m_poElements[ m_iLength ].m_iNext = ai_iNext ;
	m_poElements[ m_iLength ].m_iPrevious = ai_iPrevious ;
	m_poElements[ m_iLength ].m_poElement = ai_poElement ;

	if( ai_iPrevious == -1 )
	{	// Es el primero
		m_iFirst = m_iLength ;
	}
	else
	{	// No es el primero
		m_poElements[ ai_iPrevious ].m_iNext = m_iLength ;
	}

	if( ai_iNext == - 1 )
	{	// Es el último
		m_iLast = m_iLength ;
	}
	else
	{	// No es el último
		m_poElements[ ai_iNext ].m_iPrevious = m_iLength ;
	}

	m_iLength++ ;
}


m4return_t	ClSliceRow :: AddElement( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo, m4bool_t &ai_rbAdded )
{

    m4return_t  iResult ;
	m4bool_t	bFinnished ;
	m4bool_t	bFound ;
	m4int16_t	iNext ;
	m4int16_t	iPrevious ;
	m4date_t	dStartDate ;
	m4date_t	dEndDate ;
	m4date_t	dLocalEndDate ;


	ai_rbAdded = M4_FALSE ;

	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}


	dStartDate = ai_poElement->GetStartDate() ;
	dEndDate = ai_poElement->GetEndDate() ;

	bFinnished = M4_FALSE ;

	while( bFinnished == M4_FALSE )
	{
		bFound = M4_FALSE ;

		iNext = m_iFirst ;
		while( bFound == M4_FALSE && iNext != -1 )
		{
			if( dStartDate <= m_poElements[ iNext ].m_dEndDate )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNext = m_poElements[ iNext ].m_iNext ;
			}
		}

		if( bFound == M4_TRUE )
		{
			if( dEndDate >= m_poElements[ iNext ].m_dStartDate )
			{
				// Si hay overlap

				iResult = ai_poElement->CheckOverLap( m_poElements[ iNext ].m_poElement, dStartDate, dEndDate, ai_pvInfo ) ;

				if( iResult != M4_SUCCESS )
				{
					return( M4_ERROR ) ;
				}

				if( dEndDate <= m_poElements[ iNext ].m_dEndDate )
				{
					// Cabe entero después de cortarlo

					bFinnished = M4_TRUE ;
					dLocalEndDate = m_poElements[ iNext ].m_dStartDate - m_dUnitDate ;
					if( dStartDate <= dLocalEndDate  )
					{
						// Si después de cortar queda algo que añadir
						iPrevious = m_poElements[ iNext ].m_iPrevious ;
						InsertElement( dStartDate, dLocalEndDate, iNext, iPrevious, ai_poElement ) ;
						ai_rbAdded = M4_TRUE ;
					}
				}
				else
				{
					// No cabe entero y hay que continuar

					dLocalEndDate = m_poElements[ iNext ].m_dStartDate - m_dUnitDate ;
					if( dStartDate <= dLocalEndDate )
					{
						// Si después de cortar queda algo que añadir
						iPrevious = m_poElements[ iNext ].m_iPrevious ;
						InsertElement( dStartDate, dLocalEndDate, iNext, iPrevious, ai_poElement ) ;
						ai_rbAdded = M4_TRUE ;
					}

					// Seguimos con el cacho que falta
					dStartDate = m_poElements[ iNext ].m_dEndDate + m_dUnitDate ;
				}
			}
			else
			{
				// Si no hay overlap
				// Es del medio y cabe entero
				bFinnished = M4_TRUE ;
				iPrevious = m_poElements[ iNext ].m_iPrevious ;
				InsertElement( dStartDate, dEndDate, iNext, iPrevious, ai_poElement ) ;
				ai_rbAdded = M4_TRUE ;
			}
		}
		else
		{
			// Es el último y cabe entero
			bFinnished = M4_TRUE ;
			InsertElement( dStartDate, dEndDate, -1, m_iLast, ai_poElement ) ;
			ai_rbAdded = M4_TRUE ;
		}
	}

	if( m_poLeader == NULL )
	{
		m_poLeader = ai_poElement ;
	}


	return( M4_SUCCESS ) ;
}



// Funciones varias ===========================================================

ClSlicedElement*	ClSliceRow :: Find( m4date_t ai_dDate )
{

	m4bool_t	bFound ;
	m4int16_t	iNext ;

	
	bFound = M4_FALSE ;

	iNext = m_iFirst ;
	while( bFound == M4_FALSE && iNext != -1 )
	{
		if( ai_dDate < m_poElements[ iNext ].m_dStartDate )
		{
			// Estaría bien optimizarlo
			iNext = -1 ;
		}
		else
		{
			if( ai_dDate <= m_poElements[ iNext ].m_dEndDate && ai_dDate >= m_poElements[ iNext ].m_dStartDate )
			{
				bFound = M4_TRUE ;
			}
			else
			{
				iNext = m_poElements[ iNext ].m_iNext ;
			}
		}
	}

	if( bFound == M4_FALSE )
	{
		return( NULL ) ;
	}

	return( m_poElements[ iNext ].m_poElement ) ;
}



//=============================================================================
//
// ClSliceArray
//
// Clase que implementa los arrays de fechas
//
//=============================================================================

m4return_t	ClSliceArray :: _Delete( void )
{

	m4return_t	    iResult ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4pvoid_t	    pvPointer ;
    ClSliceRow      *poRow ;

    
    iLength = m_oRows.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oRows.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRow = ( ClSliceRow* ) pvPointer ;
        delete( poRow ) ;
	}

	if( m_ppoTable != NULL )
	{
		delete [] m_ppoTable ;
		m_ppoTable = NULL ;
	}

    return( M4_SUCCESS ) ;
}



m4return_t	ClSliceArray :: _MoveUp( m4criteriaf_t ai_fFunction, m4pvoid_t *ai_ppvElements )
{

    m4return_t	iResult ;
	m4bool_t	bFound ;
	m4bool_t	bFulfill ;
	m4uint16_t	i ;
	m4uint16_t	iActual ;
	m4pvoid_t*	ppvPointer ;

    
	iActual = 0 ;
	bFound = M4_FALSE ;
	
	for( i = m_iStart ; i < m_iEnd ; i++ )
	{
		iResult = m_oRows.GetElementPointer( i, ppvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || *ppvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		bFulfill = ai_fFunction( ppvPointer ) ;

		if( bFulfill == M4_TRUE )
		{
			ai_ppvElements[ iActual++ ] = *ppvPointer ;
			bFound = M4_TRUE ;
		}
	}

	if( bFound == M4_TRUE )
	{
		for( i = m_iStart ; i < m_iEnd ; i++ )
		{
			iResult = m_oRows.GetElementPointer( i, ppvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || *ppvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			bFulfill = ai_fFunction( ppvPointer ) ;

			if( bFulfill == M4_FALSE )
			{
				ai_ppvElements[ iActual++ ] = *ppvPointer ;
			}
		}

		for( i = m_iStart ; i < m_iEnd ; i++ )
		{
			iResult = m_oRows.SetElement( i, ai_ppvElements[ i - m_iStart ] ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}


	return( M4_SUCCESS ) ;
}



// Funciones de inicialización ================================================

ClSliceArray :: ClSliceArray( void )
{
	m_dUnitDate = M4_ONE_DAY ;
    m_iColumnDelta = M4CL_DEFAULT_DELTA ;

	m_iSize = 0 ;
	m_iStart = 0 ;
	m_iEnd = 0 ;
	m_iDelta = M4CL_DEFAULT_DELTA ;
	m_ppoTable = NULL ;
}


ClSliceArray :: ~ClSliceArray( void )
{
	_Delete() ;
}


m4return_t	ClSliceArray :: Reset( void )
{

	m4return_t	    iResult ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4pvoid_t	    pvPointer ;
    ClSliceRow      *poRow ;

    
    iLength = m_oRows.GetLength() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oRows.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRow = ( ClSliceRow* ) pvPointer ;
		poRow->Reset() ;
	}

// Dejamos el mismo tamaño con los leaders a null y ajustamos la ventana
//	m_oRows.Reset() ;
	m_iStart = 0 ;
	m_iEnd = 0 ;

    return( M4_SUCCESS ) ;
}



// Funciones de lectura de la parte fija ======================================

ClSlicedElement*	ClSliceArray :: GetLeaderNoStart( m4uint16_t ai_iPosition )
{

    m4return_t			iResult ;
	m4pvoid_t			pvPointer ;
	ClSliceRow			*poRow ;


	iResult = m_oRows.GetElement( ai_iPosition, pvPointer ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poRow = ( ClSliceRow* ) pvPointer ;

	return( poRow->GetLeader() ) ;
}



// Funciones de escritura de parte fija =======================================

m4return_t	ClSliceArray :: AddElement( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo, m4bool_t &ai_rbAdded )
{

    m4return_t			iResult ;
	m4bool_t			bFound ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4pvoid_t			pvPointer ;
	ClSliceRow			*poRow ;
	ClSlicedElement		*poLeader ;


	ai_rbAdded = M4_FALSE ;

	CHECK_CHLOG_DEBUGF( ai_poElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	bFound = M4_FALSE ;
	iLength = m_oRows.GetLength() ;

	for( i = m_iStart ; i < iLength && bFound == M4_FALSE ; i++ )
	{
		iResult = m_oRows.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRow = ( ClSliceRow* ) pvPointer ;

		poLeader = poRow->GetLeader() ;

		if( poLeader == NULL )
		{
			// Si el leader es 0 ya lo hemos encontrado, porque está vacío
			bFound = M4_TRUE ;
			m_iEnd = i + 1 ;
			continue ;
		}

		if( ai_poElement->IsEqual( poLeader ) == M4_TRUE )
		{
			bFound = M4_TRUE ;

			// Comprobamos que sean datos consistentes
			iResult = ai_poElement->CheckConsistency( poLeader, ai_pvInfo ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	if( bFound == M4_FALSE )
	{
		poRow = new ClSliceRow ;
		CHECK_CHLOG_ERRORF( poRow == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRow->Init( m_iColumnDelta, m_dUnitDate ) ;

		iResult = m_oRows.AddElement( poRow ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClSliceRow ) ) << __LINE__ << __FILE__ ) ;

		m_iEnd++ ;
	}

	iResult = poRow->AddElement( ai_poElement, ai_pvInfo, ai_rbAdded ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}


	return( M4_SUCCESS ) ;
}


// Funciones varias ===========================================================

void	ClSliceArray :: MoveToFirstBlock( void )
{

	m4return_t	    iResult ;
	m4uint16_t	    i ;
	m4pvoid_t	    pvPointer ;
    ClSliceRow      *poRow ;

	
	for( i = m_iStart ; i < m_iEnd ; i++ )
	{
		iResult = m_oRows.GetElement( i, pvPointer ) ;

		if( iResult != M4_SUCCESS || pvPointer == NULL )
		{
			continue ;
		}

		poRow = ( ClSliceRow* ) pvPointer ;
		poRow->Reset() ;
	}

	m_iEnd = m_iStart ;
	m_iStart = 0 ;
}


m4return_t	ClSliceArray :: GetArrayTable( ClSlicedElement** &ao_rppTable, m4uint16_t &ao_riColums, m4uint16_t &ao_riRows, ClDateStack *ao_poStack, m4uint16_t &ao_riNotNullColums, m4bool_t ai_bAll )
{

    m4return_t				iResult ;
	m4bool_t				bNotNull ;
	m4uint16_t				iStart ;
	m4uint16_t				i, j ;
	m4uint16_t				iColumns ;
	m4uint16_t				iPosition ;
	m4uint16_t				iSize ;
	m4date_t				dStartDate ;
	m4date_t				dEndDate ;
	m4pvoid_t				pvPointer ;
	ClSliceRow				*poRow ;
	ClSlicedElement			*poElement ;
	ClLinkedSlicedElement	*poLinkedElement ;


	ao_poStack->Clear() ;

	if( ai_bAll == M4_TRUE )
	{
		iStart = 0 ;
	}
	else
	{
		iStart = m_iStart ;
	}

	for( i = iStart ; i < m_iEnd ; i++ )
	{
		iResult = m_oRows.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poRow = ( ClSliceRow* ) pvPointer ;

		iColumns = poRow->GetLength() ;

		for( j = 0 ; j < iColumns ; j++ )
		{
			iResult = poRow->GetElement( j, poLinkedElement ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || poLinkedElement == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			dStartDate = poLinkedElement->m_dStartDate ;
			dEndDate = poLinkedElement->m_dEndDate ;

			iResult = ao_poStack->AddElement( dStartDate ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			iResult = ao_poStack->AddElement( dEndDate + m_dUnitDate ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
	}

    iResult = ao_poStack->Reduce() ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    ao_riColums = ao_poStack->GetLength() ;

	iResult = ao_poStack->AddEndDate( m_dUnitDate ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	ao_riRows = m_iEnd - iStart ;

	iSize = ao_riRows * ao_riColums ;

	if( iSize > m_iSize )
	{
		if( m_ppoTable != NULL )
		{
			delete [] m_ppoTable ;
		}

		if( m_iSize + m_iDelta > iSize )
		{
			iSize = m_iSize + m_iDelta ;
		}

		m_ppoTable = new ClSlicedElement* [ iSize ] ;
		CHECK_CHLOG_ERRORF( m_ppoTable == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( ClSlicedElement* ) * iSize ) << __LINE__ << __FILE__ ) ;

		m_iSize = iSize ;
	}


	ao_riNotNullColums = 0 ;
	iPosition = 0 ;

	for( i = 0 ; i < ao_riColums ; i++ )
	{
		iResult = ao_poStack->GetElement( i, dStartDate ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		bNotNull = M4_FALSE ;

		for( j = iStart ; j < m_iEnd ; j++ )
		{
			iResult = m_oRows.GetElement( j, pvPointer ) ;
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

			poRow = ( ClSliceRow* ) pvPointer ;

			poElement = poRow->Find( dStartDate ) ;

			if( poElement != NULL )
			{
				bNotNull = M4_TRUE ;
			}

			m_ppoTable[ iPosition++ ] = poElement ;
		}

		if( bNotNull == M4_TRUE )
		{
			ao_riNotNullColums++ ;
		}
	}

	ao_rppTable = m_ppoTable ;

	return( M4_SUCCESS ) ;
}


/*
Esta función reordena el array, de tal forma que pone primero los elementos
que cumplen un criterio y después los que no lo cumplen
Para ello primero crea un array auxiliar del tamaño de los elementos a tratar
Después recorre los elementos originales y va poniendo en el array auxiliar
los elementos que cumplen el criterio
Después recorre los elementos originales y va poniendo en el array auxiliar
los elementos que no cumplen el criterio
Al final se hace una nueva pasada asignando los valores en el orden definitivo
*/
m4return_t	ClSliceArray :: MoveUp( m4criteriaf_t ai_fFunction )
{

    m4return_t	iResult ;
	m4uint16_t	iLength ;
	m4pvoid_t	*ppvElements ;

    
	iLength = m_iEnd - m_iStart ;

	ppvElements = new m4pvoid_t[ iLength ] ;
	CHECK_CHLOG_ERRORF( ppvElements == NULL, M4_ERROR, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4pvoid_t ) * iLength ) << __LINE__ << __FILE__ ) ;

	iResult = _MoveUp( ai_fFunction, ppvElements ) ;

	delete [] ppvElements ;

	return( iResult ) ;
}


