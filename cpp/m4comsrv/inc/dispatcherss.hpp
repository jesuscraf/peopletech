//## begin module%38F5A2770252.cm preserve=no
//## end module%38F5A2770252.cm

//## begin module%38F5A2770252.cp preserve=no
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
//## end module%38F5A2770252.cp

//## Module: dispatcherss%38F5A2770252; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: D:\Work\v600\m4comsrv\inc\dispatcherss.hpp

#ifndef dispatcherss_h
#define dispatcherss_h 1

//## begin module%38F5A2770252.additionalIncludes preserve=no
//## end module%38F5A2770252.additionalIncludes

// clssaction
#include <clssaction.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>

class ClRequest;
class M4Credentials;

//## begin module%38F5A2770252.declarations preserve=no
//## end module%38F5A2770252.declarations

//## begin module%38F5A2770252.additionalDeclarations preserve=yes
//## end module%38F5A2770252.additionalDeclarations


//## begin ClDispatcherSS%38F5A17003DA.preface preserve=yes
//## end ClDispatcherSS%38F5A17003DA.preface

//## Class: ClDispatcherSS%38F5A17003DA
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F5A9520007;ClRequest { -> F}
//## Uses: <unnamed>%39AB69B602CF;M4Credentials { -> F}

class ClDispatcherSS : public ClSSImplementation  //## Inherits: <unnamed>%38F5A196038F
{
  //## begin ClDispatcherSS%38F5A17003DA.initialDeclarations preserve=yes
  //## end ClDispatcherSS%38F5A17003DA.initialDeclarations

  public:
    //## begin ClDispatcherSS::eDispatcherActions%39192D3001C9.preface preserve=yes
    //## end ClDispatcherSS::eDispatcherActions%39192D3001C9.preface

    //## Class: eDispatcherActions%39192D3001C9
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {DISPATCH_REQUEST_ACTION_ID = 0} eDispatcherActions;

    //## begin ClDispatcherSS::eDispatcherActions%39192D3001C9.postscript preserve=yes
    //## end ClDispatcherSS::eDispatcherActions%39192D3001C9.postscript

    //## begin ClDispatcherSS::eDispatcherCommands%3D7EFF6D01A6.preface preserve=yes
    //## end ClDispatcherSS::eDispatcherCommands%3D7EFF6D01A6.preface

    //## Class: eDispatcherCommands%3D7EFF6D01A6
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version%341E4BC70130
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {DISPATCHER_SS_DEBUG_ENABLED =  ClSSImplementation::TOTAL_COMMANDS, TOTAL_COMMANDS} eDispatcherCommands;

    //## begin ClDispatcherSS::eDispatcherCommands%3D7EFF6D01A6.postscript preserve=yes
    //## end ClDispatcherSS::eDispatcherCommands%3D7EFF6D01A6.postscript

    //## Constructors (specified)
      //## Operation: ClDispatcherSS%955610152
      ClDispatcherSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);


    //## Other Operations (specified)
      //## Operation: CreateAction%955610153
      m4return_t CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction);

      //## Operation: FindService%955610154
      ClSSImplementation * FindService (ClRequest *ai_poRequest);

      //## Operation: ProcessItem%955610156
      m4return_t ProcessItem (ClBaseRequest *ai_oItem);

      //## Operation: ExecuteCommand%955610157
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%955610158
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%955610159
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: Initialize%955610160
      m4return_t Initialize ();

      //## Operation: ShutDown%955610161
      m4return_t ShutDown ();

      //## Operation: Start%955610162
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      m4return_t Start ();

      //## Operation: GenerateSubsystem%955610163
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: GetService%955610164
      m4return_t GetService (m4pchar_t ai_szLogicalService, m4pchar_t ao_szPhisicalService, m4size_t ai_iLenBuffer);

      //## Operation: ProcessAction%957943671
      m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: RecoverSessionFromRequest%967533894
      m4return_t RecoverSessionFromRequest (ClRequest *ai_poRequest, ClUSSessionInterface *&ao_poSession);

  public:
    // Additional Public Declarations
      //## begin ClDispatcherSS%38F5A17003DA.public preserve=yes
      //## end ClDispatcherSS%38F5A17003DA.public

  protected:
    // Additional Protected Declarations
      //## begin ClDispatcherSS%38F5A17003DA.protected preserve=yes
      //## end ClDispatcherSS%38F5A17003DA.protected

  private:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%38F5A7550037
      //## Role: ClDispatcherSS::m_poServicesTable%38F5A75503BC
      //## begin ClDispatcherSS::m_poServicesTable%38F5A75503BC.role preserve=no  private: ClConfiguration { -> RHAN}
      ClConfiguration *m_poServicesTable;
      //## end ClDispatcherSS::m_poServicesTable%38F5A75503BC.role

    // Additional Private Declarations
      //## begin ClDispatcherSS%38F5A17003DA.private preserve=yes
      //## end ClDispatcherSS%38F5A17003DA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDispatcherSS%38F5A17003DA.implementation preserve=yes
      //## end ClDispatcherSS%38F5A17003DA.implementation

};

//## begin ClDispatcherSS%38F5A17003DA.postscript preserve=yes
//## end ClDispatcherSS%38F5A17003DA.postscript

//## begin ClDispatchRequestAction%38F3505201C1.preface preserve=yes
//## end ClDispatchRequestAction%38F3505201C1.preface

//## Class: ClDispatchRequestAction%38F3505201C1
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDispatchRequestAction : public ClSSAction  //## Inherits: <unnamed>%38F350870041
{
  //## begin ClDispatchRequestAction%38F3505201C1.initialDeclarations preserve=yes
  //## end ClDispatchRequestAction%38F3505201C1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDispatchRequestAction%955440328
      ClDispatchRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955440329
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClDispatchRequestAction%38F3505201C1.public preserve=yes
      //## end ClDispatchRequestAction%38F3505201C1.public

  protected:
    // Additional Protected Declarations
      //## begin ClDispatchRequestAction%38F3505201C1.protected preserve=yes
      //## end ClDispatchRequestAction%38F3505201C1.protected

  private:
    // Additional Private Declarations
      //## begin ClDispatchRequestAction%38F3505201C1.private preserve=yes
      //## end ClDispatchRequestAction%38F3505201C1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDispatchRequestAction%38F3505201C1.implementation preserve=yes
      //## end ClDispatchRequestAction%38F3505201C1.implementation

};

//## begin ClDispatchRequestAction%38F3505201C1.postscript preserve=yes
//## end ClDispatchRequestAction%38F3505201C1.postscript

// Class ClDispatcherSS 

// Class ClDispatchRequestAction 

//## begin module%38F5A2770252.epilog preserve=yes
//## end module%38F5A2770252.epilog


#endif
