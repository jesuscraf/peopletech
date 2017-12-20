//## begin module%37A5B1FC0213.cm preserve=no
//## end module%37A5B1FC0213.cm

//## begin module%37A5B1FC0213.cp preserve=no
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
//## end module%37A5B1FC0213.cp

//## Module: clssfactoryinterfaz%37A5B1FC0213; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\clssfactoryinterfaz.hpp

#ifndef clssfactoryinterfaz_h
#define clssfactoryinterfaz_h 1

//## begin module%37A5B1FC0213.additionalIncludes preserve=no
//## end module%37A5B1FC0213.additionalIncludes

//## begin module%37A5B1FC0213.includes preserve=yes
#include <m4subsystems_dll.hpp>
//## end module%37A5B1FC0213.includes

// ClConfiguration
#include <clconfiguration.hpp>
// propertiestable
#include <propertiestable.hpp>

class ClSSInterfaz;

//## begin module%37A5B1FC0213.declarations preserve=no
//## end module%37A5B1FC0213.declarations

//## begin module%37A5B1FC0213.additionalDeclarations preserve=yes
//## end module%37A5B1FC0213.additionalDeclarations


//## begin ClSSFactoryInterfaz%37A5AFE203B4.preface preserve=yes
//## end ClSSFactoryInterfaz%37A5AFE203B4.preface

//## Class: ClSSFactoryInterfaz%37A5AFE203B4
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A5B2FB0219;ClConfiguration { -> }
//## Uses: <unnamed>%37A5B36602C8;ClTables { -> }
//## Uses: <unnamed>%37A6AD18039E;ClSSInterfaz { -> F}

class M4_DECL_M4SUBSYSTEMS ClSSFactoryInterfaz 
{
  //## begin ClSSFactoryInterfaz%37A5AFE203B4.initialDeclarations preserve=yes
  //## end ClSSFactoryInterfaz%37A5AFE203B4.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateSubsystem%933578431
      virtual ClSSInterfaz * GenerateSubsystem (m4uint16_t ai_IType, ClConfiguration *ai_poConfig, ClTables *ai_poTables = NULL) = 0;

      //## Operation: ShutDownSubsystem%933578432
      virtual m4return_t ShutDownSubsystem (ClSSInterfaz *ai_poSubsystem) = 0;

    // Additional Public Declarations
      //## begin ClSSFactoryInterfaz%37A5AFE203B4.public preserve=yes
      //## end ClSSFactoryInterfaz%37A5AFE203B4.public

  protected:
    // Additional Protected Declarations
      //## begin ClSSFactoryInterfaz%37A5AFE203B4.protected preserve=yes
      //## end ClSSFactoryInterfaz%37A5AFE203B4.protected

  private:
    // Additional Private Declarations
      //## begin ClSSFactoryInterfaz%37A5AFE203B4.private preserve=yes
      //## end ClSSFactoryInterfaz%37A5AFE203B4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSFactoryInterfaz%37A5AFE203B4.implementation preserve=yes
      //## end ClSSFactoryInterfaz%37A5AFE203B4.implementation

};

//## begin ClSSFactoryInterfaz%37A5AFE203B4.postscript preserve=yes
//## end ClSSFactoryInterfaz%37A5AFE203B4.postscript

// Class ClSSFactoryInterfaz 

//## begin module%37A5B1FC0213.epilog preserve=yes
//## end module%37A5B1FC0213.epilog


#endif
