	
//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc
// File:             m4odmdef.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             30-August-1999
// Language:         C++
// Operating System: Windows, UNIX
// Design Document:  
//
//
// Definition:
//
//		m4odmdef.hpp
//		File parameters copied from ODMA.
//
//==============================================================================


#ifndef _H_M4DOC_ODMDEF_H__
	#define _H_M4DOC_ODMDEF_H__ 1

/* Constants */
#define M4_M4DOC_API_VERSION			100		/* Version of the API */

#ifdef _WINDOWS
	#define M4_M4DOC_DOCID_MAX		255		/* Windows */
#elif defined( _WINDOWS_ ) || defined( _MSDOS )
	#define M4_M4DOC_DOCID_MAX		80			/* Windows 3.x */
#else
	#define M4_M4DOC_DOCID_MAX		255		/* Other platforms */
#endif

#define M4_M4DOC_DMSID_MAX			9		/* Max length of a DMS ID including the */
										/* terminating NULL character.  */

#define M4_M4DOC_APPID_MAX			16		/* Max length of a application ID including */
										/* the terminating NULL character.  */


/* Maximum length of a filename returned by ODMA including the terminating NULL character.  Platform dependent. */

#ifdef _WINDOWS
	#define M4_M4DOC_FILENAME_MAX	255		/* Windows */
#elif defined( _WINDOWS_ ) || defined( _MSDOS )
	#define M4_M4DOC_FILENAME_MAX	128		/* Windows 3.x */
#elif defined( unix ) || defined( _UNIX ) || defined( UNIX ) || defined(_M4SOLSPARC_) || defined (OS_SOLARIS_2_6)
	#define M4_M4DOC_FILENAME_MAX	1024		/* Unix */
#else
	#define M4_M4DOC_FILENAME_MAX	255		/* Other platforms */
#endif

#endif


/* Siguiendo la FileManager*/
#define	M4_M4DOC_MAX_PATH 512

