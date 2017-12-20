//## begin module%407BA3A80128.cm preserve=no
//## end module%407BA3A80128.cm

//## begin module%407BA3A80128.cp preserve=no
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
//## end module%407BA3A80128.cp

//## Module: m4imdtdictionary%407BA3A80128; Package specification
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Source file: C:\m4server\m4mdtransfer\inc\m4imdtdictionary.hpp

#ifndef m4imdtdictionary_h
#define m4imdtdictionary_h 1

//## begin module%407BA3A80128.additionalIncludes preserve=no
//## end module%407BA3A80128.additionalIncludes

//## begin module%407BA3A80128.includes preserve=yes
#include <m4types.hpp>
#include "m4stl.hpp"
#include <m4mdtransfer_dll.hpp>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMDocument.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class InputSource;
XERCES_CPP_NAMESPACE_END

//## end module%407BA3A80128.includes

//## begin module%407BA3A80128.declarations preserve=no
//## end module%407BA3A80128.declarations

//## begin module%407BA3A80128.additionalDeclarations preserve=yes
//## end module%407BA3A80128.additionalDeclarations


//## begin IMDTDictionary%407B9F690138.preface preserve=yes
//## end IMDTDictionary%407B9F690138.preface

//## Class: IMDTDictionary%407B9F690138
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::M4Transformer::inc%407ABB41034A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%407BBCAC02EE; { -> F}
//## Uses: <unnamed>%466E923703C1;vector { -> }

class M4_DECL_M4MDTRANSFER IMDTDictionary 
{
  //## begin IMDTDictionary%407B9F690138.initialDeclarations preserve=yes
  //## end IMDTDictionary%407B9F690138.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetXMLObject%1081843447
      //	Obtiene el XML de un objeto traspasable del diccionario de RAMDL
      //
      //	@param ai_iObject: Identificador del objeto traspasable
      //
      //	@param ao_rpoSource: Parámetro de salida con la fuente de lectura del objeto
      //	traspasable. Este objeto pertenece al diccionario y no puede ser borrado por
      //	el cliente. Para liberarlo hay que llamar a la función ReleaseObject
      //
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
      virtual m4return_t GetXMLObject (m4pcchar_t ai_pccObject, InputSource* &ao_rpoSource) = 0;

      //## Operation: GetXMLTable%1081843448
      //	Obtiene el XML de una tabla del diccionario de Meta4
      //
      //	@param ai_pccTable: Identificador de la tabla
      //
      //	@param ao_rpoSource: Parámetro de salida con la fuente de lectura de la
      //	tabla. Este objeto pertenece al diccionario y no puede ser borrado por el
      //	cliente. Para liberarlo hay que llamar a la función ReleaseObject
      //
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
      virtual m4return_t GetXMLTable (m4pcchar_t ai_pccTable, InputSource* &ao_rpoSource) = 0;

      //## Operation: ReleaseXMLObject%1081843449
      //	Libera la fuente de lectura asociada a un objeto, tanto del diccionario de
      //	RAMDL como del de Meta4. Pone a nulo el objeto
      //	@param ao_rpoSource: Identificador del objeto a liberar
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
      virtual m4return_t ReleaseXMLObject (InputSource* &ao_rpoSource) = 0;

      //## Operation: GetDOMTable%1173788149
      //	Obtiene el DOM de una tabla del diccionario de Meta4
      //	@param ai_pccTable: Identificador de la tabla
      //	@param ao_roDocument: Parámetro de salida con el DOM de la tabla
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
      virtual m4return_t GetDOMTable (m4pcchar_t ai_pccTable, xercesc::DOMDocument* &ao_rpoDocument) = 0;

      //## Operation: GetFieldConfiguration%1173788148
      //	Obtiene la configuración para un campo de un objeto lógico determinado. Este
      //	método devuelve si un determinado campo de un objeto lógico es comparable y
      //	si es traspasable.
      //
      //	@param ai_pccObjectName: Identificador del objeto lógico
      //	@param ai_pccFieldName: Identificador del campo lógico
      //	@param ao_bIsComparable: indica si el campo es comparable (1) o no (0).
      //	@param ao_IsTransferable: indica si el campo es traspasable(1) o no (0).
      //
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error.
      virtual m4return_t GetFieldConfiguration (m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccFieldName, m4bool_t &ao_bIsComparable, m4bool_t &ao_IsTransferable) = 0;

      //## Operation: GetConfigurationIds%1181651388
      //	Obtiene los identificadores de las configuraciones activas.
      //	Este método recupera los identificadores de las configuraciones que el
      //	usuario ha especificado en la tabla de configuraciones.
      //
      //	@param ao_vIdConfig: parámetro de salida que contiene los identificadores de
      //	las configuraciones.
      //
      //	@return: M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error.
      virtual m4return_t GetEnableConfigurationIds (vector<string> &ao_vConfigIds) = 0;

    // Additional Public Declarations
      //## begin IMDTDictionary%407B9F690138.public preserve=yes
      //## end IMDTDictionary%407B9F690138.public

  protected:
    // Additional Protected Declarations
      //## begin IMDTDictionary%407B9F690138.protected preserve=yes
      //## end IMDTDictionary%407B9F690138.protected

  private:
    // Additional Private Declarations
      //## begin IMDTDictionary%407B9F690138.private preserve=yes
      //## end IMDTDictionary%407B9F690138.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin IMDTDictionary%407B9F690138.implementation preserve=yes
      //## end IMDTDictionary%407B9F690138.implementation

};

//## begin IMDTDictionary%407B9F690138.postscript preserve=yes
//## end IMDTDictionary%407B9F690138.postscript

// Class IMDTDictionary 

//## begin module%407BA3A80128.epilog preserve=yes
//## end module%407BA3A80128.epilog


#endif
