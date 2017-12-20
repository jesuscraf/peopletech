//## begin module%347EEA0E004A.cm preserve=no
//## end module%347EEA0E004A.cm

//## begin module%347EEA0E004A.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Supervisor
//	 File:              ClSupervisor.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980109
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%347EEA0E004A.cp

//## Module: clsupervisor%347EEA0E004A; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clsupervisor.hpp

#ifndef clsupervisor_h
#define clsupervisor_h 1

//## begin module%347EEA0E004A.additionalIncludes preserve=no
//## end module%347EEA0E004A.additionalIncludes

//## begin module%347EEA0E004A.includes preserve=yes
#include "m4stl.hpp"
//## end module%347EEA0E004A.includes

// m4thread
#include <m4thread.hpp>
// m4condition
#include <m4condition.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>

class ClConfiguration;
class ClSystem;

//## begin module%347EEA0E004A.declarations preserve=no
//## end module%347EEA0E004A.declarations

//## begin module%347EEA0E004A.additionalDeclarations preserve=yes
//## end module%347EEA0E004A.additionalDeclarations


//## begin ClCommandsmapTypedef%37A8463102F4.preface preserve=yes
//## end ClCommandsmapTypedef%37A8463102F4.preface

//## Class: ClCommandsmapTypedef%37A8463102F4
//## Category: Supervisor%34B50D4302F6
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A8465A0285;multimap { -> }
//## Uses: <unnamed>%37A84E600300; { -> F}
//## Uses: <unnamed>%39214F190065;ClSSInterfaz { -> }

typedef multimap < ClSSInterfaz *, m4uint32_t , less < ClSSInterfaz*> > ClCommandsmapTypedef;

//## begin ClCommandsmapTypedef%37A8463102F4.postscript preserve=yes
//## end ClCommandsmapTypedef%37A8463102F4.postscript

//## begin ClCommandsMap%37A843BE0270.preface preserve=yes
//## end ClCommandsMap%37A843BE0270.preface

//## Class: ClCommandsMap%37A843BE0270
//## Category: Supervisor%34B50D4302F6
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCommandsMap : public ClCommandsmapTypedef  //## Inherits: <unnamed>%37A84D710339
{
  //## begin ClCommandsMap%37A843BE0270.initialDeclarations preserve=yes
  //## end ClCommandsMap%37A843BE0270.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: AddCommand%933760133
      void AddCommand (ClSSInterfaz *ai_poSubsystem, m4uint32_t ai_iCommand);

      //## Operation: ExecuteAll%933760134
      m4return_t ExecuteAll ();

    // Additional Public Declarations
      //## begin ClCommandsMap%37A843BE0270.public preserve=yes
      //## end ClCommandsMap%37A843BE0270.public

  protected:
    // Additional Protected Declarations
      //## begin ClCommandsMap%37A843BE0270.protected preserve=yes
      //## end ClCommandsMap%37A843BE0270.protected

  private:
    // Additional Private Declarations
      //## begin ClCommandsMap%37A843BE0270.private preserve=yes
      //## end ClCommandsMap%37A843BE0270.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCommandsMap%37A843BE0270.implementation preserve=yes
      //## end ClCommandsMap%37A843BE0270.implementation

};

//## begin ClCommandsMap%37A843BE0270.postscript preserve=yes
//## end ClCommandsMap%37A843BE0270.postscript

//## begin ClCSSupervisor%3455E6740113.preface preserve=yes
//## end ClCSSupervisor%3455E6740113.preface

//## Class: ClCSSupervisor%3455E6740113
//	Supervisor thread that travel arround the server looking for non closed
//	elements and other events.
//## Category: Supervisor%34B50D4302F6
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BDD3F80325;m4int32_t { -> F}
//## Uses: <unnamed>%34BE2B1F0364;M4ClCrono { -> F}
//## Uses: <unnamed>%37A8571B0318;ClConfiguration { -> F}
//## Uses: <unnamed>%3843C9FD0153;ClServerEvent { -> F}

class ClCSSupervisor : public M4Thread  //## Inherits: <unnamed>%3455FB890359
{
  //## begin ClCSSupervisor%3455E6740113.initialDeclarations preserve=yes
  //## end ClCSSupervisor%3455E6740113.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCSSupervisor%878114679; C++
      //	Default constructor
      ClCSSupervisor (m4uint32_t ai_iDelayTime);

    //## Destructor (specified)
      //## Operation: ~ClCSSupervisor%878114680
      ~ClCSSupervisor ();


    //## Other Operations (specified)
      //## Operation: Run%884283581; C++
      virtual void Run (void );

      //## Operation: RegisterCommands%933760136
      //	En inicialización y a partir de la configuración se determina la lista de
      //	comandos a ejecutar en cada ciclo del supervisor.
      m4return_t RegisterCommands (ClConfiguration *ai_poConfiguration);

      //## Operation: Update%943966401
      virtual void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

    // Additional Public Declarations
      //## begin ClCSSupervisor%3455E6740113.public preserve=yes
      //## end ClCSSupervisor%3455E6740113.public

  protected:
    // Data Members for Associations

      //## Association: Supervisor::<unnamed>%34BC8FB402BB
      //## Role: ClCSSupervisor::m_pSystem%34BC8FB500E6
      //## begin ClCSSupervisor::m_pSystem%34BC8FB500E6.role preserve=no  protected: ClSystem {1 -> 1RFHAN}
      ClSystem *m_pSystem;
      //## end ClCSSupervisor::m_pSystem%34BC8FB500E6.role

      //## Association: Supervisor::<unnamed>%37A84E55023C
      //## Role: ClCSSupervisor::m_oCommandList%37A84E56027A
      //	Lsita de comandos a ejecutar, clasificados por subssitemas en los cuales han
      //	de ser ejecutados
      //## begin ClCSSupervisor::m_oCommandList%37A84E56027A.role preserve=no  protected: ClCommandsMap { -> VHAN}
      ClCommandsMap m_oCommandList;
      //## end ClCSSupervisor::m_oCommandList%37A84E56027A.role

    // Additional Protected Declarations
      //## begin ClCSSupervisor%3455E6740113.protected preserve=yes
      //## end ClCSSupervisor%3455E6740113.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iDelayTime%37A547320078
      //## begin ClCSSupervisor::m_iDelayTime%37A547320078.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iDelayTime;
      //## end ClCSSupervisor::m_iDelayTime%37A547320078.attr

    // Data Members for Associations

      //## Association: Supervisor::<unnamed>%3843C8980268
      //## Role: ClCSSupervisor::m_oStarter%3843C899011F
      //	EL supervisor ha de permanecer parado hasta que el servisor haya terminado
      //	de arrancar por completo. Para ello se usa una condition
      //## begin ClCSSupervisor::m_oStarter%3843C899011F.role preserve=no  private: ClCondition { -> VHAN}
      ClCondition m_oStarter;
      //## end ClCSSupervisor::m_oStarter%3843C899011F.role

    // Additional Private Declarations
      //## begin ClCSSupervisor%3455E6740113.private preserve=yes
      //## end ClCSSupervisor%3455E6740113.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCSSupervisor%3455E6740113.implementation preserve=yes
      //## end ClCSSupervisor%3455E6740113.implementation

    friend class ClSupervisorSS;
};

//## begin ClCSSupervisor%3455E6740113.postscript preserve=yes
//## end ClCSSupervisor%3455E6740113.postscript

// Class ClCommandsMap 

// Class ClCSSupervisor 

//## begin module%347EEA0E004A.epilog preserve=yes
//## end module%347EEA0E004A.epilog


#endif
