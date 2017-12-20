//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             DLLMAIN.CPP
// Project:			 M4FMT32.DLL	
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the DLL entries
//
//
//==============================================================================


#include "m4win.hpp"
#include "m4fmt.hpp"

#ifdef _WINDOWS

	HINSTANCE g_Fmt_hInst ;

	BOOL CALLBACK DllMain(HANDLE hInst, DWORD ul_reason_being_called, LPVOID lpReserved) 
	{
		g_Fmt_hInst = (HINSTANCE) hInst ;
		switch (ul_reason_being_called) 
		{
			case DLL_PROCESS_ATTACH:
				break;
			case DLL_THREAD_ATTACH:
				break;
			case DLL_PROCESS_DETACH:
				break;
			case DLL_THREAD_DETACH:
				break;
			default:
				break;
		}
			
		return TRUE;
		UNREFERENCED_PARAMETER(lpReserved);                                         
	}

#else

	int CALLBACK LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeap, LPSTR lpszCmdLine)
	{
		g_Fmt_hInst = hInst ;
		return 1;
	}         
          
	int CALLBACK _WEP(int nExitType)
	{   
		g_Fmt_hInst = hInst ;
		return 1;
	}

#endif

