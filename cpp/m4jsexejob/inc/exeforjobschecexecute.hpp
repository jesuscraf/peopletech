//## begin module%34E2EB8200BA.cm preserve=no
//## end module%34E2EB8200BA.cm

//## begin module%34E2EB8200BA.cp preserve=no
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
//## end module%34E2EB8200BA.cp

//## Module: ExeForJobSchecEXECUTE%34E2EB8200BA; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: e:\m4server\m4jsexejob\inc\exeforjobschecexecute.hpp

#ifndef exeforjobschecexecute_h
#define exeforjobschecexecute_h 1

//## begin module%34E2EB8200BA.additionalIncludes preserve=yes
#include <m4jsexejob_dll.hpp>
#define  JS_EXEC_DUMP_LEVEL			4
//## end module%34E2EB8200BA.additionalIncludes

// Executive
#include <executive.hpp>

class ClNavigatorFirewall;
class ClAccess;
class ClChannel;
class ClJSExeCommand;
class ClSecurity;
class ClJSChannelHandler;
class ClJSParametersHandlers;
class ClJSJob;
class ExecDumpInfo;

//## begin module%34E2EB8200BA.declarations preserve=no
//## end module%34E2EB8200BA.declarations

//## begin module%34E2EB8200BA.additionalDeclarations preserve=yes
class ClLogonInterface;
//## end module%34E2EB8200BA.additionalDeclarations


//## begin ClExeForJobSchecEXECUTE%34E2D14402D6.preface preserve=yes
//## end ClExeForJobSchecEXECUTE%34E2D14402D6.preface

//## Class: ClExeForJobSchecEXECUTE%34E2D14402D6
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35A33C7700F8;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%3781B15C0030;ClMetaDataSubsystem { -> F}
//## Uses: <unnamed>%35ACCE7502C9; { -> F}
//## Uses: <unnamed>%37FC6916029F;ClJSJob { -> F}
//## Uses: <unnamed>%380F456201A5;ClMetaDataSubsystem { -> F}
//## Uses: <unnamed>%381963CC0163;ClSecurity30 { -> F}

class M4_DECL_M4JSEXEJOB ClExeForJobSchecEXECUTE : public ClExecutive  //## Inherits: <unnamed>%34E2D1BD0078
{
  //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.initialDeclarations preserve=yes
  //## end ClExeForJobSchecEXECUTE%34E2D14402D6.initialDeclarations

  public:
    //## begin ClExeForJobSchecEXECUTE::eCommandJSExecute%37D68760003D.preface preserve=yes
    //## end ClExeForJobSchecEXECUTE::eCommandJSExecute%37D68760003D.preface

    //## Class: eCommandJSExecute%37D68760003D
    //## Category: M4JSExeJob%37F8BA0B0245
    //## Subsystem: M4JSExeJob%37F8BA260316
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum{CANCEL=ClExecutive::TOTAL_COMMANDS,SETIMEOUT,DELETE_TIMEOUT,TOTAL_COMMANDS} eCommandJSExecute;

    //## begin ClExeForJobSchecEXECUTE::eCommandJSExecute%37D68760003D.postscript preserve=yes
    //## end ClExeForJobSchecEXECUTE::eCommandJSExecute%37D68760003D.postscript

    //## Constructors (specified)
      //## Operation: ClExeForJobSchecEXECUTE%887287313
      ClExeForJobSchecEXECUTE (ClEngine *ai_pEngine, ClStatistic *ai_poStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClExeForJobSchecEXECUTE%887389853
      virtual ~ClExeForJobSchecEXECUTE ();


    //## Other Operations (specified)
      //## Operation: StartUp%887287315
      virtual m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Reset%887287317
      m4return_t Reset ();

      //## Operation: ShutDown%887287324
      virtual m4return_t  ShutDown ();

      //## Operation: GetNDumpLevels%921585527
      m4int16_t GetNDumpLevels () const;

      //## Operation: ExecuteCommand%936777418
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Execute%939286200
      m4return_t Execute (ClJSJob *ai_poJSJob);

      //## Operation: BuildMeta4Objects%940836428
      m4return_t BuildMeta4Objects (M4ClString ai_oNameFirstChannel, M4ClString ai_oNameSecondChannel);

      //## Operation: DestroyMeta4Objects%940836429
      m4return_t DestroyMeta4Objects ();

      //## Operation: SetDumpFile%940836430
      void SetDumpFile (m4pchar_t ai_pcDumpFile);

      //## Operation: GetDumpInfo%940836431
      ExecDumpInfo * GetDumpInfo () const;

      //## Operation: DumpLevel%940836432
      m4return_t DumpLevel (const m4int16_t ai_iLevel);

      //## Operation: ReleaseResources%942231905
      m4return_t ReleaseResources ();

      //## Operation: SetIdentification%942839183
      m4return_t SetIdentification (const M4ClString &ai_stServerName, const M4ClString &ai_stServiceName);

      //## Operation: Execute%945176654
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

  public:
    // Additional Public Declarations
      //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.public preserve=yes
      //## end ClExeForJobSchecEXECUTE%34E2D14402D6.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetLogonInterface%900415011
      m4return_t  GetLogonInterface (ClNavigatorFirewall *ai_pNavigator, ClLogonInterface*  &ai_poLogon);

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%37D6168802FB
      //## Role: ClExeForJobSchecEXECUTE::m_poJSExeCommand%37D616890144
      //## begin ClExeForJobSchecEXECUTE::m_poJSExeCommand%37D616890144.role preserve=no  protected: ClJSExeCommand { -> RFHAN}
      ClJSExeCommand *m_poJSExeCommand;
      //## end ClExeForJobSchecEXECUTE::m_poJSExeCommand%37D616890144.role

      //## Association: M4JSExeJob::<unnamed>%380F46EF039A
      //## Role: ClExeForJobSchecEXECUTE::m_poAccessGJDef%380F46F00143
      //## begin ClExeForJobSchecEXECUTE::m_poAccessGJDef%380F46F00143.role preserve=no  protected: ClAccess { -> RFHAN}
      ClAccess *m_poAccessGJDef;
      //## end ClExeForJobSchecEXECUTE::m_poAccessGJDef%380F46F00143.role

      //## Association: M4JSExeJob::<unnamed>%380F47100149
      //## Role: ClExeForJobSchecEXECUTE::m_poAccessExec%380F47100316
      //## begin ClExeForJobSchecEXECUTE::m_poAccessExec%380F47100316.role preserve=no  protected: ClAccess { -> RFHAN}
      ClAccess *m_poAccessExec;
      //## end ClExeForJobSchecEXECUTE::m_poAccessExec%380F47100316.role

      //## Association: M4JSExeJob::<unnamed>%380F47E90231
      //## Role: ClExeForJobSchecEXECUTE::m_poChannelGJDef%380F47E903AE
      //## begin ClExeForJobSchecEXECUTE::m_poChannelGJDef%380F47E903AE.role preserve=no  protected: ClChannel { -> RFHAN}
      ClChannel *m_poChannelGJDef;
      //## end ClExeForJobSchecEXECUTE::m_poChannelGJDef%380F47E903AE.role

      //## Association: M4JSExeJob::<unnamed>%380F48130214
      //## Role: ClExeForJobSchecEXECUTE::m_poChannelExec%380F481303E0
      //## begin ClExeForJobSchecEXECUTE::m_poChannelExec%380F481303E0.role preserve=no  protected: ClChannel { -> RFHAN}
      ClChannel *m_poChannelExec;
      //## end ClExeForJobSchecEXECUTE::m_poChannelExec%380F481303E0.role

      //## Association: M4JSExeJob::<unnamed>%380F4B740195
      //## Role: ClExeForJobSchecEXECUTE::m_poChannelHandler%380F4B750060
      //## begin ClExeForJobSchecEXECUTE::m_poChannelHandler%380F4B750060.role preserve=no  protected: ClJSChannelHandler { -> RFHAN}
      ClJSChannelHandler *m_poChannelHandler;
      //## end ClExeForJobSchecEXECUTE::m_poChannelHandler%380F4B750060.role

      //## Association: M4JSExeJob::<unnamed>%38149670017C
      //## Role: ClExeForJobSchecEXECUTE::m_poDumpInfo%3814967002DB
      //## begin ClExeForJobSchecEXECUTE::m_poDumpInfo%3814967002DB.role preserve=no  protected: ExecDumpInfo { -> 1RFHAN}
      ExecDumpInfo *m_poDumpInfo;
      //## end ClExeForJobSchecEXECUTE::m_poDumpInfo%3814967002DB.role

      //## Association: M4JSExeJob::<unnamed>%381963A00110
      //## Role: ClExeForJobSchecEXECUTE::m_poSecurity%381963A00264
      //## begin ClExeForJobSchecEXECUTE::m_poSecurity%381963A00264.role preserve=no  protected: ClSecurity { -> RFHAN}
      ClSecurity *m_poSecurity;
      //## end ClExeForJobSchecEXECUTE::m_poSecurity%381963A00264.role

      //## Association: M4JSExeJob::<unnamed>%3821AECF026B
      //## Role: ClExeForJobSchecEXECUTE::m_poParameterHandler%3821AED001B8
      //## begin ClExeForJobSchecEXECUTE::m_poParameterHandler%3821AED001B8.role preserve=no  protected: ClJSParametersHandlers { -> RFHAN}
      ClJSParametersHandlers *m_poParameterHandler;
      //## end ClExeForJobSchecEXECUTE::m_poParameterHandler%3821AED001B8.role

    // Additional Protected Declarations
      //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.protected preserve=yes
      //## end ClExeForJobSchecEXECUTE%34E2D14402D6.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcDumpFile%36EE887F00CD
      //## begin ClExeForJobSchecEXECUTE::m_pcDumpFile%36EE887F00CD.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_pcDumpFile;
      //## end ClExeForJobSchecEXECUTE::m_pcDumpFile%36EE887F00CD.attr

    // Additional Private Declarations
      //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.private preserve=yes
      //## end ClExeForJobSchecEXECUTE%34E2D14402D6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExeForJobSchecEXECUTE%34E2D14402D6.implementation preserve=yes
      //## end ClExeForJobSchecEXECUTE%34E2D14402D6.implementation

};

//## begin ClExeForJobSchecEXECUTE%34E2D14402D6.postscript preserve=yes
//## end ClExeForJobSchecEXECUTE%34E2D14402D6.postscript

// Class ClExeForJobSchecEXECUTE 


//## Other Operations (inline)
inline m4int16_t ClExeForJobSchecEXECUTE::GetNDumpLevels () const
{
  //## begin ClExeForJobSchecEXECUTE::GetNDumpLevels%921585527.body preserve=yes

		return JS_EXEC_DUMP_LEVEL;

  //## end ClExeForJobSchecEXECUTE::GetNDumpLevels%921585527.body
}

inline ExecDumpInfo * ClExeForJobSchecEXECUTE::GetDumpInfo () const
{
  //## begin ClExeForJobSchecEXECUTE::GetDumpInfo%940836431.body preserve=yes

	return m_poDumpInfo;

  //## end ClExeForJobSchecEXECUTE::GetDumpInfo%940836431.body
}

//## begin module%34E2EB8200BA.epilog preserve=yes
//## end module%34E2EB8200BA.epilog


#endif
