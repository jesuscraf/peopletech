//==============================================================================
//
// (c) Copyright 2014 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                changelistener.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                03-07-2014
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Code used in change listening
//
//==============================================================================

#include <stdlib.h>
#include "changelistener.hpp"


m4uint64_t	ClChangeListener::g_iLastTimeStamp = 1 ;


m4int8_t ClChangeListener::IsActive( void )
{
#ifdef	_WINDOWS
	m4pcchar_t		pccServer = NULL ;
	static m4int8_t	iIsActive = -1 ;


	if( iIsActive == -1 )
	{
		pccServer = getenv( "M4SERVER_PATH" ) ;

		if( pccServer != NULL )
		{
			iIsActive = 0 ;
		}
		else
		{
			iIsActive = 1 ;
		}
	}

	return( iIsActive ) ;
#else
	return( 0 ) ;
#endif
}


m4uint64_t ClChangeListener::GetLastTimeStamp( void )
{
	return( g_iLastTimeStamp ) ;
}

