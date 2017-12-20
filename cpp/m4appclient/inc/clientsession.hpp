//## begin module%347958E40291.cm preserve=no
//## end module%347958E40291.cm

//## begin module%347958E40291.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClSesionC.hpp
//	 Project:           M4ComSrv
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
//	=========================================================
//	=====================
//## end module%347958E40291.cp

//## Module: clientsession%347958E40291; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: c:\m4server\m4appclient\inc\clientsession.hpp

#ifndef clientsession_h
#define clientsession_h 1

//## begin module%347958E40291.additionalIncludes preserve=no
//## end module%347958E40291.additionalIncludes

//## begin module%347958E40291.includes preserve=yes
//## end module%347958E40291.includes

// stl
#include "m4stl.hpp"
// clientbasicdef
#include <clientbasicdef.hpp>
// clientconnection
#include <clientconnection.hpp>
// clientservice
#include <clientservice.hpp>
// clientrequest
#include <clientrequest.hpp>
// Clienttag
#include <clienttag.hpp>
// syncro
#include <syncro.hpp>
// m4string
#include <m4string.hpp>
// m4log
#include <m4log.hpp>
// controlpdus
#include <controlpdus.hpp>
// servicepdus
#include <servicepdus.hpp>

class ClCCUser;

//## begin module%347958E40291.declarations preserve=no
//## end module%347958E40291.declarations

//## begin module%347958E40291.additionalDeclarations preserve=yes
class ClCCMonitor;
//## end module%347958E40291.additionalDeclarations

class ClSessionPinger;

//## begin ClCCSession%34472F2B0183.preface preserve=yes
//## end ClCCSession%34472F2B0183.preface

//## Class: ClCCSession%34472F2B0183
//	.
//	Esta es la clase que mantiene una sesion en el cliente. Está encargada de
//	aglutinar y dar soporte a todo lo que es la parte cliente.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E9B4700036;ClAuthenticationPDU { -> }
//## Uses: <unnamed>%350E578B01B2;ClRequestPDU { -> }
//## Uses: <unnamed>%379766D0028E;ClLog { -> }
//## Uses: <unnamed>%380F21A1037E;stdXXostream { -> }
//## Uses: <unnamed>%380F335A022E;basic_ofstream { -> }
//## Uses: <unnamed>%38AD408E00D0;ClMutex { -> }
//## Uses: <unnamed>%38B127B00016;ClCCRequest { -> }

class ClCCSession 
{
  //## begin ClCCSession%34472F2B0183.initialDeclarations preserve=yes
  //## end ClCCSession%34472F2B0183.initialDeclarations

  public:
    //## Class: eSessionStatus%3517883C0270
    //## Category: M4AppClient%34E17F280252
    //## Subsystem: M4AppClient::inc%3796F8B303AC
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {S_OPEN, S_ACTIVE,S_CLOSED} eSessionStatus;

    //## Constructors (specified)
      //## Operation: ClCCSession%880446332; C++
      ClCCSession (ClCCConnection *ai_pConnection, ClCCUser *ai_pUser, m4uint32_t ai_uiControlTimeout = TIMEOUT_ADMIN_SERV, ClTimeoutFunction ai_pTimeoutFunction = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCCSession%881084616
      ~ClCCSession ();


    //## Other Operations (specified)
      //## Operation: Open%879852416; C++
      //	.
      //	Crea un nuevo servicio y devuelve el identificador en el parámetro ao_h
      //	ServiceId.
      //	.
      m4return_t Open (m4handle_t &ao_hSessionId, ClTagList *ao_oTagList = NULL, m4bool_t ai_bDefaultRole = M4_FALSE);

      //## Operation: AddService%887374527; C++
      //	.
      //	Crea un nuevo servicio y devuelve el identificador en el parámetro ao_h
      //	ServiceId.
      //	.
      m4return_t AddService (m4pchar_t ai_SZServiceType, m4handle_t &ao_hServiceId);

      //## Operation: EnableRole%887374523; C++
      m4return_t EnableRole (m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef);

      //## Operation: DisableRole%887374524; C++
      m4return_t DisableRole (m4pchar_t ai_pRoleId);

      //## Operation: RemoveService%887374528; C++
      //	.
      //	Cierra el servicio correspondiente al identificador que recibe como
      //	parámetro.
      //	.
      m4return_t RemoveService (m4handle_t ai_hService);

      //## Operation: Close%879852417; C++
      //	.
      //	Cierra el servicio correspondiente al identificador que recibe como
      //	parámetro.
      //	.
      m4return_t Close ();

      //## Operation: RemoveAllServices%887374530
      m4return_t RemoveAllServices ();

      //## Operation: GetConnection%887374531; C++
      m4return_t GetConnection (ClCCConnection *&ao_pConnection);

      //## Operation: GetUser%890127478; C++
      m4return_t GetUser (ClCCUser *&ao_pUser);

      //## Operation: GetStatus%890732553; C++
      void GetStatus (eSessionStatus &ao_eStatus);

      //## Operation: GetProps%898770811; C++
      //	Accede al mapa interno de la sesion para optener las
      //	propiedades que se le piden devolviendo M4_SUCCESS.
      //
      //	En el caso en que no encu
      m4return_t GetProps (ClTagList  *ao_cTagMap);

      //## Operation: GetService%887374529; C++
      m4return_t GetService (m4handle_t ai_hServiceId, ClCCService *&ao_pService);

      //## Operation: SetProps%898770812; C++
      //	Establece los valores de la tagList que se le pasa en la session
      m4return_t SetProps (ClTagList *ai_cTagMap);

      //## Operation: ChangePassword%898848502
      m4return_t ChangePassword (M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd);

      //## Operation: SendPing
	  m4return_t SendPing(m4uint32_t ai_iuPingTimeOut);

      //## Operation: NotifyAllServices%923902841; C++
      m4return_t NotifyAllServices (eRequestStatus ai_eIncidence);

      //## Operation: _GetAnswerData%950783980; C++
      m4return_t _GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend);

      //## Operation: _SendData%950783981; C++
      m4return_t _SendData (ClCCRequest *ai_pRequest);

      //## Operation: GetServerSessionParams%986815274
      m4return_t GetServerSessionParams (m4string_t &ao_strServerHost, m4string_t &ao_strServerBasePort, m4string_t &ao_strServerSessionId);

      //## Operation: SetCredential%986815277
      m4return_t SetCredential (m4string_t ai_strCredential);

      //## Operation: SetClientID%1056466952
      static void SetClientID (m4string_t ai_strClientID);

      //## Operation: GetSessContext%1056466953
      m4return_t GetSessContext (m4pchar_t &ao_strX509Cert, m4uint32_t &aoi_iX509CertBufLen, m4pchar_t &ao_strSessionContext, m4uint32_t &aoi_iSessContextBufLen);

      //## Operation: Open%1056466954
      m4return_t Open (m4uchar_t *ai_szCertificate, m4uint32_t ai_iCertificateLen, m4uchar_t *ai_szSessContext, m4handle_t &ao_hSessionId, ClTagList *ao_oTagList = NULL, m4bool_t ai_bDefaultRole = M4_FALSE);

      //## Operation: ConnectRequest%1105443232
      m4return_t ConnectRequest (m4handle_t &ao_hSessionId, ClTagList *ao_oTagList);

    // Data Members for Class Attributes

      //## Attribute: m_poDebugFile%35B320E60178
      //## begin ClCCSession::m_poDebugFile%35B320E60178.attr preserve=no  public: fstream * {UA} NULL
      fstream *m_poDebugFile;
      //## end ClCCSession::m_poDebugFile%35B320E60178.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%39E1E498014E
      //## Role: ClCCSession::m_strCredential%39E1E49803D9
      //## begin ClCCSession::m_strCredential%39E1E49803D9.role preserve=no  public: m4string_t { -> UHAN}
      m4string_t m_strCredential;
      //## end ClCCSession::m_strCredential%39E1E49803D9.role

	  void PDU_Lock(m4uint8_t ai_iOpID);

	  void PDU_Unlock(m4uint8_t ai_iOpID);

	  void NotifyVMExecution(m4bool_t ai_bIsStart);

  public:
    // Additional Public Declarations
      //## begin ClCCSession%34472F2B0183.public preserve=yes
      //## end ClCCSession%34472F2B0183.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_eStatus%3517875B01D4
      //## begin ClCCSession::m_eStatus%3517875B01D4.attr preserve=no  protected: eSessionStatus {UA} S_CLOSED
      eSessionStatus m_eStatus;
      //## end ClCCSession::m_eStatus%3517875B01D4.attr

      //## Attribute: m_uiControlTimeout%370B3FF00049
      //## begin ClCCSession::m_uiControlTimeout%370B3FF00049.attr preserve=no  protected: m4uint32_t {UA} ai_uiControlTimeout
      m4uint32_t m_uiControlTimeout;
      //## end ClCCSession::m_uiControlTimeout%370B3FF00049.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%34E44396007D
      //## Role: ClCCSession::m_dicServices%34E44396007F
      //## Qualifier: hService%35094B8000BA; m4handle_t
      //## begin ClCCSession::m_dicServices%34E44396007F.role preserve=no  protected: ClCCService {1 -> 1RHAN}
      map<m4handle_t, ClCCService *, less<m4handle_t> > m_dicServices;
      //## end ClCCSession::m_dicServices%34E44396007F.role

      //## Association: M4AppClient::<unnamed>%34E83B9A034A
      //## Role: ClCCSession::m_pUser%34E83B9B0195
      //## begin ClCCSession::m_pUser%34E83B9B0195.role preserve=no  protected: ClCCUser {1 -> 1RFHAN}
      ClCCUser *m_pUser;
      //## end ClCCSession::m_pUser%34E83B9B0195.role

      //## Association: M4AppClient::<unnamed>%35094CF10176
      //## Role: ClCCSession::m_pConnection%35094CF2028F
      //## begin ClCCSession::m_pConnection%35094CF2028F.role preserve=no  protected: ClCCConnection {1 -> 1RHGAN}
      ClCCConnection *m_pConnection;
      //## end ClCCSession::m_pConnection%35094CF2028F.role

    // Additional Protected Declarations
      //## begin ClCCSession%34472F2B0183.protected preserve=yes
      //## end ClCCSession%34472F2B0183.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_MutexDicServices%3563F39403CA
      //## begin ClCCSession::m_MutexDicServices%3563F39403CA.attr preserve=no  private: ClMutex {UA} 
      ClMutex m_MutexDicServices;
      //## end ClCCSession::m_MutexDicServices%3563F39403CA.attr

      //## Attribute: m_strSessionID%3742E14801AF
      //## begin ClCCSession::m_strSessionID%3742E14801AF.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_strSessionID;
      //## end ClCCSession::m_strSessionID%3742E14801AF.attr

      //## Attribute: m_strServerHost%3AD1A9A00093
      //## begin ClCCSession::m_strServerHost%3AD1A9A00093.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strServerHost;
      //## end ClCCSession::m_strServerHost%3AD1A9A00093.attr

      //## Attribute: m_strServerBasePort%3AD1A9AD0087
      //## begin ClCCSession::m_strServerBasePort%3AD1A9AD0087.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strServerBasePort;
      //## end ClCCSession::m_strServerBasePort%3AD1A9AD0087.attr

      //## Attribute: m_strServerSessionId%3AD1A9BF00F1
      //## begin ClCCSession::m_strServerSessionId%3AD1A9BF00F1.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_strServerSessionId;
      //## end ClCCSession::m_strServerSessionId%3AD1A9BF00F1.attr

      //## Attribute: m_strClientID%3EF86A5C0280
      //## begin ClCCSession::m_strClientID%3EF86A5C0280.attr preserve=no  private: static m4string_t {UA} 
      static m4string_t m_strClientID;
      //## end ClCCSession::m_strClientID%3EF86A5C0280.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3593A90C0299
      //## Role: ClCCSession::m_oTagMap%3593A90D00C4
      //## begin ClCCSession::m_oTagMap%3593A90D00C4.role preserve=no  private: ClTagList {1 -> 1VHAN}
      ClTagList m_oTagMap;
      //## end ClCCSession::m_oTagMap%3593A90D00C4.role

    // Additional Private Declarations
      //## begin ClCCSession%34472F2B0183.private preserve=yes
      //## end ClCCSession%34472F2B0183.private

	  ClMutex m_PDUMutex;

	  ClSessionPinger* m_pSessionPinger;

  private: //## implementation
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%370B45460184
      //## Role: ClCCSession::m_pTimeoutFunction%370B45470013
      //## begin ClCCSession::m_pTimeoutFunction%370B45470013.role preserve=no  implementation: ClTimeoutFunction {1 -> 1UHAN}
      ClTimeoutFunction m_pTimeoutFunction;
      //## end ClCCSession::m_pTimeoutFunction%370B45470013.role

	  m4return_t ClCCSession::_CheckServerVersion( ClTagList *ao_oTagList ) ;
	  m4return_t ClCCSession::_CheckServerEncoding( ClTagList *ao_oTagList, m4handle_t ai_hSessionId, m4bool_t& ao_bChanged ) ;

    // Additional Implementation Declarations
      //## begin ClCCSession%34472F2B0183.implementation preserve=yes
      //## end ClCCSession%34472F2B0183.implementation

};

//## begin ClCCSession%34472F2B0183.postscript preserve=yes
//## end ClCCSession%34472F2B0183.postscript

//## begin ClCCUser%344739B502B9.preface preserve=yes
//## end ClCCUser%344739B502B9.preface

//## Class: ClCCUser%344739B502B9
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCCUser 
{
  //## begin ClCCUser%344739B502B9.initialDeclarations preserve=yes
  //## end ClCCUser%344739B502B9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCUser%881084617; C++
      ClCCUser (M4ClString &ai_strUsername, m4pchar_t ai_strPasswd, m4uint32_t ai_iPasswdLen);

    //## Destructor (specified)
      //## Operation: ~ClCCUser%881084618
      ~ClCCUser ();


    //## Other Operations (specified)
      //## Operation: AddSession%887628421; C++
      m4return_t AddSession (m4handle_t ai_hConnectionId, m4handle_t ai_hSessionId);

      //## Operation: RemoveAllSessions%887628422; C++
      m4return_t RemoveAllSessions ();

      //## Operation: RemoveSession%887628423; C++
      m4return_t RemoveSession (m4handle_t ai_hConnectionId);

      //## Operation: GetName%887731065; C++
      m4return_t GetName (m4pchar_t &ao_pszName);

      //## Operation: GetPasswd%887731066; C++
      m4return_t GetPasswd (m4pchar_t &ao_pszPasswd, m4uint32_t &ao_iPasswdLen);

      //## Operation: GetSession%890042317; C++
      m4return_t GetSession (m4handle_t ai_hConnection);

      //## Operation: Encrypt2Way%1056466950
      m4return_t Encrypt2Way ();

      //## Operation: SetPassword%1101820656
      m4return_t SetPassword (m4pchar_t ai_pcPassword, m4uint32_t ai_iPasswLen);

      //## Operation: SetUserName%1101820657
      m4return_t SetUserName (m4pchar_t ai_pcUserName);

    // Additional Public Declarations
      //## begin ClCCUser%344739B502B9.public preserve=yes
      //## end ClCCUser%344739B502B9.public

  protected:
    // Additional Protected Declarations
      //## begin ClCCUser%344739B502B9.protected preserve=yes
      //## end ClCCUser%344739B502B9.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pszUsername%34E8754A0195
      //## begin ClCCUser::m_pszUsername%34E8754A0195.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pszUsername;
      //## end ClCCUser::m_pszUsername%34E8754A0195.attr

      //## Attribute: m_pszPasswd%34E8756C03C7
      //## begin ClCCUser::m_pszPasswd%34E8756C03C7.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pszPasswd;
      //## end ClCCUser::m_pszPasswd%34E8756C03C7.attr

    // Additional Private Declarations
      //## begin ClCCUser%344739B502B9.private preserve=yes
	  m4uint32_t m_iPasswdLen;
      //## end ClCCUser%344739B502B9.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCUser%344739B502B9.implementation preserve=yes
      //## end ClCCUser%344739B502B9.implementation

};

//## begin ClCCUser%344739B502B9.postscript preserve=yes
//## end ClCCUser%344739B502B9.postscript

// Class ClCCSession 

// Class ClCCUser 

//## begin module%347958E40291.epilog preserve=yes
//## end module%347958E40291.epilog

#endif
