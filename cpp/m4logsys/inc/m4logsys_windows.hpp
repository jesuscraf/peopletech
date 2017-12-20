//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4logsys_win32.hpp
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
//    Error Format (Win32)
////
//==============================================================================

#ifndef __M4LOGSYS_WIN32_HPP__
#define __M4LOGSYS_WIN32_HPP__

#ifdef _WINDOWS

#include <windows.h>
#include "m4logsys_dll.hpp"



///////////////////////////////////////////////////////////////////////
/////////////////                                //////////////////////
///////////////// Visual Basic Wrapper Functions //////////////////////
/////////////////                                //////////////////////
///////////////////////////////////////////////////////////////////////

/* 

	FormatError... Functions:

	ErrorCode or ErrorStr are the error to parse.
	ParamsArray or ParamsStr are the params to parse.

	pos_params: When using pos params, a param is like #pos0&&pos1&&...&&posn#. 
	This param chooses the position. If pos doesn't exist, it takes first pos. 
	If not using params position, use 0.

	OutParams is the output string. It must be like Dim a as String

	Returns: 
		0	OK
		-1	ERROR 
				There is not enough memory or 
				some param type (in ParamsArray) is unknown.
				ErrorCode not found.
				OutStr is undefined if ERROR.

		1	WARNING
				Never returned			

	Example:

	Dim res As Integer
	Dim Params(3) As Variant
	Dim str As String
	Dim strParams As String
	Dim strError as String

		Params(0) = 10
	Params(1) = "hola1"
	Params(2) = 20
	Params(3) = Now()

	strError="Error %3:d en %0:s"

	res = SerializeParams(Params, 1, strParams)

	res = FormatErrorString(strError, strParams, 0, str)

	res = FormatErrorStringByParams(strError, Params, 0, str)
*/


/*

	Public Declare Function FormatErrorString Lib "m4logsys" Alias 
	"?FormatErrorString_VB@@YGFPAD0EPAPAG@Z" (ByVal ErrorStr As String, 
	ByVal ParamsStr As String, ByVal pos_params As Byte, 
	ByRef OutStr As String) As Integer

*/
M4_DECL_M4LOGSYS m4int16_t WINAPI FormatErrorString_VB (
	char *ai_pszErrorString,			
	char *ai_pszParams,
	m4uchar_t ai_byIdParamPos,		
	BSTR *ao_pszOutString);

/* 

	Public Declare Function FormatErrorStringByParams Lib "m4logsys" Alias 
	"?FormatErrorStringByParams_VB@@YGFPADPAPAUtagSAFEARRAY@@EPAPAG@Z" 
	(ByVal ErrorStr As String, ByRef ParamsArray() As Variant, 
	ByVal pos_params As Byte, ByRef OutStr As String) As Integer

*/
M4_DECL_M4LOGSYS m4int16_t WINAPI FormatErrorStringByParams_VB (
	char *ai_pszErrorString,			
	SAFEARRAY **ai_ppParams,
	m4uchar_t ai_byIdParamPos,		
	BSTR *ao_pszOutString);


/* 
	Public Declare Function SerializeParams Lib "m4logsys" 
		Alias "?SerializeParams_VB@@YGFPAPAUtagSAFEARRAY@@KPAPAG@Z" 
		(ByRef Params() As Variant, ByVal Source As Long, 
		ByRef OutParams As String) As Integer

	Params is an array of Variants,i.e. Dim params(3) As Variant

	Source is a Long. It will append *S_* as the first param, i.e.
		*S1* for Source=1
  
	OutParams is the output string. It must be like Dim a as String

	Returns: 
		0	OK
		-1	ERROR 
				There is not enough memory or some param type is unknown.
				In this case, OutParams is undefined.

		1	WARNING
				Never returned			

	Example:

		Dim params(3) As Variant
		Dim str As String

		params(0) = 10
		params(1) = "hola1"
		params(2) = 20
		params(3) = Now()

		res = SerializeParams(params, 1, str)


		Result:

			str = "#*S1*#10.00000000#hola1#20.00000000#1999-02-05 13:06:56#"
			res = 0

*/
M4_DECL_M4LOGSYS m4int16_t WINAPI SerializeParams_VB(
	SAFEARRAY **ai_ppParams,
	m4uint32_t uSource,
	BSTR *ao_pszOutString);


/* 
	Public Declare Function DeserializeParams Lib "m4logsys" 
		Alias "?DeserializeParams_VB@@YGFPADPAKEPAPAUtagSAFEARRAY@@111E@Z" 
		(ByVal Params As String, ByRef ParamsControl As Long, 
		ByVal pos_params As Byte, ByRef OutParams() As String, 
		ByRef source_opt As Long, ByRef error_opt As Long, 
		ByRef extra_opt As Long, ByVal parse as byte) As Integer

	Params is a string with the params to deserialize.

	ParamsControl is a long variable. It is must be initialized to 0. When you
	call the function, ParamsControl is adjusted so when you call this
	function again, it deserializes the next set of params (separated by ||)
	If ParamsControl is -1, no more sets of params are left.

	pos_params: When using pos params, a param is like #pos0&&pos1&&...&&posn#. 
	This param chooses the position. If pos doesn't exist, it takes first pos. 
	If not using params position, use 0.

	OutParams is the array with the arguments. The former array is destroyed.
	This array has Lbound 0.
	
	source_opt is a long variable when the source level is returned
	error_opt is a long variable when the extra number is returned
	extra_opt is a long variable when the extra number is returned
	If any of these three variables is not found, its value is -1.

	parse is an optimization argument:
		0 - Arguments are always parsed.
		1 - Only errors sets (of params) are parsed (i.e. not having *Exx*).
		2 - Only extra sets (of params) are parsed (i.e. having *Exx*).
		3 - Arguments are never parsed.
	If parse is not made, OutParams can't be used.


	Returns: 
		0	OK
		-1	ERROR 
				There is not enough memory.

		1	WARNING
				Never returned.

	Example:

		Dim res As Integer
		Dim Params() As String
		Dim str As String
		Dim control As Long
		Dim source_opt As Long
		Dim error_opt As Long
		Dim extra_opt As Long

		control = 0

		str = "#*S1*E10*#10#hola1#vale#||#esto es la#2134#prueba#||#vale#"

		Do While(control<>-1)
			res = DeserializeParams(str, control, 12, Params, 
					source_opt, error_opt, extra_opt, 0)
		Loop
	
*/
M4_DECL_M4LOGSYS m4int16_t WINAPI DeserializeParams_VB(
	char *ai_pszInString,
	m4puint32_t ai_puInState,
	m4uchar_t ai_byIdParamPos,		
	SAFEARRAY **ai_ppParamsOut,
	m4puint32_t ao_puSource,
	m4puint32_t ao_puError,
	m4puint32_t ao_puExtra,
	m4uchar_t ai_byParseOption);




/*
	Set the current workdir to look for the logmsg.ini file.

	Public Declare Sub LogsysSetWorkingDir Lib "m4logsys" Alias 
	"?LogsysSetWorkingDir_VB@@YGXPAD@Z" 
	(ByVal workdir As String)

*/
M4_DECL_M4LOGSYS void WINAPI LogsysSetWorkingDir_VB(char *ai_pszParams);


/*
	Set the current lang id to look for the logmsg.ini file.

	Public Declare Sub LogsysSetLangId Lib "m4logsys" Alias 
	"?LogsysSetLangId_VB@@YGXK@Z" 
	(ByVal langId as long)

*/
M4_DECL_M4LOGSYS void WINAPI LogsysSetLangId_VB(m4uint32_t ai_uLangId);


#endif // _WINDOWS
#endif // __M4LOGSYS_WIN32_HPP__
