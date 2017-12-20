//## begin module%38D787C5012A.cm preserve=no
//## end module%38D787C5012A.cm

//## begin module%38D787C5012A.cp preserve=no
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
//## end module%38D787C5012A.cp

//## Module: ssccsubsystems%38D787C5012A; Package specification
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Source file: F:\integration\m4comsrv\inc\ssccsubsystems.hpp

#ifndef ssccsubsystems_h
#define ssccsubsystems_h 1

//## begin module%38D787C5012A.additionalIncludes preserve=no
//## end module%38D787C5012A.additionalIncludes

//## begin module%38D787C5012A.includes preserve=yes
#include <cladministrator.hpp>
//## end module%38D787C5012A.includes

// FIFOQ
#include <fifoq.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// clssaction
#include <clssaction.hpp>

class ClCSSessManager;
class ClSessManagerVisitor;

//## begin module%38D787C5012A.declarations preserve=no
//## end module%38D787C5012A.declarations

//## begin module%38D787C5012A.additionalDeclarations preserve=yes
//## end module%38D787C5012A.additionalDeclarations


//## Class: ClAdminsListSS%348D67D902BF
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminsListSS : public ClSSImplementation  //## Inherits: <unnamed>%38D792C80051
{
  public:
    //## begin ClAdminsListSS::eAdminListCommands%34F5B3E7012A.preface preserve=yes
    //## end ClAdminsListSS::eAdminListCommands%34F5B3E7012A.preface

    //## Class: eAdminListCommands%34F5B3E7012A
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems::inc%38D787F6035B
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { MOVE_OUT_ADMINISTRATOR = ClSSImplementation:: TOTAL_COMMANDS , TOTAL_COMMANDS } eAdminListCommands;

    //## begin ClAdminsListSS::eAdminListCommands%34F5B3E7012A.postscript preserve=yes
    //## end ClAdminsListSS::eAdminListCommands%34F5B3E7012A.postscript

    //## Constructors (specified)
      //## Operation: ClAdminsListSS%882177041
      //	Constructor
      ClAdminsListSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClAdminsListSS%882177042
      //	DeStructor
      ~ClAdminsListSS ();


    //## Other Operations (specified)
      //## Operation: SetControlQueue%882177043
      //	Asigna la cola de control
      void SetControlQueue (ClControlQ *ai_poControlQueue);

      //## Operation: GetControlQueue%882177044
      //	devuelve el apuntador a la cola de control
      ClControlQ * GetControlQueue ();

      //## Operation: Synchronize%884942688
      //	Synchroniza el subsystema, calculando estadísticas y lo que fuera necesario
      m4return_t Synchronize ();

      //## Operation: Initialize%885806466
      //	inicializa el subsistema
      m4return_t Initialize ();

      //## Operation: GetCommandsId%885806486
      //	Devuelve llos IDs y los literales de los comandos que se pueden ejecutar en
      //	el subsistema
      m4return_t GetCommandsId (M4DataStorage *ai_poOutStorage);

      //## Operation: ShutDown%885806510
      //	hace el shutdown del subsistema
      m4return_t ShutDown ();

      //## Operation: MoveOutAdministrator%888569455
      //	Echa a un administtrador del servidor de modo que este no puede seguir
      //	administrando.
      m4return_t MoveOutAdministrator (m4uint32_t ai_lIdAdministrator);

      //## Operation: GetCommandParams%890917794
      //	DEvuelve los parámetros necesarios para ejecutar un comado concreto
      m4return_t GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%901709419
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709420
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709421
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%348FF5DE00CE
      //## Role: ClAdminsListSS::m_poControlQueue%348FF5DF009D
      //## begin ClAdminsListSS::m_poControlQueue%348FF5DF009D.role preserve=no  public: ClControlQ { -> RHAN}
      ClControlQ *m_poControlQueue;
      //## end ClAdminsListSS::m_poControlQueue%348FF5DF009D.role

  public:
  protected:
  private:
  private: //## implementation
};

//## Class: ClAdminSS%348FC82400FE
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClAdminSS : public ClSSImplementation  //## Inherits: <unnamed>%38D792CA013A
{
  public:
    //## begin ClAdminSS::eAdminCommands%34F5B721015A.preface preserve=yes
    //## end ClAdminSS::eAdminCommands%34F5B721015A.preface

    //## Class: eAdminCommands%34F5B721015A
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems::inc%38D787F6035B
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { TOTAL_COMMANDS = ClSSImplementation::TOTAL_COMMANDS } eAdminCommands;

    //## begin ClAdminSS::eAdminCommands%34F5B721015A.postscript preserve=yes
		ClCSAdministrator * m_poAdministrator ;
    //## end ClAdminSS::eAdminCommands%34F5B721015A.postscript

    //## Constructors (specified)
      //## Operation: ClAdminSS%882177071
      ClAdminSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClAdminSS%882177072
      ~ClAdminSS ();


    //## Other Operations (specified)
      //## Operation: GetAdministrator%882177073
      ClCSAdministrator * GetAdministrator ();

      //## Operation: SetAdministrator%882177074
      void SetAdministrator (ClCSAdministrator *ai_poAdministrator);

      //## Operation: Accept%884777556
      void Accept (ClVisitor &ai_oVisitor);

      //## Operation: Synchronize%884942694
      m4return_t Synchronize ();

      //## Operation: Initialize%885806471
      m4return_t Initialize ();

      //## Operation: GetCommandsId%885806491
      m4return_t GetCommandsId (M4DataStorage *ai_poOutStorage);

      //## Operation: ShutDown%885806516
      m4return_t ShutDown ();

      //## Operation: GetCommandParams%890917800
      m4return_t GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%901709434
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709435
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709436
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: Stop%928140263
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: IsReadyToClose%928140264
      m4bool_t IsReadyToClose ();

    // Data Members for Associations


  public:
  protected:
  private:
  private: //## implementation
};

//## Class: ClSessManagerSS%348FC75A02A2
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%358632FA0257;ClSessManagerVisitor { -> F}

class ClSessManagerSS : public ClSSImplementation  //## Inherits: <unnamed>%38D793BD005D
{
  public:
    //## begin ClSessManagerSS::eConnManagerCommnads%34F5BB3303AA.preface preserve=yes
    //## end ClSessManagerSS::eConnManagerCommnads%34F5BB3303AA.preface

    //## Class: eConnManagerCommnads%34F5BB3303AA
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems::inc%38D787F6035B
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { TOTAL_COMMANDS = ClSSImplementation::TOTAL_COMMANDS } eConnManagerCommnads;

    //## begin ClSessManagerSS::eConnManagerCommnads%34F5BB3303AA.postscript preserve=yes
    //## end ClSessManagerSS::eConnManagerCommnads%34F5BB3303AA.postscript

    //## begin ClSessManagerSS::eSessManagerActions%39193A75020A.preface preserve=yes
    //## end ClSessManagerSS::eSessManagerActions%39193A75020A.preface

    //## Class: eSessManagerActions%39193A75020A
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems%38D7879E0278
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { CONTROL_REQUESST_ACTION_ID = 0} eSessManagerActions;

    //## begin ClSessManagerSS::eSessManagerActions%39193A75020A.postscript preserve=yes
    //## end ClSessManagerSS::eSessManagerActions%39193A75020A.postscript

    //## Constructors (specified)
      //## Operation: ClSessManagerSS%882177049
      //	Constructor
      ClSessManagerSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClSessManagerSS%882177050
      //	Destructor
      ~ClSessManagerSS ();


    //## Other Operations (specified)
      //## Operation: GetSessManager%882177051
      //	Devuelve el apuntador al objeto ConnectionManager
      ClCSSessManager * GetSessManager ();

      //## Operation: SetSessManager%882177052
      //	Asigna el valor al miembro que es sun apuntador al objeto ConnectionManager
      void SetSessManager (ClCSSessManager *ai_poSessManager);

      //## Operation: Accept%884777552
      //	Acepta la visita de un visitor
      void Accept (ClSessManagerVisitor &ai_oVisitor);

      //## Operation: Synchronize%884942690
      //	Synchroniza el subsistema, calcula estadísticas y lo que sea pertinente
      m4return_t Synchronize ();

      //## Operation: Initialize%885806468
      //	Inicializa el subsistema
      m4return_t Initialize ();

      //## Operation: GetCommandsId%885806488
      //	devuleve el Id y literal de los comandos que se pueden ejecutar sobre el
      //	subsistema
      m4return_t GetCommandsId (M4DataStorage *ai_poOutStorage);

      //## Operation: ShutDown%885806512
      //	hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: GetCommandParams%890917796
      //	Devuelve los parámetros que recive la función exewcute command para el caso
      //	de un comando en concreto.
      m4return_t GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%901709425
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709426
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709427
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: Start%927293861
      m4return_t Start ();

      //## Operation: Stop%928140257
      m4return_t Stop (m4uint32_t ai_lDeferedTime);

      //## Operation: IsReadyToClose%928140258
      m4bool_t IsReadyToClose ();

      //## Operation: Update%941813352
      virtual void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: ProcessItem%954935289
      m4return_t ProcessItem (ClBaseRequest *ai_oItem, m4bool_t &ai_bReply);

      //## Operation: CreateAction%955440324
      m4return_t CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction);

      //## Operation: ProcessAction%957943672
      m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

    // Data Members for Associations

      //## Association: LocalMonitor::<unnamed>%348FCE760281
      //## Role: ClSessManagerSS::m_poSessManager%348FCE770156
      //## begin ClSessManagerSS::m_poSessManager%348FCE770156.role preserve=no  public: ClCSSessManager { -> RFHAN}
      ClCSSessManager *m_poSessManager;
      //## end ClSessManagerSS::m_poSessManager%348FCE770156.role

  public:
  protected:
  private:
  private: //## implementation
};

//## begin ClControlRequestAction%38F34B9902C7.preface preserve=yes
//## end ClControlRequestAction%38F34B9902C7.preface

//## Class: ClControlRequestAction%38F34B9902C7
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClControlRequestAction : public ClSSAction  //## Inherits: <unnamed>%38F34BAF0214
{
  //## begin ClControlRequestAction%38F34B9902C7.initialDeclarations preserve=yes
  //## end ClControlRequestAction%38F34B9902C7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClControlRequestAction%955440322
      ClControlRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955440323
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClControlRequestAction%38F34B9902C7.public preserve=yes
      //## end ClControlRequestAction%38F34B9902C7.public

  protected:
    // Additional Protected Declarations
      //## begin ClControlRequestAction%38F34B9902C7.protected preserve=yes
      //## end ClControlRequestAction%38F34B9902C7.protected

  private:
    // Additional Private Declarations
      //## begin ClControlRequestAction%38F34B9902C7.private preserve=yes
      //## end ClControlRequestAction%38F34B9902C7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClControlRequestAction%38F34B9902C7.implementation preserve=yes
      //## end ClControlRequestAction%38F34B9902C7.implementation

};

//## begin ClControlRequestAction%38F34B9902C7.postscript preserve=yes
//## end ClControlRequestAction%38F34B9902C7.postscript

// Class ClAdminsListSS 

// Class ClAdminSS 

// Class ClSessManagerSS 

// Class ClControlRequestAction 

//## begin module%38D787C5012A.epilog preserve=yes
//## end module%38D787C5012A.epilog


#endif
