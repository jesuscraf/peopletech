//## begin module%349505C40261.cm preserve=no
//## end module%349505C40261.cm

//## begin module%349505C40261.cp preserve=no
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
//## end module%349505C40261.cp

//## Module: SymbolTable%349505C40261; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\symboltable.hpp

#ifndef symboltable_h
#define symboltable_h 1

//## begin module%349505C40261.additionalIncludes preserve=no
//## end module%349505C40261.additionalIncludes

//## begin module%349505C40261.includes preserve=yes
//## end module%349505C40261.includes

// ParserDefs
#include <parserdefs.hpp>
//## begin module%349505C40261.declarations preserve=no
//## end module%349505C40261.declarations

//## begin module%349505C40261.additionalDeclarations preserve=yes
#ifdef _KCC
using std::less;
#endif
//## end module%349505C40261.additionalDeclarations


//## Class: ClSymbolTable%3494FE6102C6
//	Tabla de símbolos, utilizando map de las STL, que contiene pares del tipo
//	Token - id.
//
//	Symbol table which uses STL map. It contains Token - id pairs.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3494FE6102C7;m4token_t { -> }
//## Uses: <unnamed>%3494FE6102C8;m4Idtoken_t { -> }

typedef map < m4token_t, m4Idtoken_t , less <m4token_t> > ClSymbolTable;

//## begin module%349505C40261.epilog preserve=yes
//## end module%349505C40261.epilog


#endif
