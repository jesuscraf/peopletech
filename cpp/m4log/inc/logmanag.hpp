//## begin module%377C884E0150.cm preserve=no
//## end module%377C884E0150.cm

//## begin module%377C884E0150.cp preserve=no
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
//## end module%377C884E0150.cp

//## Module: logmanag%377C884E0150; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\inc\logmanag.hpp

#ifndef _LOGMANAG_HPP_
#define _LOGMANAG_HPP_ 1

//## begin module%377C884E0150.additionalIncludes preserve=no
//## end module%377C884E0150.additionalIncludes

//## begin module%377C884E0150.includes preserve=yes
#include "m4stl.hpp"
//## end module%377C884E0150.includes

// cnfglogs
#include <cnfglogs.hpp>
// logsys
#include <logsys.hpp>
// perslog
#include <perslog.hpp>
// debuglog
#include <debuglog.hpp>
// dbgresou
#include <dbgresou.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>


//## begin module%377C884E0150.declarations preserve=no
//## end module%377C884E0150.declarations

//## begin module%377C884E0150.additionalDeclarations preserve=yes


#define M4LOG_NO_TRANSLATABLE		0x01
#define M4LOG_TRANSLATABLE			0x02
#define M4LOG_BOTH_TRANSLATABLE		0x03

//typedef map<m4int32_t, ClLogSystem *, less<m4int32_t> > LOGMAP;



//## end module%377C884E0150.additionalDeclarations


//## begin LOGMAP%377C884F0292.preface preserve=yes
//## end LOGMAP%377C884F0292.preface

//## Class: LOGMAP%377C884F0292; Instantiated Class
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _K%377C884F0329;m4int32_t { -> F}
//## Uses: _Ty%377C884F032A;ClLogSystem { -> F}

typedef map <  m4int32_t  ,  ClLogSystem * , less < m4int32_t  > > LOGMAP;

//## begin LOGMAP%377C884F0292.postscript preserve=yes
//## end LOGMAP%377C884F0292.postscript

//## begin ClLogManager%377C883E034C.preface preserve=yes
//## end ClLogManager%377C883E034C.preface

//## Class: ClLogManager%377C883E034C
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C88490250;friend ClStorageList { -> }
//## Uses: <unnamed>%377C884F02A9;m4int16_t { -> }
//## Uses: <unnamed>%377C884F02AF;m4bool_t { -> }
//## Uses: <unnamed>%377C884F02B1;m4return_t { -> }
//## Uses: <unnamed>%377C884F02D9;ClLogSystem { -> }
//## Uses: <unnamed>%377C884F02E3;m4uint32_t { -> }
//## Uses: <unnamed>%377C884F02E6;m4double_t { -> }
//## Uses: <unnamed>%377C884F02EA;m4char_t { -> }
//## Uses: <unnamed>%377C884F031C;ClStaticModuleList { -> }
//## Uses: <unnamed>%377C884F0320;ClPersistentStorage { -> }

class M4_DECL_M4LOG ClLogManager 
{
  //## begin ClLogManager%377C883E034C.initialDeclarations preserve=yes
  //## end ClLogManager%377C883E034C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLogManager%1721961973; C++
      ClLogManager ();

    //## Destructor (specified)
      //## Operation: ~ClLogManager%1090247477; C++
      ~ClLogManager ();


    //## Other Operations (specified)
      //## Operation: SetIniFile%1935879282; C++
      m4int16_t SetIniFile (M4ClString sfile);

      //## Operation: Initialize%1019146843; C++
      m4int16_t Initialize ();

      //## Operation: Initialized%-73067728; C++
      m4bool_t Initialized ()
      {
        //## begin ClLogManager::Initialized%-73067728.body preserve=yes
		return m_bInitialized;
        //## end ClLogManager::Initialized%-73067728.body
      }

      //## Operation: updateLogConfiguration%467300552; C++
      m4return_t updateLogConfiguration (const M4ClString& ai_conigFileName);

      //## Operation: SetCode%-1048504906; C++
      m4int16_t SetCode ();

      //## Operation: CreateCurrentThreadLogSystem%-1531797207; C++
      m4int16_t CreateCurrentThreadLogSystem ();

      //## Operation: CreateLogSystem%1331215295; C++
      m4int16_t CreateLogSystem (m4int32_t hthread);

      //## Operation: ExistLogSystem%-750215146; C++
      m4bool_t ExistLogSystem (m4int32_t hthread);

      //## Operation: GetCurrentLogSystem%901078942; C++
      ClLogSystem* GetCurrentLogSystem ();

      //## Operation: SetLogSystem%1992430839; C++
      ClLogSystem* SetLogSystem (ClLogSystem* ai_LogSystem);

      //## Operation: GetLogSystem%1860498566; C++
      m4return_t GetLogSystem (m4int32_t hthread, ClLogSystem*& plogSystem);

      //## Operation: GetDefaultLevelInfo%-1746903291; C++
      m4uint32_t GetDefaultLevelInfo ();

      //## Operation: TestCrono%1288571702; C++
      m4bool_t TestCrono ()
      {
        //## begin ClLogManager::TestCrono%1288571702.body preserve=yes
		return m_bcrono;
        //## end ClLogManager::TestCrono%1288571702.body
      }

      //## Operation: TotalTime%299900736; C++
      m4double_t TotalTime ()
      {
        //## begin ClLogManager::TotalTime%299900736.body preserve=yes
		return m_dTotalTime;
        //## end ClLogManager::TotalTime%299900736.body
      }

      //## Operation: AddTime%-1242098596; C++
      void AddTime (m4double_t dTime)
      {
        //## begin ClLogManager::AddTime%-1242098596.body preserve=yes
		m_dTotalTime += dTime;
        //## end ClLogManager::AddTime%-1242098596.body
      }

      //## Operation: SetCReturn%-447715444; C++
      void SetCReturn (m4char_t creturn);

      //## Operation: GetCReturn%829933598; C++
      m4char_t GetCReturn ()
      {
        //## begin ClLogManager::GetCReturn%829933598.body preserve=yes
		return m_creturn;
        //## end ClLogManager::GetCReturn%829933598.body
      }

      //## Operation: SetEndMessageChar%-1983658373; C++
      void SetEndMessageChar (m4char_t cEndMessage);

      //## Operation: GetEndMessageChar%1442077237; C++
      m4char_t GetEndMessageChar ()
      {
        //## begin ClLogManager::GetEndMessageChar%1442077237.body preserve=yes
		return m_cEndMessage;
        //## end ClLogManager::GetEndMessageChar%1442077237.body
      }

      //## Operation: getShowTime%-534090728; C++
      m4bool_t getShowTime ()
      {
        //## begin ClLogManager::getShowTime%-534090728.body preserve=yes
		 return m_bshowTime;
        //## end ClLogManager::getShowTime%-534090728.body
      }

      //## Operation: setShowTime%-746237967; C++
      void setShowTime (m4bool_t ai_bsshowTime)
      {
        //## begin ClLogManager::setShowTime%-746237967.body preserve=yes
		m_bshowTime = ai_bsshowTime ;
        //## end ClLogManager::setShowTime%-746237967.body
      }

      //## Operation: GetMaxQueueSize%1572944467; C++
      m4uint32_t GetMaxQueueSize ();

      //## Operation: SetMaxQueueSize%-869905716; C++
      void SetMaxQueueSize (m4uint32_t ai_MaxQueueSize);

      //## Operation: Error%698462692; C++
      m4bool_t Error ();

      //## Operation: Warning%2111473341; C++
      m4bool_t Warning ();

      //## Operation: DebugInfo%638440848; C++
      m4bool_t DebugInfo ();

      //## Operation: TraceInfo%1886886352; C++
      m4bool_t TraceInfo ();

      //## Operation: TestErrorMode%-1205211492; C++
      m4bool_t TestErrorMode (m4int16_t berror);

      //## Operation: ActiveError%1996652263; C++
      m4int16_t ActiveError ();

      //## Operation: ActiveWarning%-1295001793; C++
      m4int16_t ActiveWarning ();

      //## Operation: ActiveDebugInfo%-948510497; C++
      m4int16_t ActiveDebugInfo ();

      //## Operation: ActiveTraceInfo%406593264; C++
      m4int16_t ActiveTraceInfo ();

      //## Operation: EnableRemoteLog%2126153487; C++
      m4int16_t EnableRemoteLog ();

      //## Operation: DefuseError%-592654900; C++
      m4int16_t DefuseError ();

      //## Operation: DefuseWarning%1274363651; C++
      m4int16_t DefuseWarning ();

      //## Operation: DefuseDebugInfo%1650042316; C++
      m4int16_t DefuseDebugInfo ();

      //## Operation: DefuseTraceInfo%-24588130; C++
      m4int16_t DefuseTraceInfo ();

      //## Operation: DisableRemoteLog%489463073; C++
      m4int16_t DisableRemoteLog ();

      //## Operation: GetRemoteLog%849195721; C++
      m4bool_t GetRemoteLog ();

      //## Operation: StorageAlways%-1337757572; C++
      m4bool_t StorageAlways ();

      //## Operation: ActiveStorageAlways%818978504; C++
      void ActiveStorageAlways ();

      //## Operation: DefuseStorageAlways%935149151; C++
      void DefuseStorageAlways ();

      //## Operation: DestroyLogSystem%-1406380236; C++
      m4return_t DestroyLogSystem ();

      //## Operation: IncMessageCounter%2101165916; C++
      m4return_t IncMessageCounter ();

      //## Operation: GetMessageCounter%766650907; C++
      m4uint32_t GetMessageCounter ();

      //## Operation: GetModuleList%-494877376; C++
      ClStaticModuleList* GetModuleList ();

      //## Operation: AddStorage%-1672270263; C++
      m4return_t AddStorage (ClPersistentStorage* pstorage);

      //## Operation: GetStorageList%976551028; C++
      ClStorageList* GetStorageList ();

      //## Operation: isTranslatable%942747997
      m4uint32_t isTranslatable ();

      //## Operation: enableTranslateMessages%942747998
      void enableTranslateMessages ();

      //## Operation: disableTranslateMessages%942747999
      void disableTranslateMessages ();

      //## Operation: bothTranslateMessages%943956490
      void bothTranslateMessages ();

      //## Operation: getDefaultIdiom%942748000
      m4uint32_t getDefaultIdiom ();

      //## Operation: setDefaultIdiom%942748001
      void setDefaultIdiom (m4uint32_t ai_defaultIdiom);

      //## Operation: addInternalMessage%944066181
      m4return_t addInternalMessage (ClAppItem* pappoint);

    // Additional Public Declarations
      //## begin ClLogManager%377C883E034C.public preserve=yes
      //## end ClLogManager%377C883E034C.public

  protected:
    // Additional Protected Declarations
      //## begin ClLogManager%377C883E034C.protected preserve=yes
      //## end ClLogManager%377C883E034C.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C884E0205
      //## Role: ClLogManager::m_ierror%377C884E0207
      //## begin ClLogManager::m_ierror%377C884E0207.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_ierror;
      //## end ClLogManager::m_ierror%377C884E0207.role

      //## Association: M4Log::<unnamed>%377C884E0208
      //## Role: ClLogManager::m_iwarning%377C884E020A
      //## begin ClLogManager::m_iwarning%377C884E020A.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_iwarning;
      //## end ClLogManager::m_iwarning%377C884E020A.role

      //## Association: M4Log::<unnamed>%377C884E020B
      //## Role: ClLogManager::m_idebugInfo%377C884E020D
      //## begin ClLogManager::m_idebugInfo%377C884E020D.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_idebugInfo;
      //## end ClLogManager::m_idebugInfo%377C884E020D.role

      //## Association: M4Log::<unnamed>%377C884E020E
      //## Role: ClLogManager::m_itrace%377C884E0210
      //## begin ClLogManager::m_itrace%377C884E0210.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_itrace;
      //## end ClLogManager::m_itrace%377C884E0210.role

      //## Association: M4Log::<unnamed>%377C884E0211
      //## Role: ClLogManager::m_bRemoteLog%377C884E0213
      //## begin ClLogManager::m_bRemoteLog%377C884E0213.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bRemoteLog;
      //## end ClLogManager::m_bRemoteLog%377C884E0213.role

      //## Association: M4Log::<unnamed>%377C884E0214
      //## Role: ClLogManager::m_bshowTime%377C884E0216
      //## begin ClLogManager::m_bshowTime%377C884E0216.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bshowTime;
      //## end ClLogManager::m_bshowTime%377C884E0216.role

      //## Association: M4Log::<unnamed>%377C884E0217
      //## Role: ClLogManager::m_bStorageAlways%377C884E0219
      //## begin ClLogManager::m_bStorageAlways%377C884E0219.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bStorageAlways;
      //## end ClLogManager::m_bStorageAlways%377C884E0219.role

      //## Association: M4Log::<unnamed>%377C884E021A
      //## Role: ClLogManager::m_bcrono%377C884E021C
      //## begin ClLogManager::m_bcrono%377C884E021C.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bcrono;
      //## end ClLogManager::m_bcrono%377C884E021C.role

      //## Association: M4Log::<unnamed>%377C884E021E
      //## Role: ClLogManager::m_dTotalTime%377C884E0220
      //## begin ClLogManager::m_dTotalTime%377C884E0220.role preserve=no  private: m4double_t {1 -> 1VHAPN}
      m4double_t m_dTotalTime;
      //## end ClLogManager::m_dTotalTime%377C884E0220.role

      //## Association: M4Log::<unnamed>%377C884E0221
      //## Role: ClLogManager::m_bInitialized%377C884E0223
      //## begin ClLogManager::m_bInitialized%377C884E0223.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bInitialized;
      //## end ClLogManager::m_bInitialized%377C884E0223.role

      //## Association: M4Log::<unnamed>%377C884E0224
      //## Role: ClLogManager::m_creturn%377C884E0226
      //## begin ClLogManager::m_creturn%377C884E0226.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t m_creturn;
      //## end ClLogManager::m_creturn%377C884E0226.role

      //## Association: M4Log::<unnamed>%377C884E0227
      //## Role: ClLogManager::m_cEndMessage%377C884E0229
      //## begin ClLogManager::m_cEndMessage%377C884E0229.role preserve=no  private: m4char_t {1 -> 1VHAPN}
      m4char_t m_cEndMessage;
      //## end ClLogManager::m_cEndMessage%377C884E0229.role

      //## Association: M4Log::<unnamed>%377C884E0230
      //## Role: ClLogManager::m_GlobalMessageCounter%377C884E0232
      //## begin ClLogManager::m_GlobalMessageCounter%377C884E0232.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_GlobalMessageCounter;
      //## end ClLogManager::m_GlobalMessageCounter%377C884E0232.role

      //## Association: M4Log::<unnamed>%377C884E0233
      //## Role: ClLogManager::m_GlobalCounterMutex%377C884E0235
      //## begin ClLogManager::m_GlobalCounterMutex%377C884E0235.role preserve=no  private: ClMutex {1 -> 1VHAPN}
      ClMutex m_GlobalCounterMutex;
      //## end ClLogManager::m_GlobalCounterMutex%377C884E0235.role

      //## Association: M4Log::<unnamed>%377C884E0236
      //## Role: ClLogManager::m_AccessMutex%377C884E0238
      //## begin ClLogManager::m_AccessMutex%377C884E0238.role preserve=no  private: ClMutex {1 -> 1VHAPN}
      ClMutex m_AccessMutex;
      //## end ClLogManager::m_AccessMutex%377C884E0238.role

      //## Association: M4Log::<unnamed>%377C884E0239
      //## Role: ClLogManager::m_MaxQueueSize%377C884E023B
      //## begin ClLogManager::m_MaxQueueSize%377C884E023B.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_MaxQueueSize;
      //## end ClLogManager::m_MaxQueueSize%377C884E023B.role

      //## Association: M4Log::<unnamed>%377C884F0296
      //## Role: ClLogManager::m_logSystems%377C884F0298
      //## begin ClLogManager::m_logSystems%377C884F0298.role preserve=no  private: LOGMAP {1 -> 1VHAPN}
      LOGMAP m_logSystems;
      //## end ClLogManager::m_logSystems%377C884F0298.role

      //## Association: M4Log::<unnamed>%377C884F029C
      //## Role: ClLogManager::m_hmainThread%377C884F029E
      //## begin ClLogManager::m_hmainThread%377C884F029E.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_hmainThread;
      //## end ClLogManager::m_hmainThread%377C884F029E.role

      //## Association: M4Log::<unnamed>%377C884F029F
      //## Role: ClLogManager::m_configLogs%377C884F02A1
      //## begin ClLogManager::m_configLogs%377C884F02A1.role preserve=no  private: ClConfigLogs {1 -> 1VHAPN}
      ClConfigLogs m_configLogs;
      //## end ClLogManager::m_configLogs%377C884F02A1.role

      //## Association: M4Log::<unnamed>%377C884F02A2
      //## Role: ClLogManager::m_pModuleList%377C884F02A4
      //## begin ClLogManager::m_pModuleList%377C884F02A4.role preserve=no  private: ClStaticModuleList {1 -> 1RHAPN}
      ClStaticModuleList *m_pModuleList;
      //## end ClLogManager::m_pModuleList%377C884F02A4.role

      //## Association: M4Log::<unnamed>%377C884F02A6
      //## Role: ClLogManager::m_lstorages%377C884F02A8
      //## begin ClLogManager::m_lstorages%377C884F02A8.role preserve=no  private: ClStorageList {1 -> 1VHAPN}
      ClStorageList m_lstorages;
      //## end ClLogManager::m_lstorages%377C884F02A8.role

      //## Association: M4Log::<unnamed>%38313102005C
      //## Role: ClLogManager::m_translateMessages%38313102023D
      //## begin ClLogManager::m_translateMessages%38313102023D.role preserve=no  private: m4uint32_t { -> VHAN}
      m4uint32_t m_translateMessages;
      //## end ClLogManager::m_translateMessages%38313102023D.role

      //## Association: M4Log::<unnamed>%383132BA00FF
      //## Role: ClLogManager::m_uidefaultIdiom%383132BA039E
      //## begin ClLogManager::m_uidefaultIdiom%383132BA039E.role preserve=no  private: m4uint32_t { -> VHAN}
      m4uint32_t m_uidefaultIdiom;
      //## end ClLogManager::m_uidefaultIdiom%383132BA039E.role

      //## Association: M4Log::<unnamed>%384652A00000
      //## Role: ClLogManager::m_internalMessages%384652A001CD
      //## begin ClLogManager::m_internalMessages%384652A001CD.role preserve=no  private: ClAppointments { -> RHAN}
      ClAppointments *m_internalMessages;
      //## end ClLogManager::m_internalMessages%384652A001CD.role

      //## Association: M4Log::<unnamed>%3846608D0291
      //## Role: ClLogManager::m_errorExists%3846608E0076
      //## begin ClLogManager::m_errorExists%3846608E0076.role preserve=no  private: m4bool_t { -> VHAN}
      m4bool_t m_errorExists;
      //## end ClLogManager::m_errorExists%3846608E0076.role

    // Additional Private Declarations
      //## begin ClLogManager%377C883E034C.private preserve=yes
      //## end ClLogManager%377C883E034C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLogManager%377C883E034C.implementation preserve=yes
      //## end ClLogManager%377C883E034C.implementation

};

//## begin ClLogManager%377C883E034C.postscript preserve=yes
//## end ClLogManager%377C883E034C.postscript

//## begin module%377C884E0150.epilog preserve=yes
//## end module%377C884E0150.epilog


#endif
