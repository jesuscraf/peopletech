
//==============================================================================
//
// (c) Copyright  1991-2016 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4cataloguer
// File:                m4cataloguer.hpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                09-02-2016
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements the class for creating installation catalogues
//
//==============================================================================


#include "m4cataloguer.hpp"
#include "m4fileinfo.hpp"
#include "m4patternreader.hpp"
#include "m4clock.hpp"
#include "m4date.hpp"
#include "m4unicode.hpp"
#include "file_finder.hpp"
#include "zlib.h"
#include "alloc.h"
#include "7zfile.h"

#ifdef _UNIX
#include <utime.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#else
#include <conio.h>
#include <sys/utime.h>
#endif


extern void	M4SetUnicode( int ) ;

#define	M4_CATALOGUER_BUFFER_SIZE		1024 * 256




//=================================================================================
// Funciones auxiliares
//=================================================================================

#ifdef _UNIX

static void	SwapInt64( m4pchar_t ai_pcBuffer )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 7 ) ;
	*( pcNumber + 7 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 5 ) ;
	*( pcNumber + 5 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;
}

#endif



static void	Replace( m4pchar_t ai_pcBuffer, m4char_t ai_iOldChar, m4char_t ai_iNewChar )
{
	while( *ai_pcBuffer != '\0' )
	{
		if( *ai_pcBuffer == ai_iOldChar )
		{
			*ai_pcBuffer = ai_iNewChar ;
		}
		ai_pcBuffer++ ;
	}
}




//=================================================================================
//
// ICodeProgress interface implementation.
//
// Parameters
//		p: input size. Pointer to progress object
//		inSize: input size. -1 if unknown
//		outSize: output size. -1 if unknown
//
//=================================================================================

static SRes	SetProgress( void *p, UInt64 inSize, UInt64 outSize )
{

	M4Cataloguer	*poCataloguer = (M4Cataloguer*)p ;

	if( inSize > 0 )
	{
		poCataloguer->SetProgress( poCataloguer->GetTotalBytesRead() + m4uint32_t( inSize ) ) ;
	}
	return( 0 ) ;
}




//=================================================================================
//
// M4Cataloguer
//
// This class implements installation catalogue creator
//
//=================================================================================

M4Cataloguer::M4Cataloguer( m4pcchar_t ai_pccDirectory )
{
	Progress = ::SetProgress ;
	m_iLastPercentage = 0 ;
	m_iTotalBytesRead = 0 ;
	m_iTotalBytesToRead = 0 ;
	m_pccRootDirectory = ai_pccDirectory ;
	m_iRootDirectoryLength = strlen( ai_pccDirectory ) + 1 ;
	m_pccZipExension = ".m4z" ;
	m_iZipExtensionLength = strlen( m_pccZipExension ) ;
}



M4Cataloguer::~M4Cataloguer( void )
{

	vector<m4pchar_t>::iterator	it ;


	it = m_oStartIncludes.begin() ;

	while( it != m_oStartIncludes.end() )
	{
		delete( *it ) ;
		it++ ;
	}

	it = m_oEndIncludes.begin() ;

	while( it != m_oEndIncludes.end() )
	{
		delete( *it ) ;
		it++ ;
	}

	it = m_oStartExcludes.begin() ;

	while( it != m_oStartExcludes.end() )
	{
		delete( *it ) ;
		it++ ;
	}

	it = m_oEndExcludes.begin() ;

	while( it != m_oEndExcludes.end() )
	{
		delete( *it ) ;
		it++ ;
	}
}



//=================================================================================
//
// Sets progress from bytes processed.
//
// Parameters
//		ai_iBytes: Bytes processed so far
//
//=================================================================================

void	M4Cataloguer::SetProgress( m4uint32_t ai_iBytes )
{
	// Se va del 5 al 99%
	_OnProgressChanged( 5 + ( m4uint64_t( ai_iBytes ) * 95 / m_iTotalBytesToRead ) ) ;
}



//=================================================================================
//
// Generates catalogue using specified directory.
//
// Returns
//		Number of file infos processed; -2 if cataloguer is already runinng
//
//=================================================================================

m4int32_t	M4Cataloguer::Catalogue( void )
{

	m4int32_t	iResult = -1 ;
	m4int32_t	iBlocked = 0 ;
	int			iLength = -1 ;
	m4pchar_t	pcCataloguingFile = NULL ;
	m4char_t	acCataloguingFile[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	FILE*		pfFile = NULL ;


	// Se verifica si la generación ya está en marcha
	sprintf( acCataloguingFile, "%s%s~cataloging", m_pccRootDirectory, M4_DIRECTORY_SEPARATOR ) ;
	pcCataloguingFile = M4CppToANSI( acCataloguingFile, iLength ) ;

#ifdef _UNIX
	struct flock	stLock ;
	memset( &stLock, 0, sizeof( stLock ) ) ;

	int	iFd = open( pcCataloguingFile, O_RDWR | O_CREAT, 0666 ) ;

	if( iFd == -1 )
	{
		iBlocked = -1 ;
	}

	/* Bug 0320952 Ya no se bloquea el fichero
	else
	{
		stLock.l_type = F_WRLCK ;

		if( fcntl( iFd, F_SETLK, &stLock ) == -1 )
		{
			iBlocked = -1 ;
			if( errno == EAGAIN || errno == EACCES )
			{
				iBlocked = -2 ;
			}
		}
	}
	*/
#else
	HANDLE	hHandle = CreateFile( pcCataloguingFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	if( hHandle == INVALID_HANDLE_VALUE )
	{
		iBlocked = -1 ;
		if( GetLastError() == ERROR_SHARING_VIOLATION )
		{
			iBlocked = -2 ;
		}
	}
#endif

	if( iBlocked != 0 )
	{
		if( iBlocked == -2 )
		{
			// Si ya se está ejecutando se para
			printf( "\nAnother catalogue process is already in progress." ) ;
		}
		else
		{
			printf( "\nCannot create <~cataloging> for concurrency checking. Reason <%s>.", strerror( errno ) ) ;
		}
		return( -1 ) ;
	}

	iResult =  _Catalogue() ;

#ifdef _UNIX
	stLock.l_type = F_UNLCK ;
	fcntl( iFd, F_SETLK, &stLock ) ;
	close( iFd ) ;
#else
	CloseHandle( hHandle ) ;
#endif

	remove( pcCataloguingFile ) ;
	delete( pcCataloguingFile ) ;

	return( iResult ) ;
}



//=================================================================================
//
// Action to perform on progress change.
//
// Parameters
//		ai_Percentage: Progress percentage
//
//=================================================================================

void	M4Cataloguer::_OnProgressChanged( m4uint32_t ai_Percentage )
{
	if( ai_Percentage != m_iLastPercentage )
	{
		m_iLastPercentage = ai_Percentage ;
		printf( "\r%d%%", ai_Percentage ) ;
		fflush( stdout ) ;
	}
}



//=================================================================================
//
// Generates catalogue using specified directory.
// Does not check whether cataloguer is already runinng or not.
//
// Returns
//		Number of file infos processed; -1 if failure
//
//=================================================================================

m4int32_t	M4Cataloguer::_Catalogue( void )
{

	m4return_t		iResult = M4_ERROR ;
	SRes			iSRres = 0;
	m4pchar_t		pcBuffer = NULL ;
	m4char_t		acHelpCatalogue[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	CLzmaEncHandle	oEncoder = NULL ;
	CLzmaEncProps	oProperties ;
	vector<ClFile>	vFileList ;
	vector<ClFile>	vHelpList ;


	// Se empieza en el 1%
	_OnProgressChanged( 1 ) ;

	// Se obtienen los patrones
	m_oStartIncludes.clear() ;
	m_oEndIncludes.clear() ;
	m_oStartExcludes.clear() ;
	m_oEndExcludes.clear() ;

	iResult = M4PatternReader::ReadPatterns( m_pccRootDirectory, m_oStartIncludes, m_oEndIncludes, m_oStartExcludes, m_oEndExcludes ) ;

	if( iResult != M4_SUCCESS )
	{
		return( -1 ) ;
	}

	iResult = _GenerateFileLists( m_pccRootDirectory, vFileList, vHelpList, m_iTotalBytesToRead ) ;

	if( iResult != M4_SUCCESS )
	{
		return( -1 ) ;
	}

	// Se avanza al 5%
	_OnProgressChanged( 5 ) ;

	// Se crea el compresor
	oEncoder = LzmaEnc_Create( &g_Alloc ) ;

	if( oEncoder == NULL )
	{
		printf( "\nNot enough memory to allocate <CLzmaEnc>." ) ;
		return( -1 ) ;
	}

	LzmaEncProps_Init( &oProperties ) ;
	oProperties.dictSize = 1 << 23 ;
	oProperties.fb = 128 ;

	iSRres = LzmaEnc_SetProps( oEncoder, &oProperties ) ;

	if( iResult != 0 )
	{
		printf( "\nError <%d> in function <LzmaEnc_SetProps>.", iSRres ) ;
		return( -1 ) ;
	}

	pcBuffer = new m4char_t[ M4_CATALOGUER_BUFFER_SIZE ] ;

	if( pcBuffer == NULL )
	{
		printf( "\nNot enough memory to allocate <%d> bytes.", ( m4uint32_t ) ( sizeof( m4char_t ) * ( M4_CATALOGUER_BUFFER_SIZE ) ) ) ;
		return( -1 ) ;
	}

	// Se generan los ficheros del catálogo de ayuda
	iResult = _GenerateCatalogue( m_pccRootDirectory, "m4helpcatalogue.xml", vHelpList, pcBuffer, M4_CATALOGUER_BUFFER_SIZE, oEncoder ) ;

	if( iResult != M4_SUCCESS )
	{
		delete( pcBuffer ) ;
		return( -1 ) ;
	}

	// Se generan los ficheros del catálogo, con el de ayuda incluido
	sprintf( acHelpCatalogue, "%s%sm4helpcatalogue.xml", m_pccRootDirectory, M4_DIRECTORY_SEPARATOR ) ;

	ClFile	oFile( acHelpCatalogue ) ;
	vFileList.push_back( oFile ) ;

	m_iTotalBytesToRead += oFile.GetSize() ;

	iResult = _GenerateCatalogue( m_pccRootDirectory, "m4catalogue.xml", vFileList, pcBuffer, M4_CATALOGUER_BUFFER_SIZE, oEncoder ) ;

	LzmaEnc_Destroy( oEncoder, &g_Alloc, &g_Alloc ) ;
	delete( pcBuffer ) ;

	if( iResult != M4_SUCCESS )
	{
		return( -1 ) ;
	}

	// Se avanza al 100%
	_OnProgressChanged( 100 ) ;

	// Se retorna el número
	return ( m4int32_t )( vFileList.size() + vHelpList.size() ) ;
}



//=================================================================================
//
// Generates recursivelly file lists to process from a directory.
//
// Parameters
//		ai_pccDirectory: Directory to get files from
//		ai_rvFileList: List where to place files to process
//		ai_rvHelpList: List where to place help files to process
//		ao_riTotalBytes: [ref] Number of bytes to be processed
//
// Returns
//		M4_ERROR if failure; M4_SUCCESS otherwise
//
//=================================================================================

m4return_t	M4Cataloguer::_GenerateFileLists( m4pcchar_t ai_pccDirectory, vector<ClFile>& ai_rvFileList, vector<ClFile>& ai_rvHelpList, m4uint32_t& ao_riTotalBytes )
{

	m4return_t		iResult = M4_ERROR ;
	size_t			iLength = 0 ;
	m4pcchar_t		pccFilename = NULL ;
	m4char_t		acDirectoryPattern[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	ClFile			oFile ;
	vector<ClFile>	oChilds ;

	vector<ClFile>::iterator	it ;


	sprintf( acDirectoryPattern, "%s%s*", ai_pccDirectory, M4_DIRECTORY_SEPARATOR ) ;

	ClFileFinder	oFileFinder( acDirectoryPattern ) ;

	while( oFileFinder.FindNext( oFile ) != M4_ERROR )
	{
		if( oFile.IsValid() == M4_FALSE )
		{
			continue ;
		}
		
		if( oFile.IsFile() == M4_TRUE )
		{
			// Para los ficheros
			pccFilename = oFile.GetFileName() ;

			if( _HasToInclude( pccFilename ) )
			{
				iLength = strlen( pccFilename ) ;

				if( iLength > 4 && strcmpi( pccFilename + iLength - 4, ".chm" ) == 0 )
				{
					ai_rvHelpList.push_back( oFile ) ;
				}
				else
				{
					ai_rvFileList.push_back( oFile ) ;
				}
				ao_riTotalBytes += oFile.GetSize() ;
			}
		}
		else
		{
			oChilds.push_back( oFile ) ;
		}
	}

	// Para los directorios hijos
	it = oChilds.begin() ;

	while( it != oChilds.end() )
	{
		iResult = _GenerateFileLists( it->GetFileName(), ai_rvFileList, ai_rvHelpList, ao_riTotalBytes ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
		it++ ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Calculates whether a file is to be included or not.
//
// Parameters
//		ai_pccFilename: File to check for inclusion
//
// Returns
//		M4_TRUE if include path; M4_FALSE otherwise
//
//=================================================================================

m4bool_t	M4Cataloguer::_HasToInclude( m4pcchar_t ai_pccFilename )
{

	size_t		iLength = 0 ;
	size_t	iPatternLength = 0 ;
	m4char_t	acValue[ M4_CATALOGUER_MAX_PATH + 1 ] ;

	vector<m4pchar_t>::iterator it ;


	if( ai_pccFilename == NULL )
	{
		return( M4_FALSE ) ;
	}

	iLength = strlen( ai_pccFilename ) ;

	if( iLength > m_iZipExtensionLength && strcmpi( ai_pccFilename + iLength - m_iZipExtensionLength, m_pccZipExension ) == 0 )
	{
		// Si es comprimido no hay que procesarlo
		return( M4_FALSE ) ;
	}

	memcpy( acValue, ai_pccFilename + m_iRootDirectoryLength, iLength - m_iRootDirectoryLength + 1 ) ;
	Replace( acValue, '\\', '/' ) ;
	iLength = strlen( acValue ) ;

	it = m_oStartExcludes.begin() ;

	while( it != m_oStartExcludes.end() )
	{
		iPatternLength = strlen( *it ) ;

		if( iLength >= iPatternLength && strnicmp( *it, acValue, iPatternLength ) == 0 )
		{
			return( M4_FALSE ) ;
		}
		it++ ;
	}

	it = m_oEndExcludes.begin() ;

	while( it != m_oEndExcludes.end() )
	{
		iPatternLength = strlen( *it ) ;

		if( iLength >= iPatternLength && strnicmp( *it, acValue + iLength - iPatternLength, iPatternLength ) == 0 )
		{
			return( M4_FALSE ) ;
		}
		it++ ;
	}

	it = m_oStartIncludes.begin() ;
	
	while( it != m_oStartIncludes.end() )
	{
		iPatternLength = strlen( *it ) ;
		
		if( iLength >= iPatternLength && strnicmp( *it, acValue, iPatternLength ) == 0 )
		{
			return( M4_TRUE ) ;
		}
		it++ ;
	}

	it = m_oEndIncludes.begin() ;

	while( it != m_oEndIncludes.end() )
	{
		iPatternLength = strlen( *it ) ;

		if( iLength >= iPatternLength && strnicmp( *it, acValue + iLength - iPatternLength, iPatternLength ) == 0 )
		{
			return( M4_TRUE ) ;
		}
		it++ ;
	}

	return( M4_FALSE ) ;
}



//=================================================================================
//
// Generates catalogue file from file list.
//
// Parameters
//		ai_pccDirectory: Directory to generate file infos from
//		ai_pccCatalogue: Catalogue name
//		ai_rvList: File list to write to catalogue
//		ai_pcBuffer: Auxiliar buffer for reading
//		ai_iBufferSize: Auxiliar buffer size
//		ai_roEncoder: LZMA encoder
//
// Returns
//		M4_ERROR if failure; M4_SUCCESS otherwise
//
//=================================================================================

m4return_t	M4Cataloguer::_GenerateCatalogue( m4pcchar_t ai_pccDirectory, m4pcchar_t ai_pccCatalogue, vector<ClFile>& ai_rvList, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, CLzmaEncHandle& ai_roEncoder )
{

	m4return_t			iResult = M4_ERROR ;
	m4char_t			acCatalogueFile[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	M4FileInfo			*poFile = NULL ;
	vector<M4FileInfo*>	vFiles ;

	vector<ClFile>::iterator		it ;
	vector<M4FileInfo*>::iterator	itFiles ;


	// Se generan los ficheros de la lista
	it = ai_rvList.begin() ;
	
	while( it != ai_rvList.end() )
	{
		iResult = _GenerateFileInfo( *it, ai_pcBuffer, ai_iBufferSize, ai_roEncoder, poFile ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		vFiles.push_back( poFile ) ;

		m_iTotalBytesRead += it->GetSize() ;
		SetProgress( m_iTotalBytesRead ) ;

		it++ ;
	}

	// Se vuelca el catálogo
	sprintf( acCatalogueFile, "%s%s%s", ai_pccDirectory, M4_DIRECTORY_SEPARATOR, ai_pccCatalogue ) ;
	iResult = M4FileInfo::Write( acCatalogueFile, vFiles ) ;

	itFiles = vFiles.begin() ;

	while( itFiles != vFiles.end() )
	{
		if( *itFiles != NULL )
		{
			delete( *itFiles ) ;
		}
		itFiles++;
	}

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Generates file info from a file.
//
// Parameters
//		ai_rFile: File to get file info from
//		ai_pcBuffer: Auxiliar buffer for reading
//		ai_iBufferSize: Auxiliar buffer size
//		ao_rpoFile: FileInfo generated
//
// Returns
//		M4_ERROR if failure; M4_SUCCESS otherwise
//
//=================================================================================

m4return_t	M4Cataloguer::_GenerateFileInfo( ClFile& ai_roFile, m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, CLzmaEncHandle& ai_roEncoder, M4FileInfo* &ao_rpoFile )
{

	SRes				iSRres = 0 ;
	size_t				iRead = 0 ;
	size_t				iBytes = 0 ;
	int					iLength = 0 ;
	m4uint32_t			iSize = 0 ;
	m4uint32_t			iCrc = 0 ;
	m4uint64_t			iHeader = 0 ;
	m4date_t			dLastWrite = 0 ;
	m4pcchar_t			pccFilename = NULL ;
	m4pchar_t			pcANSIFilename = NULL ;
	m4pchar_t			pcANSIZipFilename = NULL ;
	m4char_t			acValue[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	m4char_t			acZipFile[ M4_CATALOGUER_MAX_PATH + 1 ] ;
	FILE				*pfFile = NULL ;
	CFileSeqInStream	inStream ;
	CFileOutStream		outStream ;
	struct stat			stStatus ;
	struct utimbuf		stTimebuf ;


	ao_rpoFile = NULL ;

	if( ai_roFile.IsFile() == M4_TRUE )
	{
		pccFilename = ai_roFile.GetFileName() ;

		iLength = (int) strlen( pccFilename ) ;

		if( iLength < 4 || ( strcmpi( pccFilename + iLength - 4, ".chm" ) != 0 && strcmpi( pccFilename + iLength - 4, ".png" ) != 0 ) )
		{
			sprintf( acZipFile, "%s%s", pccFilename, m_pccZipExension ) ;
			ClFileState	oZipFile( acZipFile ) ;

			if( oZipFile.IsValid() == M4_FALSE || oZipFile.GetLastWrite() != ai_roFile.GetLastWrite() )
			{
				FileSeqInStream_CreateVTable( &inStream ) ;
				FileOutStream_CreateVTable(&outStream ) ;

				File_Construct( &inStream.file ) ;
				File_Construct( &outStream.file ) ;

				iLength = -1 ;
				pcANSIFilename = M4CppToANSI( pccFilename, iLength ) ;
				iSRres = InFile_Open( &inStream.file, pcANSIFilename ) ;
				delete( pcANSIFilename ) ;

				if( iSRres != 0 )
				{
					printf( "\nError <%d> in function <InFile_Open> for <%s>.", iSRres, pccFilename ) ;
					return( M4_ERROR ) ;
				}

				iLength = -1 ;
				pcANSIZipFilename = M4CppToANSI( acZipFile, iLength ) ;
				iSRres = OutFile_Open( &outStream.file, pcANSIZipFilename ) ;
				delete( pcANSIZipFilename ) ;

				if( iSRres != 0 )
				{
					printf( "\nError <%d> in function <OutFile_Open> for <%s>.", iSRres, acZipFile ) ;
					File_Close( &inStream.file ) ;
					return( M4_ERROR ) ;
				}

				iBytes = 8 ;
				iHeader = ai_roFile.GetSize() ;
#ifdef _UNIX
				SwapInt64( (m4pchar_t) &iHeader ) ;
#endif
				iSRres = File_Write( &outStream.file, &iHeader, &iBytes ) ;

				if( iSRres != 0 )
				{
					printf( "\nError <%d> in function <File_Write>.", iSRres ) ;
					return( M4_ERROR ) ;
				}

				iSRres = LzmaEnc_Encode( ai_roEncoder, &outStream.s, &inStream.s, this, &g_Alloc, &g_Alloc ) ;

				File_Close( &outStream.file ) ;
				File_Close( &inStream.file ) ;

				if( iSRres != 0 )
				{
					printf( "\nError <%d> in function <LzmaEnc_Encode>.", iSRres ) ;
					return( M4_ERROR ) ;
				}

				// Se actualiza la fecha del zip
				M4FileStatus( pccFilename, stStatus ) ;
				stTimebuf.modtime = stStatus.st_mtime ;
				stTimebuf.actime = stStatus.st_atime ;

				iLength = -1 ;
				pcANSIZipFilename = M4CppToANSI( acZipFile, iLength ) ;
				utime( pcANSIZipFilename, &stTimebuf ) ;
				delete( pcANSIZipFilename ) ;
			}

			pccFilename = acZipFile ;
		}
	}

	eUniFileEncodingType_t	eEncoding = M4UniANSI ;
	pfFile = M4Fopen( pccFilename, M4UniReadBinary, eEncoding ) ;

	if( pfFile != NULL )
	{
		// Si existe el fichero se calcula el tamaño y el crc
		do
		{
			iRead = fread( ai_pcBuffer, 1, ai_iBufferSize, pfFile ) ;
		
			if( iRead > 0 )
			{
				iSize += (m4uint32_t) iRead ;
				iCrc = crc32( iCrc, (const unsigned char*) ai_pcBuffer, (unsigned int) iRead ) ;
			}
		}
		while( iRead > 0 ) ;

		fclose( pfFile ) ;
	}

	iLength = (int) strlen( pccFilename ) ;
	memcpy( acValue, pccFilename + m_iRootDirectoryLength, iLength - m_iRootDirectoryLength + 1 ) ;
	Replace( acValue, '\\', '/' ) ;

	ao_rpoFile = new M4FileInfo( acValue, iSize, iCrc ) ;
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Application main routine.
// Generates catalogue using specified directory.
//
//=================================================================================

int main(int argc, char* argv[])
{

	int				iLength = -1 ;
	m4int32_t		iProcessed = -1 ;
	m4millisec_t	iStart = 0 ;
	m4millisec_t	iEnd = 0 ;
	m4pchar_t		pccCppDirectory = NULL ;


	M4SetUnicode( 1 ) ;

	printf( "\nMeta4 PeopleNet Catalogue Creator 2.0\n" ) ;
	printf( "Copyright (C) Meta4 Spain S.A. 2016\n" ) ;
	
	if( argc < 2 )
	{
		printf( "\nNo directory specified. Please specify a directory." ) ;
	}
	else
	{
		printf( "\nCreating catalogue. Please wait.\n" ) ;

		iStart = ClPrecisionTicker::GetTimeTick() ;

		iLength = -1 ;
		pccCppDirectory = M4ANSIToCpp( argv[ 1 ], iLength ) ;
		M4Cataloguer	oCataloguer( pccCppDirectory ) ;

		iProcessed = oCataloguer.Catalogue() ;
		delete( pccCppDirectory ) ;

		iEnd = ClPrecisionTicker::GetTimeTick() ;

		if( iProcessed >= 0 )
		{
			printf( "\r<%d> files processed in <%lld> seconds to <m4catalogue.xml>\n", iProcessed, ( iEnd - iStart ) / 1000 ) ;
		}
	}

	if( argc < 3 || strcmp( argv[ 2 ], "0" ) != 0 )
	{
#ifdef _UNIX
		m4char_t	c = 0 ;

		printf( "\nPress enter to continue..." ) ;
		scanf( "%c", &c ) ;
#else
		printf( "\nPress any key to continue..." ) ;
		getch() ;
#endif
	}

	return( iProcessed ) ;
}


