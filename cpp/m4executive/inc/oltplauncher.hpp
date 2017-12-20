//## begin module%354837EC02CC.cm preserve=no
//## end module%354837EC02CC.cm

//## begin module%354837EC02CC.cp preserve=no
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
//## end module%354837EC02CC.cp

//## Module: OLTPLauncher%354837EC02CC; Package specification
//## Subsystem: M4Executive::inc%379462FF024C
//## Source file: F:\integration\m4executive\inc\oltplauncher.hpp

#ifndef oltplauncher_h
#define oltplauncher_h 1

//## begin module%354837EC02CC.additionalIncludes preserve=yes
#include <m4executive_dll.hpp>
//## end module%354837EC02CC.additionalIncludes

//## begin module%354837EC02CC.includes preserve=yes
//## end module%354837EC02CC.includes

// servicepdus
#include <servicepdus.hpp>
// USInterface
#include <usinterface.hpp>
// LauncherInterface
#include <launcher.hpp>
// Executive
#include <executive.hpp>
// queuebasedss
#include <queuebasedss.hpp>

class ClConfiguration;
class ClNavigatorFirewall;

//## begin module%354837EC02CC.declarations preserve=no
//## end module%354837EC02CC.declarations

//## begin module%354837EC02CC.additionalDeclarations preserve=yes
//## end module%354837EC02CC.additionalDeclarations


//## begin ClLauncherForOLTP%35472FA20307.preface preserve=yes
//## end ClLauncherForOLTP%35472FA20307.preface

//## Class: ClLauncherForOLTP%35472FA20307
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35483A01024F;ClEngineArgs { -> }
//## Uses: <unnamed>%35483A8102FB;ClRequestPDU { -> }
//## Uses: <unnamed>%347596CA031B;ClReplyPDU { -> }
//## Uses: <unnamed>%3559B65902C6;ClRequest { -> F}
//## Uses: <unnamed>%3561CB5C029F;ClUSSessionInterface { -> }
//## Uses: <unnamed>%36F68007004A;ClConfiguration { -> F}
//## Uses: <unnamed>%36F6802600A8;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%37975C2501DC;ClRequest { -> F}
//## Uses: <unnamed>%37975CBE0344;ClConnBase { -> F}

class M4_DECL_M4EXECUTIVE ClLauncherForOLTP : public ClLauncherInterface  //## Inherits: <unnamed>%35473603028A
{
  //## begin ClLauncherForOLTP%35472FA20307.initialDeclarations preserve=yes
  //## end ClLauncherForOLTP%35472FA20307.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLauncherForOLTP%893925593; C++
      //	Parameterized constructor for class Scheduler.
      //## Concurrency: Synchonous
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ClLauncherForOLTP (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats = NULL, ClQueueBasedSS *ai_poServiceSS = NULL);

    //## Destructor (specified)
      //## Operation: ~ClLauncherForOLTP%893925594; C++
      //	Default destructor for class Scheduler.
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ~ClLauncherForOLTP ();


    //## Other Operations (specified)
      //## Operation: Destroy%894305680
      m4return_t Destroy ();

      //## Operation: _Dump%921058930; C++
      m4return_t _Dump ();

      //## Operation: ExecuteCommand%922189094
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: _CleanUp%943284079; C++
      m4return_t _CleanUp (ClBaseRequest *ai_poRequest);

    // Additional Public Declarations
      //## begin ClLauncherForOLTP%35472FA20307.public preserve=yes
      //## end ClLauncherForOLTP%35472FA20307.public

  protected:

    //## Other Operations (specified)
      //## Operation: _GetNextRequest%943284073; C++
      m4return_t _GetNextRequest (ClBaseRequest *&ao_poRequest);

      //## Operation: _PrepareExecution%943284074; C++
      m4return_t _PrepareExecution (ClBaseRequest *ai_poRequest);

      //## Operation: _ExecuteRequest%943284075
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t _ExecuteRequest (ClBaseRequest *ai_poRequest);

      //## Operation: _ReplyRequest%943284076; C++
      m4return_t _ReplyRequest (ClBaseRequest *ai_poRequest);

    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%383A6E7A011F
      //## Role: ClLauncherForOLTP::m_oExecImage%383A6E7A02F6
      //## begin ClLauncherForOLTP::m_oExecImage%383A6E7A02F6.role preserve=no  protected: ClImage { -> 1VHAN}
      ClImage m_oExecImage;
      //## end ClLauncherForOLTP::m_oExecImage%383A6E7A02F6.role

      //## Association: M4Executive::<unnamed>%3922CBB80356
      //## Role: ClLauncherForOLTP::m_poServiceSS%3922CBB9014F
      //## begin ClLauncherForOLTP::m_poServiceSS%3922CBB9014F.role preserve=no  protected: ClQueueBasedSS { -> RHAN}
      ClQueueBasedSS *m_poServiceSS;
      //## end ClLauncherForOLTP::m_poServiceSS%3922CBB9014F.role

    // Additional Protected Declarations
      //## begin ClLauncherForOLTP%35472FA20307.protected preserve=yes
      //## end ClLauncherForOLTP%35472FA20307.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherForOLTP%35472FA20307.private preserve=yes
      //## end ClLauncherForOLTP%35472FA20307.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherForOLTP%35472FA20307.implementation preserve=yes
      //## end ClLauncherForOLTP%35472FA20307.implementation

};

//## begin ClLauncherForOLTP%35472FA20307.postscript preserve=yes
//## end ClLauncherForOLTP%35472FA20307.postscript

// Class ClLauncherForOLTP 

//## begin module%354837EC02CC.epilog preserve=yes
//## end module%354837EC02CC.epilog


#endif
