//## begin module%38560F7F0360.cm preserve=no
//## end module%38560F7F0360.cm

//## begin module%38560F7F0360.cp preserve=no
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
//## end module%38560F7F0360.cp

//## Module: Util_M4XML%38560F7F0360; Package specification
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Source file: D:\Work\v600\m4exeforchannel\inc\m4xmlutil.hpp

#ifndef util_m4xml_h
#define util_m4xml_h 1

//## begin module%38560F7F0360.additionalIncludes preserve=no
//## end module%38560F7F0360.additionalIncludes

//## begin module%38560F7F0360.includes preserve=yes
#include <m4types.hpp>
#include <m4thread.hpp>
#include "m4stl.hpp"
#include <m4var.hpp>
#include <metadatasubsys.hpp>

class ClStatus;
class ClExecutorState;
class M4DataStorage;
class ClM4XMLContext;
class ClExecutorStateManager;
class ClM4XMLInfo;
class ClChannel;
class ClChannelManager;
class ExecDumpInfo;
class ClM4ObjService;

//## begin module%38560F7F0360.declarations preserve=no
//## end module%38560F7F0360.declarations

//## begin module%38560F7F0360.additionalDeclarations preserve=yes
//## end module%38560F7F0360.additionalDeclarations


//## begin ClCMInitParameters%39E2023C0345.preface preserve=yes
//## end ClCMInitParameters%39E2023C0345.preface

//## Class: ClCMInitParameters%39E2023C0345
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39E2023C0346;ClM4XMLContext { -> F}
//## Uses: <unnamed>%39E2023C034F;string { -> }
//## Uses: <unnamed>%39E2023C0350;ClChannel { -> F}
//## Uses: <unnamed>%39E2023C0359;ClChannelManager { -> F}
//## Uses: <unnamed>%39E326FE02CA;ClM4ObjService { -> F}
//## Uses: <unnamed>%39E327E8017C;ClExecutorStateManager { -> F}
//## Uses: <unnamed>%39F717060026;ExecDumpInfo { -> F}
//## Uses: <unnamed>%40EE46C402A9;ClClientMetaData { -> }
//## Uses: <unnamed>%40EE48D5026B;ClMetaDataSubsystem { -> }

class ClCMInitParameters 
{
  //## begin ClCMInitParameters%39E2023C0345.initialDeclarations preserve=yes
  //## end ClCMInitParameters%39E2023C0345.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCMInitParameters%971105358
      ClCMInitParameters ();


    //## Other Operations (specified)
      //## Operation: SetStatus%971105361
      void SetStatus (ClStatus *ai_pStatus);

      //## Operation: GetStatus%971105362
      ClStatus * GetStatus () const;

      //## Operation: Reset%971105363
      m4return_t Reset (m4bool_t ai_bOnlyReset = M4_TRUE, m4bool_t ai_bErase = M4_TRUE);

      //## Operation: SetSubSessionMode%971105366
      void SetSubSessionMode (const string& ai_sMode);

      //## Operation: GetSubSessionMode%971105367
      const string & GetSubSessionMode () const;

      //## Operation: SetSubSessionId%971105368
      void SetSubSessionId (const string& ai_sId);

      //## Operation: GetSubSessionId%971105369
      const string & GetSubSessionId () const;

      //## Operation: SetSessionId%971105370
      void SetSessionId (m4uint32_t ai_iSessionId);

      //## Operation: GetSessionId%971105371
      m4uint32_t GetSessionId () const;

      //## Operation: SetSessionChannel%971105372
      void SetSessionChannel (ClChannel *ai_poSessionChannel);

      //## Operation: GetSessionChannel%971105373
      ClChannel * GetSessionChannel () const;

      //## Operation: SetStateManager%971105374
      void SetStateManager (ClExecutorStateManager *ai_poStateManager);

      //## Operation: GetStateManager%971105375
      ClExecutorStateManager * GetStateManager () const;

      //## Operation: SetExecutorState%971105376
      void SetExecutorState (ClExecutorState *ai_poExecutorState);

      //## Operation: GetExecutorState%971105377
      ClExecutorState * GetExecutorState () const;

      //## Operation: SetEndSubSessionId%971105378
      void SetEndSubSessionId (const string& ai_sId);

      //## Operation: GetEndSubSessionId%971105379
      const string & GetEndSubSessionId () const;

      //## Operation: SetInputDataStorage%971105380
      void SetInputDataStorage (M4DataStorage *ai_poDataStorage);

      //## Operation: GetInputDataStorage%971105381
      M4DataStorage * GetInputDataStorage () const;

      //## Operation: SetExecDumpInfo%972494018
      void SetExecDumpInfo (ExecDumpInfo *ai_poExecDumpInfo);

      //## Operation: GetExecDumpInfo%972494019
      ExecDumpInfo * GetExecDumpInfo () const;

      //## Operation: SetPersistLevel%987779724
      void SetPersistLevel (const string& ai_sLevel);

      //## Operation: GetPersistLevel%987779725
      const string & GetPersistLevel () const;

      //## Operation: SetMetadataSubsystem%1089357390
      m4void_t SetMetadataSubsystem (ClMetaDataSubsystem *ai_pMetadataSubsystem);

      //## Operation: GetMetadataSubsystem%1089357391
      ClMetaDataSubsystem* GetMetadataSubsystem () const;

	  m4void_t SetParseStatus(m4bool_t ai_bStatus);

	  m4bool_t GetParseStatus() const;

    // Additional Public Declarations
      //## begin ClCMInitParameters%39E2023C0345.public preserve=yes
      //## end ClCMInitParameters%39E2023C0345.public

  protected:
    // Additional Protected Declarations
      //## begin ClCMInitParameters%39E2023C0345.protected preserve=yes
      //## end ClCMInitParameters%39E2023C0345.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poStatus%39E2023C037F
      //## begin ClCMInitParameters::m_poStatus%39E2023C037F.attr preserve=no  private: ClStatus * {UA} NULL
      ClStatus *m_poStatus;
      //## end ClCMInitParameters::m_poStatus%39E2023C037F.attr

      //## Attribute: m_sSubSessionId%39E2023C0381
      //## begin ClCMInitParameters::m_sSubSessionId%39E2023C0381.attr preserve=no  private: string {UA} 
      string m_sSubSessionId;
      //## end ClCMInitParameters::m_sSubSessionId%39E2023C0381.attr

      //## Attribute: m_sSubSessionMode%39E2023C0382
      //## begin ClCMInitParameters::m_sSubSessionMode%39E2023C0382.attr preserve=no  private: string {UA} 
      string m_sSubSessionMode;
      //## end ClCMInitParameters::m_sSubSessionMode%39E2023C0382.attr

      //## Attribute: m_poStateManager%39E2023C0384
      //## begin ClCMInitParameters::m_poStateManager%39E2023C0384.attr preserve=no  private: ClExecutorStateManager * {UA} NULL
      ClExecutorStateManager *m_poStateManager;
      //## end ClCMInitParameters::m_poStateManager%39E2023C0384.attr

      //## Attribute: m_iSessionId%39E2023C0385
      //## begin ClCMInitParameters::m_iSessionId%39E2023C0385.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iSessionId;
      //## end ClCMInitParameters::m_iSessionId%39E2023C0385.attr

      //## Attribute: m_poSessionChannel%39E2023C0386
      //## begin ClCMInitParameters::m_poSessionChannel%39E2023C0386.attr preserve=no  private: ClChannel * {UA} NULL
      ClChannel *m_poSessionChannel;
      //## end ClCMInitParameters::m_poSessionChannel%39E2023C0386.attr

      //## Attribute: m_poExecutorState%39E2023C0387
      //## begin ClCMInitParameters::m_poExecutorState%39E2023C0387.attr preserve=no  private: ClExecutorState * {UA} NULL
      ClExecutorState *m_poExecutorState;
      //## end ClCMInitParameters::m_poExecutorState%39E2023C0387.attr

      //## Attribute: m_sEndSubSessionId%39E2023C0388
      //## begin ClCMInitParameters::m_sEndSubSessionId%39E2023C0388.attr preserve=no  private: string {UA} 
      string m_sEndSubSessionId;
      //## end ClCMInitParameters::m_sEndSubSessionId%39E2023C0388.attr

      //## Attribute: m_poInputDataStorage%39E2023C0389
      //## begin ClCMInitParameters::m_poInputDataStorage%39E2023C0389.attr preserve=no  private: M4DataStorage * {UA} NULL
      M4DataStorage *m_poInputDataStorage;
      //## end ClCMInitParameters::m_poInputDataStorage%39E2023C0389.attr

      //## Attribute: m_poExecInfo%39F714620024
      //## begin ClCMInitParameters::m_poExecInfo%39F714620024.attr preserve=no  private: ExecDumpInfo * {UA} NULL
      ExecDumpInfo *m_poExecInfo;
      //## end ClCMInitParameters::m_poExecInfo%39F714620024.attr

      //## Attribute: m_sPersistLevel%3AE0620C031A
      //## begin ClCMInitParameters::m_sPersistLevel%3AE0620C031A.attr preserve=no  private: string {UA} 
      string m_sPersistLevel;
      //## end ClCMInitParameters::m_sPersistLevel%3AE0620C031A.attr

      //## Attribute: m_poMetadataSubsystem%40EE46CD0365
      //## begin ClCMInitParameters::m_poMetadataSubsystem%40EE46CD0365.attr preserve=no  private: ClMetaDataSubsystem* {UA} NULL
      ClMetaDataSubsystem* m_poMetadataSubsystem;
      //## end ClCMInitParameters::m_poMetadataSubsystem%40EE46CD0365.attr
	  // indicates if the parsing was ok from multiple check consistencies
	  m4bool_t m_bParseStatus;

    // Additional Private Declarations
      //## begin ClCMInitParameters%39E2023C0345.private preserve=yes
      //## end ClCMInitParameters%39E2023C0345.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCMInitParameters%39E2023C0345.implementation preserve=yes
      //## end ClCMInitParameters%39E2023C0345.implementation

};

//## begin ClCMInitParameters%39E2023C0345.postscript preserve=yes
//## end ClCMInitParameters%39E2023C0345.postscript

//## begin ExecInfoMap%39E2F1F9027D.preface preserve=yes
//## end ExecInfoMap%39E2F1F9027D.preface

//## Class: ExecInfoMap%39E2F1F9027D; Instantiated Class
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39E2F1F9027F;ClM4XMLInfo { -> F}

typedef map< m4iden_t,ClM4XMLInfo *,less<m4iden_t> > ExecInfoMap;

//## begin ExecInfoMap%39E2F1F9027D.postscript preserve=yes
//## end ExecInfoMap%39E2F1F9027D.postscript

//## begin ClM4XMLInfo%39E2F1F902E1.preface preserve=yes
//## end ClM4XMLInfo%39E2F1F902E1.preface

//## Class: ClM4XMLInfo%39E2F1F902E1
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4XMLInfo 
{
  //## begin ClM4XMLInfo%39E2F1F902E1.initialDeclarations preserve=yes
  //## end ClM4XMLInfo%39E2F1F902E1.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: SetInfo%971105382
      virtual m4return_t SetInfo (m4pcchar_t ai_pcInfo) = 0;

      //## Operation: SetDebugInfo%971105383
      virtual m4return_t SetDebugInfo (m4pcchar_t ai_pcInfo) = 0;

      //## Operation: SetFileInfo%971105384
      virtual m4return_t SetFileInfo (m4pcchar_t ai_pcInfo) = 0;

      //## Operation: SetFileInfoByThread%971105385
      static void SetFileInfoByThread (m4pcchar_t ai_pcInfo);

      //## Operation: AddInstanceByThread%971105386
      void AddInstanceByThread ();

      //## Operation: IsInit%971105387
      static m4bool_t IsInit ();

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%39E2F1F90327
      //## Role: ClM4XMLInfo::m_oMap%39E2F1F90345
      //## begin ClM4XMLInfo::m_oMap%39E2F1F90345.role preserve=no  public: static ExecInfoMap { -> VHAN}
      static ExecInfoMap m_oMap;
      //## end ClM4XMLInfo::m_oMap%39E2F1F90345.role

    // Additional Public Declarations
      //## begin ClM4XMLInfo%39E2F1F902E1.public preserve=yes
      //## end ClM4XMLInfo%39E2F1F902E1.public

  protected:

    //## Other Operations (specified)
      //## Operation: SetInit%971105388
      static void SetInit ();

    // Additional Protected Declarations
      //## begin ClM4XMLInfo%39E2F1F902E1.protected preserve=yes
      //## end ClM4XMLInfo%39E2F1F902E1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bInit%39E2F1F90309
      //## begin ClM4XMLInfo::m_bInit%39E2F1F90309.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bInit;
      //## end ClM4XMLInfo::m_bInit%39E2F1F90309.attr

    // Additional Private Declarations
      //## begin ClM4XMLInfo%39E2F1F902E1.private preserve=yes
      //## end ClM4XMLInfo%39E2F1F902E1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLInfo%39E2F1F902E1.implementation preserve=yes
      //## end ClM4XMLInfo%39E2F1F902E1.implementation

};

//## begin ClM4XMLInfo%39E2F1F902E1.postscript preserve=yes
//## end ClM4XMLInfo%39E2F1F902E1.postscript

//## begin ExecInfoMapIt%39E2F1F90383.preface preserve=yes
//## end ExecInfoMapIt%39E2F1F90383.preface

//## Class: ExecInfoMapIt%39E2F1F90383
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39E2F1F90382;ExecInfoMap { -> }

typedef ExecInfoMap::iterator ExecInfoMapIt;

//## begin ExecInfoMapIt%39E2F1F90383.postscript preserve=yes
//## end ExecInfoMapIt%39E2F1F90383.postscript

//## begin ClM4XMLVariant%39EC630800BA.preface preserve=yes
//## end ClM4XMLVariant%39EC630800BA.preface

//## Class: ClM4XMLVariant%39EC630800BA
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClM4XMLVariant : public m4VariantType  //## Inherits: <unnamed>%39EC6326001D
{
  //## begin ClM4XMLVariant%39EC630800BA.initialDeclarations preserve=yes
  //## end ClM4XMLVariant%39EC630800BA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClM4XMLVariant%971793327
      ClM4XMLVariant ();


    //## Other Operations (specified)
      //## Operation: SetUsed%971793325
      void SetUsed ();

      //## Operation: IsUsed%971793326
      m4bool_t IsUsed () const;

    // Additional Public Declarations
      //## begin ClM4XMLVariant%39EC630800BA.public preserve=yes
      //## end ClM4XMLVariant%39EC630800BA.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4XMLVariant%39EC630800BA.protected preserve=yes
      //## end ClM4XMLVariant%39EC630800BA.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsUsed%39EC636A035A
      //## begin ClM4XMLVariant::m_bIsUsed%39EC636A035A.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsUsed;
      //## end ClM4XMLVariant::m_bIsUsed%39EC636A035A.attr

    // Additional Private Declarations
      //## begin ClM4XMLVariant%39EC630800BA.private preserve=yes
      //## end ClM4XMLVariant%39EC630800BA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLVariant%39EC630800BA.implementation preserve=yes
      //## end ClM4XMLVariant%39EC630800BA.implementation

};

//## begin ClM4XMLVariant%39EC630800BA.postscript preserve=yes
//## end ClM4XMLVariant%39EC630800BA.postscript

// Class ClCMInitParameters 


//## Other Operations (inline)
inline void ClCMInitParameters::SetStatus (ClStatus *ai_pStatus)
{
  //## begin ClCMInitParameters::SetStatus%971105361.body preserve=yes
	m_poStatus = ai_pStatus;
  //## end ClCMInitParameters::SetStatus%971105361.body
}

inline ClStatus * ClCMInitParameters::GetStatus () const
{
  //## begin ClCMInitParameters::GetStatus%971105362.body preserve=yes
	return m_poStatus;
  //## end ClCMInitParameters::GetStatus%971105362.body
}

inline void ClCMInitParameters::SetSubSessionMode (const string& ai_sMode)
{
  //## begin ClCMInitParameters::SetSubSessionMode%971105366.body preserve=yes
	m_sSubSessionMode = ai_sMode;
  //## end ClCMInitParameters::SetSubSessionMode%971105366.body
}

inline const string & ClCMInitParameters::GetSubSessionMode () const
{
  //## begin ClCMInitParameters::GetSubSessionMode%971105367.body preserve=yes
	return m_sSubSessionMode;
  //## end ClCMInitParameters::GetSubSessionMode%971105367.body
}

inline void ClCMInitParameters::SetSubSessionId (const string& ai_sId)
{
  //## begin ClCMInitParameters::SetSubSessionId%971105368.body preserve=yes
	m_sSubSessionId = ai_sId;
  //## end ClCMInitParameters::SetSubSessionId%971105368.body
}

inline const string & ClCMInitParameters::GetSubSessionId () const
{
  //## begin ClCMInitParameters::GetSubSessionId%971105369.body preserve=yes
	return m_sSubSessionId;
  //## end ClCMInitParameters::GetSubSessionId%971105369.body
}

inline void ClCMInitParameters::SetSessionId (m4uint32_t ai_iSessionId)
{
  //## begin ClCMInitParameters::SetSessionId%971105370.body preserve=yes
	m_iSessionId = ai_iSessionId;
  //## end ClCMInitParameters::SetSessionId%971105370.body
}

inline m4uint32_t ClCMInitParameters::GetSessionId () const
{
  //## begin ClCMInitParameters::GetSessionId%971105371.body preserve=yes
	return m_iSessionId;
  //## end ClCMInitParameters::GetSessionId%971105371.body
}

inline void ClCMInitParameters::SetSessionChannel (ClChannel *ai_poSessionChannel)
{
  //## begin ClCMInitParameters::SetSessionChannel%971105372.body preserve=yes
	m_poSessionChannel = ai_poSessionChannel;
  //## end ClCMInitParameters::SetSessionChannel%971105372.body
}

inline ClChannel * ClCMInitParameters::GetSessionChannel () const
{
  //## begin ClCMInitParameters::GetSessionChannel%971105373.body preserve=yes
	return m_poSessionChannel;
  //## end ClCMInitParameters::GetSessionChannel%971105373.body
}

inline void ClCMInitParameters::SetStateManager (ClExecutorStateManager *ai_poStateManager)
{
  //## begin ClCMInitParameters::SetStateManager%971105374.body preserve=yes
	m_poStateManager = ai_poStateManager;
  //## end ClCMInitParameters::SetStateManager%971105374.body
}

inline ClExecutorStateManager * ClCMInitParameters::GetStateManager () const
{
  //## begin ClCMInitParameters::GetStateManager%971105375.body preserve=yes
	return m_poStateManager;
  //## end ClCMInitParameters::GetStateManager%971105375.body
}

inline void ClCMInitParameters::SetExecutorState (ClExecutorState *ai_poExecutorState)
{
  //## begin ClCMInitParameters::SetExecutorState%971105376.body preserve=yes
	m_poExecutorState = ai_poExecutorState;
  //## end ClCMInitParameters::SetExecutorState%971105376.body
}

inline ClExecutorState * ClCMInitParameters::GetExecutorState () const
{
  //## begin ClCMInitParameters::GetExecutorState%971105377.body preserve=yes
	return m_poExecutorState;
  //## end ClCMInitParameters::GetExecutorState%971105377.body
}

inline void ClCMInitParameters::SetEndSubSessionId (const string& ai_sId)
{
  //## begin ClCMInitParameters::SetEndSubSessionId%971105378.body preserve=yes
	m_sEndSubSessionId = ai_sId;
  //## end ClCMInitParameters::SetEndSubSessionId%971105378.body
}

inline const string & ClCMInitParameters::GetEndSubSessionId () const
{
  //## begin ClCMInitParameters::GetEndSubSessionId%971105379.body preserve=yes
	return m_sEndSubSessionId;
  //## end ClCMInitParameters::GetEndSubSessionId%971105379.body
}

inline void ClCMInitParameters::SetInputDataStorage (M4DataStorage *ai_poDataStorage)
{
  //## begin ClCMInitParameters::SetInputDataStorage%971105380.body preserve=yes
	m_poInputDataStorage = ai_poDataStorage;
  //## end ClCMInitParameters::SetInputDataStorage%971105380.body
}

inline M4DataStorage * ClCMInitParameters::GetInputDataStorage () const
{
  //## begin ClCMInitParameters::GetInputDataStorage%971105381.body preserve=yes
	return m_poInputDataStorage;
  //## end ClCMInitParameters::GetInputDataStorage%971105381.body
}

inline void ClCMInitParameters::SetExecDumpInfo (ExecDumpInfo *ai_poExecDumpInfo)
{
  //## begin ClCMInitParameters::SetExecDumpInfo%972494018.body preserve=yes
	m_poExecInfo = ai_poExecDumpInfo;
  //## end ClCMInitParameters::SetExecDumpInfo%972494018.body
}

inline ExecDumpInfo * ClCMInitParameters::GetExecDumpInfo () const
{
  //## begin ClCMInitParameters::GetExecDumpInfo%972494019.body preserve=yes
	return m_poExecInfo;
  //## end ClCMInitParameters::GetExecDumpInfo%972494019.body
}

inline void ClCMInitParameters::SetPersistLevel (const string& ai_sLevel)
{
  //## begin ClCMInitParameters::SetPersistLevel%987779724.body preserve=yes
	m_sPersistLevel = ai_sLevel;
  //## end ClCMInitParameters::SetPersistLevel%987779724.body
}

inline const string & ClCMInitParameters::GetPersistLevel () const
{
  //## begin ClCMInitParameters::GetPersistLevel%987779725.body preserve=yes
	return m_sPersistLevel;
  //## end ClCMInitParameters::GetPersistLevel%987779725.body
}

inline m4void_t ClCMInitParameters::SetMetadataSubsystem (ClMetaDataSubsystem *ai_pMetadataSubsystem)
{
  //## begin ClCMInitParameters::SetMetadataSubsystem%1089357390.body preserve=yes
	m_poMetadataSubsystem = ai_pMetadataSubsystem;
  //## end ClCMInitParameters::SetMetadataSubsystem%1089357390.body
}

inline ClMetaDataSubsystem* ClCMInitParameters::GetMetadataSubsystem () const
{
  //## begin ClCMInitParameters::GetMetadataSubsystem%1089357391.body preserve=yes
	return 	m_poMetadataSubsystem;
  //## end ClCMInitParameters::GetMetadataSubsystem%1089357391.body
}

inline m4void_t ClCMInitParameters::SetParseStatus(m4bool_t ai_bStatus) 
{
	m_bParseStatus = ai_bStatus;
}

inline m4bool_t ClCMInitParameters::GetParseStatus() const
{
	return m_bParseStatus;
}

// Class ClM4XMLInfo 


//## Other Operations (inline)
inline void ClM4XMLInfo::SetFileInfoByThread (m4pcchar_t ai_pcInfo)
{
  //## begin ClM4XMLInfo::SetFileInfoByThread%971105385.body preserve=yes
	if (IsInit() == M4_FALSE)
		return;

	ExecInfoMapIt it = m_oMap.find(M4Thread::GetCurrentIdThread());

	if (it == m_oMap.end())
		return ;

	((*it).second)->SetFileInfo(ai_pcInfo);
  //## end ClM4XMLInfo::SetFileInfoByThread%971105385.body
}

inline m4bool_t ClM4XMLInfo::IsInit ()
{
  //## begin ClM4XMLInfo::IsInit%971105387.body preserve=yes
	return m_bInit;
  //## end ClM4XMLInfo::IsInit%971105387.body
}

inline void ClM4XMLInfo::SetInit ()
{
  //## begin ClM4XMLInfo::SetInit%971105388.body preserve=yes
	m_bInit = M4_TRUE;
  //## end ClM4XMLInfo::SetInit%971105388.body
}

// Class ClM4XMLVariant 


//## Other Operations (inline)
inline void ClM4XMLVariant::SetUsed ()
{
  //## begin ClM4XMLVariant::SetUsed%971793325.body preserve=yes
	m_bIsUsed = M4_TRUE;
  //## end ClM4XMLVariant::SetUsed%971793325.body
}

inline m4bool_t ClM4XMLVariant::IsUsed () const
{
  //## begin ClM4XMLVariant::IsUsed%971793326.body preserve=yes
	return m_bIsUsed;
  //## end ClM4XMLVariant::IsUsed%971793326.body
}

//## begin module%38560F7F0360.epilog preserve=yes
//## end module%38560F7F0360.epilog


#endif
