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

#include "version.hpp"

//----------------------------------------------
//Clase ClVersion
//----------------------------------------------
string ClVersion::GetFileVersionLicense( void )
{
	string sFileVersion(M4MODKEY_FILEVERSION_LICENSE);
	return sFileVersion;
}

string ClVersion::GetFileVersionObjMod( void )
{
	string sFileVersion(M4MODKEY_FILEVERSION_OBJMOD);
	return sFileVersion;
}

m4bool_t ClVersion::ValidateFileVersionLicense( const string & ai_sVersion )
{
	if (ai_sVersion.compare(ClVersion::GetFileVersionLicense()) == 0) {
		return M4_TRUE;
	}
	return M4_FALSE;
}

m4bool_t ClVersion::ValidateFileVersionObjMod( const string & ai_sVersion )
{
	if (ai_sVersion.compare(ClVersion::GetFileVersionObjMod()) == 0) {
		return M4_TRUE;
	}
	return M4_FALSE;
}
//----------------------------------------------
//FIN Clase ClVersion
//----------------------------------------------
