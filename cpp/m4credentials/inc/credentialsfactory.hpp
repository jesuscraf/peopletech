//## begin module%395A3A910048.cm preserve=no
//## end module%395A3A910048.cm

//## begin module%395A3A910048.cp preserve=no
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
//## end module%395A3A910048.cp

//## Module: CredentialsFactory%395A3A910048; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: D:\eduardoma\m4credentials\inc\credentialsfactory.hpp

#ifndef credentialsfactory_h
#define credentialsfactory_h 1

//## begin module%395A3A910048.additionalIncludes preserve=no
//## end module%395A3A910048.additionalIncludes

//## begin module%395A3A910048.includes preserve=yes
#include "m4credentials_dll.hpp"
//## end module%395A3A910048.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>
// m4credentials
#include <m4credentials.hpp>
// CriticalSection
#include <criticalsection.hpp>
//## begin module%395A3A910048.declarations preserve=no
//## end module%395A3A910048.declarations

//## begin module%395A3A910048.additionalDeclarations preserve=yes
//## end module%395A3A910048.additionalDeclarations


//## begin ClCredentialsFactory%395A3A6F02E9.preface preserve=yes
//## end ClCredentialsFactory%395A3A6F02E9.preface

//## Class: ClCredentialsFactory%395A3A6F02E9
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%395A3B48006A;friend M4Credentials { -> }
//## Uses: <unnamed>%395B14D202B8;ClBlockSync { -> F}
//## Uses: <unnamed>%3965AEC200D7;m4date_t { -> }
//## Uses: <unnamed>%3965AEC303D2;m4string_t { -> }

class M4_DECL_M4CREDENTIALS ClCredentialsFactory 
{
  //## begin ClCredentialsFactory%395A3A6F02E9.initialDeclarations preserve=yes
  //## end ClCredentialsFactory%395A3A6F02E9.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClCredentialsFactory%962187532
      ~ClCredentialsFactory ();


    //## Other Operations (specified)
      //## Operation: CreateCredential%962187533
      static M4Credentials * CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID);

      //## Operation: CreateCredential%962964032
      static M4Credentials * CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, m4string_t &ao_strSessionID);

      //## Operation: CreateCredential%962621271
      static M4Credentials * CreateCredential (const m4string_t &ai_strSessionID);

      //## Operation: DestroyCredential%962187534
      static m4return_t DestroyCredential (M4Credentials &ai_oCredential);

      //## Operation: Reset%963411914
      void Reset ();

      //## Operation: Instance%962187535
      static ClCredentialsFactory * Instance (const ISessionIDGenerator &ai_oSessionIDGenerator = ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator);

    // Additional Public Declarations
      //## begin ClCredentialsFactory%395A3A6F02E9.public preserve=yes
      //## end ClCredentialsFactory%395A3A6F02E9.public

  protected:

    //## Other Operations (specified)
      //## Operation: _CreateCredential%962266647
      M4Credentials * _CreateCredential (const m4string_t &ai_strMechanism, const m4string_t &ai_strLoginName, const m4string_t &ai_strPassword, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID);

      //## Operation: _CreateCredential%962621272
      M4Credentials * _CreateCredential (const m4string_t &ai_strSessionID);

      //## Operation: _DestroyCredential%962266648
      m4return_t _DestroyCredential (M4Credentials &ai_oCredential);

      //## Operation: _GetRoleList%963411915
      m4return_t _GetRoleList (M4Credentials::TRoleInfoList &ao_oRoleList);

    // Additional Protected Declarations
      //## begin ClCredentialsFactory%395A3A6F02E9.protected preserve=yes
      //## end ClCredentialsFactory%395A3A6F02E9.protected

  private:
    //## Constructors (specified)
      //## Operation: ClCredentialsFactory%962187531
      ClCredentialsFactory (const ISessionIDGenerator &ai_oSessionIDGenerator = ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator);

    // Data Members for Associations

      //## Association: M4Credentials::<unnamed>%395A3ED301E0
      //## Role: ClCredentialsFactory::s_poFactoryInstance%395A3ED5032E
      //## begin ClCredentialsFactory::s_poFactoryInstance%395A3ED5032E.role preserve=no  private: static ClCredentialsFactory { -> 1RHAN}
      static ClCredentialsFactory *s_poFactoryInstance;
      //## end ClCredentialsFactory::s_poFactoryInstance%395A3ED5032E.role

      //## Association: M4Credentials::<unnamed>%395A3F5A009A
      //## Role: ClCredentialsFactory::s_oSyncrhonizer%395A3F5A024A
      //## begin ClCredentialsFactory::s_oSyncrhonizer%395A3F5A024A.role preserve=no  private: static ClCriticalSection { -> 1VHAN}
      static ClCriticalSection s_oSyncrhonizer;
      //## end ClCredentialsFactory::s_oSyncrhonizer%395A3F5A024A.role

      //## Association: M4Credentials::<unnamed>%39606E3B02A8
      //## Role: ClCredentialsFactory::m_poSessionIDGenerator%39606E3C005A
      //## begin ClCredentialsFactory::m_poSessionIDGenerator%39606E3C005A.role preserve=no  private: ISessionIDGenerator { -> 1RHANC}
      ISessionIDGenerator const *m_poSessionIDGenerator;
      //## end ClCredentialsFactory::m_poSessionIDGenerator%39606E3C005A.role

    // Additional Private Declarations
      //## begin ClCredentialsFactory%395A3A6F02E9.private preserve=yes
      //## end ClCredentialsFactory%395A3A6F02E9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCredentialsFactory%395A3A6F02E9.implementation preserve=yes
      //## end ClCredentialsFactory%395A3A6F02E9.implementation

};

//## begin ClCredentialsFactory%395A3A6F02E9.postscript preserve=yes
//## end ClCredentialsFactory%395A3A6F02E9.postscript

// Class ClCredentialsFactory 

//## begin module%395A3A910048.epilog preserve=yes
//## end module%395A3A910048.epilog


#endif
