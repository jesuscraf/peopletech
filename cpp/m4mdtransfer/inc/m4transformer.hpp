//## begin module%407BEBC9035B.cm preserve=no
//## end module%407BEBC9035B.cm

//## begin module%407BEBC9035B.cp preserve=no
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
//## end module%407BEBC9035B.cp

//## Module: m4transformer%407BEBC9035B; Package specification
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Source file: C:\m4server\m4mdtransfer\inc\m4transformer.hpp

#ifndef m4transformer_h
#define m4transformer_h 1

//## begin module%407BEBC9035B.additionalIncludes preserve=no
//## end module%407BEBC9035B.additionalIncludes

//## begin module%407BEBC9035B.includes preserve=yes
//## end module%407BEBC9035B.includes

#include <m4mdtsaxmerger.hpp>
#include <m4mdtsaxrichtransformer.hpp>
#include <m4itransformer.hpp>
#include <m4imdtdictionary.hpp>
#include <m4saxtransformer.hpp>
#include <m4types.hpp>

class LocalFileInputSource;
class ClMDTSaxUpdater;

//## end module%407BEBC9035B.additionalDeclarations


//## begin ClMDTTransformer%407BEB2C004E.preface preserve=yes
class IMDTService;
//## end ClMDTTransformer%407BEB2C004E.preface

//## Class: ClMDTTransformer%407BEB2C004E
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%407C18BA038A;ClMDTDataSAXTransformer { -> }
//## Uses: <unnamed>%407D613A00A9;IMDTDictionary { -> }
//## Uses: <unnamed>%409F53610379;ClMDTSAXRichTransformer { -> }
//## Uses: <unnamed>%45FE93DD027C;ClMDTSAXMerger { -> }

class ClMDTTransformer : public IMDTTransformer  //## Inherits: <unnamed>%408642BB014C
{
  //## begin ClMDTTransformer%407BEB2C004E.initialDeclarations preserve=yes
  //## end ClMDTTransformer%407BEB2C004E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTTransformer%1081852893
      ClMDTTransformer ();


    //## Other Operations (specified)
      //## Operation: Init%1081852892
      m4return_t Init (IMDTService* ai_poService);

      //## Operation: GenerateXMLObjectData%1081852891
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
      //	@param ao_iChilds: Array con los hijos del objeto a traspasar. Viene dado
      //	por parejas de identificador-array de pks. Y el array de pks viene dado por
      //	parejas de pk-valor
      //
      //	@return: estado de la generación.
      m4return_t GenerateXMLObjectData (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccIdObject, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds);

      //## Operation: GenerateRichXMLObjectData%1084182898
      m4return_t GenerateRichXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName);

      //## Operation: UpdateDB%1084188477
      m4return_t UpdateDB (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName);

      //## Operation: MergeXMLObjectData%1174296500
      m4return_t MergeXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccExlcusionXMLFileName, m4pcchar_t ai_pccMergedXMLFileName);

      //## Operation: End%1081852908
      m4void_t End ();

    // Additional Public Declarations
      //## begin ClMDTTransformer%407BEB2C004E.public preserve=yes
      //## end ClMDTTransformer%407BEB2C004E.public

  protected:

    //## Other Operations (specified)
      //## Operation: _UpdateDBNoTransaction%1084288689
      m4return_t _UpdateDBNoTransaction (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName);

      //## Operation: _UpdateDB%1084188478
      m4return_t _UpdateDB (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLBaseFileName, m4pcchar_t ai_pccXMLFileName, ClMDTSaxUpdater& ai_roSaxUpdater);

      //## Operation: ParseObject%1081852894
      m4return_t ParseObject (m4pcchar_t ai_pccConnectionId, m4pcchar_t ai_pccObjId, InputSource* ai_oInput, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds);

    // Additional Protected Declarations
      //## begin ClMDTTransformer%407BEB2C004E.protected preserve=yes
      //## end ClMDTTransformer%407BEB2C004E.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bInit%407BFB63034B
      //## begin ClMDTTransformer::m_bInit%407BFB63034B.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bInit;
      //## end ClMDTTransformer::m_bInit%407BFB63034B.attr

      //## Attribute: m_poService%407D5E2B016F
      //## begin ClMDTTransformer::m_poService%407D5E2B016F.attr preserve=no  private: IMDTService* {UA} NULL
      IMDTService* m_poService;
      //## end ClMDTTransformer::m_poService%407D5E2B016F.attr

    // Additional Private Declarations
      //## begin ClMDTTransformer%407BEB2C004E.private preserve=yes
      //## end ClMDTTransformer%407BEB2C004E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTTransformer%407BEB2C004E.implementation preserve=yes
      //## end ClMDTTransformer%407BEB2C004E.implementation

};

//## begin ClMDTTransformer%407BEB2C004E.postscript preserve=yes
//## end ClMDTTransformer%407BEB2C004E.postscript

// Class ClMDTTransformer 

//## begin module%407BEBC9035B.epilog preserve=yes
//## end module%407BEBC9035B.epilog


#endif
