//## begin module%3A2BBB3B03DE.cm preserve=no
//## end module%3A2BBB3B03DE.cm

//## begin module%3A2BBB3B03DE.cp preserve=no
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
//## end module%3A2BBB3B03DE.cp

//## Module: m4dispatchermain%3A2BBB3B03DE; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: z:\m4dispatcher\inc\m4dispatchermain.hpp

#ifndef m4dispatchermain_h
#define m4dispatchermain_h 1

//## begin module%3A2BBB3B03DE.additionalIncludes preserve=no
//## end module%3A2BBB3B03DE.additionalIncludes

//## begin module%3A2BBB3B03DE.includes preserve=yes
//## end module%3A2BBB3B03DE.includes

// commadapter
#include <commadapter.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
// executiveagent
#include <executiveagent.hpp>
// serveragent
#include <serveragent.hpp>
// clientagent
#include <clientagent.hpp>
// adminagent
#include <adminagent.hpp>
// m4condition
#include <m4condition.hpp>
// snmpadmininstance
#include <snmpadmininstance.hpp>
#include <sitenodemonitor.hpp>

#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class XercesDOMParser;
XERCES_CPP_NAMESPACE_END


//## begin module%3A2BBB3B03DE.declarations preserve=no
//## end module%3A2BBB3B03DE.declarations

//## begin module%3A2BBB3B03DE.additionalDeclarations preserve=yes
//## end module%3A2BBB3B03DE.additionalDeclarations

DOMNode* GetConfigNode (const char* ai_pFile, XercesDOMParser& oParser) ;

//## begin ClDpchMain%3A2BB87D03D5.preface preserve=yes
//## end ClDpchMain%3A2BB87D03D5.preface

//## Class: ClDpchMain%3A2BB87D03D5
//## Category: M4Dispatcher::Dispatcher Main%3A2BB7B10377
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40A880B40047;M4SnmpAdminInstance { -> }

class ClDpchMain 
{
  //## begin ClDpchMain%3A2BB87D03D5.initialDeclarations preserve=yes
  //## end ClDpchMain%3A2BB87D03D5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDpchMain%976007077
      ClDpchMain ();

    //## Destructor (specified)
      //## Operation: ~ClDpchMain%976007078
      ~ClDpchMain ();


    //## Other Operations (specified)
      //## Operation: StartDispatcher%975949985
      int StartDispatcher (m4pchar_t ai_pszDpsConfigPath = 0);

      //## Operation: StopDispatcher%975949986
      void StopDispatcher ();

      //## Operation: ComposeDspLogFilePath%979212710
      m4pchar_t ComposeDspLogFilePath (m4pchar_t ai_pszDpsConfigPath);

    // Data Members for Associations

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC8B302A6
      //## Role: ClDpchMain::m_poDispatcherAgent%3A2BC8B400DA
      //## begin ClDpchMain::m_poDispatcherAgent%3A2BC8B400DA.role preserve=no  public: ClAdminAgent { -> RHAN}
      ClAdminAgent *m_poDispatcherAgent;
      //## end ClDpchMain::m_poDispatcherAgent%3A2BC8B400DA.role

	  // eh 197951
	  ClSiteHealthVerifier* m_pSiteHealthVerifier;
    // Additional Public Declarations
      //## begin ClDpchMain%3A2BB87D03D5.public preserve=yes
      //## end ClDpchMain%3A2BB87D03D5.public

  protected:
    // Additional Protected Declarations
      //## begin ClDpchMain%3A2BB87D03D5.protected preserve=yes
      //## end ClDpchMain%3A2BB87D03D5.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC76A023E
      //## Role: ClDpchMain::m_poStopper%3A2BC76B013B
      //## begin ClDpchMain::m_poStopper%3A2BC76B013B.role preserve=no  private: ClCondition { -> RHAN}
      ClCondition *m_poStopper;
      //## end ClDpchMain::m_poStopper%3A2BC76B013B.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC78F009D
      //## Role: ClDpchMain::m_poRepository%3A2BC78F02BB
      //## begin ClDpchMain::m_poRepository%3A2BC78F02BB.role preserve=no  private: ClSiteNodeRepository { -> RHAN}
      ClSiteNodeRepository *m_poRepository;
      //## end ClDpchMain::m_poRepository%3A2BC78F02BB.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC851007E
      //## Role: ClDpchMain::m_poExecutiveAgent%3A2BC852004E
      //## begin ClDpchMain::m_poExecutiveAgent%3A2BC852004E.role preserve=no  private: ClDspExecutiveAgent { -> RHAN}
      ClDspExecutiveAgent *m_poExecutiveAgent;
      //## end ClDpchMain::m_poExecutiveAgent%3A2BC852004E.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC91D03AC
      //## Role: ClDpchMain::m_poServerAgent%3A2BC91E0213
      //## begin ClDpchMain::m_poServerAgent%3A2BC91E0213.role preserve=no  private: ClServerAgent { -> RHAN}
      ClServerAgent *m_poServerAgent;
      //## end ClDpchMain::m_poServerAgent%3A2BC91E0213.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC9680346
      //## Role: ClDpchMain::m_poClientAgent%3A2BC9690212
      //## begin ClDpchMain::m_poClientAgent%3A2BC9690212.role preserve=no  private: ClClientAgent { -> RHAN}
      ClClientAgent *m_poClientAgent;
      //## end ClDpchMain::m_poClientAgent%3A2BC9690212.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC9A7012A
      //## Role: ClDpchMain::m_poCommAdapter%3A2BC9A70352
      //## begin ClDpchMain::m_poCommAdapter%3A2BC9A70352.role preserve=no  private: ClDptcherCommsAdaptor { -> RHAN}
      ClDptcherCommsAdaptor *m_poCommAdapter;
      //## end ClDpchMain::m_poCommAdapter%3A2BC9A70352.role


    // Additional Private Declarations
      //## begin ClDpchMain%3A2BB87D03D5.private preserve=yes
      //## end ClDpchMain%3A2BB87D03D5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDpchMain%3A2BB87D03D5.implementation preserve=yes
      //## end ClDpchMain%3A2BB87D03D5.implementation

};

//## begin ClDpchMain%3A2BB87D03D5.postscript preserve=yes
//## end ClDpchMain%3A2BB87D03D5.postscript

//## begin ClDpchMainSingleton%3A2BBBB002C4.preface preserve=yes
//## end ClDpchMainSingleton%3A2BBBB002C4.preface

//## Class: ClDpchMainSingleton%3A2BBBB002C4
//## Category: M4Dispatcher::Dispatcher Main%3A2BB7B10377
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDpchMainSingleton 
{
  //## begin ClDpchMainSingleton%3A2BBBB002C4.initialDeclarations preserve=yes
  //## end ClDpchMainSingleton%3A2BBBB002C4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDpchMainSingleton%975949987
      ClDpchMainSingleton ();

    //## Destructor (specified)
      //## Operation: ~ClDpchMainSingleton%975949988
      ~ClDpchMainSingleton ();


    //## Other Operations (specified)
      //## Operation: Instance%975949989
      static ClDpchMainSingleton * Instance ();

      //## Operation: FreeInstance%975949990
      static ClDpchMainSingleton * FreeInstance ();

      //## Operation: GetDpchMain%976007079
      ClDpchMain * GetDpchMain ();

    // Additional Public Declarations
      //## begin ClDpchMainSingleton%3A2BBBB002C4.public preserve=yes
      //## end ClDpchMainSingleton%3A2BBBB002C4.public

  protected:
    // Additional Protected Declarations
      //## begin ClDpchMainSingleton%3A2BBBB002C4.protected preserve=yes
      //## end ClDpchMainSingleton%3A2BBBB002C4.protected

  private:
    // Data Members for Associations

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BBC440104
      //## Role: ClDpchMainSingleton::m_poDpchMain%3A2BBC4402F0
      //## begin ClDpchMainSingleton::m_poDpchMain%3A2BBC4402F0.role preserve=no  private: static ClDpchMain { -> RHAN}
      static ClDpchMain *m_poDpchMain;
      //## end ClDpchMainSingleton::m_poDpchMain%3A2BBC4402F0.role

      //## Association: M4Dispatcher::Dispatcher Main::<unnamed>%3A2BC4B70050
      //## Role: ClDpchMainSingleton::m_poInstance%3A2BC4B90338
      //## begin ClDpchMainSingleton::m_poInstance%3A2BC4B90338.role preserve=no  private: static ClDpchMainSingleton { -> RHAN}
      static ClDpchMainSingleton *m_poInstance;
      //## end ClDpchMainSingleton::m_poInstance%3A2BC4B90338.role

    // Additional Private Declarations
      //## begin ClDpchMainSingleton%3A2BBBB002C4.private preserve=yes
      //## end ClDpchMainSingleton%3A2BBBB002C4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDpchMainSingleton%3A2BBBB002C4.implementation preserve=yes
      //## end ClDpchMainSingleton%3A2BBBB002C4.implementation

};

//## begin ClDpchMainSingleton%3A2BBBB002C4.postscript preserve=yes
//## end ClDpchMainSingleton%3A2BBBB002C4.postscript

// Class ClDpchMain 

// Class ClDpchMainSingleton 

//## begin module%3A2BBB3B03DE.epilog preserve=yes
//## end module%3A2BBB3B03DE.epilog


#endif
