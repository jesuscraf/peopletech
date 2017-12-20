//## begin module%355071B30232.cm preserve=no
//## end module%355071B30232.cm

//## begin module%355071B30232.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            UUSS
//	 File:              usservice.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980506
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%355071B30232.cp

//## Module: USService%355071B30232; Package specification
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Source file: D:\Work\v600\m4userinterface\inc\usservice.hpp

#ifndef usservice_h
#define usservice_h 1

//## begin module%355071B30232.additionalIncludes preserve=yes
#include <m4userinterface_dll.hpp>
#include <fifoq.hpp>
#include <serviceq.hpp>

class ClUSSessionInterface;
class ClConnInstance;
class ClLogSystem;


//## begin module%355071B30232.declarations preserve=no
//## end module%355071B30232.declarations

//## begin module%355071B30232.additionalDeclarations preserve=yes
#define		M4_US_CREATE_SESSION	0x00000000
#define		M4_US_AUTHENTICATE		0x00000001
#define		M4_US_CHECK_ROLE		0x00000002
#define		M4_US_CHANGE_PWD		0x00000003
#define		M4_US_CERT_LOGIN		0x00000004
#define		M4_US_GET_CERT			0x00000005
#define		M4_US_CLOSE_AUDITORY	0x00000006
#define		M4_US_CLEAN_AUDITORY	0x00000007
#define		M4_US_USR_AUDIT_LOGOFF	0x00000008	// User actions auditory.

#define		M4_RET_WAIT			M4_SUCCESS + 1
//## end module%355071B30232.additionalDeclarations


//## Class: ClUSQueue%356959020231
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3569594701D4;ClServiceQ { -> }

typedef ClServiceQ ClUSQueue;

//## begin ClUSServiceAPI%35507A7001B5.preface preserve=yes
//## end ClUSServiceAPI%35507A7001B5.preface

//## Class: ClUSServiceAPI%35507A7001B5
//## Category: M4UserInterface%35EFA1CC011D
//## Subsystem: M4UserInterface::inc%3795DCBF0108
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35507EA8007D;ClCommandRequest { -> F}
//## Uses: <unnamed>%354DCEC80358;ClUSUserInterface { -> F}
//## Uses: <unnamed>%356ACFDD0048;ClBoomerangRequest { -> F}
//## Uses: <unnamed>%36A35418027E;ClControlQ { -> }
//## Uses: <unnamed>%374ACC67000E;ClLogSystem { -> F}
//## Uses: <unnamed>%3795E6130328;ClConnInstance { -> F}

class M4_DECL_M4USERINTERFACE ClUSServiceAPI 
{
  //## begin ClUSServiceAPI%35507A7001B5.initialDeclarations preserve=yes
  //## end ClUSServiceAPI%35507A7001B5.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClUSServiceAPI%894446845
      ~ClUSServiceAPI ();


    //## Other Operations (specified)
      //## Operation: CreateUserSession%894446846; C++
      //	This function verifies that username and password are correct an allow a
      //	cliente to connect to the System.
      //
      //	Parameters (in DataStorage format):
      //	(IN): Username, password as NULL-terminated strings (M4ClString)
      //	(IN) Languate (m4int16_t)
      //	(OUT): Error code and a  pointer to a Session object (ClUSSessionInterface).
      m4return_t CreateUserSession (const m4pchar_t ai_pszUsername, const m4pchar_t ai_pszPassword, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, const m4bool_t ai_bDefRole, m4pcchar_t ai_pcLanguage, m4pcchar_t ai_pcOrganization, const m4bool_t ai_bUse2Way, m4pcchar_t ai_pccClientMachine, m4pcchar_t ai_pcLogonOrigin, m4pcchar_t ai_pcSystemInfo, m4pcchar_t ai_pcClientType, m4pcchar_t ai_pcClientPubIP, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ);

      //## Operation: CreateUserSession%899809128; C++
      //	This function verifies that username and password are correct an allow a
      //	cliente to connect to the System.
      //
      //	Parameters (in DataStorage format):
      //	(IN): Username, password as NULL-terminated strings (M4ClString)
      //	(OUT): Error code and a  pointer to a Session object (ClUSSessionInterface)
      m4return_t CreateUserSession (const m4pchar_t ai_pszUsername, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, ClControlQ *ai_pSourceQ);

      //## Operation: CheckRole%895825632; C++
      m4return_t CheckRole (ClUSSessionInterface *ai_pSession, const m4pchar_t ai_pszRoleId, m4handle_t &ao_hRequest, m4bool_t ai_bDefault = M4_FALSE, ClLogSystem *ai_poLog = NULL, ClControlQ *ai_pSourceQ = NULL);

      //## Operation: ChangePassword%894446848; C++
      //	This function verifies that the new password for the user is correct in case
      //	it can be changed.
      //
      //	Parameters (in DataStorage format):
      //	(IN): Old and new  passwords as NULL-terminated strings (M4ClString)
      //	(OUT): Error code.
      m4return_t ChangePassword (ClUSSessionInterface *ai_pSession, const m4pchar_t &ai_pszOldPassword, const m4pchar_t &ai_pszNewPassword, m4handle_t &ao_hRequest, ClLogSystem *ai_poLog = NULL, ClControlQ *ai_pSourceQ = NULL);

      //## Operation: Instance%894446849
      static ClUSServiceAPI * Instance ();

      //## Operation: Initialize%894446850; C++
      m4return_t Initialize (ClServiceQ *ai_pUUSSQ);

    // Additional Public Declarations
      //## begin ClUSServiceAPI%35507A7001B5.public preserve=yes
	  m4return_t CreateUserSession (m4string_t ai_iCertificateType , const m4pchar_t ai_pszCertificate, m4uint32_t ai_iCertificateLen , m4pchar_t ai_pszSessContext, m4pcchar_t ai_pccClientMachine, m4pcchar_t ai_pccSystemInfo, m4pcchar_t ai_pccClientType, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ) ;

	  m4return_t GenerateCertificate (ClUSSessionInterface *ai_pSession , m4pchar_t ai_szUserName , m4handle_t &ao_hRequest , ClLogSystem *ai_poLog = NULL , ClControlQ *ai_pSourceQ = NULL ) ;

	  // Auditoría de sesión
      m4return_t CloseAuditory (ClUSSessionInterface *ai_pSession, ClLogSystem *ai_poLog = NULL, ClControlQ *ai_pSourceQ = NULL);
      m4return_t CleanAuditory (ClLogSystem *ai_poLog = NULL, ClControlQ *ai_pSourceQ = NULL);

      //## end ClUSServiceAPI%35507A7001B5.public
  protected:
    //## Constructors (specified)
      //## Operation: ClUSServiceAPI%894446844; C++
      ClUSServiceAPI ();

    // Data Members for Associations

      //## Association: M4UserInterface::<unnamed>%356ACE260123
      //## Role: ClUSServiceAPI::m_pServerSession%356ACE270152
      //## begin ClUSServiceAPI::m_pServerSession%356ACE270152.role preserve=no  protected: ClUSSessionInterface {1 -> 1RFHAN}
      ClUSSessionInterface *m_pServerSession;
      //## end ClUSServiceAPI::m_pServerSession%356ACE270152.role

      //## Association: M4UserInterface::<unnamed>%356463480342
      //## Role: ClUSServiceAPI::m_pUSQ%3564634803CF
      //## begin ClUSServiceAPI::m_pUSQ%3564634803CF.role preserve=no  protected: ClUSQueue {1 -> 1RHAN}
      ClUSQueue *m_pUSQ;
      //## end ClUSServiceAPI::m_pUSQ%3564634803CF.role

      //## Association: M4UserInterface::<unnamed>%3550845902AF
      //## Role: ClUSServiceAPI::m_pInstance%3550845D0148
      //## begin ClUSServiceAPI::m_pInstance%3550845D0148.role preserve=no  protected: static ClUSServiceAPI {1 -> 1RHAN}
      static ClUSServiceAPI *m_pInstance;
      //## end ClUSServiceAPI::m_pInstance%3550845D0148.role

    // Additional Protected Declarations
      //## begin ClUSServiceAPI%35507A7001B5.protected preserve=yes
      //## end ClUSServiceAPI%35507A7001B5.protected

  private:
    // Additional Private Declarations
      //## begin ClUSServiceAPI%35507A7001B5.private preserve=yes
      //## end ClUSServiceAPI%35507A7001B5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUSServiceAPI%35507A7001B5.implementation preserve=yes
      //## end ClUSServiceAPI%35507A7001B5.implementation

  public:
	  // User actions auditory.
      m4return_t UserAuditoryLogoffAction( ClUSSessionInterface *ai_pSession, ClLogSystem *ai_poLog = NULL, ClControlQ *ai_pSourceQ = NULL ) ;
};

//## begin ClUSServiceAPI%35507A7001B5.postscript preserve=yes
//## end ClUSServiceAPI%35507A7001B5.postscript

// Class ClUSServiceAPI 

//## begin module%355071B30232.epilog preserve=yes
//## end module%355071B30232.epilog


#endif
