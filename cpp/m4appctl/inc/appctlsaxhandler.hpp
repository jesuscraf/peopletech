//## begin module%3907F8F002CE.cm preserve=no
//## end module%3907F8F002CE.cm

//## begin module%3907F8F002CE.cp preserve=no
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
//## end module%3907F8F002CE.cp

//## Module: appctlsaxhandler%3907F8F002CE; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: F:\integration\M4Appctl\inc\appctlsaxhandler.hpp

#ifndef appctlsaxhandler_h
#define appctlsaxhandler_h 1

//## begin module%3907F8F002CE.additionalIncludes preserve=no
//## end module%3907F8F002CE.additionalIncludes

//## begin module%3907F8F002CE.includes preserve=yes
#include <m4types.hpp>
//## end module%3907F8F002CE.includes

// m4xmlbase
#include <m4xmlbase.hpp>
//## begin module%3907F8F002CE.declarations preserve=no
//## end module%3907F8F002CE.declarations

//## begin module%3907F8F002CE.additionalDeclarations preserve=yes
//## end module%3907F8F002CE.additionalDeclarations


//## begin ClAppCtlSAXHandler%3907F3E0004A.preface preserve=yes
//## end ClAppCtlSAXHandler%3907F3E0004A.preface

//## Class: ClAppCtlSAXHandler%3907F3E0004A
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAppCtlSAXHandler : public M4SAXHandlerBase  //## Inherits: <unnamed>%3907F4000295
{
  //## begin ClAppCtlSAXHandler%3907F3E0004A.initialDeclarations preserve=yes
  //## end ClAppCtlSAXHandler%3907F3E0004A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppCtlSAXHandler%957775211
      ClAppCtlSAXHandler ();

    //## Destructor (specified)
      //## Operation: ~ClAppCtlSAXHandler%957775212
      ~ClAppCtlSAXHandler ();


    //## Other Operations (specified)
      //## Operation: procLauncher%956823442
      void procLauncher (const XMLCh* const name, AttributeList *attributes);

      //## Operation: GetLauncherPath%957775213
      m4pchar_t GetLauncherPath ();

      //## Operation: procServer%960832019
      void procServer (const XMLCh* const name, AttributeList *attributes);

      //## Operation: GetServerName%960883629
      m4pchar_t GetServerName ();

      //## Operation: GetRequestID%981474754
      m4uint32_t GetRequestID ();

      //## Operation: procCommand%998555465
      void procCommand (const XMLCh* const name, AttributeList *attributes);

      //## Operation: GetCommand%998555467
      m4string_t GetCommand ();

    // Additional Public Declarations
      //## begin ClAppCtlSAXHandler%3907F3E0004A.public preserve=yes
      //## end ClAppCtlSAXHandler%3907F3E0004A.public

  protected:
    // Additional Protected Declarations
      //## begin ClAppCtlSAXHandler%3907F3E0004A.protected preserve=yes
      //## end ClAppCtlSAXHandler%3907F3E0004A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_szPathLauncher%390802DF036B
      //## begin ClAppCtlSAXHandler::m_szPathLauncher%390802DF036B.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_szPathLauncher;
      //## end ClAppCtlSAXHandler::m_szPathLauncher%390802DF036B.attr

      //## Attribute: m_szServerName%394522420265
      //## begin ClAppCtlSAXHandler::m_szServerName%394522420265.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_szServerName;
      //## end ClAppCtlSAXHandler::m_szServerName%394522420265.attr

      //## Attribute: m_iRequestID%3A80275D036F
      //## begin ClAppCtlSAXHandler::m_iRequestID%3A80275D036F.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iRequestID;
      //## end ClAppCtlSAXHandler::m_iRequestID%3A80275D036F.attr

      //## Attribute: m_szCommand%3B84BF9302B8
      //## begin ClAppCtlSAXHandler::m_szCommand%3B84BF9302B8.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szCommand;
      //## end ClAppCtlSAXHandler::m_szCommand%3B84BF9302B8.attr

    // Additional Private Declarations
      //## begin ClAppCtlSAXHandler%3907F3E0004A.private preserve=yes
      //## end ClAppCtlSAXHandler%3907F3E0004A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppCtlSAXHandler%3907F3E0004A.implementation preserve=yes
      //## end ClAppCtlSAXHandler%3907F3E0004A.implementation

};

//## begin ClAppCtlSAXHandler%3907F3E0004A.postscript preserve=yes
//## end ClAppCtlSAXHandler%3907F3E0004A.postscript

// Class ClAppCtlSAXHandler 

//## begin module%3907F8F002CE.epilog preserve=yes
//## end module%3907F8F002CE.epilog


#endif
