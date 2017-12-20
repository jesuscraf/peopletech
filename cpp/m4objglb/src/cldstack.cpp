//## begin module%3912A6BD026A.cm preserve=no
//## end module%3912A6BD026A.cm

//## begin module%3912A6BD026A.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3912A6BD026A.cp

//## Module: cldstack%3912A6BD026A; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\cldstack.cpp

//## begin module%3912A6BD026A.additionalIncludes preserve=no
//## end module%3912A6BD026A.additionalIncludes

//## begin module%3912A6BD026A.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                cldstack.cpp
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
//    This module defines the fucntions of the stack for dates slices
//
//
//==============================================================================




#include "cldstack.hpp"
#include "m4objglb.hpp"
#include <math.h>
//## end module%3912A6BD026A.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A6BD026A.declarations preserve=no
//## end module%3912A6BD026A.declarations

//## begin module%3912A6BD026A.additionalDeclarations preserve=yes



// Funcion M4ClCompareDate ====================================================

int	M4ClCompareDate( m4pcvoid_t ai_pcvDate1, m4pcvoid_t ai_pcvDate2 )
{

	int			iResult ;
	m4date_t	dDifference = *( ( m4date_t* )ai_pcvDate1 ) - *( ( m4date_t* )ai_pcvDate2 ) ;


	if( fabs( dDifference ) <= M4_LN4_MATH_PRECISION )
	{
		iResult = 0 ;
	}
	else if( dDifference > 0 )
	{
		iResult = 1 ;
	}
	else
	{
		iResult = -1 ;
	}

	return( iResult ) ;
}



//=================================================================================
//
// ClDateStack
//
// Clase de array de fechas
//
//=================================================================================

//## end module%3912A6BD026A.additionalDeclarations


// Class ClDateStack 







ClDateStack::ClDateStack (void )
  //## begin ClDateStack::ClDateStack%-241389089.hasinit preserve=no
  //## end ClDateStack::ClDateStack%-241389089.hasinit
  //## begin ClDateStack::ClDateStack%-241389089.initialization preserve=yes
  //## end ClDateStack::ClDateStack%-241389089.initialization
{
  //## begin ClDateStack::ClDateStack%-241389089.body preserve=yes
	m_dStartDate = 0 ;
	m_dEndDate = 0 ;
	m_iLength = 0 ;
	m_iSize = 0 ;
	m_iDelta = M4CL_DEFAULT_DELTA ;
	m_pdElements = NULL ;
  //## end ClDateStack::ClDateStack%-241389089.body
}

ClDateStack::ClDateStack (const ClDateStack& ai_oRhs)
  //## begin ClDateStack::ClDateStack%-803545528.hasinit preserve=no
  //## end ClDateStack::ClDateStack%-803545528.hasinit
  //## begin ClDateStack::ClDateStack%-803545528.initialization preserve=yes
  //## end ClDateStack::ClDateStack%-803545528.initialization
{
  //## begin ClDateStack::ClDateStack%-803545528.body preserve=yes
	m_dStartDate = ai_oRhs.m_dStartDate ;
	m_dEndDate = ai_oRhs.m_dEndDate ;
	m_iLength = ai_oRhs.m_iLength ;
	m_iSize = ai_oRhs.m_iSize ;
	m_iDelta = ai_oRhs.m_iDelta;

	if( m_iSize > 0 )
	{
		m_pdElements = new m4date_t[ m_iSize ] ;

#ifdef _DEBUG
		if( m_pdElements == NULL )
		{
			m_iSize = 0 ;
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4date_t ) * m_iSize ) << __LINE__ << __FILE__ ) ;
			return ;
		}
#endif
	}

	if( m_iLength > 0 )
	{
		memcpy( m_pdElements, ai_oRhs.m_pdElements, sizeof( m4date_t ) * m_iLength ) ;
	}
	
  //## end ClDateStack::ClDateStack%-803545528.body
}


ClDateStack::~ClDateStack (void )
{
  //## begin ClDateStack::~ClDateStack%1915633445.body preserve=yes
	if( m_pdElements != NULL )
	{
		delete [] m_pdElements ;
		m_pdElements = NULL ;
	}
  //## end ClDateStack::~ClDateStack%1915633445.body
}



//## Other Operations (implementation)
m4return_t ClDateStack::_Increment (void )
{
  //## begin ClDateStack::_Increment%-2053271243.body preserve=yes

	m4uint16_t	iSize ;
	m4date_t	*pdTmp ;


	iSize = m_iSize + m_iDelta ;

	if( iSize == 0 )
	{
		iSize = 1 ;
	}

	pdTmp = new m4date_t[ iSize  ] ;

	CHECK_CHLOG_DEBUGF( pdTmp == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4date_t ) * iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = iSize ;

	if( m_pdElements != NULL )
	{
		if( m_iLength > 0 )
		{
			memcpy( pdTmp, m_pdElements, sizeof( m4date_t ) * m_iLength ) ;
		}

		delete [] m_pdElements ;
	}

	m_pdElements = pdTmp ;
		
	return( M4_SUCCESS ) ;
  //## end ClDateStack::_Increment%-2053271243.body
}

m4return_t ClDateStack::_Sort (void ) const
{
  //## begin ClDateStack::_Sort%1889313345.body preserve=yes
	qsort( m_pdElements, size_t( m_iLength ), sizeof( m4date_t ), M4ClCompareDate ) ;
	return( M4_SUCCESS ) ;
  //## end ClDateStack::_Sort%1889313345.body
}

m4return_t ClDateStack::Init (m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4uint16_t ai_iSize, m4uint16_t ai_iDelta)
{
  //## begin ClDateStack::Init%2054287195.body preserve=yes

	m_dStartDate = ai_dStartDate ;
	m_dEndDate = ai_dEndDate ;
	m_iLength = 0 ;
	m_iDelta = ai_iDelta ;

	if( m_pdElements != NULL )
	{
		delete [] m_pdElements ;
	}

	if( ai_iSize == 0 )
	{
		ai_iSize = 1 ;
	}

    m_pdElements = new m4date_t[ ai_iSize ] ;

	CHECK_CHLOG_DEBUGF( m_pdElements == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4date_t ) * ai_iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = ai_iSize ;

	m_pdElements[ m_iLength++ ] = ai_dStartDate ;

	return( M4_SUCCESS ) ;
  //## end ClDateStack::Init%2054287195.body
}

m4return_t ClDateStack::Clear (void )
{
  //## begin ClDateStack::Clear%408685202.body preserve=yes
	m_iLength = 1 ;
	return( M4_SUCCESS ) ;
  //## end ClDateStack::Clear%408685202.body
}

m4return_t ClDateStack::Clear (m4date_t ai_dStartDate, m4date_t ai_dEndDate)
{
  //## begin ClDateStack::Clear%-530398764.body preserve=yes
	m_dStartDate = ai_dStartDate ;
	m_dEndDate = ai_dEndDate ;
	m_iLength = 0 ;
	m_pdElements[ m_iLength++ ] = ai_dStartDate ;
	return( M4_SUCCESS ) ;
  //## end ClDateStack::Clear%-530398764.body
}

m4return_t ClDateStack::GetElement (m4uint16_t ai_iPosition, m4date_t& ao_rdElement) const
{
  //## begin ClDateStack::GetElement%-1453061178.body preserve=yes

	CHECK_CHLOG_DEBUGF( ai_iPosition >= m_iLength, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;

	ao_rdElement = m_pdElements[ ai_iPosition ] ;

	return( M4_SUCCESS ) ;
  //## end ClDateStack::GetElement%-1453061178.body
}

m4return_t ClDateStack::AddEndDate (m4date_t ai_iTimeUnit)
{
  //## begin ClDateStack::AddEndDate%43275926.body preserve=yes

	m4return_t	iResult ;


	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	m_pdElements[ m_iLength++ ] = m_dEndDate + ai_iTimeUnit ;

	return( M4_SUCCESS ) ;
  //## end ClDateStack::AddEndDate%43275926.body
}

m4return_t ClDateStack::Reduce (void )
{
  //## begin ClDateStack::Reduce%-266750812.body preserve=yes

	m4uint16_t	i ;
	m4uint16_t	iLength ;
	m4uint16_t	iPosition ;


    if( m_iLength != 0 )
    {
		_Sort() ;
	}

    iPosition = 0 ;
    iLength = m_iLength ;

    for( i = 1 ; i < iLength ; i++ )
    {
//      if( m_pdElements[ i ] != m_pdElements[ iPosition ] )
// fechas distintas
		if( fabs( m_pdElements[ i ] - m_pdElements[ iPosition ] ) > M4_LN4_MATH_PRECISION )
        {
            iPosition++ ;
            m_pdElements[ iPosition ] = m_pdElements[ i ] ;
        }
        else
        {
            m_iLength-- ;
        }
    }

    return( M4_SUCCESS ) ;
  //## end ClDateStack::Reduce%-266750812.body
}

m4return_t ClDateStack::ClipReduce (m4date_t ai_dStartDate, m4date_t ai_dEndDate)
{
  //## begin ClDateStack::ClipReduce%383569071.body preserve=yes
	m4uint16_t	i ;
	m4uint16_t	iStart=0;
	m4uint16_t	iEnd=0;
	m4return_t	iResult ;

	//metemos la fecha inicio, por si no estaba (ya que tiene que quedar como primer elemento)
	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}
	
	m_pdElements[ m_iLength++ ] = ai_dStartDate ;

	//Ordenamos
    if( m_iLength != 0 )
    {
		_Sort() ;
	}

	//Clipeamos
		//Determinamos cual es el primer elemento	
		i=0;
		while (m_pdElements[ i ] < ai_dStartDate) ++i;
		iStart=i;
		
		//Determinamos cual es el último elemento
		i=m_iLength-1;
		while (m_pdElements[ i ] > ai_dEndDate) --i;
		iEnd=i;
		
		//movemos memoria y actualizamos longitud
		m_iLength=iEnd-iStart+1;
		if( m_iLength > 0 )
		{
			memcpy( m_pdElements, &m_pdElements[iStart], sizeof( m4date_t ) * m_iLength ) ;
		}

	m_dEndDate=ai_dEndDate;
	m_dStartDate=ai_dStartDate;
	//Reducimos
	Reduce();

    return( M4_SUCCESS ) ;
  //## end ClDateStack::ClipReduce%383569071.body
}

m4return_t ClDateStack::Copy (m4date_t*& ao_pdDatesArray, m4uint16_t ai_iLength) const
{
  //## begin ClDateStack::Copy%957978193.body preserve=yes
	return M4_ERROR;
  //## end ClDateStack::Copy%957978193.body
}

ClDateStack& ClDateStack::operator = (const ClDateStack& ai_oRhs)
{
  //## begin ClDateStack::operator =%776598467.body preserve=yes
	m_dStartDate = ai_oRhs.m_dStartDate ;
	m_dEndDate = ai_oRhs.m_dEndDate ;
	m_iDelta = ai_oRhs.m_iDelta;

	if ( m_iSize < ai_oRhs.m_iSize ) //si en el tamaño de la antigua no cabe el tamaño (no los elementos) de la fuente
	{
		delete [] m_pdElements ;

		m_iSize = ai_oRhs.m_iSize ;

		m_pdElements = new m4date_t[ m_iSize ] ;		

#ifdef _DEBUG
		if( m_pdElements == NULL )
		{
			m_iSize = 0 ;
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4date_t ) * m_iSize ) << __LINE__ << __FILE__ ) ;
			return *this;
		}
#endif
	}

	m_iLength = ai_oRhs.m_iLength ;

	if( m_iLength > 0 )
	{
		memcpy( m_pdElements, ai_oRhs.m_pdElements, sizeof( m4date_t ) * m_iLength ) ;
	}

	return *this;
  //## end ClDateStack::operator =%776598467.body
}

ClDateStack& ClDateStack::operator += (const ClDateStack& ai_oRhs)
{
  //## begin ClDateStack::operator +=%-1267398125.body preserve=yes

	m4uint16_t i;

	//metemos todas las fechas de la pila de la derecha en la izq
		
	for (i=0;i < ai_oRhs.m_iLength; ++i)
	{
		AddElement(	ai_oRhs.m_pdElements[ i ] );
	}

	return *this;
  //## end ClDateStack::operator +=%-1267398125.body
}

// Additional Declarations
  //## begin ClDateStack%3912A6B703D5.declarations preserve=yes
  //## end ClDateStack%3912A6B703D5.declarations

//## begin module%3912A6BD026A.epilog preserve=yes


ClDateStack operator+ (const ClDateStack &ai_oLhs, const ClDateStack &ai_oRhs)
{
	ClDateStack temp(ai_oLhs);
	return temp+=ai_oRhs;
	
}
//## end module%3912A6BD026A.epilog
