//## begin module%359CC37A0122.cm preserve=no
//## end module%359CC37A0122.cm

//## begin module%359CC37A0122.cp preserve=no
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
//## end module%359CC37A0122.cp

//## Module: LogonInterface%359CC37A0122; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: D:\Work\v600\m4comsrv\inc\logoninterface.hpp

#ifndef logoninterface_h
#define logoninterface_h 1

//## begin module%359CC37A0122.additionalIncludes preserve=no
//## end module%359CC37A0122.additionalIncludes

#include <m4types.hpp>

class ClUSSessionInterface;



//## begin module%359CC37A0122.declarations preserve=no
//## end module%359CC37A0122.declarations

//## begin module%359CC37A0122.additionalDeclarations preserve=yes
//## end module%359CC37A0122.additionalDeclarations


//## Class: ClLogonInterface%35A2405A0347
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%359CC5A20096;m4return_t { -> }
//## Uses: <unnamed>%359CC5620190;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%35A2481E01B1;m4pchar_t { -> }

class ClLogonInterface 
{
  public:

    //## Other Operations (specified)
      //## Operation: OpenSession%899809130; C++
      virtual m4return_t OpenSession (const m4pchar_t ai_pszUsername, const m4pchar_t ai_pszPassword, ClUSSessionInterface *&ao_pSession, m4bool_t ai_bDo2Way, const m4pchar_t ai_pszClientMachine) = 0;

      //## Operation: OpenSessionEx%899809131; C++
      virtual m4return_t OpenSessionEx (const m4pchar_t ai_pszUsername, m4pchar_t ai_pszRoleId, ClUSSessionInterface *&ao_pSession) = 0;

      //## Operation: EnableRole%899809132; C++
      virtual m4return_t EnableRole (ClUSSessionInterface *ai_pSession, m4pchar_t ai_pszRoleId) = 0;

      //## Operation: CloseSession%899809133; C++
      virtual m4return_t CloseSession (ClUSSessionInterface *ai_pSession, m4bool_t ai_bStrict = M4_TRUE) = 0;

	  // Auditoría de sesión
      virtual m4return_t CloseAuditory (ClUSSessionInterface *ai_pSession) = 0;
      virtual m4return_t CleanAuditory (void) = 0;

  protected:
  private:
  private: //## implementation
};

// Class ClLogonInterface 

// Class ClLogonInterface 

//## begin module%359CC37A0122.epilog preserve=yes
//## end module%359CC37A0122.epilog


#endif
