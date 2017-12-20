//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:             cvdate.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:
//
// Definition:
//     Implementation of date conversion functions
//     M4dateToDate and DateToM4date
//
//==============================================================================

#ifdef _WINDOWS

#include "cvdate.h"

const double M4DATEDIFF = 693583.00;



DATE	M4dateToDate( m4date_t ai_dDate )
{
	DATE	dDate = ai_dDate - M4DATEDIFF ;

	if( dDate < 0 )
	{
		// Bug 0296641. En fechas negativas la parte decimal es positiva y hay que cambiarle el signo
		DATE	dInteger = m4int32_t( dDate ) ;

		if( dInteger != dDate )
		{
			dDate = ( dInteger - 1 ) + ( ( dInteger - 1 ) - dDate ) ;
		}
	}

	return( dDate ) ;
}


m4date_t	DateToM4date( DATE ai_dDate )
{
	DATE	dDate = ai_dDate ;

	if( dDate < 0 )
	{
		// Bug 0296641. En fechas negativas la parte decimal es positiva y hay que cambiarle el signo
		DATE	dInteger = m4int32_t( dDate ) ;

		if( dInteger != dDate )
		{
			dDate = dInteger + ( dInteger - dDate ) ;
		}
	}

	return( dDate + M4DATEDIFF ) ;
}


#endif

