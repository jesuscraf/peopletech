//	=============================================================================
//	
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4printpdfutils.cpp
//	 Project:           m4printpdf
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

#include "m4printpdfutils.hpp"
#include "m4printpdf.hpp"


/****************************************************************************
*	Name:			GetAcrobatVersion
*	Description:	Get Acrobat installed version from registry.
*	Parameters:
*  	Input:			ao_sAcrobatVersion: Acrobat version.
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t _GetAcrobatVersion(string & ao_sAcrobatVersion)
{
	m4bool_t	bErrorCode = M4_TRUE;
	string		sXFDFExtensionValue;
	string		sAcrobatVersionKey;
	string		sDummy = "";

	// Get ".xfdf" key value from the registry.
	bErrorCode = GetXFDFExtensionKeyValue(sXFDFExtensionValue);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}

	// Get Acrobat version.
	sAcrobatVersionKey = sXFDFExtensionValue + "\\" + "AcrobatVersion";
	bErrorCode = GetRegistryKeyValue(sAcrobatVersionKey, sDummy, KEY_QUERY_VALUE, ao_sAcrobatVersion);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

/****************************************************************************
*	Name:			GetXFDFPrinttoCommandKeyValue
*	Description:	This function returns printto/command registry key value. 
*					We look for .xfdf registry key value (AcroExch.XFDDoc), 
*					we will also look for this key within the registry.
*					When we find AcroExch.XFDDoc key value, we will go through all 
*					its tree and try to find printto key value. If we find it, 
*					we will take command key value within it.
*					This printto/command key contains the command needed to execute 
*					a printto commando for xfdf documents.
*	Parameters:
*  	Input:			ao_sXFDFPrinttoCommandKeyValue: "printto/command" 
*					registry key value.
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t GetXFDFPrinttoCommandKeyValue(string & ao_sXFDFPrinttoCommandKeyValue)
{
	m4bool_t	bErrorCode = M4_TRUE;
	LONG		retcode;
	string		sXFDFExtensionValue;
	string		sXFDFPrinttoCommandKey;
	string		sXFDFPrinttoKey;
	string		sAcrobatVersionKey;

	// Get ".xfdf" key value from the registry.
	bErrorCode = GetXFDFExtensionKeyValue(sXFDFExtensionValue);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}

	// Search within the registry for a key which has a child/grandchild/..., that matches with
	// the given key (M4_PRINTTO_KEY_NAME="printto").
	retcode = SearchForRegistryKey(M4_PRINTTO_KEY_NAME, sXFDFExtensionValue, sXFDFPrinttoKey);
	if (retcode != M4_TRUE)
	{
		return M4_FALSE;
	}

	// Get printto command key value.
	sXFDFPrinttoCommandKey = sXFDFPrinttoKey + "\\" + "command";
	string sDummy;
	bErrorCode = GetRegistryKeyValue(sXFDFPrinttoCommandKey, sDummy, KEY_QUERY_VALUE, ao_sXFDFPrinttoCommandKeyValue);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}
	
	return M4_TRUE;
}


/****************************************************************************
*	Name:			GetRegistryKeyValue
*	Description:	This function returns a registry key value, given 
*					a registry key.. 
*	Parameters:
*  	Input:			ai_sRegistryKeyName: registry key name.
*					ai_iSecurityAccessMask: security access mask.
*					ao_sRegistryKeyValue: registry key value.
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t GetRegistryKeyValue(const string &ai_sRegistryKeyName, const string &ai_sRegistrySubKeyName, m4uint32_t ai_iSecurityAccessMask, string & ao_sRegistryKeyValue)
{
	HKEY hKey;
	LONG retcode;
	DWORD dwBufLen = M4_REG_MAX_LEN;
	m4char_t szRegistryKeyValue[M4_REG_MAX_LEN];
	
	retcode = RegOpenKeyEx(HKEY_CLASSES_ROOT, ai_sRegistryKeyName.c_str(), 0, ai_iSecurityAccessMask, &hKey);
	if (retcode != ERROR_SUCCESS)
	{
		return M4_FALSE;
	}
	
	if (ai_sRegistrySubKeyName.empty())
	{
		retcode = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE) szRegistryKeyValue, &dwBufLen);
		if (retcode != ERROR_SUCCESS)
		{
			return M4_FALSE;
		}
	}
	else
	{
		retcode = RegQueryValueEx(hKey, ai_sRegistrySubKeyName.c_str(), NULL, NULL, (LPBYTE) szRegistryKeyValue, &dwBufLen);
		if (retcode != ERROR_SUCCESS)
		{
			return M4_FALSE;
		}
	}

	retcode = RegCloseKey(hKey);
	if (retcode != ERROR_SUCCESS)
	{
		return M4_FALSE;
	}
	
	ao_sRegistryKeyValue = szRegistryKeyValue;
	return M4_TRUE;
}


/****************************************************************************
*	Name:			GetXFDFExtensionKeyValue
*	Description:	This function returns .xfdf registry key value. 
*	Parameters:
*  	Input:			ao_sXFDFExtensionKeyValue: .xfdf registry key value.
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t GetXFDFExtensionKeyValue(string & ao_sXFDFExtensionKeyValue)
{
	m4bool_t bErrorCode = M4_TRUE;
	
	// Look for .xfdf extension within the registry.
	string sDummy;
	bErrorCode = GetRegistryKeyValue(".xfdf", sDummy, KEY_QUERY_VALUE, ao_sXFDFExtensionKeyValue);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}
	return M4_TRUE;
}


/****************************************************************************
*	Name:			SearchForRegistryKey
*	Description:	This function look for a given registry key (ai_sKeyName) 
*					within a registry tree which begins in ai_sParentKeyName.
*					If we find it, we will return th full path to the registry key.
*	Parameters:
*  	Input:			ai_sKeyName: registry key name to find.
*					ai_sParentKeyName: parent registry key name.
*					ao_sKeyFullPathName: full path to the registry key to be found.
*					
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t SearchForRegistryKey(const string &ai_sKeyName, const string &ai_sParentKeyName, string & ao_sKeyFullPathName)
{
	// Get the printto command key value from the registry.
	// Look for .xfdf extension within the registry.
	LONG retcode;
	HKEY hParentKey;
	retcode = RegOpenKeyEx(HKEY_CLASSES_ROOT, ai_sParentKeyName.c_str(), 0, KEY_READ, &hParentKey);
	if (retcode != ERROR_SUCCESS)
	{
		return M4_FALSE;
	}
	
	// Get number of childs for parent key.
	DWORD dwcParentSubKeys = 0; // Number of parent sub keys.
	retcode = RegQueryInfoKey(hParentKey,
					NULL,
					NULL,
					NULL,
					&dwcParentSubKeys,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL);
	
	if (retcode != ERROR_SUCCESS)
	{
		return M4_FALSE;
	}
	
	// Go through all subkeys and try to find the given subkey (ai_sKeyName).
	LONG errorCode = ERROR_SUCCESS;
	for (m4uint32_t i = 0; i < dwcParentSubKeys ; i++)
    {
		CHAR achKey[M4_REG_MAX_LEN];
		DWORD TmpVal = M4_REG_MAX_LEN;
		DWORD dwcSubKeys = 0; // Number of sub keys.
		errorCode = RegEnumKeyEx(hParentKey,
					i,
					achKey,
					&TmpVal,
					NULL,
					NULL,
					NULL,
					NULL);

		if (stricmp(achKey, ai_sKeyName.c_str()) == 0)
		{
			// We have found printto key.
			// Get "command" key value to extract the command line that will be executed.
			// This command line contains the full path to the program.
			ao_sKeyFullPathName = ai_sParentKeyName + "\\" + achKey;
			break;
		}

		// For each one of the subkeys, we also try to find "Printo" key.
		HKEY hKey;
		string sKeyName = ai_sParentKeyName + "\\" + achKey;
		retcode = RegOpenKeyEx(HKEY_CLASSES_ROOT, sKeyName.c_str(), 0, KEY_READ, &hKey);
		if (retcode != ERROR_SUCCESS)
		{
			return M4_FALSE;
		}
		
		retcode = RegQueryInfoKey(hKey,
					NULL,
					NULL,
					NULL,
					&dwcSubKeys,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL);

		retcode = RegCloseKey(hKey);
		if (retcode != ERROR_SUCCESS)
		{
			return M4_FALSE;
		}
	
		if (dwcSubKeys != 0)
		{
			string sDummy;
			retcode = SearchForRegistryKey(ai_sKeyName, sKeyName, ao_sKeyFullPathName);
			if (retcode != M4_TRUE)
			{
				return M4_FALSE;
			}
			
		}		
    }

	retcode = RegCloseKey(hParentKey);
	if (retcode != ERROR_SUCCESS)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}


/****************************************************************************
*	Name:			ExtractProgramName
*	Description:	This function takes the printto command value and get
*					the program name used to print the document.
*					For example, "C:\Program Files\Adobe\Acrobat 
*					6.0\Reader\AcroRd32.exe"  /t "%1" "%2" "%3" "%4", we will
*					return AcroRd32.exe.
*	Parameters:
*  	Input:			ai_sPrinttoCommand: printto command.
*					ao_sProgramName: program name used to print.
*					
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t ExtractProgramName(const string &ai_sPrinttoCommand, string & ao_sProgramName)
{
	// Parse key base element ids and insert them in the list.
	m4int32_t iPos = 0;
	string sPrinttoCommand;
	string sFullProgramName;
	string sProgramName;
	m4pcchar_t pDest = NULL;

	// Check command is not empty.
	if (ai_sPrinttoCommand.empty() == M4_TRUE)
	{
		return M4_FALSE;
	}

	// Check first character. If it is double quote, look for the next double quote.
	m4char_t cFirstChar = ai_sPrinttoCommand[0];
	if (cFirstChar == M4_DOUBLE_QUOTE_CHAR)
	{
		// If the first character is a double quote, this means we have a full path containing
		// spaces, so we will look for the next double quote and we will get the full program path.
		// Remove first character: double quote.
		sPrinttoCommand = ai_sPrinttoCommand.substr(1, ai_sPrinttoCommand.size());
		pDest = strchr(sPrinttoCommand.c_str(), M4_DOUBLE_QUOTE_CHAR);
		iPos = pDest - sPrinttoCommand.c_str();
		if (pDest != NULL)
		{
			m4pchar_t pFullProgramName = new m4char_t[iPos + 1];
			if (pFullProgramName == NULL)
			{
				return M4_FALSE;
			}

			strncpy(pFullProgramName, sPrinttoCommand.c_str(), iPos);
			pFullProgramName[iPos] = '\0';
			sFullProgramName = pFullProgramName;
			delete pFullProgramName;
		}
		else
		{
			return M4_FALSE;
		}
	}
	else
	{
		// If the first character is not a double quote this means that program full path has no
		// spaces within it. In this case, we will look for the first space character within 
		// the printto command, and we will find the program full path in the left side of the string.
		sPrinttoCommand = ai_sPrinttoCommand;
		pDest = strchr(sPrinttoCommand.c_str(), M4_SPACE_CHAR);
		iPos = pDest - sPrinttoCommand.c_str();
		if (pDest != NULL)
		{
			m4pchar_t pFullProgramName = new m4char_t[iPos + 1];
			if (pFullProgramName == NULL)
			{
				return M4_FALSE;
			}
			strncpy(pFullProgramName, sPrinttoCommand.c_str(), iPos);
			pFullProgramName[iPos] = '\0';
			sFullProgramName = pFullProgramName;
			delete pFullProgramName;
		}
		else
		{
			return M4_FALSE;
		}
	}

	// We just have the full path to XFDF documents. We just need to get 
	// the program name instead of full program path.
	pDest = sFullProgramName.c_str();
	while (pDest != NULL)
	{
		m4pcchar_t pAux = pDest;
		
		// Look for program name. Check slashes until we find last slash.
		pDest = strchr(pAux, M4_BACK_SLASH_CHAR);
		if (pDest != NULL)
		{
			pDest++;
		}
		else
		{
			sProgramName = pAux;
		}
	}

	ao_sProgramName = sProgramName;

	return M4_TRUE;
}


/****************************************************************************
*	Name:			GetXFDFProcessList
*	Description:	This function returns the number of instances opened
*					for a given program or process.
*	Parameters:
*  	Input:			ai_sXFDFProgramName: prgogram or process name.
*					ao_iXFDFDocNum: number of instances.
*					
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4bool_t GetXFDFProcessList(const string &ai_sXFDFProgramName, m4uint32_t & ao_iXFDFDocNum) 
{ 
    HANDLE hProcessSnap = NULL;
    BOOL bRet = FALSE;
    PROCESSENTRY32 pe32 = {0};
	ao_iXFDFDocNum = 0;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
	{
        return M4_FALSE;
	}
 
    // Fill in the size of the structure before using it. 
    pe32.dwSize = sizeof(PROCESSENTRY32); 
	bRet = Process32First(hProcessSnap,&pe32);
	while (bRet)
	{
		// Check if we have found the process we are looking for.
		if (ai_sXFDFProgramName.compare(pe32.szExeFile) == 0)
		{
			ao_iXFDFDocNum++;
		}

		// Keep on looking for.
		pe32.dwSize = sizeof(PROCESSENTRY32);
		bRet = Process32Next(hProcessSnap, &pe32);
	}
	
    // Clean up the snapshot object.
    CloseHandle(hProcessSnap); 

    return M4_TRUE; 
}

/****************************************************************************
*	Name:			EnumWindowsProc
*	Description:	This function post a WM_CLOSE message to the process given by
*					the process pid (lParam).
*	Parameters:
*  	Input:			ai_hWindow: window handle.
*					ai_lParam: an application-defined value to be passed to 
*					the callback function.
*
* 	Output:
*
*	Return:
*   Additional Information:
*
****************************************************************************/
bool CALLBACK EnumWindowsProc(HWND ai_hWindow, LPARAM ai_lParam)
{
	DWORD dwID = 0;

	// Obtener el pid del proceso a partir del handle del mismo.
	GetWindowThreadProcessId(ai_hWindow, &dwID);
	
	// Verificar que es el proceso que nos interesa y si es así, le mandamos
	// un mensahe para que se cierre.
	if (dwID == (DWORD)ai_lParam)
	{
		PostMessage(ai_hWindow, WM_CLOSE, 0, 0);
	}
	
	return true;
}

/****************************************************************************
*	Name:			_GetPrinterInfo
*	Description:	Get printer information.
*	Parameters:
*  	Input:			ai_pszPrinterName:	Printer name.
*					ao_sDriverName:		Printer driver name.
*					ao_sPortName:		Printer port name.
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _GetPrinterInfo(m4pchar_t ai_pszPrinterName, string & ao_sDriverName, string & ao_sPortName)
{
	HANDLE				hPrinter = NULL;
	DWORD				dwNeeded = 0;
	PRINTER_INFO_2 *	ppi2 = NULL;
    BOOL				bFlag = M4_FALSE;
	
	// Make sure printer name is valid
	if (OpenPrinter(ai_pszPrinterName, &hPrinter, NULL) == 0)
	{
		return M4_ERROR;
	}
	
	// Find out how much space is needed.
	GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
	if (dwNeeded == 0)
	{
		ClosePrinter(hPrinter);
		return M4_ERROR;
	}
	
	// Allocate enough space for PRINTER_INFO_2.
	ppi2 = (PRINTER_INFO_2 *)GlobalAlloc(GPTR, dwNeeded);
	if (ppi2 == NULL)
	{
		ClosePrinter(hPrinter);
		return M4_ERROR;
	}
	
	// The second GetPrinter() will fill in all the current information
	// so that all you need to do is modify what you're interested in.
	bFlag = GetPrinter(hPrinter, 2, (LPBYTE)ppi2, dwNeeded, &dwNeeded);
	if (!bFlag)
	{
		ClosePrinter(hPrinter);
		GlobalFree(ppi2);
		return M4_ERROR;
	}
	
	// Return driver and port name.
	ao_sDriverName = ppi2->pDriverName;
	ao_sPortName = ppi2->pPortName;
	
	ClosePrinter(hPrinter);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_DdeCallback
*	Description:	DDE Callback function.
*	Parameters:
*  	Input:			uType:		Transaction type.
*					uFmt:		Clipboard data format.
*					hconv:		Handle to the conversation.
*					hsz1:		Handle to a string.
*					hsz2:		Handle to a string.
*					hdata:		Handle to a global memory object.
*					dwData1:	Transaction-specific data.
*					dwData2:	Transaction-specific data.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
HDDEDATA CALLBACK _DdeCallback(
    UINT uType,     // Transaction type.
    UINT uFmt,      // Clipboard data format.
    HCONV hconv,    // Handle to the conversation.
    HSZ hsz1,       // Handle to a string.
    HSZ hsz2,       // Handle to a string.
    HDDEDATA hdata, // Handle to a global memory object.
    DWORD dwData1,  // Transaction-specific data.
	DWORD dwData2)  // Transaction-specific data.
{
    return 0;
}


/****************************************************************************
*	Name:			_GetDDEServerName
*	Description:	Get DDE server name from registry.
*	Parameters:
*  	Input:			ao_sDDEServerName:  DDE server name.
*
* 	Output:
*
*	Return:			0	No error.	
*					1	Error.
*
*  Additional Information:
*
****************************************************************************/
m4void_t _GetDDEServerName(string & ao_sDDEServerName)
{
	// Get "HKEY_CLASSES_ROOT\acrobat\shell\open\ddeexec\application" key value from the registry.
	GetRegistryKeyValue("acrobat\\shell\\open\\ddeexec\\application", "", KEY_QUERY_VALUE, ao_sDDEServerName);

	return;
}


/****************************************************************************
*	Name:			_ConnectDDEServer
*	Description:	Connect to DDE command.
*	Parameters:		ai_dwIdInst:		Specifies the application instance
*										identifier obtained by a previous call to 
*										the DdeInitialize function.
*					ai_sDDEServerName:  The service name of the server application 
*										with which a conversation is to be established.
*					ai_sTopicName:		Name of the topic on which a conversation
*										is to be established
*					ao_hConv:			Handle to the conversation.
*  	Input:		
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4void_t _ConnectDDEServer(DWORD ai_dwIdInst, const string & ai_sDDEServerName, const string & ai_sTopicName, HCONV & ao_hConv, ofstream & ai_fout)
{
	HSZ			hszApp = NULL;
	HSZ			hszTopic = NULL;
	m4int32_t	iDDEErrorCode = 0;

	// Conexión al servidor DDE.
	hszApp = DdeCreateStringHandle(ai_dwIdInst, ai_sDDEServerName.c_str(), 0);
    hszTopic = DdeCreateStringHandle(ai_dwIdInst, ai_sTopicName.c_str(), 0);
	for (int i = 0; i < M4_DDE_CONNECT_NUM; i++)
    {
		PDF_DEBUG(ai_fout << "DdeConnect[" << i << "], application [" << ai_sDDEServerName.c_str() << "], topic [" << ai_sTopicName.c_str() << "]." << endl);
		ao_hConv = DdeConnect(ai_dwIdInst, hszApp, hszTopic, NULL);
		PDF_DEBUG(ai_fout << "hszApp [" << hszApp << "], hszTopic [" << hszTopic << "], ao_hConv [" << ao_hConv << "]." << endl);
		
		// DMLERR_NO_ERROR (0).
		iDDEErrorCode = DdeGetLastError(ai_dwIdInst);
		PDF_DEBUG(ai_fout << "DdeConnect return [" << iDDEErrorCode << "]." << endl);
		
        if (ao_hConv != NULL)
		{
			break;
		}

		Sleep(M4_DDE_CONNECT_DELAY);
    }

	// Liberar los handles.
	DdeFreeStringHandle(ai_dwIdInst, hszApp);
    DdeFreeStringHandle(ai_dwIdInst, hszTopic);
}


/****************************************************************************
*	Name:			_ExecuteDDECommand
*	Description:	Execute DDE command.
*	Parameters:
*  	Input:			ai_dwIdInst:	Specifies the application instance
*									identifier obtained by a previous call to 
*									the DdeInitialize function.
*					ai_hConv:		Handle to the conversation.
*					ai_sCommand:	Handle to a string.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _ExecuteDDECommand(DWORD ai_dwIdInst, HCONV ai_hConv, string ai_sCommand)
{
	HDDEDATA	hData = NULL;

	// Esta función crea un objeto DDE y llena el objeto con los datos del 
	// búfer especificado. Una aplicación DDE utiliza esta función durante las 
	// transacciones que suponen pasar datos a la solicitud de socio. 
	hData = DdeCreateDataHandle(ai_dwIdInst, (LPBYTE)(LPCTSTR) ai_sCommand.c_str(), ai_sCommand.length() + 1, 0, NULL, CF_TEXT, 0);
	if (hData == NULL)
	{
		return M4_ERROR;
	}

	// Esta función inicia una transacción de datos entre un cliente y un servidor. 
	// Sólo una aplicación cliente DDE puede llamar a esta función, y la aplicación 
	// puede usar sólo después de establecer una conversación con el servidor.
	DdeClientTransaction((LPBYTE) hData, 0xFFFFFFFF, ai_hConv, 0L, 0, XTYP_EXECUTE, TIMEOUT_ASYNC, NULL);

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_ShellExecute
*	Description:	This calls ShellExecute system functions that performs 
*					an action on a file.
*	Parameters:
*  	Input:			ai_sFilePath: full path to the documento to be printed.
*					ai_iShowMode: flags that specify how an application is 
*					to be shown when it is opened.
*					ai_sVerboseMode: string, referred to as a verb, that 
*					specifies the action to be performed
*					ai_sParameters: Address of a null-terminated string that 
*					contains the application parameters.
*					This function is not implemented for UNIX.
*
* 	Output:
*
*	Return:			0	No error.	
*					1   Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _ShellExecute(const string &ai_sFilePath, m4int32_t ai_iShowMode, const string &ai_sVerboseMode, const string &ai_sParameters, HANDLE & aio_hProcess)
{

	aio_hProcess = 0;
	SHELLEXECUTEINFO sei = {sizeof(sei), SEE_MASK_NOCLOSEPROCESS , NULL, 
						ai_sVerboseMode.c_str(), ai_sFilePath.c_str(), 
						ai_sParameters.c_str(), NULL, 
						ai_iShowMode, NULL};
	
	
	if (ShellExecuteEx(&sei) == M4_FALSE)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR)&lpMsgBuf, 0, NULL);
		
		// Build error message.
		string sErrorMessage1 = "ShellExecuteEx(ai_sVerboseMode, ai_sFilePath, ai_sParameters) = '" + ai_sVerboseMode + "', '" + ai_sFilePath + "', '" + ai_sParameters + ".";
		string sErrorMessage2 = (LPCTSTR)lpMsgBuf;
		
		SETCODEF(M4PRINT_PDF_ERROR_SHELLEXECUTE, ERRORLOG, "#*s1*#%s#%s#", sErrorMessage1.c_str(), sErrorMessage2.c_str());
		
		// Free the buffer.
		LocalFree(lpMsgBuf); 
		
		return M4_ERROR;
	}

	aio_hProcess = sei.hProcess;
	
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_OpenXFDFDocument
*	Description:	Open XFDF document with Acrobat.
*	Parameters:
*  	Input:			ai_dwIdInst:	Specifies the application instance
*									identifier obtained by a previous call to 
*									the DdeInitialize function.
*					ai_sXFDFFile:	XFDF file path.
*					ao_hConv:		Handle to the conversation.
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t _OpenXFDFDocument(DWORD ai_dwIdInst, const string & ai_sXFDFFile, HCONV & ao_hConv, ofstream & ai_fout)
{
	m4return_t	iRet = M4_SUCCESS;
	m4int32_t	iDDEErrorCode = 0;
	HINSTANCE	hRet = NULL;
	HSZ			hszApp = NULL;
	HSZ			hszTopic = NULL;
	string		sDDEServerName;
	HANDLE		hProcessDummy = 0;
	string		sVerboseMode = "open";
	m4int32_t	iShowMode = SW_HIDE;
	string		sParameters = "";
	
	// char		szApp[] = "acroview";
	string		asDefaultDDEServers[M4_MAX_DDE_SERVER_NUM] = {"AcroviewR10", "AcroviewA10", "Acroview"};
	string		sTopicName = "control";
	
	// Abrir el fichero.
	iRet = _ShellExecute(ai_sXFDFFile, iShowMode, sVerboseMode, sParameters, hProcessDummy);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Hacemos un sleep para que dé tiempo a cargarse el formulario con los datos.
	Sleep(2000);

    // Leer servidor DDE del registro.
	_GetDDEServerName(sDDEServerName);

	// Conectarse al servidor DDE, si se ha encontrado alguno en el registro.
	// Si no se ha encontrado el nombre del servidor DDE en el registro, se prueba una lista
	// de servidores por defecto.
	if (sDDEServerName != "")
	{
		_ConnectDDEServer(ai_dwIdInst, sDDEServerName, sTopicName, ao_hConv, ai_fout);
	}

	// Aunque se haya encontrado un servidor en el registro, si no nos hemos podido conectar
	// se prueba con la lista de servidores por defecto.
	if (ao_hConv == NULL)
	{
		for (int i = 0; i < M4_MAX_DDE_SERVER_NUM; ++i)
		{
			sDDEServerName = asDefaultDDEServers[i];
			_ConnectDDEServer(ai_dwIdInst, sDDEServerName, sTopicName, ao_hConv, ai_fout);
			
			// Comprobar si se ha podido conectar.
			if (ao_hConv != NULL)
			{
				break;
			}
		}
	}

	// Comprobar que la conexión con el servidor DDE ha sido correcta.
	if (ao_hConv == NULL)
	{
		SETCODEF(M4PRINT_PDF_ERROR_DDE_CONNECT, ERRORLOG, "#*s1*#%s#%s#", sDDEServerName.c_str(), sTopicName.c_str());
		return M4_ERROR;
	}
	
	// Se manda el mensaje de abrir documento (DocOpen) antes de hacer cualquier cosa.
	string sOpenDocCmdLine = "[DocOpen(";
	sOpenDocCmdLine += "\"" + ai_sXFDFFile + "\")]";
	iRet = _ExecuteDDECommand(ai_dwIdInst, ao_hConv, sOpenDocCmdLine);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			_GetXFDFProgramName
*	Description:	This function returns the program name used to print a
*					XFDF document.
*
*	Parameters:
*  	Input:			ao_sXFDFProgramName: program name.
*
* 	Output:
*
*	Return:			0	No error.	
*					1   Error.
*
*  Additional Information:
*
****************************************************************************/

m4bool_t _GetXFDFProgramName(string & ao_sXFDFProgramName)
{
	m4bool_t bErrorCode = M4_TRUE;
	m4bool_t iErrorCode = M4_SUCCESS;
	string sPrinttoCommand;
	string sXFDFProgramName;
	
	bErrorCode = GetXFDFPrinttoCommandKeyValue(sPrinttoCommand);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}

	// Extract program name. Printto command is composed by 
	// "path + name to program" + " " + "printto parameters".
	// We just need the first path.
	iErrorCode = ExtractProgramName(sPrinttoCommand, ao_sXFDFProgramName);
	if (bErrorCode != M4_TRUE)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

/****************************************************************************
*	Name:			_IsAcrobatRunning
*	Description:	This function returns if Acrobat is currently running.
*	Parameters:
*
* 	Output:
*
*	Return:			0	No error.	
*					1   Error.
*
*  Additional Information:
*
****************************************************************************/

m4return_t _IsAcrobatRunning(m4bool_t & ao_bIsRunning)
{

	m4bool_t	bErrorCode = M4_TRUE;
	string		sXFDFProgramName = "";
	m4uint32_t	iXFDFDocNum = 0;

	ao_bIsRunning = M4_FALSE;

	// Obtener el nombre de la aplicación Acrobat para XFDF documents.
	bErrorCode = _GetXFDFProgramName(sXFDFProgramName);
	if (bErrorCode != M4_TRUE)
	{
		return M4_ERROR;
	}

	// Obtener el número de instancias o procesos de Acrobat
	// que actualmente se ejecutan en la máquina.
	bErrorCode = GetXFDFProcessList(sXFDFProgramName, iXFDFDocNum);
	if (bErrorCode != M4_TRUE)
	{
		return M4_ERROR;
	}

	// Establecer si Acrobat está ejecutándose o no.
	if (iXFDFDocNum > 0)
	{
		ao_bIsRunning = M4_TRUE;
	}

	return M4_SUCCESS;
}



