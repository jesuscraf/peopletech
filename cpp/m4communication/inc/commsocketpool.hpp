//## begin module%39D362CF0299.cm preserve=no
//## end module%39D362CF0299.cm

//## begin module%39D362CF0299.cp preserve=no
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
//## end module%39D362CF0299.cp

//## Module: commsocketpool%39D362CF0299; Package specification
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Source file: e:\mybuild\m4communication\inc\commsocketpool.hpp

#ifndef commsocketpool_h
#define commsocketpool_h 1

//## begin module%39D362CF0299.additionalIncludes preserve=no
//## end module%39D362CF0299.additionalIncludes

//## begin module%39D362CF0299.includes preserve=yes
#include <m4communication_dll.hpp>
//## end module%39D362CF0299.includes

// m4types
#include <m4types.hpp>
//stl
#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
// CriticalSection
#include <criticalsection.hpp>
// CommComun
#include <commcomun.hpp>
//## begin module%39D362CF0299.declarations preserve=no
//## end module%39D362CF0299.declarations

//## begin module%39D362CF0299.additionalDeclarations preserve=yes
//## end module%39D362CF0299.additionalDeclarations


//## begin SocketPool%39D364F70030.preface preserve=yes
//## end SocketPool%39D364F70030.preface

//## Class: SocketPool%39D364F70030; Instantiated Class
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3B386E390047;m4tliid_t { -> }

typedef vector< m4tliid_t > SocketPool;

//## begin SocketPool%39D364F70030.postscript preserve=yes
//## end SocketPool%39D364F70030.postscript

//## begin ClResource%3B386D70037D.preface preserve=yes
//## end ClResource%3B386D70037D.preface

//## Class: ClResource%3B386D70037D
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClResource 
{
  //## begin ClResource%3B386D70037D.initialDeclarations preserve=yes
  //## end ClResource%3B386D70037D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClResource%993554202; C++
      ClResource (m4tliid_t ai_TLI);


    //## Other Operations (specified)
      //## Operation: AddHit%993554203
      void AddHit ();

      //## Operation: Hits%993554204
      m4int_t Hits ();

      //## Operation: GetTLI%993554205
      m4tliid_t GetTLI ();

    // Additional Public Declarations
      //## begin ClResource%3B386D70037D.public preserve=yes
      //## end ClResource%3B386D70037D.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%3B386E6600BA
      //## Role: ClResource::m_iHits%3B386E67013E
      //## begin ClResource::m_iHits%3B386E67013E.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iHits;
      //## end ClResource::m_iHits%3B386E67013E.role

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%3B386E9A03C2
      //## Role: ClResource::m_TLI%3B386E9B0233
      //## begin ClResource::m_TLI%3B386E9B0233.role preserve=no  protected: m4tliid_t { -> VHAN}
      m4tliid_t m_TLI;
      //## end ClResource::m_TLI%3B386E9B0233.role

    // Additional Protected Declarations
      //## begin ClResource%3B386D70037D.protected preserve=yes
      //## end ClResource%3B386D70037D.protected

  private:
    // Additional Private Declarations
      //## begin ClResource%3B386D70037D.private preserve=yes
      //## end ClResource%3B386D70037D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClResource%3B386D70037D.implementation preserve=yes
      //## end ClResource%3B386D70037D.implementation

};

//## begin ClResource%3B386D70037D.postscript preserve=yes
//## end ClResource%3B386D70037D.postscript

//## begin ClResourcePoll%3B38759501B9.preface preserve=yes
//## end ClResourcePoll%3B38759501B9.preface

//## Class: ClResourcePoll%3B38759501B9; Instantiated Class
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3B387E1D0386;ClResource { -> }

typedef map< m4tliid_t,ClResource *,less<m4tliid_t> > ClResourcePoll;

//## begin ClResourcePoll%3B38759501B9.postscript preserve=yes
//## end ClResourcePoll%3B38759501B9.postscript

//## begin ClCommPoolProperties%39E322F3031F.preface preserve=yes
//## end ClCommPoolProperties%39E322F3031F.preface

//## Class: ClCommPoolProperties%39E322F3031F
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommPoolProperties 
{
  //## begin ClCommPoolProperties%39E322F3031F.initialDeclarations preserve=yes
  //## end ClCommPoolProperties%39E322F3031F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommPoolProperties%972041784
      ClCommPoolProperties ();

    //## Destructor (specified)
      //## Operation: ~ClCommPoolProperties%972041785
      ~ClCommPoolProperties ();


    //## Other Operations (specified)
      //## Operation: SetNMaxSocket%972041780; C++
      m4return_t SetNMaxSocket (m4int_t ai_iNMaxSocket);

      //## Operation: ReleaseSocket%972041781
      m4return_t ReleaseSocket (m4tliid_t ai_TLI);

      //## Operation: GetSocket%972041782
      m4tliid_t GetSocket ();

      //## Operation: GetNSocket%972041783
      m4int_t GetNSocket ();

      //## Operation: GetNFreeSocket%972292995
      m4int_t GetNFreeSocket ();

      //## Operation: AddNewSocket%972292996
      m4return_t AddNewSocket (m4tliid_t ai_TLI);

      //## Operation: GetNMaxSocket%972292997; C++
      m4int_t GetNMaxSocket ();

      //## Operation: DumpStats%993554208
      void DumpStats ();

    // Additional Public Declarations
      //## begin ClCommPoolProperties%39E322F3031F.public preserve=yes
      //## end ClCommPoolProperties%39E322F3031F.public

  protected:
    // Data Members for Associations

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%39D365140385
      //## Role: ClCommPoolProperties::m_AvailableRes%39D3651501C5
      //## begin ClCommPoolProperties::m_AvailableRes%39D3651501C5.role preserve=no  protected: SocketPool { -> VHAN}
      SocketPool m_AvailableRes;
      //## end ClCommPoolProperties::m_AvailableRes%39D3651501C5.role

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%39E3268B02C7
      //## Role: ClCommPoolProperties::m_iMaxSocket%39E3268C01BB
      //## begin ClCommPoolProperties::m_iMaxSocket%39E3268C01BB.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iMaxSocket;
      //## end ClCommPoolProperties::m_iMaxSocket%39E3268C01BB.role

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%39F4044B00E1
      //## Role: ClCommPoolProperties::m_iNSocket%39F4044C004D
      //## begin ClCommPoolProperties::m_iNSocket%39F4044C004D.role preserve=no  protected: m4int_t { -> VHAN}
      m4int_t m_iNSocket;
      //## end ClCommPoolProperties::m_iNSocket%39F4044C004D.role

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%3B38766B02D9
      //## Role: ClCommPoolProperties::m_ResourcePoll%3B38766C0104
      //## begin ClCommPoolProperties::m_ResourcePoll%3B38766C0104.role preserve=no  protected: ClResourcePoll { -> VHAN}
      ClResourcePoll m_ResourcePoll;
      //## end ClCommPoolProperties::m_ResourcePoll%3B38766C0104.role

    // Additional Protected Declarations
      //## begin ClCommPoolProperties%39E322F3031F.protected preserve=yes
      //## end ClCommPoolProperties%39E322F3031F.protected

  private:
    // Additional Private Declarations
      //## begin ClCommPoolProperties%39E322F3031F.private preserve=yes
      //## end ClCommPoolProperties%39E322F3031F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommPoolProperties%39E322F3031F.implementation preserve=yes
      //## end ClCommPoolProperties%39E322F3031F.implementation

};

//## begin ClCommPoolProperties%39E322F3031F.postscript preserve=yes
//## end ClCommPoolProperties%39E322F3031F.postscript

//## begin CommPoolMap%38E0820103DD.preface preserve=yes
//## end CommPoolMap%38E0820103DD.preface

//## Class: CommPoolMap%38E0820103DD; Instantiated Class
//	El acceso a este mapa debe de estar bloqueado por un mutex.
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39E323B902F1;ClCommPoolProperties { -> }
//## Uses: <unnamed>%39E323BC00B1;m4string_t { -> }

typedef map< m4string_t,ClCommPoolProperties *,less<string> > CommPoolMap;

//## begin CommPoolMap%38E0820103DD.postscript preserve=yes
//## end CommPoolMap%38E0820103DD.postscript

//## begin ClCommSocketPool%39D364D40333.preface preserve=yes
//## end ClCommSocketPool%39D364D40333.preface

//## Class: ClCommSocketPool%39D364D40333
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39D367A80081;m4tliid_t { -> }
//## Uses: <unnamed>%39D367B802C9;m4string_t { -> }

class M4_DECL_M4COMMUNICATION ClCommSocketPool 
{
  //## begin ClCommSocketPool%39D364D40333.initialDeclarations preserve=yes
  //## end ClCommSocketPool%39D364D40333.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommSocketPool%970156025
      ClCommSocketPool ();

    //## Destructor (specified)
      //## Operation: ~ClCommSocketPool%970156026
      virtual ~ClCommSocketPool ();


    //## Other Operations (specified)
      //## Operation: GetSocket%970156027; C++
      m4tliid_t GetSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime = 0);

      //## Operation: ReleaseSocket%970156028; C++
      m4return_t ReleaseSocket (m4string_t &ai_strUrl, m4tliid_t ai_TLIID);

      //## Operation: SetNSocketPool%971164985
      m4return_t SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket);

      //## Operation: DumpStats%993554206
      void DumpStats (m4string_t &ai_strURL);

    // Additional Public Declarations
      //## begin ClCommSocketPool%39D364D40333.public preserve=yes
      //## end ClCommSocketPool%39D364D40333.public

  protected:

    //## Other Operations (specified)
      //## Operation: AddNewSocket%970156029; C++
      m4return_t AddNewSocket (m4string_t &ai_strUrl, m4tliid_t ai_TLIID);

      //## Operation: CreateSocket%971164986
      virtual m4return_t CreateSocket (m4string_t &ai_strURL, m4tliid_t &ao_TLIID) = 0;

      //## Operation: _SetNSocketPool %993554207
      m4return_t _SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket);

      //## Operation: _GetSocket%993625534; C++
      m4tliid_t _GetSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime = 0);

      //## Operation: _GetNewSocket%993625535; C++
      m4tliid_t _GetNewSocket (m4string_t &ai_strURL, m4int_t ai_imsecWaitTime = 0);

    // Data Members for Associations

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%39D36546028D
      //## Role: ClCommSocketPool::m_oMutex%39D365470357
      //## begin ClCommSocketPool::m_oMutex%39D365470357.role preserve=no  protected: ClCriticalSection { -> VHAN}
      ClCriticalSection m_oMutex;
      //## end ClCommSocketPool::m_oMutex%39D365470357.role

      //## Association: M4Communication::M4COML::SocketPool::<unnamed>%39E325EB00D3
      //## Role: ClCommSocketPool::m_PoolMap%39E325EC0248
      //## begin ClCommSocketPool::m_PoolMap%39E325EC0248.role preserve=no  protected: CommPoolMap { -> VHAN}
      CommPoolMap m_PoolMap;
      //## end ClCommSocketPool::m_PoolMap%39E325EC0248.role

    // Additional Protected Declarations
      //## begin ClCommSocketPool%39D364D40333.protected preserve=yes
      //## end ClCommSocketPool%39D364D40333.protected

  private:
    // Additional Private Declarations
      //## begin ClCommSocketPool%39D364D40333.private preserve=yes
      //## end ClCommSocketPool%39D364D40333.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommSocketPool%39D364D40333.implementation preserve=yes
      //## end ClCommSocketPool%39D364D40333.implementation

};

//## begin ClCommSocketPool%39D364D40333.postscript preserve=yes
//## end ClCommSocketPool%39D364D40333.postscript

//## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.preface preserve=yes
//## end ClCommSocketPoolBasicTSAP%3A06CDA40018.preface

//## Class: ClCommSocketPoolBasicTSAP%3A06CDA40018
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4COMMUNICATION ClCommSocketPoolBasicTSAP : public ClCommSocketPool  //## Inherits: <unnamed>%3A06CDCD028E
{
  //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.initialDeclarations preserve=yes
  //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommSocketPoolBasicTSAP%973524719
      ClCommSocketPoolBasicTSAP ();

    //## Destructor (specified)
      //## Operation: ~ClCommSocketPoolBasicTSAP%973524720
      ~ClCommSocketPoolBasicTSAP ();

    // Additional Public Declarations
      //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.public preserve=yes
      //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.public

  protected:

    //## Other Operations (specified)
      //## Operation: CreateSocket%973524721
      m4return_t CreateSocket (m4string_t &ai_strURL, m4tliid_t &ao_TLIID);

    // Additional Protected Declarations
      //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.protected preserve=yes
      //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.protected

  private:
    // Additional Private Declarations
      //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.private preserve=yes
      //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.implementation preserve=yes
      //## end ClCommSocketPoolBasicTSAP%3A06CDA40018.implementation

};

//## begin ClCommSocketPoolBasicTSAP%3A06CDA40018.postscript preserve=yes
//## end ClCommSocketPoolBasicTSAP%3A06CDA40018.postscript

//## begin ClCommSocketPoolComm%3A227280004F.preface preserve=yes
//## end ClCommSocketPoolComm%3A227280004F.preface

//## Class: ClCommSocketPoolComm%3A227280004F
//## Category: M4Communication::M4COML::SocketPool%39D364C1024F
//## Subsystem: M4Communication::M4COML::inc%38D1FCD80387
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommSocketPoolComm : public ClCommSocketPool  //## Inherits: <unnamed>%3A2272AC0373
{
  //## begin ClCommSocketPoolComm%3A227280004F.initialDeclarations preserve=yes
  //## end ClCommSocketPoolComm%3A227280004F.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClCommSocketPoolComm%3A227280004F.public preserve=yes
      //## end ClCommSocketPoolComm%3A227280004F.public

  protected:

    //## Other Operations (specified)
      //## Operation: CreateSocket%975323612
      m4return_t CreateSocket (m4string_t &ai_strURL, m4tliid_t &ao_TLIID);

    // Additional Protected Declarations
      //## begin ClCommSocketPoolComm%3A227280004F.protected preserve=yes
      //## end ClCommSocketPoolComm%3A227280004F.protected

  private:
    // Additional Private Declarations
      //## begin ClCommSocketPoolComm%3A227280004F.private preserve=yes
      //## end ClCommSocketPoolComm%3A227280004F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommSocketPoolComm%3A227280004F.implementation preserve=yes
      //## end ClCommSocketPoolComm%3A227280004F.implementation

};

//## begin ClCommSocketPoolComm%3A227280004F.postscript preserve=yes
//## end ClCommSocketPoolComm%3A227280004F.postscript

// Class ClResource 

// Class ClCommPoolProperties 

// Class ClCommSocketPool 

// Class ClCommSocketPoolBasicTSAP 

// Class ClCommSocketPoolComm 

//## begin module%39D362CF0299.epilog preserve=yes
//## end module%39D362CF0299.epilog


#endif
