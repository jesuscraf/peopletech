//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc
// File:             m4dmtmp.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             05-August-1999
// Language:         C++
// Operating System: Windows 32, UNIX - Solaris
// Design Document:  
//
//
// Definition: 
//	Original functions from m4dmtmp.dll now on m4doc.dll.
//		M4DMGetTempFile
//		M4DMGetTempDir
//
//==============================================================================

// HEADERS.

// General Meta4 headers.
#include "m4types.hpp"
#include "m4define.hpp"
#include "m4log.hpp"
#include "m4loadll.hpp"

//This project headeres.
#include "m4docerrors.hpp"
#include "m4odmdef.hpp"
#include "m4docmultipla.hpp"
#include "m4docfunctions.hpp"

//M4FileManagger access
#include "m4filemanager.hpp"  
#include "file_misc.hpp"
#include "m4unicode.hpp"



// *****************************************************************************
// Function: M4DMGetTempFile															
// Module:   m4dmtmp
// Arguments:																	
//		IN:
//			0. thePrefix - prefix of the temporary file.
//			1. docExtension - the extension to be appended to the temporary file name.			
//
//		OUT:
//			2. uniqueFileName - a unique file name with the format: thePrefixXXXXX.docExtension
// 
// *****************************************************************************
m4return_t M4DMGetTempFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
m4pchar_t docExtension;
m4pchar_t thePrefix;

	// Check input parameters.
	if ( (ai_pvArg[0].GetType() != M4CL_CPP_TYPE_STRING_VAR) ||
		 (ai_pvArg[1].GetType() != M4CL_CPP_TYPE_STRING_VAR) )
	{
		M4DOCErrorHandling(M4DOCERROR_CHECKNULLS,"");
		// este error si es de la llamada al método C, por lo tanto no es de LN4
		return M4_ERROR;
	}
	
	thePrefix = ai_pvArg[0].Data.PointerChar;
		
	// Extension to append to the temporary file name.
	docExtension = ai_pvArg[1].Data.PointerChar;

	// Allocate memory for the output.
	//ai_pvArg[2].Data.PointerChar = (m4pchar_t) malloc(M4_M4DOC_FILENAME_MAX+1);
	/*
	ai_pvArg[2].Data.PointerChar =  new m4char_t[M4_M4DOC_FILENAME_MAX+1];
	if(ai_pvArg[2].Data.PointerChar==NULL)
		{ M4DOCErrorHandling(M4DOCERROR_MEMORY_GLOBAL,"");	return M4_ERROR; }	
	*/

//
//// ##### WINDOWS ##### 
//#if defined (ORLANDO_DI_LASSO)
#if defined(WINDOWS) || defined(_WINDOWS) || defined(_M4WINX86_) || defined(_WIN16)
unsigned short uniqueFileName[M4_M4DOC_FILENAME_MAX+1];
unsigned short tempDirectoryName[M4_M4DOC_FILENAME_MAX+1];
unsigned short* ptrExtension;
HANDLE  fileHandle;
		
	// Get the Windows temporary directory.
	if(!GetTempPathW(M4_M4DOC_FILENAME_MAX+1, (LPWSTR) tempDirectoryName))
	{
		M4DOCErrorHandling(M4DOCERROR_UNIQUENAME,"");
		return M4_ERROR;
	}

	// Create the temporary file.
	// The 0 flag means the file will be created and its name checked so it is unique.

	int iLength = -1 ;
	M4XMLCh* pwcPrefix = M4CppToUtf16( thePrefix, iLength ) ;
	UINT r = GetTempFileNameW( (LPWSTR) tempDirectoryName, (LPWSTR) pwcPrefix, 0, (LPWSTR) uniqueFileName);
	delete pwcPrefix ;

	if(!r)
	{
		M4DOCErrorHandling(M4DOCERROR_UNIQUENAME,"");
		return M4_ERROR;
	}

	// Delete file.
	if(_wremove( (wchar_t *) uniqueFileName)!=0)
	{
		M4DOCErrorHandling(M4DOCERROR_UNIQUENAME,"");
		return M4_ERROR;
	}

	// Change extension from TMP to the extension passed to this function.
	ptrExtension = (M4XMLCh*) wcsrchr((wchar_t *) uniqueFileName, '.');

	iLength = -1 ;
	M4XMLCh* pwcExtension = M4CppToUtf16( docExtension, iLength ) ;
	wcsncpy( (wchar_t *) ptrExtension+1, (wchar_t *) pwcExtension, 3);
	delete pwcExtension ;

	// Create a new file with the docExtension extension.
	fileHandle = (HANDLE)CreateFileW((LPWSTR) uniqueFileName, GENERIC_READ&GENERIC_WRITE,FILE_SHARE_READ&FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fileHandle==INVALID_HANDLE_VALUE)
		{ M4DOCErrorHandling(M4DOCERROR_UNIQUENAME,""); return M4_ERROR; }
	else CloseHandle(fileHandle);
	
	// Output value.
	iLength = -1 ;
	m4pchar_t pcUniqueFileName = M4Utf16ToCpp( uniqueFileName, iLength ) ;
	ai_pvArg[2].SetSysString(pcUniqueFileName);
	delete pcUniqueFileName ;

	return M4_SUCCESS;

//
// ###### UNIX #####
//
#else
m4pchar_t ptrUniqueName,ptrTempPath,pureName;
m4char_t vFinalName[M4_M4DOC_FILENAME_MAX+1];
m4pchar_t ptrBegName;
FILE* ptrFile;


	if( ( (ptrUniqueName = new m4char_t[L_tmpnam]) == 0 ) || ( (ptrTempPath = new m4char_t[L_tmpnam]) == 0 ) || ( (pureName = new m4char_t[M4_M4DOC_FILENAME_MAX]) == 0 ) )
	{
		 M4DOCErrorHandling(M4DOCERROR_MEMORY,"");	
		 delete [] ptrUniqueName;  ptrUniqueName = 0;
		 delete [] ptrTempPath;    ptrTempPath = 0;
		 delete [] pureName;	   pureName = 0;
		 return M4_ERROR; 
	}

	// Add prefix and extension to the temporary file name:
	//			path/pureName   -->>  path/thePrefix+pureName+docExtension
	// Get unique file name.
	ptrUniqueName = tmpnam(ptrUniqueName);
	// Extract the file name from the path. The file name begins just after the last slash.
	ptrBegName = strrchr(ptrUniqueName,'/'); 
	strcpy(pureName,ptrBegName+1); // Store the file name.
	for(int i=0;i<=strlen(thePrefix);i++) *(ptrBegName+i+1)=*(thePrefix+i); // Add the prefix to the file name.
	*(ptrBegName+strlen(thePrefix)+1)='\0';	
	strcat((ptrBegName+strlen(thePrefix)),pureName); 
	strcpy(vFinalName,ptrUniqueName);
	strcat(vFinalName,".");
	strncat(vFinalName,docExtension,3);

	// Create file.
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	ptrFile = M4Fopen(vFinalName, M4UniWrite, eEncoding);
	if(ptrFile!=NULL) 
	{
		fclose(ptrFile); // Success
	}
	else //Error.
	{	
		M4DOCErrorHandling(M4DOCERROR_UNIQUENAME,""); 
		//Free memory.
		delete [] ptrUniqueName; ptrUniqueName = 0;
		delete [] ptrTempPath;   ptrTempPath = 0;
		delete [] pureName;		 pureName = 0;
		return M4_ERROR;
	}

	// Copy the file name to the output argument.
	ai_pvArg[2].SetSysString(vFinalName);
	

	//Free memory.
	delete [] ptrUniqueName; ptrUniqueName = 0;
	delete [] ptrTempPath;   ptrTempPath = 0;
	delete [] pureName;		 pureName = 0;

	return M4_SUCCESS;

#endif
}
// *****************************************************************************

/*

// *****************************************************************************
// Function: M4DMGetTempDir															
// Module:   m4dmtmp
// Description This function returns the temporary directory.
// Arguments:	
//																
//		IN:
//
//		OUT:
//			0. tempDir - system temporary directory
// 
// *****************************************************************************
// M4DMGetTempDir
m4return_t M4DMGetTempDir(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
//
//// ##### WINDOWS ##### 
//
#if defined(WINDOWS) || defined(_WINDOWS) || defined(_M4WINX86_) || defined(_WIN16)
m4char_t tempDirectoryName[M4_M4DOC_FILENAME_MAX+1];

	// Get the Windows temporary directory.
	if(!GetTempPath(M4_M4DOC_FILENAME_MAX+1,tempDirectoryName))
		{ M4DOCErrorHandling(M4DOCERROR_M4DMGETTEMPDIR,M4DOCERROR_UNIQUENAME,""); return M4_ERROR; }
	

	ai_pvArg[0].SetSysString(tempDirectoryName);

	return M4_SUCCESS;
///
// ##### UNIX ##### 
#else
		ai_pvArg[0].SetSysString(P_tmpdir);

	return M4_SUCCESS;
#endif
}
// *****************************************************************************

*/

// *****************************************************************************
// Function: M4DMGetTempDir
// Module M4DOC.DLL
// Arguments:							
//		IN: (Como argumentos de LN4, la cabecera de la función es la standart para llamadas desde CVM
//	
//		OUT:
//			Path Temporal de la FileManager donde se crean los ficheros temporales (p.e con los blob)
//
//		RETURN:
//			A CMV M4_SUCCESS, M4_ERROR puesto que los errores son serios
//
//  Description: This function returns the temporal path, that uses the filemanager
//               to create the temp files
// ***************************************************************************** 
// M4DOC.DLL - M4DMGetTempDir

m4return_t M4DMGetTempDir(m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext){
		
	m4pchar_t    	pTempDir;
	m4return_t tReturn    = M4_SUCCESS;

		pTempDir = new m4char_t[M4_M4DOC_MAX_PATH + 1];
		memset(pTempDir, 0, M4_M4DOC_MAX_PATH + 1 );

		tReturn = ClFileManager::Instance()->GetTempDir(pTempDir);
		
		// Devolvemos siempre algo, aunque sea ""
		ai_pvArg[0].SetSysString(pTempDir);

	return tReturn ;
		

}