//## begin module%393CFB78009E.cm preserve=no
//## end module%393CFB78009E.cm

//## begin module%393CFB78009E.cp preserve=no
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
//## end module%393CFB78009E.cp

//## Module: M4XMLStreamGen%393CFB78009E; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\m4xmlstreamgen.hpp

#ifndef m4xmlstreamgen_h
#define m4xmlstreamgen_h 1

//## begin module%393CFB78009E.additionalIncludes preserve=no
//## end module%393CFB78009E.additionalIncludes

//## begin module%393CFB78009E.includes preserve=yes
#include <dspanswergen.hpp>
//## begin module%393CFB78009E.declarations preserve=no
//## end module%393CFB78009E.declarations

//## begin module%393CFB78009E.additionalDeclarations preserve=yes
//## end module%393CFB78009E.additionalDeclarations


//## begin ClStarXMLAnswerGen%3972EF210161.preface preserve=yes
//## end ClStarXMLAnswerGen%3972EF210161.preface

//## Class: ClStarXMLAnswerGen%3972EF210161
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStarXMLAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972EFFB0300
{
  //## begin ClStarXMLAnswerGen%3972EF210161.initialDeclarations preserve=yes
  //## end ClStarXMLAnswerGen%3972EF210161.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825081
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.public preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.public

  protected:
    // Additional Protected Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.protected preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.protected

  private:
    // Additional Private Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.private preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.implementation preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.implementation

};

//## begin ClStarXMLAnswerGen%3972EF210161.postscript preserve=yes
class ClAuthXMLAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972EFFB0300
{
  //## begin ClStarXMLAnswerGen%3972EF210161.initialDeclarations preserve=yes
  //## end ClStarXMLAnswerGen%3972EF210161.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825081
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.public preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.public

  protected:
    // Additional Protected Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.protected preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.protected

  private:
    // Additional Private Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.private preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStarXMLAnswerGen%3972EF210161.implementation preserve=yes
      //## end ClStarXMLAnswerGen%3972EF210161.implementation

};

//## end ClStarXMLAnswerGen%3972EF210161.postscript

//## begin ClConfigurationsXMLAnswerGen%3972EF770043.preface preserve=yes
//## end ClConfigurationsXMLAnswerGen%3972EF770043.preface

//## Class: ClConfigurationsXMLAnswerGen%3972EF770043
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConfigurationsXMLAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972EFFE0200
{
  //## begin ClConfigurationsXMLAnswerGen%3972EF770043.initialDeclarations preserve=yes
  //## end ClConfigurationsXMLAnswerGen%3972EF770043.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825083
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClConfigurationsXMLAnswerGen%3972EF770043.public preserve=yes
      //## end ClConfigurationsXMLAnswerGen%3972EF770043.public

  protected:
    // Additional Protected Declarations
      //## begin ClConfigurationsXMLAnswerGen%3972EF770043.protected preserve=yes
      //## end ClConfigurationsXMLAnswerGen%3972EF770043.protected

  private:
    // Additional Private Declarations
      //## begin ClConfigurationsXMLAnswerGen%3972EF770043.private preserve=yes
      //## end ClConfigurationsXMLAnswerGen%3972EF770043.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConfigurationsXMLAnswerGen%3972EF770043.implementation preserve=yes
      //## end ClConfigurationsXMLAnswerGen%3972EF770043.implementation

};

//## begin ClConfigurationsXMLAnswerGen%3972EF770043.postscript preserve=yes
//## end ClConfigurationsXMLAnswerGen%3972EF770043.postscript

//## begin ClCommandCtlGen%3972EF99039F.preface preserve=yes
//## end ClCommandCtlGen%3972EF99039F.preface

//## Class: ClCommandCtlGen%3972EF99039F
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommandCtlGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972F00301B7
{
  //## begin ClCommandCtlGen%3972EF99039F.initialDeclarations preserve=yes
  //## end ClCommandCtlGen%3972EF99039F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommandCtlGen%963825085
      ClCommandCtlGen (m4string_t ai_szCommand, m4uint32_t ai_iRequestID, m4string_t ai_strUserName, m4string_t ai_strPassword);


    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825084
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClCommandCtlGen%3972EF99039F.public preserve=yes
      //## end ClCommandCtlGen%3972EF99039F.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommandCtlGen%3972EF99039F.protected preserve=yes
      //## end ClCommandCtlGen%3972EF99039F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_strCommand%39730CD50210
      //## begin ClCommandCtlGen::m_strCommand%39730CD50210.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strCommand;
      //## end ClCommandCtlGen::m_strCommand%39730CD50210.attr

      //## Attribute: m_iRequestID%3A81327002BA
      //## begin ClCommandCtlGen::m_iRequestID%3A81327002BA.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iRequestID;
      //## end ClCommandCtlGen::m_iRequestID%3A81327002BA.attr

      //## Attribute: m_strUserName%3AC9A44901C6
      //## begin ClCommandCtlGen::m_strUserName%3AC9A44901C6.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strUserName;
      //## end ClCommandCtlGen::m_strUserName%3AC9A44901C6.attr

      //## Attribute: m_strPassword%3AC9A458020D
      //## begin ClCommandCtlGen::m_strPassword%3AC9A458020D.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strPassword;
      //## end ClCommandCtlGen::m_strPassword%3AC9A458020D.attr

    // Additional Private Declarations
      //## begin ClCommandCtlGen%3972EF99039F.private preserve=yes
      //## end ClCommandCtlGen%3972EF99039F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommandCtlGen%3972EF99039F.implementation preserve=yes
      //## end ClCommandCtlGen%3972EF99039F.implementation

};

//## begin ClCommandCtlGen%3972EF99039F.postscript preserve=yes
//## end ClCommandCtlGen%3972EF99039F.postscript

//## begin ClStopXMLAnswerGen%3972EF440003.preface preserve=yes
//## end ClStopXMLAnswerGen%3972EF440003.preface

//## Class: ClStopXMLAnswerGen%3972EF440003
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStopXMLAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972F00803D1
{
  //## begin ClStopXMLAnswerGen%3972EF440003.initialDeclarations preserve=yes
  //## end ClStopXMLAnswerGen%3972EF440003.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825082
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClStopXMLAnswerGen%3972EF440003.public preserve=yes
      //## end ClStopXMLAnswerGen%3972EF440003.public

  protected:
    // Additional Protected Declarations
      //## begin ClStopXMLAnswerGen%3972EF440003.protected preserve=yes
      //## end ClStopXMLAnswerGen%3972EF440003.protected

  private:
    // Additional Private Declarations
      //## begin ClStopXMLAnswerGen%3972EF440003.private preserve=yes
      //## end ClStopXMLAnswerGen%3972EF440003.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStopXMLAnswerGen%3972EF440003.implementation preserve=yes
      //## end ClStopXMLAnswerGen%3972EF440003.implementation

};

//## begin ClStopXMLAnswerGen%3972EF440003.postscript preserve=yes
//## end ClStopXMLAnswerGen%3972EF440003.postscript

class ClRemoveXMLAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3972F00803D1
{
  public:
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);
};


//## begin ClConnectReplyGen%39740EA2009D.preface preserve=yes
//## end ClConnectReplyGen%39740EA2009D.preface

//## Class: ClConnectReplyGen%39740EA2009D
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClConnectReplyGen : public ClDspAnswerGen  //## Inherits: <unnamed>%39740EC30253
{
  //## begin ClConnectReplyGen%39740EA2009D.initialDeclarations preserve=yes
  //## end ClConnectReplyGen%39740EA2009D.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963906293
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClConnectReplyGen%39740EA2009D.public preserve=yes
      //## end ClConnectReplyGen%39740EA2009D.public

  protected:
    // Additional Protected Declarations
      //## begin ClConnectReplyGen%39740EA2009D.protected preserve=yes
      //## end ClConnectReplyGen%39740EA2009D.protected

  private:
    // Additional Private Declarations
      //## begin ClConnectReplyGen%39740EA2009D.private preserve=yes
      //## end ClConnectReplyGen%39740EA2009D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConnectReplyGen%39740EA2009D.implementation preserve=yes
      //## end ClConnectReplyGen%39740EA2009D.implementation

};

//## begin ClConnectReplyGen%39740EA2009D.postscript preserve=yes
//## end ClConnectReplyGen%39740EA2009D.postscript

//## begin ClTargetConfigGen%39F6C6940041.preface preserve=yes
//## end ClTargetConfigGen%39F6C6940041.preface

//## Class: ClTargetConfigGen%39F6C6940041
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTargetConfigGen : public ClDspAnswerGen  //## Inherits: <unnamed>%39F6C6CF0028
{
  //## begin ClTargetConfigGen%39F6C6940041.initialDeclarations preserve=yes
  //## end ClTargetConfigGen%39F6C6940041.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%972463864
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClTargetConfigGen%39F6C6940041.public preserve=yes
      //## end ClTargetConfigGen%39F6C6940041.public

  protected:
    // Additional Protected Declarations
      //## begin ClTargetConfigGen%39F6C6940041.protected preserve=yes
      //## end ClTargetConfigGen%39F6C6940041.protected

  private:
    // Additional Private Declarations
      //## begin ClTargetConfigGen%39F6C6940041.private preserve=yes
      //## end ClTargetConfigGen%39F6C6940041.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTargetConfigGen%39F6C6940041.implementation preserve=yes
      //## end ClTargetConfigGen%39F6C6940041.implementation

};

//## begin ClTargetConfigGen%39F6C6940041.postscript preserve=yes
//## end ClTargetConfigGen%39F6C6940041.postscript

//## begin ClStopDpchAnswerGen%3B1CB7DC03CF.preface preserve=yes
//## end ClStopDpchAnswerGen%3B1CB7DC03CF.preface

//## Class: ClStopDpchAnswerGen%3B1CB7DC03CF
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClStopDpchAnswerGen : public ClDspAnswerGen  //## Inherits: <unnamed>%3B1CB805000D
{
  //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.initialDeclarations preserve=yes
  //## end ClStopDpchAnswerGen%3B1CB7DC03CF.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%991737905
      m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL);

    // Additional Public Declarations
      //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.public preserve=yes
      //## end ClStopDpchAnswerGen%3B1CB7DC03CF.public

  protected:
    // Additional Protected Declarations
      //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.protected preserve=yes
      //## end ClStopDpchAnswerGen%3B1CB7DC03CF.protected

  private:
    // Additional Private Declarations
      //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.private preserve=yes
      //## end ClStopDpchAnswerGen%3B1CB7DC03CF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClStopDpchAnswerGen%3B1CB7DC03CF.implementation preserve=yes
      //## end ClStopDpchAnswerGen%3B1CB7DC03CF.implementation

};

//## begin ClStopDpchAnswerGen%3B1CB7DC03CF.postscript preserve=yes
//## end ClStopDpchAnswerGen%3B1CB7DC03CF.postscript

// Class ClStarXMLAnswerGen 

// Class ClConfigurationsXMLAnswerGen 

// Class ClCommandCtlGen 

// Class ClStopXMLAnswerGen 

// Class ClConnectReplyGen 

// Class ClTargetConfigGen 

// Class ClStopDpchAnswerGen 

//## begin module%393CFB78009E.epilog preserve=yes
//## end module%393CFB78009E.epilog


#endif
