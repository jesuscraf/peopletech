//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           dllmain.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           4 feb 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    M4LogSys entry point and error messages functions.
////
//==============================================================================

#ifdef _WINDOWS
	#include <windows.h>
#endif

#include "m4var.hpp"		//M4Variant
#include "regedit.hpp"		//M4Ini
#include "syncro.hpp"		//M4Ipcs
#include "m4objreg.hpp"		//M4ObjReg


#include "filemem.hpp"
#include "m4logsys.hpp"
#include "m4lang.hpp"


#include "m4types.hpp" 
#include "m4stl.hpp" 
#include "m4unicode.hpp" 



// backslash
#ifdef _UNIX
#define M4_LOGSYS_BACK_SLASH	'/'
#else
#define M4_LOGSYS_BACK_SLASH	'\\'
#endif


typedef struct
{
	m4pchar_t pszBegin;
	m4uint32_t uLen;
	int iIsUtf8;
} st_LogsysError;

typedef map<m4uint32_t,st_LogsysError, less<m4uint32_t> > LogsysErrorMap_t;
typedef LogsysErrorMap_t::iterator LogsysErrorMapIterator_t;

typedef set<m4uint32_t, less<m4uint32_t> > LogsysErrorSet_t;
typedef LogsysErrorSet_t::iterator LogsysErrorSetIterator_t;

typedef map<m4language_t, m4uint32_t, less<m4language_t> > LogsysLangId_vs_Index_Map_t ;
typedef LogsysLangId_vs_Index_Map_t ::iterator LogsysLangId_vs_Index_MapIterator_t;


// Class for Entry and Exit point for initializations and termination
//Antiguo: class ClM4LogSysEntryPoint {
class ClM4LogSysLang
{
public:
	ClM4LogSysLang();
	~ClM4LogSysLang();

	void Init(m4language_t ai_uiLangId, m4uint32_t ai_uiLangIndex);	// Initialize object

	// Is initialized
	m4bool_t IsInitialized()
	{
		return m_bInitialized;
	}

	void  InitializeFileMem(m4pchar_t ai_pszWorkDir); // Initialize mapped file in memory
	void  CalcMaxMinError();				// Calc Min and Max error codes

	//ClMutex		m_MutexFile;				// Mutex to access mapped file - critical section in object
	ClFileInMem		m_FileInMem;				// Errors mapped file in memory for the language
	m4uint32_t		m_uMinError, m_uMaxError;	// Min and Max error

	m4language_t	m_uLangId;				// Language Id
	m4uint32_t		m_uLangIndex;			// Language Index

	LogsysErrorMap_t m_mapHits;				// Map with loaded errors
	LogsysErrorSet_t m_setFaults;			// Set with errors demanded but that doesn't exist

	m4bool_t		m_bInitialized;			// Initialized object
};


// Clase que contiene toda la operatoria de la DLL - hay una instancia static
class ClM4LogSysEntryPoint
{
public:

	ClM4LogSysEntryPoint();
	~ClM4LogSysEntryPoint();

	void Init();					// Initialize object

	ClMutex						m_Mutex;		// Mutex for critical sections - Used by Global functions
	M4ClLangInfo*				m_pLang;		// Object to handle info about all Languages

	ClM4LogSysLang				*m_aLogSysLang;	// Array of languages with errors (access using index, not id)
	LogsysLangId_vs_Index_Map_t *m_mapIdIndex;	// Map to get language index using language id

	m4bool_t		m_bWorkDirCalled;			// Global function 'LogsysSetWorkingDir' called or no
	m4char_t		m_pszWorkDir[ M4CH_MAX_WORKING_DIRECTORY + sizeof(M4CH_LOGMSG_FILE) + 2*sizeof(M4_BACK_SLASH) + 1] ;	// Filename with full path: "C:\workdir\2\logmsg.ini" - only for read

	m4uint32_t GetLangIndexById(m4language_t ai_LanguageId)
	{
		// Return the Language Index using Id
		m4uint32_t uRet = LOGSYS_INVALID_M4UINT32;
		LogsysLangId_vs_Index_MapIterator_t oIterator = m_mapIdIndex->find( ai_LanguageId );

		if ( oIterator != m_mapIdIndex->end() )
		{
			uRet = (*oIterator).second;
		}
		return uRet;
	}
};



// Factory for creation and destruction of M4LogSysEntryPoint.
class ClM4LogSysEntryPointFactory
{
public:

	ClM4LogSysEntryPointFactory ()
	{
		s_mutex = new ClMutex(M4_TRUE);
		s_pEntryPoint = NULL;
		s_bDestroying = M4_FALSE;
	}

	~ClM4LogSysEntryPointFactory ()
	{
		s_bDestroying = M4_TRUE;
		if (s_pEntryPoint != NULL)
		{
			delete s_pEntryPoint;
			s_pEntryPoint = NULL;
		}
		delete s_mutex;
		M4ClLangInfo::FreeInstance(); // To avoid memory leaks, free unique instance
	}

	ClM4LogSysEntryPoint* EntryPointInstance()
	{
		if (s_pEntryPoint == NULL)
		{
			s_mutex->Lock();
			if (s_pEntryPoint == NULL && s_bDestroying == M4_FALSE)
			{
				s_pEntryPoint = new ClM4LogSysEntryPoint();
			}
			s_mutex->Unlock();
		}
		return s_pEntryPoint;
	}

private:
	ClMutex *s_mutex;
	ClM4LogSysEntryPoint* s_pEntryPoint;
	m4bool_t s_bDestroying;
};

static ClM4LogSysEntryPointFactory factoryEntryPoint;


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////                   /////////////////////////
/////////////////////////// Private Methods	  /////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Object constructor
ClM4LogSysEntryPoint::ClM4LogSysEntryPoint()
{
	m_Mutex.Init();

	m_pLang = M4ClLangInfo::Instance();	// Get M4ClLangInfo instance

	m4uint32_t uiCount = m_pLang->GetNumberOfLanguages();
	m_aLogSysLang = new ClM4LogSysLang[ uiCount ];	// Array of objects
	m_mapIdIndex = new LogsysLangId_vs_Index_Map_t;	// Map to get language index using language id

	//Creamos el mapa <LangId, LangIndex>
	m4language_t uLangId;
	for (m4uint32_t i=0; i<uiCount; ++i)
	{
		uLangId = m_pLang->GetErrorIdByIndex(i);	// Get Language Id
		m_aLogSysLang[i].Init(uLangId, i);			// Initialize Language object
		m_mapIdIndex->insert( LogsysLangId_vs_Index_Map_t::value_type(uLangId, i) );
	}

	m_bWorkDirCalled = M4_FALSE;			// Not called LogsysSetWorkDir
	m_pszWorkDir[0]= '\0';
}

// Library Exit point
ClM4LogSysEntryPoint::~ClM4LogSysEntryPoint()
{
	if (m_mapIdIndex)
	{
		delete m_mapIdIndex;
		m_mapIdIndex = NULL;
	}

	if (m_aLogSysLang)
	{
		delete [] m_aLogSysLang;
		m_aLogSysLang = NULL;
	}
}

// Initialize Library:
// - Initialize workdir and pointer to the end
void ClM4LogSysEntryPoint::Init()
{
	if ( m_pszWorkDir && strlen(m_pszWorkDir)>0 && m_pszWorkDir[strlen(m_pszWorkDir)-1] != M4_LOGSYS_BACK_SLASH)
	{
		//Añadimos un BACK_SLASH, al final: "c:\workdir\"
		m_pszWorkDir[strlen(m_pszWorkDir)+1]='\0';
		m_pszWorkDir[strlen(m_pszWorkDir)]=M4_LOGSYS_BACK_SLASH;
	}
}




// Constructor
ClM4LogSysLang::ClM4LogSysLang()
{
	//m_MutexFile.Init();
	m_bInitialized	= M4_FALSE;		// Object not initialized
	m_uLangId		= LOGSYS_INVALID_M4UINT32;
	m_uLangIndex	= LOGSYS_INVALID_M4UINT32;
	m_uMaxError		= 0;
	m_uMinError		= 0;
}


// Language destructor
ClM4LogSysLang::~ClM4LogSysLang()
{
}

// Initialize Language with id and index
void ClM4LogSysLang::Init(m4language_t ai_uiLangId, m4uint32_t ai_uiLangIndex)
{
	m_uLangId		= ai_uiLangId;			// Language Id
	m_uLangIndex	= ai_uiLangIndex;		// Language Index
}

// Initialize Mapped file in memory (logmsg.ini)
// ai_pszWorkDir = WorkDir (without language and filename): "c:\workdir\"
void  ClM4LogSysLang::InitializeFileMem(m4pchar_t ai_pszWorkDir)
{
	if (ai_pszWorkDir == NULL) // WorkDir not found
	{
		return;
	}

	if ( IsInitialized() )	// Check if FileMem is initialized
	{
		return;
	}

	m_mapHits.clear();		// Init m_mapHits when language is changed
	m_setFaults.clear();	// Init m_setFaults when language is changed

	// Creamos el nombre completo: "c:\workdir\2\logmsg.ini"
	m4pchar_t pszLangFileName = NULL;
	m4char_t pszErrorFileName[ M4CH_MAX_WORKING_DIRECTORY + sizeof(M4CH_LOGMSG_FILE) + 2*sizeof(M4_BACK_SLASH) + 1] ;	// Filename with full path: "C:\workdir\2\logmsg.ini"

	strcpy(pszErrorFileName, ai_pszWorkDir);
	pszLangFileName = pszErrorFileName + strlen(pszErrorFileName);
	sprintf( pszLangFileName, "%d%s%s", (int)m_uLangId, M4_BACK_SLASH, M4CH_LOGMSG_FILE );

	// Abrimos fichero mapeado a memoria
	m_FileInMem.Open(pszErrorFileName, FILEMEM_READONLY | FILEMEM_OPT_SEQ);

	// If language file not found, get default language
	if (!m_FileInMem.IsOpen())
	{
		//Use default language
		sprintf(pszLangFileName, "%d%s%s", (int) M4ClLangInfo::Instance()->GetDefaultLanguageId(), M4_BACK_SLASH, M4CH_LOGMSG_FILE);
		m_FileInMem.Open(pszErrorFileName, FILEMEM_READONLY | FILEMEM_OPT_SEQ);
	}


	// For backward compatibility (Mind 3.0), we look in M4_INIT_WORK_DIR if language file not found
	if (!m_FileInMem.IsOpen())
	{
		//Use old format: "c:\workdir\logmsg.ini"
		strcpy(pszLangFileName, M4CH_LOGMSG_FILE);
		m_FileInMem.Open(pszErrorFileName, FILEMEM_READONLY | FILEMEM_OPT_SEQ);
	}

	if (m_FileInMem.IsOpen())
	{
		// If open, calculate min and max
		CalcMaxMinError();
		m_bInitialized = M4_TRUE;	// Mark as initialized
	}
}

// Calculate Min and Max error numbers in the "logmsg.ini"
void	ClM4LogSysLang::CalcMaxMinError()
{
	m4uint32_t uNumber;
	m4pchar_t pszFile;
	m4pchar_t pszFileEnd;

	m_uMinError = 0xFFFFFFFF;
	m_uMaxError = 0;

	if (m_FileInMem.GetSize()==0)  // If file doesn't exist
	{
		return;
	}
	
	pszFile=m_FileInMem.GetPtr();
	pszFileEnd=m_FileInMem.GetPtr()+m_FileInMem.GetSize();

	pszFileEnd -= 5; // Substract pattern string and something for x;, the ending " and so on
	if (pszFileEnd < pszFile)
	{
		return;
	}

	for( ; pszFile != pszFileEnd; pszFile++)
	{
		if ( (pszFile = (m4pchar_t)memchr(pszFile, '\n', pszFileEnd-pszFile)) == NULL)
			break;

		pszFile++;	// Skip \n

		if (!isdigit(*pszFile))
		{
			continue;
		}

		for (uNumber=0; pszFile!=pszFileEnd && isdigit(*pszFile); pszFile++)
		{
			uNumber=(*pszFile-'0')+uNumber*10;
		}
		
		if (pszFileEnd-5 < pszFile)
		{
			return;
		}

		if (pszFile[0] != '=' || pszFile[1] != '\"')
		{
			continue;
		}

		if (uNumber<m_uMinError)
		{
			m_uMinError=uNumber;
		}

		if (uNumber>m_uMaxError)
		{
			m_uMaxError=uNumber;
		}
	}

	return;
}





#ifdef _WINDOWS
BOOL APIENTRY DllMain(HANDLE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;   // OK
}
#endif





///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////                   /////////////////////////
/////////////////////////// Global  Functions /////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


// Function: Sets WorkingDir only once time
// Notes: Actually is called by ChLib in 'chvmenv.cpp' in 'ReadRegistry' method.
void LogsysSetWorkingDir(m4pcchar_t ai_pszWorkDir)
{

	factoryEntryPoint.EntryPointInstance()->m_Mutex.Lock();	//Critical section

	// Set workingdir for all languages
	if ( !factoryEntryPoint.EntryPointInstance()->m_bWorkDirCalled )
	{
		if (ai_pszWorkDir && strlen(ai_pszWorkDir) < sizeof(factoryEntryPoint.EntryPointInstance()->m_pszWorkDir)-2)
		{
			strcpy(factoryEntryPoint.EntryPointInstance()->m_pszWorkDir, ai_pszWorkDir);
			factoryEntryPoint.EntryPointInstance()->Init();		//Initialize WorkDir if doesn't not initialized yet
			factoryEntryPoint.EntryPointInstance()->m_bWorkDirCalled = M4_TRUE;
		}
	}

	factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
}

// Function: Sets Language Id
// Notes: Actually is called by front-end, and Visual Basic Wrapper
void LogsysSetLangId(m4uint32_t ai_uLangId)
{
	factoryEntryPoint.EntryPointInstance()->m_Mutex.Lock();	//Critical section

	M4ClLangInfo* pLang = M4ClLangInfo::Instance();				// Get instance

	pLang->modeCurrentLang = M4ClLangInfo::manualCurrentLang;	// Set manual
	pLang->SetCurrentLangId(ai_uLangId);						// Set language

	factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
}


// Function: Get Error code String in "logmsg.ini"
// Try to find the error in the hits map.
// if doesn't exist, try to find the error in the faults set,
// if not founded look the error in the logmsg.ini file
// Notes: Actually is called by ChLib in 'cldbgutil.cpp' by 'M4ErrorLookup' global function
m4return_t LogsysGetCodeString(
							m4uint32_t ai_ulCode,			// Error code
							m4pchar_t ao_pszOutString,		// Output buffer
							m4uint32_t ai_uBufferSize,		// Buffer Len
							m4puint32_t ao_uBufferFilled,	// Bytes written
							m4language_t ai_LanguageId		// Language Id
							)
{
	m4uint32_t uContLen;

	m4char_t szTemp[30] = "";
	m4uint32_t cTemp = 0;
	m4pchar_t pszFileSeparator = NULL;
	m4pchar_t pszFile = NULL;
	m4pchar_t pszFileEnd = NULL ;
	int iIsUtf8 = 0;
	int iLength = -1;
	m4pchar_t pcMessage = NULL;


	if (ao_uBufferFilled)
	{
		*ao_uBufferFilled=0;
	}

	//**** Seccion critica en toda la DLL ****
	factoryEntryPoint.EntryPointInstance()->m_Mutex.Lock();

	if (factoryEntryPoint.EntryPointInstance()->m_pszWorkDir == NULL)
	{
		// WorkDir not found
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return M4_ERROR;
	}

	/* Bug 0069089
	Hay que pasar un lenguaje válido
	*/
	factoryEntryPoint.EntryPointInstance()->m_pLang->CheckAndSetValidLangId( ai_LanguageId ) ;

	// Obtenemos el Index del Lenguage
	m4uint32_t uiLangIndex = factoryEntryPoint.EntryPointInstance()->GetLangIndexById( ai_LanguageId );

	// Bug 0299483 Si el lenguaje no está se toma el de por defecto
	if( uiLangIndex == LOGSYS_INVALID_M4UINT32 )
	{
		m4language_t	lDefault = factoryEntryPoint.EntryPointInstance()->m_pLang->GetDefaultLanguageId() ;

		if( lDefault != LOGSYS_INVALID_M4UINT32 )
		{
			uiLangIndex = factoryEntryPoint.EntryPointInstance()->GetLangIndexById( lDefault );
		}
	}

	if( uiLangIndex == LOGSYS_INVALID_M4UINT32 )
	{
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return M4_ERROR;
	}

	// Initialize Mapped File, using WorkDir, for the language selected
	ClM4LogSysLang& oLang = factoryEntryPoint.EntryPointInstance()->m_aLogSysLang[uiLangIndex];
	oLang.InitializeFileMem( factoryEntryPoint.EntryPointInstance()->m_pszWorkDir );

	m4return_t ret=M4_SUCCESS;

	// Comprobamos si el fichero esta bien mapeado
	if ( oLang.m_FileInMem.GetSize() == 0	||  // If file or error doesn't exist
			ai_ulCode < oLang.m_uMinError	||
			ai_ulCode > oLang.m_uMaxError )
	{
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return M4_ERROR;
	}


	// Try to find the error in the hits map
	LogsysErrorMapIterator_t mapHitsIterator;
	mapHitsIterator = oLang.m_mapHits.find(ai_ulCode);
	if (mapHitsIterator != oLang.m_mapHits.end())
	{
		// UNICODE FILE
		pszFile = (*mapHitsIterator).second.pszBegin;
		iLength = (*mapHitsIterator).second.uLen;
		iIsUtf8 = (*mapHitsIterator).second.iIsUtf8;

		pcMessage = M4FileToCpp( pszFile, iLength, iIsUtf8 ) ;
		uContLen = iLength ;

		// See this code is duplicated below in this function
		if (uContLen>=ai_uBufferSize)
		{
			ret=M4_WARNING;
			uContLen=ai_uBufferSize-1;
		}

		if (ao_pszOutString)
		{
			memcpy(ao_pszOutString, pcMessage, uContLen);
			ao_pszOutString[uContLen]=0;
		}

		delete [] pcMessage ;

		if (ao_uBufferFilled)
		{
			*ao_uBufferFilled=uContLen+1; // NULL included
		}
		
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return ret;
	}


	// Try to find the error in the faults set
	LogsysErrorSetIterator_t setFaultsIterator;
	setFaultsIterator = oLang.m_setFaults.find(ai_ulCode);

	if (setFaultsIterator != oLang.m_setFaults.end())
	{
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return M4_ERROR;
	}

	// Look the error in the logmsg.ini file

	pszFile = oLang.m_FileInMem.GetPtr();
	pszFileEnd = oLang.m_FileInMem.GetPtr() + oLang.m_FileInMem.GetSize();

	// UNICODE FILE
	iIsUtf8 = 0 ;
	if( oLang.m_FileInMem.GetSize() >= M4UNI_UTF8_BOM_SIZE )
	{
		if( memcmp( pszFile, acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) == 0 )
		{
			iIsUtf8 = 1 ;
		}
	}
	
	if (ao_pszOutString==NULL)
	{
		ai_uBufferSize=LOGSYS_INFINITE_BUFFER_LEN;
	}

	sprintf(szTemp, "%d=\"", (int)ai_ulCode);
	cTemp = strlen(szTemp);

	pszFileEnd -= cTemp+2; // Substract pattern string and something for the ending "
	if (pszFileEnd < pszFile)
	{
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();	// End Critical section
		return M4_ERROR;
	}

	/* Bug 0146888
	El final se debe controla con menor o igual porque el contador puede incrementarse de 2 en 2
	*/
	for( ; pszFile <= pszFileEnd; pszFile++ )
	{
		if (memcmp(pszFile, szTemp, cTemp)!=0)	// Look for the code
		{
			if ( (pszFile = (m4pchar_t)memchr(pszFile, '\n', pszFileEnd-pszFile)) == NULL)
			{
				break;
			}
			continue;
		}

		pszFile+= cTemp; // Skip code

		// Look for the next new line
		if ( (pszFileSeparator = (m4pchar_t)memchr(pszFile, '\n', pszFileEnd-pszFile)) == NULL)
		{
			break;
		}

		// Go to the last "
		while (*pszFileSeparator != '\"')
		{
			pszFileSeparator--;
		}

		if (pszFileSeparator<pszFile)
		{
			// If not ending " found, we get all the line
			pszFileSeparator = (m4pchar_t)memchr(pszFile, '\n', pszFileEnd-pszFile);
			if (*(pszFileSeparator-1) == '\r')
			{
				// In Win32, we need to remove '\r' too.
				pszFileSeparator--;
			}
		}

		uContLen=pszFileSeparator-pszFile;

		// insert the error in the hits map
		st_LogsysError stInfoMap;
		stInfoMap.pszBegin=pszFile;
		stInfoMap.uLen=uContLen;
		stInfoMap.iIsUtf8=iIsUtf8;
		oLang.m_mapHits.insert(LogsysErrorMap_t::value_type(ai_ulCode,stInfoMap));

		// UNICODE FILE
		iLength = uContLen;

		pcMessage = M4FileToCpp( pszFile, iLength, iIsUtf8 ) ;
		uContLen = iLength ;

		if (uContLen>=ai_uBufferSize)
		{
			ret=M4_WARNING;
			uContLen=ai_uBufferSize-1;
		}

		if (ao_pszOutString)
		{
			memcpy(ao_pszOutString, pcMessage, uContLen);
			ao_pszOutString[uContLen]=0;
		}

		delete [] pcMessage ;

		if (ao_uBufferFilled)
		{
			*ao_uBufferFilled=uContLen+1; // NULL included
		}
		
		factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
		return ret;
	}

	// insert the error in the faults set
	oLang.m_setFaults.insert(LogsysErrorSet_t::value_type(ai_ulCode));

	factoryEntryPoint.EntryPointInstance()->m_Mutex.Unlock();
	return M4_ERROR;
}


// Adds a new language
m4bool_t LogsysAddLanguage( m4language_t ai_iLanguage, m4LCID_t ai_iLCID, m4pcchar_t ai_pccName, m4pcchar_t ai_pccErrorDateFormat, m4pcchar_t ai_pccErrorTimeFormat )
{
	// Bug 0299483 Hay que inicializar el array de lenguajes antes de los dinámicos
	factoryEntryPoint.EntryPointInstance() ;

	return( M4ClLangInfo::Instance()->AddLanguage( ai_iLanguage, ai_iLCID, ai_pccName, ai_pccErrorDateFormat, ai_pccErrorTimeFormat ) ) ;
}


