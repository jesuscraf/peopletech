//## begin module%34981006032B.cm preserve=no
//## end module%34981006032B.cm

//## begin module%34981006032B.cp preserve=no
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
//## end module%34981006032B.cp

//## Module: Parser%34981006032B; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\parser.hpp

#ifndef parser_h
#define parser_h 1

//## begin module%34981006032B.additionalIncludes preserve=no
//## end module%34981006032B.additionalIncludes

#include <m4parser_dll.hpp>
#include <tokencmdtable.hpp>
#include <lexanalyzer.hpp>
#include <parserdefs.hpp>
#include <sintaxistable.hpp>
#include <candidatesentences.hpp>
#include <filereader.hpp>
#include <commandefs.hpp>

//## begin module%34981006032B.declarations preserve=no
//## end module%34981006032B.declarations

//## begin module%34981006032B.additionalDeclarations preserve=yes
//## end module%34981006032B.additionalDeclarations


//## begin ClSenTStruct%34BA5940034A.preface preserve=yes
//## end ClSenTStruct%34BA5940034A.preface

//## Class: ClSenTStruct%34BA5940034A
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BA59EF031A;m4pcommand_t { -> }
//## Uses: <unnamed>%34C338370233;m4parsercommandargs_t { -> }

class ClSenTStruct 
{
  //## begin ClSenTStruct%34BA5940034A.initialDeclarations preserve=yes
  //## end ClSenTStruct%34BA5940034A.initialDeclarations

  public:
    // Data Members for Class Attributes

      //## Attribute: m_oPointer%34BA5953008B
      //## begin ClSenTStruct::m_oPointer%34BA5953008B.attr preserve=no  public: m4pcommand_t {UA} 
      m4pcommand_t m_oPointer;
      //## end ClSenTStruct::m_oPointer%34BA5953008B.attr

      //## Attribute: m_oArguments%34BA59C2030B
      //## begin ClSenTStruct::m_oArguments%34BA59C2030B.attr preserve=no  public: m4parsercommandargs_t {UA} 
      m4parsercommandargs_t m_oArguments;
      //## end ClSenTStruct::m_oArguments%34BA59C2030B.attr

    // Additional Public Declarations
      //## begin ClSenTStruct%34BA5940034A.public preserve=yes
      //## end ClSenTStruct%34BA5940034A.public

  protected:
    // Additional Protected Declarations
      //## begin ClSenTStruct%34BA5940034A.protected preserve=yes
      //## end ClSenTStruct%34BA5940034A.protected

  private:
    // Additional Private Declarations
      //## begin ClSenTStruct%34BA5940034A.private preserve=yes
      //## end ClSenTStruct%34BA5940034A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSenTStruct%34BA5940034A.implementation preserve=yes
      //## end ClSenTStruct%34BA5940034A.implementation

};

//## begin ClSenTStruct%34BA5940034A.postscript preserve=yes
//## end ClSenTStruct%34BA5940034A.postscript

//## Class: ClSentenceTable%3494FE610334
//	  Esta tabla contendrá todas las instancias de sentencia que han sido
//	analizadas.
//	  Cuando se realiza un Execute del programa, se ejecutarán todas las
//	sentencias de la tabla. Tras la ejecución, la tabla quedará vacía.
//
//	This table will contain all commands analised and ready for execution. After
//	executing them, this table will remain empty.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BA59EB02EC;ClSenTStruct { -> }

typedef vector< ClSenTStruct > ClSentenceTable;

//## Class: ClParser%3494FE6102E4
//	Clase principal, que gestiona todo el parser. Se encarga de rellenar las
//	distintas tablas, recibir, y procesar las cadenas de comandos.
//
//	Main class, which manages the whole parser. Its function is fulfilling the
//	different tables, receving and processing command strings.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34B625FB029E;m4pcommand_t { -> }
//## Uses: <unnamed>%34BCF31D01B4;ClSentenceTable { -> }

class M4_DECL_M4PARSER ClParser 
{
  public:
    //## Constructors (specified)
      //## Operation: ClParser%882380970
      //	Constructor. Inicializa las tablas y pasa la tabla sintáctica a la clase de
      //	sentencias candidatas.
      //
      //	Constructor. It initialises different tables.
      ClParser (ClTokenCommandTable* ai_oTokenCommandTable, m4pchar_t ai_szInicFileName);

    //## Destructor (specified)
      //## Operation: ~ClParser%882380971
      //	Destructor
      ~ClParser ();


    //## Other Operations (specified)
      //## Operation: Parse%882380972
      //	Recibe el comando a analizar, y lo procesa.
      //
      //	It receives the command to analyse, and processes it. It returns M4_SUCCESS
      //	if the analysed string is grammaticaly correct.
      //
      //	Parameters:
      //
      //	- ai_pSentence: Sentence to analyse.
      //	- aio_pSentenceTable: SentenceTable that will store pairs Pointer_to_
      //	executor - Arguments.
      m4return_t Parse (m4pchar_t ai_pSentence, ClSentenceTable *aio_pSentenceTable);

      //## Operation: SentenceClear%884684061
      //	It clears the input sentence table.
      m4return_t SentenceClear (ClSentenceTable *aio_pSentenceTable = NULL);

    // Data Members for Associations

      //## Association: M4Parser::<unnamed>%3494FE6102DB
      //## Role: ClParser::<m_pClLexAnalyzer>%3494FE6102DD
      //## begin ClParser::<m_pClLexAnalyzer>%3494FE6102DD.role preserve=no  public: ClLexAnalyzer { -> 1VHAN}
      ClLexAnalyzer m_pClLexAnalyzer;
      //## end ClParser::<m_pClLexAnalyzer>%3494FE6102DD.role

      //## Association: M4Parser::<unnamed>%3494FE6102E8
      //## Role: ClParser::<m_pClCandidateSentences>%3494FE6102EA
      //## begin ClParser::<m_pClCandidateSentences>%3494FE6102EA.role preserve=no  public: ClCandidateSentences { -> 1VHAN}
      ClCandidateSentences m_pClCandidateSentences;
      //## end ClParser::<m_pClCandidateSentences>%3494FE6102EA.role

      //## Association: M4Parser::<unnamed>%3498E9590059
      //## Role: ClParser::<m_pClFileReader>%3498E9590366
      //## begin ClParser::<m_pClFileReader>%3498E9590366.role preserve=no  public: ClFileReader { -> 1VHAN}
      ClFileReader m_pClFileReader;
      //## end ClParser::<m_pClFileReader>%3498E9590366.role

      //## Association: M4Parser::<unnamed>%34B6166E03A8
      //## Role: ClParser::m_poTokenCommandTable%34B616700116
      //## begin ClParser::m_poTokenCommandTable%34B616700116.role preserve=no  public: ClTokenCommandTable { -> 1RHAN}
      ClTokenCommandTable *m_poTokenCommandTable;
      //## end ClParser::m_poTokenCommandTable%34B616700116.role

      //## Association: M4Parser::<unnamed>%3494FE61030D
      //## Role: ClParser::m_oSintaxis%3494FE61030F
      //## begin ClParser::m_oSintaxis%3494FE61030F.role preserve=no  public: ClSintaxisTable { -> 1VHAN}
      ClSintaxisTable m_oSintaxis;
      //## end ClParser::m_oSintaxis%3494FE61030F.role

  protected:

    //## Other Operations (specified)
      //## Operation: FinalDeComando%882438507
      //	When processing the command line, it returns true if the command is over (;).
      m4bool_t FinalDeComando (m4token_t a_oToken);

      //## Operation: FinalDeBloque%882438508
      //	When processing the command line, it returns true if the command block is
      //	over.
      m4bool_t FinalDeBloque (m4token_t a_oToken);

      //## Operation: QueueClear%884781524
      //	It clears the arguments queue
      m4void_t QueueClear (m4parsercommandargs_t *aio_oComArg);

  private:
  private: //## implementation
};

// Class ClSenTStruct 

// Class ClParser 

//## begin module%34981006032B.epilog preserve=yes
//## end module%34981006032B.epilog


#endif
