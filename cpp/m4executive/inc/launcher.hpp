//## begin module%343E2BE001AE.cm preserve=no
//## end module%343E2BE001AE.cm

//## begin module%343E2BE001AE.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Executive
//	 File:				Launcher.cpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971016
//	 Language:			C++
//	 Operating System:	WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%343E2BE001AE.cp

//## Module: LauncherInterface%343E2BE001AE; Package specification
//## Subsystem: M4Executive::inc%379462FF024C
//## Source file: F:\integration\m4executive\inc\launcher.hpp

#ifndef launcherinterface_h
#define launcherinterface_h 1

//## begin module%343E2BE001AE.additionalIncludes preserve=no
//## end module%343E2BE001AE.additionalIncludes

//## begin module%343E2BE001AE.includes preserve=yes
#include <m4executive_dll.hpp>
#include <m4srvres.hpp>
#include <clssinvoker.hpp>
//## end module%343E2BE001AE.includes

// m4types
#include <m4types.hpp>
// m4exception
#include <m4exception.hpp>
// Statistics
#include <statistics.hpp>
// CSUtil
#include <csutil.hpp>
// NavigatorF
#include <navigatorf.hpp>
// staterecord
#include <staterecord.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// ServiceQ
#include <serviceq.hpp>

class ClConfiguration;
class ClLogSystem;
class ClLauncherVisitor;
class ClExecutive;
class ClLauncherStats;

//## begin module%343E2BE001AE.declarations preserve=no
//## end module%343E2BE001AE.declarations

//## begin module%343E2BE001AE.additionalDeclarations preserve=yes
#include <m4log.hpp>
static void LogCallback(m4pvoid_t	pArg)
{
	m4pcchar_t		pszArg = (m4pcchar_t)pArg;
	SETCODEF(M4_EXCEPTION_UNKNOWN, ERRORLOG,"#*S1*#%s", pszArg);
}

//  >> ADD EFV 990322
#define OBL_LAUCHER_INFO_USER_ID		"LAUNCHER_USER_ID"
#define OBL_LAUCHER_INFO_NO_USER_ID		"NO USER"
#define OBL_LAUCHER_INFO_SESSION_ID		"LAUNCHER_SESSION_ID"
#define OBL_LAUCHER_INFO_NO_SESSION_ID	"NO SESSION ID"
#define	OBL_LAUCHER_INFO_START			"LAUNCHER_START"
#define	OBL_LAUCHER_INFO_NO_START		"-"
#define	OBL_LAUCHER_INFO_END			"LAUNCHER_END"
#define	OBL_LAUCHER_INFO_NO_END			"-"
#define OBL_LAUCHER_INFO_STATE			"LAUNCHER_STATE"
#define OBL_LAUCHER_INFO_NO_STATE		"WAITING"
//  << END EFV 990322

//## end module%343E2BE001AE.additionalDeclarations


//## begin ClLauncherInterface%34200012029B.preface preserve=yes
//## end ClLauncherInterface%34200012029B.preface

//## Class: ClLauncherInterface%34200012029B
//	This class Executes Requests received from a Service Queue.
//	Remarks
//	Es un componente del sistema de comunicaciones que extrae de la service
//	queue una request, recoge (de otros componentes) toda la información
//	relativa a la satisfacción de la misma y se la proporciona al objeto que se
//	encargará de su ejecución. Una vez finalizada la operation solicitada, se
//	obtiene un valor de resultado y se encola en la output queue para que sea
//	propagada hacia el client que la solicitó
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3558352F0087;ClNavigatorFirewall { -> }
//## Uses: <unnamed>%356D262303B9;ClLauncherVisitor { -> F}
//## Uses: <unnamed>%3694E8E80038;ClConfiguration { -> F}
//## Uses: <unnamed>%3826B88A0379; { -> F}
//## Uses: <unnamed>%34622ABD02E5;ClLauncherStats { -> F}
//## Uses: <unnamed>%383A621E03D2;M4RuntimeException { -> }
//## Uses: <unnamed>%383A754902E8;ClAdminTagList { -> }

class M4_DECL_M4EXECUTIVE ClLauncherInterface : public ClActiveClass  //## Inherits: <unnamed>%3420001202CA
{
  //## begin ClLauncherInterface%34200012029B.initialDeclarations preserve=yes
  //## end ClLauncherInterface%34200012029B.initialDeclarations

  public:
    //## begin ClLauncherInterface::eLauncherCommands%36F7AE920305.preface preserve=yes
    //## end ClLauncherInterface::eLauncherCommands%36F7AE920305.preface

    //## Class: eLauncherCommands%36F7AE920305; Instantiated Class
    //## Category: M4Executive%35F65F3D007A
    //## Subsystem: M4Executive::inc%379462FF024C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {QUIT = 0, PAUSE, RESUME, USED_MEMORY, TOTAL_COMMANDS} eLauncherCommands;

    //## begin ClLauncherInterface::eLauncherCommands%36F7AE920305.postscript preserve=yes
    //## end ClLauncherInterface::eLauncherCommands%36F7AE920305.postscript

    //## Destructor (specified)
      //## Operation: ~ClLauncherInterface%875605498; C++
      //	Default destructor for class Scheduler.
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      virtual ~ClLauncherInterface ();


    //## Other Operations (specified)
      //## Operation: Accept%877356013; C++
      //	Allows a visitor to enter the class and retrieve statistics.
      virtual void Accept (ClLauncherVisitor &ai_oStats);

      //## Operation: Initialize%893925596; C++
      virtual m4return_t Initialize (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Destroy%893925597
      virtual m4return_t Destroy () = 0;

      //## Operation: ExecuteCommand%922189093
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Run%943284072; C++
      //	This operation provides the starting point for Service Op execution. To
      //	accomplish this, several steps are taken:
      //	- Pop a Request from the Service queue.
      //	- Extract Op related info from the PDU.
      //	- Build a ClImage Object with all data needed by the executives.
      //	- Retrieve the answer and create a Reply PDU.
      //	- Push the satisfied request into an Output queue.
      virtual void Run (void );

      //## Operation: _Dump%921058929; C++
      virtual m4return_t _Dump ();

      //## Operation: _CleanUp%927196071; C++
      virtual m4return_t _CleanUp (ClBaseRequest *ai_poRequest);

      //## Operation: GetStatus%982580498
      m4int16_t GetStatus ();

      //## Operation: SetSSInvoker%994342569
      static void SetSSInvoker (ClSSInvokerAbstract *ai_poInvoker);

  public:
    // Additional Public Declarations
      //## begin ClLauncherInterface%34200012029B.public preserve=yes
      //## end ClLauncherInterface%34200012029B.public

  protected:
    //## Constructors (specified)
      //## Operation: ClLauncherInterface%875605497; C++
      //	Parameterized constructor for class Scheduler.
      //## Concurrency: Synchonous
      //## Semantics:
      //	Performs generic initialization operation upon its attributes.
      ClLauncherInterface (ClExecutive *ai_pExecutive, ClStatistic *ai_poStats = NULL, ClServiceQ *ai_poServiceQ = NULL);


    //## Other Operations (specified)
      //## Operation: _ChangeState%942750028; C++
      m4return_t _ChangeState (const m4pchar_t ai_pszState);

      //## Operation: _UpdateConfigProperty%942750029; C++
      m4return_t _UpdateConfigProperty (const m4pcchar_t ai_pszPropertyTag, const m4pcchar_t ai_pszPropertyValue);

      //## Operation: _GetNextRequest%943284068; C++
      virtual m4return_t _GetNextRequest (ClBaseRequest *&ao_poRequest) = 0;

      //## Operation: _PrepareExecution%943284069; C++
      virtual m4return_t _PrepareExecution (ClBaseRequest *ai_poRequest) = 0;

      //## Operation: _ExecuteRequest%943284070
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      virtual m4return_t _ExecuteRequest (ClBaseRequest *ai_poRequest) = 0;

      //## Operation: _ReplyRequest%943284071; C++
      virtual m4return_t _ReplyRequest (ClBaseRequest *ai_poRequest) = 0;

    // Data Members for Class Attributes

      //## Attribute: m_pNavigator%36F676F102A8
      //## begin ClLauncherInterface::m_pNavigator%36F676F102A8.attr preserve=no  protected: ClNavigatorFirewall * {UA} NULL
      ClNavigatorFirewall *m_pNavigator;
      //## end ClLauncherInterface::m_pNavigator%36F676F102A8.attr

      //## Attribute: m_pConfig%36F6771A03B5
      //## begin ClLauncherInterface::m_pConfig%36F6771A03B5.attr preserve=no  protected: ClConfiguration * {UA} NULL
      ClConfiguration *m_pConfig;
      //## end ClLauncherInterface::m_pConfig%36F6771A03B5.attr

    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%3430E39903C7
      //## Role: ClLauncherInterface::m_pExecutive%3430E39A02EC
      //## begin ClLauncherInterface::m_pExecutive%3430E39A02EC.role preserve=no  protected: ClExecutive {1 -> 1RFHAN}
      ClExecutive *m_pExecutive;
      //## end ClLauncherInterface::m_pExecutive%3430E39A02EC.role

      //## Association: M4Executive::<unnamed>%379469E00190
      //## Role: ClLauncherInterface::m_pLog%379469E1008D
      //## begin ClLauncherInterface::m_pLog%379469E1008D.role preserve=no  protected: ClLogSystem {1 -> 1RFHAN}
      ClLogSystem *m_pLog;
      //## end ClLauncherInterface::m_pLog%379469E1008D.role

      //## Association: M4Executive::<unnamed>%37946A7E0179
      //## Role: ClLauncherInterface::m_pServiceQ%37946A7F001C
      //## begin ClLauncherInterface::m_pServiceQ%37946A7F001C.role preserve=no  protected: ClServiceQ {1 -> 1RHAN}
      ClServiceQ *m_pServiceQ;
      //## end ClLauncherInterface::m_pServiceQ%37946A7F001C.role

      //## Association: M4Executive::<unnamed>%379754020353
      //## Role: ClLauncherInterface::m_bInitialized%3797540301B0
      //## begin ClLauncherInterface::m_bInitialized%3797540301B0.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bInitialized;
      //## end ClLauncherInterface::m_bInitialized%3797540301B0.role

      //## Association: M4Executive::<unnamed>%380AED0300D7
      //## Role: ClLauncherInterface::m_iExecutionTimeout%380AED040010
      //## begin ClLauncherInterface::m_iExecutionTimeout%380AED040010.role preserve=no  protected: m4uint_t { -> 1VHAN}
      m4uint_t m_iExecutionTimeout;
      //## end ClLauncherInterface::m_iExecutionTimeout%380AED040010.role

      //## Association: M4Executive::<unnamed>%3B44762801DC
      //## Role: ClLauncherInterface::m_poSSInvoker%3B4476290120
      //## begin ClLauncherInterface::m_poSSInvoker%3B4476290120.role preserve=no  protected: static ClSSInvokerAbstract { -> RFHAN}
      static ClSSInvokerAbstract *m_poSSInvoker;
      //## end ClLauncherInterface::m_poSSInvoker%3B4476290120.role

    // Additional Protected Declarations
      //## begin ClLauncherInterface%34200012029B.protected preserve=yes
      //## end ClLauncherInterface%34200012029B.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherInterface%34200012029B.private preserve=yes
      //## end ClLauncherInterface%34200012029B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherInterface%34200012029B.implementation preserve=yes
      //## end ClLauncherInterface%34200012029B.implementation

};

//## begin ClLauncherInterface%34200012029B.postscript preserve=yes
//## end ClLauncherInterface%34200012029B.postscript

//## begin ClLauncherStats%346220B60258.preface preserve=yes
//## end ClLauncherStats%346220B60258.preface

//## Class: ClLauncherStats%346220B60258
//	This class provides a  container where Service Execution statistical data is
//	to be stored. A specific visitor can retrieve such data and process them
//	conveniently.
//
//	Access to the contents of this class is performed on an ID basis and through
//	the appropriate get/set function
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3833F8FF0044;ClPrecisionTicker { -> F}

class M4_DECL_M4EXECUTIVE ClLauncherStats : public ClActiveClassStats  //## Inherits: <unnamed>%346220CA03B0
{
  //## begin ClLauncherStats%346220B60258.initialDeclarations preserve=yes
  //## end ClLauncherStats%346220B60258.initialDeclarations

  public:
    //## Class: eLauncherStatsID%34D7777A023A
    //## Category: M4Executive%35F65F3D007A
    //## Subsystem: M4Executive::inc%379462FF024C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {ERROR_REQUEST_COUNT=ClActiveClassStats::TOTAL_NUM, TIMED_OUT_REQUEST_COUNT, USER_ABORTED_REQUEST_COUNT , ADMIN_ABORTED_REQUEST_COUNT , SUCCESS_REQUEST_COUNT, CURRENT_STATE, FIV_SEC_TPS, THIR_SEC_TPS, FIV_MIN_TPS, FIV_SEC_AVG_EXEC, THIR_SEC_AVG_EXEC, FIV_MIN_AVG_EXEC, FIV_SEC_SSUSE, THIR_SEC_SSUSE, FIV_MIN_SSUSE, FIV_SEC_WAIT_TIME, THIR_SEC_WAIT_TIME, FIV_MIN_WAIT_TIME, TOTAL_NUM} eLauncherStatsID;

    //## Constructors (specified)
      //## Operation: ClLauncherStats%884795084
      //	Default constructor for class ClStatistics.
      ClLauncherStats ();

    //## Destructor (specified)
      //## Operation: ~ClLauncherStats%884795085
      //	Default constructor for class ClStatistics.
      ~ClLauncherStats ();


    //## Other Operations (specified)
      //## Operation: Reset%884795086; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%886671657; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%886671658
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      //## Operation: AddWaitTime%942928609; C++
      void AddWaitTime (m4uint_t ai_iEllapsedTime);

    //## Get and Set Operations for Associations (generated)

      //## Association: M4Executive::<unnamed>%37FC4FE70247
      //## Role: ClLauncherStats::nUserAbortedRequest%37FC4FE80158
      ClStatNumericalType& GetnUserAbortedRequest ();
      void AddnUserAbortedRequest (const ClStatNumericalType& value);

      //## Association: M4Executive::<unnamed>%37FC4F810309
      //## Role: ClLauncherStats::nErrorRequest%37FC4F8201F2
      ClStatNumericalType& GetnErrorRequest ();
      void AddnErrorRequest (const ClStatNumericalType& value);

      //## Association: M4Executive::<unnamed>%37FC500D022E
      //## Role: ClLauncherStats::nAdminAbortedRequest%37FC500E0104
      ClStatNumericalType& GetnAdminAbortedRequest ();
      void AddnAdminAbortedRequest (const ClStatNumericalType& value);

      //## Association: M4Executive::<unnamed>%37FC50410052
      //## Role: ClLauncherStats::nSuccessRequest%37FC5041032E
      ClStatNumericalType& GetnSuccessRequest ();
      void AddnSuccessRequest (const ClStatNumericalType& value);

      //## Association: M4Executive::<unnamed>%37946C34015B
      //## Role: ClLauncherStats::nTimedOutRequest%37946C340382
      ClStatNumericalType& GetnTimedOutRequest ();
      void AddnTimedOutRequest (const ClStatNumericalType& value);

    // Data Members for Class Attributes

      //## Attribute: PrepareState%38313B7C01EC
      //## begin ClLauncherStats::PrepareState%38313B7C01EC.attr preserve=no  public: static m4pchar_t {VAC} "Preparing input request"
      static const m4pchar_t  PrepareState;
      //## end ClLauncherStats::PrepareState%38313B7C01EC.attr

      //## Attribute: DiscardedState%383141080099
      //## begin ClLauncherStats::DiscardedState%383141080099.attr preserve=no  public: static m4pchar_t {UAC} "Aborted by unhandled exception"
      static const m4pchar_t  DiscardedState;
      //## end ClLauncherStats::DiscardedState%383141080099.attr

      //## Attribute: InQueueState%38341651000B
      //## begin ClLauncherStats::InQueueState%38341651000B.attr preserve=no  public: static m4pchar_t {VAC} "In Queue"
      static const m4pchar_t  InQueueState;
      //## end ClLauncherStats::InQueueState%38341651000B.attr

      //## Attribute: ReleaseState%38342A36019F
      //## begin ClLauncherStats::ReleaseState%38342A36019F.attr preserve=no  public: static m4pchar_t {VAC} "Preparing output request"
      static const m4pchar_t  ReleaseState;
      //## end ClLauncherStats::ReleaseState%38342A36019F.attr

      //## Attribute: WaitState%383434AD001B
      //## begin ClLauncherStats::WaitState%383434AD001B.attr preserve=no  public: static m4pchar_t {UAC} "Waiting for Input"
      static const m4pchar_t  WaitState;
      //## end ClLauncherStats::WaitState%383434AD001B.attr

      //## Attribute: ExecuteState%383434AD0057
      //## begin ClLauncherStats::ExecuteState%383434AD0057.attr preserve=no  public: static m4pchar_t {UAC} "Executing"
      static const m4pchar_t  ExecuteState;
      //## end ClLauncherStats::ExecuteState%383434AD0057.attr

      //## Attribute: FinishedState%3857896B031F
      //## begin ClLauncherStats::FinishedState%3857896B031F.attr preserve=no  public: static m4pchar_t {UAC} "Finished Execution"
      static const m4pchar_t  FinishedState;
      //## end ClLauncherStats::FinishedState%3857896B031F.attr

    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%383434D600F6
      //## Role: ClLauncherStats::m_oStateRecord%383434D600F8
      //## begin ClLauncherStats::m_oStateRecord%383434D600F8.role preserve=no  public: ClStateRecord { -> 1VHAN}
      ClStateRecord m_oStateRecord;
      //## end ClLauncherStats::m_oStateRecord%383434D600F8.role

  public:
    // Additional Public Declarations
      //## begin ClLauncherStats%346220B60258.public preserve=yes
      //## end ClLauncherStats%346220B60258.public

  protected:
    // Data Members for Associations

      //## Association: M4Executive::<unnamed>%37FC4FE70247
      //## begin ClLauncherStats::nUserAbortedRequest%37FC4FE80158.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nUserAbortedRequest;
      //## end ClLauncherStats::nUserAbortedRequest%37FC4FE80158.role

      //## Association: M4Executive::<unnamed>%37FC4F810309
      //## begin ClLauncherStats::nErrorRequest%37FC4F8201F2.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nErrorRequest;
      //## end ClLauncherStats::nErrorRequest%37FC4F8201F2.role

      //## Association: M4Executive::<unnamed>%37FC500D022E
      //## begin ClLauncherStats::nAdminAbortedRequest%37FC500E0104.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nAdminAbortedRequest;
      //## end ClLauncherStats::nAdminAbortedRequest%37FC500E0104.role

      //## Association: M4Executive::<unnamed>%37FC50410052
      //## begin ClLauncherStats::nSuccessRequest%37FC5041032E.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nSuccessRequest;
      //## end ClLauncherStats::nSuccessRequest%37FC5041032E.role

      //## Association: M4Executive::<unnamed>%37946C34015B
      //## begin ClLauncherStats::nTimedOutRequest%37946C340382.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nTimedOutRequest;
      //## end ClLauncherStats::nTimedOutRequest%37946C340382.role

    // Additional Protected Declarations
      //## begin ClLauncherStats%346220B60258.protected preserve=yes
      //## end ClLauncherStats%346220B60258.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherStats%346220B60258.private preserve=yes
      //## end ClLauncherStats%346220B60258.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherStats%346220B60258.implementation preserve=yes
      //## end ClLauncherStats%346220B60258.implementation

};

//## begin ClLauncherStats%346220B60258.postscript preserve=yes
//## end ClLauncherStats%346220B60258.postscript

//## begin ClLauncherVisitor%344B7EA1000A.preface preserve=yes
//## end ClLauncherVisitor%344B7EA1000A.preface

//## Class: ClLauncherVisitor%344B7EA1000A
//	This class provides a public interface for accessing statistical information
//	from ClLauncher.
//	This approach is based on the visitor behavioral pattern as described in
//	[GAM].
//## Category: M4Executive%35F65F3D007A
//## Subsystem: M4Executive::inc%379462FF024C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%356D2624003F;ClLauncherInterface { -> }
//## Uses: <unnamed>%34622AC2021A;ClLauncherStats { -> }

class M4_DECL_M4EXECUTIVE ClLauncherVisitor : public ClActiveClassVisitor  //## Inherits: <unnamed>%344B7EB602C9
{
  //## begin ClLauncherVisitor%344B7EA1000A.initialDeclarations preserve=yes
  //## end ClLauncherVisitor%344B7EA1000A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLauncherVisitor%879608295; C++
      //	Constructor for class "ClLauncherVisitor".
      ClLauncherVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitLauncher%877356014; C++
      //	This virtual operation accesses ClCSACtiveClass information from the
      //	instance provided through the ai_oActiveClass argument.
      //	Remarks
      //	· ProcessedRequests (CR): número total de peticiones procesadas.
      //	· DiscardedRequests (CR): número total de peticiones descartadas porque la
      //	conexión se ha cerrado. Este contador y el anterior se ponen a cero a la vez.
      //	· AvgProcessedRequests (SC): número medio de peticiones procesadas por
      //	segundo durante los últimos 60, 300 y 900 segundos. Se actualiza cada 5
      //	segundos.
      //	· VirtualMachineCount (M): número de máquinas virtuales manejadas por el
      //	Executor (para peticiones con estado). Si se incrementa este parámetro, el
      //	Executor crea nuevas máquinas virtuales sin más. Si se decrementa, el
      //	Executor marca para su eliminación las máquinas virtuales necesarias. A
      //	partir de ese momento, esas máquinas ya no cogerán nuevas peticiones, pero
      //	sí terminarán con las operaciones con estado que tuvieran a medio hacer. Si
      //	se incrementa el parámetro y hay máquinas marcadas para la eliminación, se
      //	desmarcan tantas máquinas como se pueda antes de crear nuevas máquinas.
      //	· Alguna otra información que haga referencia al SGBD que está utilizando y
      //	cosas así. A definir por Meta4
      virtual void VisitLauncher (ClLauncherInterface &ai_oLauncher);

    // Additional Public Declarations
      //## begin ClLauncherVisitor%344B7EA1000A.public preserve=yes
      //## end ClLauncherVisitor%344B7EA1000A.public

  protected:
    // Additional Protected Declarations
      //## begin ClLauncherVisitor%344B7EA1000A.protected preserve=yes
      //## end ClLauncherVisitor%344B7EA1000A.protected

  private:
    // Additional Private Declarations
      //## begin ClLauncherVisitor%344B7EA1000A.private preserve=yes
      //## end ClLauncherVisitor%344B7EA1000A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLauncherVisitor%344B7EA1000A.implementation preserve=yes
      //## end ClLauncherVisitor%344B7EA1000A.implementation

};

//## begin ClLauncherVisitor%344B7EA1000A.postscript preserve=yes
//## end ClLauncherVisitor%344B7EA1000A.postscript

// Class ClLauncherInterface 


//## Other Operations (inline)
inline void ClLauncherInterface::Accept (ClLauncherVisitor &ai_oStats)
{
  //## begin ClLauncherInterface::Accept%877356013.body preserve=yes
	ai_oStats.VisitLauncher(*this);
  //## end ClLauncherInterface::Accept%877356013.body
}

inline m4return_t ClLauncherInterface::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClLauncherInterface::ExecuteCommand%922189093.body preserve=yes
	return M4_ERROR;
  //## end ClLauncherInterface::ExecuteCommand%922189093.body
}

inline void ClLauncherInterface::SetSSInvoker (ClSSInvokerAbstract *ai_poInvoker)
{
  //## begin ClLauncherInterface::SetSSInvoker%994342569.body preserve=yes
	m_poSSInvoker = ai_poInvoker ;
  //## end ClLauncherInterface::SetSSInvoker%994342569.body
}

// Class ClLauncherStats 

//## Get and Set Operations for Associations (inline)

inline ClStatNumericalType& ClLauncherStats::GetnUserAbortedRequest ()
{
  //## begin ClLauncherStats::GetnUserAbortedRequest%37FC4FE80158.get preserve=no
  return m_nUserAbortedRequest;
  //## end ClLauncherStats::GetnUserAbortedRequest%37FC4FE80158.get
}

inline void ClLauncherStats::AddnUserAbortedRequest (const ClStatNumericalType& value)
{
  //## begin ClLauncherStats::AddnUserAbortedRequest%37FC4FE80158.set preserve=yes
  m_nUserAbortedRequest += value;
  //## end ClLauncherStats::AddnUserAbortedRequest%37FC4FE80158.set
}

inline ClStatNumericalType& ClLauncherStats::GetnErrorRequest ()
{
  //## begin ClLauncherStats::GetnErrorRequest%37FC4F8201F2.get preserve=no
  return m_nErrorRequest;
  //## end ClLauncherStats::GetnErrorRequest%37FC4F8201F2.get
}

inline void ClLauncherStats::AddnErrorRequest (const ClStatNumericalType& value)
{
  //## begin ClLauncherStats::AddnErrorRequest%37FC4F8201F2.set preserve=yes
  m_nErrorRequest += value;
  //## end ClLauncherStats::AddnErrorRequest%37FC4F8201F2.set
}

inline ClStatNumericalType& ClLauncherStats::GetnAdminAbortedRequest ()
{
  //## begin ClLauncherStats::GetnAdminAbortedRequest%37FC500E0104.get preserve=no
  return m_nAdminAbortedRequest;
  //## end ClLauncherStats::GetnAdminAbortedRequest%37FC500E0104.get
}

inline void ClLauncherStats::AddnAdminAbortedRequest (const ClStatNumericalType& value)
{
  //## begin ClLauncherStats::AddnAdminAbortedRequest%37FC500E0104.set preserve=yes
  m_nAdminAbortedRequest += value;
  //## end ClLauncherStats::AddnAdminAbortedRequest%37FC500E0104.set
}

inline ClStatNumericalType& ClLauncherStats::GetnSuccessRequest ()
{
  //## begin ClLauncherStats::GetnSuccessRequest%37FC5041032E.get preserve=no
  return m_nSuccessRequest;
  //## end ClLauncherStats::GetnSuccessRequest%37FC5041032E.get
}

inline void ClLauncherStats::AddnSuccessRequest (const ClStatNumericalType& value)
{
  //## begin ClLauncherStats::AddnSuccessRequest%37FC5041032E.set preserve=yes
//	AddnProcessedItems(value);
	m_nSuccessRequest += value;
  //## end ClLauncherStats::AddnSuccessRequest%37FC5041032E.set
}

inline ClStatNumericalType& ClLauncherStats::GetnTimedOutRequest ()
{
  //## begin ClLauncherStats::GetnTimedOutRequest%37946C340382.get preserve=no
  return m_nTimedOutRequest;
  //## end ClLauncherStats::GetnTimedOutRequest%37946C340382.get
}

inline void ClLauncherStats::AddnTimedOutRequest (const ClStatNumericalType& value)
{
  //## begin ClLauncherStats::AddnTimedOutRequest%37946C340382.set preserve=yes
  m_nTimedOutRequest += value;
  //## end ClLauncherStats::AddnTimedOutRequest%37946C340382.set
}

// Class ClLauncherVisitor 

//## begin module%343E2BE001AE.epilog preserve=yes
//## end module%343E2BE001AE.epilog


#endif
