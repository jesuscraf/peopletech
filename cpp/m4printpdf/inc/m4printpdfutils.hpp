//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4printpdfutils.hpp
//	 Project:           M4DMSXML
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
//	=========================================================
//	=====================

#ifndef printpdfutils_h
#define printpdfutils_h 1

#include "m4types.hpp"
#include "m4stl.hpp"

#include "m4printpdfres.hpp"
#include "chlog.hpp"

#ifdef _WINDOWS

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#endif 

// registry entry name for printto command.
#define M4_PRINTTO_KEY_NAME		"Printto"

// Characters.
#define M4_DOUBLE_QUOTE_CHAR	'"'
#define M4_SPACE_CHAR			' '
#define M4_BACK_SLASH_CHAR		'\\'

// Max registry path.
#define M4_REG_MAX_LEN			1024

// DDE connect delay.
#define M4_DDE_CONNECT_DELAY	500
#define M4_DDE_CONNECT_NUM		10
#define M4_MAX_DDE_SERVER_NUM	3

#ifdef _WINDOWS


m4bool_t GetXFDFPrinttoCommandKeyValue(string & ao_sXFDFPrinttoCommandKeyValue);
m4bool_t GetRegistryKeyValue(const string &ai_sRegistryKeyName, const string &ai_sRegistrySubKeyName, m4uint32_t ai_iSecurityAccessMask, string & ao_sRegistryKeyValue);
m4bool_t GetXFDFExtensionKeyValue(string & ao_sXFDFExtensionKeyValue);
m4bool_t SearchForRegistryKey(const string &ai_sKeyName, const string &ai_sParentKeyName, string & ao_sKeyFullPathName);
m4bool_t ExtractProgramName(const string &ai_sPrinttoCommand, string & ao_sProgramName);
m4bool_t GetXFDFProcessList(const string &ai_sXFDFProgramName, m4uint32_t & ao_iXFDFDocNum);
bool CALLBACK EnumWindowsProc(HWND ai_hWindow, LPARAM ai_lParam);

m4bool_t _GetXFDFProgramName(string & ao_sXFDFProgramName);

m4return_t _GetPrinterInfo(m4pchar_t ai_pszPrinterName, string & ao_sDriverName, string & ao_sPortName);

HDDEDATA CALLBACK _DdeCallback(UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2, HDDEDATA hdata, DWORD dwData1, DWORD dwData2);
m4void_t _GetDDEServerName(string & ao_sDDEServerName);
m4void_t _ConnectDDEServer(DWORD ai_dwIdInst, const string & ai_sDDEServerName, const string & ai_sTopicName, HCONV & ao_hConv, ofstream & ai_fout);
m4return_t _ExecuteDDECommand(DWORD ai_dwIdInst, HCONV ai_hConv, string ai_sCommand);
m4return_t _OpenXFDFDocument(DWORD ai_dwIdInst, const string & ai_sXFDFFile, HCONV & ao_hConv, ofstream & ai_fout);
m4return_t _IsAcrobatRunning(m4bool_t & ao_bIsRunning);
m4bool_t _GetAcrobatVersion(string & ao_sAcrobatVersion);
m4return_t _ShellExecute(const string &ai_sFilePath, m4int32_t ai_iShowMode, const string &ai_sVerboseMode, const string &ai_sParameters, HANDLE & aio_hProcess);


#endif


#endif
