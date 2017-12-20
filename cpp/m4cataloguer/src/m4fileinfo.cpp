
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4fileinfo.cpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                11-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the class for storing installation file information
//
//==============================================================================


#include "m4fileinfo.hpp"
#include "m4unicode.hpp"



//=================================================================================
//
// M4FileInfo
//
// This class holds file information
//
//=================================================================================

M4FileInfo::M4FileInfo( m4pcchar_t ai_pccPath, m4uint32_t ai_iSize, m4uint32_t ai_iCrc )
{

	size_t	iLength = 0 ;


	m_pcPath = NULL ;
	m_iSize = ai_iSize ;
	m_iCrc = ai_iCrc ;

	if( ai_pccPath != NULL )
	{
		iLength = strlen( ai_pccPath ) ;

		m_pcPath = new m4char_t[ iLength + 1 ] ;

		if( m_pcPath == NULL )
		{
			printf( "\nNot enough memory to allocate <%d> bytes.", ( m4uint32_t ) ( sizeof( m4char_t ) * ( iLength + 1 ) ) ) ;
			return ;
		}

		memcpy( m_pcPath, ai_pccPath, iLength + 1 ) ;
	}
}



M4FileInfo::~M4FileInfo( void )
{
	if( m_pcPath != NULL )
	{
		delete( m_pcPath ) ;
		m_pcPath = NULL ;
	}
}



//=================================================================================
//
// Write file list to a stream from a file list.
//
// Parameters
//		ai_pccFilename: File to write file list to
//		ai_rvFiles: File list
//
// Returns
//		M4_ERROR if failure; M4_SUCCESS otherwise
//
//=================================================================================

m4return_t	M4FileInfo::Write( m4pcchar_t ai_pccFilename, vector<M4FileInfo*>& ai_rvFiles )
{

	m4uchar_t	c = 0 ;
	m4uint32_t	i = 0 ;
	size_t		iLength = 0 ;
	m4pcchar_t	pccPath = NULL ;
	m4pcchar_t	pccScaped = NULL ;
	FILE*		pfFile = NULL ;

	vector<M4FileInfo*>::iterator	it ;


	eUniFileEncodingType_t	eEncoding = M4UniNative ;
	pfFile = M4Fopen( ai_pccFilename, M4UniWrite, eEncoding ) ;

	if( pfFile == NULL )
	{
		printf( "\nCannot open <%s> for writting.", ai_pccFilename ) ;
		return( M4_ERROR ) ;
	}

	fprintf( pfFile, M4XMLHeader() ) ;
	fprintf( pfFile, "<filelist>\n" ) ;

	it = ai_rvFiles.begin() ;

	while( it != ai_rvFiles.end() )
	{
		fprintf( pfFile, "  <file path='" ) ;

		pccPath = (*it)->GetPath() ;
		iLength = strlen( pccPath ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			c = pccPath[ i ] ;

			pccScaped = M4XMLScapeChar( c ) ;

			if( pccScaped != NULL )
			{
				fprintf( pfFile, "%s", pccScaped ) ;
			}
			else
			{
				fputc( c, pfFile ) ;
			}
		}

		fprintf( pfFile, "' size='%lu' crc='%lu'/>\n", (*it)->GetSize(), (*it)->GetCrc() ) ;
		it++ ;
	}

	fprintf( pfFile, "</filelist>\n" ) ;
	fclose( pfFile ) ;
	pfFile = NULL ;

	return( M4_SUCCESS ) ;
}
