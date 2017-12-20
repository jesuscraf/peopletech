
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                sliarray.hpp   
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



#include "clpstack.hpp"
#include "cldefine.hpp"
#include "cldates.hpp"



#ifndef __SLIARRAY_HPP__
#define __SLIARRAY_HPP__


//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClDateStack ;


//=============================================================================
// Definiciones
//=============================================================================

// Funcion de que chequea si un valor cunple un criterio o no
typedef	m4bool_t( *m4criteriaf_t )( const void* ) ;


//=============================================================================
//
// ClSlicedElement
//
// Clase que define el interfaz de los elementos fechados
//
//=============================================================================

class	ClSlicedElement
{

public:

virtual	m4date_t	GetStartDate( void ) const = 0 ;
virtual	m4date_t	GetEndDate( void ) const = 0 ;

virtual	m4bool_t	IsEqual( ClSlicedElement *ai_poElement ) = 0 ;
virtual	m4return_t	CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo ) = 0 ;
virtual	m4return_t	CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo ) = 0 ;

} ;



//=============================================================================
//
// ClLinkedSlicedElement
//
// Clase que implementa los elementos de las filas de los arrays de fechas
//
//=============================================================================

class	ClLinkedSlicedElement
{

public:

	m4date_t			m_dStartDate ;
	m4date_t			m_dEndDate ;
	m4int16_t			m_iNext ;
	m4int16_t			m_iPrevious ;
	ClSlicedElement		*m_poElement ;

// Funciones de inicialización ================================================
	
		ClLinkedSlicedElement( void ) ;
		~ClLinkedSlicedElement( void ) {} ;
	
} ;



//=============================================================================
//
// ClSliceRow
//
// Clase que implementa las filas de los arrays de fechas
//
//=============================================================================

class	ClSliceRow
{

protected:

	m4date_t				m_dUnitDate ;
	m4uint16_t				m_iSize ;
	m4uint16_t				m_iLength ;
	m4uint16_t				m_iDelta ;
	m4int16_t				m_iFirst ;
	m4int16_t				m_iLast ;
	ClLinkedSlicedElement	*m_poElements ;

	ClSlicedElement			*m_poLeader ;


	m4return_t	_Increment( void ) ;

public:

// Funciones de inicialización ================================================
	
		ClSliceRow( void ) ;
		~ClSliceRow( void ) ;

	void    Init( m4uint16_t ai_iDelta, m4date_t ai_dTimeUnit )
    {
        m_iDelta = ai_iDelta ;
		m_dUnitDate = ai_dTimeUnit ;
    }

	void	Reset( void )
	{
		m_iLength = 0 ;
		m_iFirst = -1 ;
		m_iLast = -1 ;
		m_poLeader = NULL ;
	}

// Funciones de lectura de la parte fija ======================================

	m4uint16_t	GetLength( void ) const
	{
		return( m_iLength ) ;
	}

	ClSlicedElement*	GetLeader( void ) const
	{
		return( m_poLeader ) ;
	}


// Otras funciones de lectura =================================================

	m4return_t	GetElement( m4uint16_t ai_iPosition, ClLinkedSlicedElement* &ao_rpoElement ) const
	{

		m4return_t  iResult ;


		if( ai_iPosition >= m_iLength )
		{
			DUMP_CHLOG_ERRORF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;
			iResult = M4_ERROR ;
		}
		else
		{
			ao_rpoElement = &( m_poElements[ ai_iPosition ] ) ;
			iResult = M4_SUCCESS ;
		}

   		return( iResult ) ;
	}


// Otras funciones de escritura ===============================================

    void		InsertElement( m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4int16_t ai_iNext, m4int16_t ai_iPrevious, ClSlicedElement *ai_poElement ) ;
    m4return_t	AddElement( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo, m4bool_t &ai_rbAdded ) ;


// Funciones varias ===========================================================

	ClSlicedElement*	Find( m4date_t ai_dDate ) ;

} ;



//=============================================================================
//
// ClSliceArray
//
// Clase que implementa los arrays de fechas
//
//=============================================================================

class	ClSliceArray
{

protected:

	m4date_t		m_dUnitDate ;
	m4uint16_t		m_iColumnDelta ;
	ClPointerStack	m_oRows ;

	m4uint16_t		m_iSize ;
	m4uint16_t		m_iStart ;
	m4uint16_t		m_iEnd ;
	m4uint16_t		m_iDelta ;
	ClSlicedElement	**m_ppoTable ;


	m4return_t	_Delete( void ) ;
	m4return_t	_MoveUp( m4criteriaf_t ai_fFunction, m4pvoid_t *ai_ppvElements ) ;

public:


// Funciones de inicialización ================================================
	
		ClSliceArray( void ) ;
		~ClSliceArray( void ) ;

    void    Init( m4uint16_t ai_iColumnDelta, m4uint16_t ai_iRowDelta, m4uint16_t ai_iTableDelta, m4date_t ai_dTimeUnit )
    {
        m_iColumnDelta = ai_iColumnDelta ;
		m_oRows.SetDelta( ai_iRowDelta ) ;
		m_iDelta = ai_iTableDelta ;
		m_dUnitDate = ai_dTimeUnit ;
    }

	m4return_t	Reset( void ) ;


// Funciones de lectura de la parte fija ======================================

	m4uint16_t	GetStart( void ) const
	{
		return( m_iStart ) ;
	}

	m4uint16_t	GetLength( void ) const
	{
		return( m_iEnd - m_iStart ) ;
	}

	ClSlicedElement*	GetLeader( m4uint16_t ai_iPosition )
	{
		return( GetLeaderNoStart( m_iStart + ai_iPosition ) ) ;
	}

	ClSlicedElement*	GetLeaderNoStart( m4uint16_t ai_iPosition ) ;


// Funciones de escritura de parte fija =======================================

    m4return_t	AddElement( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo, m4bool_t &ai_rbAdded ) ;


// Funciones varias ===========================================================

	void	MoveToFirstBlock( void ) ;

	// Se posiciona en la segunda ventana
	void	MoveToSecondBlock( void )
	{
		m_iStart = m_iEnd ;
	}

	m4return_t	GetArrayTable( ClSlicedElement** &ao_rppTable, m4uint16_t &ao_riColums, m4uint16_t &ao_riRows, ClDateStack *ao_poStack, m4uint16_t &ao_riNotNullColums, m4bool_t ai_bAll ) ;

	void	Sort( m4comparef_t ai_fFunction )
	{
		m_oRows.Sort( ai_fFunction, m_iStart, m_iEnd ) ;
	}

	// Mueve a las primeras posiciones los valores que cumplen un criterio
	m4return_t	MoveUp( m4criteriaf_t ai_fFunction ) ;

} ;


#endif

