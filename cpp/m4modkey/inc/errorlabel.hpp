//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#ifndef	_M4MODKEY_ERRORLABEL_HPP_
#define _M4MODKEY_ERRORLABEL_HPP_

#define	XMLTAGLABEL_ERROR_BAD_XML								"Error. Incorrect XML definition. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	XMLTAGLABEL_ERROR_TAG_MISSING							"Error. Missing tag required. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	XMLTAGLABEL_ERROR_TAG_DUPLICATED						"Error. Tag duplicated. (File: %s Line: %d)\n", __FILE__, __LINE__

#define	XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING			"Error. Mandatory attribute required. (File: %s Line: %d)\n", __FILE__, __LINE__
#define	XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE		"Error. Mandatory attribute require value. (File: %s Line: %d)\n", __FILE__, __LINE__

#define	XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE				"Internal error. Adding atribute. (File: %s Line: %d)\n", __FILE__, __LINE__

#define	M4MODKEY_ERROR_INTERNAL_MEMORY							"Error. Resering memory. Buffer to big. (File: %s Line: %d)\n" , __FILE__, __LINE__

#define	M4MODKEY_ERROR_ENCRYPT									"Error. Encrypting. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	M4MODKEY_ERROR_DECRYPT									"Error. Decrypting. (File: %s Line: %d)\n" , __FILE__, __LINE__

#define M4MODKEY_ERROR_SEND_KEY									"Error. Sending key error. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define M4MODKEY_ERROR_VERIFYING_KEY							"Error. Verifying key error. (File: %s Line: %d)\n" , __FILE__, __LINE__

#define	M4MODKEY_ERROR_FILE_READING								"Error. Reading file. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	M4MODKEY_ERROR_FILE_WRITING								"Error. Writing file. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	M4MODKEY_ERROR_FILE_WRITING_OVR							"Error. Impossible to overwrite. (File: %s Line: %d)\n" , __FILE__, __LINE__

#define	M4MODKEY_ERROR_VALIDATE_VERSION							"Error. Validating file version. (File: %s Line: %d)\n" , __FILE__, __LINE__
#define	M4MODKEY_ERROR_VALIDATE_LICENSE							"Error. Validating license. (File: %s Line: %d)\n" , __FILE__, __LINE__


#endif


