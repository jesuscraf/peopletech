//## begin module%39466D8101C9.cm preserve=no
//## end module%39466D8101C9.cm

//## begin module%39466D8101C9.cp preserve=no
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
//## end module%39466D8101C9.cp

//## Module: saxactioncaller%39466D8101C9; Package specification
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Source file: F:\integration\m4xmlutil\inc\saxactioncaller.hpp

#ifndef saxactioncaller_h
#define saxactioncaller_h 1

//## begin module%39466D8101C9.additionalIncludes preserve=no
//## end module%39466D8101C9.additionalIncludes

//## begin module%39466D8101C9.includes preserve=yes
//## end module%39466D8101C9.includes

// ParamList
#include <paramlist.hpp>
// m4xmlbase
#include <m4xmlbase.hpp>
// iexecutable
#include <iexecutable.hpp>
//## begin module%39466D8101C9.declarations preserve=no
//## end module%39466D8101C9.declarations

//## begin module%39466D8101C9.additionalDeclarations preserve=yes
//## end module%39466D8101C9.additionalDeclarations


//## begin ClCurrentContext%3946699000CE.preface preserve=yes
//## end ClCurrentContext%3946699000CE.preface

//## Class: ClCurrentContext%3946699000CE
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCurrentContext 
{
  //## begin ClCurrentContext%3946699000CE.initialDeclarations preserve=yes
  //## end ClCurrentContext%3946699000CE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCurrentContext%960917066
      ClCurrentContext (m4string_t ai_szCommand, ClParamList *ai_poParamList = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCurrentContext%960917067
      ~ClCurrentContext ();


    //## Other Operations (specified)
      //## Operation: GetParamList%960917068
      ClParamList * GetParamList ();

      //## Operation: GetCommand%960917074
      m4string_t GetCommand ();

    // Additional Public Declarations
      //## begin ClCurrentContext%3946699000CE.public preserve=yes
      //## end ClCurrentContext%3946699000CE.public

  protected:
    // Additional Protected Declarations
      //## begin ClCurrentContext%3946699000CE.protected preserve=yes
      //## end ClCurrentContext%3946699000CE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_szCommand%394670B20106
      //## begin ClCurrentContext::m_szCommand%394670B20106.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szCommand;
      //## end ClCurrentContext::m_szCommand%394670B20106.attr

    // Data Members for Associations

      //## Association: M4XMLUtil::<unnamed>%39466AE40177
      //## Role: ClCurrentContext::m_poParamList%39466AE5011F
      //## begin ClCurrentContext::m_poParamList%39466AE5011F.role preserve=no  private: ClParamList { -> 1RHAN}
      ClParamList *m_poParamList;
      //## end ClCurrentContext::m_poParamList%39466AE5011F.role

    // Additional Private Declarations
      //## begin ClCurrentContext%3946699000CE.private preserve=yes
      //## end ClCurrentContext%3946699000CE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCurrentContext%3946699000CE.implementation preserve=yes
      //## end ClCurrentContext%3946699000CE.implementation

};

//## begin ClCurrentContext%3946699000CE.postscript preserve=yes
//## end ClCurrentContext%3946699000CE.postscript

//## begin ClSAXHandlerActionCaller%3946685503CE.preface preserve=yes
//## end ClSAXHandlerActionCaller%3946685503CE.preface

//## Class: ClSAXHandlerActionCaller%3946685503CE
//## Category: M4XMLUtil%3906B6A20380
//## Subsystem: M4XMLUtil::inc%3906B78502AA
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4XMLUTIL ClSAXHandlerActionCaller : public M4SAXHandlerBase  //## Inherits: <unnamed>%3946689102B2
{
  //## begin ClSAXHandlerActionCaller%3946685503CE.initialDeclarations preserve=yes
  //## end ClSAXHandlerActionCaller%3946685503CE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSAXHandlerActionCaller%960917069
      ClSAXHandlerActionCaller (ClExecutableInterface *ai_poExecutableInterface, ClParamList *ai_poParamList = NULL);

    //## Destructor (specified)
      //## Operation: ~ClSAXHandlerActionCaller%960917070
      ~ClSAXHandlerActionCaller ();


    //## Other Operations (specified)
      //## Operation: procStartCommand%960917071
      void procStartCommand (const XMLCh* const name, AttributeList *attributes);

      //## Operation: procEndCommand%960917072
      void procEndCommand (const XMLCh* const name);

      //## Operation: procStartParams%960917073
      void procStartParams (const XMLCh* const name, AttributeList *attributes);

      //## Operation: GetContext%962094004
      ClCurrentContext * GetContext ();

    // Additional Public Declarations
      //## begin ClSAXHandlerActionCaller%3946685503CE.public preserve=yes
      //## end ClSAXHandlerActionCaller%3946685503CE.public

  protected:
    // Data Members for Associations

      //## Association: M4XMLUtil::<unnamed>%394669AB00E1
      //## Role: ClSAXHandlerActionCaller::m_poContext%394669AB0327
      //## begin ClSAXHandlerActionCaller::m_poContext%394669AB0327.role preserve=no  protected: ClCurrentContext { -> 1RHAN}
      ClCurrentContext *m_poContext;
      //## end ClSAXHandlerActionCaller::m_poContext%394669AB0327.role

      //## Association: M4XMLUtil::<unnamed>%39743D9A0396
      //## Role: ClSAXHandlerActionCaller::m_poParamList%39743D9B0230
      //## begin ClSAXHandlerActionCaller::m_poParamList%39743D9B0230.role preserve=no  protected: ClParamList { -> RHAN}
      ClParamList *m_poParamList;
      //## end ClSAXHandlerActionCaller::m_poParamList%39743D9B0230.role

    // Additional Protected Declarations
      //## begin ClSAXHandlerActionCaller%3946685503CE.protected preserve=yes
      //## end ClSAXHandlerActionCaller%3946685503CE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iSessionId%39588BDF0354
      //## begin ClSAXHandlerActionCaller::m_iSessionId%39588BDF0354.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iSessionId;
      //## end ClSAXHandlerActionCaller::m_iSessionId%39588BDF0354.attr

      //## Attribute: m_iXMLId%39588BF101DD
      //## begin ClSAXHandlerActionCaller::m_iXMLId%39588BF101DD.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iXMLId;
      //## end ClSAXHandlerActionCaller::m_iXMLId%39588BF101DD.attr

    // Data Members for Associations

      //## Association: M4XMLUtil::<unnamed>%39466BB900E7
      //## Role: ClSAXHandlerActionCaller::m_poExecutive%39466BB90323
      //## begin ClSAXHandlerActionCaller::m_poExecutive%39466BB90323.role preserve=no  private: ClExecutableInterface { -> RHAN}
      ClExecutableInterface *m_poExecutive;
      //## end ClSAXHandlerActionCaller::m_poExecutive%39466BB90323.role

    // Additional Private Declarations
      //## begin ClSAXHandlerActionCaller%3946685503CE.private preserve=yes
      //## end ClSAXHandlerActionCaller%3946685503CE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSAXHandlerActionCaller%3946685503CE.implementation preserve=yes
      m4bool_t m_bParamListOwner;
      //## end ClSAXHandlerActionCaller%3946685503CE.implementation

};

//## begin ClSAXHandlerActionCaller%3946685503CE.postscript preserve=yes
//## end ClSAXHandlerActionCaller%3946685503CE.postscript

// Class ClCurrentContext 

// Class ClSAXHandlerActionCaller 

//## begin module%39466D8101C9.epilog preserve=yes
//## end module%39466D8101C9.epilog


#endif
