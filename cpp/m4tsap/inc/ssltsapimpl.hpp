//## begin module%3777684302A4.cm preserve=no
//## end module%3777684302A4.cm

//## begin module%3777684302A4.cp preserve=no
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
//## end module%3777684302A4.cp

//## Module: ssltsapimpl%3777684302A4; Package specification
//## Subsystem: M4Tsap::inc%37776840008D
//	d:\compon\m4tsap\version\inc
//## Source file: d:\compon\m4tsap\version\inc\ssltsapimpl.hpp

#ifndef ssltsapimpl_h
#define ssltsapimpl_h 1

//## begin module%3777684302A4.additionalIncludes preserve=no
//## end module%3777684302A4.additionalIncludes

//## begin module%3777684302A4.includes preserve=yes

#include <m4types.hpp>
#include "tsap.hpp"
#include "tsapimpl.hpp"
#define MAX_SIZE_SSL	1024*16
//#define MAX_SIZE_SSL	1024*8


/* Cosas de SSLeay */

/* Esto sirve para que el compilador no se queje de que el prototipo de
 * crypt() está en des.h y en unistd.h. De todas formas, la solución de
 * verdad es quitarlo de des.h (o mejor, dejarlo y usarlo sólo en los Unix
 * que no tengan prototipo de crypt() en sus .h estándar).
 */
#define HEADER_DES_LOCL_H
/*
#include "rsa.h"
#include "crypto.h"
#include "x509.h"
#include "pem.h"
#include "ssl.h"
#include "err.h"
*/
#include "openssl/ssl.h"
//## end module%3777684302A4.includes

//## begin module%3777684302A4.declarations preserve=no
//## end module%3777684302A4.declarations

//## begin module%3777684302A4.additionalDeclarations preserve=yes
//## end module%3777684302A4.additionalDeclarations


//## begin ClSSLTSAPImpl%3777684302F5.preface preserve=yes
//## end ClSSLTSAPImpl%3777684302F5.preface

//## Class: ClSSLTSAPImpl%3777684302F5
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3777684400C6;ClTSAPImpl { -> }
//## Uses: <unnamed>%3777684400D2;ClInetAddress { -> }
//## Uses: <unnamed>%3777684400DD;m4bool_t { -> }

class ClSSLTSAPImpl : public ClTSAPImpl  //## Inherits: <unnamed>%377768440189
{
  //## begin ClSSLTSAPImpl%3777684302F5.initialDeclarations preserve=yes
  //## end ClSSLTSAPImpl%3777684302F5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSSLTSAPImpl%1820327814; C++
      ClSSLTSAPImpl (int ai_iMode, void* ai_pData);

      ~ClSSLTSAPImpl ( );


    //## Other Operations (specified)
      //## Operation: Accept%-1529727978; C++
      int Accept (ClTSAPImpl* ao_poTSAPImpl);

      //## Operation: Bind%1916733128; C++
      int Bind (ClInetAddress* ai_poInetAddress, int lport);

      //## Operation: Close%-1694688536; C++
      void Close ();

      //## Operation: Connect%-1656302624; C++
      int Connect (const char* ai_pcHostname, int ai_iPort);

      //## Operation: Connect%588994194; C++
      int Connect (ClInetAddress* ai_InetAddress, int ai_iPort);

      //## Operation: Create%1176852083; C++
      int Create (m4bool_t ai_bStream = M4_TRUE);

      //## Operation: Listen%-1098192822; C++
      int Listen (int ai_bBacklog);

      //## Operation: Write%-1444415595; C++
      int Write (const void* src, int len);

      //## Operation: Read%-552243267; C++
      int Read (void* dst, int len);

      //## Operation: SetBlockingIO%-125221782; C++
      int SetBlockingIO (m4bool_t ai_bBlocking = M4_TRUE);

      //## Operation: GetIdTSAP%1391325878; C++
      int GetIdTSAP ()
      {
        //## begin ClSSLTSAPImpl::GetIdTSAP%1391325878.body preserve=yes
		  return m_iSocket;
        //## end ClSSLTSAPImpl::GetIdTSAP%1391325878.body
      }

      //## Operation: GetSystemDescriptor%-1599726438; C++
      int GetSystemDescriptor ()
      {
        //## begin ClSSLTSAPImpl::GetSystemDescriptor%-1599726438.body preserve=yes
		return m_iSocket;
        //## end ClSSLTSAPImpl::GetSystemDescriptor%-1599726438.body
      }

    // Additional Public Declarations
      //## begin ClSSLTSAPImpl%3777684302F5.public preserve=yes
      //## end ClSSLTSAPImpl%3777684302F5.public

  protected:
    // Additional Protected Declarations
      //## begin ClSSLTSAPImpl%3777684302F5.protected preserve=yes
	   m4int_t m_iCeroCounter;
      //## end ClSSLTSAPImpl%3777684302F5.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iSocket%377768440104
      //## begin ClSSLTSAPImpl::m_iSocket%377768440104.attr preserve=no  private: int {VA} 
      int m_iSocket;
      //## end ClSSLTSAPImpl::m_iSocket%377768440104.attr

      //## Attribute: m_iMode%377768440187
      //## begin ClSSLTSAPImpl::m_iMode%377768440187.attr preserve=no  private: int {VA} 
      int m_iMode;
      //## end ClSSLTSAPImpl::m_iMode%377768440187.attr

      //## Attribute: m_pData%377768440188
      //## begin ClSSLTSAPImpl::m_pData%377768440188.attr preserve=no  private: void* {VA} 
      void* m_pData;
      //## end ClSSLTSAPImpl::m_pData%377768440188.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%377768440184
      //## Role: ClSSLTSAPImpl::m_ssl%377768440186
      //## begin ClSSLTSAPImpl::m_ssl%377768440186.role preserve=no  private: SSL {1 -> 1RHAPN}
      SSL *m_ssl;
      //## end ClSSLTSAPImpl::m_ssl%377768440186.role

    // Additional Private Declarations
      //## begin ClSSLTSAPImpl%3777684302F5.private preserve=yes
      //## end ClSSLTSAPImpl%3777684302F5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSLTSAPImpl%3777684302F5.implementation preserve=yes

      //## end ClSSLTSAPImpl%3777684302F5.implementation

};

//## begin ClSSLTSAPImpl%3777684302F5.postscript preserve=yes
//## end ClSSLTSAPImpl%3777684302F5.postscript

//## begin module%3777684302A4.epilog preserve=yes

int SocketInit(int ai_iModeSSL, void *ai_pData);
void SocketFinish(int ai_iModeSSL);
//## end module%3777684302A4.epilog


#endif
