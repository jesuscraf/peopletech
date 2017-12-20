//## begin module%34829E1E019C.cm preserve=no
//## end module%34829E1E019C.cm

//## begin module%34829E1E019C.cp preserve=no
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
//## end module%34829E1E019C.cp

//## Module: clientsenderq%34829E1E019C; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\clientsenderq.hpp

#ifndef clientsenderq_h
#define clientsenderq_h 1

//## begin module%34829E1E019C.additionalIncludes preserve=no
//## end module%34829E1E019C.additionalIncludes

//## begin module%34829E1E019C.includes preserve=yes
//## end module%34829E1E019C.includes

// clientrequest
#include <clientrequest.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%34829E1E019C.declarations preserve=no
//## end module%34829E1E019C.declarations

//## begin module%34829E1E019C.additionalDeclarations preserve=yes
//## end module%34829E1E019C.additionalDeclarations


//## Class: ClCCSendItem%34829C130317
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34829C5B025C;ClCCRequest { -> }

typedef ClCCRequest* ClCCSendItem;

//## Class: ClCCSendQ%347DA2B700C6; Instantiated Class
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34829C550344;ClCCSendItem { -> }

typedef FastQueue <ClCCSendItem> ClCCSendQ;

//## begin module%34829E1E019C.epilog preserve=yes
//## end module%34829E1E019C.epilog


#endif
