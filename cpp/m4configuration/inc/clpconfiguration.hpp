//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
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
//	=========================================================
//	=====================
//## end module.cp

//## Module: ClPConfiguration; Package specification
//## Subsystem: CPM4Configuration
//## Source file: D:\eduardoma\M4Configuration\version\inc\clpconfiguration.hpp

#ifndef clpconfiguration_h
#define clpconfiguration_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include <m4configuration_dll.hpp>
#include <cloblconfiguration.hpp>
#include <configtree.hpp>

class ClCryptStore;

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


//## Class: ClPropertiesConfiguration
//## Category: M4Configuration
//## Subsystem: CPM4Configuration
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4CONFIGURATION ClPropertiesConfiguration : public ClM4ConfigTree  //## Inherits: <unnamed>
{
  //## begin ClPropertiesConfiguration.initialDeclarations preserve=yes
  //## end ClPropertiesConfiguration.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPropertiesConfiguration%913835325
      ClPropertiesConfiguration (m4pchar_t ai_szProperties);

      //## Operation: ClPropertiesConfiguration%913888151
      ClPropertiesConfiguration (ClOblConfiguration *ai_poConfiguration, UINT ai_hHandle, ClCryptStore *ai_poCriptFile = NULL);

      //## Operation: ClPropertiesConfiguration%917867896
      ClPropertiesConfiguration (ClConfiguration *ai_poConfiguration);

    //## Destructor (specified)
      //## Operation: ~ClPropertiesConfiguration%913888152
      ~ClPropertiesConfiguration ();


    //## Other Operations (specified)
      //## Operation: GetStringValue%917867897; C++
      int GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength);

    // Additional Public Declarations
      //## begin ClPropertiesConfiguration.public preserve=yes
      //## end ClPropertiesConfiguration.public

  protected:
    // Data Members for Associations

      //## Association: M4Configuration::<unnamed>%36B592A6028A
      //## Role: ClPropertiesConfiguration::m_poCryptStore
      //## begin ClPropertiesConfiguration::m_poCryptStore.role preserve=no  protected: ClCryptStore { -> RFHAN}
      ClCryptStore *m_poCryptStore;
      //## end ClPropertiesConfiguration::m_poCryptStore.role

    // Additional Protected Declarations
      //## begin ClPropertiesConfiguration.protected preserve=yes
      //## end ClPropertiesConfiguration.protected

  private:
    // Additional Private Declarations
      //## begin ClPropertiesConfiguration.private preserve=yes
      //## end ClPropertiesConfiguration.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClPropertiesConfiguration.implementation preserve=yes
      //## end ClPropertiesConfiguration.implementation

};

//## begin ClPropertiesConfiguration.postscript preserve=yes
//## end ClPropertiesConfiguration.postscript



// Class ClPropertiesConfiguration 





//## begin module.epilog preserve=yes
//## end module.epilog
#endif
