//## begin module%37D75C520204.cm preserve=no
//## end module%37D75C520204.cm

//## begin module%37D75C520204.cp preserve=no
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
//## end module%37D75C520204.cp

//## Module: clservicebasess%37D75C520204; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: F:\integration\m4subsystems\inc\clservicebasess.hpp

#ifndef clservicebasess_h
#define clservicebasess_h 1

//## begin module%37D75C520204.additionalIncludes preserve=no
//## end module%37D75C520204.additionalIncludes

//## begin module%37D75C520204.includes preserve=yes
//## end module%37D75C520204.includes

// clserviceinterface
#include <clserviceinterface.hpp>
// clssaction
#include <clssaction.hpp>
// queuebasedss
#include <queuebasedss.hpp>
//## begin module%37D75C520204.declarations preserve=no
//## end module%37D75C520204.declarations

//## begin module%37D75C520204.additionalDeclarations preserve=yes
//## end module%37D75C520204.additionalDeclarations


//## Class: ClServiceBaseSS%37D75C11000B
//	Esta clase es una clase particularizada de subsistema pero una
//	generalización de los subsistemas de servicio. Cualquiera que desee
//	implementar una clase extendida de subsistema de servicio podrá heredar de
//	esta clase y con solo sobrecargar los métodos que le interese, tendrá un
//	nuevo subsistema de servicio.
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SUBSYSTEMS ClServiceBaseSS : public ClQueueBasedSS  //## Inherits: <unnamed>%38EAF7A90047
{
  //## begin ClServiceBaseSS%37D75C11000B.initialDeclarations preserve=yes
  //## end ClServiceBaseSS%37D75C11000B.initialDeclarations

  public:
    //## Class: eServiceBaseCommands%37F1D6AE01A6
    //## Category: <Top Level>
    //## Subsystem: <Top Level>
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { ADD_NEW_EXECUTIVE = ClSSImplementation::TOTAL_COMMANDS , ENABLE_EXCEPTIONS , DISABLE_EXCEPTIONS , ESTIMATE_LOAD , TOTAL_COMMANDS } eServiceBaseCommands;

    //## begin ClServiceBaseSS::eServiceBaseCommands%37F1D6AE01A6.postscript preserve=yes
    //## end ClServiceBaseSS::eServiceBaseCommands%37F1D6AE01A6.postscript

    //## Class: eServiceBaseActions%391838B5019C
    //## Category: <Top Level>
    //## Subsystem: <Top Level>
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {EXECUTE_REQUEST_ACTION= 0 } eServiceBaseActions;

    //## begin ClServiceBaseSS::eServiceBaseActions%391838B5019C.postscript preserve=yes
    //## end ClServiceBaseSS::eServiceBaseActions%391838B5019C.postscript

  public:
    //## Constructors (specified)
      //## Operation: ClServiceBaseSS%936860987
      ClServiceBaseSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClServiceBaseSS%936860988
      ~ClServiceBaseSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%936860989
      //	A virtual method that only calls to the derived class from ClSSStats
      //	Container Initialize method
      virtual m4return_t Initialize ();

      //## Operation: GenerateSubsystem%936860990
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: SetService%936948254
      void SetService (ClServiceInterface *ai_poService);

      //## Operation: Synchronize%936948255
      //	Expands the synchronize method to all of subsystem sons and then calls to
      //	the ClSSStatsContainer derived class Synchronize method.
      virtual m4return_t Synchronize ();

      //## Operation: ExecuteCommand%936948256
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: Start%936948257
      virtual m4return_t Start ();

      //## Operation: Stop%936948258
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: IsReadyToClose%936948259
      virtual m4bool_t IsReadyToClose ();

      //## Operation: GetCommandParams%936948260
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%936948261
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: AddNewExecutive%938596860
      m4return_t AddNewExecutive ();

      //## Operation: ChekExecutorExceptions%938596861
      m4return_t ChekExecutorExceptions ();

      //## Operation: EmptyQueue%938596863
      //	Vacía la cola de servicio devolviendo un error por cada request encolada.
      //	Además bloquea la cola para nopermitir que ninguna request pueda ya ser
      //	introducida en la cola y todos los threads que escuchan de ella terminen la
      //	ejecución.
      virtual m4return_t EmptyQueue ();

      //## Operation: GetService%939282715
      ClServiceInterface * GetService ();

      //## Operation: CaptureExceptions%939806808
      //	Ejecuta el comando de habilitar, deshabilitar la captura de excepciones en
      //	todos los ejecutores de este servicio e invoca al método SetUseExceptions
      //	posteriormente
      m4return_t CaptureExceptions (m4bool_t ai_bCapture);

      //## Operation: SetUseExceptions%940264615
      //	Actualiza las variables indicativas de que la captura de excepciones esté
      //	activa o no, asñi como el valor de la propiedad en el objeto configuración
      void SetUseExceptions (m4bool_t ai_bException);

      //## Operation: GetUseExceptions%940264616
      //	Devuelve si los ejecutores del servicio tienen activada o no la captura de
      //	excepciones
      m4bool_t GetUseExceptions ();

      //## Operation: ProcessItem%955100221
      virtual m4return_t ProcessItem (ClBaseRequest *ai_oItem);

      //## Operation: CreateAction%955440315
      m4return_t CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction);

      //## Operation: ProcessAction%957943669
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: EstimateLoad%958983595
      m4return_t EstimateLoad ();

      //## Operation: Reply%967113585
      virtual m4return_t Reply (ClBaseRequest *ai_poRequest);

      //## Operation: Init%993464192
      //	Método que siendovirtual casi en ningún caso será necesario sobrecargar
      //	puesto que es llamado en arranque solamente y en él se busca en la
      //	configuración del subsistema los hijos que va a tener este, también en
      //	arranque. Por cada hijo que tenga configurado se llamará al método Generate
      //	Subsystem y se inicilizará el nuevo subsistema, invocando posteriormente a
      //	su vez a a su método Init
      virtual m4return_t Init (ClConfiguration *ai_poConfiguration);

      //## Operation: RegisterSnmpCommands%1066298189
      virtual m4return_t RegisterSnmpCommands ();

    // Additional Public Declarations
      //## begin ClServiceBaseSS%37D75C11000B.public preserve=yes
      //## end ClServiceBaseSS%37D75C11000B.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_uActiveInstances%37F1E5E90166
      //## begin ClServiceBaseSS::m_uActiveInstances%37F1E5E90166.attr preserve=no  protected: m4size_t {UA} 
      m4size_t m_uActiveInstances;
      //## end ClServiceBaseSS::m_uActiveInstances%37F1E5E90166.attr

      //## Attribute: m_uMaxInstances%37F1E5FE0289
      //## begin ClServiceBaseSS::m_uMaxInstances%37F1E5FE0289.attr preserve=no  protected: m4size_t {UA} 
      m4size_t m_uMaxInstances;
      //## end ClServiceBaseSS::m_uMaxInstances%37F1E5FE0289.attr

      //## Attribute: m_uMinActiveInstances%37F1E6110286
      //## begin ClServiceBaseSS::m_uMinActiveInstances%37F1E6110286.attr preserve=no  protected: m4size_t {UA} 
      m4size_t m_uMinActiveInstances;
      //## end ClServiceBaseSS::m_uMinActiveInstances%37F1E6110286.attr

      //## Attribute: m_bUseExceptions%380B4C140226
      //	Devuleve si los ejecutores de este servicio tienen activada la captura de
      //	excepciones.
      //## begin ClServiceBaseSS::m_bUseExceptions%380B4C140226.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bUseExceptions;
      //## end ClServiceBaseSS::m_bUseExceptions%380B4C140226.attr

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37D8B18A0211
      //## Role: ClServiceBaseSS::m_poService%37D8B18B0009
      //## begin ClServiceBaseSS::m_poService%37D8B18B0009.role preserve=no  protected: ClServiceInterface { -> RHAN}
      ClServiceInterface *m_poService;
      //## end ClServiceBaseSS::m_poService%37D8B18B0009.role

    // Additional Protected Declarations
      //## begin ClServiceBaseSS%37D75C11000B.protected preserve=yes
      //## end ClServiceBaseSS%37D75C11000B.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceBaseSS%37D75C11000B.private preserve=yes
      //## end ClServiceBaseSS%37D75C11000B.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceBaseSS%37D75C11000B.implementation preserve=yes
      //## end ClServiceBaseSS%37D75C11000B.implementation

};

//## begin ClServiceBaseSS%37D75C11000B.postscript preserve=yes
//## end ClServiceBaseSS%37D75C11000B.postscript

//## Class: ClServiceBaseSSAction%38F335A30196
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClServiceBaseSSAction : public ClSSAction  //## Inherits: <unnamed>%38F335B8001A
{
  //## begin ClServiceBaseSSAction%38F335A30196.initialDeclarations preserve=yes
  //## end ClServiceBaseSSAction%38F335A30196.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServiceBaseSSAction%955440317
      ClServiceBaseSSAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955440318
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClServiceBaseSSAction%38F335A30196.public preserve=yes
      //## end ClServiceBaseSSAction%38F335A30196.public

  protected:
    // Additional Protected Declarations
      //## begin ClServiceBaseSSAction%38F335A30196.protected preserve=yes
      //## end ClServiceBaseSSAction%38F335A30196.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceBaseSSAction%38F335A30196.private preserve=yes
      //## end ClServiceBaseSSAction%38F335A30196.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceBaseSSAction%38F335A30196.implementation preserve=yes
      //## end ClServiceBaseSSAction%38F335A30196.implementation

};

//## begin ClServiceBaseSSAction%38F335A30196.postscript preserve=yes
//## end ClServiceBaseSSAction%38F335A30196.postscript

// Class ClServiceBaseSS 

// Class ClServiceBaseSSAction 

//## begin module%37D75C520204.epilog preserve=yes
//## end module%37D75C520204.epilog


#endif
