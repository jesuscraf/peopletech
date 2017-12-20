//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4serverversion.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:9/26/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines a function thats return the
//		Meta4AppServer version.
//
//==============================================================================


#include "m4string.hpp"
#include "m4versioninfo.hpp"


static const char* m4server = "m4server";
static const char* m4buildversion = "m4buildversion";


void GetM4AppServerVersion(M4ClString& ao_appServerVersion)
{	
	const M4_VS_VERSION_INFO* versionInfo = NULL;
	const char* propertyValue = NULL;

	// initizalize version.
	ao_appServerVersion = M4ClString("");

	// get versionInfo from server executables files.
	M4GetComponentVersionInformation(versionInfo, m4server);
	if (NULL == versionInfo)
	{
		return;
	}

	propertyValue = versionInfo->getVersionProperty("FileVersion");
	if (NULL != propertyValue)
	{
		ao_appServerVersion = M4ClString(propertyValue);
	}

	return;
}


void GetWelcomeMessage(M4ClString& ao_welcome)
{
	M4ClString WelcomeMessage;

	// get appServerVersion.
	M4ClString appServerVersion;
	GetM4AppServerVersion(appServerVersion);

	// get versionInfo from server executables files.
	const M4_VS_VERSION_INFO* versionInfo = NULL;
	const char* propertyValue = NULL;
	M4GetComponentVersionInformation(versionInfo, m4server);
	if (NULL == versionInfo)
	{
		WelcomeMessage = M4ClString("Meta4 Application Server.");
		WelcomeMessage += M4ClString("\n");
		WelcomeMessage += M4ClString("Copyright (c) Meta4 Spain, S.A.");

		ao_welcome = M4ClString(WelcomeMessage);
		return;
	}

	// get ProductName.
	propertyValue = versionInfo->getVersionProperty("ProductName");
	if (NULL == propertyValue)
	{
		WelcomeMessage = M4ClString("Meta4 Application Server.");
	}
	else
	{
		WelcomeMessage = M4ClString(propertyValue);
	}

	// get ProductVersion.
	/*
	if ((*M4_SERVER_LOCAL_VERSION_STR) == '\0') {
		propertyValue = versionInfo->getVersionProperty("ProductVersion");
		if (NULL != propertyValue)
		{
			WelcomeMessage += M4ClString(" - Version ");
			WelcomeMessage += M4ClString(propertyValue);
		}
	}
	else {
		WelcomeMessage += M4ClString(" - Version ");
		WelcomeMessage += M4_SERVER_LOCAL_VERSION_STR;
	}
	*/

	// get LegalCopyright.
	M4GetComponentVersionInformation(versionInfo, m4server);
	if (NULL != versionInfo)
	{
		propertyValue = versionInfo->getVersionProperty("LegalCopyright");
		if (NULL != propertyValue)
		{
			WelcomeMessage += M4ClString("\n");
			WelcomeMessage += M4ClString("Copyright ");
			WelcomeMessage += M4ClString(propertyValue);
			WelcomeMessage += M4ClString(" ");
		}
	}

	// get CompanyName.
	propertyValue = versionInfo->getVersionProperty("CompanyName");
	if (NULL != propertyValue)
	{
		WelcomeMessage += M4ClString(propertyValue);
	}
	WelcomeMessage += M4ClString("\n");
	WelcomeMessage += M4ClString("All Rights Reserved");

	/*
	// add appServerVersion
	WelcomeMessage += M4ClString("ServerBuild: ");
	WelcomeMessage += appServerVersion;

#ifdef _WINDOWS
	propertyValue = versionInfo->getVersionProperty("Internal Build");
	if (NULL != propertyValue)
	{
		WelcomeMessage += M4ClString(" (Build: ");
		WelcomeMessage += M4ClString(propertyValue);
		WelcomeMessage += M4ClString(")");
	}
	WelcomeMessage += M4ClString("\n");

	// get Meta4AppServer ID.
	M4GetComponentVersionInformation(versionInfo, m4buildversion);
	if (NULL != versionInfo)
	{
		propertyValue = versionInfo->getVersionProperty("SpecialBuild");
		if (NULL != propertyValue)
		{
			WelcomeMessage += M4ClString("Application Server Identifier: ");
			WelcomeMessage += M4ClString(propertyValue);
		}
		
	}
	WelcomeMessage += M4ClString("\n");
#endif
	*/

	WelcomeMessage += M4ClString("\n");
	WelcomeMessage += M4ClString("\n");
	ao_welcome = M4ClString(WelcomeMessage);
}



