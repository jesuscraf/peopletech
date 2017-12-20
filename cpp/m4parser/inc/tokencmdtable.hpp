//## begin module%3494023500FC.cm preserve=no
//## end module%3494023500FC.cm

//## begin module%3494023500FC.cp preserve=no
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
//## end module%3494023500FC.cp

//## Module: TokenCommandTable%3494023500FC; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\tokencmdtable.hpp

#ifndef tokencommandtable_h
#define tokencommandtable_h 1

//## begin module%3494023500FC.additionalIncludes preserve=no
//## end module%3494023500FC.additionalIncludes

#include <m4parser_dll.hpp>
#include "m4stl.hpp"
#include <parserdefs.hpp>
#include <pairidcommand.hpp>

//## begin module%3494023500FC.declarations preserve=no
//## end module%3494023500FC.declarations

//## begin module%3494023500FC.additionalDeclarations preserve=yes
//## end module%3494023500FC.additionalDeclarations


//## Class: CLTOKENCOMMANDTABLE%3493FB1A0378
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3493FB960131;ClPairIdCommand { -> }
//## Uses: <unnamed>%3496EDA102C2;map { -> }
//## Uses: <unnamed>%35168B0D03B8;m4token_t { -> }

typedef deque < ClPairIdCommand *  > CLTOKENCOMMANDTABLE;

//## Class: ClTokenCommandTable%3493FADB0132
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PARSER ClTokenCommandTable 
{
  public:

    //## Other Operations (specified)
      //## Operation: Add%882623311
      //	   Añade un nuevo comando a nuestra tabla.
      //
      //	  Salida:
      //
      //	  Si ya existía un comando con el mismo identificador que se desea añadir,
      //	devuelve M4_ERROR y no modifica la tabla.
      //	  Si no existía un comoando con este identificador, devuelve M4_SUCCESS y
      //	añade el nuevo comando a la tabla.
      m4return_t Add (ClPairIdCommand *ai_pCommand);

      //## Operation: Get%882623312
      //	  Devuelve el comando de nuestra tabla que tenga el identificador ai_pId
      //
      //	  Si no existía en la tabla ningún comando con este identificador, devuelve
      //	NULL.
      ClPairIdCommand * Get (m4command_t ai_pId);

      //## Operation: Quit%882623313
      //	  Quita de la tabla el comando que se identifique con ai_pId. Si no existía
      //	ninguno con este identificador, no hace nada.
      void Quit (m4command_t ai_pId);

      //## Operation: Clear%882623314
      //	   Vacía la tabla de comandos.
      void Clear ();

      //## Operation: Exists%882623315
      //	  Devuelve:
      //
      //	  M4_TRUE si ya existía un comando que tuviera como identificador ai_pId.
      //	  M4_FALSE en caso contrario.
      m4bool_t Exists (m4command_t ai_pId);

    // Data Members for Associations

      //## Association: M4Parser::<unnamed>%379D95080020
      //## Role: ClTokenCommandTable::m_oCommands%379D95080305
      //## begin ClTokenCommandTable::m_oCommands%379D95080305.role preserve=no  public: CLTOKENCOMMANDTABLE { -> 1VHAN}
      CLTOKENCOMMANDTABLE m_oCommands;
      //## end ClTokenCommandTable::m_oCommands%379D95080305.role

  protected:
  private:
  private: //## implementation
};

// Class ClTokenCommandTable 

//## begin module%3494023500FC.epilog preserve=yes
//## end module%3494023500FC.epilog


#endif
