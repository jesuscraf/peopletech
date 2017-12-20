//## begin module%3420135B0078.cm preserve=no
//## end module%3420135B0078.cm

//## begin module%3420135B0078.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			ConnectionManager
//	 File:				SessManager.hpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971016
//	 Language:			C++
//	 Operating System:	WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%3420135B0078.cp

//## Module: SessManager%3420135B0078; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\sessmanager.hpp

#ifndef sessmanager_h
#define sessmanager_h 1

//## begin module%3420135B0078.additionalIncludes preserve=no
//## end module%3420135B0078.additionalIncludes

//## begin module%3420135B0078.includes preserve=yes
#include "m4stl.hpp"
//## end module%3420135B0078.includes

// syncro
#include <syncro.hpp>
// Statistics
#include <statistics.hpp>
// CSUtil
#include <csutil.hpp>
// FIFOQ
#include <fifoq.hpp>
// USService
#include <usservice.hpp>

class ClTagList;
class ClSessManagerVisitor;
class ClConnection;
class ClLocalMonitor;
class ClRequest;

//## begin module%3420135B0078.declarations preserve=no
//## end module%3420135B0078.declarations

//## begin module%3420135B0078.additionalDeclarations preserve=yes
const m4uint32_t SessManagerPaused = 0 ;
const m4uint32_t SessManagerActive = 1 ;
//## end module%3420135B0078.additionalDeclarations


//## begin ClSessManagerStats%346220D303B0.preface preserve=yes
//## end ClSessManagerStats%346220D303B0.preface

//## Class: ClSessManagerStats%346220D303B0
//	This class provides a  container where Connection Manager's statistical data
//	is to be stored. A specific visitor can retrieve such data and process them
//	conveniently.
//
//	Access to the contents of this class is performed on an ID basis and through
//	the appropriate get/set functions.
//## Category: sessionManager%34183CC901EF
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B61DAB02CD;ClStatAverageType { -> }

class ClSessManagerStats : public ClActiveClassStats  //## Inherits: <unnamed>%346220E50100
{
  //## begin ClSessManagerStats%346220D303B0.initialDeclarations preserve=yes
  //## end ClSessManagerStats%346220D303B0.initialDeclarations

  public:
    //## Class: eSessManagerStatsID%34D76D0B0391
    //## Category: sessionManager%34183CC901EF
    //## Subsystem: version::inc%379ECBD70299
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {INCORRECT_PDUS_ID= ClActiveClassStats::TOTAL_NUM, UNKNOWN_HOSTS_ID, UNKNOWN_USERS_ID, INVALID_TIME_MARKS_ID, INVALID_PROTOCOLV_ID, INVALID_USER_AUT_ID, FORGED_SIGN_ID, DENIED_CONNECTS_ID, TIMEOUTS_ID, TOTAL_NUM} eSessManagerStatsID;

    //## Constructors (specified)
      //## Operation: ClSessManagerStats%884336004
      ClSessManagerStats ();

    //## Destructor (specified)
      //## Operation: ~ClSessManagerStats%884336005
      ~ClSessManagerStats ();


    //## Other Operations (specified)
      //## Operation: Reset%884195142; C++
      //	This operation resets every value in the statistics object.
      m4return_t Reset ();

      //## Operation: GetStat%886596294; C++
      //	Get operation to retrieve a literal value with index ai_hStatId.
      m4return_t GetStat (ClStatHandle &ai_hStatId, ClDataUnit &ao_poData);

      //## Operation: ResetStat%886596296
      //	This operation resets the value specified by ai_hStatId in the statistics
      //	object. In this class this operation is abstract and must be defined in
      //	derived classes.
      m4return_t ResetStat (ClStatHandle &ai_hStatId);

    //## Get and Set Operations for Associations (generated)

      //## Association: sessionManager::<unnamed>%34B3A12901F4
      //## Role: ClSessManagerStats::nIncorrectPDUs%34B3A12A01D4
      ClStatNumericalType& GetnIncorrectPDUs ();
      void AddnIncorrectPDUs (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3A8F803A9
      //## Role: ClSessManagerStats::nUnknownHosts%34B3A8F90399
      ClStatNumericalType& GetnUnknownHosts ();
      void AddnUnknownHosts (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3AB3D00FA
      //## Role: ClSessManagerStats::nUnknownUsers%34B3AB3E0000
      ClStatNumericalType& GetnUnknownUsers ();
      void AddnUnknownUsers (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3ABFD0138
      //## Role: ClSessManagerStats::nForgedSignatures%34B3ABFE00AB
      ClStatNumericalType& GetnForgedSignatures ();
      void AddnForgedSignatures (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3B0440148
      //## Role: ClSessManagerStats::nInvalidUserAuthentications%34B3B04500CB
      ClStatNumericalType& GetnInvalidUserAuthentications ();
      void AddnInvalidUserAuthentications (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3B0C8038A
      //## Role: ClSessManagerStats::nInvalidTimeMarks%34B3B0CA003E
      ClStatNumericalType& GetnInvalidTimeMarks ();
      void AddnInvalidTimeMarks (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3B5EC03A9
      //## Role: ClSessManagerStats::nInvalidProtocolVersions%34B3B5EE004E
      ClStatNumericalType& GetnInvalidProtocolVersions ();
      void AddnInvalidProtocolVersions (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3B8FE00AB
      //## Role: ClSessManagerStats::nDeniedConnections%34B3B8FF0148
      ClStatNumericalType& GetnDeniedConnections ();
      void AddnDeniedConnections (const ClStatNumericalType& value);

      //## Association: sessionManager::<unnamed>%34B3B9530000
      //## Role: ClSessManagerStats::nTimeouts%34B3B953030D
      ClStatNumericalType& GetnTimeouts ();
      void AddnTimeouts (const ClStatNumericalType& value);

  public:
    // Additional Public Declarations
      //## begin ClSessManagerStats%346220D303B0.public preserve=yes
      //## end ClSessManagerStats%346220D303B0.public

  protected:
    // Data Members for Associations

      //## Association: sessionManager::<unnamed>%34B3A12901F4
      //## begin ClSessManagerStats::nIncorrectPDUs%34B3A12A01D4.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nIncorrectPDUs;
      //## end ClSessManagerStats::nIncorrectPDUs%34B3A12A01D4.role

      //## Association: sessionManager::<unnamed>%34B3A8F803A9
      //## begin ClSessManagerStats::nUnknownHosts%34B3A8F90399.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nUnknownHosts;
      //## end ClSessManagerStats::nUnknownHosts%34B3A8F90399.role

      //## Association: sessionManager::<unnamed>%34B3AB3D00FA
      //## begin ClSessManagerStats::nUnknownUsers%34B3AB3E0000.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nUnknownUsers;
      //## end ClSessManagerStats::nUnknownUsers%34B3AB3E0000.role

      //## Association: sessionManager::<unnamed>%34B3ABFD0138
      //## begin ClSessManagerStats::nForgedSignatures%34B3ABFE00AB.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nForgedSignatures;
      //## end ClSessManagerStats::nForgedSignatures%34B3ABFE00AB.role

      //## Association: sessionManager::<unnamed>%34B3B0440148
      //## begin ClSessManagerStats::nInvalidUserAuthentications%34B3B04500CB.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nInvalidUserAuthentications;
      //## end ClSessManagerStats::nInvalidUserAuthentications%34B3B04500CB.role

      //## Association: sessionManager::<unnamed>%34B3B0C8038A
      //## begin ClSessManagerStats::nInvalidTimeMarks%34B3B0CA003E.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nInvalidTimeMarks;
      //## end ClSessManagerStats::nInvalidTimeMarks%34B3B0CA003E.role

      //## Association: sessionManager::<unnamed>%34B3B5EC03A9
      //## begin ClSessManagerStats::nInvalidProtocolVersions%34B3B5EE004E.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nInvalidProtocolVersions;
      //## end ClSessManagerStats::nInvalidProtocolVersions%34B3B5EE004E.role

      //## Association: sessionManager::<unnamed>%34B3B8FE00AB
      //## begin ClSessManagerStats::nDeniedConnections%34B3B8FF0148.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nDeniedConnections;
      //## end ClSessManagerStats::nDeniedConnections%34B3B8FF0148.role

      //## Association: sessionManager::<unnamed>%34B3B9530000
      //## begin ClSessManagerStats::nTimeouts%34B3B953030D.role preserve=no  public: ClStatNumericalType {1 -> 1VHN}
      ClStatNumericalType m_nTimeouts;
      //## end ClSessManagerStats::nTimeouts%34B3B953030D.role

    // Additional Protected Declarations
      //## begin ClSessManagerStats%346220D303B0.protected preserve=yes
      //## end ClSessManagerStats%346220D303B0.protected

  private:
    // Additional Private Declarations
      //## begin ClSessManagerStats%346220D303B0.private preserve=yes
      //## end ClSessManagerStats%346220D303B0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessManagerStats%346220D303B0.implementation preserve=yes
      //## end ClSessManagerStats%346220D303B0.implementation

};

//## begin ClSessManagerStats%346220D303B0.postscript preserve=yes
//## end ClSessManagerStats%346220D303B0.postscript

//## begin ClMapOfHandle%3552D73A02B9.preface preserve=yes
//## end ClMapOfHandle%3552D73A02B9.preface

//## Class: ClMapOfHandle%3552D73A02B9; Instantiated Class
//## Category: sessionManager%34183CC901EF
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< m4handle_t,ClRequest *,less<m4handle_t> > ClMapOfHandle;

//## begin ClMapOfHandle%3552D73A02B9.postscript preserve=yes
//## end ClMapOfHandle%3552D73A02B9.postscript

//## begin ClRequestRegistry%357ECDBD001C.preface preserve=yes
//## end ClRequestRegistry%357ECDBD001C.preface

//## Class: ClRequestRegistry%357ECDBD001C
//## Category: sessionManager%34183CC901EF
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRequestRegistry 
{
  //## begin ClRequestRegistry%357ECDBD001C.initialDeclarations preserve=yes
  //## end ClRequestRegistry%357ECDBD001C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRequestRegistry%897503491
      ClRequestRegistry ();

    //## Destructor (specified)
      //## Operation: ~ClRequestRegistry%897503492
      ~ClRequestRegistry ();


    //## Other Operations (specified)
      //## Operation: RegisterReq%897503493; C++
      m4return_t RegisterReq (m4handle_t ai_hKey, ClRequest *ai_pRequest);

      //## Operation: DeregisterReq%897503495; C++
      m4return_t DeregisterReq (m4handle_t ai_hKey, ClRequest *&ao_pRequest);

      //## Operation: FindReq%897503496; C++
      m4return_t FindReq (m4handle_t ai_hKey, ClRequest *&ao_pRequest);

    // Additional Public Declarations
      //## begin ClRequestRegistry%357ECDBD001C.public preserve=yes
      //## end ClRequestRegistry%357ECDBD001C.public

  protected:
    // Data Members for Associations

      //## Association: sessionManager::<unnamed>%357ECDDE01B3
      //## Role: ClRequestRegistry::m_oContainer%357ECDDF001D
      //## begin ClRequestRegistry::m_oContainer%357ECDDF001D.role preserve=no  protected: ClMapOfHandle {1 -> 1VHAN}
      ClMapOfHandle m_oContainer;
      //## end ClRequestRegistry::m_oContainer%357ECDDF001D.role

      //## Association: sessionManager::<unnamed>%36F0D897016A
      //## Role: ClRequestRegistry::s_oSync%36F0D89802B6
      //## begin ClRequestRegistry::s_oSync%36F0D89802B6.role preserve=no  protected: static ClMutex {1 -> 1VHAN}
      static ClMutex s_oSync;
      //## end ClRequestRegistry::s_oSync%36F0D89802B6.role

    // Additional Protected Declarations
      //## begin ClRequestRegistry%357ECDBD001C.protected preserve=yes
      //## end ClRequestRegistry%357ECDBD001C.protected

  private:
    // Additional Private Declarations
      //## begin ClRequestRegistry%357ECDBD001C.private preserve=yes
      //## end ClRequestRegistry%357ECDBD001C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRequestRegistry%357ECDBD001C.implementation preserve=yes
      //## end ClRequestRegistry%357ECDBD001C.implementation

};

//## begin ClRequestRegistry%357ECDBD001C.postscript preserve=yes
//## end ClRequestRegistry%357ECDBD001C.postscript

//## begin ClCSSessManager%34200ECB03D4.preface preserve=yes
//## end ClCSSessManager%34200ECB03D4.preface

//## Class: ClCSSessManager%34200ECB03D4
//	Esta clase se encarga de la gestión de las PDU de control enviadas por los
//	clientes (negociación y liberación de la conexión, etc.).
//## Category: sessionManager%34183CC901EF
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34475ED402EE;ClRequest { -> F}
//## Uses: <unnamed>%344B85F701EE;ClSessManagerVisitor { -> F}
//## Uses: <unnamed>%344B8A4803E2;ClConnection { -> F}
//## Uses: <unnamed>%34622A940362;ClSessManagerStats { -> }
//## Uses: <unnamed>%346AD1320166;ClPDU { -> F}
//## Uses: <unnamed>%349E772400F5;ClDirectory { -> F}
//## Uses: <unnamed>%34D0A20E015E;ClAuthenticationPDU { -> F}
//## Uses: <unnamed>%34D0A2210267;ClConnectPDU { -> F}
//## Uses: <unnamed>%34DF2C48016F; { -> }
//## Uses: <unnamed>%3535C6350190;ClAckPDU { -> F}
//## Uses: <unnamed>%356475180064;ClCommandRequest { -> F}
//## Uses: <unnamed>%356582CB00E1;ClLocalMonitor { -> F}
//## Uses: <unnamed>%3569382903B8; { -> F}
//## Uses: <unnamed>%358796D100F0;ClCSSender { -> F}
//## Uses: <unnamed>%36A4564A0166;ClBoomerangRequest { -> F}
//## Uses: <unnamed>%36A49CC90207;ClUSSessionInterface { -> F}
//## Uses: <unnamed>%36A49CFA017B;ClUSUser { -> F}
//## Uses: <unnamed>%36E7BC6D038B;ClTagList { -> F}
//## Uses: <unnamed>%36F6205800CF;m4bool_t { -> F}
//## Uses: <unnamed>%379EC586026C;ClUSServiceAPI { -> }

class ClCSSessManager : public ClActiveClass  //## Inherits: <unnamed>%34200ECB03D5
{
  //## begin ClCSSessManager%34200ECB03D4.initialDeclarations preserve=yes
  //## end ClCSSessManager%34200ECB03D4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCSSessManager%874517660; C++
      //	Parameterized constructor for class ClCSConnManager.
      //	Remarks
      //	Instantiates the parent class ClCSActiveClass with one unique thread
      //	associated to it.
      //	Constructor:
      //	Inicializa las estadísticas, crea el hilo y retorna.
      ClCSSessManager (ClLocalMonitor* ai_pMonitor, ClControlQ *ai_pControlQ, ClStatistic *ai_pStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClCSSessManager%874517661
      //	Destructor for class ClCSConnManager.
      //	Remarks
      //	Destructor:
      //	Pone marca de autodestrucción.
      ~ClCSSessManager ();


    //## Other Operations (specified)
      //## Operation: Run%875868001; C++
      //	Run:
      //	En reposo, el hilo está en una extracción bloqueante de la cola de PDU de
      //	control.
      //	Entonces incrementa el contador de PDU procesadas.
      //	Después se comprueba el tipo de petición. Básicamente, puede ser alguna PDU
      //	relacionada con la apertura de la sesión, alguna PDU relacionada con el
      //	cierre de la sesión, alguna indicación de temporizador vencido o alguna
      //	indicación de conexión comprobada por el MC.
      //	- Si es una PDU 0x10 (conexión):
      //	Bloquear el autómata de la conexión.
      //	Cancelar temporizador.
      //	Verificar que el estado de la conexión asociada es OPENING1. Si no es así,
      //	terminar la conexión (en adelante, esto significa: ignorar la PDU, pasar la
      //	conexión a CLOSED, cerrar el TSAP, actualizar estadísticas oEndTime y oLast
      //	TransitionTime e invocar Monitor.LocalCheckComplete(mal)).
      //	Verificar el número mágico y, si no es el esperado, terminar la conexión.
      //	Instanciar un objeto ClHost con el ID de puesto especificado en la PDU. Si
      //	no es válido el puesto, registrar en el log y terminar la conexión.
      //	Si el puesto usa encriptación asimétrica, instanciar un encriptador
      //	asimétrico con la clave pública del puesto y la privada del servidor y
      //	asignarlo a la conexión. Si no, instanciar el encriptador que corresponda o
      //	no instanciar nada, según el nivel de seguridad del puesto.
      //	Comprobar la firma (si el nivel de seguridad del puesto lo permite). Si no
      //	es válida, registrar en el log y terminar la conexión.
      //	De las versiones del protocolo especificadas por el cliente, seleccionar la
      //	preferida. Si no es posible seleccionar ninguna versión, pasar la conexión a
      //	CLOSING1, actualizar oLastTransitionTime, generar una PDU de rechazo de
      //	conexión (0x01) con la causa adecuada y encolarla en la cola de envío de la
      //	conexión con la marca de cerrar la conexión. Desbloquear el autómata y
      //	repetir el bucle.
      //	Generar una clave de sesión, instanciar un encriptador simétrico con esta
      //	clave y asignarlo a la conexión.
      //	Pasar la conexión a estado OPENING2 y actualizar oLastTransitionTime.
      //	Componer una PDU 0x11 (preconfirmación de conexión), encriptarla, firmarla y
      //	encolarla en la cola de envío de la conexión.
      //	Activar temporizador.
      //	Desbloquear el autómata y repetir el bucle.
      //	- Si es una PDU 0x12 (autenticación):
      //	Bloquear el autómata de la conexión.
      //	Cancelar temporizador.
      //	Verificar que la conexión está en estado OPENING2. En caso contrario,
      //	terminar la conexión.
      //	Desencriptar la PDU y verificar su integridad (marca de tiempo, firma o
      //	número mágico). Si se detecta un fallo, registrar en el log y terminar la
      //	conexión.
      //	Instanciar un objeto ClUser con el ID de usuario especificado en la PDU y
      //	asociarlo a la conexión. Si el usuario no existe, registrar en el log y
      //	terminar la conexión.
      //	Obtener la contraseña del usuario y compararla con la que viene en la PDU.
      //	Si no es correcta, registrar en el log y terminar la conexión. çççAquí y en
      //	la anterior, quizá sería más educado responder al cliente que el usuario o
      //	la contraseña no son válidos, ya que al menos el puesto sí lo era y quedamos
      //	mejor.ççç
      //	Pasar la conexión a OPENING3 y actualizar oLastTransitionTime.
      //	Invocar Monitor.LocalCheckComplete(bien) y repetir el bucle.
      //	- Si es una PDU ficticia de comprobaciones globales finalizadas.
      //	Bloquear el autómata.
      //	Si la conexión no se autoriza, pasar la conexión a CLOSING1, actualizar oLast
      //	TransitionTime, generar una PDU de rechazo de conexión (0x01) con la causa
      //	adecuada y encolarla en la cola de envío de la conexión con la marca de
      //	cerrar la conexión.
      //	En caso contrario, obtener servicios válidos para el usuario, calcular
      //	máscara de permisos, pasar la conexión a ACTIVE, actualizar oLastTransition
      //	Time, generar una PDU de confirmación de conexión (0x13) y encolarla en la
      //	cola de envío de la conexión.
      //	Desbloquear el autómata.
      //	Invocar Monitor.RegisterConnection y repetir el bucle.
      //	A partir de aquí, terminar la conexión significa: ignorar la PDU, pasar la
      //	conexión a CLOSED, cerrar el TSAP, actualizar oLastTransitionTime y oEnd
      //	Time, e invocar Monitor.RemoveConnection.
      //	- Si es una PDU ficticia de vencimiento de temporizador.
      //	Bloquear el autómata.
      //	Terminar la conexión.çççTratamiento poco refinado.ççç
      //	Desbloquear el autómata y repetir el bucle.
      //	- Si es una PDU de desconexión (0x24):
      //	Bloquear el autómata.
      //	Si el estado no es ACTIVE, terminar la conexión.
      //	Verificar la marca de tiempo y, si no es válida, terminar la conexión.
      //	Registrar motivo en el log.
      //	Pasar la conexión a CLOSING1 y actualizar oLastTransitionTime y oEndTime.
      //	Generar PDU de confirmación (0x00) y encolarla en la cola de envío de la
      //	conexión con la marca de cerrar tras enviarla.
      //	Desbloquear el autómata.
      //	Invocar Monitor.RemoveConnection y repetir el bucle.
      //	Como se ve, cualquier error de protocolo provoca un cierre inmediato de la
      //	conexión. ççç Consultar si eso es aceptable para Meta4.ççç
      virtual void Run (void );

      //## Operation: Accept%877356011; C++
      //	Allows a visitor to enter the class and retrieve statistics.
      virtual void Accept (ClActiveClassVisitor &ai_oStats);

      //## Operation: SetState%941813353
      void SetState (m4uint32_t ai_iState);

      //## Operation: _UpdateTagList%942259213; C++
      void _UpdateTagList (ClTagList &aio_oArgs, ClRequest *ai_poRequest);

      //## Operation: ProcessItem%954935297
      m4return_t ProcessItem (ClBaseRequest* ai_poItem);

    // Additional Public Declarations
      //## begin ClCSSessManager%34200ECB03D4.public preserve=yes
      //## end ClCSSessManager%34200ECB03D4.public

  protected:

    //## Other Operations (specified)
      //## Operation: _ProcessServiceReq%922096963; C++
      m4return_t _ProcessServiceReq (ClBaseRequest *&aio_pRequest, m4bool_t &ao_bReply, ClTagList *ao_poArgs = NULL);

      //## Operation: _ProcessCommandReq%922096964; C++
      m4return_t _ProcessCommandReq (ClBaseRequest *&aio_pRequest, m4bool_t &ao_bReply, ClTagList *ao_poArgs = NULL);

    // Data Members for Associations

      //## Association: sessionManager::<unnamed>%3433544D0353
      //## Role: ClCSSessManager::m_pControlQ%3433544E01EC
      //	Reference to the control queue where PDUs are read from.
      //## begin ClCSSessManager::m_pControlQ%3433544E01EC.role preserve=no  protected: ClControlQ {1 -> 1RHAN}
      ClControlQ *m_pControlQ;
      //## end ClCSSessManager::m_pControlQ%3433544E01EC.role

      //## Association: sessionManager::<unnamed>%3552D7A300F4
      //## Role: ClCSSessManager::s_oPendingReqs%3552D7A303C3
      //## begin ClCSSessManager::s_oPendingReqs%3552D7A303C3.role preserve=no  protected: static ClRequestRegistry {0..n -> 1VHAN}
      static ClRequestRegistry s_oPendingReqs;
      //## end ClCSSessManager::s_oPendingReqs%3552D7A303C3.role

      //## Association: sessionManager::<unnamed>%36F62179021F
      //## Role: ClCSSessManager::m_pServiceAPI%36F6217B007E
      //## begin ClCSSessManager::m_pServiceAPI%36F6217B007E.role preserve=no  protected: ClUSServiceAPI {1 -> 1RFHAN}
      ClUSServiceAPI *m_pServiceAPI;
      //## end ClCSSessManager::m_pServiceAPI%36F6217B007E.role

    // Additional Protected Declarations
      //## begin ClCSSessManager%34200ECB03D4.protected preserve=yes
      //## end ClCSSessManager%34200ECB03D4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iState%3822F003039C
      //## begin ClCSSessManager::m_iState%3822F003039C.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iState;
      //## end ClCSSessManager::m_iState%3822F003039C.attr

    // Additional Private Declarations
      //## begin ClCSSessManager%34200ECB03D4.private preserve=yes
      //## end ClCSSessManager%34200ECB03D4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCSSessManager%34200ECB03D4.implementation preserve=yes
      //## end ClCSSessManager%34200ECB03D4.implementation

};

//## begin ClCSSessManager%34200ECB03D4.postscript preserve=yes
//## end ClCSSessManager%34200ECB03D4.postscript

//## begin ClSessManagerVisitor%344B7DF50123.preface preserve=yes
//## end ClSessManagerVisitor%344B7DF50123.preface

//## Class: ClSessManagerVisitor%344B7DF50123
//	This class provides a public interface for accessing statistical information
//	from ClCSConnManager.
//	This approach is based on the visitor behavioral pattern as described in
//	[GAM].
//## Category: sessionManager%34183CC901EF
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%346228350035;ClSessManagerStats { -> }
//## Uses: <unnamed>%35546B5A01DB;ClCSSessManager { -> }

class ClSessManagerVisitor : public ClActiveClassVisitor  //## Inherits: <unnamed>%344B7E04000A
{
  //## begin ClSessManagerVisitor%344B7DF50123.initialDeclarations preserve=yes
  //## end ClSessManagerVisitor%344B7DF50123.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSessManagerVisitor%879608294; C++
      //	Constructor for class "ClConnManagerVisitor".
      ClSessManagerVisitor ();


    //## Other Operations (specified)
      //## Operation: VisitSessManager%877356012; C++
      //	This virtual operation accesses ClCSACtiveClass information from the
      //	instance provided through the ai_oActiveClass argument.
      //	Remarks
      //	· ProcessedPDUs (CR): número total de PDU de control procesadas.
      //	· IncorrectPDUs (CR): número total de PDU de control incorrectas (fuera de
      //	fase o con formato erróneo, por ejemplo).
      //	· AvgProcessedPDUs (CR): número medio de PDU de control procesadas por
      //	segundo durante los últimos 60, 300 y 900 segundos. Se actualiza cada 5
      //	segundos.
      //	· UnknownHosts (CR): número total de identificadores de puesto desconocidos.
      //	· UnknownUsers (CR): número total de identificadores de usuario desconocidos.
      //	· ForgedSignatures (CR): número total de firmas falsas detectadas.
      //	· InvalidUserAuthentications (CR): número total de conexiones rechazadas
      //	debido a que la autenticación del usuario era incorrecta.
      //	· InvalidTimeMarks (CR): número total de PDU con marca de tiempo incorrecta.
      //	· InvalidProtocolVersions (CR): número total de conexiones rechazadas debido
      //	a que no se pudo negociar la versión del protocolo.
      //	· DeniedConnections (CR): número total de conexiones denegadas por motivos
      //	globales (número máximo de sesiones simultáneas de un mismo usuario, etc.).
      //	· Timeouts (CR): número total de excesos de tiempo detectados. Nota: todos
      //	los contadores anteriores se ponen a cero a la vez.
      virtual void VisitSessManager (ClCSSessManager &ai_oSessManager);

    // Additional Public Declarations
      //## begin ClSessManagerVisitor%344B7DF50123.public preserve=yes
      //## end ClSessManagerVisitor%344B7DF50123.public

  protected:
    // Additional Protected Declarations
      //## begin ClSessManagerVisitor%344B7DF50123.protected preserve=yes
      //## end ClSessManagerVisitor%344B7DF50123.protected

  private:
    // Additional Private Declarations
      //## begin ClSessManagerVisitor%344B7DF50123.private preserve=yes
      //## end ClSessManagerVisitor%344B7DF50123.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSessManagerVisitor%344B7DF50123.implementation preserve=yes
      //## end ClSessManagerVisitor%344B7DF50123.implementation

};

//## begin ClSessManagerVisitor%344B7DF50123.postscript preserve=yes
//## end ClSessManagerVisitor%344B7DF50123.postscript

// Class ClSessManagerStats 

//## Get and Set Operations for Associations (inline)

inline ClStatNumericalType& ClSessManagerStats::GetnIncorrectPDUs ()
{
  //## begin ClSessManagerStats::GetnIncorrectPDUs%34B3A12A01D4.get preserve=no
  return m_nIncorrectPDUs;
  //## end ClSessManagerStats::GetnIncorrectPDUs%34B3A12A01D4.get
}

inline void ClSessManagerStats::AddnIncorrectPDUs (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnIncorrectPDUs%34B3A12A01D4.set preserve=yes
  m_nIncorrectPDUs += value;
  //## end ClSessManagerStats::AddnIncorrectPDUs%34B3A12A01D4.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnUnknownHosts ()
{
  //## begin ClSessManagerStats::GetnUnknownHosts%34B3A8F90399.get preserve=no
  return m_nUnknownHosts;
  //## end ClSessManagerStats::GetnUnknownHosts%34B3A8F90399.get
}

inline void ClSessManagerStats::AddnUnknownHosts (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnUnknownHosts%34B3A8F90399.set preserve=yes
  m_nUnknownHosts += value;
  //## end ClSessManagerStats::AddnUnknownHosts%34B3A8F90399.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnUnknownUsers ()
{
  //## begin ClSessManagerStats::GetnUnknownUsers%34B3AB3E0000.get preserve=no
  return m_nUnknownUsers;
  //## end ClSessManagerStats::GetnUnknownUsers%34B3AB3E0000.get
}

inline void ClSessManagerStats::AddnUnknownUsers (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnUnknownUsers%34B3AB3E0000.set preserve=yes
  m_nUnknownUsers += value;
  //## end ClSessManagerStats::AddnUnknownUsers%34B3AB3E0000.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnForgedSignatures ()
{
  //## begin ClSessManagerStats::GetnForgedSignatures%34B3ABFE00AB.get preserve=no
  return m_nForgedSignatures;
  //## end ClSessManagerStats::GetnForgedSignatures%34B3ABFE00AB.get
}

inline void ClSessManagerStats::AddnForgedSignatures (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnForgedSignatures%34B3ABFE00AB.set preserve=yes
  m_nForgedSignatures += value;
  //## end ClSessManagerStats::AddnForgedSignatures%34B3ABFE00AB.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnInvalidUserAuthentications ()
{
  //## begin ClSessManagerStats::GetnInvalidUserAuthentications%34B3B04500CB.get preserve=no
  return m_nInvalidUserAuthentications;
  //## end ClSessManagerStats::GetnInvalidUserAuthentications%34B3B04500CB.get
}

inline void ClSessManagerStats::AddnInvalidUserAuthentications (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnInvalidUserAuthentications%34B3B04500CB.set preserve=yes
  m_nInvalidUserAuthentications += value;
  //## end ClSessManagerStats::AddnInvalidUserAuthentications%34B3B04500CB.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnInvalidTimeMarks ()
{
  //## begin ClSessManagerStats::GetnInvalidTimeMarks%34B3B0CA003E.get preserve=no
  return m_nInvalidTimeMarks;
  //## end ClSessManagerStats::GetnInvalidTimeMarks%34B3B0CA003E.get
}

inline void ClSessManagerStats::AddnInvalidTimeMarks (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnInvalidTimeMarks%34B3B0CA003E.set preserve=yes
  m_nInvalidTimeMarks += value;
  //## end ClSessManagerStats::AddnInvalidTimeMarks%34B3B0CA003E.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnInvalidProtocolVersions ()
{
  //## begin ClSessManagerStats::GetnInvalidProtocolVersions%34B3B5EE004E.get preserve=no
  return m_nInvalidProtocolVersions;
  //## end ClSessManagerStats::GetnInvalidProtocolVersions%34B3B5EE004E.get
}

inline void ClSessManagerStats::AddnInvalidProtocolVersions (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnInvalidProtocolVersions%34B3B5EE004E.set preserve=yes
  m_nInvalidProtocolVersions += value;
  //## end ClSessManagerStats::AddnInvalidProtocolVersions%34B3B5EE004E.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnDeniedConnections ()
{
  //## begin ClSessManagerStats::GetnDeniedConnections%34B3B8FF0148.get preserve=no
  return m_nDeniedConnections;
  //## end ClSessManagerStats::GetnDeniedConnections%34B3B8FF0148.get
}

inline void ClSessManagerStats::AddnDeniedConnections (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnDeniedConnections%34B3B8FF0148.set preserve=yes
  m_nDeniedConnections += value;
  //## end ClSessManagerStats::AddnDeniedConnections%34B3B8FF0148.set
}

inline ClStatNumericalType& ClSessManagerStats::GetnTimeouts ()
{
  //## begin ClSessManagerStats::GetnTimeouts%34B3B953030D.get preserve=no
  return m_nTimeouts;
  //## end ClSessManagerStats::GetnTimeouts%34B3B953030D.get
}

inline void ClSessManagerStats::AddnTimeouts (const ClStatNumericalType& value)
{
  //## begin ClSessManagerStats::AddnTimeouts%34B3B953030D.set preserve=yes
  m_nTimeouts += value;
  //## end ClSessManagerStats::AddnTimeouts%34B3B953030D.set
}

// Class ClRequestRegistry 

// Class ClCSSessManager 

// Class ClSessManagerVisitor 


//## Other Operations (inline)
inline void ClSessManagerVisitor::VisitSessManager (ClCSSessManager &ai_oSessManager)
{
  //## begin ClSessManagerVisitor::VisitSessManager%877356012.body preserve=yes
	//	Processed PDUs
	//	Incorrect PDUs
	//	AvgProcessedPDUs
	//	Unknown Hosts
	//	Unknown Users
	//	Forged Signatures
	//	Invalid User Authentications
	//	Invalid Time Marks
	//	Invalid Protocol Version
	//	Denied Connections
	//	Time-outs
  	VisitActiveClass((ClActiveClass &)ai_oSessManager);
  //## end ClSessManagerVisitor::VisitSessManager%877356012.body
}

//## begin module%3420135B0078.epilog preserve=yes
//## end module%3420135B0078.epilog


#endif
