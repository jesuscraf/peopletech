//## begin module%3923BC03012B.cm preserve=no
//## end module%3923BC03012B.cm

//## begin module%3923BC03012B.cp preserve=no
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
//## end module%3923BC03012B.cp

//## Module: SiteNodeInfo%3923BC03012B; Subprogram specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: z:\m4dispatcher\inc\sitenodeinfo.hpp

#ifndef sitenodeinfo_h
#define sitenodeinfo_h 1

//## begin module%3923BC03012B.additionalIncludes preserve=no
//## end module%3923BC03012B.additionalIncludes


#include "m4stl.hpp"
// snmpinterfaces
#include <snmpinterfaces.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// CriticalSection
#include <criticalsection.hpp>
// m4date
#include <m4date.hpp>
#include "m4clock.hpp"
//## begin module%3923BC03012B.declarations preserve=no
//## end module%3923BC03012B.declarations

//## begin module%3923BC03012B.additionalDeclarations preserve=yes
m4pchar_t ToLower (m4pchar_t aio_string) ;



#define STATE_READY			"READY"
#define STATE_STARTING		"STARTING"
#define STATE_SHUTING_DOWN	"SHUTTING DOWN"
#define STATE_SHUTING_DOWN_MC	"SHUTTING DOWN/MEMORY"
#define STATE_HALTED		"HALTED"
#define STATE_INACTIVE		"INACTIVE"

#define STOP_MEMORY			1
#define STOP_LDB			2
#define STOP_ADMIN			3
#define STOP_GENERAL		4

typedef enum {JAVA_CLIENT = 1, CPP_CLIENT, BOTH_CLIENTS} eDspClientTypes;

//## end module%3923BC03012B.additionalDeclarations


//## begin ClSiteNodeInformation%3923B60D0267.preface preserve=yes
//## end ClSiteNodeInformation%3923B60D0267.preface

//## Class: ClSiteNodeInformation%3923B60D0267
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSiteNodeInformation 
{
  //## begin ClSiteNodeInformation%3923B60D0267.initialDeclarations preserve=yes
  //## end ClSiteNodeInformation%3923B60D0267.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSiteNodeInformation%958658601
      ClSiteNodeInformation (const m4string_t &ai_strSiteNodeID, const m4string_t ai_strHost, const m4uint32_t ai_iBasePort, const m4string_t ai_strUserName, const m4string_t ai_strPassword, const m4string_t ai_szState = STATE_STARTING, const m4double_t  ai_dCPUUsage = 0, const m4uint32_t ai_iNumSessions = 0, const m4uint32_t ai_dServerMemory = 0, const m4double_t ai_dSystemMemory = 0, const m4uint32_t ai_iConnectionID = 0, m4double_t ai_dPrivateMB = 0, m4double_t ai_dMaxProcSize = 0, m4double_t ai_dFreeMem = 0);

      //## Operation: ClSiteNodeInformation%959009686
      ClSiteNodeInformation (const ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: ClSiteNodeInformation%958658602
      ClSiteNodeInformation ();


    //## Other Operations (specified)
      //## Operation: GetSiteNodeID%958643329
      const m4string_t & GetSiteNodeID () const;

      //## Operation: UpdateInformation%958643328
      m4return_t UpdateInformation (const ClSiteNodeInformation &ai_oNodeInformation);

      //## Operation: operator=%959009687
      ClSiteNodeInformation & operator = (const ClSiteNodeInformation &ai_oRHSNode);

      //## Operation: GetNodeHost%961054362
      const m4string_t & GetNodeHost () const;

      //## Operation: GetBasePort%961054363
      const m4uint32_t GetBasePort () const;

      //## Operation: GetNodeState%961054364
      const m4string_t GetNodeState () const;

      //## Operation: SetNodeState%962263774
      m4void_t SetNodeState (m4pchar_t ai_szNodeState);

      //## Operation: IsAlive%963248410
      m4bool_t IsAlive (m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iServerTimeout);

      //## Operation: Start%963302394
      m4void_t Start ();

      //## Operation: GetConnectionID%969372804
      const m4uint32_t GetConnectionID () const;

      //## Operation: GetUserName%969528445
      const m4string_t & GetUserName () const;

	  m4void_t SetUserName( m4string_t ai_szUserName );

      //## Operation: GetPassword%969528446
      const m4string_t & GetPassword () const;

	  m4void_t SetPassword( m4string_t ai_szPassword );

	  const m4double_t GetCppLoadFactor() const;
	  const m4double_t GetJavaLoadFactor() const;

      //## Operation: GetServerMemory%972317054
      const m4uint32_t GetServerMemory () const;

      //## Operation: SetServerMemory%972317055
      m4void_t SetServerMemory (m4uint32_t ai_iServerMemory);

      //## Operation: GetSystemMemory%972317056
      const m4double_t GetSystemMemory () const;

      //## Operation: SetSystemMemory%972317057
      m4void_t SetSystemMemory (m4double_t ai_dSystemMemory);

      //## Operation: GetCPUUsage%972317058
      const m4double_t GetCPUUsage () const;

      //## Operation: SetCPUUsage%972317059
      m4void_t SetCPUUsage (m4double_t ai_dCPUUsage);

      //## Operation: GetNumSessions%972317060
      m4uint32_t GetNumSessions () const;

      //## Operation: SetNumSessions%972317061
      m4void_t SetNumSessions (m4uint32_t ai_iNumSessions);

      //## Operation: CalculateLoad%972387803
      m4return_t CalculateLoad (eDspClientTypes ai_iType);

      //## Operation: SetPersistable%972463869
      m4void_t SetPersistable (m4bool_t ai_bPersistable);

      //## Operation: IsPersistable%972463870
      m4bool_t IsPersistable () const;

      //## Operation: GetProperty%1067431241
      m4return_t GetProperty (const m4string_t& ai_szProperty, m4string_t &ai_szValue);

      //## Operation: GetLocalHost%1070448861
      const m4string_t & GetLocalHost () const;

      //## Operation: GetSnmpObject%1084868904
      M4ISnmpObject* GetSnmpObject ();

      //## Operation: SetSnmpObject%1084868905
      m4void_t SetSnmpObject (M4ISnmpObject* ai_poSnmpObject);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: IsEnabled%40B2200402FB
      const m4bool_t& GetIsEnabled () const;
      void SetIsEnabled (const m4bool_t& value);

      const m4string_t GetSiteNodeMapKey () const;

      const m4double_t GetPrivateMB () const;
      m4void_t SetPrivateMB (m4double_t ai_dPrivateMB);

      const m4double_t GetMaxProcSize () const;
      m4void_t SetMaxProcSize (m4double_t ai_dMaxProcSize);
      
	  const m4double_t GetFreeMem () const;
      m4void_t SetFreeMem (m4double_t ai_dFreeMem);

	  const m4bool_t GetDiscarded() const;
	  m4void_t SetDiscarded(m4bool_t ai_bDiscarded);

	  const m4millisec_t GetOLTPTime() const;
	  m4void_t SetOLTPTime (m4millisec_t ai_iOLTPTime);

	  const m4millisec_t GetXMLTime() const;
	  m4void_t SetXMLTime (m4millisec_t ai_iXMLTime);


    // Data Members for Class Attributes

      //## Attribute: M4NullSite%40B227830272
      //## begin ClSiteNodeInformation::M4NullSite%40B227830272.attr preserve=no  public: static ClSiteNodeInformation {UA} 
      static ClSiteNodeInformation M4NullSite;
      //## end ClSiteNodeInformation::M4NullSite%40B227830272.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%39C773090338
      //## Role: ClSiteNodeInformation::m_iConnectionID%39C7730A0307
      //## begin ClSiteNodeInformation::m_iConnectionID%39C7730A0307.role preserve=no  public: m4uint32_t { -> VHAN}
      m4uint32_t m_iConnectionID;
      //## end ClSiteNodeInformation::m_iConnectionID%39C7730A0307.role

      //## Association: M4Dispatcher::<unnamed>%39CA026A0097
      //## Role: ClSiteNodeInformation::m_strUserName%39CA026B0020
      //## begin ClSiteNodeInformation::m_strUserName%39CA026B0020.role preserve=no  public: m4string_t { -> VHAN}
      m4string_t m_strUserName;
      //## end ClSiteNodeInformation::m_strUserName%39CA026B0020.role

      //## Association: M4Dispatcher::<unnamed>%39CA0566001E
      //## Role: ClSiteNodeInformation::m_strPassword%39CA05670033
      //## begin ClSiteNodeInformation::m_strPassword%39CA05670033.role preserve=no  public: m4string_t { -> VHAN}
      m4string_t m_strPassword;
      //## end ClSiteNodeInformation::m_strPassword%39CA05670033.role

    // Additional Public Declarations
      //## begin ClSiteNodeInformation%3923B60D0267.public preserve=yes
	  	  m4uint32_t m_iStopReason ;
      //## end ClSiteNodeInformation%3923B60D0267.public

  protected:
    // Data Members for Class Attributes

      //## begin ClSiteNodeInformation::IsEnabled%40B2200402FB.attr preserve=no  public: m4bool_t {U} M4_TRUE
      m4bool_t m_bIsEnabled;
      //## end ClSiteNodeInformation::IsEnabled%40B2200402FB.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%3923BA320371
      //## Role: ClSiteNodeInformation::m_strSiteNodeID%3923BA3301BB
      //## begin ClSiteNodeInformation::m_strSiteNodeID%3923BA3301BB.role preserve=no  protected: m4string_t { -> 1VHAN}
      m4string_t m_strSiteNodeID;
      //## end ClSiteNodeInformation::m_strSiteNodeID%3923BA3301BB.role

      //## Association: M4Dispatcher::<unnamed>%396A00B60025
      //## Role: ClSiteNodeInformation::m_tLastUpdate%396A00B60274
      //## begin ClSiteNodeInformation::m_tLastUpdate%396A00B60274.role preserve=no  protected: m4time_t { -> VHAN}
      m4time_t m_tLastUpdate;
      //## end ClSiteNodeInformation::m_tLastUpdate%396A00B60274.role

      //## Association: M4Dispatcher::<unnamed>%40B2237201DD
      //## Role: ClSiteNodeInformation::m_pM4ISnmpObject%40B2237203B1
      //## begin ClSiteNodeInformation::m_pM4ISnmpObject%40B2237203B1.role preserve=no  protected: M4ISnmpObject { -> RHAN}
      M4ISnmpObject *m_pM4ISnmpObject;
      //## end ClSiteNodeInformation::m_pM4ISnmpObject%40B2237203B1.role

    // Additional Protected Declarations
      //## begin ClSiteNodeInformation%3923B60D0267.protected preserve=yes
      //## end ClSiteNodeInformation%3923B60D0267.protected

  private:
    // Data Members for Class Attributes

	  m4double_t m_dCppLoadFactor;

	  m4double_t m_dJavaLoadFactor;

      //## Attribute: m_dCPU%39F460B800E8
      //## begin ClSiteNodeInformation::m_dCPU%39F460B800E8.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_dCPU;
      //## end ClSiteNodeInformation::m_dCPU%39F460B800E8.attr

      //## Attribute: m_dSystemMemory%39F460C80131
      //## begin ClSiteNodeInformation::m_dSystemMemory%39F460C80131.attr preserve=no  private: m4double_t {UA} 
      m4double_t m_dSystemMemory;
      //## end ClSiteNodeInformation::m_dSystemMemory%39F460C80131.attr

      //## Attribute: m_iNumSessions%39F460D90208
      //## begin ClSiteNodeInformation::m_iNumSessions%39F460D90208.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iNumSessions;
      //## end ClSiteNodeInformation::m_iNumSessions%39F460D90208.attr

      //## Attribute: m_iServerMemory%39F460EC0382
      //## begin ClSiteNodeInformation::m_iServerMemory%39F460EC0382.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iServerMemory;
      //## end ClSiteNodeInformation::m_iServerMemory%39F460EC0382.attr

      //## Attribute: m_bPersistable%39F6F03D0209
      //## begin ClSiteNodeInformation::m_bPersistable%39F6F03D0209.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bPersistable;
      //## end ClSiteNodeInformation::m_bPersistable%39F6F03D0209.attr

      //## Attribute: m_iStartingtries%3B9DCBC501EF
      //## begin ClSiteNodeInformation::m_iStartingtries%3B9DCBC501EF.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iStartingtries;
      //## end ClSiteNodeInformation::m_iStartingtries%3B9DCBC501EF.attr

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%3948ED940176
      //## Role: ClSiteNodeInformation::m_strHost%3948ED950023
      //## begin ClSiteNodeInformation::m_strHost%3948ED950023.role preserve=no  private: m4string_t { -> VHAN}
      m4string_t m_strHost;
      //## end ClSiteNodeInformation::m_strHost%3948ED950023.role

      //## Association: M4Dispatcher::<unnamed>%3948EE6101CB
      //## Role: ClSiteNodeInformation::m_iBasePort%3948EE6103B6
      //## begin ClSiteNodeInformation::m_iBasePort%3948EE6103B6.role preserve=no  private: m4uint32_t { -> VHAN}
      m4uint32_t m_iBasePort;
      //## end ClSiteNodeInformation::m_iBasePort%3948EE6103B6.role

      //## Association: M4Dispatcher::<unnamed>%3948EF2D00B6
      //## Role: ClSiteNodeInformation::m_strNodeState%3948EF2D02D3
      //## begin ClSiteNodeInformation::m_strNodeState%3948EF2D02D3.role preserve=no  private: m4string_t { -> VHAN}
      m4string_t m_strNodeState;
      //## end ClSiteNodeInformation::m_strNodeState%3948EF2D02D3.role

      //## Association: M4Dispatcher::Server Repository::<unnamed>%3FCDC32C0137
      //## Role: ClSiteNodeInformation::m_strLocalHost%3FCDC32D002A
      //## begin ClSiteNodeInformation::m_strLocalHost%3FCDC32D002A.role preserve=no  private: m4string_t { -> VHAN}
      m4string_t m_strLocalHost;
      //## end ClSiteNodeInformation::m_strLocalHost%3FCDC32D002A.role

	  m4double_t m_dPrivateMB;
	  m4double_t m_dMaxProcSize;
	  m4double_t m_dFreeMem;

	  m4bool_t	m_bDiscarded ;

	  m4millisec_t m_iOLTPTime;
	  m4millisec_t m_iXMLTime;

    // Additional Private Declarations
      //## begin ClSiteNodeInformation%3923B60D0267.private preserve=yes
      //## end ClSiteNodeInformation%3923B60D0267.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSiteNodeInformation%3923B60D0267.implementation preserve=yes
      //## end ClSiteNodeInformation%3923B60D0267.implementation

};

//## begin ClSiteNodeInformation%3923B60D0267.postscript preserve=yes
//## end ClSiteNodeInformation%3923B60D0267.postscript

//## begin ClSiteNodeList%3948C1BA02F1.preface preserve=yes
//## end ClSiteNodeList%3948C1BA02F1.preface

//## Class: ClSiteNodeList%3948C1BA02F1
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3948D7F4036B;ClSiteNodeInformation { -> }
//## Uses: <unnamed>%3948DFC60098;list { -> }

typedef list < ClSiteNodeInformation>  ClSiteNodeList;

//## begin ClSiteNodeList%3948C1BA02F1.postscript preserve=yes
//## end ClSiteNodeList%3948C1BA02F1.postscript

//## begin ClSiteNodeContainer%3948D8AE0368.preface preserve=yes
//## end ClSiteNodeContainer%3948D8AE0368.preface

//## Class: ClSiteNodeContainer%3948D8AE0368
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSiteNodeContainer 
{
  //## begin ClSiteNodeContainer%3948D8AE0368.initialDeclarations preserve=yes
  //## end ClSiteNodeContainer%3948D8AE0368.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: AddNode%961054356
      m4void_t AddNode (ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: GetNextNode%961054357
      m4return_t GetNextNode (ClSiteNodeInformation &ao_oSiteNode);

      //## Operation: FindNodeConnectionID%969372805
      m4bool_t FindNodeConnectionID (m4uint32_t ai_iConnectionID, ClSiteNodeInformation &ao_oSiteNode);

    // Additional Public Declarations
      //## begin ClSiteNodeContainer%3948D8AE0368.public preserve=yes
      //## end ClSiteNodeContainer%3948D8AE0368.public

  protected:
    // Additional Protected Declarations
      //## begin ClSiteNodeContainer%3948D8AE0368.protected preserve=yes
      //## end ClSiteNodeContainer%3948D8AE0368.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%3948D8C100E4
      //## Role: ClSiteNodeContainer::m_oSiteNodeList%3948D8C103AB
      //## begin ClSiteNodeContainer::m_oSiteNodeList%3948D8C103AB.role preserve=no  private: ClSiteNodeList { -> VHAN}
      ClSiteNodeList m_oSiteNodeList;
      //## end ClSiteNodeContainer::m_oSiteNodeList%3948D8C103AB.role

    // Additional Private Declarations
      //## begin ClSiteNodeContainer%3948D8AE0368.private preserve=yes
      //## end ClSiteNodeContainer%3948D8AE0368.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSiteNodeContainer%3948D8AE0368.implementation preserve=yes
      //## end ClSiteNodeContainer%3948D8AE0368.implementation

};

//## begin ClSiteNodeContainer%3948D8AE0368.postscript preserve=yes
//## end ClSiteNodeContainer%3948D8AE0368.postscript

//## begin TSiteNodeMap%3923BAB10305.preface preserve=yes
//## end TSiteNodeMap%3923BAB10305.preface

//## Class: TSiteNodeMap%3923BAB10305; Instantiated Class
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3923BACC03B8;ClSiteNodeInformation { -> }

typedef map< m4string_t,ClSiteNodeInformation,less <m4string_t>  > TSiteNodeMap;

//## begin TSiteNodeMap%3923BAB10305.postscript preserve=yes
//## end TSiteNodeMap%3923BAB10305.postscript

//## begin ClSiteNodeRepository%3923B6990240.preface preserve=yes
//## end ClSiteNodeRepository%3923B6990240.preface

//## Class: ClSiteNodeRepository%3923B6990240
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3948D8EA011F;ClSiteNodeContainer { -> }

class ClSiteNodeRepository 
{
  //## begin ClSiteNodeRepository%3923B6990240.initialDeclarations preserve=yes
  //## end ClSiteNodeRepository%3923B6990240.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSiteNodeRepository%958658604
      ClSiteNodeRepository ();


    //## Other Operations (specified)
      //## Operation: GetCandidateNode%958643330
      m4return_t GetCandidateNode (ClSiteNodeInformation &ao_oSiteNode, eDspClientTypes ai_iClientType);

      //## Operation: AddNode%958643331
      m4return_t AddNode (const ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: UpdateNode%958643332
      m4return_t UpdateNode (ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: RemoveNode%958658603
      m4return_t RemoveNode (ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: Count%958658605
      m4size_t Count ();

      //## Operation: Clear%958658606
      void Clear ();

      //## Operation: GetNodeList%961054358
      m4return_t GetNodeList (ClSiteNodeContainer &ai_oSiteNodeList);

      //## Operation: IsConsistent%963302387
      m4bool_t IsConsistent (ClSiteNodeContainer &ao_oRemovableNodes);

      //## Operation: RemoveNodeList%963302388
      m4return_t RemoveNodeList (ClSiteNodeContainer &ai_oNodeList);

      //## Operation: GetNodeInfo%965823269
      m4return_t GetNodeInfo (ClSiteNodeInformation &aio_oSiteNode);

      //## Operation: GetAdminPort%974309622
      m4uint32_t GetAdminPort ();

      //## Operation: SetParams%974309623
      void SetParams (m4uint32_t ai_uiAdminPort, m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iServerTimeout, m4uint32_t ai_iDumpLoadTrace, m4uint32_t ai_iSessionsWeight, m4uint32_t ai_iSrvMemWeight, m4uint32_t ai_iSysMemWeight, m4uint32_t ai_iCPUWeight, m4uint32_t ai_iExecTimeWeight, m4uint32_t ai_iExecTimeThreshold);

	  m4return_t EraseNode ( ClSiteNodeInformation &ai_oSiteNode );

	  m4return_t UpdateDiscardedNode(ClSiteNodeInformation &ai_oSiteNode);

	  m4return_t UpdateOLTPTimeNode(ClSiteNodeInformation &ai_oSiteNode);

	  m4return_t UpdateXMLTimeNode(ClSiteNodeInformation &ai_oSiteNode);

	  m4uint32_t GetSessionsWeight();
	  m4uint32_t GetSrvMemWeight();
	  m4uint32_t GetSysMemWeight();
	  m4uint32_t GetCPUWeight();
	  m4uint32_t GetExecTimeWeight();
	  m4uint32_t GetExecTimeThreshold();
	  
    // Additional Public Declarations
      //## begin ClSiteNodeRepository%3923B6990240.public preserve=yes
      //## end ClSiteNodeRepository%3923B6990240.public

  protected:

    //## Other Operations (specified)
      //## Operation: _FindCandidateNode%959009680
      m4return_t _FindCandidateNode (TSiteNodeMap::iterator* ao_pCppIterator, TSiteNodeMap::iterator* ao_pJavaIterator);

      //## Operation: _AddNode%964092454
      m4return_t _AddNode (const ClSiteNodeInformation &ai_oSiteNode);

      //## Operation: _RemoveNode%964092455
      m4return_t _RemoveNode (const ClSiteNodeInformation &ai_oSiteNode);


	  m4return_t _DumpLoadTrace(m4pcchar_t ai_pccEvent, const ClSiteNodeInformation &ai_oSiteNode, m4bool_t ai_bIsCppCandidate, m4bool_t ai_bIsJavaCandidate);

    // Data Members for Class Attributes

	  TSiteNodeMap::iterator m_oCandidateJavaIterator;

	  TSiteNodeMap::iterator m_oCandidateCppIterator;

    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%3923BB87023B
      //## Role: ClSiteNodeRepository::m_oSiteNodeContainer%3923BB880079
      //## begin ClSiteNodeRepository::m_oSiteNodeContainer%3923BB880079.role preserve=no  protected: TSiteNodeMap { -> 1VHAN}
      TSiteNodeMap m_oSiteNodeContainer;
      //## end ClSiteNodeRepository::m_oSiteNodeContainer%3923BB880079.role

      //## Association: M4Dispatcher::<unnamed>%3923F80102B5
      //## Role: ClSiteNodeRepository::m_oSynchronizationObj%3923F802007C
      //## begin ClSiteNodeRepository::m_oSynchronizationObj%3923F802007C.role preserve=no  protected: ClCriticalSection { -> 1VHAN}
      ClCriticalSection m_oSynchronizationObj;
      //## end ClSiteNodeRepository::m_oSynchronizationObj%3923F802007C.role


	  static FILE* pLoadTraceFile;
    // Additional Protected Declarations
      //## begin ClSiteNodeRepository%3923B6990240.protected preserve=yes
      //## end ClSiteNodeRepository%3923B6990240.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_uiAdminPort%3A12C85300D8
      //## begin ClSiteNodeRepository::m_uiAdminPort%3A12C85300D8.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiAdminPort;
      //## end ClSiteNodeRepository::m_uiAdminPort%3A12C85300D8.attr

      //## Attribute: m_iMaxStartRetries%3BA62C2901FC
      //## begin ClSiteNodeRepository::m_iMaxStartRetries%3BA62C2901FC.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iMaxStartRetries;
      //## end ClSiteNodeRepository::m_iMaxStartRetries%3BA62C2901FC.attr

    // Additional Private Declarations
      //## begin ClSiteNodeRepository%3923B6990240.private preserve=yes
      m4uint32_t m_iServerTimeout;

	  
	  m4uint32_t m_iDumpLoadTrace;

	  m4uint32_t m_iSessionsWeight;
	  m4uint32_t m_iSrvMemWeight;
	  m4uint32_t m_iSysMemWeight;
	  m4uint32_t m_iCPUWeight;
	  m4uint32_t m_iExecTimeWeight;
	  m4uint32_t m_iExecTimeThreshold;
      //## end ClSiteNodeRepository%3923B6990240.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSiteNodeRepository%3923B6990240.implementation preserve=yes
      //## end ClSiteNodeRepository%3923B6990240.implementation

};

//## begin ClSiteNodeRepository%3923B6990240.postscript preserve=yes
//## end ClSiteNodeRepository%3923B6990240.postscript

//## begin ClSiteRepositorySingleton%3A12A8610101.preface preserve=yes
//## end ClSiteRepositorySingleton%3A12A8610101.preface

//## Class: ClSiteRepositorySingleton%3A12A8610101
//## Category: M4Dispatcher::Server Repository%39F43C04019D
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSiteRepositorySingleton 
{
  //## begin ClSiteRepositorySingleton%3A12A8610101.initialDeclarations preserve=yes
  //## end ClSiteRepositorySingleton%3A12A8610101.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetInstance%974303782
      static ClSiteNodeRepository * GetInstance ();

      //## Operation: Destroy%974303783
      //	Destruye el objeto
      static void Destroy ();

    // Additional Public Declarations
      //## begin ClSiteRepositorySingleton%3A12A8610101.public preserve=yes
      //## end ClSiteRepositorySingleton%3A12A8610101.public

  protected:
    // Additional Protected Declarations
      //## begin ClSiteRepositorySingleton%3A12A8610101.protected preserve=yes
      //## end ClSiteRepositorySingleton%3A12A8610101.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::Server Repository::<unnamed>%3A12A997028D
      //## Role: ClSiteRepositorySingleton::m_poRepository%3A12A9980163
      //## begin ClSiteRepositorySingleton::m_poRepository%3A12A9980163.role preserve=no  private: static ClSiteNodeRepository { -> RHAN}
      static ClSiteNodeRepository *m_poRepository;
      //## end ClSiteRepositorySingleton::m_poRepository%3A12A9980163.role

    // Additional Private Declarations
      //## begin ClSiteRepositorySingleton%3A12A8610101.private preserve=yes
      //## end ClSiteRepositorySingleton%3A12A8610101.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSiteRepositorySingleton%3A12A8610101.implementation preserve=yes
      //## end ClSiteRepositorySingleton%3A12A8610101.implementation

};

//## begin ClSiteRepositorySingleton%3A12A8610101.postscript preserve=yes
//## end ClSiteRepositorySingleton%3A12A8610101.postscript

// Class ClSiteNodeInformation 


//## Other Operations (inline)
inline const m4string_t & ClSiteNodeInformation::GetSiteNodeID () const
{
  //## begin ClSiteNodeInformation::GetSiteNodeID%958643329.body preserve=yes
	return m_strSiteNodeID;
  //## end ClSiteNodeInformation::GetSiteNodeID%958643329.body
}

inline const m4string_t & ClSiteNodeInformation::GetNodeHost () const
{
  //## begin ClSiteNodeInformation::GetNodeHost%961054362.body preserve=yes
	return m_strHost ;
  //## end ClSiteNodeInformation::GetNodeHost%961054362.body
}

inline const m4uint32_t ClSiteNodeInformation::GetBasePort () const
{
  //## begin ClSiteNodeInformation::GetBasePort%961054363.body preserve=yes
	return m_iBasePort ;
  //## end ClSiteNodeInformation::GetBasePort%961054363.body
}

inline const m4string_t ClSiteNodeInformation::GetNodeState () const
{
  //## begin ClSiteNodeInformation::GetNodeState%961054364.body preserve=yes
	return m_strNodeState ;
  //## end ClSiteNodeInformation::GetNodeState%961054364.body
}

inline m4void_t ClSiteNodeInformation::SetNodeState (m4pchar_t ai_szNodeState)
{
  //## begin ClSiteNodeInformation::SetNodeState%962263774.body preserve=yes
	m_strNodeState = ai_szNodeState ;
  //## end ClSiteNodeInformation::SetNodeState%962263774.body
}

inline const m4uint32_t ClSiteNodeInformation::GetConnectionID () const
{
  //## begin ClSiteNodeInformation::GetConnectionID%969372804.body preserve=yes
	return m_iConnectionID;
  //## end ClSiteNodeInformation::GetConnectionID%969372804.body
}

inline const m4string_t & ClSiteNodeInformation::GetUserName () const
{
  //## begin ClSiteNodeInformation::GetUserName%969528445.body preserve=yes
	return m_strUserName;
  //## end ClSiteNodeInformation::GetUserName%969528445.body
}

inline m4void_t ClSiteNodeInformation::SetUserName( m4string_t ai_szUserName )
{
	m_strUserName = ai_szUserName;
}

inline const m4string_t & ClSiteNodeInformation::GetPassword () const
{
  //## begin ClSiteNodeInformation::GetPassword%969528446.body preserve=yes
	return m_strPassword;
  //## end ClSiteNodeInformation::GetPassword%969528446.body
}

inline m4void_t ClSiteNodeInformation::SetPassword( m4string_t ai_szPassword )
{
	m_strPassword = ai_szPassword;
}

inline const m4double_t ClSiteNodeInformation::GetCppLoadFactor () const
{
	return m_dCppLoadFactor ;
}

inline const m4double_t ClSiteNodeInformation::GetJavaLoadFactor () const
{
	return m_dJavaLoadFactor ;
}

inline const m4uint32_t ClSiteNodeInformation::GetServerMemory () const
{
  //## begin ClSiteNodeInformation::GetServerMemory%972317054.body preserve=yes
	return m_iServerMemory ;
  //## end ClSiteNodeInformation::GetServerMemory%972317054.body
}

inline m4void_t ClSiteNodeInformation::SetServerMemory (m4uint32_t ai_iServerMemory)
{
  //## begin ClSiteNodeInformation::SetServerMemory%972317055.body preserve=yes
	m_iServerMemory = ai_iServerMemory ;
  //## end ClSiteNodeInformation::SetServerMemory%972317055.body
}

inline const m4double_t ClSiteNodeInformation::GetSystemMemory () const
{
  //## begin ClSiteNodeInformation::GetSystemMemory%972317056.body preserve=yes
	return m_dSystemMemory ;
  //## end ClSiteNodeInformation::GetSystemMemory%972317056.body
}

inline m4void_t ClSiteNodeInformation::SetSystemMemory (m4double_t ai_dSystemMemory)
{
  //## begin ClSiteNodeInformation::SetSystemMemory%972317057.body preserve=yes
	m_dSystemMemory= ai_dSystemMemory ;
  //## end ClSiteNodeInformation::SetSystemMemory%972317057.body
}

inline const m4double_t ClSiteNodeInformation::GetCPUUsage () const
{
  //## begin ClSiteNodeInformation::GetCPUUsage%972317058.body preserve=yes
	return m_dCPU ;
  //## end ClSiteNodeInformation::GetCPUUsage%972317058.body
}

inline m4void_t ClSiteNodeInformation::SetCPUUsage (m4double_t ai_dCPUUsage)
{
  //## begin ClSiteNodeInformation::SetCPUUsage%972317059.body preserve=yes
	m_dCPU = ai_dCPUUsage ;
  //## end ClSiteNodeInformation::SetCPUUsage%972317059.body
}

inline m4uint32_t ClSiteNodeInformation::GetNumSessions () const
{
  //## begin ClSiteNodeInformation::GetNumSessions%972317060.body preserve=yes
	return m_iNumSessions ;
  //## end ClSiteNodeInformation::GetNumSessions%972317060.body
}

inline m4void_t ClSiteNodeInformation::SetNumSessions (m4uint32_t ai_iNumSessions)
{
  //## begin ClSiteNodeInformation::SetNumSessions%972317061.body preserve=yes
	m_iNumSessions = ai_iNumSessions ;
  //## end ClSiteNodeInformation::SetNumSessions%972317061.body
}

inline m4void_t ClSiteNodeInformation::SetPersistable (m4bool_t ai_bPersistable)
{
  //## begin ClSiteNodeInformation::SetPersistable%972463869.body preserve=yes
	m_bPersistable  = ai_bPersistable ;
  //## end ClSiteNodeInformation::SetPersistable%972463869.body
}

inline m4bool_t ClSiteNodeInformation::IsPersistable () const
{
  //## begin ClSiteNodeInformation::IsPersistable%972463870.body preserve=yes
	return m_bPersistable  ;
  //## end ClSiteNodeInformation::IsPersistable%972463870.body
}

inline const m4string_t & ClSiteNodeInformation::GetLocalHost () const
{
  //## begin ClSiteNodeInformation::GetLocalHost%1070448861.body preserve=yes
	return m_strLocalHost.length() > 0 ? m_strLocalHost : m_strHost;
  //## end ClSiteNodeInformation::GetLocalHost%1070448861.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4bool_t& ClSiteNodeInformation::GetIsEnabled () const
{
  //## begin ClSiteNodeInformation::GetIsEnabled%40B2200402FB.get preserve=no
  return m_bIsEnabled;
  //## end ClSiteNodeInformation::GetIsEnabled%40B2200402FB.get
}

inline void ClSiteNodeInformation::SetIsEnabled (const m4bool_t& value)
{
  //## begin ClSiteNodeInformation::SetIsEnabled%40B2200402FB.set preserve=no
  m_bIsEnabled = value;
  //## end ClSiteNodeInformation::SetIsEnabled%40B2200402FB.set
}

// Class ClSiteNodeContainer 

// Class ClSiteNodeRepository 


//## Other Operations (inline)
inline m4uint32_t ClSiteNodeRepository::GetAdminPort ()
{
  //## begin ClSiteNodeRepository::GetAdminPort%974309622.body preserve=yes
	return m_uiAdminPort ;
  //## end ClSiteNodeRepository::GetAdminPort%974309622.body
}

inline void ClSiteNodeRepository::SetParams (m4uint32_t ai_uiAdminPort, m4uint32_t ai_iMaxStartRetries, m4uint32_t ai_iServerTimeout, m4uint32_t ai_iDumpLoadTrace, m4uint32_t ai_iSessionsWeight, m4uint32_t ai_iSrvMemWeight, m4uint32_t ai_iSysMemWeight, m4uint32_t ai_iCPUWeight, m4uint32_t ai_iExecTimeWeight, m4uint32_t ai_iExecTimeThreshold)
{
  //## begin ClSiteNodeRepository::SetParams%974309623.body preserve=yes
	m_uiAdminPort = ai_uiAdminPort  ;

	m_iMaxStartRetries = ai_iMaxStartRetries ;
	m_iServerTimeout = ai_iServerTimeout ;

	m_iDumpLoadTrace = ai_iDumpLoadTrace ;

	m_iSessionsWeight = ai_iSessionsWeight;
	m_iSrvMemWeight = ai_iSrvMemWeight;
	m_iSysMemWeight = ai_iSysMemWeight;
	m_iCPUWeight = ai_iCPUWeight;
	m_iExecTimeWeight = ai_iExecTimeWeight;
	m_iExecTimeThreshold = ai_iExecTimeThreshold;
  //## end ClSiteNodeRepository::SetParams%974309623.body
}

inline m4uint32_t ClSiteNodeRepository::GetSessionsWeight()
{
	return m_iSessionsWeight;
}

inline m4uint32_t ClSiteNodeRepository::GetSrvMemWeight()
{
	return m_iSrvMemWeight;
}

inline m4uint32_t ClSiteNodeRepository::GetSysMemWeight()
{
	return m_iSysMemWeight;
}

inline m4uint32_t ClSiteNodeRepository::GetCPUWeight()
{
	return m_iCPUWeight;
}

inline m4uint32_t ClSiteNodeRepository::GetExecTimeWeight()
{
	return m_iExecTimeWeight;
}

inline m4uint32_t ClSiteNodeRepository::GetExecTimeThreshold()
{
	return m_iExecTimeThreshold;
}

inline const m4string_t ClSiteNodeInformation::GetSiteNodeMapKey () const
{
  //## begin ClSiteNodeInformation::GetSiteNodeID%958643329.body preserve=yes	
	m4string_t sHostName = m_strHost.substr(0, m_strHost.find_first_of ("."));
	m4string_t sSiteNodeMapKey = m_strSiteNodeID;		
	sSiteNodeMapKey += '_' + sHostName;

	return sSiteNodeMapKey;
  //## end ClSiteNodeInformation::GetSiteNodeID%958643329.body
}

inline const m4double_t ClSiteNodeInformation::GetPrivateMB () const
{
	return m_dPrivateMB ;
}

inline m4void_t ClSiteNodeInformation::SetPrivateMB (m4double_t ai_dPrivateMB)
{
	m_dPrivateMB = ai_dPrivateMB ;
}

inline const m4double_t ClSiteNodeInformation::GetMaxProcSize () const
{
	return m_dMaxProcSize ;
}

inline m4void_t ClSiteNodeInformation::SetMaxProcSize (m4double_t ai_dMaxProcSize)
{
	m_dMaxProcSize = ai_dMaxProcSize ;
}

inline const m4double_t ClSiteNodeInformation::GetFreeMem () const
{
	return m_dFreeMem ;
}

inline m4void_t ClSiteNodeInformation::SetFreeMem (m4double_t ai_dFreeMem)
{
	m_dFreeMem = ai_dFreeMem ;
}


inline const m4bool_t ClSiteNodeInformation::GetDiscarded() const
{
	return m_bDiscarded;
}

inline m4void_t ClSiteNodeInformation::SetDiscarded(m4bool_t ai_bDiscarded)
{
	m_bDiscarded = ai_bDiscarded;
}

inline const m4millisec_t ClSiteNodeInformation::GetOLTPTime() const
{
	return m_iOLTPTime;
}

inline m4void_t ClSiteNodeInformation::SetOLTPTime (m4millisec_t ai_iOLTPTime)
{
	m_iOLTPTime = ai_iOLTPTime;
}

inline const m4millisec_t ClSiteNodeInformation::GetXMLTime() const
{
	return m_iXMLTime;
}

inline m4void_t ClSiteNodeInformation::SetXMLTime (m4millisec_t ai_iXMLTime)
{
	m_iXMLTime = ai_iXMLTime;
}


// Class ClSiteRepositorySingleton 

//## begin module%3923BC03012B.epilog preserve=yes
//## end module%3923BC03012B.epilog


#endif
