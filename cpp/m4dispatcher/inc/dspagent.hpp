//## begin module%39479DF602CA.cm preserve=no
//## end module%39479DF602CA.cm

//## begin module%39479DF602CA.cp preserve=no
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
//## end module%39479DF602CA.cp

//## Module: dspagent%39479DF602CA; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: z:\m4dispatcher\inc\dspagent.hpp

#ifndef dspagent_h
#define dspagent_h 1

//## begin module%39479DF602CA.additionalIncludes preserve=no
//## end module%39479DF602CA.additionalIncludes

//## begin module%39479DF602CA.includes preserve=yes
#include <clbaseaction.hpp>
//## end module%39479DF602CA.includes

// SiteNodeInfo
#include <sitenodeinfo.hpp>

class ClDspExecutiveAgent;

//## begin module%39479DF602CA.declarations preserve=no
//## end module%39479DF602CA.declarations

//## begin module%39479DF602CA.additionalDeclarations preserve=yes
#ifdef _WINDOWS
	#define DISPATCHER_BIN_RELATIVE_DIR  "\\bin\\"
	#define DISPATCHER_CONFIG_RELATIVE_DIR  "\\conf\\"
	#define TARGETAPPCTLFILE_RELATIVE_PATH  "/conf/m4dspini.xml"
	#define TARGETLOGFILE_RELATIVE_PATH  "\\conf\\logsysdpch.ini"
	#define ADMHOSTSFILE_RELATIVE_PATH  "/conf/m4dspadmhosts.ini"
#endif

//## end module%39479DF602CA.additionalDeclarations


//## begin ClDspAgent%396B587301FF.preface preserve=yes
//## end ClDspAgent%396B587301FF.preface

//## Class: ClDspAgent%396B587301FF
//## Category: M4Dispatcher::Agents%39F451CA0292
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDspAgent : public ClBaseActionExecutive  //## Inherits: <unnamed>%396B63D0039D
{
  //## begin ClDspAgent%396B587301FF.initialDeclarations preserve=yes
  //## end ClDspAgent%396B587301FF.initialDeclarations

  public:
    //## begin ClDspAgent::eDspAgentActions%396B5FFB0338.preface preserve=yes
    //## end ClDspAgent::eDspAgentActions%396B5FFB0338.preface

    //## Class: eDspAgentActions%396B5FFB0338
    //## Category: M4Dispatcher::Agents%39F451CA0292
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {TOTAL_ACTION = 0} eDspAgentActions;

    //## begin ClDspAgent::eDspAgentActions%396B5FFB0338.postscript preserve=yes
    //## end ClDspAgent::eDspAgentActions%396B5FFB0338.postscript

    //## Constructors (specified)
      //## Operation: ClDspAgent%963314061
      ClDspAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent);


    //## Other Operations (specified)
      //## Operation: Start%963314063
      virtual m4return_t Start () = 0;

      //## Operation: ParseXML%963314075
      m4return_t ParseXML (ClParamList *ai_poParamList);

      //## Operation: SetParams%1000738437
      static m4return_t SetParams (m4uint32_t ai_iBasePort, m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iTimeout, m4uint32_t ai_iDumpTrace, m4uint32_t ai_iSessionsWeight, m4uint32_t ai_iSrvMemWeight, m4uint32_t ai_iSysMemWeight, m4uint32_t ai_iCPUWeight, m4uint32_t ai_iExecTimeWeight, m4uint32_t ai_iExecTimeThreshold);

  public:
    // Additional Public Declarations
      //## begin ClDspAgent%396B587301FF.public preserve=yes
      //## end ClDspAgent%396B587301FF.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iControlPort%3BA6077A002C
      //## begin ClDspAgent::m_iControlPort%3BA6077A002C.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iControlPort;
      //## end ClDspAgent::m_iControlPort%3BA6077A002C.attr

      //## Attribute: m_iAdminPort%3BA6077A0054
      //## begin ClDspAgent::m_iAdminPort%3BA6077A0054.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iAdminPort;
      //## end ClDspAgent::m_iAdminPort%3BA6077A0054.attr

      //## Attribute: m_iClientSSLPort%3BA6077A009A
      //## begin ClDspAgent::m_iClientSSLPort%3BA6077A009A.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iClientSSLPort;
      //## end ClDspAgent::m_iClientSSLPort%3BA6077A009A.attr

      //## Attribute: m_iClientPort%3BA6077A0108
      //## begin ClDspAgent::m_iClientPort%3BA6077A0108.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iClientPort;
      //## end ClDspAgent::m_iClientPort%3BA6077A0108.attr

      //## Attribute: m_iMaxStartRetries%3BA7094202A0
      //## begin ClDspAgent::m_iMaxStartRetries%3BA7094202A0.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iMaxStartRetries;
      //## end ClDspAgent::m_iMaxStartRetries%3BA7094202A0.attr

      //## Attribute: m_iServerStartTimeout%3FE1F187012B
      //## begin ClDspAgent::m_iServerStartTimeout%3FE1F187012B.attr preserve=no  protected: static m4uint32_t {UA} 
      static m4uint32_t m_iServerStartTimeout;
      //## end ClDspAgent::m_iServerStartTimeout%3FE1F187012B.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%396B59D700A3
      //## Role: ClDspAgent::m_poServerRepository%396B59D70338
      //## begin ClDspAgent::m_poServerRepository%396B59D70338.role preserve=no  protected: ClSiteNodeRepository { -> RHAN}
      ClSiteNodeRepository *m_poServerRepository;
      //## end ClDspAgent::m_poServerRepository%396B59D70338.role

      //## Association: M4Dispatcher::<unnamed>%396C27F103DF
      //## Role: ClDspAgent::m_poExecutiveAgent%396C27F2016B
      //## begin ClDspAgent::m_poExecutiveAgent%396C27F2016B.role preserve=no  protected: ClDspExecutiveAgent { -> RFHAN}
      ClDspExecutiveAgent *m_poExecutiveAgent;
      //## end ClDspAgent::m_poExecutiveAgent%396C27F2016B.role

	  static m4uint32_t m_iDumpTrace;

	  static m4uint32_t m_iSessionsWeight;
	  static m4uint32_t m_iSrvMemWeight;
	  static m4uint32_t m_iSysMemWeight;
	  static m4uint32_t m_iCPUWeight;
	  static m4uint32_t m_iExecTimeWeight;
	  static m4uint32_t m_iExecTimeThreshold;

    // Additional Protected Declarations
      //## begin ClDspAgent%396B587301FF.protected preserve=yes
      //## end ClDspAgent%396B587301FF.protected

  private:
    // Additional Private Declarations
      //## begin ClDspAgent%396B587301FF.private preserve=yes
      //## end ClDspAgent%396B587301FF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspAgent%396B587301FF.implementation preserve=yes
      //## end ClDspAgent%396B587301FF.implementation

};

//## begin ClDspAgent%396B587301FF.postscript preserve=yes
//## end ClDspAgent%396B587301FF.postscript

// Class ClDspAgent 

//## begin module%39479DF602CA.epilog preserve=yes
//## end module%39479DF602CA.epilog


#endif
