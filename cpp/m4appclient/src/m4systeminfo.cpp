//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4systeminfo.cpp
//	 Project:           m4oeinit
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================


#include <m4win.hpp>
#include "m4systeminfo.hpp"

// Max registry path.
#define M4_REG_MAX_LEN 1024
#define M4_MAX_VALUE_NAME 16383

#ifdef _WINDOWS

typedef void (WINAPI *PGetNativeSystemInfo)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
typedef BOOL (WINAPI *PGetMemoryStatusEx) (MEMORYSTATUSEX*);

#endif //_WINDOWS

ClSystemInfo::ClSystemInfo()
{
}

ClSystemInfo::~ClSystemInfo()
{
}

m4return_t ClSystemInfo::GetOperatingSystem(string & ao_sOperatingSystem)
{  
#ifdef _WINDOWS

	OSVERSIONINFOEX				osversioninfo;
	SYSTEM_INFO					systeminfo;
	PGetNativeSystemInfo		pGNSI;
	PGetProductInfo				pGPI;
	BOOL						bOsVersionInfoEx;
	DWORD						dwType;
	char						szBuffer[256];
	
	// Inicializaciones.
	ao_sOperatingSystem = "";
	ZeroMemory(&systeminfo, sizeof(SYSTEM_INFO));
	ZeroMemory(&osversioninfo, sizeof(OSVERSIONINFOEX));
	
	osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osversioninfo);
	
	if (bOsVersionInfoEx == 0)
	{
		return M4_ERROR;
	}
	
	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
	pGNSI = (PGetNativeSystemInfo) GetProcAddress(GetModuleHandle("kernel32.dll"), "GetNativeSystemInfo");
	
	if (pGNSI != NULL)
	{
		pGNSI(&systeminfo);
	}
	else
	{
		GetSystemInfo(&systeminfo);
	}
	
	if (osversioninfo.dwPlatformId == VER_PLATFORM_WIN32s)
	{
		sprintf(szBuffer, "Windows %d.%d", osversioninfo.dwMajorVersion, osversioninfo.dwMinorVersion);
		ao_sOperatingSystem = szBuffer;
	}
	else if (osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		// Test for the Windows 95 product family.
		if (osversioninfo.dwMajorVersion = 4)
		{
			if (osversioninfo.dwMinorVersion = 0)
			{
				if (osversioninfo.szCSDVersion[0] == 'B' || osversioninfo.szCSDVersion[0] == 'C')
				{
					ao_sOperatingSystem = "Windows95OSR2";
				}
				else
				{
					ao_sOperatingSystem = "Windows95";
				}
				if (osversioninfo.dwMinorVersion = 10)
				{
					if (osversioninfo.szCSDVersion[0] == 'A')
					{
						ao_sOperatingSystem = "Windows98SE";
					}
					else
					{
						ao_sOperatingSystem = "Windows98";
					}
				}
				if (osversioninfo.dwMinorVersion = 90)
				{
					ao_sOperatingSystem = "WindowsMillennium";
				}
			}
		}
	}
	else if (osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT && osversioninfo.dwMajorVersion > 4)
	{
		ao_sOperatingSystem = "Microsoft ";
		
		// Specific product.
		if (osversioninfo.dwMajorVersion == 6)
		{
			if (osversioninfo.dwMinorVersion == 0)
			{
				if (osversioninfo.wProductType == VER_NT_WORKSTATION)
				{
					ao_sOperatingSystem += "Windows Vista ";
				}
				else
				{
					ao_sOperatingSystem += "Windows Server 2008 ";
				}
			}
			
			if (osversioninfo.dwMinorVersion == 1)
			{
				if (osversioninfo.wProductType == VER_NT_WORKSTATION)
				{
					ao_sOperatingSystem += "Windows 7 ";
				}
				else
				{
					ao_sOperatingSystem += "Windows Server 2008 R2 ";
				}
			}
			
			pGPI = (PGetProductInfo) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
			pGPI(osversioninfo.dwMajorVersion, osversioninfo.dwMinorVersion, 0, 0, &dwType);

			switch (dwType)
			{
            case PRODUCT_ULTIMATE:
				ao_sOperatingSystem += "Ultimate Edition";
				break;
            case PRODUCT_PROFESSIONAL:
				ao_sOperatingSystem += "Professional";
				break;
            case PRODUCT_HOME_PREMIUM:
				ao_sOperatingSystem += "Home Premium Edition";
				break;
            case PRODUCT_HOME_BASIC:
				ao_sOperatingSystem += "Home Basic Edition";
				break;
            case PRODUCT_ENTERPRISE:
				ao_sOperatingSystem += "Enterprise Edition";
				break;
            case PRODUCT_BUSINESS:
				ao_sOperatingSystem += "Business Edition";
				break;
            case PRODUCT_STARTER:
				ao_sOperatingSystem += "Starter Edition";
				break;
			case PRODUCT_PROFESSIONAL_N:
				ao_sOperatingSystem += "Professional N ";
				break;
			case PRODUCT_HOME_PREMIUM_N:
				ao_sOperatingSystem += "Home Premium N ";
				break;
			case PRODUCT_HOME_BASIC_N:
				ao_sOperatingSystem += "Home Basic N ";
				break;
			case PRODUCT_ULTIMATE_N:
				ao_sOperatingSystem += "Ultimate N ";
				break;
			case PRODUCT_ENTERPRISE_N:
				ao_sOperatingSystem += "Enterprise N ";
				break;
			case PRODUCT_BUSINESS_N:
				ao_sOperatingSystem += "Business N ";
				break;
			case PRODUCT_PROFESSIONAL_E:
				ao_sOperatingSystem += "Professional E ";
				break;
			case PRODUCT_HOME_PREMIUM_E:
				ao_sOperatingSystem += "Home Premium E ";
				break;
			case PRODUCT_HOME_BASIC_E:
				ao_sOperatingSystem += "Home Basic E ";
				break;
			case PRODUCT_ULTIMATE_E:
				ao_sOperatingSystem += "Ultimate E ";
				break;
			case PRODUCT_ENTERPRISE_E:
				ao_sOperatingSystem += "Enterprise E ";
				break;
			case PRODUCT_PRERELEASE:
				ao_sOperatingSystem += "Developer Preview ";
				break;
			case PRODUCT_PRERELEASE_N:
				ao_sOperatingSystem += "Developer Preview N ";
				break;
			case PRODUCT_EMBEDDED:
				ao_sOperatingSystem += "Embedded Standard ";
				break;
			case PRODUCT_THINPC:    
				ao_sOperatingSystem += "ThinPC ";
				break;
            case PRODUCT_CLUSTER_SERVER:
				ao_sOperatingSystem += "Cluster Server Edition";
				break;
            case PRODUCT_DATACENTER_SERVER:
				ao_sOperatingSystem += "Datacenter Edition";
				break;
            case PRODUCT_DATACENTER_SERVER_CORE:
				ao_sOperatingSystem += "Datacenter Edition (core installation)";
				break;
            case PRODUCT_ENTERPRISE_SERVER:
				ao_sOperatingSystem += "Enterprise Edition";
				break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
				ao_sOperatingSystem += "Enterprise Edition (core installation)";
				break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
				ao_sOperatingSystem += "Enterprise Edition for Itanium-based Systems";
				break;
            case PRODUCT_SMALLBUSINESS_SERVER:
				ao_sOperatingSystem += "Small Business Server";
				break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				ao_sOperatingSystem += "Small Business Server Premium Edition";
				break;
            case PRODUCT_STANDARD_SERVER:
				ao_sOperatingSystem += "Standard Edition";
				break;
            case PRODUCT_STANDARD_SERVER_CORE:
				ao_sOperatingSystem += "Standard Edition (core installation)";
				break;
            case PRODUCT_WEB_SERVER:
				ao_sOperatingSystem += "Web Server Edition";
				break;
			}
      }
	  
      if (osversioninfo.dwMajorVersion == 5 && osversioninfo.dwMinorVersion == 2)
      {
		  if (GetSystemMetrics(SM_SERVERR2))
		  {
			  ao_sOperatingSystem += "Windows Server 2003 R2, ";
		  }
		  else if (osversioninfo.wSuiteMask & VER_SUITE_STORAGE_SERVER)
		  {
			  ao_sOperatingSystem += "Windows Storage Server 2003";
		  }
		  else if (osversioninfo.wSuiteMask & VER_SUITE_WH_SERVER)
		  {
			  ao_sOperatingSystem += "Windows Home Server";
		  }
		  else if (osversioninfo.wProductType == VER_NT_WORKSTATION && systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		  {
			  ao_sOperatingSystem += "Windows XP Professional x64 Edition";
		  }
		  else 
		  {
			  ao_sOperatingSystem += "Windows Server 2003, ";
		  }
		  
		  // Server type.
		  if (osversioninfo.wProductType != VER_NT_WORKSTATION)
		  {
			  if (systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			  {
				  if (osversioninfo.wSuiteMask & VER_SUITE_DATACENTER)
				  {
					  ao_sOperatingSystem += "Datacenter Edition for Itanium-based Systems";
				  }
				  else if (osversioninfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				  {
					  ao_sOperatingSystem += "Enterprise Edition for Itanium-based Systems";
				  }
			  }
			  else if (systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			  {
				  if (osversioninfo.wSuiteMask & VER_SUITE_DATACENTER)
				  {
					  ao_sOperatingSystem += "Datacenter x64 Edition";
				  }
				  else if (osversioninfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				  {
					  ao_sOperatingSystem += "Enterprise x64 Edition";
				  }
				  else
				  {
					  ao_sOperatingSystem += "Standard x64 Edition";
				  }
			  }
			  else
			  {
				  if (osversioninfo.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
				  {
					  ao_sOperatingSystem += "Compute Cluster Edition";
				  }
				  else if (osversioninfo.wSuiteMask & VER_SUITE_DATACENTER)
				  {
					  ao_sOperatingSystem += "Datacenter Edition";
				  }
				  else if (osversioninfo.wSuiteMask & VER_SUITE_ENTERPRISE)
				  {
					  ao_sOperatingSystem += "Enterprise Edition";
				  }
				  else if (osversioninfo.wSuiteMask & VER_SUITE_BLADE)
				  {
					  ao_sOperatingSystem += "Web Edition";
				  }
				  else 
				  {
					  ao_sOperatingSystem += "Standard Edition";
				  }
			  }
		  }
      }
	  
      if (osversioninfo.dwMajorVersion == 5 && osversioninfo.dwMinorVersion == 1 )
      {
		  ao_sOperatingSystem += "Windows XP ";
		  
		  if (osversioninfo.wSuiteMask & VER_SUITE_PERSONAL)
		  {
			  ao_sOperatingSystem += "Home Edition";
		  }
		  else
		  {
			  ao_sOperatingSystem += "Professional";
		  }
      }
	  
      if (osversioninfo.dwMajorVersion == 5 && osversioninfo.dwMinorVersion == 0)
      {
		  ao_sOperatingSystem += "Windows 2000 ";
		  
		  if (osversioninfo.wProductType == VER_NT_WORKSTATION)
		  {
			  ao_sOperatingSystem += "Professional";
		  }
		  else 
		  {
			  if (osversioninfo.wSuiteMask & VER_SUITE_DATACENTER)
			  {
				  ao_sOperatingSystem += "Datacenter Server";
			  }
			  else if (osversioninfo.wSuiteMask & VER_SUITE_ENTERPRISE)
			  {
				  ao_sOperatingSystem += "Advanced Server";
			  }
			  else
			  {
				  ao_sOperatingSystem += "Server";
			  }
		  }
      }
	  
	  // Include service pack (if any) and build number.
	  if (strlen(osversioninfo.szCSDVersion) > 0)
      {
          ao_sOperatingSystem += " ";
          ao_sOperatingSystem += osversioninfo.szCSDVersion;
      }
	  
      sprintf(szBuffer, " (build %d)", osversioninfo.dwBuildNumber);
      ao_sOperatingSystem += szBuffer;
	  
      if (osversioninfo.dwMajorVersion >= 6)
      {
		  if (systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		  {
			  ao_sOperatingSystem += ", 64-bit";
		  }
		  else if (systeminfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		  {
			  ao_sOperatingSystem += ", 32-bit";
		  }
      }
   }
   else
   {  
	   return M4_ERROR;
   }

#endif //_WINDOWS

	return M4_SUCCESS;
}


m4return_t ClSystemInfo::GetProcessorInfo(string & ao_sProcessorInfo)
{
#ifdef _WINDOWS

	SYSTEM_INFO		sysInfo;
	string			sProcessorType;
	string			sArchitecture;
	string			sProcessorLevel;
	m4char_t		acProcessorNum[128];
	m4return_t		iRet;	

	// Inicializar.
	ao_sProcessorInfo = "";
	
	GetSystemInfo(&sysInfo);
	
	// Tipo de procesador.
	switch (sysInfo.dwProcessorType)
	{
	case PROCESSOR_INTEL_386:
		sProcessorType = "Type: Intel 386";
		break;
	case PROCESSOR_INTEL_486:
		sProcessorType = "Type: Intel 486";
		break;
	case PROCESSOR_INTEL_PENTIUM:
		sProcessorType = "Type: Intel Pentium compatible";
		break;
	case PROCESSOR_MIPS_R4000:
		sProcessorType = "Type: MIPS";
		break;
	case PROCESSOR_ALPHA_21064:
		sProcessorType = "Type: Alpha";
		break;
	default:
		sProcessorType = "Type: Unknown";
		break;
	}
	
	// Número de procesadores.
	sprintf(acProcessorNum, "Proc. Number: %d", sysInfo.dwNumberOfProcessors);

	// Tipo de arquitectura
	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		sArchitecture = "Architecture: Intel ";

		switch (sysInfo.wProcessorLevel) 
		{
		case 3:
            sProcessorLevel = "Level: 80386";
            /*{
				int iSteppingLevel = m_sysInfo.wProcessorRevision / 100;
				int iStepping = m_sysInfo.wProcessorRevision % 100;
				sStepping.Format( _T("Stepping: %c%u "), iSteppingLevel, iStepping);
            }*/
			break;
		case 4:
            sProcessorLevel = "Level: 80486";
            /*{
				int iSteppingLevel = m_sysInfo.wProcessorRevision / 100;
				int iStepping = m_sysInfo.wProcessorRevision % 100;
				sStepping.Format( _T("Stepping: %c%u "), iSteppingLevel, iStepping);
            }*/
			break;
		case 5:
            sProcessorLevel = "Level: Pentium";
            /*{
				typedef BOOL (*PIPFP)(DWORD);
				PIPFP lpfn = (PIPFP)::GetProcAddress(GetModuleHandle( _T("kernel32.dll") ), "IsProcessorFeaturePresentA");
				if (lpfn)
				{
					if ((lpfn)(PF_MMX_INSTRUCTIONS_AVAILABLE)) 
					{
						sProcessorLevel += _T (" MMX");
					}
				}
				
				int iModel = m_sysInfo.wProcessorRevision / 100;
				int iStepping = m_sysInfo.wProcessorRevision % 100;
				sStepping.Format( _T("Stepping: %u-%u "), iModel, iStepping);
            }*/
			break;
		case 6:
            sProcessorLevel = "Level: Pentium II/Pro";
            /*{
				int iModel = m_sysInfo.wProcessorRevision / 100;
				int iStepping = m_sysInfo.wProcessorRevision % 100;
				sStepping.Format( _T("Stepping: %u-%u "), iModel, iStepping);
            }*/
			break;
		default:
            sProcessorLevel = "Level: Unknown " + sysInfo.wProcessorLevel;
            /*{
				int iModel = m_sysInfo.wProcessorRevision / 100;
				int iStepping = m_sysInfo.wProcessorRevision % 100;
				sStepping.Format( _T("Stepping: %u-%u "), iModel, iStepping);
            }*/
            break;
		}
		break;

	case PROCESSOR_ARCHITECTURE_MIPS:
		sArchitecture = "Architecture: MIPS ";
		switch(sysInfo.wProcessorLevel)
		{
		case 0004:
			sProcessorLevel = "Level: R4000";
			break;
		default:
			sProcessorLevel = "Level: Unknown " + sysInfo.wProcessorLevel;
			break;
		}
		// sStepping.Format( _T("Stepping: 00%u"), sysinfo.wProcessorRevision);
		break;
	case PROCESSOR_ARCHITECTURE_ALPHA:
		sArchitecture = "Architecture: Alpha ";
		sProcessorLevel = "Level: " + sysInfo.wProcessorLevel;
		/*{
		int iModel = m_sysInfo.wProcessorRevision / 100;
		int iStepping = m_sysInfo.wProcessorRevision % 100;
		sStepping.Format( _T("Stepping: %c%u "), iModel, iStepping);
			}*/
		break;
	case PROCESSOR_ARCHITECTURE_PPC:
		sArchitecture = "Architecture: PowerPC ";
		switch (sysInfo.wProcessorLevel)
		{
		case 1:
			sProcessorLevel = "Level: 601";
			break;
		case 3:
			sProcessorLevel = "Level: 603";
			break;
		case 4:
			sProcessorLevel = "Level: 604";
			break;
		case 6:
			sProcessorLevel = "Level: 603+";
			break;
		case 9:
			sProcessorLevel = "Level: 604+";
			break;
		case 20:
			sProcessorLevel = "Level: 620";
			break;
		default:
			sProcessorLevel = "Level: Unknown " + sysInfo.wProcessorLevel;
			break;
		}
		/*{
		int iModel = m_sysInfo.wProcessorRevision / 100;
		int iStepping = m_sysInfo.wProcessorRevision % 100;
		sStepping.Format( _T("Stepping: %u.%u "), iModel, iStepping);
		}*/
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		sArchitecture = "Architecture: Unknown ";
		sProcessorLevel = "Level: Unknown " + sysInfo.wProcessorLevel;
		/*{
			int iModel = m_sysInfo.wProcessorRevision / 100;
			int iStepping = m_sysInfo.wProcessorRevision % 100;
			sStepping.Format( _T("Stepping: %u-%u "), iModel, iStepping);
		}*/
		break;
	default:
		sArchitecture = "Architecture: Unknown " +  sysInfo.wProcessorArchitecture;
		sProcessorLevel = "Level: Unknown " + sysInfo.wProcessorLevel;
		/*{
			int iModel = m_sysInfo.wProcessorRevision / 100;
			int iStepping = m_sysInfo.wProcessorRevision % 100;
			sStepping.Format( _T("Stepping: %u-%u "), iModel, iStepping);
		}*/
		break;
    }

	ao_sProcessorInfo = sProcessorType + ", " + acProcessorNum + ", " + sArchitecture + ", " + sProcessorLevel;

#endif //_WINDOWS

	return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetMemory(m4uint64_t & ao_iMemory)
{
#ifdef _WINDOWS

	PGetMemoryStatusEx	pGMSE = NULL;
	MEMORYSTATUSEX		memoryex;
	m4bool_t			bErrorCode = M4_FALSE;

	// Inicializar.
	ao_iMemory = 0;

	// Call GlobalMemoryStatusEx..
	pGMSE = (PGetMemoryStatusEx) GetProcAddress(GetModuleHandle("kernel32.dll"), "GlobalMemoryStatusEx");
	if (pGMSE != NULL)
	{
		memset(&memoryex, 0, sizeof(MEMORYSTATUSEX));
		memoryex.dwLength = sizeof(memoryex);
		bErrorCode = pGMSE(&memoryex);
	}
	
	if (bErrorCode == M4_TRUE)
	{
		ao_iMemory = memoryex.ullTotalPhys / (1024 * 1024);
		return M4_SUCCESS;
	}
	else
	{
		return M4_ERROR;
	}

#endif //_WINDOWS

	return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetUserLanguage(string & ao_sLanguage)
{
#ifdef _WINDOWS

	m4int32_t   iRet = M4_SUCCESS;
	m4char_t	szLanguage[128];

	// Inicializaciones
	ao_sLanguage = "";

	iRet = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLANGUAGE, szLanguage, 128);

	if (iRet != 0)
	{
		ao_sLanguage = szLanguage;
	}

#endif //_WINDOWS

	return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetNETFramework(string & ao_sNETFramework)
{
#ifdef _WINDOWS

	LONG		retcode;
	HKEY		hParentKey;
	TCHAR		achClass[MAX_PATH];
    DWORD		cchClassName = MAX_PATH;
    DWORD		cSubKeys = 0;
    DWORD		cbMaxSubKey;
    DWORD		cchMaxClass;
    DWORD		cValues;
    DWORD		cchMaxValue;
    DWORD		cbMaxValueData;
    DWORD		cbSecurityDescriptor;
    FILETIME	ftLastWriteTime;
	m4uint32_t	i = 0;
	string		sUserAgentPostPlantform;
	    
    TCHAR	achValue[M4_MAX_VALUE_NAME]; 
    DWORD	cchValue = M4_MAX_VALUE_NAME; 

	// Inicializar
	ao_sNETFramework = "";

	sUserAgentPostPlantform = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\5.0\\User Agent\\Post Platform";
	retcode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sUserAgentPostPlantform.c_str(), 0, KEY_READ, &hParentKey);
	if (retcode != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	// Obtener el número de claves que son hijas de UserAgent/PostPlatform.
	retcode = RegQueryInfoKey(
        hParentKey,              // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
	if (retcode != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Iterar por todos los valores de esta clave.
	if (cValues != 0) 
    {
		for (i = 0; i < cValues ; i++)
		{
			cchValue = M4_MAX_VALUE_NAME; 
            achValue[0] = '\0';

            retcode = RegEnumValue(hParentKey, i, 
                achValue, 
                &cchValue, 
                NULL, 
                NULL,
                NULL,
                NULL);
			
			if (retcode != M4_SUCCESS)
			{
				continue;
			}
			
			if (ao_sNETFramework.empty() == M4_TRUE)
			{
				ao_sNETFramework = achValue;
			}
			else
			{
				ao_sNETFramework = ao_sNETFramework + ";" + achValue;
			}
		}
	}

	retcode = RegCloseKey(hParentKey);
	if (retcode != M4_SUCCESS)
	{
		return M4_ERROR;
	}

#endif //_WINDOWS
	
	return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetDiskSpaceInfo(m4pchar_t ai_szDriveName, m4uint64_t & ao_iTotalBytes, m4uint64_t & ao_iFreeBytes)
{
#ifdef _WINDOWS

	BOOL		fResult = 0;
    m4pchar_t	pszDrive  = NULL;
    m4char_t	szDrive[4];

    DWORD		dwSectPerClust = 0;
    DWORD       dwBytesPerSect = 0;
	DWORD		dwFreeClusters = 0;
	DWORD		dwTotalClusters = 0;

	m4uint64_t	i64FreeBytesToCaller = 0;
	m4uint64_t	i64TotalBytes = 0;
	m4uint64_t  i64FreeBytes = 0;

	// If the drive is a drive letter and not a UNC path, append a 
	// trailing backslash to the drive letter and colon.
	if (ai_szDriveName != NULL && ai_szDriveName[1] == ':')
	{
		szDrive[0] = pszDrive[0];
		szDrive[1] = ':';
		szDrive[2] = '\\';
		szDrive[3] = '\0';
		
		pszDrive = szDrive;
	}
	else
	{
		pszDrive = ai_szDriveName;
	}

	fResult = GetDiskFreeSpace(pszDrive, &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwTotalClusters);

	if (fResult == 0)
	{
		return M4_ERROR;
	}

	/* force 64-bit math */ 
	i64TotalBytes = (m4int64_t)dwTotalClusters * dwSectPerClust *	dwBytesPerSect;
	i64FreeBytes = (m4int64_t)dwFreeClusters * dwSectPerClust * dwBytesPerSect;
	
	// En MB.
	ao_iTotalBytes = i64TotalBytes / (1024 * 1024);
	ao_iFreeBytes = i64FreeBytes / (1024 * 1024);

#endif //_WINDOWS

	return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetScreenResolution(m4uint32_t & ao_iWidth, m4uint32_t & ao_iHeight)
{
#ifdef _WINDOWS

	// width
   ao_iWidth = GetSystemMetrics(SM_CXSCREEN);

   // height
   ao_iHeight = GetSystemMetrics(SM_CYSCREEN);

#endif //_WINDOWS

   return M4_SUCCESS;
}

m4return_t ClSystemInfo::GetColorDepth(m4uint32_t & ao_iColorDepth)
{
#ifdef _WINDOWS

	HDC		hdc = 0;
	
	hdc = GetDC(NULL);
	ao_iColorDepth = GetDeviceCaps(hdc,BITSPIXEL);
	ReleaseDC(NULL, hdc);

#endif //_WINDOWS

	return M4_SUCCESS;
}

m4return_t ClSystemInfo::Serialize(string & ao_sSystemInfo)
{
#ifdef _WINDOWS

	string				sSeparator = "#";
	string				sSystemInfo;
	string				sUserAgent;
	string				sUserLang;
	string				sOperatingSystem;
	string				sProcessorInfo;
	string				sNETFramework;

	m4char_t			acMemory[128];		
	m4char_t			acUsedBytes[128];
	m4char_t			acFreeBytes[128];
	m4char_t			acScreenResolution[128];
	m4char_t			acColorDepth[128];

	m4uint64_t			iMemory = 0;
	m4uint64_t			iTotalBytes = 0;
	m4uint64_t			iFreeBytes = 0;
	m4uint32_t			iScreenWidth = 0;
	m4uint32_t			iScreenHeight = 0;
	m4uint32_t			iColorDepth = 0;
	m4uint32_t			iScreenResolution = 0;


	GetUserLanguage(sUserLang);
	GetOperatingSystem(sOperatingSystem);
	GetProcessorInfo(sProcessorInfo);
	GetNETFramework(sNETFramework);
	GetMemory(iMemory);
	GetDiskSpaceInfo(NULL, iTotalBytes, iFreeBytes);
	GetScreenResolution(iScreenWidth, iScreenHeight);
	GetColorDepth(iColorDepth);
	
	// v1.0#user_agent#idioma_usuario#sistema_operativo#browser#Framework .NET#procesador#memoria#disco_utilizado#disco_libre#resolución_pantalla#profundidad_color#
	sprintf(acMemory, "%d", iMemory);
	sprintf(acUsedBytes, "%d", iTotalBytes - iFreeBytes);
	sprintf(acFreeBytes, "%d", iFreeBytes);
	sprintf(acScreenResolution, "%dx%d", iScreenWidth, iScreenHeight);
	sprintf(acColorDepth, "%d", iColorDepth);
	
	ao_sSystemInfo = "v1.0" + sSeparator + sUserAgent + sSeparator + sUserLang + sSeparator + sOperatingSystem + sSeparator;
	ao_sSystemInfo = ao_sSystemInfo + sNETFramework + sSeparator;
	ao_sSystemInfo = ao_sSystemInfo + sProcessorInfo + sSeparator + acMemory + sSeparator + acUsedBytes + sSeparator + acFreeBytes + sSeparator;
	ao_sSystemInfo = ao_sSystemInfo + acScreenResolution + sSeparator + acColorDepth + sSeparator;

#endif //_WINDOWS

	return M4_SUCCESS;
}


