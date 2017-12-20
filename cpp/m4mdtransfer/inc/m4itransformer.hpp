//## begin module%408642760295.cm preserve=no
//## end module%408642760295.cm

//## begin module%408642760295.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.hpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%408642760295.cp

//## Module: m4itransformer%408642760295; Package specification
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Source file: C:\m4server\m4mdtransfer\inc\m4itransformer.hpp

#ifndef m4itransformer_h
#define m4itransformer_h 1

//## begin module%408642760295.additionalIncludes preserve=no
//## end module%408642760295.additionalIncludes

//## begin module%408642760295.includes preserve=yes
//## end module%408642760295.includes

//## begin module%408642760295.declarations preserve=no
//## end module%408642760295.declarations

//## begin module%408642760295.additionalDeclarations preserve=yes
#include <m4types.hpp>
//## end module%408642760295.additionalDeclarations


//## begin IMDTTransformer%4086419A00EF.preface preserve=yes
//## end IMDTTransformer%4086419A00EF.preface

//## Class: IMDTTransformer%4086419A00EF
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class IMDTTransformer 
{
  //## begin IMDTTransformer%4086419A00EF.initialDeclarations preserve=yes
  //## end IMDTTransformer%4086419A00EF.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GenerateXMLObjectData%1082385420
      //	Genera el XML de datos de un objeto traspasable para un repositorio concreto.
      //	Además proporciona una lista de los hijos de ese objeto
      //
      //	@param ai_pccIdConnection: Identificador de la conexión del repositorio que
      //	se debe emplear
      //
      //	@param ai_iIdObject: Identificador del objeto que se quiere traspasar
      //
      //	@param ai_lPKs: Referencia a un parámetro con las pk's de entrada
      //
      //	@param ai_pccXMLFileName: nombre del fichero de salida
	  //
	  //	@param ai_pccXMLExclusionFileName: nombre del fichero de campos excluidos
      //
      //	@param ao_iChilds: Array con los hijos del objeto a traspasar. Viene dado
      //	por parejas de identificador-array de pks. Y el array de pks viene dado por
      //	parejas de pk-valor
      //
      //	@return: estado de la generación.
      virtual m4return_t GenerateXMLObjectData (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccIdObject, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds) = 0;

      //## Operation: GenerateRichXMLObjectData%1084182897
      virtual m4return_t GenerateRichXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName) = 0;

      //## Operation: UpdateDB%1084200183
      virtual m4return_t UpdateDB (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName) = 0;

      //## Operation: MergeXMLObjectData%1174296498
      virtual m4return_t MergeXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccExlcusionXMLFileName, m4pcchar_t ai_pccMergedXMLFileName) = 0;

    // Additional Public Declarations
      //## begin IMDTTransformer%4086419A00EF.public preserve=yes
      //## end IMDTTransformer%4086419A00EF.public

  protected:

    //## Other Operations (specified)
      //## Operation: _UpdateDBNoTransaction%1084288690
      virtual m4return_t _UpdateDBNoTransaction (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName) = 0;

    // Additional Protected Declarations
      //## begin IMDTTransformer%4086419A00EF.protected preserve=yes
      //## end IMDTTransformer%4086419A00EF.protected

  private:
    // Additional Private Declarations
      //## begin IMDTTransformer%4086419A00EF.private preserve=yes
      //## end IMDTTransformer%4086419A00EF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IMDTTransformer%4086419A00EF.implementation preserve=yes
      //## end IMDTTransformer%4086419A00EF.implementation

};

//## begin IMDTTransformer%4086419A00EF.postscript preserve=yes
//## end IMDTTransformer%4086419A00EF.postscript

// Class IMDTTransformer 

//## begin module%408642760295.epilog preserve=yes
//## end module%408642760295.epilog


#endif
