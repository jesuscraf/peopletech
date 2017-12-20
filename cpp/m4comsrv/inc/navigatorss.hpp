//## begin module%35580A64006E.cm preserve=no
//## end module%35580A64006E.cm

//## begin module%35580A64006E.cp preserve=no
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
//## end module%35580A64006E.cp

//## Module: NavigatorSS%35580A64006E; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\navigatorss.hpp

#ifndef navigatorss_h
#define navigatorss_h 1

//## begin module%35580A64006E.additionalIncludes preserve=no
//## end module%35580A64006E.additionalIncludes

#include <clssinterfaz.hpp>
#include <navigatorf.hpp>



//## begin module%35580A64006E.declarations preserve=no
//## end module%35580A64006E.declarations

//## begin module%35580A64006E.additionalDeclarations preserve=yes
//## end module%35580A64006E.additionalDeclarations


//## begin ClNavigatorSS%3558080F0318.preface preserve=yes
//## end ClNavigatorSS%3558080F0318.preface

//## Class: ClNavigatorSS%3558080F0318
//	That is the instanciable class version of navigator. That is the class in
//	which are impolemented all the methods for looking for subsistems and
//	components
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%358645250182;ClSystem { -> F}

class ClNavigatorSS : public ClNavigatorFirewall  //## Inherits: <unnamed>%3558081D026E
{
  //## begin ClNavigatorSS%3558080F0318.initialDeclarations preserve=yes
  //## end ClNavigatorSS%3558080F0318.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetSubsystemByType%894962375
      ClSSInterfazFirewall * GetSubsystemByType (m4uint32_t ai_iID);

      //## Operation: GetSubsystemByName%895825800
      ClSSInterfazFirewall * GetSubsystemByName (m4pchar_t ai_szName);

      //## Operation: GetSubsystemElement%896526471
      void * GetSubsystemElement (m4pchar_t ai_szSubsystemName, m4pchar_t ai_szElementName = NULL);

      //## Operation: PersistConfiguration%901796091
      m4return_t PersistConfiguration (m4pchar_t ai_szProperty, m4pchar_t ai_szValue, UINT ai_hBlock);

      //## Operation: RegisterSubsystem%901796092
      m4return_t RegisterSubsystem (m4uint32_t ai_lIdParent, m4uint32_t ai_lSubsystemType, ClConfiguration *ai_poConfiguration, ClSSInterfaz * &ao_poSubsystem);

      //## Operation: GetSubsystemById%901966069
      ClSSInterfazFirewall * GetSubsystemById (m4uint32_t ai_lId);

      //## Operation: RegisterSubsystem%918583518
      m4return_t RegisterSubsystem (ClSSInterfaz *  ai_poContainer, 	// Este parámetro es el subsystema que va a contener al nuevo subsystema creado
      ClSSInterfaz *  ai_poNewNode);

      //## Operation: GetLocalMonitor%958638813
      ClLocalMonitorInterface * GetLocalMonitor ();

    // Additional Public Declarations
      //## begin ClNavigatorSS%3558080F0318.public preserve=yes
      //## end ClNavigatorSS%3558080F0318.public

  protected:
    // Additional Protected Declarations
      //## begin ClNavigatorSS%3558080F0318.protected preserve=yes
      //## end ClNavigatorSS%3558080F0318.protected

  private:
    // Additional Private Declarations
      //## begin ClNavigatorSS%3558080F0318.private preserve=yes
      //## end ClNavigatorSS%3558080F0318.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNavigatorSS%3558080F0318.implementation preserve=yes
      //## end ClNavigatorSS%3558080F0318.implementation

};

//## begin ClNavigatorSS%3558080F0318.postscript preserve=yes
//## end ClNavigatorSS%3558080F0318.postscript

// Class ClNavigatorSS 

//## begin module%35580A64006E.epilog preserve=yes
//## end module%35580A64006E.epilog


#endif
