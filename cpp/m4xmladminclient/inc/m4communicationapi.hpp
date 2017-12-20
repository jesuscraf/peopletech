//## begin module%39323E03037D.cm preserve=no
//## end module%39323E03037D.cm

//## begin module%39323E03037D.cp preserve=no
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
//## end module%39323E03037D.cp

//## Module: M4CommunicationAPI%39323E03037D; Package specification
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Source file: D:\INTEGRATION\m4xmladminclient\inc\m4communicationapi.hpp

#ifndef m4communicationapi_h
#define m4communicationapi_h 1

//## begin module%39323E03037D.additionalIncludes preserve=no
//## end module%39323E03037D.additionalIncludes

//## begin module%39323E03037D.includes preserve=yes
#include "m4xmladminclient_dll.hpp"
#include "basiclog.hpp"
#include "m4srvres.hpp"
//## end module%39323E03037D.includes

// m4condition
#include <m4condition.hpp>
// M4XmlOutStream
#include <m4xmloutstream.hpp>
// M4SAXAdmClient
#include <m4saxadmclient.hpp>
// iexecutable
#include <iexecutable.hpp>
// ParamList
#include <paramlist.hpp>
// m4communication
#include <m4communication.hpp>
// clarraylibres
#include <clarraylibres.hpp>
//## begin module%39323E03037D.declarations preserve=no
//## end module%39323E03037D.declarations

//## begin module%39323E03037D.additionalDeclarations preserve=yes
//## end module%39323E03037D.additionalDeclarations


//## begin ClCommunicationAPI%39323C3C00DA.preface preserve=yes
//## end ClCommunicationAPI%39323C3C00DA.preface

//## Class: ClCommunicationAPI%39323C3C00DA
//## Category: M4XMLAdminClient%39084C2003A4
//## Subsystem: M4XMLAdminClient::inc%39084CFD02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3934E16D02F9;ClCommunication { -> }
//## Uses: <unnamed>%393B9AF5015F;M4SAXAdmClient { -> }
//## Uses: <unnamed>%393BD4FF0359;ClIdGenerator { -> }
//## Uses: <unnamed>%3958D5680319;ClXMLOutStream { -> }

class M4_DECL_M4XMLADMINCLIENT ClCommunicationAPI : public ClExecutableInterface  //## Inherits: <unnamed>%39323F3C0228
{
  //## begin ClCommunicationAPI%39323C3C00DA.initialDeclarations preserve=yes
  //## end ClCommunicationAPI%39323C3C00DA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommunicationAPI%959594566
      ClCommunicationAPI ();

    //## Destructor (specified)
      //## Operation: ~ClCommunicationAPI%959594567
      ~ClCommunicationAPI ();


    //## Other Operations (specified)
      //## Operation: ExecuteAction%959594568
      virtual m4return_t ExecuteAction (m4pchar_t ai_pcActionName, ClParamList *ai_pParams);

      //## Operation: ReadReady%959766908
      void ReadReady ();

      //## Operation: WriteReady%959766909
      void WriteReady ();

      //## Operation: WaitWrite%959766910
      void WaitWrite ();

      //## Operation: WaitRead%959766911
      void WaitRead ();

      //## Operation: Clear%959766912
      void Clear ();

      //## Operation: Communicate%959869670
      m4return_t Communicate (char *ai_szBuffer);

      //## Operation: GetBuffer%960193450
      m4pchar_t GetBuffer ();

    // Data Members for Associations

      //## Association: M4XMLAdminClient::<unnamed>%3934F1B900F9
      //## Role: ClCommunicationAPI::<m_pClCondition>%3934F1B903CA
      //## begin ClCommunicationAPI::<m_pClCondition>%3934F1B903CA.role preserve=no  public: ClCondition { -> 1VHAN}
      ClCondition m_pClCondition;
      //## end ClCommunicationAPI::<m_pClCondition>%3934F1B903CA.role

    // Additional Public Declarations
      //## begin ClCommunicationAPI%39323C3C00DA.public preserve=yes
      //## end ClCommunicationAPI%39323C3C00DA.public

  protected:
    // Data Members for Associations

      //## Association: M4XMLAdminClient::<unnamed>%3934EF5F025C
      //## Role: ClCommunicationAPI::m_pReadParams%3934EF600091
      //## begin ClCommunicationAPI::m_pReadParams%3934EF600091.role preserve=no  protected: ClParamList { -> 1RHAN}
      ClParamList *m_pReadParams;
      //## end ClCommunicationAPI::m_pReadParams%3934EF600091.role

      //## Association: M4XMLAdminClient::<unnamed>%3934F0840113
      //## Role: ClCommunicationAPI::m_pWriteParams%3934F0840326
      //## begin ClCommunicationAPI::m_pWriteParams%3934F0840326.role preserve=no  protected: ClParamList { -> 1RHAN}
      ClParamList *m_pWriteParams;
      //## end ClCommunicationAPI::m_pWriteParams%3934F0840326.role

      //## Association: M4XMLAdminClient::<unnamed>%3934F27B0315
      //## Role: ClCommunicationAPI::m_ReadCondition%3934F27C0226
      //## begin ClCommunicationAPI::m_ReadCondition%3934F27C0226.role preserve=no  protected: ClCondition { -> 1VHAN}
      ClCondition m_ReadCondition;
      //## end ClCommunicationAPI::m_ReadCondition%3934F27C0226.role

      //## Association: M4XMLAdminClient::<unnamed>%3934F2E70021
      //## Role: ClCommunicationAPI::m_WriteCondition%3934F2E702FC
      //## begin ClCommunicationAPI::m_WriteCondition%3934F2E702FC.role preserve=no  protected: ClCondition { -> 1VHAN}
      ClCondition m_WriteCondition;
      //## end ClCommunicationAPI::m_WriteCondition%3934F2E702FC.role

    // Additional Protected Declarations
      //## begin ClCommunicationAPI%39323C3C00DA.protected preserve=yes
      //## end ClCommunicationAPI%39323C3C00DA.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcBuffer%3934EA0201C2
      //## begin ClCommunicationAPI::m_pcBuffer%3934EA0201C2.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pcBuffer;
      //## end ClCommunicationAPI::m_pcBuffer%3934EA0201C2.attr

      //## Attribute: m_bReadyRead%3934EAFF01BB
      //## begin ClCommunicationAPI::m_bReadyRead%3934EAFF01BB.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bReadyRead;
      //## end ClCommunicationAPI::m_bReadyRead%3934EAFF01BB.attr

      //## Attribute: m_bReadyWrite%3934EB560102
      //## begin ClCommunicationAPI::m_bReadyWrite%3934EB560102.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bReadyWrite;
      //## end ClCommunicationAPI::m_bReadyWrite%3934EB560102.attr

      //## Attribute: m_iSessionID%3934EB8C01C8
      //## begin ClCommunicationAPI::m_iSessionID%3934EB8C01C8.attr preserve=no  private: m4int_t {UA} 
      m4int_t m_iSessionID;
      //## end ClCommunicationAPI::m_iSessionID%3934EB8C01C8.attr

      //## Attribute: m_iCommStatus%39635E6E03A1
      //## begin ClCommunicationAPI::m_iCommStatus%39635E6E03A1.attr preserve=no  private: m4return_t {UA} M4_ERROR
      m4return_t m_iCommStatus;
      //## end ClCommunicationAPI::m_iCommStatus%39635E6E03A1.attr

    // Additional Private Declarations
      //## begin ClCommunicationAPI%39323C3C00DA.private preserve=yes
      //## end ClCommunicationAPI%39323C3C00DA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommunicationAPI%39323C3C00DA.implementation preserve=yes
      //## end ClCommunicationAPI%39323C3C00DA.implementation

};

//## begin ClCommunicationAPI%39323C3C00DA.postscript preserve=yes
//## end ClCommunicationAPI%39323C3C00DA.postscript

// Class ClCommunicationAPI 

//## begin module%39323E03037D.epilog preserve=yes
//## end module%39323E03037D.epilog


#endif
