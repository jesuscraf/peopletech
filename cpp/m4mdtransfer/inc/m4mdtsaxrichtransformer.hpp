//## begin module%4090B5580052.cm preserve=no
//## end module%4090B5580052.cm

//## begin module%4090B5580052.cp preserve=no
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
//## end module%4090B5580052.cp

//## Module: m4mdtsaxrichtransformer%4090B5580052; Package specification
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Source file: C:\m4server\m4mdtransfer\inc\m4mdtsaxrichtransformer.hpp

#ifndef m4mdtsaxrichtransformer_h
#define m4mdtsaxrichtransformer_h 1

//## begin module%4090B5580052.additionalIncludes preserve=no
//## end module%4090B5580052.additionalIncludes

//## begin module%4090B5580052.includes preserve=yes
#include "m4stl.hpp"
#include <m4imdtservice.hpp>
#include <m4imdtdictionary.hpp>
#include <m4mdtutils.hpp>
#include <xmlstreamgenerator.hpp>
#include <m4xmlbase.hpp>
#include <xercesc/dom/DOMNode.hpp>
//## end module%4090B5580052.includes


//## begin module%4090B5580052.declarations preserve=no
//## end module%4090B5580052.declarations

//## begin module%4090B5580052.additionalDeclarations preserve=yes
#define M4MDT_CAPTION_ATTRIBUTE			"caption"
#define M4MDT_NAME_ATTRIBUTE			"name"
#define M4MDT_VALUE_ATTRIBUTE			"value"
//## end module%4090B5580052.additionalDeclarations


//## begin MDTDOMTraverserMap_t%40BC554603E5.preface preserve=yes
//## end MDTDOMTraverserMap_t%40BC554603E5.preface

//## Class: MDTDOMTraverserMap_t%40BC554603E5; Instantiated Class
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%40BC5580029E;string { -> }

//## begin MDTDOMTraverserMap_t%40BC554603E5.postscript preserve=yes
//## end MDTDOMTraverserMap_t%40BC554603E5.postscript

//## begin ClMDTSAXRichTransformer%4090B35402C7.preface preserve=yes
//## end ClMDTSAXRichTransformer%4090B35402C7.preface

//## Class: ClMDTSAXRichTransformer%4090B35402C7
//	This class parses a given data XML, retrieves the translation of each
//	element and generates a rich XML file.
//## Category: M4MDTransfer::M4Transformer%4074075503E0
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%4090B43903C1;M4XMLChBase { -> F}
//## Uses: <unnamed>%4090B4650017;string { -> F}

class M4_DECL_M4MDTRANSFER ClMDTSAXRichTransformer : public M4SAXHandlerBase  //## Inherits: <unnamed>%4090B392021C
{
  //## begin ClMDTSAXRichTransformer%4090B35402C7.initialDeclarations preserve=yes
  //## end ClMDTSAXRichTransformer%4090B35402C7.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTSAXRichTransformer%1083226487
      //	Default constructor.
      ClMDTSAXRichTransformer ();

    //## Destructor (specified)
      //## Operation: ~ClMDTSAXRichTransformer%1083226488
      virtual ~ClMDTSAXRichTransformer ();


    //## Other Operations (specified)
      //## Operation: Init%1083226489
      //	Initialize class members and register XML element methods.
      //
      //	 @param ai_poService service.
      //	 @param ai_sRichXMLFile path to XML rich file.
      m4return_t Init (IMDTService* ai_poService, const string &ai_sRichXMLFile);

      //## Operation: startDocument%1083226490
      //	Receive notification of the beginning of the document.
      //	Write XML declaration into XML rich file.
      void startDocument ();

      //## Operation: endDocument%1083226491
      //	Receive notification of the end of the document.
      //	Write XML buffer to XML rich file.
      void endDocument ();

      //## Operation: characters%1083226494
      //	Receive notification of character data inside an element.
      //
      //	 @param chars   the characters.
      //	 @param length  the number of characters to use from the  character array.
      void characters (const XMLCh* const chars, const XMLSize_t length);

      //## Operation: procEndComplexElement%1083569618
      //	Receive notification of the end of an element. Enclose a complex XML element
      //	(</name>).
      //
      //	 @param name element type name.
      m4void_t procEndComplexElement (const XMLCh * const name);

      //## Operation: procEndSimpleElement%1083569619
      //	Receive notification of the end of an element. Enclose a simple XML element
      //	(> or />, depends on element value).
      //
      //	 @param name element type name.
      m4void_t procEndSimpleElement (const XMLCh * const name);

      //## Operation: procEndRegister%1084265436
      //	Receive notification of the end of an element. Enclose a simple XML element
      //	(> or />, depends on element value).
      //
      //	 @param name element type name.
      m4void_t procEndRegister (const XMLCh * const name);

    // Additional Public Declarations
      //## begin ClMDTSAXRichTransformer%4090B35402C7.public preserve=yes
      //## end ClMDTSAXRichTransformer%4090B35402C7.public

  protected:

    //## Other Operations (specified)
      //## Operation: procObjectData%1083233064
      //	Receive notification of the start of an element "object-data".
      //	Add a new attribute "caption" for the element with the value of the
      //	attribute "name".
      //
      //	 @param name   element type name.
      //	 @param attributes  specified or defaulted attributes.
      m4void_t procObjectData (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procParamValue%1083233065
      //	Receive notification of the start of an element "param-value".
      //
      //	 @param name   element type name.
      //	 @param attributes  specified or defaulted attributes.
      m4void_t procParamValue (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procTable%1083233066
      //	Receive notification of the start of an element "table".
      //	Add a new attribute "caption" for the element. The "caption" attribute value
      //	will be retrieved from the dictionary and it is the translation of the
      //	"name" attribute value. If there is no translation for this element, we will
      //	take the table name.
      //
      //	 @param name element type name.
      //	 @param attributes specified or defaulted attributes.
      m4void_t procTable (const XMLCh* const name, AttributeList& attributes);

      //## Operation: procField%1083233068
      //	Receive notification of the start of an element "field".
      //	Add a new attribute "caption" for the element. The "caption" attribute value
      //	will be retrieved from the dictionary and it is the translation of the
      //	"name" attribute value. If no translation found, we will take field name.
      //
      //	 @param name element type name.
      //	 @param attributes specified or defaulted attributes.
      m4void_t procField (const XMLCh* const name, AttributeList& attributes);

      //## Operation: LoadDictionaryInfo%1086078628
      //	Load dictionary requiered information (logig fields and lookups).
      //
      //	 @param ai_pccTableName table name.
      //	 @param ao_rpcTableCaption table caption.
      m4return_t LoadDictionaryInfo (m4pcchar_t ai_pccTableName, ClCharHolder& ao_rpcTableCaption);

      //## Operation: GenerateTableFields%1083233071
      //	Load table fields from dictionary and save logic-field nodes.
      //
      //	 @param ai_oDOMDocument dictionary DOM document.
      //	 @param ao_rpcTableCaption table field caption.
      m4return_t GenerateTableFields (const xercesc::DOMDocument *ai_pcoDOMDocument, ClCharHolder& ao_rpcTableCaption);

      //## Operation: GenerateLookUps%1086078627
      //	Load lookup information from dictionary.
      //
      //	 @param ai_oDOMDocument dictionary DOM document.
      m4return_t GenerateLookUps (const xercesc::DOMDocument *ai_pcoDOMDocument);

      //## Operation: GetFieldCaption%1083569616
      //	Get field caption (translated field name).
      //
      //	 @param ai_sFieldName field name.
      //	 @param ao_rpcFieldCaption field caption.
      m4return_t GetFieldCaption (const string &ai_sFieldName, ClCharHolder& ao_rpcFieldCaption);

      //## Operation: GetLookUpCaption%1086078629
      //	Get lookup caption for a given lookup value.
      //
      //	 @param ai_sLookUpName lookup name.
      //	 @param ai_sLookUpValue lookup value.
      //	 @param ao_rpcLookUpCaption lookup caption.
      m4return_t GetLookUpCaption (const string &ai_sLookUpName, const string &ai_sLookUpValue, ClCharHolder& ao_rpcLookUpCaption);

      //## Operation: CheckLookUp%1086165948
      //	Check if there is a lookup already inserted.
      //
      //	 @return true, if the lookup is found.
      m4bool_t CheckLookUp (const string &ai_sLookUpName);

      //## Operation: WriteElementAttributes%1085474121
      m4return_t WriteElementAttributes (const AttributeList &ai_roAttributes);

      //## Operation: DeleteLookUpMap%1086165947
      //	Delete lookup map.
      m4return_t DeleteLookUpMap ();

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40A094A7037D
      //## Role: ClMDTSAXRichTransformer::m_oOutStream%40A094A8014D
      //## begin ClMDTSAXRichTransformer::m_oOutStream%40A094A8014D.role preserve=no  protected: ofstream { -> VHAN}
      ofstream m_oOutStream;
      //## end ClMDTSAXRichTransformer::m_oOutStream%40A094A8014D.role

    // Additional Protected Declarations
      //## begin ClMDTSAXRichTransformer%4090B35402C7.protected preserve=yes
      //## end ClMDTSAXRichTransformer%4090B35402C7.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sRichXMLFile%4090B4130330
      //## begin ClMDTSAXRichTransformer::m_sRichXMLFile%4090B4130330.attr preserve=no  private: string {UA} 
      string m_sRichXMLFile;
      //## end ClMDTSAXRichTransformer::m_sRichXMLFile%4090B4130330.attr

      //## Attribute: m_poService%4090CE9703DB
      //## begin ClMDTSAXRichTransformer::m_poService%4090CE9703DB.attr preserve=no  private: IMDTService {RA} 
      IMDTService *m_poService;
      //## end ClMDTSAXRichTransformer::m_poService%4090CE9703DB.attr

      //## Attribute: m_bIsMultiline%40961C49006C
      //## begin ClMDTSAXRichTransformer::m_bIsMultiline%40961C49006C.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsMultiline;
      //## end ClMDTSAXRichTransformer::m_bIsMultiline%40961C49006C.attr

      //## Attribute: m_sFieldElements%40A093B300B5
      //## begin ClMDTSAXRichTransformer::m_sFieldElements%40A093B300B5.attr preserve=no  private: string {UA} 
      string m_sFieldElements;
      //## end ClMDTSAXRichTransformer::m_sFieldElements%40A093B300B5.attr

      //## Attribute: m_sRegisterPks%40A093C7033F
      //## begin ClMDTSAXRichTransformer::m_sRegisterPks%40A093C7033F.attr preserve=no  private: string {UA} 
      string m_sRegisterPks;
      //## end ClMDTSAXRichTransformer::m_sRegisterPks%40A093C7033F.attr

      //## Attribute: m_oLogicFields%40BD92D502E7
      //## begin ClMDTSAXRichTransformer::m_oLogicFields%40BD92D502E7.attr preserve=no  private: ClMDTDomTraverser {UA} 
      ClMDTDomTraverser m_oLogicFields;
      //## end ClMDTSAXRichTransformer::m_oLogicFields%40BD92D502E7.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::M4Transformer::<unnamed>%40BC556A01A2
      //## Role: ClMDTSAXRichTransformer::m_oLookUpMap%40BC556A0329
      //## begin ClMDTSAXRichTransformer::m_oLookUpMap%40BC556A0329.role preserve=no  private: MDTDOMTraverserMap_t { -> VHAN}
      MDTDOMTraverserMap_t m_oLookUpMap;
      //## end ClMDTSAXRichTransformer::m_oLookUpMap%40BC556A0329.role

      //## Association: M4MDTransfer::<unnamed>%4090B4440330
      //## Role: ClMDTSAXRichTransformer::m_poOutput%4090B4450076
      //## begin ClMDTSAXRichTransformer::m_poOutput%4090B4450076.role preserve=no  private: M4XMLStreamGenerator { -> RFHAN}
      M4XMLStreamGenerator *m_poOutput;
      //## end ClMDTSAXRichTransformer::m_poOutput%4090B4450076.role

    // Additional Private Declarations
      //## begin ClMDTSAXRichTransformer%4090B35402C7.private preserve=yes
      //## end ClMDTSAXRichTransformer%4090B35402C7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTSAXRichTransformer%4090B35402C7.implementation preserve=yes
      //## end ClMDTSAXRichTransformer%4090B35402C7.implementation

};

//## begin ClMDTSAXRichTransformer%4090B35402C7.postscript preserve=yes
//## end ClMDTSAXRichTransformer%4090B35402C7.postscript

// Class ClMDTSAXRichTransformer 

//## begin module%4090B5580052.epilog preserve=yes
//## end module%4090B5580052.epilog


#endif
