//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini 
// File:                regedit.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================


#include "regedit.hpp"
#include "blocksynchronization.hpp"
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "m4xmlreg.hpp"





char GetRegKeyData (char *szRegKey, char *pszRegValue, char *pszData, unsigned long* pLen)
{
	m4return_t			iResult = M4_ERROR ;
	unsigned long		iLength = 0 ;
	m4pchar_t			pcValue = NULL ;


	// Se lee del nuevo API del registro
	iResult = M4XmlRegistry::Instance()->GetValue( szRegKey, pszRegValue, pcValue ) ;

	if( iResult == M4_SUCCESS )
	{
		iLength = (unsigned long) strlen( pcValue ) ;
		memcpy( pszData, pcValue, iLength + 1 ) ;
		delete [] pcValue ;

		if( pLen != NULL )
		{
			*pLen = iLength ;
		}

		return( TRUE ) ;
	}

	return( FALSE ) ;
}



