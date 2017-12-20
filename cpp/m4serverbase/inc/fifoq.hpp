//## begin module%34422253028A.cm preserve=no
//## end module%34422253028A.cm

//## begin module%34422253028A.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			ServerQ
//	 File:				FIFOQ.hpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971001
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
//## end module%34422253028A.cp

//## Module: FIFOQ%34422253028A; Package specification
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Source file: F:\integration\m4serverbase\inc\fifoq.hpp

#ifndef fifoq_h
#define fifoq_h 1

//## begin module%34422253028A.additionalIncludes preserve=no
//## end module%34422253028A.additionalIncludes

//## begin module%34422253028A.includes preserve=yes
//## end module%34422253028A.includes

// BaseRequest
#include <baserequest.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%34422253028A.declarations preserve=no
//## end module%34422253028A.declarations

//## begin module%34422253028A.additionalDeclarations preserve=yes
//## end module%34422253028A.additionalDeclarations


//## Class: ClSendItem%342F92350176
//	This class encapsulates references to Results that will be returned to the
//	appropriate client by the Sender.
//	Remarks
//	Son peticiones que incluyen la pdu de datos con la respuesta a enviar.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34322B810279;ClBaseAction { -> }

typedef ClBaseAction *ClSendItem;

//## Class: ClOutputItem%342F935B03A8
//	This class encapsulates references to Fulfilled Requests that will be
//	assigned to a ClSender object by the ClDistributor.
//	Remarks
//	Son peticiones que contienen las respuestas suministradas por los Executors.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34322B7E0121;ClBaseRequest { -> }

typedef ClBaseRequest *ClOutputItem;

//## Class: ClOutputQ%342F9350002E; Instantiated Class
//	De ella leen uno o varios Distributors y depositan uno o varios Executors.
//	Cola síncrona, con comportamiento FIFO.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34451C0A031A;ClOutputItem { -> }

typedef FastQueue< ClBaseRequest * > ClOutputQ;

//## Class: ClReceiveItem%342FBAE9008C
//	This class encapsulates references to unfulfilled Requests that will be
//	assigned to a ClCSExecutive object by the ClScheduler.
//	Remarks
//	Sus elementos son peticiones que incluyen las pdus de datos recibidas.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34322B7802E6;ClServiceRequest { -> }

typedef ClServiceRequest *ClReceiveItem;

//## Class: ClReceiveQ%3429499200EA; Instantiated Class
//	De ella leen uno o varios Schedulers y depositan uno o varios Receivers.
//	Cola síncrona con comportamiento FIFO.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%342FBAF7035A;ClReceiveItem { -> }

typedef FastQueue< ClServiceRequest * > ClReceiveQ;

//## Class: ClControlItem%343350E2015F
//	This class encapsulates references to connection/disconnection Requests that
//	will be assigned to the ClConnectionManager object by the ClReceiver.
//	Remarks
//	Sus elementos son peticiones que incluyen las pdus de control recibidas, o
//	bien son ficticias y contienen órdenes para los Connection Manager.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F4436C0353;ClBaseRequest { -> }

typedef ClBaseRequest *ClControlItem;

//## Class: ClControlQ%343350D000C3; Instantiated Class
//	De ella leen uno o varios Connection Manager, y depositan uno o varios
//	Receiver, el Monitor Local y el Timer.
//	Cola síncrona con comportamiento FIFO.
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%343350F60288;ClControlItem { -> }

typedef FastQueue< ClControlItem > ClControlQ;

//## Class: ClSendQ%342F91B2007C; Instantiated Class
//	De ella lee un Sender, y depositan uno o varios Distributors.
//	Cola síncrona, con inserción normal, y extracción selectiva (se le debe
//	permitir al Sender acceder a la petición, para comprobar el destino de la
//	operación y que no se encuentre ya transmitiendo por el mismo).
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%342F9BBA009B;ClSendItem { -> }

typedef FastQueue< ClSendItem > ClSendQ;

//## begin module%34422253028A.epilog preserve=yes
//## end module%34422253028A.epilog


#endif
