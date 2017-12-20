
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_finder.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//
//==============================================================================


#include "file_finder.hpp"
#include "m4unicode.hpp"

#ifdef _UNIX
#include <dirent.h>
#endif


void	ClFileFinder::_Destroy( void )
{
	if( m_pcANSIPath != NULL )
	{
		delete( m_pcANSIPath ) ;
		m_pcANSIPath = NULL ;
	}

	if( m_pcANSIDirectory != NULL )
	{
		delete( m_pcANSIDirectory ) ;
		m_pcANSIDirectory = NULL ;
	}

	if( m_pcANSITemplate != NULL )
	{
		delete( m_pcANSITemplate ) ;
		m_pcANSITemplate = NULL ;
	}

	if( m_poHandle != NULL )
	{
#ifdef _WINDOWS 
		FindClose( (HANDLE)m_poHandle ) ;
#else
		closedir( (DIR *)m_poHandle ) ;
#endif
		m_poHandle = NULL ;
	}
}


ClFileFinder::ClFileFinder( void )
{
	m_pcANSIPath = NULL ;
	m_pcANSIDirectory = NULL ;
	m_pcANSITemplate = NULL ;
	m_poHandle = NULL ;
}


ClFileFinder::ClFileFinder( m4pcchar_t ai_pccPath )
{
	m_pcANSIPath = NULL ;
	m_pcANSIDirectory = NULL ;
	m_pcANSITemplate = NULL ;
	m_poHandle = NULL ;

	Init( ai_pccPath ) ;
}


ClFileFinder::~ClFileFinder( void )
{
	_Destroy() ;
}


void	ClFileFinder::Init( m4pcchar_t ai_pccPath )
{

	int		iLength = -1 ;
	size_t	iPosition = -1 ;


	_Destroy() ;

	if( ai_pccPath != NULL )
	{
		iLength = -1 ;
		m_pcANSIPath = M4CppToANSI( ai_pccPath, iLength ) ;

		iPosition = strlen( m_pcANSIPath ) - 1 ;
		while( m_pcANSIPath[ iPosition ] != '\\' && m_pcANSIPath[ iPosition ] != '/' )
		{
			iPosition-- ;
		}

		m_pcANSIDirectory = new m4char_t[ iPosition + 1 ] ;
		memcpy( m_pcANSIDirectory, m_pcANSIPath, iPosition ) ;
		m_pcANSIDirectory[ iPosition ] = '\0' ;

		m_pcANSITemplate = new m4char_t[ iLength - iPosition ] ;
		memcpy( m_pcANSITemplate, m_pcANSIPath + iPosition + 1, iLength - iPosition ) ;
	}
}


void	ClFileFinder::End( void )
{
	_Destroy() ;
}


m4return_t ClFileFinder::FindNext( ClFile& ao_roFile )
{

	int			iLength = -1 ;
	m4pchar_t	pcCppFileName = NULL ;
	m4char_t	acANSIFileName[ M4_MAX_PATH + 1 ] ;


#ifdef _WINDOWS

	BOOL			bResult = FALSE ;
	WIN32_FIND_DATA	oFindData ;

	if( m_poHandle == NULL )
	{
		//Buscamos el primer fichero dentro del directorio
		m_poHandle = FindFirstFile( m_pcANSIPath, &oFindData ) ;

		if( m_poHandle == INVALID_HANDLE_VALUE )
		{
			//Cuando nos devulve un valor erroneo es que no ninguno que coincida con la plantilla
			m_poHandle = NULL ;
			return( M4_ERROR ) ;
		}
	}
	else
	{
		bResult = FindNextFile( (HANDLE)m_poHandle, &oFindData ) ;
		
		if( bResult == FALSE  )
		{
			//Si se ha producido un error o no hay más ficheros que conincidan con el patrón no continuanos
			return M4_ERROR;
		}
	}

	//Comprobamos que no nos devuelve el propio derectorio o el directorio padre
	while( strcmp( oFindData.cFileName , "." ) == 0 || strcmp( oFindData.cFileName, ".." ) == 0 )
	{
		bResult = FindNextFile( (HANDLE)m_poHandle, &oFindData ) ;

		if( bResult == FALSE  )
		{
			//Si se ha producido un error o no hay más ficheros que conincidan con el patrón no continuanos
			return M4_ERROR;
		}
	}

	sprintf( acANSIFileName, "%s\\%s", m_pcANSIDirectory, oFindData.cFileName ) ;

	iLength = -1 ;
	pcCppFileName = M4ANSIToCpp( acANSIFileName, iLength ) ;
	ao_roFile.SetFileName( pcCppFileName );
	delete( pcCppFileName ) ;

	return( M4_SUCCESS ) ;

#else
	m4return_t		iResult = M4_ERROR ;
	struct dirent	*poDirEntry = NULL ;

	if( m_poHandle == NULL )
	{
		m_poHandle = (void*)opendir( m_pcANSIDirectory ) ;

		if( m_poHandle == NULL )
		{
			m_poHandle = NULL ;
			return( M4_ERROR ) ;
		}
	} 
	  
	poDirEntry = (struct dirent*) new m4pchar_t[ sizeof(struct dirent) + M4_MAX_PATH + 1 ] ;

	//Buscamos todos los ficheros del directorio
#ifndef _SOLARIS
	struct dirent *poDirEntryResult = (struct dirent*) new m4pchar_t[ sizeof(struct dirent) + M4_MAX_PATH + 1 ] ;

	while( readdir_r( (DIR*)m_poHandle, poDirEntry, &poDirEntryResult ) == 0 )
	{
		memcpy( poDirEntry, poDirEntryResult, sizeof(struct dirent) + M4_MAX_PATH + 1 ) ;
#else
	while( readdir_r( (DIR*)m_poHandle, poDirEntry ) != NULL )
	{
#endif
		if( strcmp( poDirEntry->d_name, "." ) != 0 && strcmp( poDirEntry->d_name, ".." ) != 0 )
		{ 	 
			//Comprobamos si se ajusta al patrón
			if( _CheckWildCards( m_pcANSITemplate, poDirEntry->d_name ) == M4_SUCCESS )
			{
				sprintf( acANSIFileName, "%s/%s", m_pcANSIDirectory, poDirEntry->d_name ) ;

				iLength = -1 ;
				pcCppFileName = M4ANSIToCpp( acANSIFileName, iLength ) ;
				ao_roFile.SetFileName( pcCppFileName );
				delete( pcCppFileName ) ;

				iResult = M4_SUCCESS ;

				// Bug 0055151 Si no se sale del while sólo devuelve el último fichero localizado.
				break;
			}
		}
	}

	delete( (m4pchar_t)poDirEntry ) ;

#ifndef _SOLARIS
	delete( (m4pchar_t)poDirEntryResult ) ;
#endif

	return( iResult ) ;
#endif
}

	
#ifdef _UNIX
m4return_t	ClFileFinder::_CheckWildCards( m4pcchar_t ai_pccTemplate, m4pcchar_t ai_pccFileName )
{

	int	iResult = 1 ;		// Resultado de la función
	int	iTemplate = 0;		// Cursor para recorrer el patron
	int	iFile = 0 ;			// Cursor para recorrer el nombre del fich
	int	iEnd = 0 ;			// Flag de control del bucle interno


	if( ai_pccTemplate == NULL || ai_pccFileName == NULL)
	{
		return( M4_ERROR ) ;
	}

	do
	{  
		switch( ai_pccTemplate[ iTemplate ] )
		{  
			case	0:
				// Se acaba el patron. Deberiamos iEnd. Si lo ultimo no era un * y queda candidato, no se acepta
				iResult = ( ai_pccFileName[ iFile ] == '\0' ) ;
				iEnd = 1 ;
				break ;
		
			case	'?':
				// Incrementamos el contador del patron. El del candidato tambien habria que incrementarlo,
				// pero si se nos acaba, no lo consideraremos valido.
				iTemplate++ ;
				iResult = ( ai_pccFileName[ iFile ] != '\0' ) ;
				iFile++ ;
				break;
	
			case	'*':
				// Comodin. Para empezar, incrementamos el contador. Saltamos posibles casos de comodines seguidos
				do
				{
					iTemplate++ ;
				}
				while( ai_pccTemplate[ iTemplate ] == '*' || ai_pccTemplate[ iTemplate ] == '?' ) ;

				// Ahora debemos avanzar el otro cursor hasta que encontremos el mismo caracter que en el patron, o se nos acabe
				do
				{
					iFile++ ;
				}
				while( ai_pccFileName[ iFile ] != ai_pccTemplate[ iTemplate ] && ai_pccFileName[ iFile ] != 0 ) ;

				break;

			default:
				// Caso general, cualquier caracter. La condicion de validez es que en el patron
				// y en el candidato, deben ser iguales
				iResult = ( ai_pccTemplate[ iTemplate ] == ai_pccFileName[ iFile] ) ;
				iTemplate++ ;
				iFile++ ;
		}
	}
	while( iResult == 1 && iEnd == M4_FALSE ) ;
   
	if( iResult == 1 )
	{
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}
#endif

