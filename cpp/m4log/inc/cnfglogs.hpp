//## begin module%377C883E00AD.cm preserve=no
//## end module%377C883E00AD.cm

//## begin module%377C883E00AD.cp preserve=no
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
//## end module%377C883E00AD.cp

//## Module: cnfglogs%377C883E00AD; Package specification
//## Subsystem: M4Log::inc%377C883D0066
//	d:\compon\m4log\version\inc
//## Source file: D:\ricardo\m4log\inc\cnfglogs.hpp

#ifndef _CNFGLOGS_HPP_
#define _CNFGLOGS_HPP_ 1

//## begin module%377C883E00AD.additionalIncludes preserve=no
//## end module%377C883E00AD.additionalIncludes

//## begin module%377C883E00AD.includes preserve=yes
//## end module%377C883E00AD.includes

#include <dbgresou.hpp>
#include <m4string.hpp>
#include "m4stl.hpp"

class M4_DECL_M4LOG ClLogManager;

//## begin module%377C883E00AD.declarations preserve=no
//## end module%377C883E00AD.declarations

//## begin module%377C883E00AD.additionalDeclarations preserve=yes


const int MESSAGESIZE = 32000;

const int M4_ERR = 0;
const int M4_OK = 1;
const int M4_REM = 2;
const m4int32_t MAX_QUEUE_SIZE = 99999999;

#define MAX_DEVICES_GROUP	32

#include "m4stl.hpp"

/*
typedef deque<M4ClString> STRQUEUE;
typedef map<m4uint32_t, m4uint32_t, less < m4uint32_t > >	INT32MAP;


class ClLogManager;
*/



//## end module%377C883E00AD.additionalDeclarations


//## begin ClDefaultConfiguration%377C883E012F.preface preserve=yes
//## end ClDefaultConfiguration%377C883E012F.preface

//## Class: ClDefaultConfiguration%377C883E012F
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDefaultConfiguration 
{
  //## begin ClDefaultConfiguration%377C883E012F.initialDeclarations preserve=yes
  //## end ClDefaultConfiguration%377C883E012F.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~ClDefaultConfiguration%-1163554636; C++
      ~ClDefaultConfiguration ();


    //## Other Operations (specified)
      //## Operation: Instance%-761489822; C++
      static ClDefaultConfiguration* Instance ();

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883E018B
      //## Role: ClDefaultConfiguration::m_sReturn%377C883E018D
      //## begin ClDefaultConfiguration::m_sReturn%377C883E018D.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sReturn;
      //## end ClDefaultConfiguration::m_sReturn%377C883E018D.role

      //## Association: M4Log::<unnamed>%377C883E018E
      //## Role: ClDefaultConfiguration::m_sStorageAll%377C883E0190
      //## begin ClDefaultConfiguration::m_sStorageAll%377C883E0190.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sStorageAll;
      //## end ClDefaultConfiguration::m_sStorageAll%377C883E0190.role

      //## Association: M4Log::<unnamed>%377C883E0191
      //## Role: ClDefaultConfiguration::m_sTimeDeb%377C883E0193
      //## begin ClDefaultConfiguration::m_sTimeDeb%377C883E0193.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sTimeDeb;
      //## end ClDefaultConfiguration::m_sTimeDeb%377C883E0193.role

      //## Association: M4Log::<unnamed>%377C883E0194
      //## Role: ClDefaultConfiguration::m_sTimeWar%377C883E0196
      //## begin ClDefaultConfiguration::m_sTimeWar%377C883E0196.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sTimeWar;
      //## end ClDefaultConfiguration::m_sTimeWar%377C883E0196.role

      //## Association: M4Log::<unnamed>%377C883E0197
      //## Role: ClDefaultConfiguration::m_sTimeErr%377C883E0199
      //## begin ClDefaultConfiguration::m_sTimeErr%377C883E0199.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sTimeErr;
      //## end ClDefaultConfiguration::m_sTimeErr%377C883E0199.role

      //## Association: M4Log::<unnamed>%377C883E019A
      //## Role: ClDefaultConfiguration::m_sOpWar%377C883E019C
      //## begin ClDefaultConfiguration::m_sOpWar%377C883E019C.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sOpWar;
      //## end ClDefaultConfiguration::m_sOpWar%377C883E019C.role

      //## Association: M4Log::<unnamed>%377C883E019D
      //## Role: ClDefaultConfiguration::m_sOpDeb%377C883E019F
      //## begin ClDefaultConfiguration::m_sOpDeb%377C883E019F.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sOpDeb;
      //## end ClDefaultConfiguration::m_sOpDeb%377C883E019F.role

      //## Association: M4Log::<unnamed>%377C883E01A0
      //## Role: ClDefaultConfiguration::m_sOpErr%377C883E01A2
      //## begin ClDefaultConfiguration::m_sOpErr%377C883E01A2.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sOpErr;
      //## end ClDefaultConfiguration::m_sOpErr%377C883E01A2.role

      //## Association: M4Log::<unnamed>%377C883E0266
      //## Role: ClDefaultConfiguration::m_sBase%377C883E0268
      //## begin ClDefaultConfiguration::m_sBase%377C883E0268.role preserve=no  public: M4ClString {1 -> 1VHAPN}
      M4ClString m_sBase;
      //## end ClDefaultConfiguration::m_sBase%377C883E0268.role

      //## Association: M4Log::<unnamed>%377C883E0269
      //## Role: ClDefaultConfiguration::m_iDefault%377C883E026B
      //## begin ClDefaultConfiguration::m_iDefault%377C883E026B.role preserve=no  public: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iDefault;
      //## end ClDefaultConfiguration::m_iDefault%377C883E026B.role

      //## Association: M4Log::<unnamed>%377C883E026C
      //## Role: ClDefaultConfiguration::m_iPort%377C883E026E
      //## begin ClDefaultConfiguration::m_iPort%377C883E026E.role preserve=no  public: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_iPort;
      //## end ClDefaultConfiguration::m_iPort%377C883E026E.role

      //## Association: M4Log::<unnamed>%377C883E026F
      //## Role: ClDefaultConfiguration::m_iFileName%377C883E0271
      //## begin ClDefaultConfiguration::m_iFileName%377C883E0271.role preserve=no  public: m4pchar_t {1 -> 1VHAPN}
      m4pchar_t m_iFileName;
      //## end ClDefaultConfiguration::m_iFileName%377C883E0271.role

      //## Association: M4Log::<unnamed>%377C883E0272
      //## Role: ClDefaultConfiguration::m_lBufferSize%377C883E0274
      //## begin ClDefaultConfiguration::m_lBufferSize%377C883E0274.role preserve=no  public: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_lBufferSize;
      //## end ClDefaultConfiguration::m_lBufferSize%377C883E0274.role

      //## Association: M4Log::<unnamed>%377C883E0275
      //## Role: ClDefaultConfiguration::m_sType%377C883E0277
      //## begin ClDefaultConfiguration::m_sType%377C883E0277.role preserve=no  public: m4int8_t {1 -> 1VHAPN}
      m4int8_t m_sType;
      //## end ClDefaultConfiguration::m_sType%377C883E0277.role

      //## Association: M4Log::<unnamed>%377C883E0278
      //## Role: ClDefaultConfiguration::m_iStorages%377C883E027A
      //## begin ClDefaultConfiguration::m_iStorages%377C883E027A.role preserve=no  public: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iStorages;
      //## end ClDefaultConfiguration::m_iStorages%377C883E027A.role

      //## Association: M4Log::<unnamed>%377C883E027B
      //## Role: ClDefaultConfiguration::m_uiDefaultMaxQueueSize%377C883E027D
      //## begin ClDefaultConfiguration::m_uiDefaultMaxQueueSize%377C883E027D.role preserve=no  public: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_uiDefaultMaxQueueSize;
      //## end ClDefaultConfiguration::m_uiDefaultMaxQueueSize%377C883E027D.role

    // Additional Public Declarations
      //## begin ClDefaultConfiguration%377C883E012F.public preserve=yes
      //## end ClDefaultConfiguration%377C883E012F.public

  protected:
    //## Constructors (specified)
      //## Operation: ClDefaultConfiguration%-60473350; C++
      ClDefaultConfiguration ();

    // Additional Protected Declarations
      //## begin ClDefaultConfiguration%377C883E012F.protected preserve=yes
      //## end ClDefaultConfiguration%377C883E012F.protected

  private:
    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883E0280
      //## Role: ClDefaultConfiguration::myInstance%377C883E0282
      //## begin ClDefaultConfiguration::myInstance%377C883E0282.role preserve=no  private: static ClDefaultConfiguration {1 -> 1RHAPN}
      static ClDefaultConfiguration *myInstance;
      //## end ClDefaultConfiguration::myInstance%377C883E0282.role

    // Additional Private Declarations
      //## begin ClDefaultConfiguration%377C883E012F.private preserve=yes
      //## end ClDefaultConfiguration%377C883E012F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDefaultConfiguration%377C883E012F.implementation preserve=yes
      //## end ClDefaultConfiguration%377C883E012F.implementation

};

//## begin ClDefaultConfiguration%377C883E012F.postscript preserve=yes
//## end ClDefaultConfiguration%377C883E012F.postscript

//## begin STRQUEUE%377C883E0388.preface preserve=yes
//## end STRQUEUE%377C883E0388.preface

//## Class: STRQUEUE%377C883E0388; Instantiated Class
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: _Ty%377C883E03D0;M4ClString { -> F}

typedef deque< M4ClString > STRQUEUE;

//## begin STRQUEUE%377C883E0388.postscript preserve=yes
//## end STRQUEUE%377C883E0388.postscript

//## begin ClConfigLogs%377C883E0283.preface preserve=yes
//## end ClConfigLogs%377C883E0283.preface

//## Class: ClConfigLogs%377C883E0283
//## Category: M4Log%377C883D016A
//## Subsystem: M4Log::inc%377C883D0066
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377C883E0361;ClStaticResourceList { -> }
//## Uses: <unnamed>%377C883E039C;m4return_t { -> }
//## Uses: <unnamed>%377C883E039D;M4ClString { -> }
//## Uses: <unnamed>%377C883E03A4;m4int32_t { -> }
//## Uses: <unnamed>%377C883E03A6;m4int16_t { -> }
//## Uses: <unnamed>%377C883E03A7;m4char_t { -> }
//## Uses: <unnamed>%377C883E03A9;m4int64_t { -> }
//## Uses: <unnamed>%377C883E03BB;m4uint32_t { -> }
//## Uses: <unnamed>%377C883E03C2;ClStaticModuleList { -> }
//## Uses: <unnamed>%3782100F01B9;ClErrorTail { -> }

class M4_DECL_M4LOG ClConfigLogs 
{
  //## begin ClConfigLogs%377C883E0283.initialDeclarations preserve=yes
  //## end ClConfigLogs%377C883E0283.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClConfigLogs%-2092739115; C++
      ClConfigLogs ();

      //## Operation: ClConfigLogs%-857815257; C++
      ClConfigLogs (ClLogManager* plogManager, M4ClString siniFile, ClStaticResourceList* resources);

    //## Destructor (specified)
      //## Operation: ~ClConfigLogs%140338991; C++
      ~ClConfigLogs ();


    //## Other Operations (specified)
      //## Operation: SetIniFile%51879479; C++
      m4return_t SetIniFile (const M4ClString& siniFile);

      //## Operation: SetLogManager%419751567; C++
      m4return_t SetLogManager (ClLogManager* plogManager);

      //## Operation: Initialize%2020929601; C++
      m4return_t Initialize ();

      //## Operation: GetDefaultLevelInfo%148456083; C++
      m4int32_t GetDefaultLevelInfo ();

      //## Operation: DecodeNextMessage%-572112631; C++
      //	private:
      m4int16_t DecodeNextMessage (m4char_t*& ai_Iterator, m4int16_t* ao_level, m4int64_t* ao_Code);

      //## Operation: GetDefaultConfig%1730464281; C++
      m4return_t GetDefaultConfig ();

      //## Operation: GetOptions%-771835827; C++
      m4return_t GetOptions ();

      //## Operation: GetConfig%1853456006; C++
      m4return_t GetConfig ();

      //## Operation: GetStorages%-211260275; C++
      m4return_t GetStorages ();

      //## Operation: GetStorageFiles%1440062945; C++
      m4return_t GetStorageFile (const M4ClString& snstorage);

      //## Operation: GetStorageQueue%-412468965; C++
      m4return_t GetStorageQueue (const M4ClString& snstorage);

      //## Operation: GetModules%-1799102039; C++
      m4return_t GetModules ();

      //## Operation: GetFlags%-1324618017; C++
      m4return_t GetFlags ();

      //## Operation: GetOutputPatterns%365418924; C++
      m4return_t GetOutputPatterns ();

      //## Operation: LevelsAnd%390278744; C++
      m4uint32_t LevelsAnd (m4uint32_t ilevel);

      //## Operation: GetDebugResources%1690520073; C++
      m4return_t GetDebugResources ();

      //## Operation: DecodeMessage%-393270688; C++
      m4return_t DecodeMessage (const M4ClString& smessage, m4uint32_t& iflag);

      //## Operation: SetModuleList%-522544410; C++
      m4return_t SetModuleList (ClStaticModuleList* ai_ModuleList);

      //## Operation: updateConfig%-25023995; C++
      m4return_t updateConfig ();

      //## Operation: updateConfig%-118056103; C++
      m4return_t updateConfig (const M4ClString& ai_configFile);

      //## Operation: GetModuleList%-351719563; C++
      ClStaticModuleList* GetModuleList ()
      {
        //## begin ClConfigLogs::GetModuleList%-351719563.body preserve=yes
			return m_pModuleList;
        //## end ClConfigLogs::GetModuleList%-351719563.body
      }

    // Additional Public Declarations
      //## begin ClConfigLogs%377C883E0283.public preserve=yes
      //## end ClConfigLogs%377C883E0283.public

  protected:
    // Additional Protected Declarations
      //## begin ClConfigLogs%377C883E0283.protected preserve=yes
      //## end ClConfigLogs%377C883E0283.protected

  private:

      //## Operation: GetEndMessageChar%1149524335; C++
      m4char_t GetEndMessageChar ();

      //## Operation: compareValues%933951072
      m4int32_t compareValues (m4pcchar_t ai_Value, m4pcchar_t ai_Default);

    // Data Members for Class Attributes

      //## Attribute: m_aiflags%385F6A7F035A
      //## begin ClConfigLogs::m_aiflags%385F6A7F035A.attr preserve=no  private: m4uint32_t [MAX_DEVICES_GROUP] {UA} 
      m4uint32_t  m_aiflags[MAX_DEVICES_GROUP];
      //## end ClConfigLogs::m_aiflags%385F6A7F035A.attr

      //## Attribute: m_mflags%385F6AD802AE
      //## begin ClConfigLogs::m_mflags%385F6AD802AE.attr preserve=no  private: m4uint32_t [MAX_DEVICES_GROUP] {UA} 
      m4uint32_t  m_mflags[MAX_DEVICES_GROUP];
      //## end ClConfigLogs::m_mflags%385F6AD802AE.attr

    // Data Members for Associations

      //## Association: M4Log::<unnamed>%377C883E0367
      //## Role: ClConfigLogs::m_iStorages%377C883E0369
      //## begin ClConfigLogs::m_iStorages%377C883E0369.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iStorages;
      //## end ClConfigLogs::m_iStorages%377C883E0369.role

      //## Association: M4Log::<unnamed>%377C883E036A
      //## Role: ClConfigLogs::m_oDefaultConfig%377C883E036C
      //## begin ClConfigLogs::m_oDefaultConfig%377C883E036C.role preserve=no  private: ClDefaultConfiguration {1 -> 1RHAPN}
      ClDefaultConfiguration *m_oDefaultConfig;
      //## end ClConfigLogs::m_oDefaultConfig%377C883E036C.role

      //## Association: M4Log::<unnamed>%377C883E036D
      //## Role: ClConfigLogs::m_showTime%377C883E036F
      //## begin ClConfigLogs::m_showTime%377C883E036F.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_showTime;
      //## end ClConfigLogs::m_showTime%377C883E036F.role

      //## Association: M4Log::<unnamed>%377C883E0370
      //## Role: ClConfigLogs::m_version%377C883E0372
      //## begin ClConfigLogs::m_version%377C883E0372.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_version;
      //## end ClConfigLogs::m_version%377C883E0372.role

      //## Association: M4Log::<unnamed>%377C883E0373
      //## Role: ClConfigLogs::m_iwarning%377C883E0375
      //## begin ClConfigLogs::m_iwarning%377C883E0375.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_iwarning;
      //## end ClConfigLogs::m_iwarning%377C883E0375.role

      //## Association: M4Log::<unnamed>%377C883E0376
      //## Role: ClConfigLogs::m_ierror%377C883E0378
      //## begin ClConfigLogs::m_ierror%377C883E0378.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_ierror;
      //## end ClConfigLogs::m_ierror%377C883E0378.role

      //## Association: M4Log::<unnamed>%377C883E0379
      //## Role: ClConfigLogs::m_idebugInfo%377C883E037B
      //## begin ClConfigLogs::m_idebugInfo%377C883E037B.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_idebugInfo;
      //## end ClConfigLogs::m_idebugInfo%377C883E037B.role

      //## Association: M4Log::<unnamed>%377C883E037C
      //## Role: ClConfigLogs::m_sbase%377C883E037E
      //## begin ClConfigLogs::m_sbase%377C883E037E.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_sbase;
      //## end ClConfigLogs::m_sbase%377C883E037E.role

      //## Association: M4Log::<unnamed>%377C883E0385
      //## Role: ClConfigLogs::m_aiDefaultFlag%377C883E0387
      //## begin ClConfigLogs::m_aiDefaultFlag%377C883E0387.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_aiDefaultFlag;
      //## end ClConfigLogs::m_aiDefaultFlag%377C883E0387.role

      //## Association: M4Log::<unnamed>%377C883E038A
      //## Role: ClConfigLogs::m_qmodules%377C883E038C
      //## begin ClConfigLogs::m_qmodules%377C883E038C.role preserve=no  private: STRQUEUE {1 -> 1VHAPN}
      STRQUEUE m_qmodules;
      //## end ClConfigLogs::m_qmodules%377C883E038C.role

      //## Association: M4Log::<unnamed>%377C883E038D
      //## Role: ClConfigLogs::m_siniFile%377C883E038F
      //	INT32MAP	m_mflags;
      //## begin ClConfigLogs::m_siniFile%377C883E038F.role preserve=no  private: M4ClString {1 -> 1VHAPN}
      M4ClString m_siniFile;
      //## end ClConfigLogs::m_siniFile%377C883E038F.role

      //## Association: M4Log::<unnamed>%377C883E0390
      //## Role: ClConfigLogs::m_numofModules%377C883E0392
      //## begin ClConfigLogs::m_numofModules%377C883E0392.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_numofModules;
      //## end ClConfigLogs::m_numofModules%377C883E0392.role

      //## Association: M4Log::<unnamed>%377C883E0394
      //## Role: ClConfigLogs::m_pModuleList%377C883E0396
      //## begin ClConfigLogs::m_pModuleList%377C883E0396.role preserve=no  private: ClStaticModuleList {1 -> 1RHAPN}
      ClStaticModuleList *m_pModuleList;
      //## end ClConfigLogs::m_pModuleList%377C883E0396.role

      //## Association: M4Log::<unnamed>%377C883E0397
      //## Role: ClConfigLogs::m_plogManager%377C883E0399
      //## begin ClConfigLogs::m_plogManager%377C883E0399.role preserve=no  private: ClLogManager {1 -> 1RFHAPN}
      ClLogManager *m_plogManager;
      //## end ClConfigLogs::m_plogManager%377C883E0399.role

    // Additional Private Declarations
      //## begin ClConfigLogs%377C883E0283.private preserve=yes
      //## end ClConfigLogs%377C883E0283.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClConfigLogs%377C883E0283.implementation preserve=yes
      m4bool_t m_bModuleListOwner;
      //## end ClConfigLogs%377C883E0283.implementation

};

//## begin ClConfigLogs%377C883E0283.postscript preserve=yes
//## end ClConfigLogs%377C883E0283.postscript

//## begin module%377C883E00AD.epilog preserve=yes
//## end module%377C883E00AD.epilog


#endif
