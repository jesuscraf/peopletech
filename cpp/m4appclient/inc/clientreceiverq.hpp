//## begin module%349511E10022.cm preserve=no
//## end module%349511E10022.cm

//## begin module%349511E10022.cp preserve=no
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
//## end module%349511E10022.cp

//## Module: clientreceiverq%349511E10022; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\clientreceiverq.hpp

#ifndef clientreceiverq_h
#define clientreceiverq_h 1

//## begin module%349511E10022.additionalIncludes preserve=no
//## end module%349511E10022.additionalIncludes

//## begin module%349511E10022.includes preserve=yes
//## end module%349511E10022.includes

// FastQ
#include <fastfifoq.hpp>
// basepdu
#include <basepdus.hpp>
//## begin module%349511E10022.declarations preserve=no
//## end module%349511E10022.declarations

//## begin module%349511E10022.additionalDeclarations preserve=yes
//## end module%349511E10022.additionalDeclarations


//## Class: ClCCReceiverItem%3487C28402CF
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%348DC1A501B1;ClPDU { -> }

typedef ClPDU* ClCCReceiverItem;

//## Class: ClCCReceiverQ%3487C242039C; Instantiated Class
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3487C29F0183;ClCCReceiverItem { -> }

typedef FastQueue <ClCCReceiverItem> ClCCReceiverQ;

//## begin module%349511E10022.epilog preserve=yes
//## end module%349511E10022.epilog


#endif
