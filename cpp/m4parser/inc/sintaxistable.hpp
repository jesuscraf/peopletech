//## begin module%3496700D007A.cm preserve=no
//## end module%3496700D007A.cm

//## begin module%3496700D007A.cp preserve=no
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
//## end module%3496700D007A.cp

//## Module: SintaxisTable%3496700D007A; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\sintaxistable.hpp

#ifndef sintaxistable_h
#define sintaxistable_h 1

//## begin module%3496700D007A.additionalIncludes preserve=no
//## end module%3496700D007A.additionalIncludes

//## begin module%3496700D007A.includes preserve=yes
//## end module%3496700D007A.includes

// ParserDefs
#include <parserdefs.hpp>
//## begin module%3496700D007A.declarations preserve=no
//## end module%3496700D007A.declarations

//## begin module%3496700D007A.additionalDeclarations preserve=yes
	#ifdef _KCC
		using std::map;
		using std::less;
		using std::pair;
	#endif
typedef pair<m4command_t,ClSentenceFrame> SintaxPair;

//## end module%3496700D007A.additionalDeclarations


//## begin ClSintaxisTable%3494FE61030C.preface preserve=yes
//## end ClSintaxisTable%3494FE61030C.preface

//## Class: ClSintaxisTable%3494FE61030C
//	Clase que gestiona la tabla sintáctica.
//
//	Class to manage syntaxis table.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34966DF2030A;ClSentenceFrame { -> }
//## Uses: <unnamed>%34979F8402E4;m4command_t { -> }

class ClSintaxisTable 
{
  //## begin ClSintaxisTable%3494FE61030C.initialDeclarations preserve=yes
  //## end ClSintaxisTable%3494FE61030C.initialDeclarations

  public:
    //## Class: CLSINTAXISTABLE%34966D960325
    //## Category: M4Parser%3494F6F901B7
    //## Subsystem: M4Parser::inc%379D926A02BC
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef map< m4command_t, ClSentenceFrame , less <m4command_t> > CLSINTAXISTABLE;

    //## Constructors (specified)
      //## Operation: ClSintaxisTable%882271173
      //	Constructor
      ClSintaxisTable ();

    //## Destructor (specified)
      //## Operation: ~ClSintaxisTable%882271174
      //	Destructor
      ~ClSintaxisTable ();


    //## Other Operations (specified)
      //## Operation: AddToken%882271175
      //	Añade un Token nuevo al marco de sentencia actual
      //
      //	Adds a new token to the current sentence frame
      m4return_t AddToken (m4token_t a_szToken);

      //## Operation: NewSentenceFrame%882271176
      //	Crea un nuevo marco de sentencia, asignando el id que se le pasa como
      //	parámetro.
      //
      //	Creates a new sentence frame, assigning the id passed as parameter.
      m4return_t NewSentenceFrame (m4command_t a_oIdCommand);

      //## Operation: GetTable%882292379
      //	Devuelve la tabla sintáctica
      //
      //	Returns the syntaxis table
      ClSintaxisTable::CLSINTAXISTABLE GetTable ();

      //## Operation: ImprimeSentences%882783680
      //	Prints syntaxis table. Used only for debugging purposes.
      m4void_t ImprimeSentences ();

  public:
    // Additional Public Declarations
      //## begin ClSintaxisTable%3494FE61030C.public preserve=yes
      //## end ClSintaxisTable%3494FE61030C.public

  protected:
    // Additional Protected Declarations
      //## begin ClSintaxisTable%3494FE61030C.protected preserve=yes
      //## end ClSintaxisTable%3494FE61030C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oSintaxisTable%349674480027
      //	Tabla física
      //
      //	Physical tabl
      //## begin ClSintaxisTable::m_oSintaxisTable%349674480027.attr preserve=no  private: CLSINTAXISTABLE {UA} 
      CLSINTAXISTABLE m_oSintaxisTable;
      //## end ClSintaxisTable::m_oSintaxisTable%349674480027.attr

      //## Attribute: m_SentenceAux%3496799102B7
      //	Almacena la sentencia a la que se están añadiendo tokens.
      //
      //	Stores the sentence in which tokens are being inserted.
      //## begin ClSintaxisTable::m_SentenceAux%3496799102B7.attr preserve=no  private: ClSentenceFrame {UA} 
      ClSentenceFrame m_SentenceAux;
      //## end ClSintaxisTable::m_SentenceAux%3496799102B7.attr

    // Additional Private Declarations
      //## begin ClSintaxisTable%3494FE61030C.private preserve=yes
      //## end ClSintaxisTable%3494FE61030C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSintaxisTable%3494FE61030C.implementation preserve=yes
      //## end ClSintaxisTable%3494FE61030C.implementation

};

//## begin ClSintaxisTable%3494FE61030C.postscript preserve=yes
//## end ClSintaxisTable%3494FE61030C.postscript

// Class ClSintaxisTable 

//## begin module%3496700D007A.epilog preserve=yes
//## end module%3496700D007A.epilog


#endif
