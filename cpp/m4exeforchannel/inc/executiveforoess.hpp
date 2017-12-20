//## begin module%39105E65005A.cm preserve=no
//## end module%39105E65005A.cm

//## begin module%39105E65005A.cp preserve=no
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
//## end module%39105E65005A.cp

//## Module: ExecutiveForOESS%39105E65005A; Subprogram specification
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Source file: F:\integration\m4exeforchannel\inc\executiveforoess.hpp

#ifndef executiveforoess_h
#define executiveforoess_h 1

//## begin module%39105E65005A.additionalIncludes preserve=no
//## end module%39105E65005A.additionalIncludes

//## begin module%39105E65005A.includes preserve=yes
//## end module%39105E65005A.includes

// ClAdminTag
#include <cladmintag.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
//## begin module%39105E65005A.declarations preserve=no
//## end module%39105E65005A.declarations

//## begin module%39105E65005A.additionalDeclarations preserve=yes
//## end module%39105E65005A.additionalDeclarations


//## begin ClExecutiveForOESS%39105AD200C2.preface preserve=yes
//## end ClExecutiveForOESS%39105AD200C2.preface

//## Class: ClExecutiveForOESS%39105AD200C2
//## Category: M4ExeForChannel%3507BDA601C6
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3910601A02E3;ClAdminCommandTag { -> F}
//## Uses: <unnamed>%3910603C002F;ClAdminTagList { -> }
//## Uses: <unnamed>%391062800372;ClLauncherInterface { -> F}

class ClExecutiveForOESS : public ClExecutiveBaseSS  //## Inherits: <unnamed>%39105B3B013B
{
  //## begin ClExecutiveForOESS%39105AD200C2.initialDeclarations preserve=yes
  //## end ClExecutiveForOESS%39105AD200C2.initialDeclarations

  public:
    //## Class: eExecutiveForOECommands%391061FD00B7
    //## Category: M4ExeForChannel%3507BDA601C6
    //## Subsystem: M4ExeForChannel%3507BDD10109
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { QUIT = ClExecutiveBaseSS::TOTAL_COMMANDS, PAUSE , RESUME, GET_USED_MEMORY , TOTAL_COMMANDS } eExecutiveForOECommands;


    //## Other Operations (specified)
      //## Operation: ExecuteCommand%957346245
      //	Ejecución de comandos del subsistema de ejecutor
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%957346246
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%957346247
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

  public:
    // Additional Public Declarations
      //## begin ClExecutiveForOESS%39105AD200C2.public preserve=yes
      //## end ClExecutiveForOESS%39105AD200C2.public

  protected:
    //## Constructors (specified)
      //## Operation: ClExecutiveForOESS%957346242
      //	constructor de la clase
      ClExecutiveForOESS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    // Additional Protected Declarations
      //## begin ClExecutiveForOESS%39105AD200C2.protected preserve=yes
      //## end ClExecutiveForOESS%39105AD200C2.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutiveForOESS%39105AD200C2.private preserve=yes
      //## end ClExecutiveForOESS%39105AD200C2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForOESS%39105AD200C2.implementation preserve=yes
      //## end ClExecutiveForOESS%39105AD200C2.implementation

};

//## begin ClExecutiveForOESS%39105AD200C2.postscript preserve=yes
//## end ClExecutiveForOESS%39105AD200C2.postscript

// Class ClExecutiveForOESS 

//## begin module%39105E65005A.epilog preserve=yes
//## end module%39105E65005A.epilog


#endif
