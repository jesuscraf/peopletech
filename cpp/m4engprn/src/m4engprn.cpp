//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             M4EngPrn.cpp
// Project:			 M4EngPrn.dll	
// Author:           Meta Software M.S. , S.A
// Date:			 15/07/98
// Language:         C++
// Operating System: WINDOWS , UNIX (HP, DEC , AIX , SUN)
//   
// Propietary:       Manuel Lazcano Perez
// Modifications: 	 Who && Date	
//
// Definition:
//
//    This module implements the exported functions of the library
//
//
//==============================================================================

#include "m4win.hpp" 
#include "platform.hpp"

#include "clfile.h"

#define _INCLUDING_FROM_M4ENGPRN_
#include "m4engprn.hpp"

//==============================================================================

/*
DLL_FUN M4_SendFileToPrinter(m4char_t* ai_pcPrinterName, M4ClFile ai_oFile)
{
	m4bool_t SwOk = M4_TRUE ;
	
	SwOk = M4_SendFilePathToPrinter( ai_pcPrinterName, (char*)ai_oFile.GetName() ) ;
	
	return SwOk;
}
*/


M4_DECL_M4ENGPRN m4bool_t WINAPI M4_SendFilePathToPrinter(m4char_t* ai_pcPrinterName, m4char_t* ai_pcDataFile)
{
	m4bool_t	SwOk=M4_TRUE ;

#ifdef _WINDOWS

	if(!Win32_SendsDataToPrinter( ai_pcPrinterName, ai_pcDataFile)){
		SwOk = M4_FALSE ;
	}


#elif defined _UNIX

	UNIX_SendsDataToPrinter(ai_pcPrinterName, ai_pcDataFile) ;

#endif
	
	return SwOk ;
}


