//## begin module%37A555F8036F.cm preserve=no
//## end module%37A555F8036F.cm

//## begin module%37A555F8036F.cp preserve=no
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
//## end module%37A555F8036F.cp

//## Module: clssimplementacion%37A555F8036F; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: z:\m4subsystems\inc\clssimplementacion.hpp

#ifndef clssimplementacion_h
#define clssimplementacion_h 1

//## begin module%37A555F8036F.additionalIncludes preserve=no
//## end module%37A555F8036F.additionalIncludes

//## begin module%37A555F8036F.includes preserve=yes
#include <m4subsystems_dll.hpp>
#include "m4stl.hpp"
//## end module%37A555F8036F.includes

// m4types
#include <m4types.hpp>
// syncronization
#include <m4syncronization.hpp>
// Statistics
#include <statistics.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// NavigatorF
#include <navigatorf.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// serverevent
#include <serverevent.hpp>
// clssfactoryinterfaz
#include <clssfactoryinterfaz.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
// m4ssapi4snmp
#include <m4ssapi4snmp.hpp>

class ClConfiguration;
class ClSSAction;

//## begin module%37A555F8036F.declarations preserve=no
//## end module%37A555F8036F.declarations

//## begin module%37A555F8036F.additionalDeclarations preserve=yes
#define PROPERTIES_ATTRIBUTES_PATH	"ATTRIBUTES.DESCRIPTORS"
#define ATTRIBUTE					"ATTRIBUTE"
#define ATTRIBUTES					"ATTRIBUTES"
#define PROPERTIES_STATISTICS_PATH	"STATISTICS.DESCRIPTORS"
#define STATISTIC					"STATISTIC"

class M4ISnmpAdministrator;
//## end module%37A555F8036F.additionalDeclarations


//## begin INTERFACESMAP%37A54C0400F5.preface preserve=yes
//## end INTERFACESMAP%37A54C0400F5.preface

//## Class: INTERFACESMAP%37A54C0400F5
//	contenedor de los subsistemas hijos de cada subsistema. És un mapa de
//	interfaces de los subsistemas hijos.
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A5763603C8;map { -> }

typedef map < m4objid_t , ClSSInterfaz *, less <m4objid_t> > INTERFACESMAP;

//## begin INTERFACESMAP%37A54C0400F5.postscript preserve=yes
//## end INTERFACESMAP%37A54C0400F5.postscript

//## begin ClSSImplementation%37A54BCB0070.preface preserve=yes
//## end ClSSImplementation%37A54BCB0070.preface

//## Class: ClSSImplementation%37A54BCB0070
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A5838B0055;ClAdminTagList { -> }
//## Uses: <unnamed>%37A5896203D0;ClIdGenerator { -> F}
//## Uses: <unnamed>%38F20ECA007E;ClSSAction { -> F}
//## Uses: <unnamed>%40A9C2E102B0;M4SSApi4Snmp { -> }

class M4_DECL_M4SUBSYSTEMS ClSSImplementation : public ClServerEvent, //## Inherits: <unnamed>%37E0C3CC027F
                        	public ClBaseActionExecutive  //## Inherits: <unnamed>%38F44C630136
{
  //## begin ClSSImplementation%37A54BCB0070.initialDeclarations preserve=yes
  //## end ClSSImplementation%37A54BCB0070.initialDeclarations

  public:
    //## begin ClSSImplementation::eImplementationCommands%37A6A2F301AF.preface preserve=yes
    //## end ClSSImplementation::eImplementationCommands%37A6A2F301AF.preface

    //## Class: eImplementationCommands%37A6A2F301AF
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {SHUT_DOWN = 0 ,RESET_STATISTICS ,TOTAL_COMMANDS} eImplementationCommands;

    //## begin ClSSImplementation::eImplementationCommands%37A6A2F301AF.postscript preserve=yes
    //## end ClSSImplementation::eImplementationCommands%37A6A2F301AF.postscript

    //## begin ClSSImplementation::eImplementationCalls%37A6A3C90044.preface preserve=yes
    //## end ClSSImplementation::eImplementationCalls%37A6A3C90044.preface

    //## Class: eImplementationCalls%37A6A3C90044
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {CALL_SYNCHRONIZE = 0, CALL_INITIALIZE , CALL_EXECUTE_COMMAND , CALL_START , STOP , CALL_ACTIVATE , TOTAL_CALLS } eImplementationCalls;

    //## begin ClSSImplementation::eImplementationCalls%37A6A3C90044.postscript preserve=yes
    //## end ClSSImplementation::eImplementationCalls%37A6A3C90044.postscript

    //## begin ClSSImplementation::eStatus%37E651D603D8.preface preserve=yes
    //## end ClSSImplementation::eStatus%37E651D603D8.preface

    //## Class: eStatus%37E651D603D8
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {SS_INACTIVE = 0 , SS_ACTIVE , SS_PAUSED , SS_SHUTING_DOWN , SS_NOT_ACCESIBLE , MAX_STATUS} eStatus;

    //## begin ClSSImplementation::eStatus%37E651D603D8.postscript preserve=yes
    //## end ClSSImplementation::eStatus%37E651D603D8.postscript

    //## begin ClSSImplementation::eImplementationAttribs%38318AC902B6.preface preserve=yes
    //## end ClSSImplementation::eImplementationAttribs%38318AC902B6.preface

    //## Class: eImplementationAttribs%38318AC902B6
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { ID_TYPE = 0 , ID_STRINGID , ID_START_DATE , ID_STATE , TOTAL_ATTRIBS} eImplementationAttribs;

    //## begin ClSSImplementation::eImplementationAttribs%38318AC902B6.postscript preserve=yes
    //## end ClSSImplementation::eImplementationAttribs%38318AC902B6.postscript

    //## Constructors (specified)
      //## Operation: ClSSImplementation%933665011
      //	Constructor válido por defecto, que toma la configuración del subsistema, la
      //	referencia las tablas (parámetros de configuración auxiliares) y la
      //	referencia al navegador que comparten los subsistemas.
      ClSSImplementation (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, m4bool_t ai_bHasOwner = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~ClSSImplementation%960286623
      virtual ~ClSSImplementation ();


    //## Other Operations (specified)
      //## Operation: AddSubsystem%933578383
      //	Add a subsystem to this node. The pointer to the subsystem to be added is
      //	passes as an argument.
      virtual m4return_t AddSubsystem (ClSSInterfaz *ai_poSubsystem);

      //## Operation: RemoveSubsystem%933578384
      //	Removes a determinated subsystem from this subsystem
      virtual m4return_t RemoveSubsystem (m4objid_t ai_lSubsystemId	// The id of the subsystem to be removed
      );

      //## Operation: ShutDown%933578385
      //	Shuts down this subsystem and all the subsystems matched to it
      virtual m4return_t ShutDown ();

      //## Operation: Synchronize%933578387
      //	Expands the synchronize method to all of subsystem sons and then calls to
      //	the derived class Synchronize method.
      virtual m4return_t Synchronize ();

      //## Operation: Initialize%933578388
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

      //## Operation: GetStatisticAttribValue%933578389
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetStatisticAttribValue (m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: FindSubsystemByType%933578391
      //	Devuelve la primera ocurrencia de un subsistema del tipo que se especifica
      //	en el parámetro a partir de la posición del árbol en la que se encuentra el
      //	sbsitema al que se invoca.
      virtual ClSSInterfaz * FindSubsystemByType (m4uint32_t ai_iType);

      //## Operation: FindSubsystemById%933578392
      //	This method has the same functionality of the other FindSubsystem method but
      //	the diference is that in this one the argument is a single id so the
      //	functions compares the arguments to its own id and if it isn't propagates
      //	the calling to each of its sons or return error if there are no sons.
      virtual ClSSInterfaz * FindSubsystemById (m4objid_t ai_lId	// The id of the subsystem to look for.
      );

      //## Operation: FindSubsystemByName%933578393
      //	Busca el primer subsitema en el árbol de subsistemas cuyo nombre coincida
      //	con el argumento que se pasa, comenzando por el subsitema que ha sido
      //	invocado y progandolo en profundidad.
      virtual ClSSInterfaz * FindSubsystemByName (m4pchar_t ai_szName	// The id of the subsystem to look for.
      );

      //## Operation: Start%933578395
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      virtual m4return_t Start ();

      //## Operation: Stop%933578396
      //	Cuando se inicaia el proceso de ShutDown se notifica a todos los subsistemas
      //	mediante este método. Como parámetro se pasa en segundos el timepo que queda
      //	hasta el shutdown definitivo y cada subsistema debe de ir haciendo en los
      //	pasos previos según corresponda las aaciones pertinentes para dejar todo
      //	consistente antes de ese momento o al menos en la última llamada (tiempo = 0)
      virtual m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: IsReadyToClose%933578397
      //	Durante el proceso de ShutDown, este métodose llamará para todos los
      //	subsistemas con objeto de verificar el momento en el que sea posible pasar a
      //	la destrucción de todos los subsistemas.
      //	Aquel subsistema que precise de una deshabitación previa a la destucción,
      //	como puede ser la finalización de los threads asociados, hará esto en un
      //	paso previo (Stop) y a la llamada de este método, devolverá true o false
      //	dependiendo de si el thread a finalizado ya o no, es decr dependiendo de si
      //	el subsistema está listo o no para ser destruido.
      virtual m4bool_t IsReadyToClose ();

      //## Operation: GetName%933578401
      //	Returns the subsystem name string
      virtual m4pchar_t GetName ();

      //## Operation: GetType%933578405
      //	Returns one integer that is the type of the subsystem. There are predefined
      //	constants for each type.
      virtual m4uint32_t GetType ();

      //## Operation: SetName%933578406
      //	Sets the name string atribute
      virtual void SetName (m4pchar_t ai_szName);

      //## Operation: SetType%933578407
      //	Sets the subsystem type by an integer parameter.
      virtual void SetType (m4uint32_t ai_iType);

      //## Operation: SetType%933578408
      //	Sets
      virtual void SetType (m4pchar_t ai_szType);

      //## Operation: SetConfiguration%933578425
      //	Asigna el apuntador al objeto configuración del subsistema
      virtual void SetConfiguration (ClConfiguration *ai_poConfiguration);

      //## Operation: GetElement%933578426
      //	Función virtual (al igual que todas las de esta clase) que sobrecargada en
      //	otra clase que herede de esta, devolverá culaquier elemento contenido por el
      //	subsistema, en forma de un puntero a void.
      //	El elemento que ha de devolver, se indica con una cadena pasada como
      //	parámetro de entrada.
      //	Elementos pueden ser todas aquellas referencias que el subsistema contenga,
      //	que por agrupación formen el conjunto lógico denominado subsistema.
      //	Por ejemplo, el subsistema sesión tendrá un elemento que es la sesión, el
      //	subsistema servicio tendrá otro que sea la cola de servicio y otro que sea
      //	en propio servicio, etc...
      virtual void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: GetId%933578427
      //	Returns the subsystem id
      virtual m4objid_t GetId ();

      //## Operation: SetId%933578428
      //	Sets the subsystem id
      virtual void SetId (m4objid_t ai_lId, m4pchar_t ai_szFatherPath);

      //## Operation: Activate%933578429
      //	Reinicializa el subistema en caso de haber sido desactivado antes de ser
      //	almacenado en la cache de subistemas.
      virtual void Activate ();

      //## Operation: Init%933665012
      //	Método que siendovirtual casi en ningún caso será necesario sobrecargar
      //	puesto que es llamado en arranque solamente y en él se busca en la
      //	configuración del subsistema los hijos que va a tener este, también en
      //	arranque. Por cada hijo que tenga configurado se llamará al método Generate
      //	Subsystem y se inicilizará el nuevo subsistema, invocando posteriormente a
      //	su vez a a su método Init
      virtual m4return_t Init (ClConfiguration *ai_poConfiguration);

      //## Operation: GenerateSubsystem%933665013
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: SetInterfaz%933665017
      //	Asigna un interfaz a la implementación del subsistema
      void SetInterfaz (ClSSInterfaz *ai_poInterfaz);

      //## Operation: FindSubsystem%936613840
      //	This method has the same functionality of the other FindSubsystem method but
      //	the diference is that in this one the argument is a single id so the
      //	functions compares the arguments to its own id and if it isn't propagates
      //	the calling to each of its sons or return error if there are no sons.
      virtual ClSSInterfaz * FindSubsystem (m4objid_t ai_lId	// The id of the subsystem to look for.
      );

      //## Operation: GetStats%937209553
      ClStatistic * GetStats ();

      //## Operation: SetStats%937209554
      //	asigna un objeto estadísticas
      void SetStats (ClStatistic *ai_poStats);

      //## Operation: SetFactory%937323528
      //	Asigna una referencia a la factoría genérica de subsistemas si este
      //	subsistema precisara dde ello por poder tener hijos de tipos dispares que se
      //	crean en la factoría genérica del server.
      void SetFactory (ClSSFactoryInterfaz *ai_poSSFactory);

      //## Operation: RegisterSnmp%1081845655
      virtual m4return_t RegisterSnmp ();

      //## Operation: SetState%937811411
      //	Cambia el estado del subsistema. Es un método protegido para que sea el
      //	propio subsistema el único que pueda cambiar el estado y además sea
      //	responsabilidad solamente suya el hacerlo, dependiendo de factores o eventos
      //	que le pueden llegar bien desde fuera como desde él mismo.
      void SetState (eStatus ai_iStatus, m4pchar_t ai_szState);

      //## Operation: GetState%939143525
      //	Devuelve el estado actual del subsistema. Siendo este del tipo enumerado
      //	definido en esta clase
      void GetState (eStatus &ao_eStatus);

      //## Operation: GetInterfaz%954148132
      ClSSInterfaz * GetInterfaz ();

      //## Operation: GetConfigNumericValue%954230998
      m4return_t GetConfigNumericValue (m4pchar_t ai_pszName, m4double_t &ao_rdNumber);

      //## Operation: GetConfigStringValue%954230999
      m4return_t GetConfigStringValue (m4pchar_t ai_pszName, m4pchar_t ao_pszString, m4size_t ai_iStringSize, m4size_t& ao_riStringLength);

      //## Operation: SetConfigNumericProperty%954231003
      m4return_t SetConfigNumericProperty (m4pchar_t ai_szProperty, m4double_t ai_iValue);

      //## Operation: SetConfigStringProperty%954231004
      m4return_t SetConfigStringProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue);

      //## Operation: GetInstance%954231007; C++
      int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: SetElement%955100217
      virtual m4return_t SetElement (m4pchar_t ai_szElement, void *ai_poElement);

      //## Operation: ProcessAction%957943670
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: GetStatisticAttribValue%994763173
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetStatisticAttribsId%994763174
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribValue%994763175
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribsId%994763176
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%994763181
      //	Ejecuta un comando de los disponibles del subsistema. Sus parámetros son: el
      //	identificador del comando (ID) la lista de parámetros de entrada y el
      //	contenedor para los parámetros de salida.
      //	Previamente a esta  llamada se deben de haber solicitado la lista de
      //	comandos y la lista de parámetros del comando en cuestión mediante los
      //	métodos GetCommandIds y GetCommandParams de la misma clase
      virtual m4return_t ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%994763182
      //	dado un comando concreto indicado por el primer parátro de entrada que
      //	especifica su ID de comando, se devuelve la lista  de parámetros de este
      //	comando.
      virtual m4return_t GetCommandParams (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%994763183
      //	Se devuelve la lista de comandos de un subsistema concreto. por cada comando
      //	se tine un identificador numérico del comando, uno en formato cadena y un
      //	falg indicativo de si el comando lleva parámetros, bien de entrada o de
      //	salida.
      virtual m4return_t GetCommandsId (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList);

      //## Operation: GetSubsystemsId%994763184
      //	This method returns a list of Id of all the son nodes of this subsystem
      virtual m4return_t GetSubsystemsId (m4pchar_t ai_szPath, vector <m4objid_t > &ao_poTags);

      //## Operation: GetssId%994922158
      m4pchar_t GetssId ();

      //## Operation: GetPath%994922159
      m4pchar_t GetPath ();

      //## Operation: HasOwner%996589393
      m4bool_t HasOwner ();

      //## Operation: GetSnmpObject%1081862806
      M4ISnmpObject* GetSnmpObject ();

      //## Operation: UnRegisterSnmp%1083593888
      virtual m4return_t UnRegisterSnmp ();

	  // Indica si el subsistema es un ejecutor o no. Por defecto no lo son
      virtual m4bool_t IsExecutor( void )
	  {
		  return( M4_FALSE ) ;
	  }

    // Data Members for Class Attributes

      //## Attribute: m_iInstance%407A5E4D01B0
      //## begin ClSSImplementation::m_iInstance%407A5E4D01B0.attr preserve=no  public: m4uint32_t {UA} 
      m4uint32_t m_iInstance;
      //## end ClSSImplementation::m_iInstance%407A5E4D01B0.attr

  public:
    // Additional Public Declarations
      //## begin ClSSImplementation%37A54BCB0070.public preserve=yes
      //## end ClSSImplementation%37A54BCB0070.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetSubsystemAttribValue%933578390
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetSubsystemAttribValue (m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage);

      //## Operation: ExecuteCommand%933578394
      //	Ejecuta un comando de los disponibles del subsistema. Sus parámetros son: el
      //	identificador del comando (ID) la lista de parámetros de entrada y el
      //	contenedor para los parámetros de salida.
      //	Previamente a esta  llamada se deben de haber solicitado la lista de
      //	comandos y la lista de parámetros del comando en cuestión mediante los
      //	métodos GetCommandIds y GetCommandParams de la misma clase
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%933578399
      //	dado un comando concreto indicado por el primer parátro de entrada que
      //	especifica su ID de comando, se devuelve la lista  de parámetros de este
      //	comando.
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%933578400
      //	Se devuelve la lista de comandos de un subsistema concreto. por cada comando
      //	se tine un identificador numérico del comando, uno en formato cadena y un
      //	falg indicativo de si el comando lleva parámetros, bien de entrada o de
      //	salida.
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetStatisticAttribsId%933578402
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetStatisticAttribsId (M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribsId%933578403
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemsId%933578404
      //	This method returns a list of Id of all the son nodes of this subsystem
      virtual m4return_t GetSubsystemsId (vector <m4objid_t > &ao_poTags);

      //## Operation: GetConfiguration%954324478
      ClConfiguration * GetConfiguration ();

      //## Operation: RegisterSnmpProperties%1066226438
      virtual m4return_t RegisterSnmpProperties (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject);

      //## Operation: RegisterSnmpStatistics%1066226439
      virtual m4return_t RegisterSnmpStatistics (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject);

      //## Operation: RegisterSnmpCommands%1066226440
      virtual m4return_t RegisterSnmpCommands (M4ISnmpAdministrator* ai_poSnmpAdmin, M4ISnmpObject* ai_poParentSnmpObject);

    // Data Members for Class Attributes

      //## Attribute: m_poTables%392412AC00AE
      //## begin ClSSImplementation::m_poTables%392412AC00AE.attr preserve=no  protected: ClTables * {UA} 
      ClTables *m_poTables;
      //## end ClSSImplementation::m_poTables%392412AC00AE.attr

      //## Attribute: m_poSnmpObject%3FF01ED70032
      //## begin ClSSImplementation::m_poSnmpObject%3FF01ED70032.attr preserve=no  protected: M4ISnmpObject* {UA} NULL
      M4ISnmpObject* m_poSnmpObject;
      //## end ClSSImplementation::m_poSnmpObject%3FF01ED70032.attr

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37A587970389
      //## Role: ClSSImplementation::m_oSyncBellowLevel%37A5879903AA
      //	Este es un elemento de sincronización del mapa de subsistemas hijos. Usando
      //	el algiritmo de lectores escritores sincroniza el acceso al mapa.
      //	Por defecto la clase ClSSimplementation no sincroniza el acceso pero los
      //	subsistema que deban hacerlo tienen que usar este objeto a tal efecto.
      //## begin ClSSImplementation::m_oSyncBellowLevel%37A5879903AA.role preserve=no  protected: ClWriterReaders { -> VHAN}
      ClWriterReaders m_oSyncBellowLevel;
      //## end ClSSImplementation::m_oSyncBellowLevel%37A5879903AA.role

      //## Association: M4Subsystems::<unnamed>%37A5B01000AD
      //## Role: ClSSImplementation::m_poSSFactory%37A5B0110144
      //	Referencia a la factoría genérica de subsistemas para dar la posibilidad de
      //	generar subsistemas no soo de los tipos que cada subsistema sabe generar
      //	sino de cualquier tipo que sepa generar la factoría global.
      //## begin ClSSImplementation::m_poSSFactory%37A5B0110144.role preserve=no  protected: static ClSSFactoryInterfaz { -> RHAN}
      static ClSSFactoryInterfaz *m_poSSFactory;
      //## end ClSSImplementation::m_poSSFactory%37A5B0110144.role

      //## Association: M4Subsystems::<unnamed>%37A69CFE01CA
      //## Role: ClSSImplementation::m_poStatistics%37A69CFF0081
      //## begin ClSSImplementation::m_poStatistics%37A69CFF0081.role preserve=no  protected: ClStatistic { -> RHAN}
      ClStatistic *m_poStatistics;
      //## end ClSSImplementation::m_poStatistics%37A69CFF0081.role

      //## Association: M4Subsystems::<unnamed>%37A69D2F0257
      //## Role: ClSSImplementation::m_poVisitor%37A69D3001FF
      //## begin ClSSImplementation::m_poVisitor%37A69D3001FF.role preserve=no  protected: ClVisitor { -> RHAN}
      ClVisitor *m_poVisitor;
      //## end ClSSImplementation::m_poVisitor%37A69D3001FF.role

      //## Association: M4Subsystems::<unnamed>%37A6C4B40042
      //## Role: ClSSImplementation::m_bReadyToClose%37A6C4B4034F
      //	Varibale interna que indica si ya está preparado el subsistema para hacer el
      //	shutdown. Es el mismo subsitema el que se debe encargar de actualizarla.
      //## begin ClSSImplementation::m_bReadyToClose%37A6C4B4034F.role preserve=no  protected: m4bool_t { -> VHAN}
      m4bool_t m_bReadyToClose;
      //## end ClSSImplementation::m_bReadyToClose%37A6C4B4034F.role

      //## Association: M4Subsystems::<unnamed>%37A55BE303B4
      //## Role: ClSSImplementation::m_lId%37A55BE403AB
      //## begin ClSSImplementation::m_lId%37A55BE403AB.role preserve=no  protected: m4objid_t { -> VHAN}
      m4objid_t m_lId;
      //## end ClSSImplementation::m_lId%37A55BE403AB.role

      //## Association: M4Subsystems::<unnamed>%37A555330041
      //## Role: ClSSImplementation::m_poNavigator%37A5553302EA
      //	Es la referencia al navegador que todos los subsistemas comparten para el
      //	acceso a las diferentes partes del árbol de subsistemas
      //## begin ClSSImplementation::m_poNavigator%37A5553302EA.role preserve=no  protected: ClNavigatorFirewall { -> RHAN}
      ClNavigatorFirewall *m_poNavigator;
      //## end ClSSImplementation::m_poNavigator%37A5553302EA.role

      //## Association: M4Subsystems::<unnamed>%37A54E76013B
      //## Role: ClSSImplementation::m_oBellowLevel%37A54E770042
      //	El es mapa que contiene a la lista de subsistemas hijos de cada subsistemas.
      //## begin ClSSImplementation::m_oBellowLevel%37A54E770042.role preserve=no  protected: INTERFACESMAP { -> VHAN}
      INTERFACESMAP m_oBellowLevel;
      //## end ClSSImplementation::m_oBellowLevel%37A54E770042.role

      //## Association: M4Subsystems::<unnamed>%37A54FC201CE
      //## Role: ClSSImplementation::m_oUsedInterfaces%37A54FC300AD
      //## begin ClSSImplementation::m_oUsedInterfaces%37A54FC300AD.role preserve=no  protected: INTERFACESMAP { -> VHAN}
      INTERFACESMAP m_oUsedInterfaces;
      //## end ClSSImplementation::m_oUsedInterfaces%37A54FC300AD.role

      //## Association: M4Subsystems::<unnamed>%37A54F3F0343
      //## Role: ClSSImplementation::m_poInterfaz%37A54F4002EA
      //	Referencia al interfaz propio de cada subsistema
      //## begin ClSSImplementation::m_poInterfaz%37A54F4002EA.role preserve=no  protected: ClSSInterfaz { -> RHAN}
      ClSSInterfaz *m_poInterfaz;
      //## end ClSSImplementation::m_poInterfaz%37A54F4002EA.role

    // Additional Protected Declarations
      //## begin ClSSImplementation%37A54BCB0070.protected preserve=yes
      //## end ClSSImplementation%37A54BCB0070.protected

  private:
    //## Constructors (specified)
      //## Operation: ClSSImplementation%954148131
      //	Constructor por defecto no accesible.
      ClSSImplementation ();

    // Data Members for Class Attributes

      //## Attribute: m_szSSName%37A6AAD00362
      //## begin ClSSImplementation::m_szSSName%37A6AAD00362.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_szSSName;
      //## end ClSSImplementation::m_szSSName%37A6AAD00362.attr

      //## Attribute: m_lType%37A6AF0E009D
      //	Este miembro contiene el tipo al que petenece el subsisterma. Los defines
      //	utilizados para detereminar los tipos de los subsistemas se encuentran en el
      //	archivo de cabecera subsistemids.hpp
      //## begin ClSSImplementation::m_lType%37A6AF0E009D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_lType;
      //## end ClSSImplementation::m_lType%37A6AF0E009D.attr

      //## Attribute: m_bActive%37F8EC260399
      //	Variable miembro en donde tenemos el estado del subsistema. Existen
      //	subsistemas que siempre van a estar en estado de ACTIVO y otros que
      //	cambiarán entre los posibles estados que cada uno pueda tomar.
      //## begin ClSSImplementation::m_bActive%37F8EC260399.attr preserve=no  private: eStatus {UA} 
      eStatus m_bActive;
      //## end ClSSImplementation::m_bActive%37F8EC260399.attr

      //## Attribute: m_ssId%3B4D4E33005A
      //## begin ClSSImplementation::m_ssId%3B4D4E33005A.attr preserve=no  private: m4string_t {UA} "0"
      m4string_t m_ssId;
      //## end ClSSImplementation::m_ssId%3B4D4E33005A.attr

      //## Attribute: m_ssPath%3B4D4E760345
      //## begin ClSSImplementation::m_ssPath%3B4D4E760345.attr preserve=no  private: m4string_t {UA} "0"
      m4string_t m_ssPath;
      //## end ClSSImplementation::m_ssPath%3B4D4E760345.attr

      //## Attribute: m_bHasOwner%3B66BF94036B
      //## begin ClSSImplementation::m_bHasOwner%3B66BF94036B.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_bHasOwner;
      //## end ClSSImplementation::m_bHasOwner%3B66BF94036B.attr

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37A5BA1B02D0
      //## Role: ClSSImplementation::m_poConfiguration%37A5BA1C017D
      //	Este miembro contiene la configuración de cada subsistema. Por consulta a
      //	este miembro puede obtener los valores de las propiedades con las que se ha
      //	configurado el subsistema
      //## begin ClSSImplementation::m_poConfiguration%37A5BA1C017D.role preserve=no  private: ClConfiguration { -> RFHAN}
      ClConfiguration *m_poConfiguration;
      //## end ClSSImplementation::m_poConfiguration%37A5BA1C017D.role

    // Additional Private Declarations
      //## begin ClSSImplementation%37A54BCB0070.private preserve=yes
      //## end ClSSImplementation%37A54BCB0070.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSImplementation%37A54BCB0070.implementation preserve=yes
      //## end ClSSImplementation%37A54BCB0070.implementation

};

//## begin ClSSImplementation%37A54BCB0070.postscript preserve=yes
//## end ClSSImplementation%37A54BCB0070.postscript

// Class ClSSImplementation 


//## Other Operations (inline)
inline ClStatistic * ClSSImplementation::GetStats ()
{
  //## begin ClSSImplementation::GetStats%937209553.body preserve=yes
	return m_poStatistics ;
  //## end ClSSImplementation::GetStats%937209553.body
}

inline void ClSSImplementation::SetStats (ClStatistic *ai_poStats)
{
  //## begin ClSSImplementation::SetStats%937209554.body preserve=yes
	m_poStatistics = ai_poStats ;
  //## end ClSSImplementation::SetStats%937209554.body
}

//## begin module%37A555F8036F.epilog preserve=yes
//## end module%37A555F8036F.epilog


#endif
