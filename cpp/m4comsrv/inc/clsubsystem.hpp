//## begin module%3492936003D4.cm preserve=no
//## end module%3492936003D4.cm

//## begin module%3492936003D4.cp preserve=no
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
//## end module%3492936003D4.cp

//## Module: ClSubsystem%3492936003D4; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clsubsystem.hpp

#ifndef clsubsystem_h
#define clsubsystem_h 1

//## begin module%3492936003D4.additionalIncludes preserve=no
#include <m4comsrv_dll.hpp>
#include <m4condition.hpp>
#include <statistics.hpp>
#include <cladmintag.hpp>
#include <systeminteface.hpp>
#include <clssimplementacion.hpp>

class ClServEnv;
class ClCSSessManager;
class ClUSServiceSS;
class ClNavigatorSS;
class ClSystemVisitor;
class ClSubsystem ;
class ClCryptStore ;





//## end module%3492936003D4.additionalDeclarations


//## begin ClSystem%348D6FE90173.preface preserve=yes
//## end ClSystem%348D6FE90173.preface

//## Class: ClSystem%348D6FE90173
//	This is the root node of the subsystems tree. All sended request from
//	administrator threads are procesed in first instance by this
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%356962A3033F;ClNavigatorSS { -> F}
//## Uses: <unnamed>%356AFB83014F;ClServEnv { -> F}
//## Uses: <unnamed>%356AFBD801DE;ClUSServiceSS { -> F}
//## Uses: <unnamed>%356AFBF102E8;ClUSServiceAPI { -> F}
//## Uses: <unnamed>%358631980349;ClCSSessManager { -> F}
//## Uses: <unnamed>%35B31A540279;ClAdminTagList { -> }
//## Uses: <unnamed>%36A496F3023A;ClSessionSS { -> F}
//## Uses: <unnamed>%36A49A1D0217;ClUSSession { -> F}
//## Uses: <unnamed>%36D67FA9025A;ClSystemVisitor { -> F}

class M4_DECL_M4COMSRV ClSystem : public ClSystemInterface, //## Inherits: <unnamed>%369CDCED011E
                             	public ClSSImplementation  //## Inherits: <unnamed>%39212F11023B
{
  //## begin ClSystem%348D6FE90173.initialDeclarations preserve=yes
  //## end ClSystem%348D6FE90173.initialDeclarations

  public:
    //## begin ClSystem::eSystemCommands%37A962FD0379.preface preserve=yes
    //## end ClSystem::eSystemCommands%37A962FD0379.preface

    //## Class: eSystemCommands%37A962FD0379
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { SYNCHRONIZE_CALL = ClSSImplementation::TOTAL_COMMANDS, GARBAGE_COLLECTION , ESTIMATE_LOAD , TOTAL_COMMANDS} eSystemCommands;

    //## begin ClSystem::eSystemCommands%37A962FD0379.postscript preserve=yes
    //## end ClSystem::eSystemCommands%37A962FD0379.postscript

    //## Destructor (specified)
      //## Operation: ~ClSystem%882023702
      //	Destructor
      ~ClSystem ();


    //## Other Operations (specified)
      //## Operation: Accept%884777573
      void Accept (ClSystemVisitor &ai_oVisitor);

      //## Operation: Synchronize%884942686
      //	Makes the shynchronize tasks for the system object
      m4return_t Synchronize ();

      //## Operation: Initialize%885806506
      //	Initializes the system, for example this method will initialize the literal
      //	and properties tables for introspective calls.
      m4return_t Initialize ();

      //## Operation: ShutDown%886498837
      //	Shouts down the server
      m4return_t ShutDown ();

      //## Operation: ExecuteCommandRoot%889700967
      //	This method in this class only dispach the same method to the apropiate
      //	subsystem after looking for it, of course only if the serching is
      //	successfull.
      m4return_t ExecuteCommandRoot (m4pchar_t ai_szPath, 	// The path to the node that it wanted to process the final function. The path
      	// is compossed by all the identifiers of the subsystems witch are in the tree
      	// path to the wanted subsystem, separated all of then by point and finished
      	// by point and coma.
      m4char_t ai_cCommand, M4DataStorage *ai_poDataParams, M4DataStorage *ao_poDataOut);

      //## Operation: RemoveSubsystem%889784166
      //	This method in this class only dispach the same method to the apropiate
      //	subsystem after looking for it, of course only if the serching is
      //	successfull.
      m4return_t RemoveSubsystem (m4objid_t ai_lIdBase, 	// That is the subsystem id from witch the subsystem is going to be removed.
      	// First of all the system does a finding of that subsystem and then call its
      	// own RemoveSubsystem method
      m4objid_t ai_lSubsystemId);

      //## Operation: Instance%895825798
      static ClSystem * Instance ();

      //## Operation: StartUp%895825799
      m4return_t StartUp (ClCondition *ai_poCondition, 	// The accesses to the subsystems trees is controled in the first level that
      	// is the system class, because of that that class has a semaphore to allow
      	// only one acess at the same moment.
      m4pchar_t ai_szConfigPath, 	// On construction the path to the file containing the initial configutaion is
      	// passed to the system object. That file is suposed to be an obl file, but
      	// the argument is only an string with the path.
      m4pchar_t ai_szUser, m4pchar_t ai_szPassword, ClCryptStore *ai_poCryptStore, m4pchar_t ai_szPropertiesFile, m4pchar_t ai_szLiterals, m4pchar_t ai_szTables, m4pchar_t ai_szLogSys, m4pchar_t ai_szProcessName, m4pchar_t ai_szConfigName, m4uint32_t ai_uControlRetryTime, m4uint32_t ai_iDispatcherPort, m4pchar_t ai_szDispatcherHost, m4uint32_t ai_iBasePort);

      //## Operation: LogOn%896170609
      m4return_t LogOn ();

      //## Operation: GetCommandParamsRoot%901005786
      m4return_t GetCommandParamsRoot (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: ExecuteCommand%901005793
      m4return_t ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: PersistConfiguration%901740423
      m4return_t PersistConfiguration (m4pchar_t ai_szProperty, m4pchar_t ai_szValue, UINT ai_hBlock = 0);

      //## Operation: ExecuteCommand%910862269
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%910862270
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: GetLiteral%916246420
      m4pchar_t GetLiteral (m4pchar_t ai_tag);

      //## Operation: GetStringSpecificationValue%916246421
      m4return_t GetStringSpecificationValue (char *ai_szpath, char *ai_szProperty, char *ao_szValue, size_t ai_iLenBuffer, size_t &ao_iLenOut);

      //## Operation: GetId%916246422
      //	Returns the subsystem id
      m4objid_t GetId ();

      //## Operation: GenerateSubsystem%919968179
      ClSSInterfaz * GenerateSubsystem (m4uint16_t ai_IType, ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL);

      //## Operation: Update%919968182
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: GetSubsystemsId%920285555
      //	This method in this class only dispach the same method to the apropiate
      //	subsystem after looking for it, of course only if the serching is
      //	successfull.
      m4return_t GetSubsystemsId (m4pchar_t ai_szPath, 	// The path to the node that it wanted to process the final function. The path
      	// is compossed by all the identifiers of the subsystems witch are in the tree
      	// path to the wanted subsystem, separated all of then by point and finished
      	// by point and coma.
      vector <m4objid_t > &ao_poTags);

      //## Operation: StopExecution%929965060
      m4bool_t StopExecution ();

      //## Operation: Exit%929965061
      m4return_t Exit (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: GetCommandsId%933847946
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GarbageCollection%936257855
      void GarbageCollection ();

      //## Operation: SetType%936613843
      //	Sets the subsystem type by an integer parameter.
      void SetType (m4uint16_t ai_iType);

      //## Operation: CheckSystemMemory%937580167
      m4return_t CheckSystemMemory (m4uint32_t ai_lPrevRefreshRatio, m4uint32_t ao_lPosRefreshRatio);

      //## Operation: EstimateLoad%962873113
      m4return_t EstimateLoad (ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsIdRoot%994763185
      m4return_t GetCommandsIdRoot (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList);

      //## Operation: GetSubsystemAttribValue%994933976
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      m4return_t GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribsId%994933977
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      m4return_t GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

      //## Operation: GetStatisticAttribValue%994933978
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      m4return_t GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetStatisticAttribsId%994933979
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      m4return_t GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

  public:
    // Additional Public Declarations
      //## begin ClSystem%348D6FE90173.public preserve=yes

      //## end ClSystem%348D6FE90173.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%36D5A7B70044
      //## Role: ClSystem::m_poSystem%36D5A7BC0024
      //## begin ClSystem::m_poSystem%36D5A7BC0024.role preserve=no  protected: static ClSystem { -> RHAN}
      static ClSystem *m_poSystem;
      //## end ClSystem::m_poSystem%36D5A7BC0024.role

    // Additional Protected Declarations
      //## begin ClSystem%348D6FE90173.protected preserve=yes
      //## end ClSystem%348D6FE90173.protected

  private:
    //## Constructors (specified)
      //## Operation: ClSystem%882023701
      //	Constructor
      ClSystem ();

    // Data Members for Class Attributes

      //## Attribute: m_szConfigPath%34E40D2D02C3
      //	On construction the path to the file containing the initial configutaion is
      //	passed to the system object. That file is suposed to be an obl file, but the
      //	argument is only an string with the path.
      //## begin ClSystem::m_szConfigPath%34E40D2D02C3.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szConfigPath;
      //## end ClSystem::m_szConfigPath%34E40D2D02C3.attr

      //## Attribute: m_szUserName%356ADA3C0376
      //## begin ClSystem::m_szUserName%356ADA3C0376.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szUserName;
      //## end ClSystem::m_szUserName%356ADA3C0376.attr

      //## Attribute: m_szPassword%356ADA7B0042
      //## begin ClSystem::m_szPassword%356ADA7B0042.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szPassword;
      //## end ClSystem::m_szPassword%356ADA7B0042.attr

      //## Attribute: m_bServerStarted%376E221A0017
      //## begin ClSystem::m_bServerStarted%376E221A0017.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bServerStarted;
      //## end ClSystem::m_bServerStarted%376E221A0017.attr

      //## Attribute: m_bStopExecution%376E2312035D
      //## begin ClSystem::m_bStopExecution%376E2312035D.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bStopExecution;
      //## end ClSystem::m_bStopExecution%376E2312035D.attr

      //## Attribute: m_lDeferedTime%376E448602AD
      //## begin ClSystem::m_lDeferedTime%376E448602AD.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_lDeferedTime;
      //## end ClSystem::m_lDeferedTime%376E448602AD.attr

      //## Attribute: m_iBasePort%3944ABFD001A
      //## begin ClSystem::m_iBasePort%3944ABFD001A.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iBasePort;
      //## end ClSystem::m_iBasePort%3944ABFD001A.attr

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%384780C7038C
      //## Role: ClSystem::m_poCondition%384780C80211
      //## begin ClSystem::m_poCondition%384780C80211.role preserve=no  private: ClCondition { -> RHAN}
      ClCondition *m_poCondition;
      //## end ClSystem::m_poCondition%384780C80211.role

    // Additional Private Declarations
      //## begin ClSystem%348D6FE90173.private preserve=yes
      //## end ClSystem%348D6FE90173.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSystem%348D6FE90173.implementation preserve=yes
      //## end ClSystem%348D6FE90173.implementation

};

//## begin ClSystem%348D6FE90173.postscript preserve=yes
//## end ClSystem%348D6FE90173.postscript

//## begin ClSystemVisitor%34BDFB4000F8.preface preserve=yes
//## end ClSystemVisitor%34BDFB4000F8.preface

//## Class: ClSystemVisitor%34BDFB4000F8
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BDFBE002F7;ClSystem { -> }

class ClSystemVisitor : public ClVisitor  //## Inherits: <unnamed>%36DEB05E02DA
{
  //## begin ClSystemVisitor%34BDFB4000F8.initialDeclarations preserve=yes
  //## end ClSystemVisitor%34BDFB4000F8.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: VisitSystem%884863683
      void VisitSystem (ClSystem &ai_oSsystem);

    // Additional Public Declarations
      //## begin ClSystemVisitor%34BDFB4000F8.public preserve=yes
      //## end ClSystemVisitor%34BDFB4000F8.public

  protected:
    // Additional Protected Declarations
      //## begin ClSystemVisitor%34BDFB4000F8.protected preserve=yes
      //## end ClSystemVisitor%34BDFB4000F8.protected

  private:
    // Additional Private Declarations
      //## begin ClSystemVisitor%34BDFB4000F8.private preserve=yes
      //## end ClSystemVisitor%34BDFB4000F8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSystemVisitor%34BDFB4000F8.implementation preserve=yes
      //## end ClSystemVisitor%34BDFB4000F8.implementation

};

//## begin ClSystemVisitor%34BDFB4000F8.postscript preserve=yes
class ClSystemStats : public ClStatistic  //## Inherits: <unnamed>%3459D8F503B4
{
  public:



    typedef enum { SYSTEM_MEMORY_USAGE = ClStatistic::TOTAL_NUM , SYSTEM_CPU_USAGE , SERVER_MEMORY , TOTAL_NUM } eSystemStatistics;

	ClSystemStats () ;
      m4return_t Reset ();

      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_oValue);

      m4return_t SetStat (ClStatHandle &ai_hStatId, const m4pchar_t ao_pszValue);

      m4return_t ResetStat (ClStatHandle &ai_hStatId);

      m4void_t SetSystemMemoryUsage (m4double_t ai_NewValue);

      m4void_t SetSystemCPUUsage (m4double_t ai_NewValue);
      
	  m4void_t SetServerMemory (m4double_t ai_NewValue);

	  m4void_t SetPercentServerMemory (m4double_t ai_NewValue);

      m4void_t GetSystemMemoryUsage (m4double_t &ao_NewValue);

      m4void_t GetSystemCPUUsage (m4double_t &ao_NewValue);
      
	  m4void_t GetServerMemory (m4double_t &ao_NewValue);

	  m4void_t GetPercentServerMemory (m4double_t &ao_NewValue);


      
  private:

	  m4double_t m_iServerMemory ;

	  m4double_t m_iSystemMemoryUsage ;
	  
	  m4double_t m_iSystemCPUUsage ;

	  m4double_t m_iPercentServerMemory ;
};

//## end ClSystemVisitor%34BDFB4000F8.postscript

// Class ClSystem 

// Class ClSystemVisitor 

//## begin module%3492936003D4.epilog preserve=yes
//## end module%3492936003D4.epilog


#endif
