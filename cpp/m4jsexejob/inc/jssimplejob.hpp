//## begin module%37E602B502F3.cm preserve=no
//## end module%37E602B502F3.cm

//## begin module%37E602B502F3.cp preserve=no
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
//## end module%37E602B502F3.cp

//## Module: jssimplejob%37E602B502F3; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\version\inc\jssimplejob.hpp

#ifndef jssimplejob_h
#define jssimplejob_h 1

//## begin module%37E602B502F3.additionalIncludes preserve=no
//## end module%37E602B502F3.additionalIncludes

//## begin module%37E602B502F3.includes preserve=yes
//## end module%37E602B502F3.includes

// m4jsjob_dll
#include <m4jsexejob_dll.hpp>
// jsjob
#include <jsjob.hpp>

class M4ClString;

//## begin module%37E602B502F3.declarations preserve=no
//## end module%37E602B502F3.declarations

//## begin module%37E602B502F3.additionalDeclarations preserve=yes
#define LN4_CODE_TYPE 0
#define SYSTEM_CODE_TYPE 1
//## end module%37E602B502F3.additionalDeclarations


//## begin ClJSSimpleJob%375BE74303CD.preface preserve=yes
//## end ClJSSimpleJob%375BE74303CD.preface

//## Class: ClJSSimpleJob%375BE74303CD
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSSimpleJob : public ClJSJob  //## Inherits: <unnamed>%37A025870389
{
  //## begin ClJSSimpleJob%375BE74303CD.initialDeclarations preserve=yes
  //## end ClJSSimpleJob%375BE74303CD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSSimpleJob%937821613
      //	Constructor de la clase.
      //	Recibe 4 parámetros:
      //	   - Identificador de la planificación (idSchedJob)
      //	   - Identificador de la ejecución (idExecution)
      //	   - Identificador del trabajo (id)
      //	   - Identificador local en la estructura (localId). Por defecto vale 0
      ClJSSimpleJob (const m4uint_t &ai_uiId);


    //## Other Operations (specified)
      //## Operation: PrintOn%938424582
      void PrintOn (ostream &ai_ostream);

      //## Operation: GetNextExecutableTask%938600381
      ClJSJob * GetNextExecutableTask ();

      //## Operation: GetTaskWithLocalId%939135441
      ClJSJob * GetTaskWithLocalId (const m4uint32_t &ai_uiLocalId);

    // Additional Public Declarations
      //## begin ClJSSimpleJob%375BE74303CD.public preserve=yes
      //## end ClJSSimpleJob%375BE74303CD.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uiCodeType%37A085D2035C
      //	El tipo de código puede ser:
      //	0 - LN4
      //	1 - Comandos del subsistema
      //## begin ClJSSimpleJob::m_uiCodeType%37A085D2035C.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_uiCodeType;
      //## end ClJSSimpleJob::m_uiCodeType%37A085D2035C.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E765A100A6
      //## Role: ClJSSimpleJob::m_stCode%37E765A20011
      //## begin ClJSSimpleJob::m_stCode%37E765A20011.role preserve=no  protected: M4ClString { -> RFHAN}
      M4ClString *m_stCode;
      //## end ClJSSimpleJob::m_stCode%37E765A20011.role

    // Additional Protected Declarations
      //## begin ClJSSimpleJob%375BE74303CD.protected preserve=yes
      //## end ClJSSimpleJob%375BE74303CD.protected

  private:
    // Additional Private Declarations
      //## begin ClJSSimpleJob%375BE74303CD.private preserve=yes
      //## end ClJSSimpleJob%375BE74303CD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSSimpleJob%375BE74303CD.implementation preserve=yes
      //## end ClJSSimpleJob%375BE74303CD.implementation

};

//## begin ClJSSimpleJob%375BE74303CD.postscript preserve=yes
//## end ClJSSimpleJob%375BE74303CD.postscript

// Class ClJSSimpleJob 

//## begin module%37E602B502F3.epilog preserve=yes
//## end module%37E602B502F3.epilog


#endif
