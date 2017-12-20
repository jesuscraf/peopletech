//## begin module%3912A6BD030B.cm preserve=no
//## end module%3912A6BD030B.cm

//## begin module%3912A6BD030B.cp preserve=no
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
//## end module%3912A6BD030B.cp

//## Module: clpstack%3912A6BD030B; Package body
//## Subsystem: m4objglb::src%3912A6F00372
//## Source file: d:\new\m4objglb\src\clpstack.cpp

//## begin module%3912A6BD030B.additionalIncludes preserve=no
//## end module%3912A6BD030B.additionalIncludes

//## begin module%3912A6BD030B.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                clpstack.cpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                11-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions of an array of void pointers
//
//
//==============================================================================



#include "clpstack.hpp"
#include "m4objglb.hpp"
//## end module%3912A6BD030B.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A6BD030B.declarations preserve=no
//## end module%3912A6BD030B.declarations

//## begin module%3912A6BD030B.additionalDeclarations preserve=yes





// Funcion bclosersearch ======================================================

void*  M4VMBCloserSearch(
							const void *key,
							const void *base,
							size_t num,
							size_t width,
							int (*compare)(const void *, const void *),
							m4uint16_t &ao_riElement
						)
{
        char *lo = (char *)base;
        char *hi = (char *)base + (num - 1) * width;
        char *mid;
        unsigned int half;
        int result;

        while (lo <= hi)
		{
			half = num / 2 ;
            mid = lo + (num & 1 ? half : (half - 1)) * width;
            if (!(result = (*compare)(key,mid)))
                    return(mid);
            else if (result < 0)
            {
                    hi = mid - width;
                    num = num & 1 ? half : half-1;
            }
            else    {
                    lo = mid + width;
                    num = half;
            }
		}

		if( lo > base )
		{
			ao_riElement = ( lo - (char *)base ) / width;
		}

        return(NULL);
}





//=================================================================================
//
// ClReferenceStack
//
// Clase de referencia a punteros a void
//
//=================================================================================

//## end module%3912A6BD030B.additionalDeclarations


// Class ClReferenceStack 




//## Other Operations (implementation)
m4pcvoid_t ClReferenceStack::Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement) const
{
  //## begin ClReferenceStack::Find%1404830808.body preserve=yes

    m4pcvoid_t  pcvResult ;


    if( m_iLength == 0 )
    {
        pcvResult = NULL ;
    }
    else
    {
		// Se supone que siempre está ordenada
        pcvResult = M4VMBSearch( ai_pcvElement, m_ppvElements, size_t( m_iLength ), sizeof( m4pvoid_t ), ai_fFunction ) ;
    }

	return( pcvResult ) ;
  //## end ClReferenceStack::Find%1404830808.body
}

m4uint32_t ClReferenceStack::GetTotalSize (m4uint32_t& ao_riStatic)
{
  //## begin ClReferenceStack::GetTotalSize%1244502316.body preserve=yes
	ao_riStatic = 0 ;

	ao_riStatic += sizeof( m4uint16_t ) ;		// m_iLength
	ao_riStatic += sizeof( m4pvoid_t* ) ;		// m_ppvElements

	return( ao_riStatic ) ;
  //## end ClReferenceStack::GetTotalSize%1244502316.body
}

// Additional Declarations
  //## begin ClReferenceStack%3912A6BB0331.declarations preserve=yes
  //## end ClReferenceStack%3912A6BB0331.declarations

// Class ClPointerStack 







ClPointerStack::ClPointerStack (void )
  //## begin ClPointerStack::ClPointerStack%-1633119007.hasinit preserve=no
  //## end ClPointerStack::ClPointerStack%-1633119007.hasinit
  //## begin ClPointerStack::ClPointerStack%-1633119007.initialization preserve=yes
  //## end ClPointerStack::ClPointerStack%-1633119007.initialization
{
  //## begin ClPointerStack::ClPointerStack%-1633119007.body preserve=yes
	m_iSize = 0 ;
	m_iLength = 0 ;
    m_iDelta = M4CL_DEFAULT_DELTA ;
    m_bSorted = M4_TRUE ;
	m_ppvElements = NULL ;
  //## end ClPointerStack::ClPointerStack%-1633119007.body
}


ClPointerStack::~ClPointerStack (void )
{
  //## begin ClPointerStack::~ClPointerStack%1586451529.body preserve=yes
	if( m_ppvElements != NULL )
	{
		delete [] m_ppvElements ;
		m_ppvElements = NULL ;
	}
  //## end ClPointerStack::~ClPointerStack%1586451529.body
}



//## Other Operations (implementation)
m4return_t ClPointerStack::Init (m4uint16_t ai_iSize, m4uint16_t ai_iDelta)
{
  //## begin ClPointerStack::Init%-896158974.body preserve=yes

	m_iLength = 0 ;
    m_iDelta = ai_iDelta ;
    m_bSorted = M4_TRUE ;

	if( m_ppvElements != NULL )
	{
		delete [] m_ppvElements ;
	}

	if( ai_iSize == 0 )
	{
		ai_iSize = 1 ;
	}

	m_ppvElements = new m4pvoid_t[ ai_iSize ] ;

	CHECK_CHLOG_DEBUGF( m_ppvElements == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4pvoid_t ) * ai_iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = ai_iSize ;

	return( M4_SUCCESS ) ;
  //## end ClPointerStack::Init%-896158974.body
}

m4return_t ClPointerStack::AddElement (m4pvoid_t ai_pvElement)
{
  //## begin ClPointerStack::AddElement%1033999823.body preserve=yes

    m4return_t  iResult ;


	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

    m_ppvElements[ m_iLength++ ] = ai_pvElement ;
    m_bSorted = M4_FALSE ;

	return( M4_SUCCESS ) ;
  //## end ClPointerStack::AddElement%1033999823.body
}

m4pcvoid_t ClPointerStack::Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement) const
{
  //## begin ClPointerStack::Find%-1539410701.body preserve=yes

    m4pcvoid_t  pcvResult ;
    m4uint16_t  i ;
    m4uint16_t  iFound ;


    if( m_iLength == 0 )
    {
        pcvResult = NULL ;
    }
    else
    {
        if( m_bSorted == M4_TRUE )
        {
            pcvResult = M4VMBSearch( ai_pcvElement, m_ppvElements, size_t( m_iLength ), sizeof( m4pvoid_t ), ai_fFunction ) ;
        }
        else
        {
            iFound = 0 ;
            for( i = 0 ; i < m_iLength && iFound == 0 ; i++ )
            {
                if( ( *ai_fFunction ) ( ai_pcvElement, &m_ppvElements[ i ] ) == 0 )
                {
                    pcvResult = &m_ppvElements[ i ] ;
                    iFound = 1 ;
                }
            }

            if( iFound == 0 )
            {
                pcvResult = NULL ;
            }
        }
    }

	return( pcvResult ) ;
  //## end ClPointerStack::Find%-1539410701.body
}

m4pcvoid_t ClPointerStack::Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement, m4uint16_t& ao_riElement) const
{
  //## begin ClPointerStack::Find%-1866465103.body preserve=yes

    m4pcvoid_t  pcvResult ;


	ao_riElement = 0 ;
    pcvResult = NULL ;

    if( m_iLength != 0 && m_bSorted == M4_TRUE )
    {
        pcvResult = M4VMBCloserSearch( ai_pcvElement, m_ppvElements, size_t( m_iLength ), sizeof( m4pvoid_t ), ai_fFunction, ao_riElement ) ;
    }

	return( pcvResult ) ;
  //## end ClPointerStack::Find%-1866465103.body
}

m4uint32_t ClPointerStack::GetTotalSize (m4uint32_t& ao_riStatic, m4uint32_t& ao_riPointers)
{
  //## begin ClPointerStack::GetTotalSize%-760774687.body preserve=yes

	ao_riStatic = 0 ;
	ao_riPointers = 0 ;

	ao_riStatic += sizeof( m4uint16_t ) ;		// m_iSize
	ao_riStatic += sizeof( m4uint16_t ) ;		// m_iLength
	ao_riStatic += sizeof( m4uint16_t ) ;		// m_iDelta
	ao_riStatic += sizeof( m4bool_t ) ;			// m_bSorted
	ao_riStatic += sizeof( m4pvoid_t* ) ;		// m_ppvElements

	ao_riPointers = sizeof( m4pvoid_t ) * m_iSize ;

	return( ao_riStatic + ao_riPointers ) ;
  //## end ClPointerStack::GetTotalSize%-760774687.body
}

m4return_t ClPointerStack::_Increment (void )
{
  //## begin ClPointerStack::_Increment%1205121754.body preserve=yes

	m4uint16_t	iSize ;
	m4pvoid_t   *ppvTmp ;


	iSize = m_iSize + m_iDelta ;

	if( iSize == 0 )
	{
		iSize = 1 ;
	}

	ppvTmp = new m4pvoid_t[ iSize ] ;

	CHECK_CHLOG_DEBUGF( ppvTmp == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4pvoid_t ) * iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = iSize ;

	if( m_ppvElements != NULL )
	{
		if( m_iLength > 0 )
		{
			memcpy( ppvTmp, m_ppvElements, sizeof( m4pvoid_t ) * m_iLength ) ;
		}

		delete [] m_ppvElements ;
	}

	m_ppvElements = ppvTmp ;
		
	return( M4_SUCCESS ) ;
  //## end ClPointerStack::_Increment%1205121754.body
}

m4return_t ClPointerStack::AddElementIfNotIn (m4pvoid_t ai_pvElement)
{
  //## begin ClPointerStack::AddElementIfNotIn%2002713089.body preserve=yes

    m4return_t  iResult ;
    m4uint16_t  i ;


    for( i = 0 ; i < m_iLength ; i++ )
    {
        if( m_ppvElements[ i ] == ai_pvElement )
        {
	        return( M4_SUCCESS ) ;
        }
    }

    iResult = AddElement( ai_pvElement ) ;

	return( iResult ) ;
  //## end ClPointerStack::AddElementIfNotIn%2002713089.body
}

m4return_t ClPointerStack::InsertElement (m4pvoid_t ai_pvElement, m4uint16_t ai_iPosition, m4uint8_t ai_iOrdered)
{
  //## begin ClPointerStack::InsertElement%1880403762.body preserve=yes

    m4return_t  iResult ;
	size_t		iSize ;


	if( m_iLength >= m_iSize )
	{
		iResult = _Increment() ;

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	if( ai_iPosition >= m_iLength )
	{
		m_ppvElements[ m_iLength ] = ai_pvElement ;
	}
	else
	{
		iSize = m_iLength - ai_iPosition ;
		memmove( m_ppvElements + ai_iPosition + 1, m_ppvElements + ai_iPosition, iSize * sizeof( m4pvoid_t ) ) ;
		m_ppvElements[ ai_iPosition ] = ai_pvElement ;
	}

	m_iLength++ ;

	if( ai_iOrdered == 0 )
	{
		m_bSorted = M4_FALSE ;
	}

	return( M4_SUCCESS ) ;
  //## end ClPointerStack::InsertElement%1880403762.body
}

m4return_t ClPointerStack::Sort (m4comparef_t ai_fFunction)
{
  //## begin ClPointerStack::Sort%-1846020125.body preserve=yes

    if( m_bSorted == M4_FALSE && m_iLength != 0 )
    {
	    qsort( m_ppvElements, size_t( m_iLength ), sizeof( m4pvoid_t ), ai_fFunction ) ;
        m_bSorted = M4_TRUE ;
    }

	return( M4_SUCCESS ) ;
  //## end ClPointerStack::Sort%-1846020125.body
}


m4return_t ClPointerStack::Sort (m4comparef_t ai_fFunction, m4uint16_t ai_iStart, m4uint16_t ai_iEnd)
{
  //## begin ClPointerStack::Sort%-1846020125.body preserve=yes

	m4uint16_t	iLength ;


	if( ai_iEnd > m_iLength )
	{
		ai_iEnd = m_iLength ;
	}

	if( ai_iStart > ai_iEnd )
	{
		ai_iStart = ai_iEnd ;
	}

	iLength = ai_iEnd - ai_iStart ;

	// En esta función se ordena siempre independientemente de la variable de ordenación
    if( iLength > 0 )
    {
	    qsort( m_ppvElements + ai_iStart, size_t( iLength ), sizeof( m4pvoid_t ), ai_fFunction ) ;
    }

	return( M4_SUCCESS ) ;
  //## end ClPointerStack::Sort%-1846020125.body
}


void ClPointerStack::ReAllocatePointers (size_t ai_iOffset)
{
  //## begin ClPointerStack::ReAllocatePointers%-1908955308.body preserve=yes

    m4uint16_t  i ;


    for( i = 0 ; i < m_iLength ; i++ )
    {
		m_ppvElements[ i ] = m4pchar_t( m_ppvElements[ i ] ) + ai_iOffset ;
    }
  //## end ClPointerStack::ReAllocatePointers%-1908955308.body
}

// Additional Declarations
  //## begin ClPointerStack%3912A6BB0364.declarations preserve=yes
  //## end ClPointerStack%3912A6BB0364.declarations

//## begin module%3912A6BD030B.epilog preserve=yes
//## end module%3912A6BD030B.epilog
