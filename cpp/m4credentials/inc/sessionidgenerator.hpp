//## begin module%39606F190397.cm preserve=no
//## end module%39606F190397.cm

//## begin module%39606F190397.cp preserve=no
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
//## end module%39606F190397.cp

//## Module: SessionIDGenerator%39606F190397; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: F:\integration\m4credentials\inc\sessionidgenerator.hpp

#ifndef sessionidgenerator_h
#define sessionidgenerator_h 1

//## begin module%39606F190397.additionalIncludes preserve=no
//## end module%39606F190397.additionalIncludes

//## begin module%39606F190397.includes preserve=yes
#include "m4credentials_dll.hpp"
//## end module%39606F190397.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
//## begin module%39606F190397.declarations preserve=no
//## end module%39606F190397.declarations

//## begin module%39606F190397.additionalDeclarations preserve=yes
//## end module%39606F190397.additionalDeclarations


//## Class: ISessionIDGenerator%39606CF80253
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3965AA7700E1;m4string_t { -> }
//## Uses: <unnamed>%3965AA7B02C7;m4date_t { -> }
//## Uses: <unnamed>%39AB913C0165;ClIdGenerator { -> }

class M4_DECL_M4CREDENTIALS ISessionIDGenerator
{
  public:

    //## Other Operations (specified)
      //## Operation: CreateSessionID%962621265
      virtual m4return_t CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const = 0;

      //## Operation: ExtractVariables%962621266
      virtual m4return_t ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const = 0;

    // Data Members for Class Attributes

      //## Attribute: M4DateExpired%396B4051031A
      //## begin ISessionIDGenerator::M4DateExpired%396B4051031A.attr preserve=no  public: static m4return_t {VAC} -2
      static const m4return_t  M4DateExpired;
      //## end ISessionIDGenerator::M4DateExpired%396B4051031A.attr

  protected:
  private:
  private: //## implementation
};

//## Class: ClDefaultSessionIDGenerator%39606DAA00A0
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CREDENTIALS ClDefaultSessionIDGenerator : public ISessionIDGenerator  //## Inherits: <unnamed>%39606DB50205
{
  public:
    //## Constructors (specified)
      //## Operation: ClDefaultSessionIDGenerator%962621267
      ClDefaultSessionIDGenerator ();


    //## Other Operations (specified)
      //## Operation: CreateSessionID%962621268
      virtual m4return_t CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const;

      //## Operation: ExtractVariables%962621269
      virtual m4return_t ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const;

    // Data Members for Associations

      //## Association: M4Credentials::<unnamed>%39606E1700BB
      //## Role: ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator%39606E180244
      //## begin ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator%39606E180244.role preserve=no  public: static ClDefaultSessionIDGenerator { -> 1VHANC}
      static const ClDefaultSessionIDGenerator  M4DefaultSessionIDGenerator;
      //## end ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator%39606E180244.role

  protected:
  private:
  private: //## implementation
};

//## begin module%39606F190397.epilog preserve=yes
//## end module%39606F190397.epilog


#endif
