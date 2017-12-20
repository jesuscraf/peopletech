
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jconfiguration.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a class for java configuration values.
//
//
//==============================================================================

#include "m4jconfiguration.hpp"
#include "m4javares.hpp"
#include "m4stl.hpp"
#include "m4objlog.hpp"
#include "m4regimpl.hpp"
#include "m4unicode.hpp"
#include "file_manager.hpp"
#include "file_finder.hpp"
#include "m4thread.hpp"
#include "blocksynchronization.hpp"
//#include "m4win2ux.hpp"
#include "zlib.h"

#include <sys/stat.h>

extern "C" {
#include "zipapi.h"
}


#ifdef _UNIX
	#include <pthread.h>

	#define	M4_JAVA_MAX_PATH				PATH_MAX
	#define	M4_JAVA_CLIENT					"java"
	#define	M4_JAVA_SERVER					"java"
	#define	M4_JAVA_PATH_SEPARATOR			":"
	#define	M4_DIRECTORY_SEPARATOR			"/"

	void	M4_DECL_M4JAVA	M4SetThreadMinPriority( void )
	{
		int					iPolicy = -1 ;
		struct sched_param	stParam ;
		pthread_t			iThread = pthread_self() ;

		int iError = pthread_getschedparam( iThread, &iPolicy, &stParam ) ;

		if( iError == 0 )
		{
			stParam.sched_priority = sched_get_priority_min( iPolicy ) ;
			iError = pthread_setschedparam( iThread, iPolicy, &stParam ) ;
		}
	}
#else
	#include <shlwapi.h>

	#define	M4_JAVA_MAX_PATH				MAX_PATH
	#define	M4_JAVA_CLIENT					"javaw.exe"
	#define	M4_JAVA_SERVER					"java.exe"
	#define	M4_JAVA_PATH_SEPARATOR			";"
	#define	M4_DIRECTORY_SEPARATOR			"\\"

	void	M4_DECL_M4JAVA	M4SetThreadMinPriority( void )
	{
		BOOL bSet = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_IDLE ) ;
	}
#endif




extern "C" M4_THREAD_RETURN	M4DecompressClassPath( m4pvoid_t ai_pvArgument )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bNeedToDecompress = M4_FALSE ;
	m4pchar_t			pcClassPath = NULL ;
	string				sClassPath ;
	string				sFileClassPath ;
	M4JavaConfiguration	*poConfiguration = NULL ;


	M4SetThreadMinPriority() ;

	pcClassPath = (m4pchar_t)ai_pvArgument ;

	if( pcClassPath != NULL )
	{
		poConfiguration = M4JavaConfiguration::Instance( NULL ) ;

		if( poConfiguration != NULL )
		{
			iResult = poConfiguration->NormalizeClassPath( pcClassPath, M4_TRUE, bNeedToDecompress, sClassPath, sFileClassPath ) ;

			if( iResult == M4_SUCCESS )
			{
				iResult = poConfiguration->SetClassPath( sClassPath.c_str(), sFileClassPath.c_str() ) ;
			}
		}
		delete( pcClassPath ) ;
	}

	return( 0 ) ;
}


m4pchar_t	M4CloneString( m4pcchar_t ai_pccSource, m4int32_t ai_iLength = -1 )
{
	if( ai_pccSource == NULL )
	{
		return( NULL ) ;
	}

	if( ai_iLength < 0 )
	{
		ai_iLength = strlen( ai_pccSource ) ;
	}

	m4pchar_t	pcResult = new m4char_t[ ai_iLength + 1 ] ;
	CHECK_CHLOG_ERRORF( pcResult == NULL, NULL, M4JAVA_NO_MEMORY, m4uint32_t( ( ai_iLength + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
	memcpy( pcResult, ai_pccSource, ai_iLength ) ;
	pcResult[ ai_iLength ] = '\0' ;

	return( pcResult ) ;
}


void	M4FreeString( m4pchar_t &aio_rpcString )
{
	if( aio_rpcString != NULL )
	{
		delete( aio_rpcString ) ;
		aio_rpcString = NULL ;
	}
}


m4return_t	M4Split( m4pchar_t ai_pcSource, m4pcchar_t ai_pccSeparator, vector<m4pchar_t>& ao_rvpcValues )
{

	m4pchar_t	pcToken = NULL ;


	ao_rvpcValues.clear() ;

	if( ai_pcSource != NULL )
	{
		pcToken = strtok( ai_pcSource, ai_pccSeparator ) ;

		while( pcToken != NULL )
		{
			if( *pcToken != '\0' )
			{
				ao_rvpcValues.push_back( pcToken ) ;
			}
			pcToken = strtok( NULL, ai_pccSeparator ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// M4JavaConfiguration
//
// Clase que define la configuración java
//
//=================================================================================

// Parte estática =============================================================

M4JavaConfiguration*	M4JavaConfiguration::m_poInstance = NULL ;
ClMutex					M4JavaConfiguration::m_oInstanceMutex( M4_TRUE ) ;


M4JavaConfiguration*	M4JavaConfiguration::Instance( ClVMBaseEnv *ai_poEnvironment )
{

	m4return_t			iResult = M4_ERROR ;
	M4JavaConfiguration	*poInstance = NULL ;


	if( ai_poEnvironment == NULL )
	{
		return( m_poInstance ) ;
	}

	if( m_poInstance == NULL )
	{
		ClMutBlock	oBlock( &m_oInstanceMutex ) ;

		if( m_poInstance == NULL )
		{
			poInstance = new M4JavaConfiguration() ;
			CHECK_CHLOG_ERRORF( poInstance == NULL, NULL, M4JAVA_NO_MEMORY, m4uint32_t( sizeof( M4JavaConfiguration ) ) << __LINE__ << __FILE__ ) ;

			iResult = poInstance->Open( ai_poEnvironment ) ;

			if( iResult != M4_SUCCESS )
			{
				delete( poInstance ) ;
				poInstance = NULL ;
			}

			m_poInstance = poInstance ;
		}
	}

	return( m_poInstance ) ;
}


void	M4JavaConfiguration::Release( void )
{
	if( m_poInstance != NULL )
	{
		ClMutBlock	oBlock( &m_oInstanceMutex ) ;

		if( m_poInstance != NULL )
		{
			delete( m_poInstance ) ;
			m_poInstance = NULL ;
		}
	}
}


// Funciones de inicialización ================================================
	
M4JavaConfiguration::M4JavaConfiguration( void )
{
	_Init() ;
}


M4JavaConfiguration::M4JavaConfiguration( ClVMBaseEnv *ai_poEnvironment )
{
	_Init() ;
	Open( ai_poEnvironment ) ;
}


M4JavaConfiguration::~M4JavaConfiguration( void )
{
	Close() ;
}


m4return_t	M4JavaConfiguration::Open( ClVMBaseEnv *ai_poEnvironment )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bNeedToDecompress = M4_FALSE ;
	int					iStatus = -1 ;
	m4uint32_t			i = 0 ;
	m4pcchar_t			pccDirectory = NULL ;
	m4pcchar_t			pccServer = NULL ;
	m4pcchar_t			pccJava = NULL ;
	m4pcchar_t			pccJavaHome = NULL ;
	m4pcchar_t			pccArguments = NULL ;
	m4pcchar_t			pccTrace = NULL ;
	m4pchar_t			pcClassPath = NULL ;
	m4pchar_t			pcError = NULL ;
	m4char_t			acJarDirectory[ M4_JAVA_MAX_PATH + 1 ] ;
	struct stat			stStatus ;
	string				sSource ;
	string				sExecutable ;
	string				sClassPath ;
	string				sFileClassPath ;
	vector<m4pchar_t>	vpcArguments ;
	M4Thread			*poThread = NULL ;


	CHECK_CHLOG_ERRORF( ai_poEnvironment == NULL, M4_ERROR, M4JAVA_NULL_ARGUMENT, "ai_poEnvironment" << "M4JavaConfiguration::Open" << __LINE__ << __FILE__ ) ;

	Close() ;

	// Jar directory
	pccDirectory = ai_poEnvironment->GetTempDirectory() ;

	if( pccDirectory != NULL && *pccDirectory != '\0')
	{
		sprintf( acJarDirectory, "%s%s%s", pccDirectory, M4_DIRECTORY_SEPARATOR, "JAR" ) ;
		m_pcJarDirectory = M4CloneString( acJarDirectory ) ;

		iResult = M4MakeDirectory( m_pcJarDirectory, &pcError ) ;

		if( iResult == -1 )
		{
			DUMP_CHLOG_ERRORF( M4JAVA_BAD_JAR_DIRECTORY, m_pcJarDirectory << pcError ) ;
		}

		if( pcError != NULL )
		{
			delete( pcError ) ;
		}

		if( iResult == -1 )
		{
			return( M4_ERROR ) ;
		}

		iStatus = M4FileStatus( m_pcJarDirectory, stStatus ) ;
		CHECK_CHLOG_ERRORF( iStatus != 0 || ( stStatus.st_mode & S_IFDIR ) == 0, M4_ERROR, M4JAVA_BAD_JAR_DIRECTORY, m_pcJarDirectory << "A file named JAR already exists" ) ;
	}

	CHECK_CHLOG_ERRORF( m_pcJarDirectory == NULL || *m_pcJarDirectory == '\0', M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Java
	pccServer = getenv( "M4SERVER_PATH" ) ;

	if( pccServer != NULL )
	{
		pccJava = M4_JAVA_SERVER ;
	}
	else
	{
		pccJava = M4_JAVA_CLIENT ;
	}

	pccJavaHome = ai_poEnvironment->GetJavaHome() ;

	if( pccJavaHome != NULL && *pccJavaHome != '\0' )
	{
		iResult = _ExpandPath( pccJavaHome, sExecutable ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		sExecutable.append( M4_DIRECTORY_SEPARATOR ) ;
		sExecutable.append( "bin" ) ;
		sExecutable.append( M4_DIRECTORY_SEPARATOR ) ;
		sExecutable.append( pccJava ) ;
	}
	else
	{
		sExecutable = pccJava ;
	}

	m_pcExecutable = M4CloneString( sExecutable.c_str(), sExecutable.size() ) ;
	CHECK_CHLOG_ERRORF( m_pcExecutable == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// ClassPath
	iResult = NormalizeClassPath( ai_poEnvironment->GetJavaClassPath(), M4_FALSE, bNeedToDecompress, sClassPath, sFileClassPath ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = SetClassPath( sClassPath.c_str(), sFileClassPath.c_str() ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( bNeedToDecompress == M4_TRUE )
	{
		// Se expande en otro thread
		pcClassPath = M4CloneString( ai_poEnvironment->GetJavaClassPath() ) ;
		CHECK_CHLOG_ERRORF( pcClassPath == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poThread = new M4Thread() ;
		CHECK_CHLOG_ERRORF( poThread == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( sizeof( M4Thread ) ) << __LINE__ << __FILE__ ) ;

		iResult = poThread->Create( M4DecompressClassPath, pcClassPath ) ;
		delete( poThread ) ;
		poThread = NULL ;
	}

	// Java Arguments
	pccArguments = ai_poEnvironment->GetJavaArguments() ;

	if( pccArguments != NULL && *pccArguments != '\0' )
	{
		sSource = pccArguments ;
		iResult = M4Split( (m4pchar_t)sSource.c_str(), ";", vpcArguments ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	m_iArgumentsLength = vpcArguments.size() ;
	m_ppcArguments = new m4pchar_t[ m_iArgumentsLength ] ;
	CHECK_CHLOG_ERRORF( m_ppcArguments == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t(  m_iArgumentsLength * sizeof( m4pchar_t ) ) << __LINE__ << __FILE__ ) ;
	memset( m_ppcArguments, 0, m_iArgumentsLength * sizeof( m4pchar_t ) ) ;

	for( i = 0 ; i < m_iArgumentsLength ; i++ )
	{
		m_ppcArguments[ i ] = M4CloneString( vpcArguments[ i ] ) ;
		CHECK_CHLOG_ERRORF( m_ppcArguments[ i ] == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	// Java trace
	pccTrace = ai_poEnvironment->GetJavaTrace() ;

	if( pccTrace != NULL && strcmp( pccTrace, "1" ) == 0 )
	{
		m_bTrace = M4_TRUE ;
	}
	else
	{
		m_bTrace = M4_FALSE ;
	}

	// Timeout
	m_iTimeOut = ai_poEnvironment->GetJavaTimeout() ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaConfiguration::Close( void )
{

	m4uint32_t	i = 0 ;


	M4FreeString( m_pcJarDirectory ) ;
	M4FreeString( m_pcHome ) ;
	M4FreeString( m_pcExecutable ) ;
	M4FreeString( m_pcClassPath ) ;
	M4FreeString( m_pcClassPathFile ) ;

	if( m_ppcArguments != NULL )
	{
		for( i = 0 ; i < m_iArgumentsLength ; i++ )
		{
			M4FreeString( m_ppcArguments[ i ] ) ;
		}
		delete( m_ppcArguments ) ;
		m_ppcArguments = NULL ;
	}
	m_iArgumentsLength = 0 ;

	return( M4_SUCCESS ) ;
}


// Funciones de classpath =====================================================

m4pcchar_t	M4JavaConfiguration::GetClassPath( m4pcchar_t& ao_rpccClassPathFile )
{
	ClMutBlock	oBlock( &m_oClassPathMutex ) ;

	ao_rpccClassPathFile = m_pcClassPathFile ;
	return( m_pcClassPath ) ;
}


m4return_t	M4JavaConfiguration::SetClassPath( m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccFileClassPath )
{
	int			iLength = -1 ;
	m4pchar_t	pcClassPath = NULL ;
	m4pchar_t	pcANSIFileClassPath = NULL ;
	m4pchar_t	pcClassPathFile = NULL ;
	m4char_t	acFile[ M4_JAVA_MAX_PATH + 1 ] ;
	FILE		*pfFile = NULL ;


	pcClassPath = M4CloneString( ai_pccClassPath ) ;
	CHECK_CHLOG_ERRORF( pcClassPath == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	sprintf( acFile, "%s%sm4classpath.cfg", m_pcJarDirectory, M4_DIRECTORY_SEPARATOR ) ;
	pcClassPathFile = M4CloneString( acFile ) ;
	CHECK_CHLOG_ERRORF( pcClassPathFile == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ClMutBlock	oBlock( &m_oClassPathMutex ) ;

	M4FreeString( m_pcClassPath ) ;
	m_pcClassPath = pcClassPath ;

	M4FreeString( m_pcClassPathFile ) ;
	m_pcClassPathFile = pcClassPathFile ;

	eUniFileEncodingType_t	eEncoding = M4UniANSI ;
	pfFile = M4Fopen( pcClassPathFile, M4UniWrite, eEncoding ) ;

	if( pfFile != NULL )
	{
		iLength = -1 ;
		pcANSIFileClassPath = M4CppToANSI( ai_pccFileClassPath, iLength ) ;

		fprintf( pfFile, "%s", pcANSIFileClassPath ) ;
		delete( pcANSIFileClassPath ) ;
		fclose( pfFile ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaConfiguration::NormalizePath( m4pcchar_t ai_pccPath, m4bool_t ai_bDecompress, m4bool_t& ao_rbNeedToDecompress, string& ao_rsJarPath )
{

	m4return_t		iResult = M4_ERROR ;
	int				iLength = -1 ;
	int				iStatus = -1 ;
	int				iRename = -1 ;
	m4uint32_t		iCrc = 0 ;
	struct stat		stStatus ;
	m4pchar_t		pcJarFile = NULL ;
	m4pchar_t		pcJarFileName = NULL ;
	m4pchar_t		pcTempDirectory = NULL ;
	m4char_t		acJarDirectory [ M4_JAVA_MAX_PATH + 1 ] ;
	m4char_t		acTempDirectory[ M4_JAVA_MAX_PATH + 1 ] ;
	m4char_t		acJarNameFile[ M4_JAVA_MAX_PATH + 1 ] ;
	FILE			*pfFile = NULL ;
	static ClMutex	s_oMutex( M4_TRUE ) ;


	ao_rbNeedToDecompress = M4_FALSE ;
	ao_rsJarPath = ai_pccPath ;

	CHECK_CHLOG_ERRORF( ai_pccPath == NULL, M4_ERROR, M4JAVA_NULL_ARGUMENT, "ai_pccPath" << "M4JavaConfiguration::NormalizePath" << __LINE__ << __FILE__ ) ;

	iLength = strlen( ai_pccPath ) ;

	if( iLength > 4 && strcmpi( ai_pccPath + iLength - 4, ".jar" ) == 0 )
	{
		// Si es un jar se intenta usar el directorio expandido
		iCrc = _CalculateCrc( ai_pccPath ) ;

		if( iCrc > 0 )
		{
			// Si tiene crc se busca el directorio expandido
			sprintf( acJarDirectory, "%s%s%lu.jar", m_pcJarDirectory, M4_DIRECTORY_SEPARATOR, iCrc ) ;

			iStatus = M4FileStatus( acJarDirectory, stStatus ) ;

			if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) != 0 )
			{
				// Si existe y es un directorio se ha encontrado
				ao_rsJarPath = acJarDirectory ;
			}
			else
			{
				// Si no existe o no es un directorio hay que descomprimir
				ao_rbNeedToDecompress = M4_TRUE ;

				if( ai_bDecompress == M4_TRUE )
				{
					// Toda expansión se hace serializada
					ClMutBlock	oBlock( &s_oMutex ) ;

					iStatus = M4FileStatus( acJarDirectory, stStatus ) ;

					if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) != 0 )
					{
						// Si existe y es un directorio se ha encontrado
						ao_rsJarPath = acJarDirectory ;
						ao_rbNeedToDecompress = M4_FALSE ;
					}
					else
					{
						if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) == 0 )
						{
							// Si existe y no es un directorio se borra
							M4RemoveFile( acJarDirectory ) ;
						}

						// Si no existe como directorio se intenta expandir
						sprintf( acTempDirectory, "%s%s~%lu.jar", m_pcJarDirectory, M4_DIRECTORY_SEPARATOR, iCrc ) ;
						iStatus = M4FileStatus( acTempDirectory, stStatus ) ;

						if( iStatus == 0 )
						{
							// Si existe el temporal se borra
							if( ( stStatus.st_mode & S_IFDIR ) == 0 )
							{
								// Si es fichero
								M4RemoveFile( acTempDirectory ) ;
							}
							else
							{
								// Si es directorio
								ClFileDirOperations::RemoveDirectory( acTempDirectory ) ;
							}
						}

						iLength = -1 ;
						pcJarFile = M4CppToANSI( ai_pccPath, iLength ) ;

						iLength = -1 ;
						pcTempDirectory = M4CppToANSI( acTempDirectory, iLength ) ;

						iResult = M4FMExpandZip( pcJarFile, pcTempDirectory ) ;

						if( pcJarFile != NULL )
						{
							delete( pcJarFile ) ;
						}

						if( pcTempDirectory != NULL )
						{
							delete( pcTempDirectory ) ;
						}

						if( iResult == 0 )
						{
							iStatus = M4FileStatus( acTempDirectory, stStatus ) ;

							if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) != 0 )
							{
								// Se renombra el directorio
								iRename = M4RenameFile( acTempDirectory, acJarDirectory ) ;

								if( iRename == 0 )
								{
									iStatus = M4FileStatus( acJarDirectory, stStatus ) ;

									if( iStatus == 0 && ( stStatus.st_mode & S_IFDIR ) != 0 )
									{
										// Si existe y es un directorio se ha encontrado
										ao_rsJarPath = acJarDirectory ;
										ao_rbNeedToDecompress = M4_FALSE ;

										// Se escribe el nombre original
										pcJarFileName = strrchr( (m4pchar_t)ai_pccPath, '\\' ) ;

										if( pcJarFileName == NULL )
										{
											pcJarFileName = strrchr( (m4pchar_t)ai_pccPath, '/' ) ;
										}

										if( pcJarFileName != NULL )
										{
											sprintf( acJarNameFile, "%s%s%s", acJarDirectory, M4_DIRECTORY_SEPARATOR, pcJarFileName + 1 ) ;
											acJarNameFile[ strlen( acJarNameFile ) - 4 ] = '\0' ;

											eUniFileEncodingType_t	eEncoding = M4UniNative ;
											pfFile = M4Fopen( acJarNameFile, M4UniWrite, eEncoding ) ;

											if( pfFile != NULL )
											{
												fprintf( pfFile, ai_pccPath ) ;
												fclose( pfFile ) ;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaConfiguration::NormalizeClassPath( m4pcchar_t ai_pccClassPath, m4bool_t ai_bDecompress, m4bool_t& ao_rbNeedToDecompress, string& ao_rsClassPath, string& ao_rsFileClassPath )
{

	m4return_t			iResult = M4_ERROR ;
	m4uint32_t			i = 0 ;
	m4uint32_t			iSize = 0 ;
	m4uint32_t			iLength = 0 ;
	m4pcchar_t			pccFileName = NULL ;
	string				sSource ;
	string				sExpandedPath ;
	ClFile				oFile ;
	vector<m4pchar_t>	vpcPaths ;


	ao_rbNeedToDecompress = M4_FALSE ;
	ao_rsClassPath = "" ;
	ao_rsFileClassPath = "" ;

	if( ai_pccClassPath != NULL && *ai_pccClassPath != '\0' )
	{
		sSource = ai_pccClassPath ;
		iResult = M4Split( (m4pchar_t)sSource.c_str(), M4_JAVA_PATH_SEPARATOR, vpcPaths ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	iSize = vpcPaths.size() ;

	for( i = 0 ; i < iSize ; i++ )
	{
		iResult = _ExpandPath( vpcPaths[ i ], sExpandedPath ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		iLength = sExpandedPath.size() ;

		if( iLength > 2 && sExpandedPath.at( iLength - 1 ) == '*' && ( sExpandedPath.at( iLength - 2 ) == '\\' || sExpandedPath.at( iLength - 2 ) == '/' ) )
		{
			// Se añaden los jar del directorio del asterisco
			ClFileFinder	oFileFinder( sExpandedPath.c_str() ) ;

			while( oFileFinder.FindNext( oFile ) != M4_ERROR )
			{
				if( oFile.IsValid() == M4_FALSE )
				{
					continue ;
				}
				
				pccFileName = oFile.GetFileName() ;
				iLength = strlen( pccFileName ) ;

				if( oFile.IsFile() == M4_TRUE )
				{
					if( iLength > 4 && strcmpi( pccFileName + iLength - 4, ".jar" ) == 0 )
					{
						iResult = _AddPath( pccFileName, ai_bDecompress, M4_TRUE, ao_rbNeedToDecompress, ao_rsClassPath, ao_rsFileClassPath ) ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}
					}
				}
				else
				{
					if( iLength > 10 && strcmpi( pccFileName + iLength - 10, "properties" ) == 0 && ( pccFileName[ iLength - 11 ] == '\\' || pccFileName[ iLength - 11 ] == '/' ) )
					{
						if( ao_rsFileClassPath.size() > 0 )
						{
							ao_rsFileClassPath.append( "\n" ) ;
						}
						ao_rsFileClassPath.append( sExpandedPath.substr( 0, sExpandedPath.size() - 2 ) ) ;
					}
				}
			}
		}
		else
		{
			// Se añaden los path sin asterisco
			// Bug 0289814 y sin descomprimir
			iResult = _AddPath( sExpandedPath.c_str(), M4_FALSE, M4_FALSE, ao_rbNeedToDecompress, ao_rsClassPath, ao_rsFileClassPath ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}


// Funciones internas =========================================================

void	M4JavaConfiguration::_Init( void )
{
	m_pcJarDirectory = NULL ;
	m_pcHome = NULL ;
	m_pcExecutable = NULL ;
	m_pcClassPath = NULL ;
	m_pcClassPathFile = NULL ;
	m_ppcArguments = NULL ;
	m_iArgumentsLength = 0 ;
	m_bTrace = M4_TRUE ;
	m_iTimeOut = 0 ;

	m_oClassPathMutex.Init() ;
}


m4pcchar_t	M4JavaConfiguration::_GetHome( void )
{

	int	iLength = 0 ;


	if( m_pcHome == NULL )
	{
#ifdef _WINDOWS

		BOOL		bResult = FALSE ;
		DWORD		dwResult = 0 ;
		M4XMLCh		awcDirectory[ M4_JAVA_MAX_PATH + 1 ] ;

		dwResult = GetModuleFileNameW( NULL, (LPWSTR) awcDirectory, M4_JAVA_MAX_PATH ) ;

		if( dwResult > 0 )
		{
			bResult = PathRemoveFileSpecW( (LPWSTR) awcDirectory ) ;

			iLength = -1 ;
			m_pcHome = M4Utf16ToCpp( awcDirectory, iLength ) ;
		}
#else
		iLength = -1 ;
		m_pcHome = M4ANSIToCpp( getenv( "APPHOME" ), iLength ) ;
#endif
	}

	return( m_pcHome ) ;
}


m4return_t	M4JavaConfiguration::_ExpandPath( m4pcchar_t ai_pccPath, string& ao_rsPath )
{

	m4uint32_t	iLength = 0 ;
	m4pcchar_t	pccHome = NULL ;


	ao_rsPath = "" ;

	CHECK_CHLOG_ERRORF( ai_pccPath == NULL, M4_ERROR, M4JAVA_NULL_ARGUMENT, "ai_pccPath" << "M4JavaConfiguration::_ExpandPath" << __LINE__ << __FILE__ ) ;

	iLength = strlen( ai_pccPath ) ;

	if( iLength >= 2 && ai_pccPath[ 0 ] == '.' && ( ai_pccPath[ 1 ] == '\\' || ai_pccPath[ 1 ] == '/' ) )
	{
		pccHome = _GetHome() ;

		if( pccHome != NULL )
		{
			ao_rsPath.append( pccHome ) ;
			ai_pccPath++ ;
		}
	}

	ao_rsPath.append( ai_pccPath ) ;
	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaConfiguration::_AddPath( m4pcchar_t ai_pccPath, m4bool_t ai_bDecompress, m4bool_t ai_bNormalize, m4bool_t& ao_rbNeedToDecompress, string& ao_rsClassPath, string& ao_rsFileClassPath )
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bNeedToDecompress = M4_FALSE ;
	m4uint32_t	iLength = 0 ;
	string		sJarPath ;

	
	CHECK_CHLOG_ERRORF( ai_pccPath == NULL, M4_ERROR, M4JAVA_NULL_ARGUMENT, "ai_pccPath" << "M4JavaConfiguration::_AddPath" << __LINE__ << __FILE__ ) ;

	if( ai_bNormalize == M4_TRUE )
	{
		iResult = NormalizePath( ai_pccPath, ai_bDecompress, bNeedToDecompress, sJarPath ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}
	else
	{
		sJarPath = ai_pccPath ;
	}

	if( bNeedToDecompress == M4_TRUE )
	{
		ao_rbNeedToDecompress = M4_TRUE ;
	}

	iLength = strlen( ai_pccPath ) ;

	if( ao_rsClassPath.size() == 0 )
	{
		if( iLength >= 17 && strcmpi( ai_pccPath + iLength - 17, "m4javaservice.jar" ) == 0 )
		{
			ao_rsClassPath = sJarPath ;
			return( M4_SUCCESS ) ;
		}
	}

	if( iLength >= 12 && strcmpi( ai_pccPath + iLength - 12, "m4common.jar" ) == 0 )
	{
		// Se pone el m4common al principio
		if( ao_rsFileClassPath.size() > 0 )
		{
			sJarPath.append( "\n" ) ;
		}
		ao_rsFileClassPath.insert( 0, sJarPath ) ;
	}
	else
	{
		if( ao_rsFileClassPath.size() > 0 )
		{
			ao_rsFileClassPath.append( "\n" ) ;
		}
		ao_rsFileClassPath.append( sJarPath ) ;
	}

	return( M4_SUCCESS ) ;
}


m4uint32_t	M4JavaConfiguration::_CalculateCrc( m4pcchar_t ai_pccPath )
{

	m4uint32_t	iCrc = 0 ;
	size_t		iRead = 0 ;
	m4char_t	acBuffer[ 1024 * 16 + 1 ] ;
	FILE		*pfFile = NULL ;


	CHECK_CHLOG_ERRORF( ai_pccPath == NULL, M4_ERROR, M4JAVA_NULL_ARGUMENT, "ai_pccPath" << "M4JavaConfiguration::_CalculateCrc" << __LINE__ << __FILE__ ) ;

	eUniFileEncodingType_t	eEncoding = M4UniANSI ;
	pfFile = M4Fopen( ai_pccPath, M4UniReadBinary, eEncoding ) ;

	if( pfFile != NULL )
	{
		// Si existe el fichero se calcula el crc
		do
		{
			iRead = fread( acBuffer, 1, 1024 * 16, pfFile ) ;
		
			if( iRead > 0 )
			{
				iCrc = crc32( iCrc, (const unsigned char*) acBuffer, iRead ) ;
			}
		}
		while( iRead > 0 ) ;

		fclose( pfFile ) ;
	}

	return( iCrc ) ;
}


