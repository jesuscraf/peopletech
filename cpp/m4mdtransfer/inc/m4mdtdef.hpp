//==============================================================================
//
// (c) Copyright  1991-2002 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4metadatatransfer
// File:                m4mdtdef.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                31-03-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module includes some defines and structs for internal use.
//	  It also includes some macros for managing errors.
//
//
//==============================================================================


#ifndef	__M4MDTDEF_HPP__
#define	__M4MDTDEF_HPP__

// M4MDTErrorManager
#include "m4mdterror.hpp"


// ============================================================================
// Defines of array limits.
// ============================================================================
#define M4MDT_MAX_FORMATED_ERROR_SIZE				4096
#define M4MDT_DEFAULT_ARRAY_SIZE					128
#define M4MDT_VECTOR_DEFAULT_SIZE					10
#define M4MDT_VERSION_MAX_SIZE						128
#define M4MDT_MAX_FILE_NAME_SIZE					1024
#define M4MDT_MAX_REAL_TABLE_NAME_SIZE				50
#define M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE		50
#define M4MDT_MAX_UPDATE_FUNCTION_NAME_SIZE			100

#define M4MDT_SIZE_DOUBLE							8
#define M4MDT_SIZE_TIMESTAMP						19
#define M4MDT_MAX_SIZE_VARCHAR_ACCESS				255
#define M4MDT_SIZE_LONG								10000		// Por el problema con access 97 ponemos 10000

// Número de idiomas de presentaciones
#define	M4MDT_LANGUAGE_NUMBER						7

// Nombres de las funciones de conversión
#define M4MDT_PRES2OBL_NAME							"presentation2OBL"
#define M4MDT_OBL2PRES_NAME							"OBL2Presentation"
#define M4MDT_TODAY_NAME							"today"
#define M4MDT_BLOB2FILE_NAME						"BLOB2File"
#define M4MDT_FILE2BLOB_NAME						"file2BLOB"
#define M4MDT_BLOB2EXTERNAL_NAME					"BLOB2External"
#define M4MDT_EXTERNAL2BLOB_NAME					"external2BLOB"
#define M4MDT_VALUE_NAME							"value"
#define M4MDT_NORMALIZERETURNS_NAME					"normalizeReturns"
#define M4MDT_PRESENTATIONPACK2EXTERNAL_NAME		"presentationPackage2External"
#define M4MDT_EXTERNAL2PRESENTATIONPACK_NAME		"external2PresentationPackage"
#define M4MDT_FIELD2FILE_NAME						"Field2File"
#define M4MDT_FILE2FIELD_NAME						"File2Field"

// Número de argumentos de las funciones de conversión
#define	M4MDT_PRES2OBL_ARG_NUMBER					10
#define	M4MDT_OBL2PRES_ARG_NUMBER					10
#define	M4MDT_TODAY_ARG_NUMBER						1
#define	M4MDT_BLOB2FILE_ARG_NUMBER					4
#define	M4MDT_FILE2BLOB_ARG_NUMBER					5
#define	M4MDT_BLOB2EXTERNAL_ARG_NUMBER				4	// Al menos 4
#define	M4MDT_EXTERNAL2BLOB_ARG_NUMBER				4
#define	M4MDT_VALUE_ARG_NUMBER						2
#define M4MDT_NORMALIZERETURNS_ARG_NUMBER			2
#define	M4MDT_PRESENTATIONPACK2EXTERNAL_ARG_NUMBER	3	// Al menos 3
#define	M4MDT_EXTERNAL2PRESENTATIONPACK_ARG_NUMBER	3
#define	M4MDT_FILE2FIELD_ARG_NUMBER					2

#define	M4MDT_MAX_UPDATE_FUNCTION_ARG_NUMBER		10


// Número de documentos en la caché
#define	M4MDT_MAX_CACHE_DOMS						200	// A día de hoy son 232 en total


// ============================================================================
// Cabecera de XML
// ============================================================================
#define M4MDT_XML_COPYRIGHT							"<!--\nCopyright 1991-2004 by Meta4, Inc.\nCentro Europa Empresarial - Edf. Roma\nC/ Rozabella, 8\n28230 Las Rozas - Madrid\nSpain\n\nPrivate and Confidential\nThe information contained in this document is the property of Meta4.\nIt is for the exclusive use of designated employees\nand not for distribution without prior written authorization.\n-->\n"


// ============================================================================
// Comienzo y final de los CDATA
// ============================================================================
#define M4MDT_XML_CDATA_PREFIX						"\n#STARTVALUE#\n"
#define M4MDT_XML_CDATA_PREFIX_LENGTH				14
#define M4MDT_XML_CDATA_SUFIX						"\n#ENDVALUE#\n"
#define M4MDT_XML_CDATA_SUFIX_LENGTH				12


// ============================================================================
// Modo de volcado del xml de datos
// ============================================================================
#define	M4MDT_DATAXML_NONE							0x00
#define	M4MDT_DATAXML_FIRST							0x01
#define	M4MDT_DATAXML_SECOND						0x02
#define	M4MDT_DATAXML_ALL							0x03


// ============================================================================
// XML defines
// ============================================================================

// quote
#define M4MDT_XML_QUOTE								"&quot;"

// CDATA
#define M4MD_XML_CDATA_START						"<![CDATA["
#define M4MD_XML_CDATA_END							"]]>\n"



// ============================================================================
// Register PKs separator for rich XML data.
// ============================================================================
#define M4MDT_PKS_SEPARATOR							";"

// ============================================================================
// XML null values.
// ============================================================================
#define M4MDT_XML_NULL_VALUE						"NULL"

// ============================================================================
// Lenguajes
// ============================================================================
#define M4MDT_LANG_ENGLISH							2
#define M4MDT_LANG_SPANISH							3

#define M4MDT_LANG_ENGLISH_STR						"2"
#define M4MDT_LANG_SPANISH_STR						"3"


// ============================================================================
// Nombre de la conexión del diccionario
// ============================================================================
#define M4MDT_DICTIONARY_CONNECTION					"__M4MDT_DICTIONARY_CONNECTION"

// ============================================================================
// Nombre del fichero de errores
// ============================================================================
#define M4MDT_ERROR_FILE_NAME						"m4mdterror.log"

// ============================================================================
// Nombre del fichero de errores
// Defines de los niveles de traza de odbc
// ============================================================================
#define M4MDT_ODBC_TRACE_FILE_NAME					"m4mdtodbc.log"

// ============================================================================
// Nombre del fichero temporal que se crea para los BLOBs
// ============================================================================
#define M4MDT_BLOB_FILE_NAME						"~mdtblobfile"

// ============================================================================
// Niveles de traza
// ============================================================================
#define M4MDT_TRACE_SENTENCE						0x0001
#define M4MDT_TRACE_DATA							0x0002


// ============================================================================
// Mapeo de _snprintf y _vnsprintf en windows
// ============================================================================
#ifdef _WINDOWS
#define vsnprintf					_vsnprintf
#define snprintf					_snprintf
#endif


// ============================================================================
// Defines for error management.
// ============================================================================

// ============================================================================
// Para comprobar un error y salir de la función sin volcar error
// ============================================================================
#define M4MDT_CHECK_RETURN(cond, result)		do { if( cond ) { return( result ) ; } } while( 0 )

// ============================================================================
// Para volcar un error sin retornar de la función
// ============================================================================
#define M4MDT_DUMP_ERROR0(id)									do { M4MDTErrorManager::DumpError( id, "#*S1*#%d#%s#",										__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR1(id, a, format)						do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a,						__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR2(id, a, b, format)						do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b,					__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR3(id, a, b, c, format)					do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c,				__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR4(id, a, b, c, d, format)				do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c, d,				__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR5(id, a, b, c, d, e, format)			do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c, d, e,			__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR6(id, a, b, c, d, e, f, format)			do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c, d, e, f,		__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR7(id, a, b, c, d, e, f, g, format)		do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c, d, e, f, g,	__LINE__, __FILE__ ) ; } while( 0 )
#define M4MDT_DUMP_ERROR8(id, a, b, c, d, e, f, g, h, format)	do { M4MDTErrorManager::DumpError( id, "#*S1*#"##format##"#%d#%s#", a, b, c, d, e, f, g, h,	__LINE__, __FILE__ ) ; } while( 0 )

// ============================================================================
// Para comprobar y volcar un error sin retornar de la función
// ============================================================================
#define M4MDT_CHECK_ERROR0(cond, id)									do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#%d#%s#%s#",										__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR1(cond, id, a, format)							do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a,						__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR2(cond, id, a, b, format)						do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b,						__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR3(cond, id, a, b, c, format)					do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c,					__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR4(cond, id, a, b, c, d, format)				do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d,				__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR5(cond, id, a, b, c, d, e, format)				do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e,			__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR6(cond, id, a, b, c, d, e, f, format)			do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f,			__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR7(cond, id, a, b, c, d, e, f, g, format)		do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g,		__LINE__, __FILE__, #cond ) ; } while( 0 )
#define M4MDT_CHECK_ERROR8(cond, id, a, b, c, d, e, f, g, h, format)	do { M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g, h,	__LINE__, __FILE__, #cond ) ; } while( 0 )

// ============================================================================
// Para comprobar y volcar un error retornando de la función
// ============================================================================
#define M4MDT_RETURN_ERROR0(cond, result, id)									do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#%d#%s#%s#",										__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR1(cond, result, id, a, format)						do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a,						__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR2(cond, result, id, a, b, format)						do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b,						__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR3(cond, result, id, a, b, c, format)					do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c,					__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR4(cond, result, id, a, b, c, d, format)				do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d,				__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR5(cond, result, id, a, b, c, d, e, format)			do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e,			__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR6(cond, result, id, a, b, c, d, e, f, format)			do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f,			__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR7(cond, result, id, a, b, c, d, e, f, g, format)		do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g,		__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )
#define M4MDT_RETURN_ERROR8(cond, result, id, a, b, c, d, e, f, g, h, format)	do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g, h,	__LINE__, __FILE__, #cond ) ) { return( result ) ; } ; } while( 0 )

// ============================================================================
// Para comprobar retornando de la función void
// ============================================================================
#define M4MDT_VOID_ERROR0(cond, id)									do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#%d#%s#%s#",										__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR1(cond, id, a, format)						do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a,						__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR2(cond, id, a, b, format)					do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b,						__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR3(cond, id, a, b, c, format)				do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c,					__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR4(cond, id, a, b, c, d, format)				do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d,				__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR5(cond, id, a, b, c, d, e, format)			do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e,			__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR6(cond, id, a, b, c, d, e, f, format)		do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f,			__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR7(cond, id, a, b, c, d, e, f, g, format)	do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g,		__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )
#define M4MDT_VOID_ERROR8(cond, id, a, b, c, d, e, f, g, h, format)	do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g, h,	__LINE__, __FILE__, #cond ) ) { return; } ; } while( 0 )

// ============================================================================
// Para comprobar y volcar un error limpiando variables y retornando de la función
// ============================================================================
#define M4MDT_CLEAN_ERROR0(cond, result, id, action)									do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#%d#%s#%s#",										__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR1(cond, result, id, action, a, format)							do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a,						__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR2(cond, result, id, action, a, b, format)						do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b,						__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR3(cond, result, id, action, a, b, c, format)					do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c,					__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR4(cond, result, id, action, a, b, c, d, format)				do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d,				__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR5(cond, result, id, action, a, b, c, d, e, format)				do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e,			__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR6(cond, result, id, action, a, b, c, d, e, f, format)			do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f,			__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR7(cond, result, id, action, a, b, c, d, e, f, g, format)		do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g,		__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )
#define M4MDT_CLEAN_ERROR8(cond, result, id, action, a, b, c, d, e, f, g, h, format)	do { if( M4MDTErrorManager::CheckError( cond, id, "#*S1*#"##format##"#%d#%s#%s#", a, b, c, d, e, f, g, h,	__LINE__, __FILE__, #cond ) ) { action ; return( result ) ; } ; } while( 0 )



#endif

