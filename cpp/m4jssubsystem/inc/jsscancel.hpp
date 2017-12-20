//## begin module%37D7C62F005F.cm preserve=no
//## end module%37D7C62F005F.cm

//## begin module%37D7C62F005F.cp preserve=no
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
//## end module%37D7C62F005F.cp

//## Module: JSSCancel%37D7C62F005F; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\FuentesServidor\m4jssubsystem\version\inc\jsscancel.hpp

#ifndef jsscancel_h
#define jsscancel_h 1

//## begin module%37D7C62F005F.additionalIncludes preserve=no
//## end module%37D7C62F005F.additionalIncludes

//## begin module%37D7C62F005F.includes preserve=yes
//## end module%37D7C62F005F.includes

// clexecutivebasess
#include <clexecutivebasess.hpp>
// StatsJSEXECUTE
#include <clstatsjsexecute.hpp>
// CcExecutor
#include <ccexecutor.hpp>
// LauncherForCancelExecutor28
#include <launcherccexe28.hpp>
// ExeForJobSchecEXECUTE
#include <exeforjobschecexecute.hpp>
//## begin module%37D7C62F005F.declarations preserve=no
//## end module%37D7C62F005F.declarations

//## begin module%37D7C62F005F.additionalDeclarations preserve=yes
//## end module%37D7C62F005F.additionalDeclarations


//## begin ClJSSCancel%37D7C4D2011B.preface preserve=yes
//## end ClJSSCancel%37D7C4D2011B.preface

//## Class: ClJSSCancel%37D7C4D2011B
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E6222E0352;ClStatsJSEXECUTE { -> }
//## Uses: <unnamed>%37EB741D03C6;ClEngineForJobSchecEXECUTE { -> }
//## Uses: <unnamed>%37EB744C01B1;ClLauncherForCancelExecutor28 { -> }
//## Uses: <unnamed>%37EB742C01C9;ClCcExecutor { -> }

class ClJSSCancel : public ClExecutiveBaseSS  //## Inherits: <unnamed>%37D7C5BF025D
{
  //## begin ClJSSCancel%37D7C4D2011B.initialDeclarations preserve=yes
  //## end ClJSSCancel%37D7C4D2011B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSSCancel%936888044
      ClJSSCancel (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);


    //## Other Operations (specified)
      //## Operation: Initialize%936888045
      m4return_t Initialize ();

      //## Operation: ExecuteCommand%936888046
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%936888047
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%936888048
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: Start%936888049
      m4return_t Start ();

      //## Operation: Stop%937812721
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: SetDumpFile%940237695
      m4return_t SetDumpFile (m4pchar_t ai_pcDumpFile);

      //## Operation: SetIdentification%940337547
      m4return_t SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName);

    // Additional Public Declarations
      //## begin ClJSSCancel%37D7C4D2011B.public preserve=yes
	  void SetOutputQ ( ClOutputQ * aiOutputQ ) 
	  {
		  m_poOutputQ = aiOutputQ ;
	  }
      //## end ClJSSCancel%37D7C4D2011B.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pcDumpFile%380AE5EE02D2
      //## begin ClJSSCancel::m_pcDumpFile%380AE5EE02D2.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcDumpFile;
      //## end ClJSSCancel::m_pcDumpFile%380AE5EE02D2.attr

      //## Attribute: m_oServerName%380C8879012C
      //## begin ClJSSCancel::m_oServerName%380C8879012C.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServerName;
      //## end ClJSSCancel::m_oServerName%380C8879012C.attr

      //## Attribute: m_oServiceName%380C887B021F
      //## begin ClJSSCancel::m_oServiceName%380C887B021F.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServiceName;
      //## end ClJSSCancel::m_oServiceName%380C887B021F.attr

    // Additional Protected Declarations
      //## begin ClJSSCancel%37D7C4D2011B.protected preserve=yes
      //## end ClJSSCancel%37D7C4D2011B.protected

  private:
    // Additional Private Declarations
      //## begin ClJSSCancel%37D7C4D2011B.private preserve=yes
	  ClOutputQ * m_poOutputQ ;
      //## end ClJSSCancel%37D7C4D2011B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSSCancel%37D7C4D2011B.implementation preserve=yes
      //## end ClJSSCancel%37D7C4D2011B.implementation

};

//## begin ClJSSCancel%37D7C4D2011B.postscript preserve=yes
//## end ClJSSCancel%37D7C4D2011B.postscript

// Class ClJSSCancel 

//## begin module%37D7C62F005F.epilog preserve=yes
//## end module%37D7C62F005F.epilog


#endif
