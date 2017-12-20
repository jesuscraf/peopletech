
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4buildversion.dll
// File:                m4mindversion.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                10-09-2002
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the mind and m2 version functions
//
//
//==============================================================================



#include "m4buildversion.hpp"


// Value must be updated each new tech service pack!!!
#define MIND_VERSION "8.1SP4"


m4pchar_t	g_pcVersion = NULL ;
m4pchar_t	g_pcDBVersion = NULL ;

// ---------------------------------------------------
// Enh 163151.
// Devuelve la version de mind
// ---------------------------------------------------
m4pcchar_t	GetMindVersion( void )
{
	if( g_pcVersion == NULL )
	{
		g_pcVersion = new m4char_t[ strlen( MIND_VERSION ) +  1 ] ;
		strcpy( g_pcVersion, MIND_VERSION ) ;
	}

	return( g_pcVersion ) ;
}

// ---------------------------------------------------
// Enh 249085.
// Devuelve la lista de versiones admitidas
// ---------------------------------------------------
m4pcchar_t	GetMindDBVersion( void )
{
	return( g_pcDBVersion ) ;
}

// ---------------------------------------------------
// Enh 249085.
// Asigna la lista de versiones admitidas
// ---------------------------------------------------
void	SetMindDBVersion( m4pcchar_t ai_pccVersionsList )
{
	size_t	iSize ;

	if( ai_pccVersionsList == NULL )
	{
		return ;
	}
	
	iSize = strlen( ai_pccVersionsList ) ;

	if( g_pcDBVersion == NULL )
	{
		g_pcDBVersion = new m4char_t[ iSize + 1 ] ;
	}
	else if( iSize > strlen( g_pcDBVersion ) )
	{
		delete g_pcDBVersion ;
		g_pcDBVersion = new m4char_t[ iSize + 1 ] ;
	}

	strcpy( g_pcDBVersion, ai_pccVersionsList ) ;
}


// Devuelve la version de los .m2
m4uint16_t	GetM2VersionNumber( void )
{
	return( 0 + ( 814 << 6 ) ) ;
}


// Devuelve la version de los .m2 en formato cadena
m4pcchar_t	GetM2VersionString( void )
{
	return( "814.000.000" ) ;
}
