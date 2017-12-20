//## begin module%3496BEB602B0.cm preserve=no
//## end module%3496BEB602B0.cm

//## begin module%3496BEB602B0.cp preserve=no
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
//## end module%3496BEB602B0.cp

//## Module: CandidateSentences%3496BEB602B0; Package specification
//## Subsystem: M4Parser::inc%379D926A02BC
//## Source file: D:\FuentesServidor\M4Parser\version\inc\candidatesentences.hpp

#ifndef candidatesentences_h
#define candidatesentences_h 1

//## begin module%3496BEB602B0.additionalIncludes preserve=no
//## end module%3496BEB602B0.additionalIncludes

//## begin module%3496BEB602B0.includes preserve=yes
//## end module%3496BEB602B0.includes

// SintaxisTable
#include <sintaxistable.hpp>
//## begin module%3496BEB602B0.declarations preserve=no
//## end module%3496BEB602B0.declarations

//## begin module%3496BEB602B0.additionalDeclarations preserve=yes
//## end module%3496BEB602B0.additionalDeclarations


//## begin ClCandidateSentences%3494FE6102EE.preface preserve=yes
//## end ClCandidateSentences%3494FE6102EE.preface

//## Class: ClCandidateSentences%3494FE6102EE
//	  Tabla que contendrá una copia de la tabla sintáctica, otra tabla con las
//	sentencias candidatas, y un contador como puntero a la parte de la sentencia
//	que se está analizando.
//
//	  En esta tabla se van a meter todas las sentencias que son candidatas para
//	el análisis final y de ella se irán sacando a medida que se van descartando.
//
//	  Al final de la entrada debe quedar una que esté en la tabla y que además
//	esté terminada de analizar. Caso contrario, la sentencia introducida no será
//	correcta.
//
//	  Table which will contain a copy of the syntaxis table, a candidate
//	sentences table, and a pointer to the not-analised sentence slice.
//
//	  This table will contain all candidate sentences to final analysis, which
//	will be extraídas as they are discarded.
//
//	  In the end only one sentence should remain in the table: that will be the
//	sentence to execute. In other case, the command line will be wrong.
//## Category: M4Parser%3494F6F901B7
//## Subsystem: M4Parser::inc%379D926A02BC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3496C1E2000F;ClSintaxisTable { -> }

class ClCandidateSentences 
{
  //## begin ClCandidateSentences%3494FE6102EE.initialDeclarations preserve=yes
  //## end ClCandidateSentences%3494FE6102EE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCandidateSentences%882292380
      //	Constructor
      ClCandidateSentences ();

    //## Destructor (specified)
      //## Operation: ~ClCandidateSentences%882292381
      //	Destructor.
      ~ClCandidateSentences ();


    //## Other Operations (specified)
      //## Operation: InicTable%882292382
      //	Inicializa la tabla sintáctica.
      //
      //	It initialises sintax table.
      m4return_t InicTable (ClSintaxisTable::CLSINTAXISTABLE a_oTable);

      //## Operation: InicSentence%882292383
      //	Inicializa la tabla de sentencias candidatas almacenando las sentencias cuyo
      //	primer token coincide con a_oToken. Devuelve M4_ERROR si no hay ninguna
      //	sentencia candidata
      //
      //	Initialises the candidate sentences table storing all posible sentences
      //	which first token matches a_oToken. It returns M4_ERROR if there's none.
      m4return_t InicSentence (m4token_t a_oToken);

      //## Operation: AddToken%882292384
      //	Elimina aquellas Sentencias Candidatas que no cotejan bien con a_oToken.
      //	Devuelve M4_ERROR si no hay ninguna sentencia candidata
      //
      //	Removes all candidate sentences which don't match a_oToken. It returns M4_
      //	ERROR if there's no candidate sentence left.
      m4return_t AddToken (m4token_t a_oToken);

      //## Operation: EndSentence%882292385
      //	Devuelve la tabla de sentencias candidatas. Si no hay sólo una, devuelve la
      //	tabla vacía.
      //
      //	It returns candidate sentence table. If there's no only one left, it returns
      //	an empty table.
      ClSintaxisTable::CLSINTAXISTABLE EndSentence ();

      //## Operation: ImprimeTablaSintactica%882783677
      //	Prints sintax table. Only for debugging purposes.
      m4void_t ImprimeTablaSintactica ();

      //## Operation: ImprimeSentenciasCandidatas%882783678
      //	Prints candidate sentences table. Only for debugging purposes.
      m4void_t ImprimeSentenciasCandidatas ();

    // Additional Public Declarations
      //## begin ClCandidateSentences%3494FE6102EE.public preserve=yes
      //## end ClCandidateSentences%3494FE6102EE.public

  protected:
    // Additional Protected Declarations
      //## begin ClCandidateSentences%3494FE6102EE.protected preserve=yes
      //## end ClCandidateSentences%3494FE6102EE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oSintaxisTable%3496BBD801B0
      //	Copia de la tabla sintáctica completa
      //
      //	Syntax table
      //## begin ClCandidateSentences::m_oSintaxisTable%3496BBD801B0.attr preserve=no  private: ClSintaxisTable::CLSINTAXISTABLE {UA} 
      ClSintaxisTable::CLSINTAXISTABLE m_oSintaxisTable;
      //## end ClCandidateSentences::m_oSintaxisTable%3496BBD801B0.attr

      //## Attribute: m_oCandidateSentences%3496C2A7030B
      //	Tabla que contiene las sentencias candidatas
      //
      //	Candidate sentences table
      //## begin ClCandidateSentences::m_oCandidateSentences%3496C2A7030B.attr preserve=no  private: ClSintaxisTable::CLSINTAXISTABLE {UA} 
      ClSintaxisTable::CLSINTAXISTABLE m_oCandidateSentences;
      //## end ClCandidateSentences::m_oCandidateSentences%3496C2A7030B.attr

      //## Attribute: m_uSentencePosition%3496C3200002
      //	Contador de la posición por la que se está procesando a las sentencias
      //	candidatas.
      //
      //	Pointer to the next word in candidate sentences
      //## begin ClCandidateSentences::m_uSentencePosition%3496C3200002.attr preserve=no  private: m4uint16_t {UA} 
      m4uint16_t m_uSentencePosition;
      //## end ClCandidateSentences::m_uSentencePosition%3496C3200002.attr

      //## Attribute: m_uSentenceCounter%3496D367031B
      //	Contador de las sentencias candidatas que hay actualmente en la tabla
      //
      //	Number of candidate sentences in table
      //## begin ClCandidateSentences::m_uSentenceCounter%3496D367031B.attr preserve=no  private: m4uint16_t {UA} 0
      m4uint16_t m_uSentenceCounter;
      //## end ClCandidateSentences::m_uSentenceCounter%3496D367031B.attr

    // Additional Private Declarations
      //## begin ClCandidateSentences%3494FE6102EE.private preserve=yes
      //## end ClCandidateSentences%3494FE6102EE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCandidateSentences%3494FE6102EE.implementation preserve=yes
      //## end ClCandidateSentences%3494FE6102EE.implementation

};

//## begin ClCandidateSentences%3494FE6102EE.postscript preserve=yes
//## end ClCandidateSentences%3494FE6102EE.postscript

// Class ClCandidateSentences 

//## begin module%3496BEB602B0.epilog preserve=yes
//## end module%3496BEB602B0.epilog


#endif
