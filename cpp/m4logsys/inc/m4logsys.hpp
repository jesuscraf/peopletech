//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4logsys.hpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           19 jan 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Error Format
////
//==============================================================================

#ifndef __M4LOGSYS_HPP__
#define __M4LOGSYS_HPP__


#include "m4logsys_dll.hpp"
#include "m4logsys_windows.hpp"


// Constants for DeserializeParams
#define LOGSYS_PARSE_ALWAYS	0	// Normal behavior
#define LOGSYS_PARSE_ERROR		1	// Parse only if it is an error (i.e. not an extra)
#define LOGSYS_PARSE_EXTRA		2	// Parse only if it is an extra (i.e. not an error)
#define LOGSYS_PARSE_NEVER		3	// Never parse

// Structure for FormatErrorExtraArray
typedef struct {
	m4uint32_t uExtraType;
	m4uint32_t uExtraHandle;
} pairFormatErrorExtra_t;

inline int operator <= (const pairFormatErrorExtra_t& a, const pairFormatErrorExtra_t& b)
{
	return a.uExtraType<=b.uExtraType;
}

inline int operator < (const pairFormatErrorExtra_t& a, const pairFormatErrorExtra_t& b)
{
	return a.uExtraType<b.uExtraType;
}


#define LOGSYS_INVALID_M4UINT32 0xFFFFFFFF

#define LOGSYS_MAX_ERROR_STRING_LEN 4096	// Max length in a logmsg.ini message
#define LOGSYS_MAX_PARAM_LEN		255		// Max number of params
#define LOGSYS_MAX_EXTRA_PARAMS		255		// Max number of extras

#define LOGSYS_PARAM_SEPARATOR_BEGIN "#"
#define LOGSYS_PARAM_SEPARATOR_BEGIN_LEN (sizeof(LOGSYS_PARAM_SEPARATOR_BEGIN)-1)

#define LOGSYS_PARAM_SEPARATOR_END "#"
#define LOGSYS_PARAM_SEPARATOR_END_LEN (sizeof(LOGSYS_PARAM_SEPARATOR_END)) // NULL included

#define LOGSYS_PARAM_SEPARATOR_INTER "#"
#define LOGSYS_PARAM_SEPARATOR_INTER_LEN (sizeof(LOGSYS_PARAM_SEPARATOR_INTER)-1)

#define LOGSYS_NULL_STRING " <NULL> "

#define LOGSYS_INFINITE_BUFFER_LEN 10000000

class m4VariantType;
/*************************************************************************

NAME: FormatErrorCode

**************************************************************************

ABSTRACT: 
	Parse an error code and a param string.

NOTES:
	It is almost the same as FormatErrorString but ai_ulCode is
	the error code (instead of a error string).
	It returns M4_ERROR if error code is not found.

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t  FormatErrorCode (
	m4uint32_t ai_ulCode,			
	m4pchar_t ai_pszParams,			
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang=LOGSYS_INVALID_M4UINT32
	);


/*************************************************************************

NAME: FormatErrorString

**************************************************************************

ABSTRACT: 
	Parse an error string and a param string.

NOTES:
	It is almost the same as FormatErrorStringByParams but ai_pszParams is
	the param string. See DeserializeParams for info about its format.
	If || are found, they are considered as different messages and a newline
	is added. For instance #.....#||#*S1*E123*#...#...# means there are two
	messages, 123 is the error code of the second message.
	Extra params are like #..#||#*Exx*#...#...#

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t FormatErrorString (
	m4pchar_t ai_pszErrorString,  
	m4pchar_t ai_pszParams,			
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang=LOGSYS_INVALID_M4UINT32);


/*************************************************************************

NAME: FormatErrorCodeByParams

**************************************************************************

ABSTRACT: 
	Parse an error code and a param array.

NOTES:
	It is almost the same as FormatErrorStringByParams but ai_ulCode is
	the error code (instead of a error string).
	It returns M4_ERROR if error code is not found.

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t FormatErrorCodeByParams (
	m4uint32_t ai_ulCode,			
	m4VariantType *ai_pvParams,
	m4uint32_t ai_uParams,
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang=LOGSYS_INVALID_M4UINT32);


/*************************************************************************

NAME: FormatErrorStringByParams

**************************************************************************

ABSTRACT: 
	Parse an error string and a param array.

ARGUMENTS:

	m4pchar_t ai_pszErrorString (input)
		An string with the error string. The argument placeholder is like 
		%param_num:param_type. param_num is the number of argument. param_type
		can be s for string, d for dates and n for numbers.

	m4VariantType *ai_pvParams (input)
		Params of the error.

	m4uint32_t ai_uParams (input)
		Number of params of the error.

	m4uchar_t ai_byIdParamPos (input)
		When using pos params, a param is like 
		#pos0&&pos1&&...&&posn#. This param chooses the position. 
		If pos doesn't exist, it takes first pos. If not using positions
		params, use 0.

	m4pchar_t ao_pszOutString (output)
		Parsed string. It can be NULL if you just want to know
		the size of the output string. 

	m4uint32_t ai_uBufferSize (input)
		Size of ao_pvParams array. If ao_pszOutString is NULL, this argument
		is ignored.

	m4puint32_t ao_uBufferFilled (output)
		Number of chars (last NULL included) found. 
		It can be NULL if you don't need it.

	m4language_t ai_uiLang (input)
		Language for date format
		
RETURN:
	m4return_t 
		M4_ERROR 
			If ai_uBufferSize=0 when ao_pszOutString!=NULL because we
			don't have room for even a NULL.
		
		M4_WARNING
			If there is no room for the output string

NOTES:
	When ao_pszOutString is NULL, ao_uBufferFilled returns the EXACT size 
	needed.
	If some non-empty param is not used, it is appened at the end of the
	parsed string with a newline separator.

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t FormatErrorStringByParams (
	m4pchar_t ai_pszErrorString,  
	m4VariantType *ai_pvParams,
	m4uint32_t ai_uParams,
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang);


/*************************************************************************

NAME: SerializeParams

**************************************************************************

ABSTRACT: 
	Convert an array of params into a string.

ARGUMENTS:
	m4VariantType *ai_pvParams (input)
		Array with params

	m4uint32_t ai_uNumParams (input)
		Size of ai_pvParams array.

	m4uint32_t ai_uSource (input)
		Source text, normally 1. It will append *S1* as first param in
		the string.
		
	m4pchar_t ao_pszOutString (output)
		String to get the params. It can be NULL if you just want to know
		the size of the output string. 
  
	m4uint32_t ai_uBufferSize (input)
		Size of ao_pszOutString. If ao_pszOutString is NULL, 
		this argument is ignored.

	m4puint32_t ao_uBufferFilled) (output)
		Number of chars (last NULL included) found. 
		It can be NULL if you don't need it.

RETURN:
	m4return_t 
		M4_ERROR 
			If ai_uBufferSize=0 when ao_pszOutString!=NULL because we
			don't have room for even a NULL.

			If an argument isn't neither M4CL_CPP_TYPE_NUMBER, 
			M4CL_CPP_TYPE_DATE, M4CL_CPP_TYPE_STRING_VAR nor 
			M4CL_CPP_TYPE_NULL.
		
		M4_WARNING
			If there is no room for all the params found. 

NOTES:
	It returns something like #backupsource#param1#param2...#paramn.
	When ao_pszOutString is NULL, it returns the EXACT size needed.

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t SerializeParams(
	m4VariantType *ai_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4uint32_t ai_uSource,	
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_puBufferFilled);


/*************************************************************************

NAME: DeserializeParams

**************************************************************************

ABSTRACT: 
	Convert an string into an array of params.

ARGUMENTS:
	m4pchar_t aio_ppszInString (input / output)
		A pointer to a string like #...#...#...# with the params string.
		If the string is like #...#||#...#, it will point to the char
		following ||, or will be NULL if no more || are left

	m4uchar_t ai_byIdParamPos (input)
		When using params pos, a param is like 
		#pos0&&pos1&&...&&posn#. This param chooses the position. 
		If pos doesn't exist, it takes first pos. If not using positions
		params, use 0.
	  
	m4VariantType *ao_pvParams (output)
		Array to get the params. It can be NULL if you just want to know
		the number of arguments.
	
	m4uint32_t ai_uNumParams (input)
		Size of ao_pvParams array. If ao_pvParams is NULL, this argument
		is ignored.

	m4puint32_t ao_uParamsFilled (output)
		Number of params found. It can be NULL if you don't need it.

	m4puint32_t ao_puSource (output)
		Level of source found, i.e. *Sx*. It can be NULL if you don't need it.

	m4puint32_t ao_puError (output)
		Number source found, i.e. *Nxxxx*. It can be NULL if you don't need it.

	m4puint32_t ao_puExtra (output)
		Number of extra found, i.e. *Ex*. It can be NULL if you don't need it.

	m4uchar_t ai_byParseOption (input)
		This is an optimization params. It can be:
		LOGSYS_PARSE_ALWAYS	0	// Normal behavior
		LOGSYS_PARSE_ERROR	1	// Parse only if it is an error (i.e. not an extra)
		LOGSYS_PARSE_EXTRA	2	// Parse only if it is an extra (i.e. not an error)
		Don't parse means ao_pvParams, ao_uNumParams and ao_uParamsFilled 
		are ignored. Only ao_puSource, ao_puError and ao_puExtra are defined.

RETURN:
	m4return_t 
		M4_ERROR 
			Never returned
		
		M4_WARNING
			If there is no room for all the params found. 

NOTES:
	If some of ao_puSource, ao_puError and/or ao_puExtra are not found
	its returned value is LOGSYS_INVALID_M4UINT32 (0xFFFFFFFF).

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t DeserializeParams(
	m4pchar_t *ai_ppszInString,		
	m4uchar_t ai_byIdParamPos,		
	m4VariantType *ao_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4puint32_t ao_puParamsFilled,
	m4puint32_t ao_puSource=NULL,
	m4puint32_t ao_puError=NULL,
	m4puint32_t ao_puExtra=NULL,
	m4uchar_t ai_byParseOption=LOGSYS_PARSE_ALWAYS);



/*************************************************************************

NAME: FormatErrorExtraArray

**************************************************************************

ABSTRACT: 
	Get extras of a param string.

ARGUMENTS:
	m4pchar_t ai_pszParams (input)
		The param string (see DeserializeParams for more info).
 
	pairFormatErrorExtra_t *ao_pExtra (output)
		It can be NULL if you just want to know the number of extras.
		m4puint32_t ao_pExtra.uExtraType
			Array to get the extra types. 
		m4puint32_t ao_pExtra.uExtraHandle
			Array to get the extra handles. 
	  
	m4uint32_t ai_uNumExtras (input)
		Size of ai_puExtraType and ai_puExtraHandle. If both of them are NULL,
		it is ignored.

	m4puint32_t ao_puExtrasFilled (output)
		Number of extras found. It can be NULL if you don't need it.

RETURN:
	m4return_t 
		M4_ERROR 
			Never returned
		
		M4_WARNING
			If there is no room for all the extras found. 

NOTES:

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t FormatErrorExtraArray (
	m4pchar_t ai_pszParams,			
	pairFormatErrorExtra_t *ao_pExtra,
	m4uint32_t ai_uNumExtras,		
	m4puint32_t ao_puExtrasFilled);


/*************************************************************************

NAME: FormatErrorExtraParams

**************************************************************************

ABSTRACT: 
	Get the params of an extra.

ARGUMENTS:
	m4pchar_t ai_pszParams (input)
		The param string (see DeserializeParams for more info).

	m4uint32_t ai_uExtraHandle (input)
		Handle of the extra (see FormatErrorExtraArray).

	m4uchar_t ai_byIdParamPos (input)
		When using params pos, a param is like 
		#pos0&&pos1&&...&&posn#. This param chooses the position. 
		If pos doesn't exist, it takes first pos. If not using positions
		params, use 0.

	m4VariantType *ao_pvParams (output)
		Array to get the params. It can be NULL if you just want to know
		the number of arguments.
	
	m4uint32_t ai_uNumParams (input)
		Size of ao_pvParams array. If ao_pvParams is NULL, this argument
		is ignored.

	m4puint32_t ao_uParamsFilled (output)
		Number of params found. It can be NULL if you don't need it.

RETURN:
	m4return_t 
		M4_ERROR 
			Never returned
		
		M4_WARNING
			If there is no room for all the params found. 

NOTES:

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t FormatErrorExtraParams (
	m4pchar_t ai_pszParams,			
	m4uint32_t ai_uExtraHandle,
	m4uchar_t ai_byIdParamPos,		
	m4VariantType *ao_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4puint32_t ao_puParamsFilled);



/*************************************************************************

NAME: LogsysSetWorkingDir

**************************************************************************

ABSTRACT: 
	Set the working dir to look for logmsg.ini file.

ARGUMENTS:
	m4pcchar_t ai_pszWorkDir (input)
		The workdir name.

NOTES:
	Can't be called in the constructor of a global member.

*************************************************************************/
M4_DECL_M4LOGSYS void LogsysSetWorkingDir(m4pcchar_t ai_pszWorkDir);


/*************************************************************************

NAME: LogsysSetLangId

**************************************************************************

ABSTRACT: 
	Set the language id to look for logmsg.ini file.

ARGUMENTS:
	m4uint32_t ai_uLangId (input)
		The language id.

NOTES:
	Can't be called in the constructor of a global member.

*************************************************************************/
M4_DECL_M4LOGSYS void LogsysSetLangId(m4uint32_t ai_uLangId);



/*************************************************************************

NAME: LogsysGetCodeString

**************************************************************************

ABSTRACT: 
	Search an error code in logmsg.ini.

ARGUMENTS:

	m4uint32_t ai_ulCode	(input)
		Error code to search

	m4pchar_t ao_pszOutString (output)
		Output buffer

	m4uint32_t ai_uBufferSize (input)
		Size of ao_pszOutString. If ao_pszOutString is NULL, 
		this argument is ignored.

	m4puint32_t ao_uBufferFilled (output)
		Number of chars (last NULL included) found. 
		It can be NULL if you don't need it.

RETURN:
	m4return_t 
		M4_ERROR 
			Error code not found
		
		M4_WARNING
			If there is no room for all the params found. 

NOTES:

*************************************************************************/
M4_DECL_M4LOGSYS m4return_t LogsysGetCodeString(
	m4uint32_t ai_ulCode,			// Error code
	m4pchar_t ao_pszOutString,		// Output buffer
	m4uint32_t ai_uBufferSize,		// Buffer Len
	m4puint32_t ao_uBufferFilled,	// Bytes written
	m4language_t ai_LanguageId = LOGSYS_INVALID_M4UINT32 // Language Id
	);



M4_DECL_M4LOGSYS m4bool_t LogsysAddLanguage( m4language_t ai_iLanguage, m4LCID_t ai_iLCID, m4pcchar_t ai_pccName, m4pcchar_t ai_pccErrorDateFormat, m4pcchar_t ai_pccErrorTimeFormat ) ;


#endif // __M4LOGSYS_HPP__
