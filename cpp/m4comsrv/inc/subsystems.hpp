//## begin module%349293A2028F.cm preserve=no
//## end module%349293A2028F.cm

//## begin module%349293A2028F.cp preserve=no
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
//## end module%349293A2028F.cp

//## Module: subsystems%349293A2028F; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\subsystems.hpp

#ifndef subsystems_h
#define subsystems_h 1

//## begin module%349293A2028F.additionalIncludes preserve=no
//## end module%349293A2028F.additionalIncludes

//## begin module%349293A2028F.includes preserve=yes
#include <logsys.hpp>
//## end module%349293A2028F.includes

// ClConfiguration
#include <clconfiguration.hpp>
// clsupervisor
#include <clsupervisor.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%349293A2028F.declarations preserve=no
//## end module%349293A2028F.declarations

//## begin module%349293A2028F.additionalDeclarations preserve=yes
class ClCSAdministrator ;
//## end module%349293A2028F.additionalDeclarations


//## begin ClSupervisorSS%357FFFC203E6.preface preserve=yes
//## end ClSupervisorSS%357FFFC203E6.preface

//## Class: ClSupervisorSS%357FFFC203E6
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSupervisorSS : public ClSSImplementation  //## Inherits: <unnamed>%37A6BA41015E
{
  //## begin ClSupervisorSS%357FFFC203E6.initialDeclarations preserve=yes
  //## end ClSupervisorSS%357FFFC203E6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSupervisorSS%897935265
      ClSupervisorSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSupervisorSS%897935266
      ~ClSupervisorSS ();


    //## Other Operations (specified)
      //## Operation: ShutDown%897935268
      //	Hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: Synchronize%897935269
      //	Synchroniza los datos del subsistem, como por ejemplo la actualización de
      //	las estadísticas
      m4return_t Synchronize ();

      //## Operation: Initialize%897935270
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: GetSupervisor%897935272
      ClCSSupervisor * GetSupervisor ();

      //## Operation: SetSupervisor%897935273
      void SetSupervisor (ClCSSupervisor *ai_poSupervisor);

      //## Operation: ExecuteCommand%901709476
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709477
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709478
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: Start%927293869
      m4return_t Start ();

      //## Operation: Stop%928140273
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: IsReadyToClose%928140274
      m4bool_t IsReadyToClose ();

      //## Operation: GenerateSubsystem%933760135
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: Update%944159881
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%358004B801CF
      //## Role: ClSupervisorSS::m_poSupervisor%358004BA0178
      //## begin ClSupervisorSS::m_poSupervisor%358004BA0178.role preserve=no  public: ClCSSupervisor { -> RFHAN}
      ClCSSupervisor *m_poSupervisor;
      //## end ClSupervisorSS::m_poSupervisor%358004BA0178.role

    // Additional Public Declarations
      //## begin ClSupervisorSS%357FFFC203E6.public preserve=yes
      //## end ClSupervisorSS%357FFFC203E6.public

  protected:
    // Data Members for Associations

      //## Association: Supervisor::<unnamed>%37A8513903DB
      //## Role: ClSupervisorSS::<m_pClCommandsMap>%37A8513A03E6
      //	Es un mapa en el que el supervisor almacena los pares subsistema - comando
      //	que debe ejecutar (los comandos que tiene que ejecutar en que subsitemas)
      //## begin ClSupervisorSS::<m_pClCommandsMap>%37A8513A03E6.role preserve=no  protected: ClCommandsMap { -> RHAN}
      ClCommandsMap *m_pClCommandsMap;
      //## end ClSupervisorSS::<m_pClCommandsMap>%37A8513A03E6.role

      //## Association: Supervisor::<unnamed>%37A959220064
      //## Role: ClSupervisorSS::m_poCommandsConfig%37A95922038F
      //	Referencia a un objeto configuración en el que está la lista de comandos a
      //	ser ejecutados por el supervisor.
      //## begin ClSupervisorSS::m_poCommandsConfig%37A95922038F.role preserve=no  protected: ClConfiguration { -> RHAN}
      ClConfiguration *m_poCommandsConfig;
      //## end ClSupervisorSS::m_poCommandsConfig%37A95922038F.role

    // Additional Protected Declarations
      //## begin ClSupervisorSS%357FFFC203E6.protected preserve=yes
      //## end ClSupervisorSS%357FFFC203E6.protected

  private:
    // Additional Private Declarations
      //## begin ClSupervisorSS%357FFFC203E6.private preserve=yes
      //## end ClSupervisorSS%357FFFC203E6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSupervisorSS%357FFFC203E6.implementation preserve=yes
      //## end ClSupervisorSS%357FFFC203E6.implementation

};

//## begin ClSupervisorSS%357FFFC203E6.postscript preserve=yes
//## end ClSupervisorSS%357FFFC203E6.postscript

//## begin ClSupervisorsSS%3581533C035C.preface preserve=yes
//## end ClSupervisorsSS%3581533C035C.preface

//## Class: ClSupervisorsSS%3581533C035C
//	Agrupación lógica de supervisores en un subsistema genérico
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSupervisorsSS : public ClSSImplementation  //## Inherits: <unnamed>%37A6BA43023D
{
  //## begin ClSupervisorsSS%3581533C035C.initialDeclarations preserve=yes
  //## end ClSupervisorsSS%3581533C035C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSupervisorsSS%897983838
      ClSupervisorsSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSupervisorsSS%897983839
      ~ClSupervisorsSS ();


    //## Other Operations (specified)
      //## Operation: GetCommandsId%897983840
      //	Devuleve los Ids y los literales de los comandsos que puede ejecutar el
      //	subsistema
      m4return_t GetCommandsId (M4DataStorage *ao_poDataStorage);

      //## Operation: ShutDown%897983841
      //	Hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: Synchronize%897983842
      //	Synchroniza los datos del subsistem, como por ejemplo la actualización de
      //	las estadísticas
      m4return_t Synchronize ();

      //## Operation: Initialize%897983843
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: ExecuteCommand%901709479
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709480
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709481
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandParams%910862285
      m4return_t GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage);

      //## Operation: GenerateSubsystem%933665014
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    // Additional Public Declarations
      //## begin ClSupervisorsSS%3581533C035C.public preserve=yes
      //## end ClSupervisorsSS%3581533C035C.public

  protected:
    // Additional Protected Declarations
      //## begin ClSupervisorsSS%3581533C035C.protected preserve=yes
      //## end ClSupervisorsSS%3581533C035C.protected

  private:
    // Additional Private Declarations
      //## begin ClSupervisorsSS%3581533C035C.private preserve=yes
      //## end ClSupervisorsSS%3581533C035C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSupervisorsSS%3581533C035C.implementation preserve=yes
      //## end ClSupervisorsSS%3581533C035C.implementation

};

//## begin ClSupervisorsSS%3581533C035C.postscript preserve=yes
//## end ClSupervisorsSS%3581533C035C.postscript

// Class ClSupervisorSS 

// Class ClSupervisorsSS 

//## begin module%349293A2028F.epilog preserve=yes
//## end module%349293A2028F.epilog


#endif
