
//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc 
// File:             functions.hpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             26-August-1999
// Language:         C++
// Operating System: WINDOWS, UNIX
// Design Document:  
//
//
// Definition:
//			This module defines the build_appropriate_dll_name function.
//
//
//==============================================================================

#ifndef _HPP_M4DOC_FUNCTIONS_HPP__ 
	#define _HPP_M4DOC_FUNCTIONS_HPP__ 1

// General Meta4 headers.
#include "m4doc_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
#include "m4define.hpp"
#include "m4var.hpp"
#include "m4log.hpp"
#include "m4docmultipla.hpp"




// Basic DM functions.
// Type definition of DM dlls function pointers,
typedef m4return_t (*DMSDELETEDOC)( m4pchar_t , m4pchar_t );
typedef m4return_t (*DMSLOADDOC)( m4pchar_t , m4pchar_t , m4pchar_t );
typedef m4return_t (*DMSCLOSEDATABASE)( m4pchar_t );
typedef m4return_t (*DMSNEWDOC)( m4pchar_t , m4pchar_t , m4pchar_t , m4pchar_t );
typedef m4return_t (*DMSSAVEDOC)( m4pchar_t , m4pchar_t , m4pchar_t );
typedef m4return_t (*GETALLPK)( const string&, const string& ai_sFormulae, string& );

typedef m4return_t (*DMSLOADDOCAD)( m4pchar_t , m4pchar_t , m4pchar_t, m4pchar_t, m4pchar_t );
typedef m4return_t (*DMSNEWDOCAD)( m4pchar_t , m4pchar_t , m4pchar_t , m4pchar_t, m4pchar_t, m4pchar_t );
typedef m4return_t (*DMSSAVEDOCAD)( m4pchar_t , m4pchar_t , m4pchar_t, m4pchar_t, m4pchar_t );


// Misc.
// Function that builds the appropiate full dll name according to the Operating System.
void GetFullDllName(string& sDllName);
// Función que calcula el tamaño de un fichero dado por su Path.
m4return_t GetFileSize (const string& ai_sFilePath, m4uint32_t& ao_iFileSize);
// Función que calcula la clave MD5 de un fichero dado por su Path.
m4return_t GetMD5KeyFromPath (const string& ai_sDocPath, string& ao_sMD5Key);


extern "C" M4_DECL_M4DOC m4return_t LoadDoc( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t NewDoc( m4VariantType *ai_pvArg,  m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn,  m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t DeleteDoc( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t SaveDoc( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DOC m4return_t GetAllPKDoc( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DOC m4return_t M4CloseDatabase( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DOC m4return_t M4ExecuteCommand(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg,  m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DOC m4return_t M4GenerateZip(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4GenerateCRC(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4ExpandZip(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);


// Temporary file handling functions
extern "C" M4_DECL_M4DOC m4return_t M4DMGetTempFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4DMGetTempDir(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

// File management functions.
extern "C" M4_DECL_M4DOC m4return_t M4CopyFileGD( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4DMDeleteFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4DMSetPermissionOnFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4ExistFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4ExistDir( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4GetDir( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4DeleteFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

// Shell de procesos
extern "C" M4_DECL_M4DOC m4return_t M4ShellExecute(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

//FileManager Wrapper functions.
extern "C" M4_DECL_M4DOC m4return_t M4GetFileManagerOwnerOfTheFile(m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4SetFileManagerOwnerOfTheFile(m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4CreatePath(m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);

// Verity Extended Search functions.
extern "C" M4_DECL_M4DOC m4return_t M4OpenExtSearch (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4CloseExtSearch (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4GetNextFile (m4VariantType *ai_pvArg,m4uint32_t ai_iLongArg,m4VariantType &ai_pvReturn,m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4GetFileSize (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

// MD5.
extern "C" M4_DECL_M4DOC m4return_t M4GetMD5Key (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4RemoveDir (m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DOC m4return_t M4FileToBinary( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4DOC m4return_t M4BinaryToFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4DOC m4return_t M4StringToBinary( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;
extern "C" M4_DECL_M4DOC m4return_t M4BinaryToString( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext ) ;

extern "C" M4_DECL_M4DOC m4return_t M4OpenDefBrowser(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

// For import/export excel in server side.
extern "C" M4_DECL_M4DOC m4return_t LoadCSVFile(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DOC m4return_t M4CopyFileGDRetrying( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
#endif


//Dejar retorno de carro al final para UNIX
