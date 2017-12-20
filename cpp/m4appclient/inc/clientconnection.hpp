//## begin module%38ABD2B600C1.cm preserve=no
//## end module%38ABD2B600C1.cm

//## begin module%38ABD2B600C1.cp preserve=no
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
//## end module%38ABD2B600C1.cp

//## Module: clientconnection%38ABD2B600C1; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: C:\m4server\m4appclient\inc\clientconnection.hpp

#ifndef clientconnection_h
#define clientconnection_h 1

//## begin module%38ABD2B600C1.additionalIncludes preserve=no
//## end module%38ABD2B600C1.additionalIncludes

//## begin module%38ABD2B600C1.includes preserve=yes
#include "m4appclient_dll.hpp"
//## end module%38ABD2B600C1.includes

// clientbasicdef
#include <clientbasicdef.hpp>
// pduherram
#include <pduherram.hpp>
// clientsenderq
#include <clientsenderq.hpp>
// clientrequest
#include <clientrequest.hpp>
// clientsession
#include <clientsession.hpp>
// clientmonitor
#include <clientmonitor.hpp>
// FastQ
#include <fastfifoq.hpp>
// ConnBase
#include <connbase.hpp>
// iexecutable
#include <iexecutable.hpp>
// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
// m4string
#include <m4string.hpp>
// m4communication
#include <m4communication.hpp>

class ClClientExecutableInterface;

//## begin module%38ABD2B600C1.declarations preserve=no
//## end module%38ABD2B600C1.declarations

//## begin module%38ABD2B600C1.additionalDeclarations preserve=yes
//## end module%38ABD2B600C1.additionalDeclarations


//## begin eConnectionStatus%36EFA88402FA.preface preserve=yes
//## end eConnectionStatus%36EFA88402FA.preface

//## Class: eConnectionStatus%36EFA88402FA
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum{ M4_CONNECTION_NOTSAP,M4_CONNECTION_OPENING_TSAP,M4_CONNECTION_STARTING,M4_CONNECTION_RECONNECTING,M4_CONNECTION_TSAP_OK,M4_CONNECTION_RUNNING,M4_CONNECTION_FINISHING,M4_CONNECTION_CLOSING_TSAP,M4_CONNECTION_CLOSING,M4_CONNECTION_CANCELING,M4_CONNECTION_START_CLOSING} eConnectionStatus;

//## begin eConnectionStatus%36EFA88402FA.postscript preserve=yes
//## end eConnectionStatus%36EFA88402FA.postscript

//## begin ClCPDUQueue%392E2B3603D3.preface preserve=yes
//## end ClCPDUQueue%392E2B3603D3.preface

//## Class: ClCPDUQueue%392E2B3603D3; Instantiated Class
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef FastQueue< ClPDU * > ClCPDUQueue;

//## begin ClCPDUQueue%392E2B3603D3.postscript preserve=yes
//## end ClCPDUQueue%392E2B3603D3.postscript

//## Class: ClCCConnection%344C62F20059
//	.
//	Esta clase toma toda la funcionalidad de la conexión y añade alguna muy
//	específica del cliente.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36F13990021E;eConnectionStatus { -> }
//## Uses: <unnamed>%38ABDA4900F1;ClGetPDUTSAP { -> }
//## Uses: <unnamed>%38AD45A50200;ClCCMonitor { -> }
//## Uses: <unnamed>%38AD462F0230;ClCCRequest { -> }
//## Uses: <unnamed>%38B10C2E020F;ClCCSendQ { -> }

class M4_DECL_M4APPCLIENT ClCCConnection : public ClConnBase  //## Inherits: <unnamed>%349EAE830152
{
  public:
    //## Constructors (specified)
      //## Operation: ClCCConnection%921763083; C++
      //	Permite crear una tsap con las características que se le pasan
      ClCCConnection (M4ClString &ai_strHost, m4int_t ai_iHostPort, m4bool_t ai_bSSL, M4ClString &ai_strCACertPath, m4int_t ai_iCommsType = M4_NULL_COMMS_TYPE);

    //## Destructor (specified)
      //## Operation: ~ClCCConnection%881084619; C++
      virtual ~ClCCConnection ();


    //## Other Operations (specified)
      //## Operation: GetServerTime%879852408
      //	.
      //	Nos devuelve el momento actual como si lo estuviesemos midiendo en el
      //	servidor.
      //	.
      m4int64_t GetServerTime ();

      //## Operation: GetTSAP%880446330; C++
      m4return_t GetTSAP (ClTSAP *&ao_pTSAP);

      //## Operation: RemoveAllSessions%887374532; C++
      m4return_t RemoveAllSessions ();

      //## Operation: AddSession%887452284; C++
      m4return_t AddSession (m4handle_t ai_hSessionId, ClCCSession *ai_pSession);

      //## Operation: RemoveSession%887452285; C++
      m4return_t RemoveSession (m4handle_t ai_hSession);

      //## Operation: GetSession%890042316; C++
      m4return_t GetSession (m4handle_t ai_hSession, ClCCSession *ao_pSession);

      //## Operation: SetMainSession%901562156; C++
      m4return_t SetMainSession (ClCCSession *ai_pSession);

      //## Operation: GetConnType%921676538; C++
      virtual m4char_t GetConnType ();

      //## Operation: SetMonitor%921763084; C++
      m4return_t SetMonitor (ClCCMonitor *ai_pMonitor = NULL);

      //## Operation: CreateTSAP%921763085; C++
      m4return_t CreateTSAP (m4pchar_t ai_pcAlterHostID = NULL, m4int_t ai_iAlterHostPort = 0);

      //## Operation: GetError%921763086
      m4return_t GetError ();

      //## Operation: DestroyTSAP%921763087
      m4return_t DestroyTSAP ();

      //## Operation: GetConnStatus%922184354
      //	Devuelve un 0 si es una ClConnection normal y otro valor si es de otro tipo
      eConnectionStatus GetConnStatus ();

      //## Operation: GetTSAPStatus%922441070
      eConnectionStatus GetTSAPStatus ();

      //## Operation: SetConnStatus%923902838; C++
      m4return_t SetConnStatus (eConnectionStatus ai_eConnStatus);

      //## Operation: SetTSAPStatus%923902839; C++
      m4return_t SetTSAPStatus (eConnectionStatus ai_eTSAPStatus);

      //## Operation: NotifyIncidence%923902842; C++
      m4return_t NotifyIncidence (eRequestStatus ai_eIncidence);

      //## Operation: Reconnect%927110475; C++
      virtual m4return_t Reconnect (m4pchar_t ai_pcServerIP, m4int_t ai_iServerPort);

      //## Operation: Recycle%942925902; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      m4return_t Recycle ();

      //## Operation: CanRecycle%942925903
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      m4bool_t CanRecycle ();

      //## Operation: Destroy%942925904
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      m4return_t Destroy ();

      //## Operation: CanDestroy%942925905
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      m4bool_t CanDestroy ();

      //## Operation: SetRecursiveDelete%947781333; C++
      m4return_t SetRecursiveDelete (m4bool_t ai_bRecursive);

      //## Operation: _GetAnswerData%950783982; C++
      virtual m4return_t _GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend);

      //## Operation: _SendData%950783983; C++
      m4return_t _SendData (ClCCRequest *ai_pRequest);

      //## Operation: _SendRawData%950783987; C++
      virtual m4return_t _SendRawData (ClCCRequest *ai_pRequest);

      //## Operation: _CompressData%951147243; C++
      m4return_t _CompressData (ClPDU *ai_pPDU, m4uint32_t ai_uiCompressType, m4uint32_t ai_uiCompressLevel);

      //## Operation: _DecompressData%951147244; C++
      m4return_t _DecompressData (ClPDU *ai_pPDU, m4uint32_t ai_uiCompressType);

      //## Operation: _GetAnswerPDU%951317374; C++
      virtual m4return_t _GetAnswerPDU (ClPDU *&ao_pPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout = -1, m4bool_t ai_bRetrying = false, m4pchar_t ai_pcBuffer = NULL, m4uint_t ai_uiBufferSize = 0);

      //## Operation: GetEvent%959326859; C++
      m4return_t GetEvent (ClPDU *ai_poPDU);

      //## Operation: SetServerEventFunction%959326860; C++
      m4return_t SetServerEventFunction (ClServerEventFunction ai_EventFunction);

      //## Operation: DetectedBrokenConn%982054917
      m4return_t DetectedBrokenConn ();

      //## Operation: GetCurrentHost%1101820652
      m4return_t GetCurrentHost (M4ClString &ao_strCurrentHost);

      //## Operation: SetCurrentHost%1101820653
      m4return_t SetCurrentHost (const M4ClString &ai_strCurrentHost);

      //## Operation: GetHost%1101820654
      m4return_t GetHost (M4ClString &ao_strHost);

      //## Operation: SetHost%1101820655
      m4return_t SetHost (const M4ClString &ai_strHost);

      //## Operation: GetAuthServer%1173278265
      m4return_t GetAuthServer (M4ClString &ao_strAuthSrv);

    // Data Members for Class Attributes

      //## Attribute: m_iHostPort%36F0FB36001D
      //## begin ClCCConnection::m_iHostPort%36F0FB36001D.attr preserve=no  public: m4int_t {UA} 
      m4int_t m_iHostPort;
      //## end ClCCConnection::m_iHostPort%36F0FB36001D.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%381802ED012B
      //## Role: ClCCConnection::m_strHost%381802EE00A0
      //## begin ClCCConnection::m_strHost%381802EE00A0.role preserve=no  public: M4ClString { -> UHAN}
      M4ClString m_strHost;
      //## end ClCCConnection::m_strHost%381802EE00A0.role

      //## Association: M4AppClient::<unnamed>%392E60EA02EB
      //## Role: ClCCConnection::m_pEventFunction%392E60EB01FC
      //## begin ClCCConnection::m_pEventFunction%392E60EB01FC.role preserve=no  public: ClServerEventFunction { -> VHAN}
      ClServerEventFunction m_pEventFunction;
      //## end ClCCConnection::m_pEventFunction%392E60EB01FC.role

  protected:

    //## Other Operations (specified)
      //## Operation: ReleaseTSAP%973687120
      m4return_t ReleaseTSAP ();

    // Data Members for Class Attributes

      //## Attribute: m_cType%36EFAA4F02BE
      //## begin ClCCConnection::m_cType%36EFAA4F02BE.attr preserve=no  protected: m4char_t {UA} M4_FALSE
      m4char_t m_cType;
      //## end ClCCConnection::m_cType%36EFAA4F02BE.attr

      //## Attribute: m_pClientMonitor%36F0E18801C9
      //## begin ClCCConnection::m_pClientMonitor%36F0E18801C9.attr preserve=no  protected: ClCCMonitor * {UA} 
      ClCCMonitor *m_pClientMonitor;
      //## end ClCCConnection::m_pClientMonitor%36F0E18801C9.attr

      //## Attribute: m_bSSL%36F0FB3C02ED
      //## begin ClCCConnection::m_bSSL%36F0FB3C02ED.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bSSL;
      //## end ClCCConnection::m_bSSL%36F0FB3C02ED.attr

      //## Attribute: m_eConnStatus%36F136F20199
      //## begin ClCCConnection::m_eConnStatus%36F136F20199.attr preserve=no  protected: eConnectionStatus {UA} M4_CONNECTION_STARTING
      eConnectionStatus m_eConnStatus;
      //## end ClCCConnection::m_eConnStatus%36F136F20199.attr

      //## Attribute: m_eTSAPStatus%36FB9B9C021E
      //## begin ClCCConnection::m_eTSAPStatus%36FB9B9C021E.attr preserve=no  protected: eConnectionStatus {UA} M4_CONNECTION_NOTSAP
      eConnectionStatus m_eTSAPStatus;
      //## end ClCCConnection::m_eTSAPStatus%36FB9B9C021E.attr

      //## Attribute: m_iCurrentHostPort%3742F6A1029E
      //## begin ClCCConnection::m_iCurrentHostPort%3742F6A1029E.attr preserve=no  protected: m4int_t {UA} 
      m4int_t m_iCurrentHostPort;
      //## end ClCCConnection::m_iCurrentHostPort%3742F6A1029E.attr

      //## Attribute: m_strURL%3A0969C60214
      //## begin ClCCConnection::m_strURL%3A0969C60214.attr preserve=no  protected: m4string_t {UA} 
      m4string_t m_strURL;
      //## end ClCCConnection::m_strURL%3A0969C60214.attr

      //## Attribute: m_strAuthServer%45EED268005D
      //## begin ClCCConnection::m_strAuthServer%45EED268005D.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_strAuthServer;
      //## end ClCCConnection::m_strAuthServer%45EED268005D.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%35094CF10176
      //## Role: ClCCConnection::m_pMainSession%35094CF201F3
      //## begin ClCCConnection::m_pMainSession%35094CF201F3.role preserve=no  protected: ClCCSession {1 -> 1RHAN}
      ClCCSession *m_pMainSession;
      //## end ClCCConnection::m_pMainSession%35094CF201F3.role

      //## Association: M4AppClient::<unnamed>%3818032F01BC
      //## Role: ClCCConnection::m_strCACertPath%3818032F03B0
      //## begin ClCCConnection::m_strCACertPath%3818032F03B0.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strCACertPath;
      //## end ClCCConnection::m_strCACertPath%3818032F03B0.role

      //## Association: M4AppClient::<unnamed>%3818037001B5
      //## Role: ClCCConnection::m_strCurrentHost%381803710152
      //## begin ClCCConnection::m_strCurrentHost%381803710152.role preserve=no  protected: M4ClString { -> UHAN}
      M4ClString m_strCurrentHost;
      //## end ClCCConnection::m_strCurrentHost%381803710152.role

      //## Association: M4AppClient::<unnamed>%392E294B026E
      //## Role: ClCCConnection::m_hCommHandle%392E294C00CB
      //## begin ClCCConnection::m_hCommHandle%392E294C00CB.role preserve=no  protected: m4handle_t { -> VHAN}
      m4handle_t m_hCommHandle;
      //## end ClCCConnection::m_hCommHandle%392E294C00CB.role

      //## Association: M4AppClient::<unnamed>%392E299B0179
      //## Role: ClCCConnection::m_pComms%392E299C00E4
      //## begin ClCCConnection::m_pComms%392E299C00E4.role preserve=no  protected: ClCommunication { -> RHAN}
      ClCommunication *m_pComms;
      //## end ClCCConnection::m_pComms%392E299C00E4.role

      //## Association: M4AppClient::<unnamed>%392E2A50033C
      //## Role: ClCCConnection::m_pExecutable%392E2A51035B
      //## begin ClCCConnection::m_pExecutable%392E2A51035B.role preserve=no  protected: ClClientExecutableInterface { -> RFHAN}
      ClClientExecutableInterface *m_pExecutable;
      //## end ClCCConnection::m_pExecutable%392E2A51035B.role

      //## Association: M4AppClient::<unnamed>%392E2B720054
      //## Role: ClCCConnection::m_PDUQueue%392E2B7300C3
      //## begin ClCCConnection::m_PDUQueue%392E2B7300C3.role preserve=no  protected: ClCPDUQueue { -> VHAN}
      ClCPDUQueue m_PDUQueue;
      //## end ClCCConnection::m_PDUQueue%392E2B7300C3.role

      //## Association: M4AppClient::<unnamed>%396ED1D901FF
      //## Role: ClCCConnection::m_iCommsType%396ED1DA00DE
      //## begin ClCCConnection::m_iCommsType%396ED1DA00DE.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iCommsType;
      //## end ClCCConnection::m_iCommsType%396ED1DA00DE.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iRef%344C6353008B
      //	.
      //	Este contador lleva la cuenta del número de usos que se están haciendo de
      //	este objeto.
      //
      //	Cada vez que el Monitor del cliente hace un open conexión con el mismo
      //	puesto, usuario y password y dirección se devuelve la referencia de esta
      //	conexión y se incrementa este contador.
      //	.
      //## begin ClCCConnection::m_iRef%344C6353008B.attr preserve=no  private: m4uint16_t {UA} 0
      m4uint16_t m_iRef;
      //## end ClCCConnection::m_iRef%344C6353008B.attr

      //## Attribute: m_cError%36F0FFC00000
      //## begin ClCCConnection::m_cError%36F0FFC00000.attr preserve=no  private: m4return_t {UA} M4_SUCCESS
      m4return_t m_cError;
      //## end ClCCConnection::m_cError%36F0FFC00000.attr

      //## Attribute: m_bRecursive%387DFFE202E7
      //## begin ClCCConnection::m_bRecursive%387DFFE202E7.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bRecursive;
      //## end ClCCConnection::m_bRecursive%387DFFE202E7.attr

      //## Attribute: m_pTSAPSelect%38ABD9A30229
      //## begin ClCCConnection::m_pTSAPSelect%38ABD9A30229.attr preserve=no  private: ClTSAPSelect * {UA} NULL
      ClTSAPSelect *m_pTSAPSelect;
      //## end ClCCConnection::m_pTSAPSelect%38ABD9A30229.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%381803C4018E
      //## Role: ClCCConnection::m_pMutex%381803C40379
      //## begin ClCCConnection::m_pMutex%381803C40379.role preserve=no  private: ClMutex { -> RHAN}
      ClMutex *m_pMutex;
      //## end ClCCConnection::m_pMutex%381803C40379.role

  private: //## implementation
    // Data Members for Class Attributes

      //## Attribute: m_lTime%344C72A403AA
      //	.
      //	Este miembro me permitirá calcular el momento en el que sale una petición
      //	sincronizado con el servidor de aplicaciones con el que estemos conectados.
      //	.
      //## begin ClCCConnection::m_lTime%344C72A403AA.attr preserve=no  implementation: m4int64_t {UA} 
      m4int64_t m_lTime;
      //## end ClCCConnection::m_lTime%344C72A403AA.attr

};

//## begin ClClientExecutableInterface%392E2A1603BB.preface preserve=yes
//## end ClClientExecutableInterface%392E2A1603BB.preface

//## Class: ClClientExecutableInterface%392E2A1603BB
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClClientExecutableInterface : public ClExecutableInterface  //## Inherits: <unnamed>%392E2A2E031F
{
  //## begin ClClientExecutableInterface%392E2A1603BB.initialDeclarations preserve=yes
  //## end ClClientExecutableInterface%392E2A1603BB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClClientExecutableInterface%959326861; C++
      ClClientExecutableInterface (ClCPDUQueue *ai_PDUQueue);


    //## Other Operations (specified)
      //## Operation: ExecuteAction%959326858
      m4return_t ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList);

      //## Operation: ProcessPDU%963819983; C++
      m4return_t ProcessPDU (ClPDU *ai_poPDU);

      //## Operation: SetConnection%963819984; C++
      m4return_t SetConnection (ClCCConnection *ai_pConnection);

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%392E65580151
      //## Role: ClClientExecutableInterface::m_pPDUQueue%392E65590076
      //## begin ClClientExecutableInterface::m_pPDUQueue%392E65590076.role preserve=no  public: ClCPDUQueue { -> RHAN}
      ClCPDUQueue *m_pPDUQueue;
      //## end ClClientExecutableInterface::m_pPDUQueue%392E65590076.role

    // Additional Public Declarations
      //## begin ClClientExecutableInterface%392E2A1603BB.public preserve=yes
      //## end ClClientExecutableInterface%392E2A1603BB.public

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%392E2A50033C
      //## Role: ClClientExecutableInterface::m_pConnection%392E2A510347
      //## begin ClClientExecutableInterface::m_pConnection%392E2A510347.role preserve=no  protected: ClCCConnection { -> RHGAN}
      ClCCConnection *m_pConnection;
      //## end ClClientExecutableInterface::m_pConnection%392E2A510347.role

    // Additional Protected Declarations
      //## begin ClClientExecutableInterface%392E2A1603BB.protected preserve=yes
      //## end ClClientExecutableInterface%392E2A1603BB.protected

  private:
    // Additional Private Declarations
      //## begin ClClientExecutableInterface%392E2A1603BB.private preserve=yes
      //## end ClClientExecutableInterface%392E2A1603BB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClClientExecutableInterface%392E2A1603BB.implementation preserve=yes
      //## end ClClientExecutableInterface%392E2A1603BB.implementation

};

//## begin ClClientExecutableInterface%392E2A1603BB.postscript preserve=yes
//## end ClClientExecutableInterface%392E2A1603BB.postscript

// Class ClCCConnection 

// Class ClClientExecutableInterface 

//## begin module%38ABD2B600C1.epilog preserve=yes
//## end module%38ABD2B600C1.epilog


#endif
