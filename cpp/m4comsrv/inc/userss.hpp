//## begin module%34C86C19026B.cm preserve=no
//## end module%34C86C19026B.cm

//## begin module%34C86C19026B.cp preserve=no
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
//## end module%34C86C19026B.cp

//## Module: userss%34C86C19026B; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\userss.hpp

#ifndef userss_h
#define userss_h 1

//## begin module%34C86C19026B.additionalIncludes preserve=no
//## end module%34C86C19026B.additionalIncludes

//## begin module%34C86C19026B.includes preserve=yes
#include <m4srvres.hpp>
#include <clpconfiguration.hpp>
#include <subsystemids.hpp>
#include <ssnames.hpp>
#include <clcache.hpp>
#include <clssimplementacion.hpp>
//## end module%34C86C19026B.includes

// syncro
#include <syncro.hpp>
// Statistics
#include <statistics.hpp>
// sessiondic
#include <sessiondic.hpp>
// clssaction
#include <clssaction.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>

class ClConnection;
class ClLocalMonitor;
class ClUsersSSStats;
class ClUsersSSVisitor;
class ClLogonInterface;
class ClUSSessionVisitor;
class ClUSUserInterface;
class ClUSUserVisitor;
class ClUSSessionInterface;
class ClCacheTest;

//## begin module%34C86C19026B.declarations preserve=no
//## end module%34C86C19026B.declarations

//## begin module%34C86C19026B.additionalDeclarations preserve=yes
//A modelizar
class M4Credentials ;


#define ID_UUSS_LOGON		"LOGON_INTERFACE"
#define DEF_STRING_SIZE		20
//## end module%34C86C19026B.additionalDeclarations


//## begin ClUsersSS%34C7653103CD.preface preserve=yes
//## end ClUsersSS%34C7653103CD.preface

//## Class: ClUsersSS%34C7653103CD
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34C8784F01D0;ClUsersSSVisitor { -> F}
//## Uses: <unnamed>%350D6A6B01AD;ClUsersSSStats { -> F}
//## Uses: <unnamed>%3540C2DD0151;ClConnection { -> F}
//## Uses: <unnamed>%35697C490191;ClLocalMonitor { -> F}
//## Uses: <unnamed>%35A3265A00B2;ClLogonInstance { -> F}
//## Uses: <unnamed>%36A46B1F0223;ClRequest { -> F}
//## Uses: <unnamed>%374E6ECB000F;ClMutex { -> }
//## Uses: <unnamed>%37A01C6F0370;ClLogonInterface { -> F}
//## Uses: <unnamed>%3961C56400A1;ClRequestCollector { -> F}

class ClUsersSS : public ClSSImplementation  //## Inherits: <unnamed>%38E9AD5301DC
{
  //## begin ClUsersSS%34C7653103CD.initialDeclarations preserve=yes
  //## end ClUsersSS%34C7653103CD.initialDeclarations

  public:
    //## begin ClUsersSS::eUsersCommands%350D8CCF0076.preface preserve=yes
    //## end ClUsersSS::eUsersCommands%350D8CCF0076.preface

    //## Class: eUsersCommands%350D8CCF0076
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { ADD_NEW_USER  = ClSSImplementation::TOTAL_COMMANDS , ALLOW_NEW_USER , CHECK_TIMEOUT_SESSIONS , SEND_MESSAGE_TO_ALL_USERS_SESSIONS , ESTIMATE_LOAD, TURN_ON_USER_TIMEOUT ,TURN_OFF_USER_TIMEOUT , CLOSE_ALL_SESSIONS , TOTAL_COMMANDS } eUsersCommands;

    //## begin ClUsersSS::eUsersCommands%350D8CCF0076.postscript preserve=yes
    //## end ClUsersSS::eUsersCommands%350D8CCF0076.postscript

    //## begin ClUsersSS::eUsersActions%39193CEF0085.preface preserve=yes
    //## end ClUsersSS::eUsersActions%39193CEF0085.preface

    //## Class: eUsersActions%39193CEF0085
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { NEW_USER_ACTION_ID = 0 , END_USER_SESSION_ACTION_ID ,   RECOVER_SESSION_ACTION_ID } eUsersActions;

    //## begin ClUsersSS::eUsersActions%39193CEF0085.postscript preserve=yes
    //## end ClUsersSS::eUsersActions%39193CEF0085.postscript

    //## Constructors (specified)
      //## Operation: ClUsersSS%890993640
      ClUsersSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClUsersSS%890993641
      ~ClUsersSS ();


    //## Other Operations (specified)
      //## Operation: Accept%885550068
      void Accept (ClUsersSSVisitor &ai_poVisitor);

      //## Operation: ExecuteCommand%910862257
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Initialize%885891302
      m4return_t Initialize ();

      //## Operation: ShutDown%885891304
      m4return_t ShutDown ();

      //## Operation: Synchronize%886518768
      virtual m4return_t Synchronize ();

      //## Operation: GetUserAccount%893085117
      ClUSUserInterface * GetUserAccount (m4objid_t ai_lUserId);

      //## Operation: EndUserSession%893085120
      m4return_t EndUserSession (m4objid_t ai_lIdSession, m4objid_t ai_lIdUser);

      //## Operation: GetElement%899883297
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: GetCommandParams%910862259
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: EndUserAccount%917285328
      m4return_t EndUserAccount (m4objid_t ai_iUserId);

      //## Operation: AddUserSession%893085118; C++
      m4objid_t AddUserSession (ClUSUserInterface * &ai_poUser, ClUSSessionInterface *ai_poSession);

      //## Operation: Stop%928140275
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: IsReadyToClose%928140276
      m4bool_t IsReadyToClose ();

      //## Operation: CheckTimeoutSessions%933330708
      m4return_t CheckTimeoutSessions ();

      //## Operation: SendMessageToAllUsers%943272855
      m4return_t SendMessageToAllUsers (m4pchar_t ai_szMessage);

      //## Operation: GetCommandsId%943286018
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: CreateAction%955610144
      m4return_t CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction);

      //## Operation: ProcessAction%957943674
      m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: EstimateLoad%962700384
      m4return_t EstimateLoad (ClAdminTagList &ao_oTagList);

      //## Operation: RecoverSessionFromCredential%967564558
      m4return_t RecoverSessionFromCredential (M4Credentials *ai_poCredential, ClUSSessionInterface *&ao_poSession);

  public:
    // Additional Public Declarations
      //## begin ClUsersSS%34C7653103CD.public preserve=yes
      //## end ClUsersSS%34C7653103CD.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%3551DFC50035
      //## Role: ClUsersSS::m_pUSCache%3551DFC60371
      //## begin ClUsersSS::m_pUSCache%3551DFC60371.role preserve=no  protected: ClCacheTest {1 -> 1RFHAN}
      ClCacheTest *m_pUSCache;
      //## end ClUsersSS::m_pUSCache%3551DFC60371.role

    // Additional Protected Declarations
      //## begin ClUsersSS%34C7653103CD.protected preserve=yes
      //## end ClUsersSS%34C7653103CD.protected

  private:

    //## Other Operations (specified)
      //## Operation: AddUserAccount%893085116; C++
      m4objid_t AddUserAccount (ClUSUserInterface * &ai_poUser);

    // Data Members for Class Attributes

      //## Attribute: m_oUsersMutex%374E6E7601E4
      //## begin ClUsersSS::m_oUsersMutex%374E6E7601E4.attr preserve=no  private: ClMutex {UA} M4_TRUE
      ClMutex m_oUsersMutex;
      //## end ClUsersSS::m_oUsersMutex%374E6E7601E4.attr

    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%39ABE01A0063
      //## Role: ClUsersSS::m_poCredentialGenerator%39ABE01A03B6
      //## begin ClUsersSS::m_poCredentialGenerator%39ABE01A03B6.role preserve=no  private: ISessionIDGenerator { -> RHAN}
      ISessionIDGenerator *m_poCredentialGenerator;
      //## end ClUsersSS::m_poCredentialGenerator%39ABE01A03B6.role

      //## Association: Subsystem::UsersSubsystem::<unnamed>%3A26453701D4
      //## Role: ClUsersSS::m_oSessRepository%3A2645380081
      //## begin ClUsersSS::m_oSessRepository%3A2645380081.role preserve=no  private: ClSessionDic { -> VHAN}
      ClSessionDic m_oSessRepository;
      //## end ClUsersSS::m_oSessRepository%3A2645380081.role

    // Additional Private Declarations
      //## begin ClUsersSS%34C7653103CD.private preserve=yes
	  m4return_t _CloseAllSessions ( ) ;
      //## end ClUsersSS%34C7653103CD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUsersSS%34C7653103CD.implementation preserve=yes
      //## end ClUsersSS%34C7653103CD.implementation

};

//## begin ClUsersSS%34C7653103CD.postscript preserve=yes
//## end ClUsersSS%34C7653103CD.postscript

//## begin ClIdentitySS%34C7653A0326.preface preserve=yes
//## end ClIdentitySS%34C7653A0326.preface

//## Class: ClIdentitySS%34C7653A0326
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%353C6F0F030D;ClUsersSSVisitor { -> F}
//## Uses: <unnamed>%353CA1750373;ClUSUserVisitor { -> F}

class ClIdentitySS : public ClSSImplementation  //## Inherits: <unnamed>%38E9ADC90253
{
  //## begin ClIdentitySS%34C7653A0326.initialDeclarations preserve=yes
  //## end ClIdentitySS%34C7653A0326.initialDeclarations

  public:
    //## begin ClIdentitySS::eIdentityCommands%350D6BF20367.preface preserve=yes
    //## end ClIdentitySS::eIdentityCommands%350D6BF20367.preface

    //## Class: eIdentityCommands%350D6BF20367
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { LOCK_ALL_SESSIONS = ClSSImplementation::TOTAL_COMMANDS, LOGOFF_ALL_SESSIONS , SEND_MESSAGE_TO_ALL_SESSIONS , TOTAL_COMMANDS }  eIdentityCommands;

    //## begin ClIdentitySS::eIdentityCommands%350D6BF20367.postscript preserve=yes
    //## end ClIdentitySS::eIdentityCommands%350D6BF20367.postscript

    //## Constructors (specified)
      //## Operation: ClIdentitySS%890993642
      ClIdentitySS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClIdentitySS%890993643
      ~ClIdentitySS ();


    //## Other Operations (specified)
      //## Operation: GetUser%885550069
      ClUSUserInterface * GetUser ();

      //## Operation: SetUser%885550070
      void SetUser (ClUSUserInterface *ai_poUser);

      //## Operation: Accept%885550071
      void Accept (ClUSUserVisitor &ai_oVisitor);

      //## Operation: Initialize%885891305
      m4return_t Initialize ();

      //## Operation: ShutDown%885891307
      m4return_t ShutDown ();

      //## Operation: Synchronize%886518769
      virtual m4return_t Synchronize ();

      //## Operation: LockAllSessions%890086921
      m4return_t LockAllSessions ();

      //## Operation: LogOffAllSessions%890086922
      m4return_t LogOffAllSessions ();

      //## Operation: ExecuteCommand%910862260
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%910862261
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%910862262
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: Stop%928256386
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: SendMessageToAllSessions%943272856
      m4return_t SendMessageToAllSessions (m4pchar_t ai_szMessage);

      //## Operation: GetElement%967450716
      //	Función virtual (al igual que todas las de esta clase) que sobrecargada en
      //	otra clase que herede de esta, devolverá culaquier elemento contenido por el
      //	subsistema, en forma de un puntero a void.
      //	El elemento que ha de devolver, se indica con una cadena pasada como
      //	parámetro de entrada.
      //	Elementos pueden ser todas aquellas referencias que el subsistema contenga,
      //	que por agrupación formen el conjunto lógico denominado subsistema.
      //	Por ejemplo, el subsistema sesión tendrá un elemento que es la sesión, el
      //	subsistema servicio tendrá otro que sea la cola de servicio y otro que sea
      //	en propio servicio, etc...
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: SetElement%967450717
      m4return_t SetElement (m4pchar_t ai_szElement, void *ai_poElement);

  public:
    // Additional Public Declarations
      //## begin ClIdentitySS%34C7653A0326.public preserve=yes
      //## end ClIdentitySS%34C7653A0326.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%353F044D0132
      //## Role: ClIdentitySS::m_poUser%353F044E0142
      //## begin ClIdentitySS::m_poUser%353F044E0142.role preserve=no  protected: ClUSUserInterface {1 -> 1RFHAN}
      ClUSUserInterface *m_poUser;
      //## end ClIdentitySS::m_poUser%353F044E0142.role

    // Additional Protected Declarations
      //## begin ClIdentitySS%34C7653A0326.protected preserve=yes
      //## end ClIdentitySS%34C7653A0326.protected

  private:
    // Additional Private Declarations
      //## begin ClIdentitySS%34C7653A0326.private preserve=yes
      //## end ClIdentitySS%34C7653A0326.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClIdentitySS%34C7653A0326.implementation preserve=yes
      //## end ClIdentitySS%34C7653A0326.implementation

};

//## begin ClIdentitySS%34C7653A0326.postscript preserve=yes
//## end ClIdentitySS%34C7653A0326.postscript

//## begin ClUsersSSVisitor%34C86E4802C9.preface preserve=yes
//## end ClUsersSSVisitor%34C86E4802C9.preface

//## Class: ClUsersSSVisitor%34C86E4802C9
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%350D6ACB006A;ClUsersSSStats { -> F}

class ClUsersSSVisitor : public ClVisitor  //## Inherits: <unnamed>%34C86E8F0249
{
  //## begin ClUsersSSVisitor%34C86E4802C9.initialDeclarations preserve=yes
  //## end ClUsersSSVisitor%34C86E4802C9.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitUsersSS%885550073
      void VisitUsersSS (ClUsersSS &ai_oUsersSS);

    // Additional Public Declarations
      //## begin ClUsersSSVisitor%34C86E4802C9.public preserve=yes
      //## end ClUsersSSVisitor%34C86E4802C9.public

  protected:
    // Additional Protected Declarations
      //## begin ClUsersSSVisitor%34C86E4802C9.protected preserve=yes
      //## end ClUsersSSVisitor%34C86E4802C9.protected

  private:
    // Additional Private Declarations
      //## begin ClUsersSSVisitor%34C86E4802C9.private preserve=yes
      //## end ClUsersSSVisitor%34C86E4802C9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUsersSSVisitor%34C86E4802C9.implementation preserve=yes
      //## end ClUsersSSVisitor%34C86E4802C9.implementation

};

//## begin ClUsersSSVisitor%34C86E4802C9.postscript preserve=yes
//## end ClUsersSSVisitor%34C86E4802C9.postscript

//## begin ClSessionSS%34C76544032A.preface preserve=yes
//## end ClSessionSS%34C76544032A.preface

//## Class: ClSessionSS%34C76544032A
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%353C6EF90243;ClUSSessionVisitor { -> F}
//## Uses: <unnamed>%37831B3A0077;ClPDU { -> F}

class ClSessionSS : public ClSSImplementation  //## Inherits: <unnamed>%38E9AE2C0332
{
  //## begin ClSessionSS%34C76544032A.initialDeclarations preserve=yes
  //## end ClSessionSS%34C76544032A.initialDeclarations

  public:
    //## begin ClSessionSS::eSessionCommands%350D8B3603D5.preface preserve=yes
    //## end ClSessionSS::eSessionCommands%350D8B3603D5.preface

    //## Class: eSessionCommands%350D8B3603D5
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { SET_DEBUG_LEVEL = ClSSImplementation::TOTAL_COMMANDS , VIEW_TRACES , SUSPEND_RESUME , LOG_OFF , SEND_MESSAGE_TO_SESSION , CHECK_TIMEOUT_SESSION , TOTAL_COMMANDS} eSessionCommands;

    //## begin ClSessionSS::eSessionCommands%350D8B3603D5.postscript preserve=yes
    //## end ClSessionSS::eSessionCommands%350D8B3603D5.postscript

    //## begin ClSessionSS::eSessionStatus%35AF27EE0198.preface preserve=yes
    //## end ClSessionSS::eSessionStatus%35AF27EE0198.preface

    //## Class: eSessionStatus%35AF27EE0198
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { AUTHENTICATED , AUTHORIZED , BLOCKED , CANCELING , CANCELED } eSessionStatus;

    //## begin ClSessionSS::eSessionStatus%35AF27EE0198.postscript preserve=yes
    //## end ClSessionSS::eSessionStatus%35AF27EE0198.postscript

    //## Constructors (specified)
      //## Operation: ClSessionSS%890993644
      ClSessionSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSessionSS%899809124
      ~ClSessionSS ();


    //## Other Operations (specified)
      //## Operation: Accept%885550072
      void Accept (ClUSSessionVisitor &ai_oVisitor);

      //## Operation: GetSession%885550074
      ClUSSessionInterface * GetSession ();

      //## Operation: SetSession%885550075
      void SetSession (ClUSSessionInterface *ai_poSession);

      //## Operation: Initialize%885891308
      m4return_t Initialize ();

      //## Operation: ShutDown%885891310
      m4return_t ShutDown ();

      //## Operation: Synchronize%886518770
      virtual m4return_t Synchronize ();

      //## Operation: SetDebugLevel%890086923
      m4return_t SetDebugLevel ();

      //## Operation: ViewTraces%890086924
      m4return_t ViewTraces ();

      //## Operation: Suspend%890086925
      m4return_t Suspend ();

      //## Operation: Resume%890086926
      m4return_t Resume ();

      //## Operation: LogOff%890086927
      m4return_t LogOff ();

      //## Operation: GetElement%896526474
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: ExecuteCommand%910862263
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%910862264
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%910862265
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: Stop%928256387
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: CheckTimeout%933665003
      m4bool_t CheckTimeout ();

      //## Operation: SendCSMessage%941113627
      m4return_t SendCSMessage (m4uint32_t ai_iEventMessage, m4pchar_t ai_pMessage);

      //## Operation: SetElement%955100222
      m4return_t SetElement (m4pchar_t ai_szElement, void *ai_poElement);

  public:
    // Additional Public Declarations
      //## begin ClSessionSS%34C76544032A.public preserve=yes
      //## end ClSessionSS%34C76544032A.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::UsersSubsystem::<unnamed>%353F00CE02C9
      //## Role: ClSessionSS::m_poSession%353F00CF00C6
      //## begin ClSessionSS::m_poSession%353F00CF00C6.role preserve=no  protected: ClUSSessionInterface {1 -> 1RFHAN}
      ClUSSessionInterface *m_poSession;
      //## end ClSessionSS::m_poSession%353F00CF00C6.role

    // Additional Protected Declarations
      //## begin ClSessionSS%34C76544032A.protected preserve=yes
      //## end ClSessionSS%34C76544032A.protected

  private:
    // Additional Private Declarations
      //## begin ClSessionSS%34C76544032A.private preserve=yes
      //## end ClSessionSS%34C76544032A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessionSS%34C76544032A.implementation preserve=yes
      //## end ClSessionSS%34C76544032A.implementation

};

//## begin ClSessionSS%34C76544032A.postscript preserve=yes
//## end ClSessionSS%34C76544032A.postscript

//## begin ClUsersSSStats%350D691A00FE.preface preserve=yes
//## end ClUsersSSStats%350D691A00FE.preface

//## Class: ClUsersSSStats%350D691A00FE
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClUsersSSStats : public ClStatistic  //## Inherits: <unnamed>%350D82D000F8
{
  //## begin ClUsersSSStats%350D691A00FE.initialDeclarations preserve=yes
  //## end ClUsersSSStats%350D691A00FE.initialDeclarations

  public:
    //## begin ClUsersSSStats::eUsersStats%350D9B1602A2.preface preserve=yes
    //## end ClUsersSSStats::eUsersStats%350D9B1602A2.preface

    //## Class: eUsersStats%350D9B1602A2
    //## Category: Subsystem::UsersSubsystem%34FD8CE30132
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { USERS_LOGED = ClStatistic::TOTAL_NUM , TOTAL_SESSIONS , CURRENT_SESSIONS , TOTAL_NUM } eUsersStats;

    //## begin ClUsersSSStats::eUsersStats%350D9B1602A2.postscript preserve=yes
    //## end ClUsersSSStats::eUsersStats%350D9B1602A2.postscript

    //## Constructors (specified)
      //## Operation: ClUsersSSStats%890086954
      ClUsersSSStats ();


    //## Other Operations (specified)
      //## Operation: AddnUsersLoged%890086928
      void AddnUsersLoged (ClStatNumericalType ai_lUsersLoged);

      //## Operation: GetnUsersLoged%890086929
      ClStatNumericalType GetnUsersLoged ();

      //## Operation: AddnSessions%890086930
      void AddnSessions (ClStatNumericalType ai_lSessions);

      //## Operation: GetnSessions%890086931
      ClStatNumericalType GetnSessions ();

      //## Operation: Reset%890086948; C++
      //	This operation resets every value in the statistics object. In this class
      //	this operation is abstract and must be defined in derived classes.
      m4return_t Reset ();

      //## Operation: GetStat%890086952; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%890086953
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: AddCurrentSessions%1013505804
      void AddCurrentSessions (m4int32_t ai_iSessions);

      //## Operation: GetCurrentSessions%1013505805
      m4uint32_t GetCurrentSessions ();

  public:
    // Additional Public Declarations
      //## begin ClUsersSSStats%350D691A00FE.public preserve=yes
      //## end ClUsersSSStats%350D691A00FE.public

  protected:
    // Additional Protected Declarations
      //## begin ClUsersSSStats%350D691A00FE.protected preserve=yes
      //## end ClUsersSSStats%350D691A00FE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lUsersLoged%350D696202F6
      //## begin ClUsersSSStats::m_lUsersLoged%350D696202F6.attr preserve=no  private: ClStatNumericalType {UA} 0
      ClStatNumericalType m_lUsersLoged;
      //## end ClUsersSSStats::m_lUsersLoged%350D696202F6.attr

      //## Attribute: m_lTotalSessions%350D82F503C2
      //## begin ClUsersSSStats::m_lTotalSessions%350D82F503C2.attr preserve=no  private: ClStatNumericalType {UA} 0
      ClStatNumericalType m_lTotalSessions;
      //## end ClUsersSSStats::m_lTotalSessions%350D82F503C2.attr

      //## Attribute: m_lCurrentSessions%3C68DEAD0039
      //## begin ClUsersSSStats::m_lCurrentSessions%3C68DEAD0039.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_lCurrentSessions;
      //## end ClUsersSSStats::m_lCurrentSessions%3C68DEAD0039.attr

    // Additional Private Declarations
      //## begin ClUsersSSStats%350D691A00FE.private preserve=yes
      //## end ClUsersSSStats%350D691A00FE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClUsersSSStats%350D691A00FE.implementation preserve=yes
      //## end ClUsersSSStats%350D691A00FE.implementation

};

//## begin ClUsersSSStats%350D691A00FE.postscript preserve=yes
//## end ClUsersSSStats%350D691A00FE.postscript

//## begin ClNewUserAction%38F4AE5B03BA.preface preserve=yes
//## end ClNewUserAction%38F4AE5B03BA.preface

//## Class: ClNewUserAction%38F4AE5B03BA
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClNewUserAction : public ClSSAction  //## Inherits: <unnamed>%38F4AE6401AB
{
  //## begin ClNewUserAction%38F4AE5B03BA.initialDeclarations preserve=yes
  //## end ClNewUserAction%38F4AE5B03BA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNewUserAction%955559601
      ClNewUserAction (ClBaseActionExecutive *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955559602
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClNewUserAction%38F4AE5B03BA.public preserve=yes
      //## end ClNewUserAction%38F4AE5B03BA.public

  protected:
    // Additional Protected Declarations
      //## begin ClNewUserAction%38F4AE5B03BA.protected preserve=yes
      //## end ClNewUserAction%38F4AE5B03BA.protected

  private:
    // Additional Private Declarations
      //## begin ClNewUserAction%38F4AE5B03BA.private preserve=yes
      //## end ClNewUserAction%38F4AE5B03BA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNewUserAction%38F4AE5B03BA.implementation preserve=yes
      //## end ClNewUserAction%38F4AE5B03BA.implementation

};

//## begin ClNewUserAction%38F4AE5B03BA.postscript preserve=yes
//## end ClNewUserAction%38F4AE5B03BA.postscript

//## begin ClEndUserSessionAction%38F5CF16029E.preface preserve=yes
//## end ClEndUserSessionAction%38F5CF16029E.preface

//## Class: ClEndUserSessionAction%38F5CF16029E
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClEndUserSessionAction : public ClSSAction  //## Inherits: <unnamed>%38F5CF3700A6
{
  //## begin ClEndUserSessionAction%38F5CF16029E.initialDeclarations preserve=yes
  //## end ClEndUserSessionAction%38F5CF16029E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEndUserSessionAction%955610165
      ClEndUserSessionAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955610166
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClEndUserSessionAction%38F5CF16029E.public preserve=yes
      //## end ClEndUserSessionAction%38F5CF16029E.public

  protected:
    // Additional Protected Declarations
      //## begin ClEndUserSessionAction%38F5CF16029E.protected preserve=yes
      //## end ClEndUserSessionAction%38F5CF16029E.protected

  private:
    // Additional Private Declarations
      //## begin ClEndUserSessionAction%38F5CF16029E.private preserve=yes
      //## end ClEndUserSessionAction%38F5CF16029E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEndUserSessionAction%38F5CF16029E.implementation preserve=yes
      //## end ClEndUserSessionAction%38F5CF16029E.implementation

};

//## begin ClEndUserSessionAction%38F5CF16029E.postscript preserve=yes
//## end ClEndUserSessionAction%38F5CF16029E.postscript

//## begin ClRecoverSesionAction%39ABDD7F0371.preface preserve=yes
//## end ClRecoverSesionAction%39ABDD7F0371.preface

//## Class: ClRecoverSesionAction%39ABDD7F0371
//## Category: Subsystem::UsersSubsystem%34FD8CE30132
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRecoverSesionAction : public ClSSAction  //## Inherits: <unnamed>%39ABDD8D0104
{
  //## begin ClRecoverSesionAction%39ABDD7F0371.initialDeclarations preserve=yes
  //## end ClRecoverSesionAction%39ABDD7F0371.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRecoverSesionAction%967564559
      ClRecoverSesionAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%967564560
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClRecoverSesionAction%39ABDD7F0371.public preserve=yes
      //## end ClRecoverSesionAction%39ABDD7F0371.public

  protected:
    // Additional Protected Declarations
      //## begin ClRecoverSesionAction%39ABDD7F0371.protected preserve=yes
      //## end ClRecoverSesionAction%39ABDD7F0371.protected

  private:
    // Additional Private Declarations
      //## begin ClRecoverSesionAction%39ABDD7F0371.private preserve=yes
      //## end ClRecoverSesionAction%39ABDD7F0371.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRecoverSesionAction%39ABDD7F0371.implementation preserve=yes
      //## end ClRecoverSesionAction%39ABDD7F0371.implementation

};

//## begin ClRecoverSesionAction%39ABDD7F0371.postscript preserve=yes
//## end ClRecoverSesionAction%39ABDD7F0371.postscript

// Class ClUsersSS 

// Class ClIdentitySS 

// Class ClUsersSSVisitor 

// Class ClSessionSS 

// Class ClUsersSSStats 

// Class ClNewUserAction 

// Class ClEndUserSessionAction 

// Class ClRecoverSesionAction 

//## begin module%34C86C19026B.epilog preserve=yes
//## end module%34C86C19026B.epilog


#endif
