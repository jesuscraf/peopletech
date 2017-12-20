
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4filemanager.dll
// File:                file_manager.cpp   
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


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>


#ifdef _UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <io.h>
#include <sys/stat.h>
#endif



#include <syncro.hpp>
#include "file_finder.hpp"
#include "m4string.hpp"

#include <file_misc.hpp>
#include <m4types.hpp>
#include <file_manager.hpp>
#include "m4unicode.hpp"


#ifdef _WINDOWS
	#ifdef _DEBUG
		#define LOG_FILE	"c:\\temp\\m4filemanager.log"
		static FILE *ficLog;
		#define REMOVE_LOGFILE unlink(LOG_FILE)
		// UNICODE FILE
		#define WRITE_LOGFILE(x) { eUniFileEncodingType_t eEncoding = M4UniNative; ficLog=M4Fopen(LOG_FILE, M4UniAppend, eEncoding); if (ficLog!=NULL) { fprintf(ficLog,"ClFileManager: %s\n",x); fclose(ficLog); } }
	#else
		#define REMOVE_LOGFILE
		#define WRITE_LOGFILE(x)
	#endif
#else		// no _WINDOWS
	#define REMOVE_LOGFILE
	#define WRITE_LOGFILE(x)
#endif	// _WINDOWS

// backslash
#ifdef _UNIX
	#define M4_FM_BACK_SLASH	'/'
#else
	#define M4_FM_BACK_SLASH	'\\'
#endif
#define M4_FM_UNIX_SLASH '/'
#define M4_FM_WIN_SLASH '\\'


ClMutex oMutexFileManager(M4_TRUE);
ClMutex oMutexTemporalFileCreator(M4_TRUE);



// Class ClFileManager 



#include "m4thread.hpp"

static void	DumpTrace( m4pcchar_t ai_pccFile, m4pcchar_t ai_pccString, m4int16_t ai_iErase, long ao_lReferences, long ao_lTotal )
{

	m4iden_t		lThread = 0 ;
	long			lTime = 0 ;

	static	FILE*	s_pfFile = NULL ;


	if( s_pfFile == NULL )
	{
		m4char_t	acBuffer[ 512 + 1 ] ;
		m4pchar_t	pccDirectory = NULL ;

#ifdef _WINDOWS
		pccDirectory = getenv( "TEMP" ) ;
#else
		pccDirectory = getenv( "HOME" ) ;
#endif

		if( pccDirectory != NULL )
		{
			sprintf( acBuffer, "%s%cfilemanager.log", pccDirectory, M4_FM_BACK_SLASH ) ;
		}
		else
		{
			sprintf( acBuffer, "filemanager.log" ) ;
		}

		s_pfFile = fopen( acBuffer, "w" ) ;
	}

	if( s_pfFile != NULL )
	{
#ifdef _WINDOWS
		lTime = GetTickCount() ;
#else
		struct timeval	stNow ;

		gettimeofday( &stNow, NULL ) ;
		lTime = stNow.tv_usec ;
#endif

		lThread = M4Thread::GetCurrentIdThread() ;

		fprintf( s_pfFile, "%ld\t@%ld\t%s\t%s\t%d\t%ld\t%ld\n", lTime, lThread, ai_pccFile, ai_pccString, ai_iErase, ao_lReferences, ao_lTotal ) ;
		fflush( s_pfFile ) ;
	}
}



ClFileManager ClFileManager::m_oInstance = ClFileManager();


ClFileManager::ClFileManager ()
{
	REMOVE_LOGFILE;
}


ClFileManager::~ClFileManager ()
{
#ifdef _DEBUG
	itClFileList itGarbage;
	m4char_t szText[M4_MAX_PATH+50];
#endif

	m4uint32_t cRef;
	itControlMap_t itControl;
	m4uint32_t cCont;

	CollectGarbage();	

	
	// See if there is some interest file
	itControl=m_mapControl.begin();
	while (itControl!=m_mapControl.end())
	{
		cRef=(*itControl).second.cRef;

#ifdef _DEBUG
		sprintf(szText,"File %s had still %u references.",
              (const m4char_t*)((*itControl).first),(unsigned) cRef);
		WRITE_LOGFILE(szText);
#endif

		for (cCont=0; cCont<cRef; cCont++)
		{
			UnSetInterestInFile((*itControl).first);
		}
		itControl=m_mapControl.begin();
	}

#ifdef _DEBUG
	// See if there is some unremoved file
	for (itGarbage=m_oPendingGarbage.begin(); itGarbage!=m_oPendingGarbage.end(); itGarbage++)
	{
		sprintf(szText,"File %s couldn't be removed.", (const m4char_t*)(*itGarbage));
		WRITE_LOGFILE(szText);
	}
#endif
}


ClFileManager* ClFileManager::Instance ()
{
	return &m_oInstance;
}


m4return_t ClFileManager::SetDefaultDirectory (const m4char_t* ai_pszDefaultTempDirectory)
{
	oMutexFileManager.Lock();
	m_oDefaultDir = ai_pszDefaultTempDirectory;
	oMutexFileManager.Unlock();
	return M4_SUCCESS;
}


m4return_t ClFileManager::SetInterest (const ClFileName& ai_oFileName, m4bool_t bErase)
{
	ControlPair_t pairControl;
	st_Control stControl;
	
	stControl.bErase=bErase;
	stControl.cRef=1;

	oMutexFileManager.Lock();

	pairControl = m_mapControl.insert(ControlMap_t::value_type(ai_oFileName, stControl));

	if (pairControl.second == M4_FALSE)
	{
		// Already existed
		(*(pairControl.first)).second.cRef++;
	}

//	DumpTrace( ai_oFileName, "+", (*(pairControl.first)).second.bErase, (*(pairControl.first)).second.cRef, m_mapControl.size() ) ;

	oMutexFileManager.Unlock();

	return M4_SUCCESS;
}


m4return_t ClFileManager::CreateTempFile (ClFileName& ao_oFileName, const m4char_t* ai_pszPrefix, const m4char_t* ai_pszExtension, const m4char_t* ai_pszTempDirectory, m4pint32_t ao_pError)
{
	m4char_t szTemp[M4_MAX_PATH + 1];
	m4pcchar_t pccTempDir;
	m4return_t res;
	
	oMutexFileManager.Lock();

	if (ai_pszTempDirectory)
	{
		pccTempDir = ai_pszTempDirectory;
	}
	else
	{
		if (m_oDefaultDir.IsNull())
		{
			pccTempDir=NULL;
		}
		else
		{
			pccTempDir=m_oDefaultDir.GetFileName();
		}
	}

	oMutexFileManager.Unlock();

	res=ClTemporalFileCreator::CreateTempFile(szTemp, ai_pszPrefix, ai_pszExtension, pccTempDir, ao_pError);

//	DumpTrace( szTemp, "$", M4_TRUE, 0, m_mapControl.size() ) ;

	if (res==M4_ERROR)
	{
		return res;
	}

	ao_oFileName.SetFileName(szTemp);
	return SetInterest(ao_oFileName,M4_TRUE);
}


m4return_t ClFileManager::UnSetInterestInFile (const ClFileName& ai_oFileName)
{
	itControlMap_t itControl;
	int iLength = -1;
	m4pchar_t pcANSIFileName = NULL;
	m4uint32_t cRef;

	if (ai_oFileName.IsNull())
	{
		return M4_SUCCESS;
	}

	oMutexFileManager.Lock();

	itControl=m_mapControl.find(ai_oFileName);
	if (itControl==m_mapControl.end())
	{
		// File name doesn't exist
		oMutexFileManager.Unlock();
		return M4_ERROR;
	}
	
	cRef = --(*itControl).second.cRef ;

//	DumpTrace( ai_oFileName, "-", (*itControl).second.bErase, cRef, m_mapControl.size() ) ;

	if( cRef == 0)
	{
		// Reference down to 0
		if ((*itControl).second.bErase==M4_TRUE)
		{
			// Want remove
			iLength = -1;
			pcANSIFileName = M4CppToANSI( ai_oFileName.GetFileName(), iLength ) ;
#ifdef _WINDOWS
			_chmod(pcANSIFileName, _S_IREAD | _S_IWRITE); // Read/write permissions
#endif
			if (remove(pcANSIFileName)!=0 && errno!=ENOENT)
			{
				DumpTrace( ai_oFileName.GetFileName(), strerror( errno ), errno, cRef, (long) m_mapControl.size() ) ;

				// If we can't remove it
				m_oPendingGarbage.push_back(ai_oFileName);
			}
			delete( pcANSIFileName ) ;
		}
		m_mapControl.erase(itControl);
	}

	oMutexFileManager.Unlock();
	return M4_SUCCESS;
}


m4return_t ClFileManager::CollectGarbage ()
{
	int iLength = -1;
	m4pchar_t pcANSIFileName = NULL;
	itClFileList itGarbage, itNextGarbage;

	oMutexFileManager.Lock();

	itNextGarbage=m_oPendingGarbage.begin();

	while (itNextGarbage!=m_oPendingGarbage.end())
	{
		itGarbage=itNextGarbage;
		itNextGarbage++;

		iLength = -1;
		pcANSIFileName = M4CppToANSI( (*itGarbage).GetFileName(), iLength ) ;

#ifdef _WINDOWS
		_chmod(pcANSIFileName, _S_IREAD | _S_IWRITE); // Read/write permissions
#endif
		if (remove(pcANSIFileName)==0 || errno==ENOENT)
		{
			DumpTrace( (*itGarbage).GetFileName(), strerror( errno ), errno, 0, (long) m_mapControl.size() ) ;
			// If we can remove it
			m_oPendingGarbage.erase(itGarbage);
		}
		delete( pcANSIFileName ) ;
	}

	if (m_oPendingGarbage.empty())
	{
		oMutexFileManager.Unlock();
		return M4_SUCCESS;
	} else
	{
		oMutexFileManager.Unlock();
		return M4_ERROR;
	}
}


m4return_t ClFileManager::GetPendingGarbageList (ClFileList& ao_oFileNames) const
{
	oMutexFileManager.Lock();
	ao_oFileNames=m_oPendingGarbage;
	oMutexFileManager.Unlock();
	return M4_SUCCESS;
}


const void* ClFileManager::GetUniqueId (const ClFileName& ai_oFileName)
{
	itControlMap_t itControl;
	void *pRes;

	if (ai_oFileName.IsNull())
	{
		return NULL;
	}

	oMutexFileManager.Lock();

	itControl=m_mapControl.find(ai_oFileName);
	if (itControl==m_mapControl.end())
	{
		// File name doesn't exist
		oMutexFileManager.Unlock();
		return NULL;
	}
	
	pRes= (void*)(const m4char_t*)(*itControl).first;

	oMutexFileManager.Unlock();
	return (const void*)pRes;
}


m4return_t ClFileManager::SetFileManagerOwnerOfTheFile (const ClFileName& ai_oFileName, m4bool_t ai_bFileManagerIsOwnerOfTheFile)
{
	itControlMap_t itControl;

	if (ai_oFileName.IsNull())
	{
		return M4_SUCCESS;
	}

	oMutexFileManager.Lock();

	itControl=m_mapControl.find(ai_oFileName);
	if (itControl==m_mapControl.end())
	{
		// File name doesn't exist
		oMutexFileManager.Unlock();
		return M4_ERROR;
	}
	
	//Actualize owner information
	(*itControl).second.bErase = ai_bFileManagerIsOwnerOfTheFile;

//	DumpTrace( ai_oFileName, "@", (*itControl).second.bErase, (*itControl).second.cRef, m_mapControl.size() ) ;

			
	oMutexFileManager.Unlock();
	return M4_SUCCESS;
}


m4bool_t ClFileManager::GetFileManagerOwnerOfTheFile (const ClFileName& ai_oFileName)
{
	itControlMap_t itControl;

	if (ai_oFileName.IsNull())
	{
		return M4_FALSE;
	}

	oMutexFileManager.Lock();
	itControl=m_mapControl.find(ai_oFileName);
	if (itControl==m_mapControl.end())
	{
		// File name doesn't exist
		oMutexFileManager.Unlock();
		return M4_FALSE;
	}

	oMutexFileManager.Unlock();
	return (*itControl).second.bErase;
}


m4return_t ClFileManager::CreatePath (const m4char_t* ai_pszPathToCreate)
{
	return ClFileDirOperations::CreatePath(ai_pszPathToCreate);
}


m4return_t ClFileManager::GetTempDir(m4char_t* ao_pszTempDir)
{
	oMutexFileManager.Lock();

	if (m_oDefaultDir.IsNull())
	{
		ao_pszTempDir=NULL;
	}
	else 
	{
		m_oDefaultDir.GetFileName(ao_pszTempDir);
	}

	oMutexFileManager.Unlock();

	return M4_SUCCESS;
}



// Class ClTemporalFileCreator 

m4uint16_t ClTemporalFileCreator::m_usVarPart = 0;
m4uint32_t ClTemporalFileCreator::m_cCont = 0;


m4return_t ClTemporalFileCreator::CreateTempFile (m4pchar_t ao_pszNewFileName, const m4char_t* ai_pszPrefix, const m4char_t* ai_pszExtension, const m4char_t* ai_pszDirectory, m4pint32_t ao_pError)
{
	FILE *		fic;
	int			iLength = -1;
	m4pchar_t	pcANSIFileName = NULL;
	m4pchar_t	pcCppFileName = NULL;
	m4pchar_t	pszBase;
	m4char_t	szFileName[M4_MAX_PATH + 1];
	m4char_t	acANSINewFileName[M4_MAX_PATH + 1];
	time_t		timeVar;
	m4uint32_t	cRepe=0;
	size_t		uLen;

	m4bool_t bRootDir = M4_FALSE;

	// If no directory, create the file in the working directory.
	if (ai_pszDirectory==NULL || ai_pszDirectory[0] == '\0')
	{ 
		szFileName[0]='\0';
	}
	else
	{
		strcpy(szFileName,ai_pszDirectory);
		uLen = strlen(szFileName);

#ifdef _WINDOWS	// Win32
		// If it is like 'c:\'.
		if (uLen==3 && szFileName[1]==':' && szFileName[2]=='\\')
		{
			bRootDir = M4_TRUE;
		}
#else				// Unix
		// If it is like '/'.
		if (uLen==1 && szFileName[0]=='/')
		{
			bRootDir = M4_TRUE;
		}

#endif

		// Remove the last backslash if it is not a root directory.
		if (szFileName[uLen-1] == M4_FM_BACK_SLASH && bRootDir == M4_FALSE)
		{
			szFileName[uLen-1]='\0'; // Remove last backslash
			uLen--;
		}

		if (ClFileState(szFileName).IsDir() == M4_FALSE)
		{
			if (ao_pError)
			{
				*ao_pError = M4_TEMPFILE_INVALID_DIR;
			}
			return M4_ERROR;
		}

		// Add backslash if not a root directory
		if (bRootDir == M4_FALSE)
		{
			szFileName[uLen+1]='\0';
			szFileName[uLen]=M4_FM_BACK_SLASH;
		}
	}

	if (ai_pszPrefix)
	{
		strcat(szFileName, ai_pszPrefix);
	}

	pszBase=szFileName+strlen(szFileName);
	
	oMutexTemporalFileCreator.Lock();
	
	if (m_usVarPart==0)
	{
		time(&timeVar);
		m_usVarPart=(m4uint16_t)timeVar;
	}

	while(M4_TRUE)
	{
		if(cRepe++==5)
		{	// If many faults, remake the seed.
			cRepe=0;
			time(&timeVar);
			m_usVarPart=(m4uint16_t)timeVar;
		}

		m_cCont++;
		if (ai_pszExtension)
		{
			sprintf(pszBase,"%um%u.%s", (unsigned)m_usVarPart,(unsigned)m_cCont,ai_pszExtension);
		}
		else
		{
			sprintf(pszBase,"%um%u", (unsigned)m_usVarPart,(unsigned)m_cCont);
		}

		iLength = -1 ;
		pcANSIFileName = M4CppToANSI( szFileName, iLength ) ;

		if( ( fic = fopen(pcANSIFileName, "r") ) == NULL )
		{
			if( ( fic = fopen(pcANSIFileName, "w" ) ) == NULL )
			{
				// Create empty file
				oMutexTemporalFileCreator.Unlock();

				if (ao_pError)
				{
					if (errno == EMFILE)
					{
						*ao_pError = M4_TEMPFILE_NO_RESOURCES;
					}
					else
					{
						*ao_pError = M4_TEMPFILE_NO_PERMISSION;
					}
				}
				delete( pcANSIFileName ) ;
				return M4_ERROR;
			}
			fclose(fic);

#ifdef _WIN32
			_fullpath(acANSINewFileName, pcANSIFileName, M4_MAX_PATH);
#else
			realpath(pcANSIFileName, acANSINewFileName);
#endif
			iLength = -1 ;
			pcCppFileName = M4ANSIToCpp( acANSINewFileName, iLength ) ;
			strcpy( ao_pszNewFileName, pcCppFileName ) ;
			delete( pcCppFileName ) ;

			oMutexTemporalFileCreator.Unlock();
			if (ao_pError)
			{
				*ao_pError = M4_TEMPFILE_SUCCESS;
			}
			delete( pcANSIFileName ) ;
			return M4_SUCCESS;
		}
		else
		{
			// Bad temp file
			fclose(fic);
		}
		
		delete( pcANSIFileName ) ;
	}

	M4_ASSERT(0);
	oMutexTemporalFileCreator.Unlock();
	if (ao_pError)
	{
		*ao_pError = M4_TEMPFILE_NO_RESOURCES;
	}
	return M4_ERROR;
}



  // Class ClFileDirOperations 


m4return_t ClFileDirOperations::CreatePath (const m4char_t* ai_pszPathToCreate)
{

	m4char_t	sAuxString[M4_MAX_PATH + 1];
	m4char_t	sDirString[M4_MAX_PATH + 1];
	size_t		iLen=0;
	size_t		iPos=0;
	m4char_t *	pcPos;
	m4uint16_t	iMoveCounter =0;


	//If it is null  nothing to do
	if (ai_pszPathToCreate == NULL || *ai_pszPathToCreate == '\0')
	{
		return M4_SUCCESS;
	}

	strcpy(sAuxString,ai_pszPathToCreate);

	//Pongo los slash de forma correcta en función de si es Unix o Win32
	_ActualizePathSlash(sAuxString);
 
	// Bug 0288628 Si no acaba en slash se añade para que se construya el último directorio
	iLen = strlen(sAuxString);
	if(iLen > 0 && sAuxString[iLen - 1] != M4_FM_BACK_SLASH)
	{
		sAuxString[iLen] = M4_FM_BACK_SLASH;
		sAuxString[iLen + 1] = '\0';
	}

	do {
		pcPos = strchr(&sAuxString[iPos], M4_FM_BACK_SLASH);
      
		if(pcPos)
		{
			iPos = pcPos - sAuxString + 1;
			strncpy(sDirString,sAuxString,iPos);
			sDirString[iPos] = '\0';

			//Inicio cambio: Para que pueda reconocer en windows paths UNC
			//19 Abril 2002
#ifdef _WINDOWS
			char string[] = "\\\\";
			int  result;
			result = strcmp( string, sDirString );
			
			if (result == 0)
			{
				pcPos = strchr(&sAuxString[iPos], M4_FM_BACK_SLASH);
				if(pcPos)
				{
					iPos = pcPos - sAuxString + 1;
					strncpy(sDirString,sAuxString,iPos);
					sDirString[iPos] = '\0';
					pcPos = strchr(&sAuxString[iPos], M4_FM_BACK_SLASH);

					if(pcPos)
					{
						iPos = pcPos - sAuxString + 1;
						strncpy(sDirString,sAuxString,iPos);
						sDirString[iPos] = '\0';
					}
				}
			}				
#endif
			//Fin cambio 19 Abril 2002

			if(M4MakeDirectory(sDirString) == -1)
			{
				return M4_ERROR;
			}
		}
	} while (pcPos);

	return M4_SUCCESS;
}
   


m4void_t ClFileDirOperations::_ActualizePathSlash (m4char_t* aio_pszPath)
{
	//Actualizo los slash en función del sistema operativo

   	m4char_t *	pcPos;
	size_t		 iPos=0, iOldPos=0;
	
    //si no es nulo
	if (aio_pszPath)
	{
		do
		{
#ifdef _UNIX
			pcPos = strchr(&aio_pszPath[iOldPos], M4_FM_WIN_SLASH);
#else
			pcPos = strchr(&aio_pszPath[iOldPos], M4_FM_UNIX_SLASH);
#endif
			if(pcPos)
			{
				iPos=pcPos - &aio_pszPath[iOldPos]+1;
				pcPos[0] = M4_FM_BACK_SLASH;
				iOldPos+=iPos;
			};
		}
		while (pcPos);
	}
}


m4return_t ClFileDirOperations::RemoveDirectory (const m4char_t *ai_pszDirectory, const m4bool_t ai_bRecursive)
{	
	// Compone el nombre del path mas la condición de búsqueda de directorios.
	m4string_t sPathName = m4string_t(ai_pszDirectory) + M4_FM_BACK_SLASH + "*";

	ClFileFinder oFileFinder( sPathName.c_str() );

	ClFile oFile;
	while( oFileFinder.FindNext( oFile ) != M4_ERROR )
	{
		// Comprueba si se trata de un fichero o un subdirectorio.
		if( oFile.IsDir() == M4_TRUE )
		{
			if( ai_bRecursive == M4_TRUE )
			{
				// Es un directorio.
				if( RemoveDirectory( oFile.GetFileName(), M4_TRUE ) != M4_SUCCESS )
				{
					return M4_ERROR;
				}
			}
		}
		else
		{
			// Es un archivo.
			if( M4RemoveFile( oFile.GetFileName() ) == -1 )
			{
				return M4_ERROR;
			}
		}
	}

	oFileFinder.End() ;

	// Borra el directorio si existe y está vacio.
	if( M4RemoveDirectory( ai_pszDirectory ) == -1 )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
