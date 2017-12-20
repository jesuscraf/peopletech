//## begin module%42931EB40255.cm preserve=no
//## end module%42931EB40255.cm

//## begin module%42931EB40255.cp preserve=no
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
//## end module%42931EB40255.cp

//## Module: clientconnectionwininet%42931EB40255; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: C:\m4server\m4appclient\inc\clientconnectionwininet.hpp

#ifndef clientconnectionwininet_h
#define clientconnectionwininet_h 1

//## begin module%42931EB40255.additionalIncludes preserve=no
//## end module%42931EB40255.additionalIncludes

//## begin module%42931EB40255.includes preserve=yes
#include <WinINet.h>
#include "commprotocols.hpp"
//## end module%42931EB40255.includes

// clientconnection
#include <clientconnection.hpp>
// m4types
#include <m4types.hpp>
// m4condition
#include <m4condition.hpp>
// threads
#include <m4thread.hpp>
//## begin module%42931EB40255.declarations preserve=no
//## end module%42931EB40255.declarations

//## begin module%42931EB40255.additionalDeclarations preserve=yes
//## end module%42931EB40255.additionalDeclarations


//## begin ClWinInetConnection%423E92D70341.preface preserve=yes
//## end ClWinInetConnection%423E92D70341.preface

//## Class: ClWinInetConnection%423E92D70341
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%423E99330263;m4int32_t { -> }

// bg 125975
// Class ClWInetHandlesCloser
class ClWInetHandlesCloser: public M4Thread
{
	public:

		ClWInetHandlesCloser(HINTERNET ai_hSession, HINTERNET ai_hConnect, HINTERNET ai_hRequest);

	protected:

		HINTERNET m_hSession;
		HINTERNET m_hConnect;
		HINTERNET m_hRequest;	

	public:

		void Run(void);
};
// Class ClWInetHandlesCloser


class M4_DECL_M4APPCLIENT ClWinInetConnection : public ClCCConnection  //## Inherits: <unnamed>%42930D8B0189
{
  //## begin ClWinInetConnection%423E92D70341.initialDeclarations preserve=yes
  //## end ClWinInetConnection%423E92D70341.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClWinInetConnection%1111399686; C++
      ClWinInetConnection (M4ClString &ai_sHost, m4int32_t ai_iHostPort, const M4ClString &ai_sServerName, m4int32_t ai_iServerPort, const string &ai_sServletName, m4bool_t ai_bSSL, M4ClString &ai_strCACertPath);

    //## Destructor (specified)
      //## Operation: ~ClWinInetConnection%1111399687; C++
      virtual ~ClWinInetConnection ();


    //## Other Operations (specified)
      //## Operation: _GetAnswerData%1111399711; C++
      m4return_t _GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend);

      //## Operation: _SendRawData%1111399713; C++
      virtual m4return_t _SendRawData (ClCCRequest *ai_pRequest);

      //## Operation: _GetAnswerPDU%1111399716; C++
      virtual m4return_t _GetAnswerPDU (ClPDU *&ao_poPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout = -1, m4bool_t ai_bRetrying = false, m4pchar_t ai_pcBuffer = NULL, m4uint_t ai_uiBufferSize = 0);

      //## Operation: Reconnect%1115711669; C++
      m4return_t Reconnect (m4pchar_t ai_pcServerIP, m4int_t ai_iServerPort);

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%45EECBCA0235
      //## Role: ClWinInetConnection::m_pReqCondition%45EECBCB0061
      //## begin ClWinInetConnection::m_pReqCondition%45EECBCB0061.role preserve=no  public: ClCondition {1 -> RHAN}
      ClCondition *m_pReqCondition;
      //## end ClWinInetConnection::m_pReqCondition%45EECBCB0061.role

      //## Association: M4AppClient::<unnamed>%45EECD9A0270
      //## Role: ClWinInetConnection::m_pCloseCondition%45EECD9B003E
      //## begin ClWinInetConnection::m_pCloseCondition%45EECD9B003E.role preserve=no  public: ClCondition {1 -> RHAN}
      ClCondition *m_pCloseCondition;
      //## end ClWinInetConnection::m_pCloseCondition%45EECD9B003E.role

    // Additional Public Declarations
      //## begin ClWinInetConnection%423E92D70341.public preserve=yes
	  HINTERNET getRequestHandle() { return m_hRequest; }

	  void	setIgnoreSSLErrors( m4bool_t ai_bIgnoreSSLErrors )
	  {
		  m_bIgnoreSSLErrors = ai_bIgnoreSSLErrors;
	  }
      //## end ClWinInetConnection%423E92D70341.public

  protected:
    // Additional Protected Declarations
      //## begin ClWinInetConnection%423E92D70341.protected preserve=yes
      //## end ClWinInetConnection%423E92D70341.protected

  private:

    //## Other Operations (specified)
      //## Operation: BuildPDU%1112692646
      m4return_t BuildPDU (ClPDU *&ao_poPDU, m4pchar_t &aio_pcBuffer, m4uint32_t &aio_uiBufferSize);

      //## Operation: BuildURL%1115711668
      m4return_t BuildURL (const string &ai_sAppServerName, m4int32_t ai_iAppServerPort, m4bool_t ai_bUseSSL, string &aio_sURL);

      //## Operation: CheckHeaderInformation%1131442403
      m4return_t CheckHeaderInformation (m4int32_t &aio_iStatusCode, string &aio_sStatusMessage);

	  m4return_t _EndRequest(m4int32_t ai_iTimeout, m4bool_t ai_bRetrying);

    // Data Members for Class Attributes

      //## Attribute: m_hSession%42402DBE0207
      //## begin ClWinInetConnection::m_hSession%42402DBE0207.attr preserve=no  private: HINTERNET {UA} 
      HINTERNET m_hSession;
      //## end ClWinInetConnection::m_hSession%42402DBE0207.attr

      //## Attribute: m_hConnect%423ECE1B03D5
      //## begin ClWinInetConnection::m_hConnect%423ECE1B03D5.attr preserve=no  private: HINTERNET {UA} 
      HINTERNET m_hConnect;
      //## end ClWinInetConnection::m_hConnect%423ECE1B03D5.attr

      //## Attribute: m_hRequest%423FDA5F02E1
      //## begin ClWinInetConnection::m_hRequest%423FDA5F02E1.attr preserve=no  private: HINTERNET {UA} 
      HINTERNET m_hRequest;
      //## end ClWinInetConnection::m_hRequest%423FDA5F02E1.attr

      //## Attribute: m_sServletName%42402DC700CE
      //## begin ClWinInetConnection::m_sServletName%42402DC700CE.attr preserve=no  private: string {UA} 
      string m_sServletName;
      //## end ClWinInetConnection::m_sServletName%42402DC700CE.attr

      //## Attribute: m_sURL%4280C023020E
      //## begin ClWinInetConnection::m_sURL%4280C023020E.attr preserve=no  private: string {UA} 
      string m_sURL;
      //## end ClWinInetConnection::m_sURL%4280C023020E.attr

      //## Attribute: m_bSSL%4281E088030B
      //## begin ClWinInetConnection::m_bSSL%4281E088030B.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bSSL;
      //## end ClWinInetConnection::m_bSSL%4281E088030B.attr

      //## Attribute: m_sCACertPath%429192DC030A
      //## begin ClWinInetConnection::m_sCACertPath%429192DC030A.attr preserve=no  private: string {UA} 
      string m_sCACertPath;
      //## end ClWinInetConnection::m_sCACertPath%429192DC030A.attr

    // Additional Private Declarations
      //## begin ClWinInetConnection%423E92D70341.private preserve=yes
      ClCommPDUProtocol m_protocol;

	  M4ClString m_sHost;

	  m4int32_t m_iHostPort;

	  ClWInetHandlesCloser* m_pWInetHds;

	  m4bool_t	m_bIgnoreSSLErrors;

      //## end ClWinInetConnection%423E92D70341.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWinInetConnection%423E92D70341.implementation preserve=yes
      //## end ClWinInetConnection%423E92D70341.implementation

};

//## begin ClWinInetConnection%423E92D70341.postscript preserve=yes
m4return_t GetErrorMessage (m4int32_t &aio_iErrorCode, string &aio_sErrorMsg);

void CALLBACK InetCallbackFunc( HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength );
//## end ClWinInetConnection%423E92D70341.postscript

// Class ClWinInetConnection 

//## begin module%42931EB40255.epilog preserve=yes
//## end module%42931EB40255.epilog


#endif
