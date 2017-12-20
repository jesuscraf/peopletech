//## begin module%37EB903001F8.cm preserve=no
//## end module%37EB903001F8.cm

//## begin module%37EB903001F8.cp preserve=no
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
//## end module%37EB903001F8.cp

//## Module: CommonVariables%37EB903001F8; Package specification
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Source file: F:\integration\M4Appctl\inc\commonvariables.hpp

#ifndef _COMMONVARIABLES_HPP
#define _COMMONVARIABLES_HPP 1

//## begin module%37EB903001F8.additionalIncludes preserve=no
//## end module%37EB903001F8.additionalIncludes

//## begin module%37EB903001F8.includes preserve=yes
#include <m4types.hpp>
#include <m4string.hpp>                
#ifdef _WINDOWS
#include <windows.h>  
#endif
//## end module%37EB903001F8.includes

// syncro
#include <syncro.hpp>
// m4condition
#include <m4condition.hpp>
//## begin module%37EB903001F8.declarations preserve=no
//## end module%37EB903001F8.declarations

//## begin module%37EB903001F8.additionalDeclarations preserve=yes
//## end module%37EB903001F8.additionalDeclarations


//## begin ClCommonVariables%37EB8D010123.preface preserve=yes
//## end ClCommonVariables%37EB8D010123.preface

//## Class: ClCommonVariables%37EB8D010123
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommonVariables 
{
  //## begin ClCommonVariables%37EB8D010123.initialDeclarations preserve=yes
  //## end ClCommonVariables%37EB8D010123.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCommonVariables%938158802
      ClCommonVariables ();

    //## Destructor (specified)
      //## Operation: ~ClCommonVariables%941022967
      ~ClCommonVariables ();


    //## Other Operations (specified)
      //## Operation: SetHost%982239795
      void SetHost (m4pchar_t ai_szHost);

      //## Operation: GetHost%982239796
      m4pchar_t GetHost ();

    // Data Members for Class Attributes

      //## Attribute: m_bStop_Demon%37EB8D310014
      //## begin ClCommonVariables::m_bStop_Demon%37EB8D310014.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t m_bStop_Demon;
      //## end ClCommonVariables::m_bStop_Demon%37EB8D310014.attr

      //## Attribute: m_iAdminPort%382AD9ED0257
      //## begin ClCommonVariables::m_iAdminPort%382AD9ED0257.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_iAdminPort;
      //## end ClCommonVariables::m_iAdminPort%382AD9ED0257.attr

      //## Attribute: m_ulNumOfAdminConnections%382BD7BE00E7
      //## begin ClCommonVariables::m_ulNumOfAdminConnections%382BD7BE00E7.attr preserve=no  public: m4uint32_t {UA} 0
      m4uint32_t m_ulNumOfAdminConnections;
      //## end ClCommonVariables::m_ulNumOfAdminConnections%382BD7BE00E7.attr

      //## Attribute: m_iControlPort%392D417D02FB
      //## begin ClCommonVariables::m_iControlPort%392D417D02FB.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_iControlPort;
      //## end ClCommonVariables::m_iControlPort%392D417D02FB.attr

      //## Attribute: m_strHost%3A8BCCE300D3
      //## begin ClCommonVariables::m_strHost%3A8BCCE300D3.attr preserve=no  public: m4pchar_t {UA} 
      m4pchar_t m_strHost;
      //## end ClCommonVariables::m_strHost%3A8BCCE300D3.attr

    // Data Members for Associations

      //## Association: M4Appctl::<unnamed>%3816F94701C3
      //## Role: ClCommonVariables::m_poMutex%3816F9480124
      //## begin ClCommonVariables::m_poMutex%3816F9480124.role preserve=no  public: ClMutex { -> RHAN}
      ClMutex *m_poMutex;
      //## end ClCommonVariables::m_poMutex%3816F9480124.role

      //## Association: M4Appctl::<unnamed>%38171C9C036D
      //## Role: ClCommonVariables::m_poCondition%38171C9D0301
      //## begin ClCommonVariables::m_poCondition%38171C9D0301.role preserve=no  public: ClCondition { -> RHAN}
      ClCondition *m_poCondition;
      //## end ClCommonVariables::m_poCondition%38171C9D0301.role

      //## Association: M4Appctl::<unnamed>%38171D4501CC
      //## Role: ClCommonVariables::m_poControlCondConnection%38171D460214
      //## begin ClCommonVariables::m_poControlCondConnection%38171D460214.role preserve=no  public: ClCondition { -> RHAN}
      ClCondition *m_poControlCondConnection;
      //## end ClCommonVariables::m_poControlCondConnection%38171D460214.role

      //## Association: M4Appctl::<unnamed>%382BD81500CE
      //## Role: ClCommonVariables::m_poAdminCondConnection%382BD81503E6
      //## begin ClCommonVariables::m_poAdminCondConnection%382BD81503E6.role preserve=no  public: ClCondition { -> RHAN}
      ClCondition *m_poAdminCondConnection;
      //## end ClCommonVariables::m_poAdminCondConnection%382BD81503E6.role

    // Additional Public Declarations
      //## begin ClCommonVariables%37EB8D010123.public preserve=yes
      //## end ClCommonVariables%37EB8D010123.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommonVariables%37EB8D010123.protected preserve=yes
      //## end ClCommonVariables%37EB8D010123.protected

  private:
    // Additional Private Declarations
      //## begin ClCommonVariables%37EB8D010123.private preserve=yes
      //## end ClCommonVariables%37EB8D010123.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommonVariables%37EB8D010123.implementation preserve=yes
      //## end ClCommonVariables%37EB8D010123.implementation

};

//## begin ClCommonVariables%37EB8D010123.postscript preserve=yes
//## end ClCommonVariables%37EB8D010123.postscript

//## begin ClProcessNameLess%37F8C95C0110.preface preserve=yes
//## end ClProcessNameLess%37F8C95C0110.preface

//## Class: ClProcessNameLess%37F8C95C0110
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClProcessNameLess 
{
  //## begin ClProcessNameLess%37F8C95C0110.initialDeclarations preserve=yes
  //## end ClProcessNameLess%37F8C95C0110.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator()%939052189
      m4bool_t operator () (const m4pchar_t pcProcessNameA, const m4pchar_t pcProcessNameB) const;

    // Additional Public Declarations
      //## begin ClProcessNameLess%37F8C95C0110.public preserve=yes
      //## end ClProcessNameLess%37F8C95C0110.public

  protected:
    // Additional Protected Declarations
      //## begin ClProcessNameLess%37F8C95C0110.protected preserve=yes
      //## end ClProcessNameLess%37F8C95C0110.protected

  private:
    // Additional Private Declarations
      //## begin ClProcessNameLess%37F8C95C0110.private preserve=yes
      //## end ClProcessNameLess%37F8C95C0110.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProcessNameLess%37F8C95C0110.implementation preserve=yes
      //## end ClProcessNameLess%37F8C95C0110.implementation

};

//## begin ClProcessNameLess%37F8C95C0110.postscript preserve=yes
//## end ClProcessNameLess%37F8C95C0110.postscript

//## begin ClTSAPIdLess%37FB6728034F.preface preserve=yes
//## end ClTSAPIdLess%37FB6728034F.preface

//## Class: ClTSAPIdLess%37FB6728034F
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTSAPIdLess 
{
  //## begin ClTSAPIdLess%37FB6728034F.initialDeclarations preserve=yes
  //## end ClTSAPIdLess%37FB6728034F.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: operator()%939052198
      m4bool_t operator () (const m4uint32_t TSAPIdA, const m4uint32_t TSAPIdB) const;

    // Additional Public Declarations
      //## begin ClTSAPIdLess%37FB6728034F.public preserve=yes
      //## end ClTSAPIdLess%37FB6728034F.public

  protected:
    // Additional Protected Declarations
      //## begin ClTSAPIdLess%37FB6728034F.protected preserve=yes
      //## end ClTSAPIdLess%37FB6728034F.protected

  private:
    // Additional Private Declarations
      //## begin ClTSAPIdLess%37FB6728034F.private preserve=yes
      //## end ClTSAPIdLess%37FB6728034F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTSAPIdLess%37FB6728034F.implementation preserve=yes
      //## end ClTSAPIdLess%37FB6728034F.implementation

};

//## begin ClTSAPIdLess%37FB6728034F.postscript preserve=yes
//## end ClTSAPIdLess%37FB6728034F.postscript

//## begin ClXMLDoc%39045B27017B.preface preserve=yes
//## end ClXMLDoc%39045B27017B.preface

//## Class: ClXMLDoc%39045B27017B
//## Category: M4Appctl%37EA673500B9
//## Subsystem: M4Appctl::inc%37EA675D02F1
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4pchar_t ClXMLDoc;

//## begin ClXMLDoc%39045B27017B.postscript preserve=yes
//## end ClXMLDoc%39045B27017B.postscript

// Class ClCommonVariables 

// Class ClProcessNameLess 

// Class ClTSAPIdLess 

//## begin module%37EB903001F8.epilog preserve=yes
//## end module%37EB903001F8.epilog


#endif
