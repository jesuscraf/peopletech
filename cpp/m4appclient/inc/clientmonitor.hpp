//## begin module%347958800323.cm preserve=no
//## end module%347958800323.cm

//## begin module%347958800323.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              ClMonitorC.hpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980313
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
//## end module%347958800323.cp

//## Module: clientmonitor%347958800323; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\mybuild\m4appclient\inc\clientmonitor.hpp

#ifndef clientmonitor_h
#define clientmonitor_h 1

//## begin module%347958800323.additionalIncludes preserve=no
//## end module%347958800323.additionalIncludes

//## begin module%347958800323.includes preserve=yes
#include "m4appclient_dll.hpp"
//## end module%347958800323.includes

// clientreceiverq
#include <clientreceiverq.hpp>
// clientsenderq
#include <clientsenderq.hpp>
// syncro
#include <syncro.hpp>
// m4types
#include <m4types.hpp>
// stl
#include "m4stl.hpp"
// m4log
#include <m4log.hpp>

class ClCCDistributor;
class ClCCReceiver;
class ClCCService;
class ClCCRequest;
class ClCCSender;
class ClCCUser;
class ClCCConnection;
class ClCCSession;

//## begin module%347958800323.declarations preserve=no
//## end module%347958800323.declarations

//## begin module%347958800323.additionalDeclarations preserve=yes
//## end module%347958800323.additionalDeclarations


//## Class: ClCCMonitor%34472F040046
//	.
//	Cuando arranca el Monitor local hay que pasarles una serie de parametros que
//	si bien muchos no son totalmente imprecindible si que son necesarios.
//
//	La cadena de parámetros viene en un formato de variable=valor.  El conjunto
//	de variables y constantes irá variando a lo largo del tiempo.
//
//	· ID_SITE : Esta será la variable que identifica el puesto que tenemos.
//	· PRIVATE_KEY_SITE : Clave privada del puesto.
//	· GLOBAL_TIMEOUT_FOR_CONNECT : Por defecto es 60 sg. Si queremos cambiar el
//	por defecto usaremos esta variable.
//	· GLOBAL_TIMEOUT_FOR_OPEN_SERVICE : Por defecto 60 sg. Si queremos cambiar
//	el por defecto usaremos esta variable.
//	· GLOBAL_LOG_SYSTEM : Decidimos si queremos tener activo el sistema de log o
//	no.  Por defecto se dice que no va a haber sistema de log.
//	· GLOBAL_STATISTIC	: Se si se mantienen las estadísticas o no.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%350CF872031B;ClCCConnection { -> F}
//## Uses: <unnamed>%350CF8920359;ClCCUser { -> F}
//## Uses: <unnamed>%3552FED4021D;map { -> }
//## Uses: <unnamed>%379766C70349;ClLog { -> }

class M4_DECL_M4APPCLIENT ClCCMonitor 
{
  public:
    //## Destructor (specified)
      //## Operation: ~ClCCMonitor%881161849
      ~ClCCMonitor ();


    //## Other Operations (specified)
      //## Operation: InitCSC%879852431; C++
      //	.
      //	Realiza las tareas de inicialización del CSC. Comprueba que no se ha
      //	construido ninguna otra instancia de la clase, y en ese caso invoca al
      //	constructor.
      //	.
      static m4return_t InitCSC ();

      //## Operation: StopCSC%885808224; C++
      static m4return_t StopCSC ();

      //## Operation: GetClientMonitor%880446334; C++
      //	.
      //	El monitor implementa el patrón de diseño singleton. Este método devuelve un
      //	puntero a la única instancia de la clase. Si no existe ninguna devuelve
      //	error. De esta forma se permite que el resto de los componentes tengan
      //	acceso al monitor.
      //	.
      static m4return_t GetClientMonitor (ClCCMonitor *&ao_pMonitor);

      //## Operation: GetSendQueue%881225505
      //	.
      //	Devuelve un puntero a la cola del Sender.
      //	.
      ClCCSendQ* GetSendQueue ();

      //## Operation: GetReceiverQueue%881685995
      //	.
      //	Devuelve un puntero a la cola del Receiver.
      //	.
      ClCCReceiverQ* GetReceiverQueue ();

      //## Operation: GetService%881257609; C++
      //	.
      //	Recibe como argumento el identificador de un servicio, y devuelve un puntero
      //	a ese servicio.
      //	.
      m4return_t GetService (m4handle_t ai_hServiceId, ClCCService *&ao_pService);

      //## Operation: GetRequest%881742137; C++
      //	.
      //	Recibe como argumento el identificador de una request, y devuelve un puntero
      //	a la misma.
      //	.
      m4return_t GetRequest (m4handle_t ai_hRequestId, ClCCRequest *&ao_pRequest);

      //## Operation: GetSession%889785669; C++
      //	.
      //	Recibe como argumento el identificador de un servicio, y devuelve un puntero
      //	a ese servicio.
      //	.
      m4return_t GetSession (m4handle_t ai_hSessionId, ClCCSession *&ao_pSession);

      //## Operation: AddService%881686000; C++
      //	.
      //	Inserta una entrada en el mapa de los servicios abiertos.
      //	.
      m4return_t AddService (m4handle_t ai_hServiceId, ClCCService* ai_pService);

      //## Operation: AddRequest%881742135; C++
      //	.
      //	Inserta una entrada en el mapa de las request pendientes..
      //	.
      m4return_t AddRequest (m4handle_t ai_hRequestId, ClCCRequest* ai_pRequest);

      //## Operation: AddSession%882869291; C++
      m4return_t AddSession (m4handle_t ai_hSessionId, ClCCSession *ai_pSession);

      //## Operation: RemoveService%881686001; C++
      //	.
      //	Borra la entrada correspondiente en el mapa de los servicios abiertos.
      //	.
      m4return_t RemoveService (m4handle_t ai_hServiceId);

      //## Operation: RemoveRequest%881742136; C++
      //	.
      //	Borra la entrada correspondiente en el mapa de las request pendientes.
      //	.
      m4return_t RemoveRequest (m4handle_t ai_hRequestId);

      //## Operation: RemoveSession%884854703; C++
      m4return_t RemoveSession (m4handle_t ai_hSessionId);

      //## Operation: CancelRequest%879876511; C++
      //	.
      //	Este método está orientado a poder eliminar una petición en el servidor si
      //	todavía no está ejecutada del todo. Cuando se hace una llamada a esta
      //	petición se le pasa un handle de petición. Este me indica cual es la
      //	petición que tengo que cancelar.
      //
      //	Mando al servidor la cancelación y me puede devolver dos posibles resultados:
      //
      //	Si me devuleve exito quiere decir que se ha cancelado. En este caso
      //	desbloqueo los posibles bloqueos existentes.
      //
      //	Si me devuelve fracaso quiere decir que no se ha podido cancelar porque
      //	probablemente esté ya en curso la petición.
      //
      //	FFS.- Este tema está muy verde, a falta de definir un poco más la
      //	arquitectura cliente-servidor.
      //	.
      m4return_t CancelRequest (m4handle_t ai_hRID);

      //## Operation: InitTSAP%885835377
      //	Initializes the TSAP library.
      m4return_t InitTSAP ();

      //## Operation: AddConnection%890042318; C++
      m4return_t AddConnection (m4handle_t ai_hConnectionId, ClCCConnection *ai_pConnection);

      //## Operation: RemoveConnection%890042319; C++
      m4return_t RemoveConnection (m4handle_t ai_hConnectionId);

      //## Operation: GetRequestNumber%950715095; C++
      m4return_t GetRequestNumber (m4handle_t &ao_hRequestNumber);

      //## Operation: GetCommsType%963558168
      static m4int_t GetCommsType ();

      //## Operation: SetCommsType%972486059; C++
      m4return_t SetCommsType (m4int_t ai_iCommsType);

    // Data Members for Class Attributes

      //## Attribute: m_IDSite%346AFA270135
      //	.
      //	Identificador del puesto
      //	.
      //## begin ClCCMonitor::m_IDSite%346AFA270135.attr preserve=no  public: m4int16_t {UA} 
      m4int16_t m_IDSite;
      //## end ClCCMonitor::m_IDSite%346AFA270135.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%348DC2DC0186
      //## Role: ClCCMonitor::m_poDistributor%348DC2DD012D
      //## begin ClCCMonitor::m_poDistributor%348DC2DD012D.role preserve=no  public: ClCCDistributor {1 -> 1RFHAN}
      ClCCDistributor *m_poDistributor;
      //## end ClCCMonitor::m_poDistributor%348DC2DD012D.role

      //## Association: M4AppClient::<unnamed>%3447382F00CD
      //## Role: ClCCMonitor::m_poReceiver%3447382F0344
      //## begin ClCCMonitor::m_poReceiver%3447382F0344.role preserve=no  public: ClCCReceiver {1 -> 1RFHAN}
      ClCCReceiver *m_poReceiver;
      //## end ClCCMonitor::m_poReceiver%3447382F0344.role

      //## Association: M4AppClient::<unnamed>%348D75580218
      //## Role: ClCCMonitor::m_poReceiverQ%348D755902EB
      //## begin ClCCMonitor::m_poReceiverQ%348D755902EB.role preserve=no  public: ClCCReceiverQ { -> RHAN}
      ClCCReceiverQ *m_poReceiverQ;
      //## end ClCCMonitor::m_poReceiverQ%348D755902EB.role

      //## Association: M4AppClient::<unnamed>%3447383500B8
      //## Role: ClCCMonitor::m_poSender%34473835036B
      //## begin ClCCMonitor::m_poSender%34473835036B.role preserve=no  public: ClCCSender {1 -> 1RFHAN}
      ClCCSender *m_poSender;
      //## end ClCCMonitor::m_poSender%34473835036B.role

      //## Association: M4AppClient::<unnamed>%34829D380232
      //## Role: ClCCMonitor::m_poSendQ%34829D390337
      //## begin ClCCMonitor::m_poSendQ%34829D390337.role preserve=no  public: ClCCSendQ { -> RHAN}
      ClCCSendQ *m_poSendQ;
      //## end ClCCMonitor::m_poSendQ%34829D390337.role

      //## Association: M4AppClient::<unnamed>%38AAD2870122
      //## Role: ClCCMonitor::m_NRequestMutex%38AAD288019C
      //## begin ClCCMonitor::m_NRequestMutex%38AAD288019C.role preserve=no  public: ClMutex { -> UHAN}
      ClMutex m_NRequestMutex;
      //## end ClCCMonitor::m_NRequestMutex%38AAD288019C.role

  protected:
    //## Constructors (specified)
      //## Operation: ClCCMonitor%879852433; C++
      //	.
      //	Constructor por defecto privado para que no pueda ser llamado
      //	.
      ClCCMonitor ();

    // Data Members for Class Attributes

      //## Attribute: m_iCommunicationType%396ED850003D
      //## begin ClCCMonitor::m_iCommunicationType%396ED850003D.attr preserve=no  protected: static m4int_t {UA} 0
      static m4int_t m_iCommunicationType;
      //## end ClCCMonitor::m_iCommunicationType%396ED850003D.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3447387003B6
      //## Role: ClCCMonitor::m_dicRequests%344738710227
      //## Qualifier: hRequest%344738890394; m4handle_t
      //## begin ClCCMonitor::m_dicRequests%344738710227.role preserve=no  protected: ClCCRequest {1 -> 1RFHAN}
      map<m4handle_t, ClCCRequest *, less<m4handle_t> > m_dicRequests;
      //## end ClCCMonitor::m_dicRequests%344738710227.role

      //## Association: M4AppClient::<unnamed>%348D96A50085
      //## Role: ClCCMonitor::m_dicServices%348D96A6029A
      //## Qualifier: hServ%348D96AE0269; m4handle_t
      //## begin ClCCMonitor::m_dicServices%348D96A6029A.role preserve=no  protected: ClCCService {1 -> 1RFHAN}
      map<m4handle_t, ClCCService *, less<m4handle_t> > m_dicServices;
      //## end ClCCMonitor::m_dicServices%348D96A6029A.role

      //## Association: M4AppClient::<unnamed>%35092005001E
      //## Role: ClCCMonitor::m_dicSessions%3509200502FD
      //## Qualifier: hSession%3509248801A5; m4handle_t
      //## begin ClCCMonitor::m_dicSessions%3509200502FD.role preserve=no  protected: ClCCSession {1 -> 1RFHAN}
      map<m4handle_t, ClCCSession *, less<m4handle_t> > m_dicSessions;
      //## end ClCCMonitor::m_dicSessions%3509200502FD.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_TSAPIsReady%34CCC324001B
      //	Displays the TSAP status. If the TSAP library is ok the member displays true.
      //## begin ClCCMonitor::m_TSAPIsReady%34CCC324001B.attr preserve=no  private: static m4bool_t {VA} M4_FALSE
      static m4bool_t m_TSAPIsReady;
      //## end ClCCMonitor::m_TSAPIsReady%34CCC324001B.attr

      //## Attribute: m_MutexDicSessions%3562ACB70021
      //## begin ClCCMonitor::m_MutexDicSessions%3562ACB70021.attr preserve=no  private: ClMutex {UA} 
      ClMutex m_MutexDicSessions;
      //## end ClCCMonitor::m_MutexDicSessions%3562ACB70021.attr

      //## Attribute: m_MutexDicRequests%3562AD1C01C1
      //## begin ClCCMonitor::m_MutexDicRequests%3562AD1C01C1.attr preserve=no  private: ClMutex {UA} 
      ClMutex m_MutexDicRequests;
      //## end ClCCMonitor::m_MutexDicRequests%3562AD1C01C1.attr

      //## Attribute: m_MutexDicServices%3562AD3F0352
      //## begin ClCCMonitor::m_MutexDicServices%3562AD3F0352.attr preserve=no  private: ClMutex {UA} 
      ClMutex m_MutexDicServices;
      //## end ClCCMonitor::m_MutexDicServices%3562AD3F0352.attr

      //## Attribute: m_oMutexInitCSC%35ADB2B8001B
      //## begin ClCCMonitor::m_oMutexInitCSC%35ADB2B8001B.attr preserve=no  private: static ClMutex {UA} 
      static ClMutex m_oMutexInitCSC;
      //## end ClCCMonitor::m_oMutexInitCSC%35ADB2B8001B.attr

      //## Attribute: m_bFirstTimeCommInit%39F6FA4E03DC
      //## begin ClCCMonitor::m_bFirstTimeCommInit%39F6FA4E03DC.attr preserve=no  private: static m4bool_t {UA} M4_TRUE
      static m4bool_t m_bFirstTimeCommInit;
      //## end ClCCMonitor::m_bFirstTimeCommInit%39F6FA4E03DC.attr

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%348593E902C2
      //## Role: ClCCMonitor::m_pMonitor%348593F100D9
      //## begin ClCCMonitor::m_pMonitor%348593F100D9.role preserve=no  private: static ClCCMonitor { -> RHAN}
      static ClCCMonitor *m_pMonitor;
      //## end ClCCMonitor::m_pMonitor%348593F100D9.role

      //## Association: M4AppClient::<unnamed>%38AABD7A0065
      //## Role: ClCCMonitor::M4FirstRequestNumber%38AABD7B0138
      //## begin ClCCMonitor::M4FirstRequestNumber%38AABD7B0138.role preserve=no  private: static m4handle_t {1 -> 1UHANC}
      static const m4handle_t  M4FirstRequestNumber;
      //## end ClCCMonitor::M4FirstRequestNumber%38AABD7B0138.role

      //## Association: M4AppClient::<unnamed>%38AAD249015F
      //## Role: ClCCMonitor::m_hRequestNumber%38AAD24A01CE
      //## begin ClCCMonitor::m_hRequestNumber%38AAD24A01CE.role preserve=no  private: m4handle_t { -> UHAN}
      m4handle_t m_hRequestNumber;
      //## end ClCCMonitor::m_hRequestNumber%38AAD24A01CE.role

  private: //## implementation
	static m4int_t m_iNRefs;
};

// Class ClCCMonitor 

//## begin module%347958800323.epilog preserve=yes
//## end module%347958800323.epilog


#endif
