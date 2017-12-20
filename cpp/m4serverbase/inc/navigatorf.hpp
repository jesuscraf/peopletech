//## begin module%35580A4A03CE.cm preserve=no
//## end module%35580A4A03CE.cm

//## begin module%35580A4A03CE.cp preserve=no
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
//## end module%35580A4A03CE.cp

//## Module: NavigatorF%35580A4A03CE; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\navigatorf.hpp

#ifndef navigatorf_h
#define navigatorf_h 1

//## begin module%35580A4A03CE.additionalIncludes preserve=no
//## end module%35580A4A03CE.additionalIncludes

//## begin module%35580A4A03CE.includes preserve=yes
//#include <m4subsystems_dll.hpp>
#include <clconfiguration.hpp>
#include <cllminterface.hpp>
//## end module%35580A4A03CE.includes

// m4types
#include <m4types.hpp>
//## begin module%35580A4A03CE.declarations preserve=no
//## end module%35580A4A03CE.declarations

//## begin module%35580A4A03CE.additionalDeclarations preserve=yes
//## end module%35580A4A03CE.additionalDeclarations


//## begin ClSSInterfazFirewall%39239B680344.preface preserve=yes
//## end ClSSInterfazFirewall%39239B680344.preface

//## Class: ClSSInterfazFirewall%39239B680344
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSSInterfazFirewall 
{
  //## begin ClSSInterfazFirewall%39239B680344.initialDeclarations preserve=yes
  //## end ClSSInterfazFirewall%39239B680344.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetConfigNumericValue%958635014
      virtual m4return_t GetConfigNumericValue (m4pchar_t ai_pszName, m4double_t &ao_rdNumber) = 0;

      //## Operation: GetConfigStringValue%958635015
      virtual m4return_t GetConfigStringValue (m4pchar_t ai_pszName, m4pchar_t ao_pszString, m4size_t ai_iStringSize, m4size_t& ao_riStringLength) = 0;

      //## Operation: SetConfigNumericProperty%958635016
      virtual m4return_t SetConfigNumericProperty (m4pchar_t ai_szProperty, m4double_t ai_iValue) = 0;

      //## Operation: SetConfigStringProperty%958635017
      virtual m4return_t SetConfigStringProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue) = 0;

    // Additional Public Declarations
      //## begin ClSSInterfazFirewall%39239B680344.public preserve=yes
      //## end ClSSInterfazFirewall%39239B680344.public

  protected:
    // Additional Protected Declarations
      //## begin ClSSInterfazFirewall%39239B680344.protected preserve=yes
      //## end ClSSInterfazFirewall%39239B680344.protected

  private:
    // Additional Private Declarations
      //## begin ClSSInterfazFirewall%39239B680344.private preserve=yes
      //## end ClSSInterfazFirewall%39239B680344.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSInterfazFirewall%39239B680344.implementation preserve=yes
      //## end ClSSInterfazFirewall%39239B680344.implementation

};

//## begin ClSSInterfazFirewall%39239B680344.postscript preserve=yes
//## end ClSSInterfazFirewall%39239B680344.postscript

//## begin ClNavigatorFirewall%355807B9013E.preface preserve=yes
//## end ClNavigatorFirewall%355807B9013E.preface

//## Class: ClNavigatorFirewall%355807B9013E
//	This is an abstract class for using subsystem tree mechanisims outside
//	server executable. Everybody who wants to use those mechanisims, uses a
//	reference to this abstract class and then someone (probably the server)
//	initializes the reference with an instatiable class object
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35583FFF01F7;m4uint32_t { -> }
//## Uses: <unnamed>%39239B880278;ClSSInterfazFirewall { -> }

class ClNavigatorFirewall 
{
  //## begin ClNavigatorFirewall%355807B9013E.initialDeclarations preserve=yes
  //## end ClNavigatorFirewall%355807B9013E.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetSubsystemByType%894962379
      //	Busca en todo el arbol de subsystemas el primer subsitema que pertenece al
      //	tipo que se poasa como parámetro.
      virtual ClSSInterfazFirewall * GetSubsystemByType (m4uint32_t ai_iID) = 0;

      //## Operation: GetSubsystemByName%895825801
      //	Busca en todo el árbol de subsitemas, la primera ocurrencia de un subsistema
      //	con el nombre que se indica en el parámetro
      virtual ClSSInterfazFirewall * GetSubsystemByName (m4pchar_t ai_szName) = 0;

      //## Operation: GetSubsystemElement%896526472
      //	Primeramente busca por nombre el subsitema que encaje con el nombre de
      //	subsistema que se pasa como primer parámetro, una vez encontrado este
      //	devuelve el elemento identificado por la cadena que se pasa como segundo
      //	parámetro
      virtual void * GetSubsystemElement (m4pchar_t ai_szSubsystemName, m4pchar_t ai_szElementName = NULL) = 0;

      //## Operation: PersistConfiguration%901796093
      virtual m4return_t PersistConfiguration (m4pchar_t ai_szProperty, m4pchar_t ai_szValue, UINT ai_hBlock) = 0;

      //## Operation: GetSubsystemById%901966070
      //	Devuelve el subsistema cuyo ID coincida con el pasado en forma de parámetro
      //	(único en el árbol de subsistemas, puesto que los Id son únicos también)
      virtual ClSSInterfazFirewall * GetSubsystemById (m4uint32_t ai_lId) = 0;

      //## Operation: GetLocalMonitor%958635018
      virtual ClLocalMonitorInterface * GetLocalMonitor () = 0;

    // Additional Public Declarations
      //## begin ClNavigatorFirewall%355807B9013E.public preserve=yes
      //## end ClNavigatorFirewall%355807B9013E.public

  protected:
    // Additional Protected Declarations
      //## begin ClNavigatorFirewall%355807B9013E.protected preserve=yes
      //## end ClNavigatorFirewall%355807B9013E.protected

  private:
    // Additional Private Declarations
      //## begin ClNavigatorFirewall%355807B9013E.private preserve=yes
      //## end ClNavigatorFirewall%355807B9013E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNavigatorFirewall%355807B9013E.implementation preserve=yes
      //## end ClNavigatorFirewall%355807B9013E.implementation

};

//## begin ClNavigatorFirewall%355807B9013E.postscript preserve=yes
//## end ClNavigatorFirewall%355807B9013E.postscript

// Class ClSSInterfazFirewall 

// Class ClNavigatorFirewall 

//## begin module%35580A4A03CE.epilog preserve=yes
//## end module%35580A4A03CE.epilog


#endif
