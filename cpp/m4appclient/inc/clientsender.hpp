//## begin module%34795A4000F6.cm preserve=no
//## end module%34795A4000F6.cm

//## begin module%34795A4000F6.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClSenderC.hpp
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
//## end module%34795A4000F6.cp

//## Module: clientsender%34795A4000F6; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\clientsender.hpp

#ifndef clientsender_h
#define clientsender_h 1

//## begin module%34795A4000F6.additionalIncludes preserve=no
//## end module%34795A4000F6.additionalIncludes

//## begin module%34795A4000F6.includes preserve=yes
//	#include "inc/consoletools.hpp"
//## end module%34795A4000F6.includes

// clientconnection
#include <clientconnection.hpp>
// clientsenderq
#include <clientsenderq.hpp>
// clientsession
#include <clientsession.hpp>
// CSUtil
#include <csutil.hpp>
// tsap
#include <tsap.hpp>
//## begin module%34795A4000F6.declarations preserve=no
//## end module%34795A4000F6.declarations

//## begin module%34795A4000F6.additionalDeclarations preserve=yes
//## end module%34795A4000F6.additionalDeclarations


//## begin ClCCSender%344737DF03AD.preface preserve=yes
//## end ClCCSender%344737DF03AD.preface

//## Class: ClCCSender%344737DF03AD
//	.
//	El sender recoge peticiones de una cola y las va volcando a la red.
//	Las cifra y las vuelca colocando la marca de tiempo.
//	.
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34E48A85009C;ClTSAP { -> }
//## Uses: <unnamed>%34E49B7300DA;ClCCSession { -> }
//## Uses: <unnamed>%38ABD657006C;ClCCConnection { -> }

class ClCCSender : public ClActiveClass  //## Inherits: <unnamed>%3479573302F2
{
  //## begin ClCCSender%344737DF03AD.initialDeclarations preserve=yes
  //## end ClCCSender%344737DF03AD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCCSender%880446326
      //	.
      //	Recibe como parámetro un puntero a la cola de la que irá leyendo las
      //	peticiones que tiene que enviar.
      //	.
      ClCCSender (ClCCSendQ* ai_pRequestQueue);

    //## Destructor (specified)
      //## Operation: ~ClCCSender%880368623
      ~ClCCSender ();


    //## Other Operations (specified)
      //## Operation: Run%879852436
      //	.
      //	Va recogiendo request de la cola y envia las PDUs al servidor.
      //	.
      void Run ();

      //## Operation: SendPDU%880368624
      //	.
      //	Este método se invoca desde el método Run() para enviar una PDU al servidor,
      //	una vez recogida de la cola. Recibe como parámetros la PDU que debe enviar y
      //	el TSAP.
      //	.
      m4return_t SendPDU (ClCCRequest *ai_pRequest, ClTSAP* ai_pTSAP);

    // Additional Public Declarations
      //## begin ClCCSender%344737DF03AD.public preserve=yes
      //## end ClCCSender%344737DF03AD.public

  protected:
    // Data Members for Associations

      //## Association: M4AppClient::<unnamed>%34829D8E01DB
      //## Role: ClCCSender::m_pSenderQ%34829D8F0146
      //## begin ClCCSender::m_pSenderQ%34829D8F0146.role preserve=no  protected: ClCCSendQ {1 -> 1RHAN}
      ClCCSendQ *m_pSenderQ;
      //## end ClCCSender::m_pSenderQ%34829D8F0146.role

    // Additional Protected Declarations
      //## begin ClCCSender%344737DF03AD.protected preserve=yes
      //## end ClCCSender%344737DF03AD.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetRequestFromQueue%880368622
      //	.
      //	Cada vez que un cliente solicita el envío de una petición, el CSC almacena
      //	dicha petición en una cola de envío. Este método obtiene una petición de esa
      //	cola para enviarla al servidor.
      //	.
      ClCCRequest* GetRequestFromQueue ();

    // Data Members for Class Attributes

      //## Attribute: m_iCount%3479B2180373
      //## begin ClCCSender::m_iCount%3479B2180373.attr preserve=no  private: m4int16_t {UA} 0
      m4int16_t m_iCount;
      //## end ClCCSender::m_iCount%3479B2180373.attr

      //## Attribute: m_iRequestCount%3479B4C601D5
      //## begin ClCCSender::m_iRequestCount%3479B4C601D5.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iRequestCount;
      //## end ClCCSender::m_iRequestCount%3479B4C601D5.attr

    // Additional Private Declarations
      //## begin ClCCSender%344737DF03AD.private preserve=yes
      //## end ClCCSender%344737DF03AD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCCSender%344737DF03AD.implementation preserve=yes
      //## end ClCCSender%344737DF03AD.implementation

};

//## begin ClCCSender%344737DF03AD.postscript preserve=yes
//## end ClCCSender%344737DF03AD.postscript

// Class ClCCSender 

//## begin module%34795A4000F6.epilog preserve=yes
//## end module%34795A4000F6.epilog


#endif
