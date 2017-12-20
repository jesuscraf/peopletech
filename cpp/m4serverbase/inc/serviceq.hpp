//## begin module%34421DE702E7.cm preserve=no
//## end module%34421DE702E7.cm

//## begin module%34421DE702E7.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:           ServerQ
//	 File:             ServerQ.hpp
//	 Project:			M4ComSrv
//	 Author:           Meta Software M.S. , S.A
//	 Date:				971001
//	 Language:         C++
//	 Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%34421DE702E7.cp

//## Module: ServiceQ%34421DE702E7; Package specification
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Source file: D:\eduardoma\m4serverbase\inc\serviceq.hpp

#ifndef serviceq_h
#define serviceq_h 1

//## begin module%34421DE702E7.additionalIncludes preserve=no
//## end module%34421DE702E7.additionalIncludes

//## begin module%34421DE702E7.includes preserve=yes
#include <baserequest.hpp>
//## end module%34421DE702E7.includes

// FastQ
#include <fastfifoq.hpp>
//## begin module%34421DE702E7.declarations preserve=no
//## end module%34421DE702E7.declarations

//## begin module%34421DE702E7.additionalDeclarations preserve=yes
//## end module%34421DE702E7.additionalDeclarations


//## Class: ClServiceItem%342F67A602DD
//	This class encapsulates references to Requests that will be assigned to a Cl
//	CSServiceQ object by the ClCSScheduler.
//	Remarks
//	The Service Queue must support two ordering criteria: FIFO and Priority
//	based. In order for the items to allow ordering operations, they must
//	include information related to priority.
//	Son peticiones ya verificadas y planificadas
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef ClBaseRequest *ClServiceItem;

//## Class: ClServiceQ%3420F995016E; Instantiated Class
//	De ella leen uno o varios Executors, y depositan uno o varios Schedulers.
//	Cola síncrona, con comportamiento simultáneo FIFO y por prioridades. Se
//	encuentra dividida realmente en dos colas similares (FIFO y por prioridades)
//	una para peticiones pertenecientes a operaciones con estado, y otra con
//	peticiones pertenecientes a operaciones sin estado. La extracción es
//	selectiva, en función del ID del Executor que solicite la operación (es
//	necesario seleccionar entre sin estado y con estado, y para peticiones de
//	operaciones con estado localizar una petición válida que pueda ser entregada
//	al Executor).
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%342F6A9F03A8;ClServiceItem { -> }

typedef FastQueue< ClBaseRequest * > ClServiceQ;

//## begin ClLogicalService%358E2AC20327.preface preserve=yes
//## end ClLogicalService%358E2AC20327.preface

//## Class: ClLogicalService%358E2AC20327
//## Category: M4ServerBase::ServerQ%341D51F4022A
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4pchar_t ClLogicalService;

//## begin ClLogicalService%358E2AC20327.postscript preserve=yes
//## end ClLogicalService%358E2AC20327.postscript

//## begin module%34421DE702E7.epilog preserve=yes
//## end module%34421DE702E7.epilog


#endif
