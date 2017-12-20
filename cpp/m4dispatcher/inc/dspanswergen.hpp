//## begin module%3972F06401D4.cm preserve=no
//## end module%3972F06401D4.cm

//## begin module%3972F06401D4.cp preserve=no
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
//## end module%3972F06401D4.cp

//## Module: dspanswergen%3972F06401D4; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: F:\integration\m4dispatcher\inc\dspanswergen.hpp

#ifndef dspanswergen_h
#define dspanswergen_h 1

//## begin module%3972F06401D4.additionalIncludes preserve=no
//## end module%3972F06401D4.additionalIncludes

//## begin module%3972F06401D4.includes preserve=yes
#include "m4stl.hpp"
#include <m4string.hpp>
#include <logsys.hpp>

class ClSiteNodeContainer;

//## begin module%3972F06401D4.declarations preserve=no
//## end module%3972F06401D4.declarations

//## begin module%3972F06401D4.additionalDeclarations preserve=yes
//## end module%3972F06401D4.additionalDeclarations


//## begin ClDspAnswerGen%393CF8D80387.preface preserve=yes
//## end ClDspAnswerGen%393CF8D80387.preface

//## Class: ClDspAnswerGen%393CF8D80387
//## Category: M4Dispatcher::Answer%39F44EA902AE
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%393CF8D80388;m4string_t { -> }
//## Uses: <unnamed>%393CF8D80389;basic_ostringstream { -> }
//## Uses: <unnamed>%3959B2EB007D;ClSiteNodeContainer { -> F}
//## Uses: <unnamed>%3959B3140361;ClSiteNodeInformation { -> F}
//## Uses: <unnamed>%3959B531021F;M4XMLStreamGenerator { -> F}
//## Uses: <unnamed>%3A657CDD0390;ClLogSystem { -> }

class ClDspAnswerGen 
{
  //## begin ClDspAnswerGen%393CF8D80387.initialDeclarations preserve=yes
  //## end ClDspAnswerGen%393CF8D80387.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateAnswer%963825080
      virtual m4return_t GenerateAnswer (ClSiteNodeContainer &ai_oSiteNodeContainer, m4void_t *&ao_strStream, m4return_t ai_iRet = M4_SUCCESS, ClLogSystem *ai_poLog = NULL) = 0;

    // Additional Public Declarations
      //## begin ClDspAnswerGen%393CF8D80387.public preserve=yes
      //## end ClDspAnswerGen%393CF8D80387.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_bTrace%3969B3E6000C
      //## begin ClDspAnswerGen::m_bTrace%3969B3E6000C.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bTrace;
      //## end ClDspAnswerGen::m_bTrace%3969B3E6000C.attr

    // Additional Protected Declarations
      //## begin ClDspAnswerGen%393CF8D80387.protected preserve=yes
      //## end ClDspAnswerGen%393CF8D80387.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_strBuffer%393CF8D803A8
      //## begin ClDspAnswerGen::m_strBuffer%393CF8D803A8.attr preserve=no  private: strstream {UA} 
      strstream m_strBuffer;
      //## end ClDspAnswerGen::m_strBuffer%393CF8D803A8.attr

    // Additional Private Declarations
      //## begin ClDspAnswerGen%393CF8D80387.private preserve=yes
      //## end ClDspAnswerGen%393CF8D80387.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDspAnswerGen%393CF8D80387.implementation preserve=yes
      //## end ClDspAnswerGen%393CF8D80387.implementation

};

//## begin ClDspAnswerGen%393CF8D80387.postscript preserve=yes
//## end ClDspAnswerGen%393CF8D80387.postscript

// Class ClDspAnswerGen 

//## begin module%3972F06401D4.epilog preserve=yes
//## end module%3972F06401D4.epilog


#endif
