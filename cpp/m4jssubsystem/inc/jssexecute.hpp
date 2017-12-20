//## begin module%37D379BE01D5.cm preserve=no
//## end module%37D379BE01D5.cm

//## begin module%37D379BE01D5.cp preserve=no
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
//## end module%37D379BE01D5.cp

//## Module: JSSExecute%37D379BE01D5; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\inc\jssexecute.hpp

#ifndef jssexecute_h
#define jssexecute_h 1

//## begin module%37D379BE01D5.additionalIncludes preserve=no
//## end module%37D379BE01D5.additionalIncludes

//## begin module%37D379BE01D5.includes preserve=yes
//## end module%37D379BE01D5.includes

// Executive
#include <executive.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
// StatsJSEXECUTE
#include <clstatsjsexecute.hpp>
//## begin module%37D379BE01D5.declarations preserve=no
//## end module%37D379BE01D5.declarations

//## begin module%37D379BE01D5.additionalDeclarations preserve=yes
//## end module%37D379BE01D5.additionalDeclarations


//## begin ClJSSExecute%37CE8D260166.preface preserve=yes
//## end ClJSSExecute%37CE8D260166.preface

//## Class: ClJSSExecute%37CE8D260166
//	Esta clase será el subsistema de los ejecutores de jobs . Será la encargada
//	de crear el ejecutor al que esté asociado . También será la encargada de
//	gestionar las estadísticas de un ejecutor concreto a la herramienta de
//	administración y es e ella a la que pedirá el subsistema el comando cde
//	cancelación de un ejecutor
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D3DE350047;ClStatsJSEXECUTE { -> }
//## Uses: <unnamed>%3804B1FE0186;ClJSLauncherExecutor { -> }
//## Uses: <unnamed>%3804CD6703D5; { -> }

class ClJSSExecute : public ClExecutiveBaseSS  //## Inherits: <unnamed>%37D7C32F0390
{
  //## begin ClJSSExecute%37CE8D260166.initialDeclarations preserve=yes
  //## end ClJSSExecute%37CE8D260166.initialDeclarations

  public:
    //## begin ClJSSExecute::eJSExecuteCommands%37EA55BF00C4.preface preserve=yes
    //## end ClJSSExecute::eJSExecuteCommands%37EA55BF00C4.preface

    //## Class: eJSExecuteCommands%37EA55BF00C4
    //## Category: M4JSSubsystem%35F51CAB0378
    //## Subsystem: M4JSSubsystem%35F539720231
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {JS_CANCEL_JOB=ClExecutiveBaseSS::TOTAL_COMMANDS, JS_CHANGE_TIMEOUT, JS_DELETE_TIMEOUT, TOTAL_COMMANDS} eJSExecuteCommands;

    //## begin ClJSSExecute::eJSExecuteCommands%37EA55BF00C4.postscript preserve=yes
    //## end ClJSSExecute::eJSExecuteCommands%37EA55BF00C4.postscript

    //## Constructors (specified)
      //## Operation: ClJSSExecute%936606367
      ClJSSExecute (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);


    //## Other Operations (specified)
      //## Operation: Initialize%936606368
      m4return_t Initialize ();

      //## Operation: ExecuteCommand%936606369
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%936606370
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%936606371
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: Start%936606372
      m4return_t Start ();

      //## Operation: Stop%937812723
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: SetDumpFile%940237697
      m4return_t SetDumpFile (m4pchar_t ai_pcDumpFile);

      //## Operation: SetIdentification%942840134
      m4return_t SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName);

  public:
    // Additional Public Declarations
      //## begin ClJSSExecute%37CE8D260166.public preserve=yes
	  void SetOutputQ ( ClOutputQ * aiOutputQ ) 
	  {
		  m_poOutputQ = aiOutputQ ;
	  }
      //## end ClJSSExecute%37CE8D260166.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pcDumpFile%380AE5F303D4
      //## begin ClJSSExecute::m_pcDumpFile%380AE5F303D4.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcDumpFile;
      //## end ClJSSExecute::m_pcDumpFile%380AE5F303D4.attr

      //## Attribute: m_oServerName%3832CDD50283
      //## begin ClJSSExecute::m_oServerName%3832CDD50283.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServerName;
      //## end ClJSSExecute::m_oServerName%3832CDD50283.attr

      //## Attribute: m_oServiceName%3832CDD90333
      //## begin ClJSSExecute::m_oServiceName%3832CDD90333.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServiceName;
      //## end ClJSSExecute::m_oServiceName%3832CDD90333.attr

    // Data Members for Associations

      //## Association: M4JSSubsystem::<unnamed>%382C173002C2
      //## Role: ClJSSExecute::m_poExecutive%382C173101E7
      //## begin ClJSSExecute::m_poExecutive%382C173101E7.role preserve=no  protected: ClExecutive { -> RHAN}
      ClExecutive *m_poExecutive;
      //## end ClJSSExecute::m_poExecutive%382C173101E7.role

    // Additional Protected Declarations
      //## begin ClJSSExecute%37CE8D260166.protected preserve=yes
      //## end ClJSSExecute%37CE8D260166.protected

  private:
    // Additional Private Declarations
      //## begin ClJSSExecute%37CE8D260166.private preserve=yes
	  ClOutputQ * m_poOutputQ ;
      //## end ClJSSExecute%37CE8D260166.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSSExecute%37CE8D260166.implementation preserve=yes
      //## end ClJSSExecute%37CE8D260166.implementation

};

//## begin ClJSSExecute%37CE8D260166.postscript preserve=yes
//## end ClJSSExecute%37CE8D260166.postscript

// Class ClJSSExecute 

//## begin module%37D379BE01D5.epilog preserve=yes
//## end module%37D379BE01D5.epilog


#endif
