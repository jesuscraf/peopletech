//## begin module%3498F0CD0342.cm preserve=no
//## end module%3498F0CD0342.cm

//## begin module%3498F0CD0342.cp preserve=no
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
//## end module%3498F0CD0342.cp

//## Module: FileReader%3498F0CD0342; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\filereader.hpp

#ifndef filereader_h
#define filereader_h 1

//## begin module%3498F0CD0342.additionalIncludes preserve=no
//## end module%3498F0CD0342.additionalIncludes

//## begin module%3498F0CD0342.includes preserve=yes
#include "m4stl.hpp"
//## end module%3498F0CD0342.includes

// ParserDefs
#include <parserdefs.hpp>
//## begin module%3498F0CD0342.declarations preserve=no
//## end module%3498F0CD0342.declarations

//## begin module%3498F0CD0342.additionalDeclarations preserve=yes
//## end module%3498F0CD0342.additionalDeclarations


//## begin ClFileReader%3498E90C01C1.preface preserve=yes
//## end ClFileReader%3498E90C01C1.preface

//## Class: ClFileReader%3498E90C01C1
//	Clase para leer un fichero palabra a palabra. Tendremos que ir leyendo
//	tokens hasta que cambiemos de sección, y luego comandos.
//
//	Class to read language file. It reads it word by word, reading first the
//	tokens and afterwards the commands.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3498F146002F;ClSentenceFrame { -> }

class ClFileReader 
{
  //## begin ClFileReader%3498E90C01C1.initialDeclarations preserve=yes
  //## end ClFileReader%3498E90C01C1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClFileReader%882438502
      //	Constructor
      ClFileReader ();

    //## Destructor (specified)
      //## Operation: ~ClFileReader%882438503
      //	Destructor
      ~ClFileReader ();


    //## Other Operations (specified)
      //## Operation: GetToken%882438497
      //	Devuelve un token
      //
      //	It returns a word from the language file
      m4token_t GetToken ();

      //## Operation: GetIdCommand%882438498
      //	Devuelve el id del comando
      //
      //	It returns the command id
      m4command_t GetIdCommand ();

      //## Operation: InSection%882438499
      //	Devuelve true si nos encontramos en la sección que se le pasa como parámetro.
      //
      //	It returns true if we are in the section passed as parameter.
      m4bool_t InSection (m4pchar_t a_szSection);

      //## Operation: InicFile%882438500
      //	Loads language file (the one passed as parameter).
      m4return_t InicFile (m4pchar_t a_szFileName);

      //## Operation: EndLine%882438504
      //	Devuelve true si el siguiente token se encuentra en una nueva línea
      //
      //	It returns true if next word is in a different line.
      m4bool_t EndLine ();

      //## Operation: CloseFile%882438506
      //	Cierra el fichero .ini
      //
      //	It closes language file
      m4return_t CloseFile ();

    // Additional Public Declarations
      //## begin ClFileReader%3498E90C01C1.public preserve=yes
      //## end ClFileReader%3498E90C01C1.public

  protected:

    //## Other Operations (specified)
      //## Operation: EliminaEspacios%882438501
      //	It positions over the following word.
      m4void_t EliminaEspacios ();

    // Additional Protected Declarations
      //## begin ClFileReader%3498E90C01C1.protected preserve=yes
      //## end ClFileReader%3498E90C01C1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_File%3498F9160298
      //	Fichero .ini
      //
      //	Language file
      //## begin ClFileReader::m_File%3498F9160298.attr preserve=no  private: ifstream {UA} 
      ifstream m_File;
      //## end ClFileReader::m_File%3498F9160298.attr

      //## Attribute: m_szSection%3499064F014A
      //	Sección del fichero en la que nos encontramos (TOKENS, COMMANDS, etc...).
      //
      //	File section we are in (TOKENS, COMMANDS, etc...).
      //## begin ClFileReader::m_szSection%3499064F014A.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szSection;
      //## end ClFileReader::m_szSection%3499064F014A.attr

    // Additional Private Declarations
      //## begin ClFileReader%3498E90C01C1.private preserve=yes
      //## end ClFileReader%3498E90C01C1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileReader%3498E90C01C1.implementation preserve=yes
      //## end ClFileReader%3498E90C01C1.implementation

};

//## begin ClFileReader%3498E90C01C1.postscript preserve=yes
//## end ClFileReader%3498E90C01C1.postscript

// Class ClFileReader 

//## begin module%3498F0CD0342.epilog preserve=yes
//## end module%3498F0CD0342.epilog


#endif
