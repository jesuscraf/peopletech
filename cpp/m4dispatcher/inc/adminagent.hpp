//## begin module%396B5C630259.cm preserve=no
//## end module%396B5C630259.cm

//## begin module%396B5C630259.cp preserve=no
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
//## end module%396B5C630259.cp

//## Module: adminagent%396B5C630259; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\adminagent.hpp

#ifndef adminagent_h
#define adminagent_h 1

//## begin module%396B5C630259.additionalIncludes preserve=no
//## end module%396B5C630259.additionalIncludes

//## begin module%396B5C630259.includes preserve=yes
//## end module%396B5C630259.includes

#include "m4stl.hpp"
// cltimer
#include <cltimer.hpp>
// dspagent
#include <dspagent.hpp>
// m4condition
#include <m4condition.hpp>
// appctlnodeinfo
#include <appctlnodeinfo.hpp>
//## begin module%396B5C630259.declarations preserve=no
//## end module%396B5C630259.declarations

//## begin module%396B5C630259.additionalDeclarations preserve=yes
//## end module%396B5C630259.additionalDeclarations


//## begin ClAdminAgent%3947972F0207.preface preserve=yes
//## end ClAdminAgent%3947972F0207.preface

//## Class: ClAdminAgent%3947972F0207
//## Category: M4Dispatcher::Agents%39F451CA0292
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3948B1B403E3;ClSAXHandlerActionCaller { -> F}
class ClAdminAgent : public ClDspAgent  //## Inherits: <unnamed>%396B59B201C2
{
  //## begin ClAdminAgent%3947972F0207.initialDeclarations preserve=yes
  //## end ClAdminAgent%3947972F0207.initialDeclarations

  public:
    //## begin ClAdminAgent::eAdminAgentActions%3948AB5F0355.preface preserve=yes
    //## end ClAdminAgent::eAdminAgentActions%3948AB5F0355.preface

    //## Class: eAdminAgentActions%3948AB5F0355
    //## Category: M4Dispatcher::Agents%39F451CA0292
    //## Subsystem: M4Dispatcher::inc%3923BBF7032C
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

	  typedef enum {  PARSE_XML_ID =  ClDspAgent::TOTAL_ACTION , CONFIGURATIONS_QUERY_ACTION_ID , START_SERVER_QUERY_ACTION_ID , STOP_SERVER_QUERY_ACTION_ID ,  CHECK_CONSISTENCY_ACTION_ID , START_INITIAL_CONFIG_ACTION_ID , REGISTER_APPCTL_ACTION_ID, STOP_DPCH_ACTION_ID , SIGNAL_DPCH_ID , REGISTER_TARGET_CONFIG_ID , UPDATE_APPCTL_CONFIG_ID, REMOVE_SERVER_QUERY_ACTION_ID, CLOSE_CONNECTION_ADMIN_ID	 } eAdminAgentActions;

    //## begin ClAdminAgent::eAdminAgentActions%3948AB5F0355.postscript preserve=yes
    //## end ClAdminAgent::eAdminAgentActions%3948AB5F0355.postscript

    //## Constructors (specified)
      //## Operation: ClAdminAgent%960967021
      ClAdminAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent);

    //## Destructor (specified)
      //## Operation: ~ClAdminAgent%960967022
      ~ClAdminAgent ();


    //## Other Operations (specified)
      //## Operation: ProcessAction%960967038
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: Start%960967039
      m4return_t Start ();

      //## Operation: GetConfigurationList%961054359
      m4return_t GetConfigurationList (ClSiteNodeContainer &ao_poConfigurationList);

      //## Operation: StartConfiguration%962122458
      m4return_t StartConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo);

      //## Operation: StopConfiguration%962122459
      m4return_t StopConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo);

	  m4return_t RemoveConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo);

      //## Operation: UpdateConfiguration%962811789
      m4return_t UpdateConfiguration (m4string_t ai_szConfigId, m4string_t ai_strHost, m4uint32_t ai_iBasePort, m4string_t ai_strUserName, m4string_t ai_strPassword, m4string_t ai_szState, m4double_t ai_dLoad);

      //## Operation: CheckConsistency%963302393
      void CheckConsistency ();

      //## Operation: StartHaltedNodes%969528444
      m4return_t StartHaltedNodes (ClSiteNodeContainer &ai_oSiteNodes);

      //## Operation: InitializeTargetConfigurations%972463871
      m4return_t InitializeTargetConfigurations ();

      //## Operation: ReadTargetFile%972463872
      m4return_t ReadTargetFile (m4pchar_t ai_szFileName, m4pchar_t &ao_pBuffer, m4uint32_t &ao_iBufferLen);

      //## Operation: WriteTargetFile%972463873
      m4return_t WriteTargetFile (m4pchar_t ai_szFileName, m4pchar_t ai_pBuffer, m4uint32_t ai_iBufferLen);

      //## Operation: PersistTarget%972463874
      m4return_t PersistTarget ();

      //## Operation: InitializeAppctlRepository%972463883
      m4return_t InitializeAppctlRepository (m4pchar_t ai_pszDspConfigPath = 0);

      //## Operation: AddAppctlInfo%972463884
      m4return_t AddAppctlInfo (ClParamList *ai_poParamList);

      //## Operation: Terminate%973255784
      m4return_t Terminate ();

      //## Operation: GetDspConfigPath%976007080
      static m4pchar_t GetDspConfigPath ();

      //## Operation: ComposeDspConfigPath%976007081
      static m4pchar_t ComposeDspConfigPath (m4pchar_t ai_psAppCtlExePath = NULL);

      //## Operation: GetDspTargetPath%976007082
      m4pchar_t GetDspTargetPath ();

      //## Operation: ComposeDspTargetPath%976007083
      m4pchar_t ComposeDspTargetPath (m4pchar_t ai_psDspExePath = NULL);

      //## Operation: RegisterTargetNode%991920579
      m4return_t RegisterTargetNode (ClParamList *ai_poParamList);

	  // eh 165963
      static m4pchar_t GetDspAdminHostsConfigPath ();
      static m4pchar_t ComposeDspAdminHostsConfigPath (m4pchar_t ai_pcExePath = NULL);

	  // for authentication
	  m4bool_t CheckHaveToAuthenticate();
	  m4bool_t CheckAuthConnection(m4uint32_t ai_uiConnectionId, m4pcchar_t ai_pccToken);
	  m4void_t ReleaseConnection(m4uint32_t ai_uiConnectionId);

	  m4bool_t IsInitializing();

  public:
    // Additional Public Declarations
      //## begin ClAdminAgent%3947972F0207.public preserve=yes
      //## end ClAdminAgent%3947972F0207.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::Agents::<unnamed>%39F71D9A0262
      //## Role: ClAdminAgent::m_oAppctlRepository%39F71D9B00B5
      //## begin ClAdminAgent::m_oAppctlRepository%39F71D9B00B5.role preserve=no  protected: ClAppctlNodeRepository { -> VHAN}
      ClAppctlNodeRepository m_oAppctlRepository;
      //## end ClAdminAgent::m_oAppctlRepository%39F71D9B00B5.role

    // Additional Protected Declarations
      //## begin ClAdminAgent%3947972F0207.protected preserve=yes
	  m4pchar_t m_pcDspConfigDirPath;
	  m4pchar_t m_pcAuthPass;
      //## end ClAdminAgent%3947972F0207.protected

	  m4pchar_t GetDspConfigDirPath();
	  m4void_t LoadPasswordFromFile();
	  m4bool_t m_bHasAuthentication;
	  m4bool_t m_bIsInitializing;

  private:
    // Data Members for Class Attributes

	  typedef set<  m4uint32_t, less< const m4uint32_t> > AuthConnections_t;
	  typedef AuthConnections_t::iterator AuthConnectionsIt_t;

	  AuthConnections_t m_oAuthConnections;

      //## Attribute: m_bStarting%39F6F68D02C1
      //## begin ClAdminAgent::m_bStarting%39F6F68D02C1.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bStarting;
      //## end ClAdminAgent::m_bStarting%39F6F68D02C1.attr

      //## Attribute: m_psTargetFile%3A2FA6D60099
      //## begin ClAdminAgent::m_psTargetFile%3A2FA6D60099.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_psTargetFile;
      //## end ClAdminAgent::m_psTargetFile%3A2FA6D60099.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%3949E8A80334
      //## Role: ClAdminAgent::m_oStopper%3949E8A90191
      //## begin ClAdminAgent::m_oStopper%3949E8A90191.role preserve=no  private: ClCondition { -> VHAN}
      ClCondition m_oStopper;
      //## end ClAdminAgent::m_oStopper%3949E8A90191.role

      //## Association: M4Dispatcher::<unnamed>%396AE60C012A
      //## Role: ClAdminAgent::m_poTimer%396AE60C035B
      //## begin ClAdminAgent::m_poTimer%396AE60C035B.role preserve=no  private: ClTimer { -> RHAN}
      ClTimer *m_poTimer;
      //## end ClAdminAgent::m_poTimer%396AE60C035B.role

      //## Association: M4Dispatcher::Agents::<unnamed>%39FEFBF503CA
      //## Role: ClAdminAgent::m_poDpchStopper%39FEFBF60368
      //## begin ClAdminAgent::m_poDpchStopper%39FEFBF60368.role preserve=no  private: ClCondition { -> RHAN}
      ClCondition *m_poDpchStopper;
      //## end ClAdminAgent::m_poDpchStopper%39FEFBF60368.role

    // Additional Private Declarations
      //## begin ClAdminAgent%3947972F0207.private preserve=yes
      //## end ClAdminAgent%3947972F0207.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAdminAgent%3947972F0207.implementation preserve=yes
      //## end ClAdminAgent%3947972F0207.implementation

};

//## begin ClAdminAgent%3947972F0207.postscript preserve=yes
//## end ClAdminAgent%3947972F0207.postscript

// Class ClAdminAgent 

//## begin module%396B5C630259.epilog preserve=yes
//## end module%396B5C630259.epilog


#endif
