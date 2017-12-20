//## begin module%3958D853011E.cm preserve=no
//## end module%3958D853011E.cm

//## begin module%3958D853011E.cp preserve=no
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
//## end module%3958D853011E.cp

//## Module: M4XmlOutStream%3958D853011E; Package specification
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Source file: D:\INTEGRATION\m4xmladminclient\inc\m4xmloutstream.hpp

#ifndef m4xmloutstream_h
#define m4xmloutstream_h 1

//## begin module%3958D853011E.additionalIncludes preserve=no
//## end module%3958D853011E.additionalIncludes

//## begin module%3958D853011E.includes preserve=yes
#include "m4xmladminclient_dll.hpp"
#include "basiclog.hpp"
#include "m4srvres.hpp"
//## end module%3958D853011E.includes

// xmlstreamgenerator
#include <xmlstreamgenerator.hpp>
//## begin module%3958D853011E.declarations preserve=no
//## end module%3958D853011E.declarations

//## begin module%3958D853011E.additionalDeclarations preserve=yes
//## end module%3958D853011E.additionalDeclarations


//## begin ClXMLOutStream%3958D4EB00A2.preface preserve=yes
//## end ClXMLOutStream%3958D4EB00A2.preface

//## Class: ClXMLOutStream%3958D4EB00A2
//## Category: M4XMLAdminClient%39084C2003A4
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4XMLADMINCLIENT ClXMLOutStream : public M4XMLStreamGenerator  //## Inherits: <unnamed>%3958D56E03A3
{
  //## begin ClXMLOutStream%3958D4EB00A2.initialDeclarations preserve=yes
  //## end ClXMLOutStream%3958D4EB00A2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClXMLOutStream%962275920
      ClXMLOutStream (m4string_t ai_strQuote = "\"");

    //## Destructor (specified)
      //## Operation: ~ClXMLOutStream%962275921
      ~ClXMLOutStream ();


    //## Other Operations (specified)
      //## Operation: ConfigurationsCommand%962124054
      m4return_t ConfigurationsCommand (m4string_t ai_strUser, m4string_t ai_strPassword);

      //## Operation: StartServerCommand%962124055
      m4return_t StartServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort = "");

      //## Operation: StopServerCommand%962124056
      m4return_t StopServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort = "");

      //## Operation: StopDispatcherCommand%974134062
      m4return_t StopDispatcherCommand (m4string_t ai_strUser, m4string_t ai_strPassword);

	  m4return_t RemoveServerCommand (m4string_t ai_strHost, m4string_t ai_strConfiguration, m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t ai_strBasePort = "");

    // Additional Public Declarations
      //## begin ClXMLOutStream%3958D4EB00A2.public preserve=yes
      //## end ClXMLOutStream%3958D4EB00A2.public

  protected:
    // Additional Protected Declarations
      //## begin ClXMLOutStream%3958D4EB00A2.protected preserve=yes
      //## end ClXMLOutStream%3958D4EB00A2.protected

  private:
    // Additional Private Declarations
      //## begin ClXMLOutStream%3958D4EB00A2.private preserve=yes
	  m4return_t BuildToken(m4string_t ai_strUser, m4string_t ai_strPassword, m4string_t & ao_strToken);
      //## end ClXMLOutStream%3958D4EB00A2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClXMLOutStream%3958D4EB00A2.implementation preserve=yes
      //## end ClXMLOutStream%3958D4EB00A2.implementation

};

//## begin ClXMLOutStream%3958D4EB00A2.postscript preserve=yes
//## end ClXMLOutStream%3958D4EB00A2.postscript

// Class ClXMLOutStream 

//## begin module%3958D853011E.epilog preserve=yes
//## end module%3958D853011E.epilog


#endif
