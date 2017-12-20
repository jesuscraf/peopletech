//## begin module%34795AA7004A.cm preserve=no
//## end module%34795AA7004A.cm

//## begin module%34795AA7004A.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClReceiverC.hpp
//	 Project:           M4ComSrv
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
//	=========================================================
//	=====================
//## end module%34795AA7004A.cp

//## Module: clientreceiver%34795AA7004A; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: C:\m4server\m4appclient\inc\clientreceiver.hpp

#ifndef clientreceiver_h
#define clientreceiver_h 1

//## begin module%34795AA7004A.additionalIncludes preserve=no
//## end module%34795AA7004A.additionalIncludes

//## begin module%34795AA7004A.includes preserve=yes
// JAV Lo pongo porque Rose98 no va
#include <clientsession.hpp>
// clientconnectionhttp
#include <clientconnectionhttp.hpp>
//stl
#include "m4stl.hpp"
//## end module%34795AA7004A.includes

// clientbasicdef
#include <clientbasicdef.hpp>
// eventdisplayer
#include <eventdisplayer.hpp>
// pduherram
#include <pduherram.hpp>
// clientreceiverq
#include <clientreceiverq.hpp>
// CSUtil
#include <csutil.hpp>
// syncro
#include <syncro.hpp>
// tsap
#include <tsap.hpp>
// servicepdus
#include <servicepdus.hpp>
// m4eventhandlers
#include <m4eventhandlers.hpp>

class ClCCConnection;

//## begin module%34795AA7004A.declarations preserve=no
//## end module%34795AA7004A.declarations

//## begin module%34795AA7004A.additionalDeclarations preserve=yes
//## end module%34795AA7004A.additionalDeclarations


//## begin ClCCReceiver%3447381B00B1.preface preserve=yes
//## end ClCCReceiver%3447381B00B1.preface

//## Class: ClCCReceiver%3447381B00B1
//	.
//	El Receiver recoge datos de la red, los convierte en PDUs y los vuelca a una
//	cola.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E4A32E03B9;ClGetPDUTSAP { -> }
//## Uses: <unnamed>%350E54750387;ClReplyPDU { -> }
//## Uses: <unnamed>%35BC7216010A;map { -> }

class ClCCReceiver : public ClActiveClass  //## Inherits: <unnamed>%3479572203B6
{
  //## begin ClCCReceiver%3447381B00B1.initialDeclarations preserve=yes
  //## end ClCCReceiver%3447381B00B1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCReceiver%879959079
      //	.
      //	Recibe como parámetro un puntero a la cola sobre la que va a dejar las PDUs
      //	que vaya recibiendo.
      //	.
      ClCCReceiver (ClCCReceiverQ* ai_pReceiverQueue);

    //## Destructor (specified)
      //## Operation: ~ClCCReceiver%880965773
      ~ClCCReceiver ();


    //## Other Operations (specified)
      //## Operation: AddConnection%880965774
      //	.
      //	Añade la connection (y su TSAP correspondiente) al diccionario del receiver.
      //	Despues llama al AddTSAP para añadirla a la mascara de la select
      //	.
      m4return_t AddConnection (ClCCConnection* ai_pConnection);

      //## Operation: RemoveConnection%881084615
      //	.
      //	Elimina  la conexión del diccionario y llama a RemoveTSAP
      //	.
      m4return_t RemoveConnection (ClCCConnection* ai_pConnection);

      //## Operation: Run%879852434
      //	Lee del TSAP que tiene pendientes y los va realizando las operaciones
      //	pendientes de realizar.
      //
      //	Las lecturas se realizan en modo bloqueantes y trayendo todos los datos
      //	relacionados con la PDU que tiene que viajar como respuesta.
      void Run ();

      //## Operation: AddTSAP%922371775; C++
      //	Añade el TSAP que recibe como parámetro a la máscara del Select, incrementa
      //	el numero de tsap que tiene y  si es la primera se hace salir al receiver
      //	del wait en el que esta.
      m4return_t AddTSAP (ClTSAP *ai_pTSAP);

      //## Operation: RemoveTSAP%922371776; C++
      //	Elimina el TSAP de la máscara del Select.
      //	.
      m4return_t RemoveTSAP (ClTSAP *ai_pTSAP);

      //## Operation: FullRemoveConn%948876482; C++
      m4return_t FullRemoveConn (ClCCConnection* ai_pConnection);

      //## Operation: _FullRemoveTSAP%951384407; C++
      m4return_t _FullRemoveTSAP (ClCCConnection *ai_pConnection);

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3487C3CA03B4
      //## Role: ClCCReceiver::<m_pClCCReceiverQ>%3487C3CB0243
      //## begin ClCCReceiver::<m_pClCCReceiverQ>%3487C3CB0243.role preserve=no  public: ClCCReceiverQ { -> RHAN}
      ClCCReceiverQ *m_pClCCReceiverQ;
      //## end ClCCReceiver::<m_pClCCReceiverQ>%3487C3CB0243.role

      //## Association: M4AppClient::<unnamed>%36FA4739032F
      //## Role: ClCCReceiver::m_pMutex%36FA473A033A
      //## begin ClCCReceiver::m_pMutex%36FA473A033A.role preserve=no  public: ClMutex { -> 1..1RHAN}
      ClMutex *m_pMutex;
      //## end ClCCReceiver::m_pMutex%36FA473A033A.role

    // Additional Public Declarations
      //## begin ClCCReceiver%3447381B00B1.public preserve=yes
      //## end ClCCReceiver%3447381B00B1.public

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%34E49FFF031C
      //## Role: ClCCReceiver::m_pSemaphore%34E4A0000148
      //## begin ClCCReceiver::m_pSemaphore%34E4A0000148.role preserve=no  protected: ClSemaphore {1 -> 1..1RHAN}
      ClSemaphore *m_pSemaphore;
      //## end ClCCReceiver::m_pSemaphore%34E4A0000148.role

      //## Association: M4AppClient::<unnamed>%34E4A25603C8
      //## Role: ClCCReceiver::m_pTSAPSelect%34E4A25701F4
      //## begin ClCCReceiver::m_pTSAPSelect%34E4A25701F4.role preserve=no  protected: ClTSAPSelect {1 -> 1RHAN}
      ClTSAPSelect *m_pTSAPSelect;
      //## end ClCCReceiver::m_pTSAPSelect%34E4A25701F4.role

      //## Association: M4AppClient::<unnamed>%35BC5D7F0205
      //## Role: ClCCReceiver::m_dicTSAP%35BC5D80018F
      //## Qualifier: hTSAP%35BC5DF70186; m4handle_t
      //## begin ClCCReceiver::m_dicTSAP%35BC5D80018F.role preserve=no  protected: ClCCConnection {1 -> 1RFHAN}
      map<m4handle_t, ClCCConnection *, less<m4handle_t> > m_dicTSAP;
      //## end ClCCReceiver::m_dicTSAP%35BC5D80018F.role

    // Additional Protected Declarations
      //## begin ClCCReceiver%3447381B00B1.protected preserve=yes
      //## end ClCCReceiver%3447381B00B1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iNumeroTSAP%34950533018D
      //## begin ClCCReceiver::m_iNumeroTSAP%34950533018D.attr preserve=no  private: m4int16_t {UA} 0
      m4int16_t m_iNumeroTSAP;
      //## end ClCCReceiver::m_iNumeroTSAP%34950533018D.attr

      //## Attribute: m_oCondEmptyTSAP%3614B02001B5
      //## begin ClCCReceiver::m_oCondEmptyTSAP%3614B02001B5.attr preserve=no  private: ClCondition {UA} 
      ClCondition m_oCondEmptyTSAP;
      //## end ClCCReceiver::m_oCondEmptyTSAP%3614B02001B5.attr

    // Additional Private Declarations
      //## begin ClCCReceiver%3447381B00B1.private preserve=yes
      //## end ClCCReceiver%3447381B00B1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCReceiver%3447381B00B1.implementation preserve=yes
      //## end ClCCReceiver%3447381B00B1.implementation

};

//## begin ClCCReceiver%3447381B00B1.postscript preserve=yes
//## end ClCCReceiver%3447381B00B1.postscript

//## begin ClCCDistributor%348DBC810140.preface preserve=yes
//## end ClCCDistributor%348DBC810140.preface

//## Class: ClCCDistributor%348DBC810140
//	.
//	El distributor recoge PDUs de la cola del receiver y se encarga de hacerlas
//	llegar a la request correspondiente.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37985B830013;ClServerEventDisplayer { -> }
//## Uses: <unnamed>%380F2D3600A7;ClLogBasedEventHandler { -> }

class ClCCDistributor : public ClActiveClass  //## Inherits: <unnamed>%348DBCBE02EC
{
  //## begin ClCCDistributor%348DBC810140.initialDeclarations preserve=yes
  //## end ClCCDistributor%348DBC810140.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCDistributor%881686003
      //	.
      //	Recibe como parámetro un puntero a la cola de la que debe ir recogiendo PDUs.
      //	.
      ClCCDistributor (ClCCReceiverQ* ai_pReceiverQueue);

    //## Destructor (specified)
      //## Operation: ~ClCCDistributor%881686004
      ~ClCCDistributor ();


    //## Other Operations (specified)
      //## Operation: GetPDUFromQueue%881686005
      //	.
      //	Recupera una PDU de la cola.
      //	.
      ClPDU* GetPDUFromQueue ();

      //## Operation: Run%881686006
      void Run ();

      //## Operation: ReturnRequest%881742134
      m4return_t ReturnRequest (ClPDU* ai_pPDU);

      //## Operation: SetServerEventFunction%929691987; C++
      m4return_t SetServerEventFunction (ClServerEventFunction ai_EventFunction);

      //## Operation: GetServerEventFunction%941097607; C++
      m4return_t GetServerEventFunction (ClServerEventFunction &ao_EventFunction);

      //## Operation: GetEvent%948366550; C++
      m4return_t GetEvent (ClPDU *ai_pPDU);

    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%348DBCED0308
      //## Role: ClCCDistributor::<m_pClCCReceiverQ>%348DBCEF03A1
      //## begin ClCCDistributor::<m_pClCCReceiverQ>%348DBCEF03A1.role preserve=no  public: ClCCReceiverQ { -> RHAN}
      ClCCReceiverQ *m_pClCCReceiverQ;
      //## end ClCCDistributor::<m_pClCCReceiverQ>%348DBCEF03A1.role

    // Additional Public Declarations
      //## begin ClCCDistributor%348DBC810140.public preserve=yes
      //## end ClCCDistributor%348DBC810140.public

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%3769F6680086
      //## Role: ClCCDistributor::m_pEventFunction%3769F66802FD
      //## begin ClCCDistributor::m_pEventFunction%3769F66802FD.role preserve=no  protected: ClServerEventFunction {1 -> 1UHAN}
      ClServerEventFunction m_pEventFunction;
      //## end ClCCDistributor::m_pEventFunction%3769F66802FD.role

    // Additional Protected Declarations
      //## begin ClCCDistributor%348DBC810140.protected preserve=yes
      //## end ClCCDistributor%348DBC810140.protected

  private:
    // Additional Private Declarations
      //## begin ClCCDistributor%348DBC810140.private preserve=yes
	  m4uint32_t m_iPDUCount;
      //## end ClCCDistributor%348DBC810140.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCDistributor%348DBC810140.implementation preserve=yes
      //## end ClCCDistributor%348DBC810140.implementation

};

//## begin ClCCDistributor%348DBC810140.postscript preserve=yes
//## end ClCCDistributor%348DBC810140.postscript

// Class ClCCReceiver 

// Class ClCCDistributor 

//## begin module%34795AA7004A.epilog preserve=yes
//## end module%34795AA7004A.epilog


#endif
