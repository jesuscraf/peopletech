//## begin module%3F5313D80137.cm preserve=no
//## end module%3F5313D80137.cm

//## begin module%3F5313D80137.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%3F5313D80137.cp

//## Module: clrollingfile%3F5313D80137; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: C:\m4server\m4serverbase\src\rollingfile.cpp

//## begin module%3F5313D80137.additionalIncludes preserve=no
//## end module%3F5313D80137.additionalIncludes

//## begin module%3F5313D80137.includes preserve=yes
#include <stdio.h>
#include <errno.h>

// rollingfile
#include <rollingfile.hpp>
// ClFileState
#include <file_misc.hpp>
// m4thread
#include <m4thread.hpp>
//Regedit
#include <regedit.hpp>
//M4clock
#include <m4clock.hpp>
//M4Date
#include <m4date.hpp>

#include <m4log.hpp>
#include <m4srvres.hpp>
#include <blocksynchronization.hpp>
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "m4regdef.hpp"

//## end module%3F5313D80137.includes

//## begin module%3F5313D80137.declarations preserve=no
//## end module%3F5313D80137.declarations

//## begin module%3F5313D80137.additionalDeclarations preserve=yes
#define M4_DEFAULT_FILE_EXTENSION		".log"

// Minimum file number.
#define M4_MIN_FILE_NUM				1
// Minimum file size allowed.
#define M4_MIN_FILE_SIZE			2 * 1024 // 2 KB
// Maximum file number allowed
#define M4_MAX_FILE_NUM				25
// Maximum file size allowed.
#define M4_MAX_FILE_SIZE			100 * 1024 * 1024 // 100 MB

#define M4_REGEDIT_MAX_PATH				255
#define M4_REGEDIT_CVM_LOG_KEY			"CVM\\LOG"
#define M4_REGEDIT_MAX_FILE_NUM_KEY		"MaxRollingFileNum"
#define M4_REGEDIT_MAX_FILE_SIZE_KEY	"MaxRollingFileSize"
//## end module%3F5313D80137.additionalDeclarations


// Class ClRollingFile 








ClRollingFile::ClRollingFile ()
  //## begin ClRollingFile::ClRollingFile%1062412773.hasinit preserve=no
  //## end ClRollingFile::ClRollingFile%1062412773.hasinit
  //## begin ClRollingFile::ClRollingFile%1062412773.initialization preserve=yes
  //## end ClRollingFile::ClRollingFile%1062412773.initialization
{
  //## begin ClRollingFile::ClRollingFile%1062412773.body preserve=yes

	m_iMaxFileNum = M4_MIN_FILE_NUM ;
	m_iMaxFileSize = M4_MIN_FILE_SIZE ;
	m_sFilePath = "" ;
	m_sHeader = "" ;
	m_oMutex.Init() ;
	m_bTickCount = M4_FALSE ;
	m_bThreadId = M4_FALSE ;
	m_pfFile = NULL ;

  //## end ClRollingFile::ClRollingFile%1062412773.body
}


ClRollingFile::~ClRollingFile ()
{
  //## begin ClRollingFile::~ClRollingFile%1062412775.body preserve=yes

	if( m_pfFile != NULL )
	{
		fclose( m_pfFile ) ;
		m_pfFile = NULL ;
	}

  //## end ClRollingFile::~ClRollingFile%1062412775.body
}



//## Other Operations (implementation)
m4return_t ClRollingFile::Init (const m4string_t &ai_sFilePath, m4uint32_t ai_iMaxFileNum, m4uint32_t ai_iMaxFileSize)
{
  //## begin ClRollingFile::Init%1128499375.body preserve=yes
	
	m4return_t	iResult = M4_SUCCESS ;
	m4char_t	acPath [ M4_REGEDIT_MAX_PATH + 1] ;
	m4char_t	acValue[ M4_REGEDIT_MAX_PATH + 1 ] ;
	m4uint32_t	iSize = 0 ;


	m_iMaxFileNum = ai_iMaxFileNum ;
	m_iMaxFileSize = ai_iMaxFileSize ;

	if( m_iMaxFileNum == m4uint32_t( -1 ) || m_iMaxFileSize == m4uint32_t( -1 ) )
	{
		// Compose LOG registry path.
		M4ComposeRegistryPath( M4_NEW_REGEDIT_KEY_SERVER, M4_REGEDIT_CVM_LOG_KEY, acPath, M4_REGEDIT_MAX_PATH, iSize ) ;
	}
		
	if( m_iMaxFileNum == m4uint32_t( -1 ) )
	{
		if( GetRegKeyData( acPath, M4_REGEDIT_MAX_FILE_NUM_KEY, acValue ) == TRUE )
		{
			m_iMaxFileNum = atoi( acValue ) ;
		}
	}

	if( m_iMaxFileSize == m4uint32_t( -1 ) )
	{
		if( GetRegKeyData( acPath, M4_REGEDIT_MAX_FILE_SIZE_KEY, acValue ) == TRUE )
		{
			m_iMaxFileSize = atoi( acValue ) * 1024 ;
		}
	}

	// Check limits for file size and file number.
	if( m_iMaxFileNum < M4_MIN_FILE_NUM )
	{
		SETCODEF( M4_ERR_ROLLING_FILE_MAX_FILE_NUM_WARN, WARNINGLOG, "#*S1*#%d#%d#%d#", M4_MIN_FILE_NUM, M4_MAX_FILE_NUM, M4_MIN_FILE_NUM ) ;
		m_iMaxFileNum = M4_MIN_FILE_NUM ;
	}
	else if( m_iMaxFileNum > M4_MAX_FILE_NUM )
	{
		SETCODEF( M4_ERR_ROLLING_FILE_MAX_FILE_NUM_WARN, WARNINGLOG, "#*S1*#%d#%d#%d#", M4_MIN_FILE_NUM, M4_MAX_FILE_NUM, M4_MAX_FILE_NUM ) ;
		m_iMaxFileNum = M4_MAX_FILE_NUM ;
	}

	if( m_iMaxFileSize < M4_MIN_FILE_SIZE )
	{
		SETCODEF( M4_ERR_ROLLING_FILE_MAX_FILE_SIZE_WARN, WARNINGLOG, "#*S1*#%d#%d#%d#", M4_MIN_FILE_SIZE, M4_MAX_FILE_SIZE, M4_MIN_FILE_SIZE ) ;
		m_iMaxFileSize = M4_MIN_FILE_SIZE ;
	}
	else if( m_iMaxFileSize > M4_MAX_FILE_SIZE )
	{
		SETCODEF( M4_ERR_ROLLING_FILE_MAX_FILE_SIZE_WARN, WARNINGLOG, "#*S1*#%d#%d#%d#", M4_MIN_FILE_SIZE, M4_MAX_FILE_SIZE, M4_MAX_FILE_SIZE ) ;
		m_iMaxFileSize = M4_MAX_FILE_SIZE ;
	}


	iResult = SetFile( ai_sFilePath ) ;

	return( iResult ) ;
  //## end ClRollingFile::Init%1128499375.body
}

m4return_t ClRollingFile::SetFile (const m4string_t &ai_sFilePath)
{
  //## begin ClRollingFile::SetFile%1062412778.body preserve=yes
	// Set file name.
	m4string_t sFileNameWithoutExtension, sFileExtension;

	// Check file path is not empty.
	if (ai_sFilePath.empty() == M4_TRUE)
	{
		SETCODEF(M4_ERR_ROLLING_FILE_NO_VALID_FILE_PATH, ERRORLOG, "#*S1*#%s#", ai_sFilePath.c_str());
		return M4_ERROR;
	}
	else
	{
		m_sFilePath = ai_sFilePath;
	}

	// Get file extension and the rest of the file path.
	SplitFileName(sFileNameWithoutExtension, sFileExtension);
		
	// Get current date.
	M4ClTimeStamp tsDate;
	m4char_t pBuffer[24];
	m4int32_t iYear, iMonth, iDay, iHour, iMinutes, iSeconds;
	tsDate.now();
	tsDate.get(iYear, iMonth, iDay, iHour, iMinutes, iSeconds);
	sprintf(pBuffer, "_%d-%02d-%02d@%02d%02d%02d", iYear, iMonth, iDay, iHour, iMinutes, iSeconds);
	
	m4string_t sBackupFilePath, sDateNow;
	sDateNow = pBuffer;

	// Check file path and backup current log file, if exists.
	ClFileState oFileState(m_sFilePath.c_str());
	if (oFileState.IsFile() == M4_TRUE)
	{
		// Rename file adding the current system date to the 
		// file name.
		sBackupFilePath = sFileNameWithoutExtension + sDateNow + sFileExtension;
		ClMutBlock oBlock(&m_oMutex);

		/*
		Bug 0121541
		Se controla el retorno contra 0 y no M4_SUCCESS
		Se cambia el mensaje de error para dar más información
		*/
		if (M4RenameFile(m_sFilePath.c_str(), sBackupFilePath.c_str()) != 0)
		{
			SETCODEF(M4_ERR_ROLLING_FILE_RENAME_ERROR, ERRORLOG, "#*S1*#%s#%s#%s#", m_sFilePath.c_str(), sBackupFilePath.c_str(), strerror(errno));
		}
	}

	// We also need to check if there are other files that matches 
	// fileName + fileIndex + .fileExtension, where fileIndex is, as
	// maximum, M4_MAX_FILE_NUM.
	m4int32_t i = 0;
	m4char_t pIndex[8];
	m4string_t sIndex, sFilePath;
	sprintf(pIndex, "%d", i);
	sIndex = pIndex;

	// Build file name.
	sFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
	oFileState.SetFileName(sFilePath.c_str());
	while (i < M4_MAX_FILE_NUM)
	{
		if (oFileState.IsFile() == M4_TRUE)
		{
			sBackupFilePath = sFileNameWithoutExtension + sIndex + sDateNow + sFileExtension;
			ClMutBlock oBlock(&m_oMutex);

			/*
			Bug 0121541
			Se controla el retorno contra 0 y no M4_SUCCESS
			Se cambia el mensaje de error para dar más información
			*/
			if (M4RenameFile(sFilePath.c_str(), sBackupFilePath.c_str()) != 0)
			{
				SETCODEF(M4_ERR_ROLLING_FILE_RENAME_ERROR, ERRORLOG, "#*S1*#%s#%s#%s#", sFilePath.c_str(), sBackupFilePath.c_str(), strerror(errno));
			}
		}

		i++;
		sprintf(pIndex, "%d", i);
		sIndex = pIndex;
		
		sFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
		oFileState.SetFileName(sFilePath.c_str());
	}
	return M4_SUCCESS;
  //## end ClRollingFile::SetFile%1062412778.body
}

m4return_t ClRollingFile::SetHeader (const m4string_t &ai_sHeader)
{
  //## begin ClRollingFile::SetHeader%1062412779.body preserve=yes
	m_sHeader = ai_sHeader;
	
	// Print header.
	if (m_sHeader.empty() == M4_FALSE)
		return PrintHeader();

	return M4_SUCCESS;
  //## end ClRollingFile::SetHeader%1062412779.body
}

void ClRollingFile::SetMaxFileNum (m4uint32_t ai_iMaxFileNum)
{
  //## begin ClRollingFile::SetMaxFileNum%1062412780.body preserve=yes
	m_iMaxFileNum = ai_iMaxFileNum;
  //## end ClRollingFile::SetMaxFileNum%1062412780.body
}

void ClRollingFile::SetMaxFileSize (m4uint32_t ai_iMaxFileSize)
{
  //## begin ClRollingFile::SetMaxFileSize%1062412781.body preserve=yes
	m_iMaxFileSize = ai_iMaxFileSize;
  //## end ClRollingFile::SetMaxFileSize%1062412781.body
}

m4return_t ClRollingFile::SetFilePath (const m4string_t &ai_sFileName)
{
  //## begin ClRollingFile::SetFilePath%1063623438.body preserve=yes
	return SetFile(ai_sFileName);
  //## end ClRollingFile::SetFilePath%1063623438.body
}

m4uint32_t ClRollingFile::GetMaxFileNum ()
{
  //## begin ClRollingFile::GetMaxFileNum%1063354420.body preserve=yes
	return m_iMaxFileNum;
  //## end ClRollingFile::GetMaxFileNum%1063354420.body
}

m4uint32_t ClRollingFile::GetMaxFileSize ()
{
  //## begin ClRollingFile::GetMaxFileSize%1063354421.body preserve=yes
	return m_iMaxFileSize;
  //## end ClRollingFile::GetMaxFileSize%1063354421.body
}

m4string_t ClRollingFile::GetFilePath ()
{
  //## begin ClRollingFile::GetFilePath%1063623439.body preserve=yes
	return m_sFilePath;
  //## end ClRollingFile::GetFilePath%1063623439.body
}

m4return_t ClRollingFile::PrintHeader ()
{
  //## begin ClRollingFile::PrintHeader%1062412782.body preserve=yes
	if (m_sFilePath.empty() == M4_TRUE)
	{
		SETCODEF(M4_ERR_ROLLING_FILE_NO_VALID_FILE_PATH, ERRORLOG, "#*S1*#%s#", m_sFilePath.c_str());
		return M4_ERROR;
	}

	// Prepares to append to file.

	// fix bug 0095066
	ClMutBlock oBlock(&m_oMutex);

	if( m_pfFile == NULL )
	{
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		m_pfFile = M4Fopen( m_sFilePath.c_str(), M4UniAppend, eEncoding ) ;

		if( m_pfFile == NULL )
		{
			SETCODEF(M4_ERR_ROLLING_FILE_CANNOT_OPEN_FILE, ERRORLOG, "#*S1*#%s#", m_sFilePath.c_str());
			return M4_ERROR;
		}
	}

	// Writes generic information
	if( m_pfFile != NULL )
	{
		fprintf( m_pfFile, "%s\n", m_sHeader.c_str() ) ;
		fflush( m_pfFile ) ;
	}
    
	return M4_SUCCESS;
  //## end ClRollingFile::PrintHeader%1062412782.body
}

m4return_t ClRollingFile::PrintLn (const m4string_t &ai_sMessage)
{
  //## begin ClRollingFile::PrintLn%1062412783.body preserve=yes
	m4bool_t bDoBackUp = M4_TRUE;
	m4bool_t bEndOfLine = M4_TRUE;
	m4return_t iRet = PrintInfo(ai_sMessage, bDoBackUp, bEndOfLine);
	return iRet;
  //## end ClRollingFile::PrintLn%1062412783.body
}

m4return_t ClRollingFile::PrintStr (const m4string_t &ai_sStr)
{
  //## begin ClRollingFile::PrintStr%1128499373.body preserve=yes
	m4bool_t bDoBackUp = M4_FALSE;
	m4bool_t bEndOfLine = M4_FALSE;
	m4return_t iRet = PrintInfo(ai_sStr, bDoBackUp, bEndOfLine);
	return iRet;
  //## end ClRollingFile::PrintStr%1128499373.body
}

m4return_t ClRollingFile::PrintInfo (const m4string_t &ai_sInfo, m4bool_t ai_bDoBackUp, m4bool_t ai_bEndOfLine)
{
  //## begin ClRollingFile::PrintInfo%1128499374.body preserve=yes
	if (m_sFilePath.empty() == M4_TRUE)
	{
		SETCODEF(M4_ERR_ROLLING_FILE_NO_VALID_FILE_PATH, ERRORLOG, "#*S1*#%s#", m_sFilePath.c_str());
		return M4_ERROR;
	}

	// Check file size. If the file has exceeded maximum file
    // length, do a backup file.
	// If max num files is 1, there will not be backup files. So, we
    // do not need to do backup.
	if (ai_bDoBackUp == M4_TRUE)
	{
		m4int32_t iFileSize = GetFileSize();
		if (m_iMaxFileNum > 1 && iFileSize >= m_iMaxFileSize)
		{
			if (DoBackup() != M4_SUCCESS)
			{
				SETCODEF(M4_ERR_ROLLING_FILE_BACKUP_ERROR, ERRORLOG, "#*S1*#");
				return M4_ERROR;
			}
		}
	}

	// Check if tick count and thread id has been enabled. If they are 
	// enable, we will print tick count information and thread id before
	// the message.
	m4char_t pBuffer[256];
	string sMessage = "";
	if (m_bThreadId == M4_TRUE)
	{
        sprintf(pBuffer, "%u\t", M4Thread::GetCurrentIdThread());
		string sThreadId = pBuffer;
		sMessage.append(sThreadId);
	}

	if (m_bTickCount == M4_TRUE)
	{
        // bugid: 102707 & 103020
		sprintf(pBuffer, "%lu\t", (unsigned long)ClPrecisionTicker::GetTimeTick());
		string sTickCount = pBuffer;
		sMessage.append(sTickCount);
	}

	// Writes information.
	sMessage.append(ai_sInfo);
	if (ai_bEndOfLine == M4_TRUE)
	{
		sMessage.append("\n");
	}
	
	// Prepares to append to file.
	// fix bug 0095066
	ClMutBlock oBlock(&m_oMutex);

	if( m_pfFile == NULL )
	{
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		m_pfFile = M4Fopen( m_sFilePath.c_str(), M4UniAppend, eEncoding ) ;

		if( m_pfFile == NULL )
		{
			SETCODEF(M4_ERR_ROLLING_FILE_CANNOT_OPEN_FILE, ERRORLOG, "#*S1*#%s#", m_sFilePath.c_str());
			return M4_ERROR;
		}
	}

	if( m_pfFile != NULL )
	{
		fprintf( m_pfFile, "%s", sMessage.c_str() ) ;
		fflush( m_pfFile ) ;
	}
    
	return M4_SUCCESS;
  //## end ClRollingFile::PrintInfo%1128499374.body
}

m4return_t ClRollingFile::DoBackup ()
{
  //## begin ClRollingFile::DoBackup%1062412784.body preserve=yes
	// Get file extension and full file path without extension.
	m4string_t sFileNameWithoutExtension, sFileExtension;
	SplitFileName(sFileNameWithoutExtension, sFileExtension);

	// We will begin from the maximum number of files until the
    // current file (without index).
	// Backup index files start in 0 and we do not bear in mind
	// current file (the only one without index).
	m4uint32_t iMaxFileIndex = m_iMaxFileNum - 2;
	m4char_t pIndex[8];
	m4string_t sIndex, sFilePath;
	sprintf(pIndex, "%d", iMaxFileIndex);
	sIndex = pIndex;
	
	// Build backup file name.
	sFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
	
	// Check if this file exists.
	ClFileState oFileState(sFilePath.c_str());
	if (oFileState.IsFile() == M4_TRUE)
	{
		// We have reached the maximum number of files.
		// Delete this file.
		if (M4RemoveFile(sFilePath.c_str()) != 0)
		{
			SETCODEF(M4_ERR_ROLLING_FILE_DELETE_ERROR, ERRORLOG, "#*S1*#%s#", sFilePath.c_str());
		}
	}	
	
	// Check the rest of backup files.
	m4int32_t iIndex = iMaxFileIndex - 1;
	while (iIndex >= 0)
	{
		sprintf(pIndex, "%d", iIndex);
		sIndex = pIndex;
		
		// Build backup file name.
		sFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
		
		// Check if file exists.
		oFileState.SetFileName(sFilePath.c_str());
		if (oFileState.IsFile() == M4_TRUE)
		{
			// Move file.
			m4int32_t iIndexAux = iIndex + 1;
			sprintf(pIndex, "%d", iIndexAux);
			sIndex = pIndex;
			m4string_t sBackupFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
			
			ClMutBlock oBlock(&m_oMutex);

			/*
			Bug 0121541
			Se controla el retorno contra 0 y no M4_SUCCESS
			Se cambia el mensaje de error para dar más información
			*/
			if (M4RenameFile(sFilePath.c_str(), sBackupFilePath.c_str()) != 0)
			{
				SETCODEF(M4_ERR_ROLLING_FILE_RENAME_ERROR, ERRORLOG, "#*S1*#%s#%s#%s#", sFilePath.c_str(), sBackupFilePath.c_str(), strerror(errno));
			}
		}
		iIndex--;
	}
	
	// Backup current file (the one without index).
	oFileState.SetFileName(m_sFilePath.c_str());
	if (oFileState.IsFile() == M4_TRUE)
	{
		// Move file.
		sprintf(pIndex, "%d", iIndex + 1);
		sIndex = pIndex;
		m4string_t sBackupFilePath = sFileNameWithoutExtension + sIndex + sFileExtension;
		
		ClMutBlock oBlock(&m_oMutex);

		/*
		Bug 0121541
		Se controla el retorno contra 0 y no M4_SUCCESS
		Se cambia el mensaje de error para dar más información
		Además se cierra el fichero para que no esté pillado
		*/
		if( m_pfFile != NULL )
		{
			fclose( m_pfFile ) ;
			m_pfFile = NULL ;
		}

		if (M4RenameFile(m_sFilePath.c_str(), sBackupFilePath.c_str()) != 0)
		{
			SETCODEF(M4_ERR_ROLLING_FILE_RENAME_ERROR, ERRORLOG, "#*S1*#%s#%s#%s#", m_sFilePath.c_str(), sBackupFilePath.c_str(), strerror(errno));
		}
	}
	
	// Print header into new file.
	if (PrintHeader() != M4_SUCCESS)
		return M4_ERROR;
    
	return M4_SUCCESS;
  //## end ClRollingFile::DoBackup%1062412784.body
}

m4uint32_t ClRollingFile::GetFileSize ()
{
  //## begin ClRollingFile::GetFileSize%1062412785.body preserve=yes
	return ClFileState(m_sFilePath.c_str()).GetSize();
  //## end ClRollingFile::GetFileSize%1062412785.body
}

void ClRollingFile::SplitFileName (m4string_t &aio_FilePathWithoutExtension, m4string_t &aio_FileExtension)
{
  //## begin ClRollingFile::SplitFileName%1062412786.body preserve=yes
	// Set file name.
	m4pcchar_t pFilePath = m_sFilePath.c_str();
	m4int32_t iPos = 0;
	m4pchar_t pRest = NULL, pFileExtension = NULL;

	// search backward for character '.' (extension separator)
	m4pcchar_t pDest = strrchr(pFilePath, '.');

	// if pDest has an '\' on NT or an '/' on UNIX, the dot found was on a directory name.
	// In this case, the filename has not extension.
	if (pDest != NULL)
	{
#ifdef _WINDOWS
		char cFileSeparator = '\\';
#else
		char cFileSeparator = '/';
#endif
		if (strchr(pDest, cFileSeparator) != NULL)
		{
			pDest = NULL;
		}
	}

	if (pDest == NULL)
	{
		// File extension not found. Add default file extension.
		pRest = new m4char_t[strlen(pFilePath) + 1];
		pFileExtension = new m4char_t[8];
		strcpy(pFileExtension, M4_DEFAULT_FILE_EXTENSION);
		strcpy(pRest, pFilePath);

		m_sFilePath.append(M4_DEFAULT_FILE_EXTENSION);
	}
	else
	{
		iPos = pDest - pFilePath;
		pRest = new m4char_t[iPos + 1];
		pFileExtension = new m4char_t[strlen(pFilePath) - iPos + 1];	

		strncpy(pRest, pFilePath, iPos);
		pRest[iPos] = '\0';
		
		strncpy(pFileExtension, pFilePath + iPos, strlen(pFilePath) - iPos);
		pFileExtension[strlen(pFilePath) - iPos] = '\0';
	}

	aio_FilePathWithoutExtension = pRest;
	aio_FileExtension = pFileExtension;
	
	if (pFileExtension != NULL)
		delete pFileExtension;
	
	if (pRest != NULL)
		delete pRest;
  //## end ClRollingFile::SplitFileName%1062412786.body
}

void ClRollingFile::EnableTickCount ()
{
  //## begin ClRollingFile::EnableTickCount%1062412787.body preserve=yes
	m_bTickCount = M4_TRUE;
  //## end ClRollingFile::EnableTickCount%1062412787.body
}

void ClRollingFile::EnableThreadId ()
{
  //## begin ClRollingFile::EnableThreadId%1062412788.body preserve=yes
	m_bThreadId = M4_TRUE;
  //## end ClRollingFile::EnableThreadId%1062412788.body
}


FILE* ClRollingFile::GetFile( void ) const
{
	return( m_pfFile ) ;
}


// Additional Declarations
  //## begin ClRollingFile%3F53183A039D.declarations preserve=yes
  //## end ClRollingFile%3F53183A039D.declarations

//## begin module%3F5313D80137.epilog preserve=yes
//## end module%3F5313D80137.epilog
