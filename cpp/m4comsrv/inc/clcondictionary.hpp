//## begin module%34224022015E.cm preserve=no
//## end module%34224022015E.cm

//## begin module%34224022015E.cp preserve=no
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
//## end module%34224022015E.cp

//## Module: clcondictionary%34224022015E; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clcondictionary.hpp

#ifndef __CLCONDICTIONARY__HPP__
#define __CLCONDICTIONARY__HPP__ 1

//## begin module%34224022015E.additionalIncludes preserve=no
//## end module%34224022015E.additionalIncludes

//## begin module%34224022015E.includes preserve=yes
#include <clbasedictionary.hpp>


//## begin module%34224022015E.declarations preserve=no
//## end module%34224022015E.declarations

//## begin module%34224022015E.additionalDeclarations preserve=yes
//## end module%34224022015E.additionalDeclarations


//## begin ClConDictionary%342240220277.preface preserve=yes
//## end ClConDictionary%342240220277.preface

//## Class: ClConDictionary%342240220277
//	Dictionary for connections. This dictionary is joined to the user dictionary
//	in the way that each user has a pool of connections. moreover the server
//	connection dictionary, the users have their own connection dictionary
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3422402D02E9;ClBaseDictionary { -> }
//## Uses: <unnamed>%345F0A5F02EE;ClConnection { -> F}
//## Uses: <unnamed>%358F7F9C0130;ClUSSession { -> F}
//## Uses: <unnamed>%36A47D5B027F;ClRequest { -> F}

class ClConDictionary : public ClBaseDictionary  //## Inherits: <unnamed>%3422402D02F7
{
  //## begin ClConDictionary%342240220277.initialDeclarations preserve=yes
  //## end ClConDictionary%342240220277.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: RemoveConnection%-1963334655; C++
      //	Destroy one connection
      m4return_t RemoveConnection (m4objid_t ai_IdConnection);

      //## Operation: Synchronize%898585987
      m4return_t Synchronize ();

      //## Operation: Replace%967446812
      m4return_t Replace (m4objid_t ai_lConnectionID, ClConnInstance *ai_poConection);

      //## Operation: GetAndAddRequest%989340981; C++
      //	Return the item refered by the id passed
      ClConnInstance * GetAndAddRequest (m4int64_t ai_Id);

      //## Operation: GetUserAndSessionId%1010489722
      m4return_t GetUserAndSessionId (m4objid_t ai_iConnectionId, m4objid_t &ao_iUserId, m4objid_t &ao_iSessionId);

    // Additional Public Declarations
      //## begin ClConDictionary%342240220277.public preserve=yes
      //## end ClConDictionary%342240220277.public

  protected:
    // Additional Protected Declarations
      //## begin ClConDictionary%342240220277.protected preserve=yes
      //## end ClConDictionary%342240220277.protected

  private:
    // Additional Private Declarations
      //## begin ClConDictionary%342240220277.private preserve=yes
      //## end ClConDictionary%342240220277.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConDictionary%342240220277.implementation preserve=yes
      //## end ClConDictionary%342240220277.implementation

};

//## begin ClConDictionary%342240220277.postscript preserve=yes
//## end ClConDictionary%342240220277.postscript

//## begin module%34224022015E.epilog preserve=yes
//## end module%34224022015E.epilog


#endif
