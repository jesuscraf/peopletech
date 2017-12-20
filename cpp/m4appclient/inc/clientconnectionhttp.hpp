//## begin module%42931E340270.cm preserve=no
//## end module%42931E340270.cm

//## begin module%42931E340270.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%42931E340270.cp

//## Module: clientconnectionhttp%42931E340270; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: C:\m4server\m4appclient\inc\clientconnectionhttp.hpp

#ifndef clientconnectionhttp_h
#define clientconnectionhttp_h 1

//## begin module%42931E340270.additionalIncludes preserve=no
//## end module%42931E340270.additionalIncludes

//## begin module%42931E340270.includes preserve=yes
//## end module%42931E340270.includes

// clientconnection
#include <clientconnection.hpp>
// parsehttp
#include <parsehttp.hpp>
// m4string
#include <m4string.hpp>
//## begin module%42931E340270.declarations preserve=no
//## end module%42931E340270.declarations

//## begin module%42931E340270.additionalDeclarations preserve=yes
//## end module%42931E340270.additionalDeclarations


//## begin ClCCConnectionHTTP%36EFA41302E1.preface preserve=yes
//## end ClCCConnectionHTTP%36EFA41302E1.preface

//## Class: ClCCConnectionHTTP%36EFA41302E1
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCCConnectionHTTP : public ClCCConnection  //## Inherits: <unnamed>%36EFA4190127
{
  //## begin ClCCConnectionHTTP%36EFA41302E1.initialDeclarations preserve=yes
  //## end ClCCConnectionHTTP%36EFA41302E1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCConnectionHTTP%921676533; C++
      ClCCConnectionHTTP (M4ClString ai_strHost, m4int_t ai_iHostPort, M4ClString ai_strServer, m4int_t ai_iServerPort, M4ClString ai_strServlet, m4bool_t ai_bSSL, M4ClString ai_strCACertPath);

    //## Destructor (specified)
      //## Operation: ~ClCCConnectionHTTP%926436108; C++
      ~ClCCConnectionHTTP ();


    //## Other Operations (specified)
      //## Operation: ProccessHeaders%921676535; C++
      m4return_t ProccessHeaders (m4pchar_t &ao_pcBuffer, m4uint32_t &ao_uiBufferSize);

      //## Operation: CreateHeaders%921676539; C++
      m4return_t CreateHeaders (m4pchar_t ao_pcBuffer, m4uint32_t &ao_iBufferSize, m4uint32_t ai_uiPDUSize, m4uint32_t ai_uiTimeout, m4handle_t ai_hRID = 0);

      //## Operation: GetNewTSAP%922184352; C++
      m4return_t GetNewTSAP (ClTSAP *&ao_pTSAP, m4uint32_t ai_uiPDUSize, m4uint32_t ai_uiTimeout, m4handle_t ai_hRID = 0);

      //## Operation: SendHeaders%922184353; C++
      m4return_t SendHeaders (m4pchar_t ai_pcBuffer, m4uint32_t ai_uiHeaderSize);

      //## Operation: GetConnType%923902843
      //	Devuelve un 1 si es ClCConnectionHTTP
      m4char_t GetConnType ();

      //## Operation: _GetAnswerPDU%951384406; C++
      m4return_t _GetAnswerPDU (ClPDU *&ao_pPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout = -1, m4bool_t ai_bRetrying = false, m4pchar_t ai_pcBuffer = NULL, m4uint_t ai_uiBufferSize = 0);

      //## Operation: _SendRawData%951410088; C++
      m4return_t _SendRawData (ClCCRequest *ai_pRequest);

    // Additional Public Declarations
      //## begin ClCCConnectionHTTP%36EFA41302E1.public preserve=yes
      //## end ClCCConnectionHTTP%36EFA41302E1.public

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3739970F01C5
      //## Role: ClCCConnectionHTTP::m_Parser%373997100113
      //## begin ClCCConnectionHTTP::m_Parser%373997100113.role preserve=no  protected: ClHTTPParser {1 -> 1UHAN}
      ClHTTPParser m_Parser;
      //## end ClCCConnectionHTTP::m_Parser%373997100113.role

      //## Association: M4AppClient::<unnamed>%381804600192
      //## Role: ClCCConnectionHTTP::m_strServlet%381804610021
      //## begin ClCCConnectionHTTP::m_strServlet%381804610021.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strServlet;
      //## end ClCCConnectionHTTP::m_strServlet%381804610021.role

      //## Association: M4AppClient::<unnamed>%3818047F0151
      //## Role: ClCCConnectionHTTP::m_strPrivCookie%3818047F02FF
      //## begin ClCCConnectionHTTP::m_strPrivCookie%3818047F02FF.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strPrivCookie;
      //## end ClCCConnectionHTTP::m_strPrivCookie%3818047F02FF.role

      //## Association: M4AppClient::<unnamed>%3818049D03C1
      //## Role: ClCCConnectionHTTP::m_strPublicCookie%3818049E0368
      //## begin ClCCConnectionHTTP::m_strPublicCookie%3818049E0368.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strPublicCookie;
      //## end ClCCConnectionHTTP::m_strPublicCookie%3818049E0368.role

      //## Association: M4AppClient::<unnamed>%381804C800BF
      //## Role: ClCCConnectionHTTP::m_strServer%381804C9007B
      //## begin ClCCConnectionHTTP::m_strServer%381804C9007B.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strServer;
      //## end ClCCConnectionHTTP::m_strServer%381804C9007B.role

    // Additional Protected Declarations
      //## begin ClCCConnectionHTTP%36EFA41302E1.protected preserve=yes
      //## end ClCCConnectionHTTP%36EFA41302E1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iServerPort%3742F65A00ED
      //## begin ClCCConnectionHTTP::m_iServerPort%3742F65A00ED.attr preserve=no  private: m4int_t {UA} 
      m4int_t m_iServerPort;
      //## end ClCCConnectionHTTP::m_iServerPort%3742F65A00ED.attr

    // Additional Private Declarations
      //## begin ClCCConnectionHTTP%36EFA41302E1.private preserve=yes
      //## end ClCCConnectionHTTP%36EFA41302E1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCConnectionHTTP%36EFA41302E1.implementation preserve=yes
      //## end ClCCConnectionHTTP%36EFA41302E1.implementation

};

//## begin ClCCConnectionHTTP%36EFA41302E1.postscript preserve=yes
//## end ClCCConnectionHTTP%36EFA41302E1.postscript

// Class ClCCConnectionHTTP 

//## begin module%42931E340270.epilog preserve=yes
//## end module%42931E340270.epilog


#endif
