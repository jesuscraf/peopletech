//==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 File:              m4versioninfo.hpp
//	 Project:           m4versioninfo
//	 Author:            Meta Software M.S., S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX (HP, DEC, AIX, SUN)
//	 Design Document:
//
//	 Definition:
//
//   Compilation/link options:
//		WinNt:	libraries: version.lib
//		UNIX:	You need compile the resource file with the
//			perl script rccompiler, that you can find in this
//			component.
//			Read documentation file to get more info.
//
//	Samples:
//		const M4_VS_VERSION_INFO* vi=NULL;
//		const char* value1=NULL;
//		char *filename = "myfile";
//
//		vi is the variable where you get the version info.
//		filename is the name of file you want get version info,
//			without extension.
//		M4GetComponentVersionInformation(vi, fileName);
//		if (NULL != vi)
//			cout << vi << endl;
//
//		value1 = vi->getVersionProperty("CompanyName");
//		if (NULL != value1)
//			cout << value1 << endl;
//
//	CAUTION:
//		The 'M4GetComponentVersionInformation' method is 
//		implemented, in the UNIX version, as a macro.
//		It's very important that you use this methotd like in
//		the above example.
//
//		In Windows, 'filename', is the name of the library or
//		executable that you want to extract version information.
//		In Unix, 'filename' is the word that you have specified
//		to the resource compiler script.
//
//		Read documentation file for additional info.
//
//=============================================================================

#ifndef _M4RESOURCES_HPP_
#define _M4RESOURCES_HPP_

//----------------------------------------------------------------
// Check for a valid platform.
//----------------------------------------------------------------
#if defined (_WINDOWS)
#	define M4_WINDOWS_RESOURCES
#elif defined (_UNIX)
#	define M4_UNIX_RESOURCES
#else
#	error "Platform not supported!!"
#endif

//----------------------------------------------------------------
// Platform dependencies.
//----------------------------------------------------------------
#if defined (M4_WINDOWS_RESOURCES)
#	include <wtypes.h>
#	include <winver.h>

#elif defined (M4_UNIX_RESOURCES)

#else
#	error "Platform not supported!!"
#endif

//----------------------------------------------------------------
// global includes.
//----------------------------------------------------------------
#include <stdio.h>
#include "m4stl.hpp"


//===========================================================================
// Class: M4_VS_FIXEDFILEINFO.
// Definition: This class store the fixed file version information, in
//	the versioninfo resource.
//===========================================================================
class M4_VS_FIXEDFILEINFO	
{
private:
	M4_VS_FIXEDFILEINFO() {}
public:
	friend ostream& operator<<(ostream &aio_os, const M4_VS_FIXEDFILEINFO* ai_vi_fixedfileinfo);
};


//===========================================================================
// Class: M4_VS_VERSION_INFO.
// Definition: This class store the file version information, in
//	the versioninfo resource.
//===========================================================================
class M4_VS_VERSION_INFO	
{
private:
	M4_VS_VERSION_INFO() {}
	const char* GetTranslationCode(const void* ai_dwLang, const void* ai_dwCodePage) const;
public:
	friend ostream& operator<<(ostream &aio_os, const M4_VS_VERSION_INFO* ai_vs_version_info);
	const char *getVersionProperty(const char *ai_property, const char *ai_language=NULL, const char *ai_codepage=NULL) const;
};


//===========================================================================
// Class: M4VersionInfo.
// Definition: This class provides methods to get information from 
//	versioninfo resource.
//===========================================================================
#if defined (M4_WINDOWS_RESOURCES)

//===========================================================================
// function: M4GetComponentVersionInformation.
//===========================================================================
void M4GetComponentVersionInformation(const char *&ao_versionInfo, const char* ai_filename);


#elif defined (M4_UNIX_RESOURCES)

//===========================================================================
// function: M4GetComponentVersionInformation.
// description: In Unix platforms, this function is implementes as a macro.
//	Read in the header of this file, how use it.
//	The macro implementation is in the file m4vi_unix.
//===========================================================================

#else
#	error "Platform not supported!!"
#endif


//===========================================================================
// Class Implementation.
// The implemetation for the above classes are inline methods.
//===========================================================================
#if defined (M4_WINDOWS_RESOURCES)
#	include "m4vi_nt.hpp"
#elif defined (M4_UNIX_RESOURCES)
#	include "m4vi_unix.hpp"
#else
#	error "Platform not supported!!"
#endif


#endif // _M4RESOURCES_HPP_
