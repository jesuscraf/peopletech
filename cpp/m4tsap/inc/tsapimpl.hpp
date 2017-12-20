//## begin module%3777684302CC.cm preserve=no
//## end module%3777684302CC.cm

//## begin module%3777684302CC.cp preserve=no
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
//## end module%3777684302CC.cp

//## Module: tsapimpl%3777684302CC; Package specification
//## Subsystem: M4Tsap::inc%37776840008D
//	d:\compon\m4tsap\version\inc
//## Source file: d:\compon\m4tsap\version\inc\tsapimpl.hpp

#ifndef tsapimpl_h
#define tsapimpl_h 1

//## begin module%3777684302CC.additionalIncludes preserve=no
//## end module%3777684302CC.additionalIncludes

//## begin module%3777684302CC.includes preserve=yes

#include "m4types.hpp"
//## end module%3777684302CC.includes

// tsap
#include <tsap.hpp>
//## begin module%3777684302CC.declarations preserve=no
//## end module%3777684302CC.declarations

//## begin module%3777684302CC.additionalDeclarations preserve=yes

#define	M4TSAP_STR_NOT_INITIALIZED					"The member m_poTSAPImpl isn't properly initilized"
#define	M4TSAP_STR_OK								"OK"
#define	M4TSAP_STR_NOT_IMPLEMENTED					"Option not implemented"		
#define	M4TSAP_STR_NOT_KNOWN						"Unknown option"		
#define	M4TSAP_STR_SOCKET_FAILED					"Function socket() failed"		
#define	M4TSAP_STR_BIND_FAILED						"Function bind() failed" 	
#define	M4TSAP_STR_ACCEPT_FAILED					"Function accept() failed"		
#define	M4TSAP_STR_CONNECT_FAILED					"Function connect failed"		
#define	M4TSAP_STR_LISTEN_FAILED					"Function listen() failed"		
#define	M4TSAP_STR_WRITE_FAILED						"Function write() failed"	
#define	M4TSAP_STR_READ_FAILED						"Function read() failed"
#define	M4TSAP_STR_INVALID_HOSTNAME					"Invalid hostname"	
#define	M4TSAP_STR_INVALID_INET_ADDRESS				"Invalid IP Address"

#define	M4TSAP_STR_CERTIFICATE_CA					"Certificate CA invalid"
#define	M4TSAP_STR_INVALID_PRIVATE_KEY				"Invalid private key"		
#define	M4TSAP_STR_INVALID_CERTIFICATE				"Invalid certificate"	

#define	M4TSAP_STR_SSL_CLIENT_NOT_INITIALIZED		"SSL client structures not initialized"
#define	M4TSAP_STR_SSL_SERVER_NOT_INITIALIZED		"SSL server structures not initialized"
#define	M4TSAP_STR_SSL_SSL_NEW_FAILED				"Function SSL_new() failed"
#define	M4TSAP_STR_SSL_SET_CIPHER_LIST_FAILED		"Function SSL_set_cipher_list() failed"
#define	M4TSAP_STR_SSL_ACCEPT_FAILED				"Function SSL_accept() failed"
#define	M4TSAP_STR_SSL_CONNECT_FAILED				"Function SSL_connect() failed"
#define	M4TSAP_STR_SSL_GET_PEER_CERTIFICATE_FAILED	"Function SSL_get_peer_certificate() failed"
#define	M4TSAP_STR_SSL_X509_NAME_ONLINE_FAILED		"Function X509_name_online() failed"
#define	M4TSAP_STR_SSL_M_SSL_NOT_INITIALIZED		"SSL connection structs not initalized"
#define	M4TSAP_STR_SSL_WRITE_FAILED					"Function SSL_write() failed"
#define	M4TSAP_STR_SSL_READ_FAILED					"Function SSL_read() failed"
#define M4TSAP_STR_ERROR_INVEROSIMIL				"Error inverosimil"

class	ClInetAddress;

//## end module%3777684302CC.additionalDeclarations


//## begin ClTSAPImpl%3777684400C5.preface preserve=yes
//## end ClTSAPImpl%3777684400C5.preface

//## Class: ClTSAPImpl%3777684400C5; Abstract
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3777684401C7;m4bool_t { -> }

class ClTSAPImpl 
{
  //## begin ClTSAPImpl%3777684400C5.initialDeclarations preserve=yes
  //## end ClTSAPImpl%3777684400C5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTSAPImpl%-660507616; C++
      ClTSAPImpl ();

    //## Destructor (specified)
      //## Operation: ~ClTSAPImpl%-1398153073; C++
      virtual ~ClTSAPImpl ();


    //## Other Operations (specified)
      //## Operation: Accept%809632346; C++
      virtual int Accept (ClTSAPImpl* newTSAP) = 0;

      //## Operation: Bind%-1130802475; C++
      virtual int Bind (ClInetAddress* ai_poInetAddress, int ai_iPort) = 0;

      //## Operation: Close%-275244212; C++
      virtual void Close () = 0;

      //## Operation: Connect%1649333805; C++
      virtual int Connect (const char* ai_pcHostname, int ai_iPort) = 0;

      //## Operation: Connect%-588804452; C++
      virtual int Connect (ClInetAddress* ai_poInetAddress, int ai_iPort) = 0;

      //## Operation: Create%-1858177015; C++
      virtual int Create (m4bool_t ai_bStream) = 0;

      //## Operation: Listen%441136820; C++
      virtual int Listen (int ai_iBacklog) = 0;

      //## Operation: Write%-312926416; C++
      virtual int Write (const void* ai_pcSrc, int ai_iLen) = 0;

      //## Operation: Read%876268434; C++
      virtual int Read (void* ao_pDst, int ai_iLen) = 0;

      //## Operation: SetBlockingIO%-2102505327; C++
      virtual int SetBlockingIO (m4bool_t ai_bBlocking = M4_TRUE) = 0;

      //## Operation: GetInetAddress%-200261066; C++
      virtual ClInetAddress* GetInetAddress () const;

      //## Operation: GetLocalPort%-595919322; C++
      virtual int GetLocalPort () const;

      //## Operation: GetPort%151074921; C++
      virtual int GetPort () const;

      //## Operation: ToString%-503072780; C++
      virtual char* ToString () const;

      //## Operation: GetSystemDescriptor%1974263940; C++
      virtual int GetSystemDescriptor () = 0;

      //## Operation: GetError%116183132; C++
      virtual int GetError ();

      //## Operation: GetStringError%-2079943342; C++
      virtual char* GetStringError ();

    // Additional Public Declarations
      //## begin ClTSAPImpl%3777684400C5.public preserve=yes
      //## end ClTSAPImpl%3777684400C5.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iPort%3777684401DD
      //## begin ClTSAPImpl::m_iPort%3777684401DD.attr preserve=no  protected: int {VA} 
      int m_iPort;
      //## end ClTSAPImpl::m_iPort%3777684401DD.attr

      //## Attribute: m_iLocalPort%3777684401DE
      //## begin ClTSAPImpl::m_iLocalPort%3777684401DE.attr preserve=no  protected: int {VA} 
      int m_iLocalPort;
      //## end ClTSAPImpl::m_iLocalPort%3777684401DE.attr

      //## Attribute: m_iError%3777684401E2
      //## begin ClTSAPImpl::m_iError%3777684401E2.attr preserve=no  protected: int {VA} 
      int m_iError;
      //## end ClTSAPImpl::m_iError%3777684401E2.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%3777684401DF
      //## Role: ClTSAPImpl::m_poInetAddress%3777684401E1
      //## begin ClTSAPImpl::m_poInetAddress%3777684401E1.role preserve=no  protected: ClInetAddress {1 -> 1RHAPN}
      ClInetAddress *m_poInetAddress;
      //## end ClTSAPImpl::m_poInetAddress%3777684401E1.role

    // Additional Protected Declarations
      //## begin ClTSAPImpl%3777684400C5.protected preserve=yes
      //## end ClTSAPImpl%3777684400C5.protected

  private:
    // Additional Private Declarations
      //## begin ClTSAPImpl%3777684400C5.private preserve=yes
      //## end ClTSAPImpl%3777684400C5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTSAPImpl%3777684400C5.implementation preserve=yes
      //## end ClTSAPImpl%3777684400C5.implementation

};

//## begin ClTSAPImpl%3777684400C5.postscript preserve=yes
//## end ClTSAPImpl%3777684400C5.postscript

// Class ClTSAPImpl 

//## begin module%3777684302CC.epilog preserve=yes
//## end module%3777684302CC.epilog


#endif
