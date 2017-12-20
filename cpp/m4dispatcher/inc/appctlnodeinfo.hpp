//## begin module%39F71B9F0321.cm preserve=no
//## end module%39F71B9F0321.cm

//## begin module%39F71B9F0321.cp preserve=no
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
//## end module%39F71B9F0321.cp

//## Module: appctlnodeinfo%39F71B9F0321; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: D:\Work\v600\m4dispatcher\inc\appctlnodeinfo.hpp

#ifndef appctlnodeinfo_h
#define appctlnodeinfo_h 1

//## begin module%39F71B9F0321.additionalIncludes preserve=no
//## end module%39F71B9F0321.additionalIncludes

//## begin module%39F71B9F0321.includes preserve=yes
//## end module%39F71B9F0321.includes
#include "m4stl.hpp"
// m4string
#include <m4string.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
//## begin module%39F71B9F0321.declarations preserve=no
//## end module%39F71B9F0321.declarations

//## begin module%39F71B9F0321.additionalDeclarations preserve=yes
//## end module%39F71B9F0321.additionalDeclarations


//## begin ClAppctlNodeInfo%39F71A290373.preface preserve=yes
//## end ClAppctlNodeInfo%39F71A290373.preface

//## Class: ClAppctlNodeInfo%39F71A290373
//## Category: M4Dispatcher::AppCtlRepository%39F719A301A4
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39F721ED01B9;m4string_t { -> }

class ClAppctlNodeInfo 
{
  //## begin ClAppctlNodeInfo%39F71A290373.initialDeclarations preserve=yes
  //## end ClAppctlNodeInfo%39F71A290373.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppctlNodeInfo%972463875
      ClAppctlNodeInfo (m4string_t ai_szHost, m4uint32_t ai_iPort);


    //## Other Operations (specified)
      //## Operation: GetSiteNodeHost%972463879
      m4string_t GetSiteNodeHost () const;

      //## Operation: GetAppctlPort%972463880
      m4uint32_t GetAppctlPort () const;

      //## Operation: operator =%972463876
      ClAppctlNodeInfo & operator = (const ClAppctlNodeInfo &ai_oAppctlNodeInfo);

      //## Operation: operator ==%972463881
      friend m4bool_t operator == (const ClAppctlNodeInfo &ai_oRAppctlInfo, const ClAppctlNodeInfo &ai_oLAppctlinfo);

    // Additional Public Declarations
      //## begin ClAppctlNodeInfo%39F71A290373.public preserve=yes
      //## end ClAppctlNodeInfo%39F71A290373.public

  protected:
    // Additional Protected Declarations
      //## begin ClAppctlNodeInfo%39F71A290373.protected preserve=yes
      //## end ClAppctlNodeInfo%39F71A290373.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_szSiteNodeHost%39F71A760143
      //## begin ClAppctlNodeInfo::m_szSiteNodeHost%39F71A760143.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szSiteNodeHost;
      //## end ClAppctlNodeInfo::m_szSiteNodeHost%39F71A760143.attr

      //## Attribute: m_iAppctlPort%39F71A8E0329
      //## begin ClAppctlNodeInfo::m_iAppctlPort%39F71A8E0329.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iAppctlPort;
      //## end ClAppctlNodeInfo::m_iAppctlPort%39F71A8E0329.attr

    // Additional Private Declarations
      //## begin ClAppctlNodeInfo%39F71A290373.private preserve=yes
      //## end ClAppctlNodeInfo%39F71A290373.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppctlNodeInfo%39F71A290373.implementation preserve=yes
      //## end ClAppctlNodeInfo%39F71A290373.implementation

};

//## begin ClAppctlNodeInfo%39F71A290373.postscript preserve=yes
//## end ClAppctlNodeInfo%39F71A290373.postscript

//## begin TAppMap%39F719C201E5.preface preserve=yes
//## end TAppMap%39F719C201E5.preface

//## Class: TAppMap%39F719C201E5; Instantiated Class
//## Category: M4Dispatcher::AppCtlRepository%39F719A301A4
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39F71BFB00C1;ClAppctlNodeInfo { -> }

typedef map< m4string_t,ClAppctlNodeInfo,less <m4string_t>  > TAppMap;

//## begin TAppMap%39F719C201E5.postscript preserve=yes
//## end TAppMap%39F719C201E5.postscript

//## begin ClAppctlNodeRepository%39F71C1400F9.preface preserve=yes
//## end ClAppctlNodeRepository%39F71C1400F9.preface

//## Class: ClAppctlNodeRepository%39F71C1400F9
//## Category: M4Dispatcher::AppCtlRepository%39F719A301A4
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FCDFD6B0002;ClSiteNodeRepository { -> }

class ClAppctlNodeRepository 
{
  //## begin ClAppctlNodeRepository%39F71C1400F9.initialDeclarations preserve=yes
  //## end ClAppctlNodeRepository%39F71C1400F9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAppctlNodeRepository%972463877
      ClAppctlNodeRepository ();


    //## Other Operations (specified)
      //## Operation: AddNode%972463878
      m4return_t AddNode (const ClAppctlNodeInfo &ai_oAppctlNodeInfo);

      //## Operation: GetNodeInfo%972463882
      m4return_t GetNodeInfo (ClAppctlNodeInfo &ai_oAppctlNodeInfo);

    // Additional Public Declarations
      //## begin ClAppctlNodeRepository%39F71C1400F9.public preserve=yes
      //## end ClAppctlNodeRepository%39F71C1400F9.public

  protected:

    //## Other Operations (specified)
      //## Operation: ConvertToNodeHost%1070463940
      ClAppctlNodeInfo ConvertToNodeHost (ClAppctlNodeInfo &ai_oInfo);

    // Data Members for Associations

      //## Association: M4Dispatcher::AppCtlRepository::<unnamed>%39F71CAB0313
      //## Role: ClAppctlNodeRepository::m_oAppctlMap%39F71CAC01FD
      //## begin ClAppctlNodeRepository::m_oAppctlMap%39F71CAC01FD.role preserve=no  protected: TAppMap { -> VHAN}
      TAppMap m_oAppctlMap;
      //## end ClAppctlNodeRepository::m_oAppctlMap%39F71CAC01FD.role

    // Additional Protected Declarations
      //## begin ClAppctlNodeRepository%39F71C1400F9.protected preserve=yes
      //## end ClAppctlNodeRepository%39F71C1400F9.protected

  private:
    // Additional Private Declarations
      //## begin ClAppctlNodeRepository%39F71C1400F9.private preserve=yes
      //## end ClAppctlNodeRepository%39F71C1400F9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAppctlNodeRepository%39F71C1400F9.implementation preserve=yes
      //## end ClAppctlNodeRepository%39F71C1400F9.implementation

};

//## begin ClAppctlNodeRepository%39F71C1400F9.postscript preserve=yes
//## end ClAppctlNodeRepository%39F71C1400F9.postscript

// Class ClAppctlNodeInfo 


//## Other Operations (inline)
inline m4string_t ClAppctlNodeInfo::GetSiteNodeHost () const
{
  //## begin ClAppctlNodeInfo::GetSiteNodeHost%972463879.body preserve=yes
	return m_szSiteNodeHost ;
  //## end ClAppctlNodeInfo::GetSiteNodeHost%972463879.body
}

inline m4uint32_t ClAppctlNodeInfo::GetAppctlPort () const
{
  //## begin ClAppctlNodeInfo::GetAppctlPort%972463880.body preserve=yes
	return m_iAppctlPort ;
  //## end ClAppctlNodeInfo::GetAppctlPort%972463880.body
}

// Class ClAppctlNodeRepository 

//## begin module%39F71B9F0321.epilog preserve=yes
//## end module%39F71B9F0321.epilog


#endif
