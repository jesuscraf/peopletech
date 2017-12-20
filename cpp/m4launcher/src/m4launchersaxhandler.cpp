//## begin module%390559630264.cm preserve=no
//## end module%390559630264.cm

//## begin module%390559630264.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%390559630264.cp

//## Module: m4launchersaxhandler%390559630264; Package body
//## Subsystem: M4Launcher::src%390558C803B5
//## Source file: F:\integration\m4launcher\src\m4launchersaxhandler.cpp

//## begin module%390559630264.additionalIncludes preserve=no
//## end module%390559630264.additionalIncludes

//## begin module%390559630264.includes preserve=yes
#include <clappinfo.hpp>
//## end module%390559630264.includes

// m4launchersaxhandler
#include <m4launchersaxhandler.hpp>
// m4xmlbase
#include <m4xmlbase.hpp>
#include "m4unicode.hpp"
//## begin module%390559630264.declarations preserve=no
//## end module%390559630264.declarations

//## begin module%390559630264.additionalDeclarations preserve=yes
//## end module%390559630264.additionalDeclarations


// Class M4LauncherSAXHandler 

M4LauncherSAXHandler::M4LauncherSAXHandler ()
  //## begin M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.hasinit preserve=no
  //## end M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.hasinit
  //## begin M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.initialization preserve=yes
  //## end M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.initialization
{
  //## begin M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.body preserve=yes
	 
	RegisterElementProc ( "dispatcher" , (ElementProcStart)&M4LauncherSAXHandler::procDispatcher );
	RegisterElementProc ( "launcher" , (ElementProcStart)&M4LauncherSAXHandler::procLauncher );
	RegisterElementProc ( "process" , (ElementProcStart)&M4LauncherSAXHandler::procServer );
	RegisterElementProc ( "startup" , (ElementProcStart)&M4LauncherSAXHandler::procStartup );
	RegisterElementProc ( "params" , (ElementProcStart)&M4LauncherSAXHandler::procParams );
	RegisterElementProc ( "user" , (ElementProcStart)&M4LauncherSAXHandler::procUser );
	RegisterElementProc ( "process_path" , (ElementProcStart)&M4LauncherSAXHandler::procServerPath );
	RegisterElementProc ( "command" , (ElementProcStart)&M4LauncherSAXHandler::procCommand );
  //## end M4LauncherSAXHandler::M4LauncherSAXHandler%956741692.body
}


M4LauncherSAXHandler::~M4LauncherSAXHandler ()
{
  //## begin M4LauncherSAXHandler::~M4LauncherSAXHandler%956741693.body preserve=yes
  //## end M4LauncherSAXHandler::~M4LauncherSAXHandler%956741693.body
}



//## Other Operations (implementation)
void M4LauncherSAXHandler::procDispatcher (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procDispatcher%956741694.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "host", attributes->getValue( "host" ) ) ;
	procValue( "port",  attributes->getValue( "port" ) ) ;
	procValue( "slapsedtime", attributes->getValue( "slapsedtime" ) ) ;
  //## end M4LauncherSAXHandler::procDispatcher%956741694.body
}

void M4LauncherSAXHandler::procLauncher (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procLauncher%956741695.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "launcherpath", attributes->getValue( "bin" ) ) ;
  //## end M4LauncherSAXHandler::procLauncher%956741695.body
}

void M4LauncherSAXHandler::procServer (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procServer%956741696.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "name", attributes->getValue( "name" ) ) ;
	procValue( "bin", attributes->getValue( "bin" ) ) ;
	procValue( "parameters", attributes->getValue( "parameters" ) ) ;
  //## end M4LauncherSAXHandler::procServer%956741696.body
}

void M4LauncherSAXHandler::procStartup (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procStartup%956741697.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "startupobl" ,attributes->getValue( "startupobl" ) ) ;
	procValue( "run_mode", attributes->getValue( "run_mode" ) ) ;
  //## end M4LauncherSAXHandler::procStartup%956741697.body
}

void M4LauncherSAXHandler::procUser (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procUser%956741698.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "user", attributes->getValue( "username" ) ) ;
	procValue( "password", attributes->getValue( "password" ) ) ;
  //## end M4LauncherSAXHandler::procUser%956741698.body
}

void M4LauncherSAXHandler::procParams (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procParams%956741699.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue( "password", attributes->getValue( "password" ) ) ;
	procValue( "user", attributes->getValue( "user" ) ) ;
	procValue( "baseport", attributes->getValue( "baseport" ) ) ;
  //## end M4LauncherSAXHandler::procParams%956741699.body
}

void M4LauncherSAXHandler::procServerPath (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procServerPath%956741700.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	ClAppInfo* appinfo = ClAppInfo::Instance () ;

	m4char_t path [1000] ;

	if (! attributes->getValue ( "path" ) )
	{
		return ;
	}

	// UNICODE XML
	string sPath;
	M4XMLToSTL( (M4XMLCh*)attributes->getValue( "path" ), sPath ) ;

	strcpy( path, sPath.c_str() ) ;

	appinfo->Parse( path, 1000 ) ;

	procValue( "path", attributes->getValue( "path" ) ) ;
  //## end M4LauncherSAXHandler::procServerPath%956741700.body
}

// bg 0129818
void M4LauncherSAXHandler::procValue (const char *ai_szName, const XMLCh *ai_szValue)
{
  //## begin M4LauncherSAXHandler::procValue%956746927.body preserve=yes
	ClAppInfo * appinfo = ClAppInfo::Instance() ;

	// UNICODE XML
	string sValue;

	M4XMLToSTL( (M4XMLCh*)ai_szValue, sValue ) ;

	appinfo->SetValue( ai_szName , (char*) sValue.c_str() );
  //## end M4LauncherSAXHandler::procValue%956746927.body
}

void M4LauncherSAXHandler::procCommand (const XMLCh* const name, AttributeList* attributes)
{
  //## begin M4LauncherSAXHandler::procCommand%960883631.body preserve=yes
	m4uint32_t len ;
	if (! ( len = attributes -> getLength() ) )
	{
		return ;
	}

	// UNICODE XML
	procValue ( "command", attributes->getValue( "id" ) ) ;
  //## end M4LauncherSAXHandler::procCommand%960883631.body
}

// Additional Declarations
  //## begin M4LauncherSAXHandler%3906C21A0115.declarations preserve=yes
  //## end M4LauncherSAXHandler%3906C21A0115.declarations

//## begin module%390559630264.epilog preserve=yes
//## end module%390559630264.epilog
