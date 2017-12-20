//## begin module%342689320375.cm preserve=no
//## end module%342689320375.cm

//## begin module%342689320375.cp preserve=no
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
//## end module%342689320375.cp

//## Module: ClLocalMonitor%342689320375; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\cllocalmonitor.hpp

#ifndef cllocalmonitor_h
#define cllocalmonitor_h 1

//## begin module%342689320375.additionalIncludes preserve=no
//## end module%342689320375.additionalIncludes

//## begin module%342689320375.includes preserve=yes
#include <tsap.hpp>
#include <conninstance.hpp>
#include <cllminterface.hpp>
#include <navigatorf.hpp>
#include <fifoq.hpp>
#include <m4log.hpp>
#include <sessiondic.hpp>
#include <invoker.hpp>
#include <cllist.hpp>
#include <clservicedicc.hpp>
#include <clcondictionary.hpp>

class ClConfiguration;
class ClCSSessManager;
class ClLMStats;
class ClLMVisitor;
class ClSSFactory;
class ClSystem;
class ClUsersSS;
class ClRequestSS;
class ClRequest;
class ClLauncherInterface;
class ClSSInterfaz;
class ClCSAdministrator ;
class ClAdminAcceptor ;
class ClConnection;

//## end module%342689320375.additionalDeclarations


//## begin eCommType%39A649990362.preface preserve=yes
//## end eCommType%39A649990362.preface

//## Class: eCommType%39A649990362
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum { M4_V_BASE_COMM = 0 , M4_WORKER_BASE_COMM } eCommType;

//## begin eCommType%39A649990362.postscript preserve=yes
//## end eCommType%39A649990362.postscript

//## Class: ClLocalMonitor%34222C0402DB
//	The local monitor controls the server tasks such as the number of current
//	services or the number of queues in the system.
//	It know the state of the server by the dictionary it has and it is requested
//	about this state by the other instances of modules in the serve
//## Category: LocalMonitor%379EC99A01CC
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345798BD0041; { -> F}
//## Uses: <unnamed>%34587A9100CC;ClLMVisitor { -> F}
//## Uses: <unnamed>%347EFFEB0305;ClLog { -> }
//## Uses: <unnamed>%34D05A080246;ClLauncherInterface { -> F}
//## Uses: <unnamed>%34D05A1B02BC;ClCSSessManager { -> F}
//## Uses: <unnamed>%34E895C003B3;ClSSFactory { -> F}
//## Uses: <unnamed>%355737830300;ClUsersSS { -> F}
//## Uses: <unnamed>%356301FD0159; { -> F}
//## Uses: <unnamed>%357D7A7802DF;ClRequestSS { -> F}
//## Uses: <unnamed>%367A68EC015C;ClConfiguration { -> F}
//## Uses: <unnamed>%367A6A9203A7;ClRequest { -> F}
//## Uses: <unnamed>%36A36F7C02C7;ClNavigatorFirewall { -> }
//## Uses: <unnamed>%36A37C1C01E8;ClConnection { -> F}
//## Uses: <unnamed>%371F6C950236;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%377C88C00383;ClLogicalService { -> F}
//## Uses: <unnamed>%377C912C01FD;ClUSSessionStats { -> F}
//## Uses: <unnamed>%37D38EF50117;ClServiceInterface { -> F}
//## Uses: <unnamed>%393BDC3603CB;ClSSInterfaz { -> F}
//## Uses: <unnamed>%39A649CB0223;eCommType { -> }
//## Uses: <unnamed>%34D0672801B2; { -> F}

class ClLocalMonitor : public ClLocalMonitorInterface  //## Inherits: <unnamed>%37D37C3B01DB
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClLocalMonitor%874936019
      //	Destructor
      ~ClLocalMonitor ();


    //## Other Operations (specified)
      //## Operation: GetConnection%874936020
      //	This method is caled by the receiver when it recieve a request, to know the
      //	connection object for this request and all its atributes.
      //	the function return the pointer to the connection
      ClConnInstance * GetConnection (m4int32_t ai_IdConnection	// The connection ID for indentify the connection
      );

      //## Operation: NewConnection%875696747
      //	the acapter thread recieve a new conneciton request and call this local
      //	monitor method passing it the current connection object pointer.
      //	Then this method call the apropiate administrator method object to procees
      //	the request as it require.
      m4return_t NewConnection (ClConnection *ai_poConnection);

      //## Operation: RemoveConnection%875696750
      //	This operation remove the connection entry from the dictinary.
      //	This function is normaly called by the receiver after remove its connection
      //	object and destroy the phisical connection.
      m4return_t RemoveConnection (ClConnInstance* ai_poConnection);

      //## Operation: DestroyConnection%876753079
      m4return_t DestroyConnection (ClConnInstance* ai_poConnection);

      //## Operation: ShutDown%876753077
      //	when the administrator want to shut down the server station, must use this
      //	method.
      //	By using it before shout down the sistem, the services are closed.
      m4return_t ShutDown ();

      //## Operation: CreateService%875000090
      //	it's an administrator method that creates a new service and put it into the
      //	apropiate dictionary.
      ClServiceInterface * CreateService (m4int32_t ai_IdService, m4uint16_t ai_iNumQItems = 0);

      //## Operation: ShutDownService%876753078
      //	Disables a service which service Id is passed as argument.
      //	The only thing this method does is to put the service enable flag to FALSE
      //	and then when all the requests have been proccessed the Local Monitor
      //	supervisor thread terminates the launcher threads and when the threads have
      //	terminated the service is remeved from server.
      m4return_t ShutDownService (m4int32_t ai_IdService);

      //## Operation: CreateSessionManager%876990186
      //	it's an administrator method that creates a new service and put it into the
      //	apropiate dictionary.
      ClCSSessManager * CreateSessionManager (ClUsersSS *ai_poUsersSS);

      //## Operation: ShutDownSessionManager%878114632
      //	Shut down one determined connection manager
      m4return_t ShutDownSessionManager (ClCSSessManager *ai_poSessManager = NULL);

      //## Operation: CreateLauncher%878114633
      //	Create a Launcher and attaches it to a batch or a general service queue.
      ClLauncherInterface * CreateLauncher (m4int32_t ai_iService, m4char_t ai_iType, ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL, m4pchar_t ai_szDllName = NULL, m4pchar_t ai_szFuncName = NULL);

      //## Operation: ShutDownLauncher%878114634
      //	Shut down one launcher of a service saying the number of the service group
      //	to be removed from and the Id of the Launcher
      m4return_t ShutDownLauncher (ClLauncherInterface *ai_poLauncher);

      //## Operation: Accept%878200308
      //	Accept a visitor object to visit the local Monitor
      void Accept (ClLMVisitor &ai_oVisitor);

      //## Operation: CreateAdministrator%878225915
      //	Create a new adminostrator thread with the new admistrative connection
      //	accepted.
      ClCSAdministrator * CreateAdministrator (ClTSAP *ai_poTSAP);

      //## Operation: CreateAdminAcceptor%887707939
      ClAdminAcceptor * CreateAdminAcceptor (m4uint16_t ai_iControlPort, m4uint32_t ai_iConnectionType = 0);

      //## Operation: ShutDownAdministrator%878225916
      //	Shuts down one administrator
      m4return_t ShutDownAdministrator (ClCSAdministrator *ai_poAdministrator = NULL);

      //## Operation: GetControlQueue%882177110
      ClControlQ * GetControlQueue ();

      //## Operation: GetStats%890061477
      ClStatistic * GetStats ();

      //## Operation: StartUp%894962381
      m4return_t StartUp ();

      //## Operation: Instance%895738253
      static ClLocalMonitor * Instance ();

      //## Operation: GetSessionDic%901531332
      ClSessionDic * GetSessionDic ();

      //## Operation: SetStats%901796097
      void SetStats (ClLMStats *ai_poStats);

      //## Operation: Synchronize%936613830
      void Synchronize ();

      //## Operation: DestroyConnectionId%962784433
      m4return_t DestroyConnectionId (m4uint32_t ai_IdConnection);

      //## Operation: GetCommType%967197715
      eCommType GetCommType ();

      //## Operation: SetCommType%967197716
      void SetCommType (eCommType ai_iCommType);

    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%342262C90355
      //## Role: ClLocalMonitor::m_oInvoker%342262CA0338
      //## begin ClLocalMonitor::m_oInvoker%342262CA0338.role preserve=no  public: ClInvoker {0 -> 1VHAN}
      ClInvoker m_oInvoker;
      //## end ClLocalMonitor::m_oInvoker%342262CA0338.role

      //## Association: LocalMonitor::<unnamed>%342263E3007F
      //## Role: ClLocalMonitor::m_oServiceDictionary%342263E303E6
      //## begin ClLocalMonitor::m_oServiceDictionary%342263E303E6.role preserve=no  public: ClServiceDictionary { -> VHN}
      ClServiceDictionary m_oServiceDictionary;
      //## end ClLocalMonitor::m_oServiceDictionary%342263E303E6.role

      //## Association: LocalMonitor::<unnamed>%342263E90182
      //## Role: ClLocalMonitor::m_oConDictionary%342263EA009D
      //## begin ClLocalMonitor::m_oConDictionary%342263EA009D.role preserve=no  public: ClConDictionary { -> VHN}
      ClConDictionary m_oConDictionary;
      //## end ClLocalMonitor::m_oConDictionary%342263EA009D.role

      //## Association: LocalMonitor::<unnamed>%3438A50F0160
      //## Role: ClLocalMonitor::m_oOutputQueue%3438A510017F
      //## begin ClLocalMonitor::m_oOutputQueue%3438A510017F.role preserve=no  public: ClOutputQ {1 -> 1VHAN}
      ClOutputQ m_oOutputQueue;
      //## end ClLocalMonitor::m_oOutputQueue%3438A510017F.role

      //## Association: LocalMonitor::<unnamed>%34462C880191
      //## Role: ClLocalMonitor::m_oControlQueue%34462C8900FC
      //## begin ClLocalMonitor::m_oControlQueue%34462C8900FC.role preserve=no  public: ClControlQ {1 -> 1VHAN}
      ClControlQ m_oControlQueue;
      //## end ClLocalMonitor::m_oControlQueue%34462C8900FC.role

      //## Association: LocalMonitor::<unnamed>%3459D805017E
      //## Role: ClLocalMonitor::m_poLMStats%3459D80600E0
      //## begin ClLocalMonitor::m_poLMStats%3459D80600E0.role preserve=no  public: ClLMStats { -> RFHN}
      ClLMStats *m_poLMStats;
      //## end ClLocalMonitor::m_poLMStats%3459D80600E0.role

      //## Association: LocalMonitor::<unnamed>%346AFF5D0356
      //## Role: ClLocalMonitor::m_oActiveList%346AFF6002B0
      //## begin ClLocalMonitor::m_oActiveList%346AFF6002B0.role preserve=no  public: ClActiveList { -> VHN}
      ClActiveList m_oActiveList;
      //## end ClLocalMonitor::m_oActiveList%346AFF6002B0.role

      //## Association: LocalMonitor::<unnamed>%34D0622001D8
      //## Role: ClLocalMonitor::m_oSchedulerList%34D06223018C
      //## begin ClLocalMonitor::m_oSchedulerList%34D06223018C.role preserve=no  public: ClList { -> VHN}
      ClList m_oSchedulerList;
      //## end ClLocalMonitor::m_oSchedulerList%34D06223018C.role

      //## Association: LocalMonitor::<unnamed>%34D062DA024D
      //## Role: ClLocalMonitor::m_oReceiverList%34D062DB02D1
      //## begin ClLocalMonitor::m_oReceiverList%34D062DB02D1.role preserve=no  public: ClList { -> VHN}
      ClList m_oReceiverList;
      //## end ClLocalMonitor::m_oReceiverList%34D062DB02D1.role

      //## Association: LocalMonitor::<unnamed>%34D0630E02CA
      //## Role: ClLocalMonitor::m_oSenderList%34D0630F03E4
      //## begin ClLocalMonitor::m_oSenderList%34D0630F03E4.role preserve=no  public: ClList { -> VHN}
      ClList m_oSenderList;
      //## end ClLocalMonitor::m_oSenderList%34D0630F03E4.role

      //## Association: LocalMonitor::<unnamed>%34D0633200AF
      //## Role: ClLocalMonitor::m_oSessionManagerList%34D063330197
      //## begin ClLocalMonitor::m_oSessionManagerList%34D063330197.role preserve=no  public: ClList { -> VHN}
      ClList m_oSessionManagerList;
      //## end ClLocalMonitor::m_oSessionManagerList%34D063330197.role

      //## Association: LocalMonitor::<unnamed>%34D063970155
      //## Role: ClLocalMonitor::m_oLauncherList%34D0639801A6
      //## begin ClLocalMonitor::m_oLauncherList%34D0639801A6.role preserve=no  public: ClList { -> VHN}
      ClList m_oLauncherList;
      //## end ClLocalMonitor::m_oLauncherList%34D0639801A6.role

      //## Association: LocalMonitor::<unnamed>%34D063F50182
      //## Role: ClLocalMonitor::m_oAdministratorList%34D063F6027E
      //## begin ClLocalMonitor::m_oAdministratorList%34D063F6027E.role preserve=no  public: ClList { -> VHN}
      ClList m_oAdministratorList;
      //## end ClLocalMonitor::m_oAdministratorList%34D063F6027E.role

      //## Association: LocalMonitor::<unnamed>%34D06CFB00F7
      //## Role: ClLocalMonitor::m_oDistributorList%34D06CFC00E5
      //## begin ClLocalMonitor::m_oDistributorList%34D06CFC00E5.role preserve=no  public: ClList { -> VHN}
      ClList m_oDistributorList;
      //## end ClLocalMonitor::m_oDistributorList%34D06CFC00E5.role

      //## Association: LocalMonitor::<unnamed>%34EC4E9902AE
      //## Role: ClLocalMonitor::m_oClosedConnection%34EC4E9C03AC
      //## begin ClLocalMonitor::m_oClosedConnection%34EC4E9C03AC.role preserve=no  public: ClConDictionary { -> VHAN}
      ClConDictionary m_oClosedConnection;
      //## end ClLocalMonitor::m_oClosedConnection%34EC4E9C03AC.role

      //## Association: LocalMonitor::<unnamed>%3563E1180314
      //## Role: ClLocalMonitor::m_poLocalMonitor%3563E11E0309
      //## begin ClLocalMonitor::m_poLocalMonitor%3563E11E0309.role preserve=no  public: static ClLocalMonitor { -> RHAN}
      static ClLocalMonitor *m_poLocalMonitor;
      //## end ClLocalMonitor::m_poLocalMonitor%3563E11E0309.role

  protected:
    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%34D0704E0123
      //## Role: ClLocalMonitor::m_poSystem%34D0704F01E3
      //## begin ClLocalMonitor::m_poSystem%34D0704F01E3.role preserve=no  public: ClSystem { -> RFHN}
      ClSystem *m_poSystem;
      //## end ClLocalMonitor::m_poSystem%34D0704F01E3.role

      //## Association: LocalMonitor::<unnamed>%35BC464802B6
      //## Role: ClLocalMonitor::m_oSessionDic%35BC464902C2
      //## begin ClLocalMonitor::m_oSessionDic%35BC464902C2.role preserve=no  protected: ClSessionDic { -> VHAN}
      ClSessionDic m_oSessionDic;
      //## end ClLocalMonitor::m_oSessionDic%35BC464902C2.role

  private:
    //## Constructors (specified)
      //## Operation: ClLocalMonitor%874936018
      //	Defect constructor
      ClLocalMonitor ();


    //## Other Operations (specified)
      //## Operation: BuildCommands%875000089
      //	This is a previous operatin of the l0cal monitor and consist of buil the
      //	objects of the command motor. Those objects are made for administrator
      //	methods and each object process each administrator method
      m4return_t BuildCommands ();

    // Data Members for Class Attributes

      //## Attribute: m_iCommunicationType%39A64939009D
      //## begin ClLocalMonitor::m_iCommunicationType%39A64939009D.attr preserve=no  private: eCommType {UA} 
      eCommType m_iCommunicationType;
      //## end ClLocalMonitor::m_iCommunicationType%39A64939009D.attr

  private: //## implementation
  //## begin ClLocalMonitor%34222C0402DB.friends preserve=no
    friend class ClLMVisitor;
  //## end ClLocalMonitor%34222C0402DB.friends
};

// Class ClLocalMonitor 


//## Other Operations (inline)
inline eCommType ClLocalMonitor::GetCommType ()
{
  //## begin ClLocalMonitor::GetCommType%967197715.body preserve=yes
	return m_iCommunicationType ;
  //## end ClLocalMonitor::GetCommType%967197715.body
}

inline void ClLocalMonitor::SetCommType (eCommType ai_iCommType)
{
  //## begin ClLocalMonitor::SetCommType%967197716.body preserve=yes
	m_iCommunicationType= ai_iCommType ;
  //## end ClLocalMonitor::SetCommType%967197716.body
}

//## begin module%342689320375.epilog preserve=yes
//## end module%342689320375.epilog


#endif
