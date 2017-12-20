//## begin module%3AE01F550061.cm preserve=no
//## end module%3AE01F550061.cm

//## begin module%3AE01F550061.cp preserve=no
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
//## end module%3AE01F550061.cp

//## Module: cancelevent%3AE01F550061; Package specification
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Source file: F:\integration\m4exeforchannel\inc\cancelevent.hpp

#ifndef cancelevent_h
#define cancelevent_h 1

//## begin module%3AE01F550061.additionalIncludes preserve=no
//## end module%3AE01F550061.additionalIncludes

//## begin module%3AE01F550061.includes preserve=yes
//## end module%3AE01F550061.includes

// serverevent
#include <serverevent.hpp>
//## begin module%3AE01F550061.declarations preserve=no
//## end module%3AE01F550061.declarations

//## begin module%3AE01F550061.additionalDeclarations preserve=yes
//## end module%3AE01F550061.additionalDeclarations


//## begin ClCancelRequestEvent%3AE01E7C033A.preface preserve=yes
//## end ClCancelRequestEvent%3AE01E7C033A.preface

//## Class: ClCancelRequestEvent%3AE01E7C033A
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCancelRequestEvent : public ClServerEvent  //## Inherits: <unnamed>%3AE01E9201F1
{
  //## begin ClCancelRequestEvent%3AE01E7C033A.initialDeclarations preserve=yes
  //## end ClCancelRequestEvent%3AE01E7C033A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetSessionId%987766678
      m4uint32_t GetSessionId ();

      //## Operation: GetRequestId%987766679
      m4uint32_t GetRequestId ();

      //## Operation: SetRequestId%987766680
      void SetRequestId (m4uint32_t ai_RequestId);

      //## Operation: SetSessionId%987766681
      void SetSessionId (m4uint32_t ai_iSessionId);

    // Additional Public Declarations
      //## begin ClCancelRequestEvent%3AE01E7C033A.public preserve=yes
      //## end ClCancelRequestEvent%3AE01E7C033A.public

  protected:
    // Additional Protected Declarations
      //## begin ClCancelRequestEvent%3AE01E7C033A.protected preserve=yes
      //## end ClCancelRequestEvent%3AE01E7C033A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iRequestId%3AE01FFF0174
      //## begin ClCancelRequestEvent::m_iRequestId%3AE01FFF0174.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iRequestId;
      //## end ClCancelRequestEvent::m_iRequestId%3AE01FFF0174.attr

      //## Attribute: m_iSessionId%3AE0201601B3
      //## begin ClCancelRequestEvent::m_iSessionId%3AE0201601B3.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iSessionId;
      //## end ClCancelRequestEvent::m_iSessionId%3AE0201601B3.attr

    // Additional Private Declarations
      //## begin ClCancelRequestEvent%3AE01E7C033A.private preserve=yes
      //## end ClCancelRequestEvent%3AE01E7C033A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCancelRequestEvent%3AE01E7C033A.implementation preserve=yes
      //## end ClCancelRequestEvent%3AE01E7C033A.implementation

};

//## begin ClCancelRequestEvent%3AE01E7C033A.postscript preserve=yes
//## end ClCancelRequestEvent%3AE01E7C033A.postscript

// Class ClCancelRequestEvent 


//## Other Operations (inline)
inline m4uint32_t ClCancelRequestEvent::GetSessionId ()
{
  //## begin ClCancelRequestEvent::GetSessionId%987766678.body preserve=yes
	return m_iSessionId ;
  //## end ClCancelRequestEvent::GetSessionId%987766678.body
}

inline m4uint32_t ClCancelRequestEvent::GetRequestId ()
{
  //## begin ClCancelRequestEvent::GetRequestId%987766679.body preserve=yes
	return m_iRequestId ;
  //## end ClCancelRequestEvent::GetRequestId%987766679.body
}

inline void ClCancelRequestEvent::SetRequestId (m4uint32_t ai_RequestId)
{
  //## begin ClCancelRequestEvent::SetRequestId%987766680.body preserve=yes
	m_iRequestId = ai_RequestId ;
  //## end ClCancelRequestEvent::SetRequestId%987766680.body
}

inline void ClCancelRequestEvent::SetSessionId (m4uint32_t ai_iSessionId)
{
  //## begin ClCancelRequestEvent::SetSessionId%987766681.body preserve=yes
	m_iSessionId = ai_iSessionId ;
  //## end ClCancelRequestEvent::SetSessionId%987766681.body
}

//## begin module%3AE01F550061.epilog preserve=yes
//## end module%3AE01F550061.epilog


#endif
