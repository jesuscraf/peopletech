//## begin module%37E603040365.cm preserve=no
//## end module%37E603040365.cm

//## begin module%37E603040365.cp preserve=no
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
//## end module%37E603040365.cp

//## Module: jssessionsettings%37E603040365; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\version\inc\jssessionsettings.hpp

#ifndef jssessionsettings_h
#define jssessionsettings_h 1

//## begin module%37E603040365.additionalIncludes preserve=no
//## end module%37E603040365.additionalIncludes

//## begin module%37E603040365.includes preserve=yes
//## end module%37E603040365.includes

// m4string
#include <m4string.hpp>
//## begin module%37E603040365.declarations preserve=no
//## end module%37E603040365.declarations

//## begin module%37E603040365.additionalDeclarations preserve=yes
//## end module%37E603040365.additionalDeclarations


//## begin ClJSSessionSettings%375CD907024E.preface preserve=yes
//## end ClJSSessionSettings%375CD907024E.preface

//## Class: ClJSSessionSettings%375CD907024E
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClJSSessionSettings 
{
  //## begin ClJSSessionSettings%375CD907024E.initialDeclarations preserve=yes
  //## end ClJSSessionSettings%375CD907024E.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ClJSSessionSettings%375CD907024E.public preserve=yes
      //## end ClJSSessionSettings%375CD907024E.public

  protected:
    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E7613E0296
      //## Role: ClJSSessionSettings::m_stUser%37E7613F0189
      //## begin ClJSSessionSettings::m_stUser%37E7613F0189.role preserve=no  protected: M4ClString { -> RHAN}
      M4ClString *m_stUser;
      //## end ClJSSessionSettings::m_stUser%37E7613F0189.role

      //## Association: M4JSExeJob::M4JSJob::stRole%37E76183029F
      //## Role: ClJSSessionSettings::m_stRole%37E761840160
      //## begin ClJSSessionSettings::m_stRole%37E761840160.role preserve=no  protected: M4ClString { -> RHAN}
      M4ClString *m_stRole;
      //## end ClJSSessionSettings::m_stRole%37E761840160.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E761E5026E
      //## Role: ClJSSessionSettings::m_stContext%37E761E7027B
      //## begin ClJSSessionSettings::m_stContext%37E761E7027B.role preserve=no  protected: M4ClString { -> RHAN}
      M4ClString *m_stContext;
      //## end ClJSSessionSettings::m_stContext%37E761E7027B.role

    // Additional Protected Declarations
      //## begin ClJSSessionSettings%375CD907024E.protected preserve=yes
      //## end ClJSSessionSettings%375CD907024E.protected

  private:
    // Additional Private Declarations
      //## begin ClJSSessionSettings%375CD907024E.private preserve=yes
      //## end ClJSSessionSettings%375CD907024E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSSessionSettings%375CD907024E.implementation preserve=yes
      //## end ClJSSessionSettings%375CD907024E.implementation

};

//## begin ClJSSessionSettings%375CD907024E.postscript preserve=yes
//## end ClJSSessionSettings%375CD907024E.postscript

// Class ClJSSessionSettings 

//## begin module%37E603040365.epilog preserve=yes
//## end module%37E603040365.epilog


#endif
