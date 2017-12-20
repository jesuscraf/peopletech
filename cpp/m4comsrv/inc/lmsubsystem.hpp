//## begin module%39376A8201ED.cm preserve=no
//## end module%39376A8201ED.cm

//## begin module%39376A8201ED.cp preserve=no
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
//## end module%39376A8201ED.cp

//## Module: lmsubsystem%39376A8201ED; Package specification
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Source file: F:\integration\m4comsrv\inc\lmsubsystem.hpp

#ifndef lmsubsystem_h
#define lmsubsystem_h 1

//## begin module%39376A8201ED.additionalIncludes preserve=no
//## end module%39376A8201ED.additionalIncludes

//## begin module%39376A8201ED.includes preserve=yes
//## end module%39376A8201ED.includes

// actionworker
#include <actionworker.hpp>
// iexecutable
#include <iexecutable.hpp>
// requestdic
#include <requestdic.hpp>
// LMStats
#include <lmstats.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// clssaction
#include <clssaction.hpp>
// xmlrequest
#include <xmlrequest.hpp>
// m4communication
#include <m4communication.hpp>

class ClLocalMonitorInterface;
class ClConnection;

//## begin module%39376A8201ED.declarations preserve=no
//## end module%39376A8201ED.declarations

//## begin module%39376A8201ED.additionalDeclarations preserve=yes
typedef map < m4objid_t , m4objid_t * , less < m4objid_t > > ClRequestSessionMap ;
//## end module%39376A8201ED.additionalDeclarations


//## Class: ClLocalMonitorSS%349286C9016D
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35168E02008D;ClLMVisitor { -> F}
//## Uses: <unnamed>%35168E12032F;ClLMStats { -> F}
//## Uses: <unnamed>%39376F73019C;ClConnection { -> F}

class ClLocalMonitorSS : public ClSSImplementation, //## Inherits: <unnamed>%3961B4AE014A
            	public ClExecutableInterface  //## Inherits: <unnamed>%3961B4E0026E
{
  public:
    //## begin ClLocalMonitorSS::eLocalMonitorCommands%37CD44E202E7.preface preserve=yes
    //## end ClLocalMonitorSS::eLocalMonitorCommands%37CD44E202E7.preface

    //## Class: eLocalMonitorCommands%37CD44E202E7
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems::inc%38D787F6035B
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { GARBAGE_COLLECTION = ClSSImplementation::TOTAL_COMMANDS , ADD_COMMUNICATION_WORKER , REMOVE_COMMUNICATION_WORKER  , TOTAL_COMMANDS} eLocalMonitorCommands;

    //## begin ClLocalMonitorSS::eLocalMonitorCommands%37CD44E202E7.postscript preserve=yes
    //## end ClLocalMonitorSS::eLocalMonitorCommands%37CD44E202E7.postscript

    //## begin ClLocalMonitorSS::eLMActions%393773D90259.preface preserve=yes
    //## end ClLocalMonitorSS::eLMActions%393773D90259.preface

    //## Class: eLMActions%393773D90259
    //## Category: SSCCSubsystems%38D783280244
    //## Subsystem: ssccsubsystems%38D7879E0278
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {POST_ACCEPT_PDU_PROTOCOL_CONNECTIONS_ID = 0 ,POST_READ_PDU_PROTOCOL_CONNECTION ,  POST_ACCEPT_ADMIN_CONNECTIONS ,POST_WRITE_PDU_PROTOCOL_CONNECTION ,  RETURN_REQUEST , SEND_BUFFER_AS_CLIENT_ACTION_ID , CLOSE_CONNECTION_ACTION_ID  , OPEN_CONNECTION_ACTION_ID ,REGISTER_REQUEST_ACTION_ID , DEREGISTER_REQUEST_ACTION_ID , CANCEL_USER_REQUEST_ACTION_ID , PROCESS_REQUEST_ACTION_ID , REMOVE_CONNECTION_ID , REPLACE_REQUEST_ACTION_ID } eLMActions;

    //## begin ClLocalMonitorSS::eLMActions%393773D90259.postscript preserve=yes
    //## end ClLocalMonitorSS::eLMActions%393773D90259.postscript

    //## Constructors (specified)
      //## Operation: ClLocalMonitorSS%882177093
      ClLocalMonitorSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClLocalMonitorSS%882177094
      ~ClLocalMonitorSS ();


    //## Other Operations (specified)
      //## Operation: SetLocalMonitor%882177095
      void SetLocalMonitor (ClLocalMonitor *ai_poLocalMonitor);

      //## Operation: GetLocalMonitor%882177096
      ClLocalMonitorInterface * GetLocalMonitor ();

      //## Operation: Accept%884777562
      void Accept (ClLMVisitor &ai_oVisitor);

      //## Operation: Synchronize%884942700
      m4return_t Synchronize ();

      //## Operation: Initialize%885806477
      m4return_t Initialize ();

      //## Operation: ShutDown%885806522
      m4return_t ShutDown ();

      //## Operation: ExecuteCommand%901709449
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandsId%901709450
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901709451
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GarbageCollection%936177782
      void GarbageCollection ();

      //## Operation: Start%954935299
      //	Despues de la iniciaización se procede al aranque en si, este arranque puede
      //	ser crear los threads asociados o por ejemplo establecer vínculos con los
      //	subsistemas a los que se ha de suscribir. Todo esto se realiza en la llamada
      //	a Stop
      m4return_t Start ();

      //## Operation: CreateAction%955440320
      m4return_t CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction);

      //## Operation: FindService%955610151
      ClSSImplementation * FindService (ClRequest *ai_poRequest);

      //## Operation: ProcessAction%957943668
      m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction);

      //## Operation: GetElement%958560329
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

      //## Operation: BuildRequestFromPDU%959932988
      m4return_t BuildRequestFromPDU (ClConnInstance *ai_poConnection, ClPDU *ai_poPDU, ClRequest *&ao_poRequest, m4uint64_t ai_uiStartRecordTime = 0);

      //## Operation: ConnectionReaded%959932991
      m4return_t ConnectionReaded (ClParamList *ai_poParamList);

      //## Operation: ConnectionAccepted%959932992
      m4return_t ConnectionAccepted (ClParamList *ai_poParamList);

      //## Operation: ExecuteAction%959932993
      m4return_t ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList);

      //## Operation: AdminConnectionAccepted%959932994
      m4return_t AdminConnectionAccepted (ClParamList *ai_poParamList);

      //## Operation: SendRequest%959932995
      m4return_t SendRequest (ClRequest *ai_poRequest);

      //## Operation: RemoveRequest%959948239
      m4return_t RemoveRequest (ClParamList *ai_poParamList);

      //## Operation: _CreateRequest%962700383
      ClRequest * _CreateRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, ClConnInstance *ai_oConnection, ClPDU *ai_poInputPDU, m4uint64_t ai_uiStartRecordTime);

      //## Operation: OpenConnection%962784427
      m4return_t OpenConnection (m4string_t ai_szHost, m4uint32_t ai_iPort, m4objid_t &ao_iConnectionId);

      //## Operation: SendBuffer%962784428
      m4return_t SendBuffer (m4objid_t ai_iConnectionId, m4pchar_t ai_szBuffer, m4uint32_t ai_iSize);

      //## Operation: CloseConnection%962784431
      m4return_t CloseConnection (m4uint32_t &ao_iConnectionId);

      //## Operation: RemoveBuffer%962784432
      m4return_t RemoveBuffer (ClParamList *ai_pParams);

      //## Operation: RegisterRequest%969874233
      m4return_t RegisterRequest (ClRequest *ai_poRequest);

      //## Operation: CancelRequest%970139114
      m4return_t CancelRequest (m4objid_t ai_iRequestID , m4objid_t ai_iSessionId = 0);

      //## Operation: RemoveRequest%970561855
      m4return_t RemoveRequest (m4uint32_t ai_lIdConnection, m4objid_t ai_lIdRequest, m4millisec_t ai_dWrittenTime);

      //## Operation: RemoveBuffer%970572582
      m4return_t RemoveBuffer (m4uint32_t ai_lConnectionId, m4objid_t ai_lPetitionId);

      //## Operation: AddCommWorker%971889048
      m4return_t AddCommWorker ();

      //## Operation: ProcessPDU%975318793
      m4return_t ProcessPDU (ClPDU *ai_poPDU, ClConnInstance *ai_poConnection, ClParamList* ai_poParamList);

      //## Operation: Stop%977242948
      //	Cuando se inicaia el proceso de ShutDown se notifica a todos los subsistemas
      //	mediante este método. Como parámetro se pasa en segundos el timepo que queda
      //	hasta el shutdown definitivo y cada subsistema debe de ir haciendo en los
      //	pasos previos según corresponda las aaciones pertinentes para dejar todo
      //	consistente antes de ese momento o al menos en la última llamada (tiempo = 0)
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: IsReadyToClose%977242949
      //	Durante el proceso de ShutDown, este métodose llamará para todos los
      //	subsistemas con objeto de verificar el momento en el que sea posible pasar a
      //	la destrucción de todos los subsistemas.
      //	Aquel subsistema que precise de una deshabitación previa a la destucción,
      //	como puede ser la finalización de los threads asociados, hará esto en un
      //	paso previo (Stop) y a la llamada de este método, devolverá true o false
      //	dependiendo de si el thread a finalizado ya o no, es decr dependiendo de si
      //	el subsistema está listo o no para ser destruido.
      m4bool_t IsReadyToClose ();

      //## Operation: StopReading%977301111
      m4return_t StopReading ();

      //## Operation: RemoveConnection%993726127
      m4return_t RemoveConnection (m4uint32_t ai_idConnection);

      //## Operation: REap%994350081
      void REap ();

      //## Operation: ReplaceRequest%994350082
      m4return_t ReplaceRequest (ClRequest *ai_poOldRequest, ClRequest *ai_poNewRequest);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%3492875E02DA
      //## Role: ClLocalMonitorSS::m_poLocalMonitor%3492875F01F5
      //## begin ClLocalMonitorSS::m_poLocalMonitor%3492875F01F5.role preserve=no  public: ClLocalMonitorInterface { -> RFHAN}
      ClLocalMonitorInterface *m_poLocalMonitor;
      //## end ClLocalMonitorSS::m_poLocalMonitor%3492875F01F5.role

  public:

	  m4return_t RemoveCommWorker () ;

  protected:
    // Data Members for Associations

      //## Association: SSCCSubsystems::<unnamed>%393771B90134
      //## Role: ClLocalMonitorSS::m_poCommunication%393771B903E8
      //## begin ClLocalMonitorSS::m_poCommunication%393771B903E8.role preserve=no  protected: ClCommunication { -> RHAN}
      ClCommunication *m_poCommunication;
      //## end ClLocalMonitorSS::m_poCommunication%393771B903E8.role

      //## Association: SSCCSubsystems::<unnamed>%39D9C4E503DA
      //## Role: ClLocalMonitorSS::m_oXMLRequestDic%39D9C4E60237
      //## begin ClLocalMonitorSS::m_oXMLRequestDic%39D9C4E60237.role preserve=no  protected: ClXMLRequestDic { -> VHAN}
      ClXMLRequestDic m_oXMLRequestDic;
      //## end ClLocalMonitorSS::m_oXMLRequestDic%39D9C4E60237.role

      //## Association: Subsystem::<unnamed>%37CCF18A003F
      //## Role: ClLocalMonitorSS::m_poGBVisitor%37CCF18A037F
      //## begin ClLocalMonitorSS::m_poGBVisitor%37CCF18A037F.role preserve=no  protected: ClLMVisitor { -> RHAN}
      ClLMVisitor *m_poGBVisitor;
      //## end ClLocalMonitorSS::m_poGBVisitor%37CCF18A037F.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iBasePort%3A3F9B7703D6
      //## begin ClLocalMonitorSS::m_iBasePort%3A3F9B7703D6.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iBasePort;
      //## end ClLocalMonitorSS::m_iBasePort%3A3F9B7703D6.attr

    // Data Members for Associations

      //## Association: SSCCSubsystems::<unnamed>%39379F1601B5
      //## Role: ClLocalMonitorSS::m_oRequestDic%39379F1700A8
      //## begin ClLocalMonitorSS::m_oRequestDic%39379F1700A8.role preserve=no  private: ClRequestDic { -> VHAN}
      ClRequestDic m_oRequestDic;

	  ClMutex oMutexMap ;

	  ClRequestSessionMap m_RequestSessionMap ;
      //## end ClLocalMonitorSS::m_oRequestDic%39379F1700A8.role

      //## Association: SSCCSubsystems::<unnamed>%3A222EB70234
      //## Role: ClLocalMonitorSS::m_oWorkerList%3A222EB800CD
      //## begin ClLocalMonitorSS::m_oWorkerList%3A222EB800CD.role preserve=no  private: ClActionWorkerList { -> VHAN}
      ClActionWorkerList m_oWorkerList;
      //## end ClLocalMonitorSS::m_oWorkerList%3A222EB800CD.role

      //## Association: SSCCSubsystems::<unnamed>%3A2231C30073
      //## Role: ClLocalMonitorSS::m_oActionsQ%3A2231C4033B
      //## begin ClLocalMonitorSS::m_oActionsQ%3A2231C4033B.role preserve=no  private: ClActionsQ { -> VHAN}
      ClActionsQ m_oActionsQ;
      //## end ClLocalMonitorSS::m_oActionsQ%3A2231C4033B.role

      //## Association: SSCCSubsystems::<unnamed>%3A24F9BB003C
      //## Role: ClLocalMonitorSS::m_oActionsRecycleBin%3A24F9BB0263
      //## begin ClLocalMonitorSS::m_oActionsRecycleBin%3A24F9BB0263.role preserve=no  private: ClActionsQ { -> VHAN}
      ClActionsQ m_oActionsRecycleBin;
      //## end ClLocalMonitorSS::m_oActionsRecycleBin%3A24F9BB0263.role

  private: //## implementation
};

//## begin ClProcessRequestAction%38F21D500218.preface preserve=yes
//## end ClProcessRequestAction%38F21D500218.preface

//## Class: ClProcessRequestAction%38F21D500218
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClProcessRequestAction : public ClSSAction  //## Inherits: <unnamed>%38F21DD802FA
{
  //## begin ClProcessRequestAction%38F21D500218.initialDeclarations preserve=yes
  //## end ClProcessRequestAction%38F21D500218.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClProcessRequestAction%975318792
      ClProcessRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%975318790
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClProcessRequestAction%38F21D500218.public preserve=yes
      //## end ClProcessRequestAction%38F21D500218.public

  protected:
    // Additional Protected Declarations
      //## begin ClProcessRequestAction%38F21D500218.protected preserve=yes
      //## end ClProcessRequestAction%38F21D500218.protected

  private:
    // Additional Private Declarations
      //## begin ClProcessRequestAction%38F21D500218.private preserve=yes
      //## end ClProcessRequestAction%38F21D500218.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProcessRequestAction%38F21D500218.implementation preserve=yes
      //## end ClProcessRequestAction%38F21D500218.implementation

};

//## begin ClProcessRequestAction%38F21D500218.postscript preserve=yes
//## end ClProcessRequestAction%38F21D500218.postscript

//## begin ClReturnRequestAction%39376ABB0040.preface preserve=yes
//## end ClReturnRequestAction%39376ABB0040.preface

//## Class: ClReturnRequestAction%39376ABB0040
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39376AE201C3;ClLocalMonitorSS { -> }

class ClReturnRequestAction : public ClSSAction  //## Inherits: <unnamed>%39C73FBB0241
{
  //## begin ClReturnRequestAction%39376ABB0040.initialDeclarations preserve=yes
  //## end ClReturnRequestAction%39376ABB0040.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClReturnRequestAction%959932989
      ClReturnRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%959932990
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClReturnRequestAction%39376ABB0040.public preserve=yes
      //## end ClReturnRequestAction%39376ABB0040.public

  protected:
    // Additional Protected Declarations
      //## begin ClReturnRequestAction%39376ABB0040.protected preserve=yes
      //## end ClReturnRequestAction%39376ABB0040.protected

  private:
    // Additional Private Declarations
      //## begin ClReturnRequestAction%39376ABB0040.private preserve=yes
      //## end ClReturnRequestAction%39376ABB0040.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClReturnRequestAction%39376ABB0040.implementation preserve=yes
      //## end ClReturnRequestAction%39376ABB0040.implementation

};

//## begin ClReturnRequestAction%39376ABB0040.postscript preserve=yes
//## end ClReturnRequestAction%39376ABB0040.postscript

//## begin ClSendBufferAsClientAction%3962E6B402A5.preface preserve=yes
//## end ClSendBufferAsClientAction%3962E6B402A5.preface

//## Class: ClSendBufferAsClientAction%3962E6B402A5
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3962E7490213;ClLocalMonitorSS { -> }

class ClSendBufferAsClientAction : public ClSSAction  //## Inherits: <unnamed>%39C73FB90220
{
  //## begin ClSendBufferAsClientAction%3962E6B402A5.initialDeclarations preserve=yes
  //## end ClSendBufferAsClientAction%3962E6B402A5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSendBufferAsClientAction%962784429
      ClSendBufferAsClientAction (ClSSImplementation *ai_poExecutive);

    // Additional Public Declarations
      //## begin ClSendBufferAsClientAction%3962E6B402A5.public preserve=yes
      //## end ClSendBufferAsClientAction%3962E6B402A5.public

  protected:

    //## Other Operations (specified)
      //## Operation: Execute%962784430
      m4return_t Execute ();

    // Additional Protected Declarations
      //## begin ClSendBufferAsClientAction%3962E6B402A5.protected preserve=yes
      //## end ClSendBufferAsClientAction%3962E6B402A5.protected

  private:
    // Additional Private Declarations
      //## begin ClSendBufferAsClientAction%3962E6B402A5.private preserve=yes
      //## end ClSendBufferAsClientAction%3962E6B402A5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSendBufferAsClientAction%3962E6B402A5.implementation preserve=yes
      //## end ClSendBufferAsClientAction%3962E6B402A5.implementation

};

//## begin ClSendBufferAsClientAction%3962E6B402A5.postscript preserve=yes
//## end ClSendBufferAsClientAction%3962E6B402A5.postscript

//## begin CloseConnectionAction%396313F60042.preface preserve=yes
//## end CloseConnectionAction%396313F60042.preface

//## Class: CloseConnectionAction%396313F60042
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CloseConnectionAction : public ClSSAction  //## Inherits: <unnamed>%39C73FB50383
{
  //## begin CloseConnectionAction%396313F60042.initialDeclarations preserve=yes
  //## end CloseConnectionAction%396313F60042.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CloseConnectionAction%962784434
      CloseConnectionAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%962784435
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin CloseConnectionAction%396313F60042.public preserve=yes
      //## end CloseConnectionAction%396313F60042.public

  protected:
    // Additional Protected Declarations
      //## begin CloseConnectionAction%396313F60042.protected preserve=yes
      //## end CloseConnectionAction%396313F60042.protected

  private:
    // Additional Private Declarations
      //## begin CloseConnectionAction%396313F60042.private preserve=yes
      //## end CloseConnectionAction%396313F60042.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CloseConnectionAction%396313F60042.implementation preserve=yes
      //## end CloseConnectionAction%396313F60042.implementation

};

//## begin CloseConnectionAction%396313F60042.postscript preserve=yes
//## end CloseConnectionAction%396313F60042.postscript

//## begin ClOpenConnectionAction%39C73EF602DE.preface preserve=yes
//## end ClOpenConnectionAction%39C73EF602DE.preface

//## Class: ClOpenConnectionAction%39C73EF602DE
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClOpenConnectionAction : public ClSSAction  //## Inherits: <unnamed>%39C73FDB0070
{
  //## begin ClOpenConnectionAction%39C73EF602DE.initialDeclarations preserve=yes
  //## end ClOpenConnectionAction%39C73EF602DE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClOpenConnectionAction%969373407
      ClOpenConnectionAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%969373408
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClOpenConnectionAction%39C73EF602DE.public preserve=yes
      //## end ClOpenConnectionAction%39C73EF602DE.public

  protected:
    // Additional Protected Declarations
      //## begin ClOpenConnectionAction%39C73EF602DE.protected preserve=yes
      //## end ClOpenConnectionAction%39C73EF602DE.protected

  private:
    // Additional Private Declarations
      //## begin ClOpenConnectionAction%39C73EF602DE.private preserve=yes
      //## end ClOpenConnectionAction%39C73EF602DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClOpenConnectionAction%39C73EF602DE.implementation preserve=yes
      //## end ClOpenConnectionAction%39C73EF602DE.implementation

};

//## begin ClOpenConnectionAction%39C73EF602DE.postscript preserve=yes
//## end ClOpenConnectionAction%39C73EF602DE.postscript

//## begin ClRegisterRequestAction%38F589BF01EF.preface preserve=yes
//## end ClRegisterRequestAction%38F589BF01EF.preface

//## Class: ClRegisterRequestAction%38F589BF01EF
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRegisterRequestAction : public ClSSAction  //## Inherits: <unnamed>%38F589FC0323
{
  //## begin ClRegisterRequestAction%38F589BF01EF.initialDeclarations preserve=yes
  //## end ClRegisterRequestAction%38F589BF01EF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRegisterRequestAction%955610147
      ClRegisterRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955610148
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClRegisterRequestAction%38F589BF01EF.public preserve=yes
      //## end ClRegisterRequestAction%38F589BF01EF.public

  protected:
    // Additional Protected Declarations
      //## begin ClRegisterRequestAction%38F589BF01EF.protected preserve=yes
      //## end ClRegisterRequestAction%38F589BF01EF.protected

  private:
    // Additional Private Declarations
      //## begin ClRegisterRequestAction%38F589BF01EF.private preserve=yes
      //## end ClRegisterRequestAction%38F589BF01EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRegisterRequestAction%38F589BF01EF.implementation preserve=yes
      //## end ClRegisterRequestAction%38F589BF01EF.implementation

};

//## begin ClRegisterRequestAction%38F589BF01EF.postscript preserve=yes
//## end ClRegisterRequestAction%38F589BF01EF.postscript

//## begin ClDeRegisterRequestAction%38F589EC012B.preface preserve=yes
//## end ClDeRegisterRequestAction%38F589EC012B.preface

//## Class: ClDeRegisterRequestAction%38F589EC012B
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClDeRegisterRequestAction : public ClSSAction  //## Inherits: <unnamed>%38F58A150076
{
  //## begin ClDeRegisterRequestAction%38F589EC012B.initialDeclarations preserve=yes
  //## end ClDeRegisterRequestAction%38F589EC012B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDeRegisterRequestAction%955610149
      ClDeRegisterRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%955610150
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClDeRegisterRequestAction%38F589EC012B.public preserve=yes
      //## end ClDeRegisterRequestAction%38F589EC012B.public

  protected:
    // Additional Protected Declarations
      //## begin ClDeRegisterRequestAction%38F589EC012B.protected preserve=yes
      //## end ClDeRegisterRequestAction%38F589EC012B.protected

  private:
    // Additional Private Declarations
      //## begin ClDeRegisterRequestAction%38F589EC012B.private preserve=yes
      //## end ClDeRegisterRequestAction%38F589EC012B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDeRegisterRequestAction%38F589EC012B.implementation preserve=yes
      //## end ClDeRegisterRequestAction%38F589EC012B.implementation

};

//## begin ClDeRegisterRequestAction%38F589EC012B.postscript preserve=yes
//## end ClDeRegisterRequestAction%38F589EC012B.postscript

//## begin ClCancelUserRequestAction%39227A250254.preface preserve=yes
//## end ClCancelUserRequestAction%39227A250254.preface

//## Class: ClCancelUserRequestAction%39227A250254
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClCancelUserRequestAction : public ClSSAction  //## Inherits: <unnamed>%39227A32000E
{
  //## begin ClCancelUserRequestAction%39227A250254.initialDeclarations preserve=yes
  //## end ClCancelUserRequestAction%39227A250254.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCancelUserRequestAction%958560330
      ClCancelUserRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%958560331
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClCancelUserRequestAction%39227A250254.public preserve=yes
      //## end ClCancelUserRequestAction%39227A250254.public

  protected:
    // Additional Protected Declarations
      //## begin ClCancelUserRequestAction%39227A250254.protected preserve=yes
      //## end ClCancelUserRequestAction%39227A250254.protected

  private:
    // Additional Private Declarations
      //## begin ClCancelUserRequestAction%39227A250254.private preserve=yes
      //## end ClCancelUserRequestAction%39227A250254.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCancelUserRequestAction%39227A250254.implementation preserve=yes
      //## end ClCancelUserRequestAction%39227A250254.implementation

};

//## begin ClCancelUserRequestAction%39227A250254.postscript preserve=yes
//## end ClCancelUserRequestAction%39227A250254.postscript

//## begin ClRemoveRequestAction%3AF904540106.preface preserve=yes
//## end ClRemoveRequestAction%3AF904540106.preface

//## Class: ClRemoveRequestAction%3AF904540106
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRemoveRequestAction : public ClSSAction  //## Inherits: <unnamed>%3AF9046402E0
{
  //## begin ClRemoveRequestAction%3AF904540106.initialDeclarations preserve=yes
  //## end ClRemoveRequestAction%3AF904540106.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRemoveRequestAction%989398207
      ClRemoveRequestAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%989398208
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClRemoveRequestAction%3AF904540106.public preserve=yes
      //## end ClRemoveRequestAction%3AF904540106.public

  protected:
    // Additional Protected Declarations
      //## begin ClRemoveRequestAction%3AF904540106.protected preserve=yes
      //## end ClRemoveRequestAction%3AF904540106.protected

  private:
    // Additional Private Declarations
      //## begin ClRemoveRequestAction%3AF904540106.private preserve=yes
      //## end ClRemoveRequestAction%3AF904540106.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRemoveRequestAction%3AF904540106.implementation preserve=yes
      //## end ClRemoveRequestAction%3AF904540106.implementation

};

//## begin ClRemoveRequestAction%3AF904540106.postscript preserve=yes
//## end ClRemoveRequestAction%3AF904540106.postscript

//## begin ClRemoveConnectionAction%3B3B0E59022C.preface preserve=yes
//## end ClRemoveConnectionAction%3B3B0E59022C.preface

//## Class: ClRemoveConnectionAction%3B3B0E59022C
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClRemoveConnectionAction : public ClSSAction  //## Inherits: <unnamed>%3B3B0E670127
{
  //## begin ClRemoveConnectionAction%3B3B0E59022C.initialDeclarations preserve=yes
  //## end ClRemoveConnectionAction%3B3B0E59022C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClRemoveConnectionAction%993726128
      ClRemoveConnectionAction (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%993726129
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClRemoveConnectionAction%3B3B0E59022C.public preserve=yes
      //## end ClRemoveConnectionAction%3B3B0E59022C.public

  protected:
    // Additional Protected Declarations
      //## begin ClRemoveConnectionAction%3B3B0E59022C.protected preserve=yes
      //## end ClRemoveConnectionAction%3B3B0E59022C.protected

  private:
    // Additional Private Declarations
      //## begin ClRemoveConnectionAction%3B3B0E59022C.private preserve=yes
      //## end ClRemoveConnectionAction%3B3B0E59022C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClRemoveConnectionAction%3B3B0E59022C.implementation preserve=yes
      //## end ClRemoveConnectionAction%3B3B0E59022C.implementation

};

//## begin ClRemoveConnectionAction%3B3B0E59022C.postscript preserve=yes
//## end ClRemoveConnectionAction%3B3B0E59022C.postscript

//## begin ClReplaceRequest%3B4493480089.preface preserve=yes
//## end ClReplaceRequest%3B4493480089.preface

//## Class: ClReplaceRequest%3B4493480089
//## Category: SSCCSubsystems%38D783280244
//## Subsystem: ssccsubsystems::inc%38D787F6035B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClReplaceRequest : public ClSSAction  //## Inherits: <unnamed>%3B44935A03CE
{
  //## begin ClReplaceRequest%3B4493480089.initialDeclarations preserve=yes
  //## end ClReplaceRequest%3B4493480089.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClReplaceRequest%994350079
      ClReplaceRequest (ClSSImplementation *ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: Execute%994350080
      m4return_t Execute ();

    // Additional Public Declarations
      //## begin ClReplaceRequest%3B4493480089.public preserve=yes
      //## end ClReplaceRequest%3B4493480089.public

  protected:
    // Additional Protected Declarations
      //## begin ClReplaceRequest%3B4493480089.protected preserve=yes
      //## end ClReplaceRequest%3B4493480089.protected

  private:
    // Additional Private Declarations
      //## begin ClReplaceRequest%3B4493480089.private preserve=yes
      //## end ClReplaceRequest%3B4493480089.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClReplaceRequest%3B4493480089.implementation preserve=yes
      //## end ClReplaceRequest%3B4493480089.implementation

};

//## begin ClReplaceRequest%3B4493480089.postscript preserve=yes
//## end ClReplaceRequest%3B4493480089.postscript

// Class ClLocalMonitorSS 

// Class ClProcessRequestAction 

// Class ClReturnRequestAction 

// Class ClSendBufferAsClientAction 

// Class CloseConnectionAction 

// Class ClOpenConnectionAction 

// Class ClRegisterRequestAction 

// Class ClDeRegisterRequestAction 

// Class ClCancelUserRequestAction 

// Class ClRemoveRequestAction 

// Class ClRemoveConnectionAction 

// Class ClReplaceRequest 

//## begin module%39376A8201ED.epilog preserve=yes
//## end module%39376A8201ED.epilog


#endif
