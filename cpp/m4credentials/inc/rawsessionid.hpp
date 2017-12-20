//## begin module%39633C160129.cm preserve=no
//## end module%39633C160129.cm

//## begin module%39633C160129.cp preserve=no
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
//## end module%39633C160129.cp

//## Module: RawSessionID%39633C160129; Subprogram specification
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Source file: F:\integration\m4credentials\inc\rawsessionid.hpp

#ifndef rawsessionid_h
#define rawsessionid_h 1

//## begin module%39633C160129.additionalIncludes preserve=no
//## end module%39633C160129.additionalIncludes

//## begin module%39633C160129.includes preserve=yes
#include "m4credentials_dll.hpp"
//## end module%39633C160129.includes

// SessionIDGenerator
#include <sessionidgenerator.hpp>
//## begin module%39633C160129.declarations preserve=no
//## end module%39633C160129.declarations

//## begin module%39633C160129.additionalDeclarations preserve=yes
//## end module%39633C160129.additionalDeclarations


//## begin ClRawSessionIDGenerator%39633B7B02DE.preface preserve=yes
//## end ClRawSessionIDGenerator%39633B7B02DE.preface

//## Class: ClRawSessionIDGenerator%39633B7B02DE
//## Category: M4Credentials%3959C61A006F
//## Subsystem: M4Credentials::inc%3959D0BB013A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3965C8B80305;ostrstream { -> F}
//## Uses: <unnamed>%396DDE5303D4;TStringUtils { -> F}

class M4_DECL_M4CREDENTIALS ClRawSessionIDGenerator : public ISessionIDGenerator  //## Inherits: <unnamed>%39633BBD031F
{
  //## begin ClRawSessionIDGenerator%39633B7B02DE.initialDeclarations preserve=yes
  //## end ClRawSessionIDGenerator%39633B7B02DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRawSessionIDGenerator%962964029
      ClRawSessionIDGenerator ();


    //## Other Operations (specified)
      //## Operation: CreateSessionID%962964030
      virtual m4return_t CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const;

      //## Operation: ExtractVariables%962964031
      virtual m4return_t ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const;

    // Additional Public Declarations
      //## begin ClRawSessionIDGenerator%39633B7B02DE.public preserve=yes
      //## end ClRawSessionIDGenerator%39633B7B02DE.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: M4SessionIDFieldSeparator%396DF2A8034D
      //## begin ClRawSessionIDGenerator::M4SessionIDFieldSeparator%396DF2A8034D.attr preserve=no  protected: static m4char_t {VAC} 3
      static const m4char_t  M4SessionIDFieldSeparator;
      //## end ClRawSessionIDGenerator::M4SessionIDFieldSeparator%396DF2A8034D.attr

      //## Attribute: M4SessionIDEndSeparator%396DF2D60335
      //## begin ClRawSessionIDGenerator::M4SessionIDEndSeparator%396DF2D60335.attr preserve=no  protected: static m4char_t {UAC} 4
      static const m4char_t  M4SessionIDEndSeparator;
      //## end ClRawSessionIDGenerator::M4SessionIDEndSeparator%396DF2D60335.attr

    // Additional Protected Declarations
      //## begin ClRawSessionIDGenerator%39633B7B02DE.protected preserve=yes
      //## end ClRawSessionIDGenerator%39633B7B02DE.protected

  private:
    // Additional Private Declarations
      //## begin ClRawSessionIDGenerator%39633B7B02DE.private preserve=yes
      //## end ClRawSessionIDGenerator%39633B7B02DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRawSessionIDGenerator%39633B7B02DE.implementation preserve=yes
      //## end ClRawSessionIDGenerator%39633B7B02DE.implementation

};

//## begin ClRawSessionIDGenerator%39633B7B02DE.postscript preserve=yes
//## end ClRawSessionIDGenerator%39633B7B02DE.postscript

// Class ClRawSessionIDGenerator 

//## begin module%39633C160129.epilog preserve=yes
//## end module%39633C160129.epilog


#endif
