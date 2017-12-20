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


#ifndef	_M4MODKEY_VERSION_HPP_
#define _M4MODKEY_VERSION_HPP_

#include "m4stl.hpp"
#include "globdec.hpp"

//---------------------------------------------------------------
//Versionado de licencias.
const m4char_t M4MODKEY_LICENSEVERSION_VERSION_1[]			= "1.0";
const m4char_t M4MODKEY_LICENSEVERSION_VERSION_2[]			= "2.0";

const m4char_t M4MODKEY_LICENSEVERSION_DEFAULT[]			= "2.0";
//---------------------------------------------------------------


//---------------------------------------------------------------
//Versionado de generacion.
const m4char_t M4MODKEY_FILEVERSION_OBJMOD[]				= "000.000.001";
const m4char_t M4MODKEY_FILEVERSION_LICENSE_INFO[]			= "000.000.001";
const m4char_t M4MODKEY_FILEVERSION_LICENSE[]				= "000.000.001";
//---------------------------------------------------------------

//----------------------------------------------
//Clase ClVersion
//----------------------------------------------
class ClVersion {
public:
	static string GetFileVersionLicense( void );
	static string GetFileVersionObjMod( void );

	static m4bool_t ValidateFileVersionLicense( const string & ai_sVersion );
	static m4bool_t ValidateFileVersionObjMod( const string & ai_sVersion );
};

//----------------------------------------------
//FIN Clase ClVersion
//----------------------------------------------

#endif


