//## begin module%35A2433F03E3.cm preserve=no
//## end module%35A2433F03E3.cm

//## begin module%35A2433F03E3.cp preserve=no
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
//## end module%35A2433F03E3.cp

//## Module: LogonInstance%35A2433F03E3; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: D:\Work\v600\m4comsrv\inc\logoninstance.hpp

#ifndef logoninstance_h
#define logoninstance_h 1

//## begin module%35A2433F03E3.additionalIncludes preserve=no
//## end module%35A2433F03E3.additionalIncludes

#include <logoninterface.hpp>

class ClUsersSS;
class ClUSServiceAPI;



//## begin module%35A2433F03E3.declarations preserve=no
//## end module%35A2433F03E3.declarations

//## begin module%35A2433F03E3.additionalDeclarations preserve=yes
//## end module%35A2433F03E3.additionalDeclarations


//## begin ClLogonInstance%359CC3270345.preface preserve=yes
//## end ClLogonInstance%359CC3270345.preface

//## Class: ClLogonInstance%359CC3270345
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36A4984A0157;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%371F70F702C6;ClIdGenerator { -> F}

class ClLogonInstance : public ClLogonInterface  //## Inherits: <unnamed>%35A2425B0182
{
  //## begin ClLogonInstance%359CC3270345.initialDeclarations preserve=yes
  //## end ClLogonInstance%359CC3270345.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLogonInstance%899809127
      ClLogonInstance (ClUsersSS *ai_pUUSS);


    //## Other Operations (specified)
      //## Operation: OpenSession%899466669; C++
      m4return_t OpenSession (const m4pchar_t ai_pszUsername, const m4pchar_t ai_pszPassword, ClUSSessionInterface *&ao_pSession, m4bool_t ai_bDo2Way, const m4pchar_t ai_pszClientMachine);

      //## Operation: OpenSessionEx%899466670; C++
      m4return_t OpenSessionEx (const m4pchar_t ai_pszUsername, m4pchar_t ai_pszRoleId, ClUSSessionInterface *&ao_pSession);

      //## Operation: EnableRole%899466671; C++
      m4return_t EnableRole (ClUSSessionInterface *ai_pSession, m4pchar_t ai_pszRoleId);

      //## Operation: CloseSession%899809125; C++
      m4return_t CloseSession (ClUSSessionInterface *ai_pSession, m4bool_t ai_bStrict = M4_TRUE);

	  // Auditoría de sesión
      m4return_t CloseAuditory (ClUSSessionInterface *ai_pSession);
      m4return_t CleanAuditory (void);

    // Additional Public Declarations
      //## begin ClLogonInstance%359CC3270345.public preserve=yes
      //## end ClLogonInstance%359CC3270345.public

  protected:
    //## Constructors (specified)
      //## Operation: ClLogonInstance%899809134
      ClLogonInstance ();

    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%35A243E500A7
      //## Role: ClLogonInstance::m_pUUSS%35A243E502F9
      //## begin ClLogonInstance::m_pUUSS%35A243E502F9.role preserve=no  protected: ClUsersSS {0..n -> 1RFHAN}
      ClUsersSS *m_pUUSS;
      //## end ClLogonInstance::m_pUUSS%35A243E502F9.role

      //## Association: Subsystem::UsersSubsystem::<unnamed>%35A32D7E0229
      //## Role: ClLogonInstance::m_pService%35A32D800044
      //## begin ClLogonInstance::m_pService%35A32D800044.role preserve=no  protected: ClUSServiceAPI {0..n -> 1RFHAN}
      ClUSServiceAPI *m_pService;
      //## end ClLogonInstance::m_pService%35A32D800044.role

    // Additional Protected Declarations
      //## begin ClLogonInstance%359CC3270345.protected preserve=yes
      //## end ClLogonInstance%359CC3270345.protected

  private:
    // Additional Private Declarations
      //## begin ClLogonInstance%359CC3270345.private preserve=yes
      //## end ClLogonInstance%359CC3270345.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLogonInstance%359CC3270345.implementation preserve=yes
      //## end ClLogonInstance%359CC3270345.implementation

};

//## begin ClLogonInstance%359CC3270345.postscript preserve=yes
//## end ClLogonInstance%359CC3270345.postscript

// Class ClLogonInstance 

//## begin module%35A2433F03E3.epilog preserve=yes
//## end module%35A2433F03E3.epilog


#endif
