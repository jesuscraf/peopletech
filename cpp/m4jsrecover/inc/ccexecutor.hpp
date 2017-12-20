//## begin module%36E625280300.cm preserve=no
//## end module%36E625280300.cm

//## begin module%36E625280300.cp preserve=no
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
//## end module%36E625280300.cp

//## Module: CcExecutor%36E625280300; Package specification
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Source file: D:\Fuentes\m4jsrecover\inc\ccexecutor.hpp

#ifndef ccexecutor_h
#define ccexecutor_h 1

//## begin module%36E625280300.additionalIncludes preserve=no
//## end module%36E625280300.additionalIncludes

//## begin module%36E625280300.includes preserve=yes
#include <m4jsrecover_dll.hpp>
#include <jsjob.hpp>
//## end module%36E625280300.includes

// Executive
#include <executive.hpp>
// m4types
#include <m4types.hpp>
// jsRequest
#include <jsrequest.hpp>
// jsjobbuilderfromm4object
#include <jsjobbuilderfromm4object.hpp>

class ClNavigatorFirewall;
class ClChannelManager;
class ClAccess;
class ClChannel;
class ClM4ObjService;

//## begin module%36E625280300.declarations preserve=no
//## end module%36E625280300.declarations

//## begin module%36E625280300.additionalDeclarations preserve=yes
//## end module%36E625280300.additionalDeclarations


//## begin ClCcExecutor%36E4EA4102BC.preface preserve=yes
//## end ClCcExecutor%36E4EA4102BC.preface

//## Class: ClCcExecutor%36E4EA4102BC
//## Category: M4JSRecover%37F8B5B70134
//## Subsystem: M4JSRecover::inc%37F8B6990107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37EB704002DE;ClMetaDataSubSystem { -> }
//## Uses: <unnamed>%37E201060386;ClJSRequest { -> F}
//## Uses: <unnamed>%37EB6FF501D1;ClSSInterfaz { -> }
//## Uses: <unnamed>%37EB70B10041;IExecutor { -> }
//## Uses: <unnamed>%37EB6F840066;m4return_t { -> }
//## Uses: <unnamed>%36E63BB30359;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%37F8C2090388;ClJSQueue { -> }
//## Uses: <unnamed>%382C0F74015C;ClStatsJSSubsystem { -> }

class M4_DECL_M4JSRECOVER ClCcExecutor : public ClExecutive  //## Inherits: <unnamed>%36E4EAE501F9
{
  //## begin ClCcExecutor%36E4EA4102BC.initialDeclarations preserve=yes
  //## end ClCcExecutor%36E4EA4102BC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCcExecutor%921053001
      ClCcExecutor (ClEngine* ai_pEngine, ClStatistic* ai_pStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCcExecutor%921053002
      ~ClCcExecutor ();


    //## Other Operations (specified)
      //## Operation: StartUp%921053005
      m4return_t StartUp (ClNavigatorFirewall* ai_pNavigator = NULL, ClConfiguration* ai_pConfig = NULL);

      //## Operation: ShutDown%921053006
      m4return_t ShutDown ();

      //## Operation: Execute%921053007
      m4return_t Execute (ClImage& aio_pImage);

      //## Operation: Reset%921053008
      m4return_t Reset ();

      //## Operation: ProccessRequest%937557557
      m4return_t ProccessRequest (ClJSRequest *ai_poJSRequest, ClJSQueue &ao_poJSQueue);

      //## Operation: SetIdentification%940334698
      m4return_t SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName);

      //## Operation: ReleaseResources%942857887
      m4return_t ReleaseResources ();

    // Additional Public Declarations
      //## begin ClCcExecutor%36E4EA4102BC.public preserve=yes
      //## end ClCcExecutor%36E4EA4102BC.public

  protected:

    //## Other Operations (specified)
      //## Operation: LoadChannel%937559766
      m4return_t LoadChannel (m4pchar_t ai_strChannel);

      //## Operation: UnLoadChannel%937559767
      m4return_t UnLoadChannel ();

      //## Operation: CheckCancel%937559768; C++
      m4return_t CheckCancel (ClJSQueue &ai_oJSQueue);

      //## Operation: CheckBoxExpiration%937559769
      m4return_t CheckBoxExpiration ();

      //## Operation: CheckResExpiration%937559770
      m4return_t CheckResExpiration (m4double_t JulianDate);

      //## Operation: CheckScheduledJobs%939977799
      m4return_t CheckScheduledJobs (m4uint_t &ai_uiMaxNumJobs, ClJSQueue &ai_oJSQueue);

      //## Operation: UnmarkExecution%949062689
      m4return_t UnmarkExecution (ClJSJob *ai_pJob);

      //## Operation: SendNewJobRequest%939977803
      m4return_t SendNewJobRequest (ClJSJob *ai_poJob, ClJSQueue &ai_oJSQueue);

      //## Operation: SendEndOfJobsRequest%939977804
      m4return_t SendEndOfJobsRequest (const m4bool_t ai_bSetInmediateRequest, ClJSQueue &ai_oJSQueue);

      //## Operation: SendCancelRequest%941016333
      m4return_t SendCancelRequest (ClJSJob *ai_poJob, ClJSQueue &ai_oJSQueue);

      //## Operation: SetServerName%940334696
      m4return_t SetServerName (const M4ClString &ai_stServerName);

      //## Operation: SetServiceName%940334697
      m4return_t SetServiceName (const M4ClString &ai_stServiceName);

      //## Operation: InitializeRecoveryM4O%940947251
      m4return_t InitializeRecoveryM4O ();

      //## Operation: CleanDatabase%941731803
      m4return_t CleanDatabase (m4uint32_t &ai_uiFlag);

    // Data Members for Class Attributes

      //## Attribute: m_stServerName%380C5CD301FD
      //## begin ClCcExecutor::m_stServerName%380C5CD301FD.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_stServerName;
      //## end ClCcExecutor::m_stServerName%380C5CD301FD.attr

      //## Attribute: m_stServiceName%380C5CEA0106
      //## begin ClCcExecutor::m_stServiceName%380C5CEA0106.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_stServiceName;
      //## end ClCcExecutor::m_stServiceName%380C5CEA0106.attr

      //## Attribute: m_bIsValid%3815B6E00397
      //## begin ClCcExecutor::m_bIsValid%3815B6E00397.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bIsValid;
      //## end ClCcExecutor::m_bIsValid%3815B6E00397.attr

      //## Attribute: m_bIsInitialized%381F164301A7
      //## begin ClCcExecutor::m_bIsInitialized%381F164301A7.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bIsInitialized;
      //## end ClCcExecutor::m_bIsInitialized%381F164301A7.attr

    // Data Members for Associations

      //## Association: M4JSRecover::<unnamed>%380B2679026D
      //## Role: ClCcExecutor::m_poBuilder%380B267A011A
      //## begin ClCcExecutor::m_poBuilder%380B267A011A.role preserve=no  protected: ClJSJobBuilderFromM4Object { -> RHAN}
      ClJSJobBuilderFromM4Object *m_poBuilder;
      //## end ClCcExecutor::m_poBuilder%380B267A011A.role

      //## Association: M4JSRecover::<unnamed>%38147E4D02F0
      //## Role: ClCcExecutor::m_pClChannelManager%38147E4E00FD
      //## begin ClCcExecutor::m_pClChannelManager%38147E4E00FD.role preserve=no  protected: ClChannelManager { -> RFHAN}
      ClChannelManager *m_pClChannelManager;
      //## end ClCcExecutor::m_pClChannelManager%38147E4E00FD.role

      //## Association: M4JSRecover::<unnamed>%38147EC20186
      //## Role: ClCcExecutor::m_pClChannel%38147EC30105
      //## begin ClCcExecutor::m_pClChannel%38147EC30105.role preserve=no  protected: ClChannel { -> RFHAN}
      ClChannel *m_pClChannel;
      //## end ClCcExecutor::m_pClChannel%38147EC30105.role

      //## Association: M4JSRecover::<unnamed>%38147EF300A0
      //## Role: ClCcExecutor::m_pClAccess%38147EF40098
      //## begin ClCcExecutor::m_pClAccess%38147EF40098.role preserve=no  protected: ClAccess { -> RFHAN}
      ClAccess *m_pClAccess;
      //## end ClCcExecutor::m_pClAccess%38147EF40098.role

      //## Association: M4JSRecover::<unnamed>%381481D10363
      //## Role: ClCcExecutor::m_pRecoveryM4O%381481D2035A
      //## begin ClCcExecutor::m_pRecoveryM4O%381481D2035A.role preserve=no  protected: ClChannel { -> RFHAN}
      ClChannel *m_pRecoveryM4O;
      //## end ClCcExecutor::m_pRecoveryM4O%381481D2035A.role

      //## Association: M4JSRecover::<unnamed>%3814832702B0
      //## Role: ClCcExecutor::m_pRecoveryM4OAccess%381483280239
      //## begin ClCcExecutor::m_pRecoveryM4OAccess%381483280239.role preserve=no  protected: ClAccess { -> RFHAN}
      ClAccess *m_pRecoveryM4OAccess;
      //## end ClCcExecutor::m_pRecoveryM4OAccess%381483280239.role

      //## Association: M4JSRecover::<unnamed>%38F1C039032C
      //## Role: ClCcExecutor::m_poObjService%38F1C03A00FC
      //## begin ClCcExecutor::m_poObjService%38F1C03A00FC.role preserve=no  protected: ClM4ObjService { -> RFHAN}
      ClM4ObjService *m_poObjService;
      //## end ClCcExecutor::m_poObjService%38F1C03A00FC.role

    // Additional Protected Declarations
      //## begin ClCcExecutor%36E4EA4102BC.protected preserve=yes
      //## end ClCcExecutor%36E4EA4102BC.protected

  private:
    // Additional Private Declarations
      //## begin ClCcExecutor%36E4EA4102BC.private preserve=yes
      //## end ClCcExecutor%36E4EA4102BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCcExecutor%36E4EA4102BC.implementation preserve=yes
      //## end ClCcExecutor%36E4EA4102BC.implementation

};

//## begin ClCcExecutor%36E4EA4102BC.postscript preserve=yes
//## end ClCcExecutor%36E4EA4102BC.postscript

// Class ClCcExecutor 

//## begin module%36E625280300.epilog preserve=yes
//## end module%36E625280300.epilog


#endif
