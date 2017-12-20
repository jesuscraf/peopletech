//## begin module%3495053202B1.cm preserve=no
//## end module%3495053202B1.cm

//## begin module%3495053202B1.cp preserve=no
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
//## end module%3495053202B1.cp

//## Module: LexAnalyzer%3495053202B1; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\lexanalyzer.hpp

#ifndef lexanalyzer_h
#define lexanalyzer_h 1

//## begin module%3495053202B1.additionalIncludes preserve=no
//## end module%3495053202B1.additionalIncludes

//## begin module%3495053202B1.includes preserve=yes
//## end module%3495053202B1.includes

// SymbolTable
#include <symboltable.hpp>
//## begin module%3495053202B1.declarations preserve=no
//## end module%3495053202B1.declarations

//## begin module%3495053202B1.additionalDeclarations preserve=yes
//## end module%3495053202B1.additionalDeclarations


//## begin ClLexAnalyzer%3494FE6102BC.preface preserve=yes
//## end ClLexAnalyzer%3494FE6102BC.preface

//## Class: ClLexAnalyzer%3494FE6102BC
//	Procesa cada palabra de la cadena comando, comprueba si es un token o no, y
//	devuelve el resultado a ClParser. Siempre actúa bajo las directrices del Cl
//	Parser.
//
//	Processes each word from the command string, checks wheter it's a token or
//	not, and gets the result to ClParser. It always acts under the orders from Cl
//	Parser.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClLexAnalyzer 
{
  //## begin ClLexAnalyzer%3494FE6102BC.initialDeclarations preserve=yes
  //## end ClLexAnalyzer%3494FE6102BC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLexAnalyzer%882181783
      //	Constructor
      ClLexAnalyzer ();

    //## Destructor (specified)
      //## Operation: ~ClLexAnalyzer%882263090
      //	Destructor
      ~ClLexAnalyzer ();


    //## Other Operations (specified)
      //## Operation: AddSymbol%882263087
      //	Añade el símbolo que se le pasa como parámetro a la tabla de símbolos,
      //	asignando su id automaticamente. Devuelve M4_ERROR si el símbolo ya está
      //	definido.
      //
      //	Adds a symbol to the symbol table, assigning its id automatically. It
      //	returns M4_ERROR if the symbol is already defined.
      m4return_t AddSymbol (m4token_t a_acToken);

      //## Operation: GetWord%882263088
      //	Devuelve 0 si el token analizado no existe y su valor en a_szTokenValue, o
      //	el id del token en caso contrario. Final de cadena= ""
      //
      //	It returns token value in a_szTokenValue, and 0 if the token doesn't exists
      //	or it's value if it does.
      m4Idtoken_t GetWord (m4pchar_t* a_szTokenValue);

      //## Operation: InicString%882263089
      //	Recibe una cadena para procesar. Aquí cadena se entiende como string cuyo
      //	último carácter es \0.
      //
      //	It initialises the string to process.
      m4return_t InicString (m4pchar_t a_acCadena);

      //## Operation: ImprimeLista%882783679
      //	Imprime la tabla de símbolos. Codificada por motivos únicamente de
      //	depuración.
      //
      //	It prints symbol table. Codified only for debugging purposes.
      m4void_t ImprimeLista ();

    // Additional Public Declarations
      //## begin ClLexAnalyzer%3494FE6102BC.public preserve=yes
      //## end ClLexAnalyzer%3494FE6102BC.public

  protected:

    //## Other Operations (specified)
      //## Operation: Find%882438505
      //	Devuelve un iterador a la posición de la tabla donde se encuentra el token
      //	que se le pasa como parámetro, o tabla.end() si no pertenece a dicha tabla.
      //
      //	It returns an iterator to the position where the token is found if it exists
      //	inside the table, or an iterator to tabla.end if it's not found.
      ClSymbolTable::iterator Find (m4token_t a_oToken);

    // Additional Protected Declarations
      //## begin ClLexAnalyzer%3494FE6102BC.protected preserve=yes
      //## end ClLexAnalyzer%3494FE6102BC.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oCountTokens%3496423501AB
      //	Contador de tokens en la tabla de símbolos
      //
      //	Tokens counter
      //## begin ClLexAnalyzer::m_oCountTokens%3496423501AB.attr preserve=no  private: m4Idtoken_t {UA} 0
      m4Idtoken_t m_oCountTokens;
      //## end ClLexAnalyzer::m_oCountTokens%3496423501AB.attr

      //## Attribute: m_szCadena%3496426101C2
      //	Cadena completa que se está procesando
      //
      //	Complete array in proccess
      //## begin ClLexAnalyzer::m_szCadena%3496426101C2.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szCadena;
      //## end ClLexAnalyzer::m_szCadena%3496426101C2.attr

      //## Attribute: m_szActualToken%3496428A0207
      //	Token que se está procesando actualmente.
      //
      //	Current token in proccess
      //## begin ClLexAnalyzer::m_szActualToken%3496428A0207.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szActualToken;
      //## end ClLexAnalyzer::m_szActualToken%3496428A0207.attr

      //## Attribute: m_szActualCadena%349642B00067
      //	Puntero para recorrer m_szCadena.
      //
      //	Pointer to go along m_szCadena
      //## begin ClLexAnalyzer::m_szActualCadena%349642B00067.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szActualCadena;
      //## end ClLexAnalyzer::m_szActualCadena%349642B00067.attr

    // Data Members for Associations

      //## Association: M4Parser::<unnamed>%3494FE6102BE
      //## Role: ClLexAnalyzer::m_oSymbolTable%3494FE6102C0
      //## begin ClLexAnalyzer::m_oSymbolTable%3494FE6102C0.role preserve=no  private: ClSymbolTable { -> 1VHAN}
      ClSymbolTable m_oSymbolTable;
      //## end ClLexAnalyzer::m_oSymbolTable%3494FE6102C0.role

    // Additional Private Declarations
      //## begin ClLexAnalyzer%3494FE6102BC.private preserve=yes
      //## end ClLexAnalyzer%3494FE6102BC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLexAnalyzer%3494FE6102BC.implementation preserve=yes
      //## end ClLexAnalyzer%3494FE6102BC.implementation

};

//## begin ClLexAnalyzer%3494FE6102BC.postscript preserve=yes
//## end ClLexAnalyzer%3494FE6102BC.postscript

// Class ClLexAnalyzer 

//## begin module%3495053202B1.epilog preserve=yes
//## end module%3495053202B1.epilog


#endif
