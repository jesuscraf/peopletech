//## begin module%38F20BE90327.cm preserve=no
//## end module%38F20BE90327.cm

//## begin module%38F20BE90327.cp preserve=no
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
//## end module%38F20BE90327.cp

//## Module: clssaction%38F20BE90327; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\clssaction.hpp

#ifndef clssaction_h
#define clssaction_h 1

//## begin module%38F20BE90327.additionalIncludes preserve=no
//## end module%38F20BE90327.additionalIncludes

//## begin module%38F20BE90327.includes preserve=yes
//## end module%38F20BE90327.includes

// clbaseaction
#include <clbaseaction.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
//## begin module%38F20BE90327.declarations preserve=no
//## end module%38F20BE90327.declarations

//## begin module%38F20BE90327.additionalDeclarations preserve=yes
//## end module%38F20BE90327.additionalDeclarations


//## begin ClSSAction%38F209F60095.preface preserve=yes
//## end ClSSAction%38F209F60095.preface

//## Class: ClSSAction%38F209F60095
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F343670049;IGarbageable { -> }

class M4_DECL_M4SUBSYSTEMS ClSSAction : public ClBaseAction  //## Inherits: <unnamed>%38F4441D036B
{
  //## begin ClSSAction%38F209F60095.initialDeclarations preserve=yes
  //## end ClSSAction%38F209F60095.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSSAction%955440314
      ClSSAction (ClSSImplementation *ai_poExecutive, m4bool_t ai_bDeleteParamList = M4_FALSE);


    //## Other Operations (specified)
      //## Operation: Execute%955384505
      virtual m4return_t Execute () = 0;

    // Additional Public Declarations
      //## begin ClSSAction%38F209F60095.public preserve=yes
      //## end ClSSAction%38F209F60095.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iNParams%38F33B2E00D8
      //## begin ClSSAction::m_iNParams%38F33B2E00D8.attr preserve=no  protected: m4uint32_t {UA} 
      m4uint32_t m_iNParams;
      //## end ClSSAction::m_iNParams%38F33B2E00D8.attr

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%38F2DC1B0207
      //## Role: ClSSAction::m_poSSProcessor%38F2DC1C000A
      //## begin ClSSAction::m_poSSProcessor%38F2DC1C000A.role preserve=no  protected: ClSSImplementation { -> RHAN}
      ClSSImplementation *m_poSSProcessor;
      //## end ClSSAction::m_poSSProcessor%38F2DC1C000A.role

    // Additional Protected Declarations
      //## begin ClSSAction%38F209F60095.protected preserve=yes
      //## end ClSSAction%38F209F60095.protected

  private:
    // Additional Private Declarations
      //## begin ClSSAction%38F209F60095.private preserve=yes
      //## end ClSSAction%38F209F60095.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSAction%38F209F60095.implementation preserve=yes
      //## end ClSSAction%38F209F60095.implementation

};

//## begin ClSSAction%38F209F60095.postscript preserve=yes
//## end ClSSAction%38F209F60095.postscript

// Class ClSSAction 

//## begin module%38F20BE90327.epilog preserve=yes
//## end module%38F20BE90327.epilog


#endif
