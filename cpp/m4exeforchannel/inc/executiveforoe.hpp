//## begin module%38F316530126.cm preserve=no
//## end module%38F316530126.cm

//## begin module%38F316530126.cp preserve=no
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
//## end module%38F316530126.cp

//## Module: executiveforoe%38F316530126; Subprogram specification
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Source file: e:\mybuild\m4exeforchannel\inc\executiveforoe.hpp

#ifndef executiveforoe_h
#define executiveforoe_h 1

//## begin module%38F316530126.additionalIncludes preserve=no
//## end module%38F316530126.additionalIncludes

//## begin module%38F316530126.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
//## end module%38F316530126.includes

// Executive
#include <executive.hpp>
// m4string
#include <m4string.hpp>
// ExecDumpInfo
#include <execdumpinfo.hpp>

#include "request.hpp"
#include "execlogs.hpp"

class ClNavigatorFirewall;
class ClConfiguration;
class ClChannelManager;
class ClM4ObjService;

//## begin module%38F316530126.declarations preserve=no
//## end module%38F316530126.declarations

//## begin module%38F316530126.additionalDeclarations preserve=yes
//## end module%38F316530126.additionalDeclarations


//## begin ClExecutiveForOE%38C8D8420200.preface preserve=yes
//## end ClExecutiveForOE%38C8D8420200.preface

//## Class: ClExecutiveForOE%38C8D8420200; Abstract
//## Category: M4ExeForChannel%3507BDA601C6
//## Subsystem: M4ExeForChannel::inc%379C5C480310
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38CD10D20175;ClMetaDataSubsystem { -> F}
//## Uses: <unnamed>%38CD117200DF;ClConfiguration { -> F}
//## Uses: <unnamed>%38CD130E02EA;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%38C8DFFC0377;ofstream { -> F}
//## Uses: <unnamed>%38CE33A003A3;ClChannel { -> F}
//## Uses: <unnamed>%38F5ECBA03CE;ClLConn_Interface { -> F}
//## Uses: <unnamed>%38F5F0F50230;ClCMObjectDirectory { -> F}
//## Uses: <unnamed>%38F731BF0077;ClOblConfiguration { -> F}
//## Uses: <unnamed>%391059350096;ClAdminTag { -> F}
//## Uses: <unnamed>%391059480292;ClStatus { -> F}
//## Uses: <unnamed>%392AB29E017F;ClMutBlock { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForOE : public ClExecutive  //## Inherits: <unnamed>%38C8E34A03D8
{
  //## begin ClExecutiveForOE%38C8D8420200.initialDeclarations preserve=yes
  //## end ClExecutiveForOE%38C8D8420200.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: StartUp%952690403
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: ShutDown%952690405
      m4return_t ShutDown ();

      //## Operation: Reset%952690406; C++
      m4return_t Reset ();

      //## Operation: DumpLevel%952690407; C++
      m4return_t DumpLevel (const m4int16_t ai_iLevel);

      //## Operation: GetNDumpLevels%952963879
      m4int16_t GetNDumpLevels () const;

      //## Operation: ReleaseResources%952690408; C++
      m4return_t ReleaseResources ();

      //## Operation: SetDumpFile%955635477
      void SetDumpFile (m4pchar_t ai_pcDumpFile);

      //## Operation: ExecuteCommand%957346241
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

	  static ClCriticalSection& GetExecuteMutex();

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%38F485FB024E
      //## Role: ClExecutiveForOE::m_poDumpInfo%38F485FC0232
      //## begin ClExecutiveForOE::m_poDumpInfo%38F485FC0232.role preserve=no  public: ExecDumpInfo { -> 1RHAN}
      ExecDumpInfo *m_poDumpInfo;
      //## end ClExecutiveForOE::m_poDumpInfo%38F485FC0232.role

    // Additional Public Declarations
      //## begin ClExecutiveForOE%38C8D8420200.public preserve=yes
      //## end ClExecutiveForOE%38C8D8420200.public

  protected:
    //## Constructors (specified)
      //## Operation: ClExecutiveForOE%952963880
      ClExecutiveForOE (ClEngine *ai_pEngine, ClStatistic *ai_poStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForOE%952963881
      ~ClExecutiveForOE ();


    //## Other Operations (specified)
      //## Operation: _DumpHeader%953028446
      void _DumpHeader (ostream &ai_oOstream);

      //## Operation: _DumpLevel%953028447
      void _DumpLevel (m4int16_t ai_iLevel, ostream &ai_oOstream);

      //## Operation: RecycleDumpInfo%1005730073
      void RecycleDumpInfo ();

	  m4return_t GetSessionValue( ClChannel* ai_pSessionCh, m4pchar_t ai_pcItem, void* &ao_pVal );

	  m4return_t setTracesExtInfo( ClRequestStatistics* ai_pStats, ClChannel* ai_pSessionCh, m4pcchar_t ai_pcHost, m4pcchar_t ai_pcPort, m4pcchar_t ai_pcSession, m4pcchar_t ai_pcReqUniqueId, eLogFile_t ai_iLogType );

	  m4return_t resetTracesExtInfo( eLogFile_t ai_iLogType );

	  m4return_t rowsQuotaProlog(ClChannel* ai_pSessionCh, ClUSSessionTicketInterface* ai_pSessionTck, m4bool_t ai_bIsOLTP);

	  void rowsQuotaEpilog(ClRequestStatistics* ai_pReqStats, ClUSSessionTicketInterface* ai_pSessionTck);

    // Data Members for Class Attributes

      //## Attribute: m_iMaxDumpLevel%38CD1B230173
      //## begin ClExecutiveForOE::m_iMaxDumpLevel%38CD1B230173.attr preserve=no  protected: m4size_t {UA} 0
      m4size_t m_iMaxDumpLevel;
      //## end ClExecutiveForOE::m_iMaxDumpLevel%38CD1B230173.attr

      //## Attribute: M4ExecutiveDumpFilenameProp%3912BEBF0153
      //## begin ClExecutiveForOE::M4ExecutiveDumpFilenameProp%3912BEBF0153.attr preserve=no  protected: static m4pchar_t {VAC} "DUMP_FILE"
      static const m4pchar_t  M4ExecutiveDumpFilenameProp;
      //## end ClExecutiveForOE::M4ExecutiveDumpFilenameProp%3912BEBF0153.attr

      //## Attribute: M4ExecutiveStringIdProp%3912BF000265
      //## begin ClExecutiveForOE::M4ExecutiveStringIdProp%3912BF000265.attr preserve=no  protected: static m4pchar_t {UAC} "STRINGID"
      static const m4pchar_t  M4ExecutiveStringIdProp;
      //## end ClExecutiveForOE::M4ExecutiveStringIdProp%3912BF000265.attr

      //## Attribute: M4ExecutiveDataCacheProp%3912BF010343
      //## begin ClExecutiveForOE::M4ExecutiveDataCacheProp%3912BF010343.attr preserve=no  protected: static m4pchar_t {UAC} "DATA_CACHE"
      static const m4pchar_t  M4ExecutiveDataCacheProp;
      //## end ClExecutiveForOE::M4ExecutiveDataCacheProp%3912BF010343.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%38CD1D59038A
      //## Role: ClExecutiveForOE::m_strStringId%38CD1D5B031E
      //## begin ClExecutiveForOE::m_strStringId%38CD1D5B031E.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strStringId;
      //## end ClExecutiveForOE::m_strStringId%38CD1D5B031E.role

      //## Association: M4ExeForChannel::<unnamed>%38C8DB8A00E6
      //## Role: ClExecutiveForOE::m_strDumpFilename%38C8DB8A02EF
      //## begin ClExecutiveForOE::m_strDumpFilename%38C8DB8A02EF.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strDumpFilename;
      //## end ClExecutiveForOE::m_strDumpFilename%38C8DB8A02EF.role

      //## Association: M4ExeForChannel::<unnamed>%38C8FA7F0394
      //## Role: ClExecutiveForOE::m_pChannelManager%38C8FA7F0396
      //## begin ClExecutiveForOE::m_pChannelManager%38C8FA7F0396.role preserve=no  protected: ClChannelManager {1 -> 1RFHAN}
      ClChannelManager *m_pChannelManager;
      //## end ClExecutiveForOE::m_pChannelManager%38C8FA7F0396.role

      //## Association: M4ExeForChannel::<unnamed>%38F48B1E00D3
      //## Role: ClExecutiveForOE::m_pObjectService%38F48B1E039A
      //## begin ClExecutiveForOE::m_pObjectService%38F48B1E039A.role preserve=no  protected: ClM4ObjService { -> 1RFHAN}
      ClM4ObjService *m_pObjectService;
      //## end ClExecutiveForOE::m_pObjectService%38F48B1E039A.role

    // Additional Protected Declarations
      //## begin ClExecutiveForOE%38C8D8420200.protected preserve=yes
      //## end ClExecutiveForOE%38C8D8420200.protected

  private:
    //## Constructors (specified)
      //## Operation: ClExecutiveForOE%952963882
      ClExecutiveForOE ();

    // Additional Private Declarations
      //## begin ClExecutiveForOE%38C8D8420200.private preserve=yes
      //## end ClExecutiveForOE%38C8D8420200.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForOE%38C8D8420200.implementation preserve=yes
      //## end ClExecutiveForOE%38C8D8420200.implementation

};

//## begin ClExecutiveForOE%38C8D8420200.postscript preserve=yes
//## end ClExecutiveForOE%38C8D8420200.postscript

// Class ClExecutiveForOE 


//## Other Operations (inline)
inline m4int16_t ClExecutiveForOE::GetNDumpLevels () const
{
  //## begin ClExecutiveForOE::GetNDumpLevels%952963879.body preserve=yes
	return m_iMaxDumpLevel;
  //## end ClExecutiveForOE::GetNDumpLevels%952963879.body
}

inline void ClExecutiveForOE::SetDumpFile (m4pchar_t ai_pcDumpFile)
{
  //## begin ClExecutiveForOE::SetDumpFile%955635477.body preserve=yes
	m_strDumpFilename = ai_pcDumpFile;
  //## end ClExecutiveForOE::SetDumpFile%955635477.body
}

// exports the session mutex
inline ClCriticalSection& ClExecutiveForOE::GetExecuteMutex() 
{
	return m_oMut;
}

//## begin module%38F316530126.epilog preserve=yes
//## end module%38F316530126.epilog


#endif
