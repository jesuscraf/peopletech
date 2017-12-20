//## begin module%3495064800F8.cm preserve=no
//## end module%3495064800F8.cm

//## begin module%3495064800F8.cp preserve=no
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
//## end module%3495064800F8.cp

//## Module: ParserDefs%3495064800F8; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\parserdefs.hpp

#ifndef parserdefs_h
#define parserdefs_h 1

//## begin module%3495064800F8.additionalIncludes preserve=no
//## end module%3495064800F8.additionalIncludes

//## begin module%3495064800F8.includes preserve=yes
#include "m4stl.hpp"
//## end module%3495064800F8.includes

// m4types
#include <m4types.hpp>
// ClCommand
#include <clcommand.hpp>
//## begin module%3495064800F8.declarations preserve=no
//## end module%3495064800F8.declarations

//## begin module%3495064800F8.additionalDeclarations preserve=yes
#ifdef _KCC
using std::vector;
using std::map;
using std::deque;
#endif
//## end module%3495064800F8.additionalDeclarations


//## Class: ClSentenceFrame%3494FE6102FD
//	Class to store sentences
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%349BF39600F6;vector { -> }

typedef vector< m4pchar_t > ClSentenceFrame;

//## Class: m4token_t%3494FE6102D0
//	Tipo de un token. Actualmente está definido como m4pchar_t
//
//	Token type. Right now defined as m4pchar_t
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4pchar_t m4token_t;

//## Class: m4Idtoken_t%3494FE6102DA
//	Identificador de un token
//
//	Token identifier
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%349656F4022A;m4uint16_t { -> }

typedef m4uint16_t m4Idtoken_t;

//## Class: m4command_t%34979F260394
//	Type of a command
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B3A808012A;m4pchar_t { -> }

typedef m4pchar_t m4command_t;

//## Class: m4parsercommandargs_t%349985CD00AB
//	Type of command arguments.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%349BF6B2006E;deque { -> }

typedef deque< m4token_t > m4parsercommandargs_t;

//## begin module%3495064800F8.epilog preserve=yes
//## end module%3495064800F8.epilog


#endif
