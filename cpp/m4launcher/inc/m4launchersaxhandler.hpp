//## begin module%3905596200BE.cm preserve=no
//## end module%3905596200BE.cm

//## begin module%3905596200BE.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%3905596200BE.cp

//## Module: m4launchersaxhandler%3905596200BE; Package specification
//## Subsystem: M4Launcher::inc%390558BC008D
//## Source file: F:\integration\m4launcher\inc\m4launchersaxhandler.hpp

#ifndef m4launchersaxhandler_h
#define m4launchersaxhandler_h 1

//## begin module%3905596200BE.additionalIncludes preserve=no
//## end module%3905596200BE.additionalIncludes

//## begin module%3905596200BE.includes preserve=yes
#include <m4types.hpp>
//## end module%3905596200BE.includes

// m4xmlbase
#include <m4xmlbase.hpp>
//## begin module%3905596200BE.declarations preserve=no
//## end module%3905596200BE.declarations

//## begin module%3905596200BE.additionalDeclarations preserve=yes
//## end module%3905596200BE.additionalDeclarations


//## begin M4LauncherSAXHandler%3906C21A0115.preface preserve=yes
//## end M4LauncherSAXHandler%3906C21A0115.preface

//## Class: M4LauncherSAXHandler%3906C21A0115
//## Category: M4Launcher%390555CF02DF
//## Subsystem: M4Launcher::inc%390558BC008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4LauncherSAXHandler : public M4SAXHandlerBase  //## Inherits: <unnamed>%3906C24D000A
{
  //## begin M4LauncherSAXHandler%3906C21A0115.initialDeclarations preserve=yes
  //## end M4LauncherSAXHandler%3906C21A0115.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4LauncherSAXHandler%956741692
      M4LauncherSAXHandler ();

    //## Destructor (specified)
      //## Operation: ~M4LauncherSAXHandler%956741693
      ~M4LauncherSAXHandler ();


    //## Other Operations (specified)
      //## Operation: procDispatcher%956741694
      void procDispatcher (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procLauncher%956741695
      void procLauncher (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procServer%956741696
      void procServer (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procStartup%956741697
      void procStartup (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procUser%956741698
      void procUser (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procParams%956741699
      void procParams (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procServerPath%956741700
      void procServerPath (const XMLCh* const name, AttributeList* attributes);

      //## Operation: procValue%956746927
      void procValue (const char *ai_szName, const XMLCh *ai_szValue);

      //## Operation: procCommand%960883631
      void procCommand (const XMLCh* const name, AttributeList* attributes);

    // Additional Public Declarations
      //## begin M4LauncherSAXHandler%3906C21A0115.public preserve=yes
      //## end M4LauncherSAXHandler%3906C21A0115.public

  protected:
    // Additional Protected Declarations
      //## begin M4LauncherSAXHandler%3906C21A0115.protected preserve=yes
      //## end M4LauncherSAXHandler%3906C21A0115.protected

  private:
    // Additional Private Declarations
      //## begin M4LauncherSAXHandler%3906C21A0115.private preserve=yes
      //## end M4LauncherSAXHandler%3906C21A0115.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4LauncherSAXHandler%3906C21A0115.implementation preserve=yes
      //## end M4LauncherSAXHandler%3906C21A0115.implementation

};

//## begin M4LauncherSAXHandler%3906C21A0115.postscript preserve=yes
//## end M4LauncherSAXHandler%3906C21A0115.postscript

// Class M4LauncherSAXHandler 

//## begin module%3905596200BE.epilog preserve=yes
//## end module%3905596200BE.epilog


#endif
