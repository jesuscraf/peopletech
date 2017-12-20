//==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 File:              m4vi_nt.hpp
//	 Project:           m4versioninfo
//	 Author:            Meta Software M.S., S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition: Implementation for classes defined in
//		m4versioninfo.hpp
//
//	 Samples: See m4versioninfo.hpp file.
//
//=============================================================================

#if defined (M4_WINDOWS_RESOURCES)

#include "m4stl.hpp"
#include "m4trace.hpp"

//===========================================================================
// Version Information Strings.
// This array holds the default properties in the version info resource.
//===========================================================================
static char* m4VersionInformationStrings[] =
{
	"BuildInformation",
	"CompanyName",
	"FileDescription",
	"FileVersion",
	"BuildServer",
	"InternalName",
	"LegalCopyright",
	"OriginalFilename",
	"ProductName",
	"ProductVersion"
};	


//===========================================================================
// Class: M4_VS_FIXEDFILEINFO.
//===========================================================================

inline ostream& operator<<(ostream &aio_os, const M4_VS_FIXEDFILEINFO* ai_vi_fixedfileinfo)
{
	m4TraceLevel(11, cout << "M4_VS_FIXEDFILEINFO:" << endl);
	
	aio_os << "M4_VS_FIXEDFILEINFO:" << endl;
	return aio_os;
}



//===========================================================================
// Class: M4_VS_VERSION_INFO.
//===========================================================================

inline ostream& operator<<(ostream &aio_os, const M4_VS_VERSION_INFO* ai_vs_version_info)
{
	int nStrings = 	sizeof(m4VersionInformationStrings)/sizeof(char*);	
	m4TraceLevel(12, cout << "Number of strings: " << nStrings << endl);

	for (int i=0; i<nStrings; i++)
	{
		const char* fileVersionInfoValue =
			ai_vs_version_info->getVersionProperty(m4VersionInformationStrings[i]);
		if (NULL != fileVersionInfoValue)
		{
			aio_os << setw(20) << m4VersionInformationStrings[i] << ": " << (char *)fileVersionInfoValue << endl;
		}
	}
	
	return aio_os;
}


inline const char* M4_VS_VERSION_INFO::getVersionProperty(const char *ai_property, const char *ai_language, const char *ai_codepage) const
{
	m4TraceLevel(11, cout << "M4_VS_VERSION_INFO::getVersionProperty" << endl);
	
	//
	// check input parameters.
	//
	if (NULL == ai_property)
	{
		return NULL;
	}

	//
	// get Language Information.
	//
	LPVOID lpLanguageInfo;
	UINT uiLanguageLen;
	if(VerQueryValue(
		(LPVOID)this, 
		TEXT("\\VarFileInfo\\Translation"),
		&lpLanguageInfo,
		&uiLanguageLen))
	{
		UINT nLanguages = uiLanguageLen/sizeof(DWORD);
		m4TraceLevel(11, cout << "Supported Languages:" << endl);
		m4TraceLevel(12, cout << "   Number of languages: " << nLanguages << endl);
		
		char pcInfoId[1024];
		LPVOID lpInfoValue;
		UINT uiInfoLSize;
		
		sprintf(pcInfoId,"\\StringFileInfo\\%s\\%s", GetTranslationCode(lpLanguageInfo, NULL), ai_property);
		m4TraceLevel(12, cout << "pcInfoId: " << pcInfoId << endl);
		if(VerQueryValue(
			(LPVOID)this, 
			TEXT(pcInfoId),
			&lpInfoValue,
			&uiInfoLSize))
		{
			m4TraceLevel(11, cout << "Version Info Value: " << (char *)lpInfoValue << endl);
			return (const char *)lpInfoValue;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}	
	
	return NULL;
}


inline const char* M4_VS_VERSION_INFO::GetTranslationCode(const void* ai_dwLang,const void* ai_dwCodePage) const
{
	static char translationCode[64];
	WORD *pword = (WORD *)ai_dwLang;
	sprintf (translationCode, "%04x%04x",pword[0], pword[1]);
	return translationCode;
}



//===========================================================================
// function: M4GetComponentVersionInformation.
//===========================================================================
inline void M4GetComponentVersionInformation(const M4_VS_VERSION_INFO *&ao_versionInfo, const char* ai_filename)
{
	//
	// check input parameters.
	//
	if (NULL == ai_filename)
	{
		ao_versionInfo = NULL;
		return;
	}

	// WinNt file extensions.
	const char *wNText[] = {"dll", "exe", "lib"};
	const int nNtext = 3;

	// get filevesion info and filename.
	char filename[1024];
	DWORD dwVersionInfoSize;

	for (int i=0; i<nNtext; i++)
	{
		sprintf(filename, "%s.%s", ai_filename, wNText[i]);
		
		/*
		 * The GetFileVersionInfoSize function determines whether the 
		 * operating system can obtain version information about a specified 
		 * file. If version information is available, GetFileVersionInfoSize 
		 * returns the size, in bytes, of that information. 
		 */
		DWORD dwHandle;			// variable to receive zero
		dwVersionInfoSize = GetFileVersionInfoSize((LPTSTR)filename, &dwHandle);
		if (dwVersionInfoSize)
		{
			break;
		}
	}

	if (!dwVersionInfoSize)
	{
		m4TraceHeaderLevel(10, cout << "Error in GetFileVersionInfoSize()" << endl);
		ao_versionInfo = NULL;
		return;
	}

	/*
	 * The GetFileVersionInfo function returns version information about 
	 * a specified file. 
	 */
	static char versionInfo[4096];
	dwVersionInfoSize = 4096;
	if (!::GetFileVersionInfo((LPTSTR)filename, (DWORD)0, dwVersionInfoSize, versionInfo))
	{
		m4TraceHeaderLevel(10, cout << "Error in GetFileVersionInfo()" << endl);
		ao_versionInfo = NULL;
		return;
	}

	ao_versionInfo = (const M4_VS_VERSION_INFO *)versionInfo;
}




#endif // M4_WINDOWS_RESOURCES
