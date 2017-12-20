//## begin module%3A801D8900D2.cm preserve=no
//## end module%3A801D8900D2.cm

//## begin module%3A801D8900D2.cp preserve=no
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
//## end module%3A801D8900D2.cp

//## Module: appctlreq%3A801D8900D2; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: F:\integration\M4Appctl\inc\appctlreq.hpp

#ifndef appctlreq_h
#define appctlreq_h 1

//## begin module%3A801D8900D2.additionalIncludes preserve=no
//## end module%3A801D8900D2.additionalIncludes

//## begin module%3A801D8900D2.includes preserve=yes
//## end module%3A801D8900D2.includes

// logsys
#include <logsys.hpp>
// m4types
#include <m4types.hpp>
//## begin module%3A801D8900D2.declarations preserve=no
//## end module%3A801D8900D2.declarations

//## begin module%3A801D8900D2.additionalDeclarations preserve=yes
//## end module%3A801D8900D2.additionalDeclarations


//## begin ClAppCtlRequest%3A801AFD02F1.preface preserve=yes
//## end ClAppCtlRequest%3A801AFD02F1.preface

//## Class: ClAppCtlRequest%3A801AFD02F1
//## Category: M4Appctl::Request%3A801AEC03D3
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAppCtlRequest 
{
  //## begin ClAppCtlRequest%3A801AFD02F1.initialDeclarations preserve=yes
  //## end ClAppCtlRequest%3A801AFD02F1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppCtlRequest%981474748
      ClAppCtlRequest (m4pchar_t ai_szBuffer, m4uint32_t ai_iConnectionID);

    //## Destructor (specified)
      //## Operation: ~ClAppCtlRequest%981474749
      ~ClAppCtlRequest ();


    //## Other Operations (specified)
      //## Operation: GetBufferOut%981474750
      m4pchar_t GetBufferOut ();

      //## Operation: GenerateAnwer%981474751
      m4pchar_t GenerateAnwer ();

      //## Operation: SetResult%981474752
      void SetResult (m4return_t ai_iResult);

      //## Operation: GetBufferIn%981474753
      m4pchar_t GetBufferIn ();

      //## Operation: SetRequestID%981474755
      void SetRequestID (m4uint32_t ai_iRequestID);

      //## Operation: GetLog%982066311
      ClLogSystem * GetLog ();

      //## Operation: GetConnectionID%998490241
      m4uint32_t GetConnectionID ();

    // Additional Public Declarations
      //## begin ClAppCtlRequest%3A801AFD02F1.public preserve=yes
	m4pchar_t GenerateDispatcherUpdate (m4pchar_t ai_szConfigName , m4pchar_t ai_szAppCtlHost , m4uint32_t ai_iAppCtlPort) ;

	  void SetConnectionID ( m4uint32_t iConnectionId ) ;

	  ClAppCtlRequest () ;
      //## end ClAppCtlRequest%3A801AFD02F1.public

  protected:
    // Additional Protected Declarations
      //## begin ClAppCtlRequest%3A801AFD02F1.protected preserve=yes
      //## end ClAppCtlRequest%3A801AFD02F1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iResult%3A801CF00399
      //## begin ClAppCtlRequest::m_iResult%3A801CF00399.attr preserve=no  private: m4return_t {UA} 
      m4return_t m_iResult;
      //## end ClAppCtlRequest::m_iResult%3A801CF00399.attr

      //## Attribute: m_iRequestID%3A801D2F00FB
      //## begin ClAppCtlRequest::m_iRequestID%3A801D2F00FB.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iRequestID;
      //## end ClAppCtlRequest::m_iRequestID%3A801D2F00FB.attr

      //## Attribute: m_iConnectionID%3A801E3F0160
      //## begin ClAppCtlRequest::m_iConnectionID%3A801E3F0160.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iConnectionID;
      //## end ClAppCtlRequest::m_iConnectionID%3A801E3F0160.attr

    // Data Members for Associations

      //## Association: M4Appctl::Request::<unnamed>%3A801B9E01EE
      //## Role: ClAppCtlRequest::m_szBufferIn%3A801B9E03E4
      //## begin ClAppCtlRequest::m_szBufferIn%3A801B9E03E4.role preserve=no  private: m4pchar_t { -> VHAN}
      m4pchar_t m_szBufferIn;
      //## end ClAppCtlRequest::m_szBufferIn%3A801B9E03E4.role

      //## Association: M4Appctl::Request::<unnamed>%3A801BBC01FC
      //## Role: ClAppCtlRequest::m_szBufferOut%3A801BBD01C1
      //## begin ClAppCtlRequest::m_szBufferOut%3A801BBD01C1.role preserve=no  private: m4pchar_t { -> VHAN}
      m4pchar_t m_szBufferOut;
      //## end ClAppCtlRequest::m_szBufferOut%3A801BBD01C1.role

      //## Association: M4Appctl::Request::<unnamed>%3A803D21023B
      //## Role: ClAppCtlRequest::m_poLog%3A803D2201E2
      //## begin ClAppCtlRequest::m_poLog%3A803D2201E2.role preserve=no  private: ClLogSystem { -> RHAN}
      ClLogSystem *m_poLog;
      //## end ClAppCtlRequest::m_poLog%3A803D2201E2.role

    // Additional Private Declarations
      //## begin ClAppCtlRequest%3A801AFD02F1.private preserve=yes
      //## end ClAppCtlRequest%3A801AFD02F1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppCtlRequest%3A801AFD02F1.implementation preserve=yes
      //## end ClAppCtlRequest%3A801AFD02F1.implementation

};

//## begin ClAppCtlRequest%3A801AFD02F1.postscript preserve=yes
//## end ClAppCtlRequest%3A801AFD02F1.postscript

// Class ClAppCtlRequest 

//## begin module%3A801D8900D2.epilog preserve=yes
//## end module%3A801D8900D2.epilog


#endif
