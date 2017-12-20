//## begin module%34C863090189.cm preserve=no
//## end module%34C863090189.cm

//## begin module%34C863090189.cp preserve=no
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
//## end module%34C863090189.cp

//## Module: sessiondic%34C863090189; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\sessiondic.hpp

#ifndef sessiondic_h
#define sessiondic_h 1

//## begin module%34C863090189.additionalIncludes preserve=no
//## end module%34C863090189.additionalIncludes

#include <clbasedictionary.hpp>

class ClUSSessionInterface;


//## begin module%34C863090189.declarations preserve=no
//## end module%34C863090189.declarations

//## begin module%34C863090189.additionalDeclarations preserve=yes
//## end module%34C863090189.additionalDeclarations


//## begin ClSessionDic%34C7A3850236.preface preserve=yes
//## end ClSessionDic%34C7A3850236.preface

//## Class: ClSessionDic%34C7A3850236
//	 Sessions dictinary. It will contain  alist of sessions. It's a son of ClBase
//	Dictionary so it has the methods to access to the items.
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34CCF14802CB;ClConnection { -> F}
//## Uses: <unnamed>%353F7ABA01D4;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%3781F71403D9;ClLocalMonitor { -> F}
//## Uses: <unnamed>%378206CE034C;ClUSSessionStats { -> F}

class ClSessionDic : public ClBaseDictionary  //## Inherits: <unnamed>%34C7A3E1038D
{
  //## begin ClSessionDic%34C7A3850236.initialDeclarations preserve=yes
  //## end ClSessionDic%34C7A3850236.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: AddSession%885550081
      //	This method adds an existing session object or endeed a pointer to it to the
      //	dictionary, using its own id for that.
      m4return_t AddSession (ClUSSessionInterface *ai_poSession	// The ponter to the session object to be deliver into the session list.
      );

      //## Operation: RemoveSession%885897765
      m4return_t RemoveSession (m4uint32_t ai_lIdSession);

      //## Operation: Synchronize%901531333
      void Synchronize ();

      //## Operation: CheckTimeouts%985177782
      m4return_t CheckTimeouts (vector <m4objid_t> &ao_vSessions, vector <m4objid_t> &ao_vUsers);

      //## Operation: GetAndAddRequest%989337235; C++
      //	Return the item refered by the id passed
      ClUSSessionInterface * GetAndAddRequest (m4int64_t ai_Id);

    // Additional Public Declarations
      //## begin ClSessionDic%34C7A3850236.public preserve=yes
      //## end ClSessionDic%34C7A3850236.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessionDic%34C7A3850236.protected preserve=yes
      //## end ClSessionDic%34C7A3850236.protected

  private:
    // Additional Private Declarations
      //## begin ClSessionDic%34C7A3850236.private preserve=yes
      //## end ClSessionDic%34C7A3850236.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessionDic%34C7A3850236.implementation preserve=yes
      //## end ClSessionDic%34C7A3850236.implementation

};

//## begin ClSessionDic%34C7A3850236.postscript preserve=yes
//## end ClSessionDic%34C7A3850236.postscript

// Class ClSessionDic 

//## begin module%34C863090189.epilog preserve=yes
//## end module%34C863090189.epilog


#endif
