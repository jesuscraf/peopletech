//## begin module%3917F51B02A1.cm preserve=no
//## end module%3917F51B02A1.cm

//## begin module%3917F51B02A1.cp preserve=no
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
//## end module%3917F51B02A1.cp

//## Module: m4event%3917F51B02A1; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\factor\m4sysall\inc\m4event.hpp

#ifndef m4event_h
#define m4event_h 1

//## begin module%3917F51B02A1.additionalIncludes preserve=no
//## end module%3917F51B02A1.additionalIncludes

//## begin module%3917F51B02A1.includes preserve=yes
#include "m4sysall_dll.hpp"
//## end module%3917F51B02A1.includes

// m4types
#include <m4types.hpp>
//## begin module%3917F51B02A1.declarations preserve=no
//## end module%3917F51B02A1.declarations

//## begin module%3917F51B02A1.additionalDeclarations preserve=yes
//## end module%3917F51B02A1.additionalDeclarations


//## begin ClEvent%3912C5840102.preface preserve=yes
//## end ClEvent%3912C5840102.preface

//## Class: ClEvent%3912C5840102
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClEvent 
{
  //## begin ClEvent%3912C5840102.initialDeclarations preserve=yes
  //## end ClEvent%3912C5840102.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEvent%1770628297; C++
      ClEvent (void* ai_pContainer, m4int32_t ai_Id)
        //## begin ClEvent::ClEvent%1770628297.hasinit preserve=no
        //## end ClEvent::ClEvent%1770628297.hasinit
        //## begin ClEvent::ClEvent%1770628297.initialization preserve=yes
		:	m_pContainer(ai_pContainer),
			m_Id(ai_Id)
        //## end ClEvent::ClEvent%1770628297.initialization
      {
        //## begin ClEvent::ClEvent%1770628297.body preserve=yes
        //## end ClEvent::ClEvent%1770628297.body
      }

      //## Operation: ClEvent%957962177
      ClEvent (const ClEvent &ai_Event)
        //## begin ClEvent::ClEvent%957962177.hasinit preserve=no
        //## end ClEvent::ClEvent%957962177.hasinit
        //## begin ClEvent::ClEvent%957962177.initialization preserve=yes
		:	m_pContainer(ai_Event.m_pContainer),
			m_Id(ai_Event.m_Id)
        //## end ClEvent::ClEvent%957962177.initialization
      {
        //## begin ClEvent::ClEvent%957962177.body preserve=yes
        //## end ClEvent::ClEvent%957962177.body
      }

      //## Operation: ClEvent%959332519
      ClEvent ()
        //## begin ClEvent::ClEvent%959332519.hasinit preserve=no
        //## end ClEvent::ClEvent%959332519.hasinit
        //## begin ClEvent::ClEvent%959332519.initialization preserve=yes
		:	m_pContainer(0),
			m_Id(0)
        //## end ClEvent::ClEvent%959332519.initialization
      {
        //## begin ClEvent::ClEvent%959332519.body preserve=yes
        //## end ClEvent::ClEvent%959332519.body
      }

    //## Destructor (specified)
      //## Operation: ~ClEvent%1774086144; C++
      virtual ~ClEvent (void )
      {
        //## begin ClEvent::~ClEvent%1774086144.body preserve=yes
        //## end ClEvent::~ClEvent%1774086144.body
      }


    //## Other Operations (specified)
      //## Operation: GetContainer%719338712; C++
      void* GetContainer (void )
      {
        //## begin ClEvent::GetContainer%719338712.body preserve=yes
		  return m_pContainer;
        //## end ClEvent::GetContainer%719338712.body
      }

      //## Operation: GetId%-749157167; C++
      m4int32_t GetId (void )
      {
        //## begin ClEvent::GetId%-749157167.body preserve=yes
		  return m_Id;
        //## end ClEvent::GetId%-749157167.body
      }

    // Additional Public Declarations
      //## begin ClEvent%3912C5840102.public preserve=yes
      //## end ClEvent%3912C5840102.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pContainer%3912C5840103
      //## begin ClEvent::m_pContainer%3912C5840103.attr preserve=no  protected: void* {VA} 
      void* m_pContainer;
      //## end ClEvent::m_pContainer%3912C5840103.attr

    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C5840121
      //## Role: ClEvent::m_Id%3912C5840123
      //## begin ClEvent::m_Id%3912C5840123.role preserve=no  protected: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_Id;
      //## end ClEvent::m_Id%3912C5840123.role

    // Additional Protected Declarations
      //## begin ClEvent%3912C5840102.protected preserve=yes
      //## end ClEvent%3912C5840102.protected

  private:
    // Additional Private Declarations
      //## begin ClEvent%3912C5840102.private preserve=yes
      //## end ClEvent%3912C5840102.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEvent%3912C5840102.implementation preserve=yes
      //## end ClEvent%3912C5840102.implementation

};

//## begin ClEvent%3912C5840102.postscript preserve=yes
//## end ClEvent%3912C5840102.postscript

//## begin ClEventOneArg%3917F4A50338.preface preserve=yes
//## end ClEventOneArg%3917F4A50338.preface

//## Class: ClEventOneArg%3917F4A50338; Parameterized Class
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClEventOneArg : public ClEvent  //## Inherits: <unnamed>%3917F4A50356
{
  //## begin ClEventOneArg%3917F4A50338.initialDeclarations preserve=yes
  //## end ClEventOneArg%3917F4A50338.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventOneArg%957872926
      ClEventOneArg (void *ai_pContainer, m4int32_t ai_iId, m4int32_t &ai_oArg)
        //## begin ClEventOneArg::ClEventOneArg%957872926.hasinit preserve=no
        //## end ClEventOneArg::ClEventOneArg%957872926.hasinit
        //## begin ClEventOneArg::ClEventOneArg%957872926.initialization preserve=yes
		:	m_oArg1(ai_oArg),
			ClEvent(ai_pContainer,ai_iId)
        //## end ClEventOneArg::ClEventOneArg%957872926.initialization
      {
        //## begin ClEventOneArg::ClEventOneArg%957872926.body preserve=yes
        //## end ClEventOneArg::ClEventOneArg%957872926.body
      }

      //## Operation: ClEventOneArg%957962178
      ClEventOneArg (const ClEventOneArg &ai_Event)
        //## begin ClEventOneArg::ClEventOneArg%957962178.hasinit preserve=no
        //## end ClEventOneArg::ClEventOneArg%957962178.hasinit
        //## begin ClEventOneArg::ClEventOneArg%957962178.initialization preserve=yes
		:	m_oArg1(ai_Event.m_oArg1),
			ClEvent(ai_Event)
        //## end ClEventOneArg::ClEventOneArg%957962178.initialization
      {
        //## begin ClEventOneArg::ClEventOneArg%957962178.body preserve=yes
        //## end ClEventOneArg::ClEventOneArg%957962178.body
      }

      //## Operation: ClEventOneArg%959332520
      ClEventOneArg ()
        //## begin ClEventOneArg::ClEventOneArg%959332520.hasinit preserve=no
        //## end ClEventOneArg::ClEventOneArg%959332520.hasinit
        //## begin ClEventOneArg::ClEventOneArg%959332520.initialization preserve=yes
        //## end ClEventOneArg::ClEventOneArg%959332520.initialization
      {
        //## begin ClEventOneArg::ClEventOneArg%959332520.body preserve=yes
        //## end ClEventOneArg::ClEventOneArg%959332520.body
      }

    // Data Members for Class Attributes

      //## Attribute: m_oArg1%3917FB8302E6
      //## begin ClEventOneArg::m_oArg1%3917FB8302E6.attr preserve=no  public: Arg1 {UA} 
	  m4int32_t m_oArg1;
      //## end ClEventOneArg::m_oArg1%3917FB8302E6.attr

    // Additional Public Declarations
      //## begin ClEventOneArg%3917F4A50338.public preserve=yes
      //## end ClEventOneArg%3917F4A50338.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventOneArg%3917F4A50338.protected preserve=yes
      //## end ClEventOneArg%3917F4A50338.protected

  private:
    // Additional Private Declarations
      //## begin ClEventOneArg%3917F4A50338.private preserve=yes
      //## end ClEventOneArg%3917F4A50338.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventOneArg%3917F4A50338.implementation preserve=yes
      //## end ClEventOneArg%3917F4A50338.implementation

};

//## begin ClEventOneArg%3917F4A50338.postscript preserve=yes
//## end ClEventOneArg%3917F4A50338.postscript

//## begin ClEventTwoArg%391801BC0047.preface preserve=yes
//## end ClEventTwoArg%391801BC0047.preface

//## Class: ClEventTwoArg%391801BC0047; Parameterized Class
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClEventTwoArg : public ClEventOneArg  //## Inherits: <unnamed>%3918072D031B
{
  //## begin ClEventTwoArg%391801BC0047.initialDeclarations preserve=yes
  //## end ClEventTwoArg%391801BC0047.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEventTwoArg%957872927
      ClEventTwoArg (void *ai_pContainer, m4int32_t ai_iId, m4int32_t &ai_oArg1, m4int32_t &ai_oArg2)
        //## begin ClEventTwoArg::ClEventTwoArg%957872927.hasinit preserve=no
        //## end ClEventTwoArg::ClEventTwoArg%957872927.hasinit
        //## begin ClEventTwoArg::ClEventTwoArg%957872927.initialization preserve=yes
		:	m_oArg2(ai_oArg2),
			ClEventOneArg(ai_pContainer,ai_iId,ai_oArg1)
        //## end ClEventTwoArg::ClEventTwoArg%957872927.initialization
      {
        //## begin ClEventTwoArg::ClEventTwoArg%957872927.body preserve=yes
        //## end ClEventTwoArg::ClEventTwoArg%957872927.body
      }

      //## Operation: ClEventTwoArg%957962176
      ClEventTwoArg (const ClEventTwoArg &ai_Event)
        //## begin ClEventTwoArg::ClEventTwoArg%957962176.hasinit preserve=no
        //## end ClEventTwoArg::ClEventTwoArg%957962176.hasinit
        //## begin ClEventTwoArg::ClEventTwoArg%957962176.initialization preserve=yes
		:	m_oArg2(ai_Event.m_oArg2),
			ClEventOneArg(ai_Event)
        //## end ClEventTwoArg::ClEventTwoArg%957962176.initialization
      {
        //## begin ClEventTwoArg::ClEventTwoArg%957962176.body preserve=yes
        //## end ClEventTwoArg::ClEventTwoArg%957962176.body
      }

      //## Operation: ClEventTwoArg%959332521
      ClEventTwoArg ()
        //## begin ClEventTwoArg::ClEventTwoArg%959332521.hasinit preserve=no
        //## end ClEventTwoArg::ClEventTwoArg%959332521.hasinit
        //## begin ClEventTwoArg::ClEventTwoArg%959332521.initialization preserve=yes
        //## end ClEventTwoArg::ClEventTwoArg%959332521.initialization
      {
        //## begin ClEventTwoArg::ClEventTwoArg%959332521.body preserve=yes
        //## end ClEventTwoArg::ClEventTwoArg%959332521.body
      }

    // Data Members for Class Attributes

      //## Attribute: m_oArg2%391807D802D1
      //## begin ClEventTwoArg::m_oArg2%391807D802D1.attr preserve=no  public: Arg2 {UA} 
	  m4int32_t m_oArg2;
      //## end ClEventTwoArg::m_oArg2%391807D802D1.attr

    // Additional Public Declarations
      //## begin ClEventTwoArg%391801BC0047.public preserve=yes
      //## end ClEventTwoArg%391801BC0047.public

  protected:
    // Additional Protected Declarations
      //## begin ClEventTwoArg%391801BC0047.protected preserve=yes
      //## end ClEventTwoArg%391801BC0047.protected

  private:
    // Additional Private Declarations
      //## begin ClEventTwoArg%391801BC0047.private preserve=yes
      //## end ClEventTwoArg%391801BC0047.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClEventTwoArg%391801BC0047.implementation preserve=yes
      //## end ClEventTwoArg%391801BC0047.implementation

};

//## begin ClEventTwoArg%391801BC0047.postscript preserve=yes
//## end ClEventTwoArg%391801BC0047.postscript

//## begin ClEventIntInt%3918285E01F7.preface preserve=yes
//## end ClEventIntInt%3918285E01F7.preface

//## Class: ClEventIntInt%3918285E01F7; Instantiated Class
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef ClEventTwoArg ClEventIntInt;

//## begin ClEventIntInt%3918285E01F7.postscript preserve=yes
//## end ClEventIntInt%3918285E01F7.postscript

//## begin module%3917F51B02A1.epilog preserve=yes
//## end module%3917F51B02A1.epilog


#endif
