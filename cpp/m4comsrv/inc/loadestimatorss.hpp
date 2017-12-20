//## begin module%37E661C70079.cm preserve=no
//## end module%37E661C70079.cm

//## begin module%37E661C70079.cp preserve=no
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
//## end module%37E661C70079.cp

//## Module: loadestimatorss%37E661C70079; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\loadestimatorss.hpp

#ifndef loadestimatorss_h
#define loadestimatorss_h 1

//## begin module%37E661C70079.additionalIncludes preserve=no
//## end module%37E661C70079.additionalIncludes

#include <clconfiguration.hpp>
#include <loadrule.hpp>
#include <clssimplementacion.hpp>


//## begin module%37E661C70079.declarations preserve=no
//## end module%37E661C70079.declarations

//## begin module%37E661C70079.additionalDeclarations preserve=yes
//## end module%37E661C70079.additionalDeclarations


//## begin ClLoadEstimatorSS%37E65D0D006F.preface preserve=yes
//## end ClLoadEstimatorSS%37E65D0D006F.preface

//## Class: ClLoadEstimatorSS%37E65D0D006F
//	Subsistema asociado a la gestión del AppCtl
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3960BAA80327;ClXMLMaker { -> F}

class ClLoadEstimatorSS : public ClSSImplementation  //## Inherits: <unnamed>%37E65D2802F8
{
  //## begin ClLoadEstimatorSS%37E65D0D006F.initialDeclarations preserve=yes
  //## end ClLoadEstimatorSS%37E65D0D006F.initialDeclarations

  public:
    //## begin ClLoadEstimatorSS::eAppDispatcherSSCommands%391BD5F20001.preface preserve=yes
    //## end ClLoadEstimatorSS::eAppDispatcherSSCommands%391BD5F20001.preface

    //## Class: eAppDispatcherSSCommands%391BD5F20001
    //## Category: Subsystem%348FEFFE016C
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { ESTIMATE_LOAD = ClSSImplementation::TOTAL_COMMANDS ,  TOTAL_COMMANDS} eAppDispatcherSSCommands;

    //## begin ClLoadEstimatorSS::eAppDispatcherSSCommands%391BD5F20001.postscript preserve=yes
    //## end ClLoadEstimatorSS::eAppDispatcherSSCommands%391BD5F20001.postscript

    //## Constructors (specified)
      //## Operation: ClLoadEstimatorSS%937841128
      ClLoadEstimatorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClLoadEstimatorSS%937841129
      ~ClLoadEstimatorSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%937841130
      //	Inicializa la gestión de la comunicación con el AppCtl, para ello se
      //	auntoconfigura con la configuración obtenida y activa los mecanismo de
      //	control.
      virtual m4return_t Initialize ();

      //## Operation: Start%937841131
      virtual m4return_t Start ();

      //## Operation: Stop%937841132
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: Update%938714043
      //	Gestión de eventos recibidos desde otros subsistemas
      virtual void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: ShutDown%940242447
      //	Shuts down this subsystem and all the subsystems matched to it
      virtual m4return_t ShutDown ();

      //## Operation: IsReadyToClose%940244005
      virtual m4bool_t IsReadyToClose ();

      //## Operation: NotifyDispatcher%958053247
      //	Cuando se produce un cambio en el estado del servidor, se le notifica
      //	llamando a esta función que despierta al thread que se encuentraescuchando
      //	en el select para que mande el mensaje oportuno
      m4return_t NotifyDispatcher (M4ClString ai_szMessage);

      //## Operation: CreateMessage%958053248
      //	Dependiendo del estadodel servidor se ha de mandar uno u otro mensaje al App
      //	Ctl
      m4return_t CreateMessage (m4pchar_t status, M4ClString &ao_Mesage, ClParamList *ai_poParamList);

      //## Operation: EstimateServerLoad%958053249
      m4return_t EstimateServerLoad (ClParamList &ao_oParamList);

      //## Operation: ExecuteCommand%958118079
      //	Ejecuta un comando de los disponibles del subsistema. Sus parámetros son: el
      //	identificador del comando (ID) la lista de parámetros de entrada y el
      //	contenedor para los parámetros de salida.
      //	Previamente a esta  llamada se deben de haber solicitado la lista de
      //	comandos y la lista de parámetros del comando en cuestión mediante los
      //	métodos GetCommandIds y GetCommandParams de la misma clase
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%958118080
      //	dado un comando concreto indicado por el primer parátro de entrada que
      //	especifica su ID de comando, se devuelve la lista  de parámetros de este
      //	comando.
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%958118081
      //	Se devuelve la lista de comandos de un subsistema concreto. por cada comando
      //	se tine un identificador numérico del comando, uno en formato cadena y un
      //	falg indicativo de si el comando lleva parámetros, bien de entrada o de
      //	salida.
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: EstimateLoad%958118082
      m4return_t EstimateLoad ();

      //## Operation: InitializeEstimationRules%960220701
      m4return_t InitializeEstimationRules ();

      //## Operation: GenerateSubsystem%960220702
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

      //## Operation: CalculateLoad%962640310
      m4return_t CalculateLoad (ClParamList &ai_pParams);

      //## Operation: Connect%969382660
      m4return_t Connect ();

      //## Operation: Disconnect%995012991
      m4return_t Disconnect ();

  public:
    // Additional Public Declarations
      //## begin ClLoadEstimatorSS%37E65D0D006F.public preserve=yes
      //## end ClLoadEstimatorSS%37E65D0D006F.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%393BD46803B5
      //## Role: ClLoadEstimatorSS::m_oLoadRulesTable%393BD469032A
      //## begin ClLoadEstimatorSS::m_oLoadRulesTable%393BD469032A.role preserve=no  protected: ClLoadRulesTable { -> VHAN}
      ClLoadRulesTable m_oLoadRulesTable;
      //## end ClLoadEstimatorSS::m_oLoadRulesTable%393BD469032A.role

    // Additional Protected Declarations
      //## begin ClLoadEstimatorSS%37E65D0D006F.protected preserve=yes
      //## end ClLoadEstimatorSS%37E65D0D006F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_lDispatcherPort%391AE947021D
      //## begin ClLoadEstimatorSS::m_lDispatcherPort%391AE947021D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_lDispatcherPort;
      //## end ClLoadEstimatorSS::m_lDispatcherPort%391AE947021D.attr

      //## Attribute: m_iSleepTime%391AE94702A9
      //## begin ClLoadEstimatorSS::m_iSleepTime%391AE94702A9.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iSleepTime;
      //## end ClLoadEstimatorSS::m_iSleepTime%391AE94702A9.attr

      //## Attribute: m_szProcessName%391AE9470321
      //## begin ClLoadEstimatorSS::m_szProcessName%391AE9470321.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_szProcessName;
      //## end ClLoadEstimatorSS::m_szProcessName%391AE9470321.attr

      //## Attribute: m_szConfigName%391AE9480033
      //## begin ClLoadEstimatorSS::m_szConfigName%391AE9480033.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_szConfigName;
      //## end ClLoadEstimatorSS::m_szConfigName%391AE9480033.attr

      //## Attribute: m_szState%391AE94800E8
      //## begin ClLoadEstimatorSS::m_szState%391AE94800E8.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_szState;
      //## end ClLoadEstimatorSS::m_szState%391AE94800E8.attr

      //## Attribute: m_szDispatcherHost%3961C96602D0
      //## begin ClLoadEstimatorSS::m_szDispatcherHost%3961C96602D0.attr preserve=no  private: M4ClString {UA} 
      M4ClString m_szDispatcherHost;
      //## end ClLoadEstimatorSS::m_szDispatcherHost%3961C96602D0.attr

      //## Attribute: m_szLocalHost%3977094402A1
      //## begin ClLoadEstimatorSS::m_szLocalHost%3977094402A1.attr preserve=no  private: m4string_t {UA} 
      m4string_t m_szLocalHost;
      //## end ClLoadEstimatorSS::m_szLocalHost%3977094402A1.attr

      //## Attribute: m_iBasePort%39770DE800E7
      //## begin ClLoadEstimatorSS::m_iBasePort%39770DE800E7.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iBasePort;
      //## end ClLoadEstimatorSS::m_iBasePort%39770DE800E7.attr

      //## Attribute: m_iDspConnectionId%39C79AF201F8
      //## begin ClLoadEstimatorSS::m_iDspConnectionId%39C79AF201F8.attr preserve=no  private: m4int_t {UA} 
      m4int_t m_iDspConnectionId;
      //## end ClLoadEstimatorSS::m_iDspConnectionId%39C79AF201F8.attr

      //## Attribute: m_bConnected%39C79BC000A0
      //## begin ClLoadEstimatorSS::m_bConnected%39C79BC000A0.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bConnected;
      //## end ClLoadEstimatorSS::m_bConnected%39C79BC000A0.attr

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%393BD6A10205
      //## Role: ClLoadEstimatorSS::m_poLoadRulesConfig%393BD6A20120
      //## begin ClLoadEstimatorSS::m_poLoadRulesConfig%393BD6A20120.role preserve=no  private: ClConfiguration { -> RHAN}
      ClConfiguration *m_poLoadRulesConfig;
      //## end ClLoadEstimatorSS::m_poLoadRulesConfig%393BD6A20120.role

    // Additional Private Declarations
      //## begin ClLoadEstimatorSS%37E65D0D006F.private preserve=yes
      //## end ClLoadEstimatorSS%37E65D0D006F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLoadEstimatorSS%37E65D0D006F.implementation preserve=yes
      //## end ClLoadEstimatorSS%37E65D0D006F.implementation

};

//## begin ClLoadEstimatorSS%37E65D0D006F.postscript preserve=yes
//## end ClLoadEstimatorSS%37E65D0D006F.postscript

// Class ClLoadEstimatorSS 

//## begin module%37E661C70079.epilog preserve=yes
//## end module%37E661C70079.epilog


#endif
