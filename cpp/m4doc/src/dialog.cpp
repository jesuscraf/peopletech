
//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4dlg.dll
// File:             functions.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             26-January-1999
// Language:         C++
// Operating System: Windows 32 ONLY!
// Design Document:  
//
//
// Definition:
//
//			This file defines the following functions:
//				SelectFile
//
//
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "dialog.hpp"
#include "m4unicode.hpp"	
#include "m4odmdef.hpp"



/* ****************************************************************************** */
// Function: SelectFile
// Module:   m4dlg.dll librray
//
// Arguments:																	
//
//		IN		ai_pvArg[0] :	extension to be used as filter.
//
//		OUT		ai_pvArg[1] :	filePath, user's selection.
//
// Description: 
//		This function creates an Open common dialog box 
//		that lets the user specify the drive, directory
//		and the name of a file or set of files to open.
//		The box is filtered by the parameter 'extension'.
//
/* ****************************************************************************** */
// M4DLG.DLL - SelectFile
m4return_t SelectFile ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4uint32_t iCount = 0, i_aux = 0, iLen;
	m4char_t lp_file[M4_M4DOC_FILENAME_MAX];
	m4char_t *lp_extension;
	OPENFILENAME ofn;

	// no usamos para nada la máquina virtual
		//ClVMRunContext*  pvmContext = (ClVMRunContext*)ai_pvContext;
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;

	// The first argument is the extension.
	// Check for a NULL pointer.
	if( ai_pvArg[0].Data.PointerChar == NULL )
	{
		if( (ai_pvArg[0].Data.PointerChar=(m4pchar_t)malloc(2)) == NULL) return M4_ERROR;
		else *ai_pvArg[0].Data.PointerChar='\0';
	}
	
	iLen = strlen(ai_pvArg[iCount].Data.PointerChar);
	if ( iLen > 0)
	{
		if( (lp_extension=(m4pchar_t)malloc(7+2*iLen))==NULL ) return M4_ERROR;
		lp_extension[0] = '*';
		lp_extension[1] = '.';
		for(i_aux=0;i_aux<iLen;i_aux++)
			lp_extension[2+i_aux] = ai_pvArg[iCount].Data.PointerChar[i_aux];
		lp_extension[2+i_aux] = '\0';
		lp_extension[3+i_aux] = '*';
		lp_extension[4+i_aux] = '.';

		for(i_aux=0;i_aux<iLen;i_aux++)
			lp_extension[5+iLen+i_aux] = ai_pvArg[iCount].Data.PointerChar[i_aux];
		lp_extension[5+iLen+i_aux] = '\0';
		lp_extension[6+iLen+i_aux] = '\0';
	}
	else
	{
		if( (lp_extension=(m4char_t *)malloc(9)) == NULL ) return M4_ERROR;
		lp_extension[0] = '*';
		lp_extension[1] = '.';
		lp_extension[2] = '*';
		lp_extension[3] = '\0';
		lp_extension[4] = '*';
		lp_extension[5] = '.';
		lp_extension[6] = '*';
		lp_extension[7] = '\0';
		lp_extension[8] = '\0';
	}

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = (LPCTSTR)lp_extension;
	ofn.nFilterIndex = 1;
	lp_file[0] = '\0';
	ofn.lpstrFile = lp_file;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.nMaxFile = sizeof(lp_file);
	ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;

    //
	// The GetOpenFileName function creates an Open common dialog box 
	// that lets the user specify the drive, directory, and the name of a file or set of files to open. 
	//
	GetOpenFileName(&ofn);

	free((void *)lp_extension);

	/* Bug 0174509
	Hay que pasar de ANSI a CPP
	*/
	int iLength = -1 ;
	m4pchar_t pcFile = M4ANSIToCpp( lp_file, iLength ) ;
	ai_pvArg[1].SetSysString(pcFile);
	delete pcFile ;
   	
	return M4_SUCCESS;
}


/* ****************************************************************************** */
// Function: SaveFileAs
//
// Arguments:																	
//
//		IN		ai_pvArg[0] :	file name.
//
//		OUT		ai_pvArg[1] :	filePath, user's selection.
//
// Description: 
//		This function creates an Open common dialog box 
//		that lets the user specify the drive, directory
//		and the name of a file to save.
/* ****************************************************************************** */
m4return_t SaveFileAs ( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4char_t		lp_file[ M4_M4DOC_FILENAME_MAX ];
	m4pchar_t		pcFile;
	int				iLength = -1;
	OPENFILENAME	ofn;
	bool			bRet;


	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;


	if( ai_pvArg[0].Data.PointerChar != NULL && strlen( ai_pvArg[0].Data.PointerChar ) > 0 )
	{
		pcFile = M4CppToANSI( ai_pvArg[0].Data.PointerChar, iLength );
		if( iLength < M4_M4DOC_FILENAME_MAX )
		{
			strcpy( lp_file, pcFile );
		}
		delete pcFile;
	}
	else
	{
		lp_file[0] = '\0';
	}


	ZeroMemory( &ofn, sizeof(OPENFILENAME) );
	ofn.lStructSize     = sizeof( OPENFILENAME );
	ofn.hwndOwner       = GetActiveWindow();
	ofn.lpstrFile       = lp_file;
	ofn.lpstrFileTitle  = NULL;
	ofn.nMaxFileTitle   = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.nMaxFile        = sizeof( lp_file );
	ofn.Flags           = OFN_PATHMUSTEXIST;


	bRet = GetSaveFileName( &ofn );

	if( !bRet )
	{
		ai_pvReturn.Data.DoubleData	= M4_ERROR;
		return M4_SUCCESS;
	}


	iLength = -1 ;
	pcFile = M4ANSIToCpp( lp_file, iLength ) ;
	ai_pvArg[1].SetSysString(pcFile);
	delete pcFile ;


	ai_pvReturn.Data.DoubleData	= M4_SUCCESS;
   	

	return M4_SUCCESS;
}