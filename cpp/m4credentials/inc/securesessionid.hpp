//## begin module%396C91870291.cm preserve=no
//## end module%396C91870291.cm

//## begin module%396C91870291.cp preserve=no
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
//## end module%396C91870291.cp

//## Module: SecureSessionID%396C91870291; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: F:\integration\m4credentials\inc\securesessionid.hpp

#ifndef securesessionid_h
#define securesessionid_h 1

//## begin module%396C91870291.additionalIncludes preserve=no
//## end module%396C91870291.additionalIncludes

//## begin module%396C91870291.includes preserve=yes
//## end module%396C91870291.includes

// RawSessionID
#include <rawsessionid.hpp>
//## begin module%396C91870291.declarations preserve=no
//## end module%396C91870291.declarations

//## begin module%396C91870291.additionalDeclarations preserve=yes
//## end module%396C91870291.additionalDeclarations


//## begin ClSecureSessionIDGenerator%396C909903A7.preface preserve=yes
//## end ClSecureSessionIDGenerator%396C909903A7.preface

//## Class: ClSecureSessionIDGenerator%396C909903A7
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%396DDE56014E;TStringUtils { -> F}

class M4_DECL_M4CREDENTIALS ClSecureSessionIDGenerator : public ClRawSessionIDGenerator  //## Inherits: <unnamed>%396C90CD00C7
{
  //## begin ClSecureSessionIDGenerator%396C909903A7.initialDeclarations preserve=yes
  //## end ClSecureSessionIDGenerator%396C909903A7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSecureSessionIDGenerator%963411916
      ClSecureSessionIDGenerator (m4uint_t ai_iCipherNPasses = 1);


    //## Other Operations (specified)
      //## Operation: CreateSessionID%963411917
      virtual m4return_t CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const;

      //## Operation: ExtractVariables%963411918
      virtual m4return_t ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const;

    // Additional Public Declarations
      //## begin ClSecureSessionIDGenerator%396C909903A7.public preserve=yes
      //## end ClSecureSessionIDGenerator%396C909903A7.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iCipherNPasses%396EFDA30101
      //## begin ClSecureSessionIDGenerator::m_iCipherNPasses%396EFDA30101.attr preserve=no  protected: m4uint_t {VA} 
      m4uint_t m_iCipherNPasses;
      //## end ClSecureSessionIDGenerator::m_iCipherNPasses%396EFDA30101.attr

      //## Attribute: M4MaxCipherNPasses%396EFDD603C1
      //## begin ClSecureSessionIDGenerator::M4MaxCipherNPasses%396EFDD603C1.attr preserve=no  protected: static m4uint_t {VAC} 4
      static const m4uint_t  M4MaxCipherNPasses;
      //## end ClSecureSessionIDGenerator::M4MaxCipherNPasses%396EFDD603C1.attr

    // Additional Protected Declarations
      //## begin ClSecureSessionIDGenerator%396C909903A7.protected preserve=yes
      //## end ClSecureSessionIDGenerator%396C909903A7.protected

  private:
    // Additional Private Declarations
      //## begin ClSecureSessionIDGenerator%396C909903A7.private preserve=yes
      //## end ClSecureSessionIDGenerator%396C909903A7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSecureSessionIDGenerator%396C909903A7.implementation preserve=yes
      //## end ClSecureSessionIDGenerator%396C909903A7.implementation

};

//## begin ClSecureSessionIDGenerator%396C909903A7.postscript preserve=yes
//## end ClSecureSessionIDGenerator%396C909903A7.postscript

// Class ClSecureSessionIDGenerator 

//## begin module%396C91870291.epilog preserve=yes
//## end module%396C91870291.epilog


#endif
