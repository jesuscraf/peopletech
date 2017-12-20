//## begin module%45F8010E02FF.cm preserve=no
//## end module%45F8010E02FF.cm

//## begin module%45F8010E02FF.cp preserve=no
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
//## end module%45F8010E02FF.cp

//## Module: m4mdtsaxmerger%45F8010E02FF; Package specification
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Source file: C:\m4server\m4mdtransfer\inc\m4mdtsaxmerger.hpp

#ifndef m4mdtsaxmerger_h
#define m4mdtsaxmerger_h 1

//## begin module%45F8010E02FF.additionalIncludes preserve=no
//## end module%45F8010E02FF.additionalIncludes

//## begin module%45F8010E02FF.includes preserve=yes
#include "m4stl.hpp"
#include <m4imdtservice.hpp>
#include <m4imdtdictionary.hpp>
#include "m4mdtutils.hpp"
#include "m4mdtdef.hpp"
#include "xmlstreamgenerator.hpp"
#include <m4xmlbase.hpp>

#include "xercesc/parsers/XercesDOMParser.hpp"

//## end module%45F8010E02FF.includes

// m4xmlbase


//## begin module%45F8010E02FF.declarations preserve=no
//## end module%45F8010E02FF.declarations

//## begin module%45F8010E02FF.additionalDeclarations preserve=yes
//## end module%45F8010E02FF.additionalDeclarations


//## begin fieldsMap_t%45FFF8CD0282.preface preserve=yes
//## end fieldsMap_t%45FFF8CD0282.preface

//## Class: fieldsMap_t%45FFF8CD0282; Instantiated Class
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%45FFF91A0206;string { -> }

typedef map< string,string,less <string> > fieldsMap_t;

//## begin fieldsMap_t%45FFF8CD0282.postscript preserve=yes
//## end fieldsMap_t%45FFF8CD0282.postscript

//## begin ClMDTSAXMerger%45F7FD85028A.preface preserve=yes
//## end ClMDTSAXMerger%45F7FD85028A.preface

//## Class: ClMDTSAXMerger%45F7FD85028A
//	This class merges a XML data file with and XML complementary file (XML file
//	with excluded fields and pks).
//## Category: M4MDTransfer%3A3DFB720294
//## Subsystem: M4MDTransfer::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%45F9442403D8;string { -> }

class M4_DECL_M4MDTRANSFER  ClMDTSAXMerger : public M4SAXHandlerBase  //## Inherits: <unnamed>%45F9326A00B2
{
  //## begin ClMDTSAXMerger%45F7FD85028A.initialDeclarations preserve=yes
  //## end ClMDTSAXMerger%45F7FD85028A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMDTSAXMerger%1173884848
      //	Default constructor.
      ClMDTSAXMerger ();

    //## Destructor (specified)
      //## Operation: ~ClMDTSAXMerger%1173884849
      //	Release
      ~ClMDTSAXMerger ();


    //## Other Operations (specified)
      //## Operation: Init%1173884850
      //	Initialization.
      //	 @param ai_sXMLExclusionFileName exlcusion XML file name. This XML file
      //	contains fields excluded from XML data file. It could be empty (no exclusion
      //	file).
      //	 @param ai_sXMLMergedFileName merged XML file. This file contains all XML
      //	object data file plus exlcuded files.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t Init (IMDTService* ai_poService, const string &ai_sXMLExclusionFileName, const string &ai_sXMLMergedFileName);

      //## Operation: startDocument%1173884851
      //	Receive notification of the beginning of the document.
      //	Write XML declaration into XML rich file.
      void startDocument ();

      //## Operation: endDocument%1173884852
      //	Receive notification of the end of the document.
      //	Write XML buffer to XML rich file.
      void endDocument ();

      //## Operation: characters%1173884853
      //	Receive notification of character data inside an element.
      //
      //	 @param chars   the characters.
      //	 @param length  the number of characters to use from the  character array.
      void characters (const XMLCh* const chars, const XMLSize_t length);

    // Additional Public Declarations
      //## begin ClMDTSAXMerger%45F7FD85028A.public preserve=yes
      //## end ClMDTSAXMerger%45F7FD85028A.public

  protected:

    //## Other Operations (specified)
      //## Operation: _ProcObjectData%1173884857
      //	Receive notification of the start of an element .
      //
      //	 @param name   element type name.
      //	 @param attributes  specified or defaulted attributes.
      m4void_t _ProcObjectData (const XMLCh* const name, AttributeList& attributes);

      //## Operation: _ProcParamValue%1174575090
      //	Receive notification of the start of an element .
      //
      //	 @param name   element type name.
      //	 @param attributes  specified or defaulted attributes.
      m4void_t _ProcParamValue (const XMLCh* const name, AttributeList& attributes);

      //## Operation: _ProcTable%1173884859
      //	Receive notification of the start of an element "table".
      //	Save table name.
      //
      //	 @param name element type name.
      //	 @param attributes specified or defaulted attributes.
      m4void_t _ProcTable (const XMLCh* const name, AttributeList& attributes);

      //## Operation: _ProcRegister%1174575089
      //	Receive notification of the start of an element "table".
      //	Save table name.
      //
      //	 @param name element type name.
      //	 @param attributes specified or defaulted attributes.
      m4void_t _ProcRegister (const XMLCh* const name, AttributeList& attributes);

      //## Operation: _ProcField%1173884860
      //	Receive notification of the start of an element "field".
      //	Add a new attribute "caption" for the element. The "caption" attribute value
      //	will be retrieved from the dictionary and it is the translation of the
      //	"name" attribute value. If no translation found, we will take field name.
      //
      //	 @param name element type name.
      //	 @param attributes specified or defaulted attributes.
      m4void_t _ProcField (const XMLCh* const name, AttributeList& attributes);

      //## Operation: _ProcEndComplexElement%1173884854
      //	Receive notification of the end of an element. Enclose a complex XML element
      //	(</name>).
      //
      //	 @param name element type name.
      m4void_t _ProcEndComplexElement (const XMLCh * const name);

      //## Operation: _ProcEndSimpleElement%1173884855
      //	Receive notification of the end of an element. Enclose a simple XML element
      //	(> or />, depends on element value).
      //
      //	 @param name element type name.
      m4void_t _ProcEndSimpleElement (const XMLCh * const name);

      //## Operation: _ProcEndRegister%1173884856
      //	Receive notification of the end of a "register" element. This method looks
      //	for exclusion data for the current register and current table and dumps it.
      //
      //	 @param name element type name.
      m4void_t _ProcEndRegister (const XMLCh * const name);

      //## Operation: _LoadExclusionData%1174305607
      //	Loads exclusion data in a DOM document from input source.
      //
      //	 @param ai_sXMLExclusionFileName exlcusion XML file name. This XML file
      //	contains fields excluded from XML data file. It could be empty (no exclusion
      //	file).
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _LoadExclusionData (const string &ai_sXMLExclusionFileName);

      //## Operation: _LoadDictionaryData%1174403379
      m4return_t _LoadDictionaryData (const string &ai_sTableName, const string &ai_sFieldName, DOMNode* &ao_rpoLogicFieldNode);

      //## Operation: _GetXMLExclusionData%1174305608
      //	Gets exclusion XML data in an input source.
      //
      //	 @param ai_sXMLExclusionFileName exlcusion XML file name. This XML file
      //	contains fields excluded from XML data file. It could be empty (no exclusion
      //	file).
      //
      //	 @param  ao_rpoSource exlcusion data.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _GetXMLExclusionData (const string &ai_sExclusionXMLFileName, InputSource* &ao_rpoSource);

      //## Operation: _ReleaseXMLExclusionData%1174305609
      //	Release input source of an exclusion object.
      m4return_t _ReleaseXMLExclusionData (InputSource* &ao_rpoSource);

      //## Operation: _CheckIsPKField%1174296497
      //	Check if a given field is a PK field.
      //
      //	 @param ai_pccFieldName field name.
      //	 @param ao_bIsPk true if the given field is a PK, false if it is not a PK.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _CheckIsPKField (const string &ai_sTableName, const string &ai_sFieldName, m4bool_t &ao_bIsPk);

      //## Operation: _DumpElement%1174296501
      //	Dumps XML element data.
      //	 @param name element name.
      //	 @param ai_roAttributes element attributes.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _DumpElement (const XMLCh* const name, const AttributeList &ai_roAttributes, m4bool_t ai_bCloseElement);

      //## Operation: _DumpElementAttributes%1173884861
      //	Dumps attributes for an element.
      //
      //	 @param ai_roAttributes element attributes.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _DumpElementAttributes (const AttributeList &ai_roAttributes);

      //## Operation: _ProcessExclusionFields%1174305606
      //	Process exclusion fields for the current table and register.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _ProcessExclusionFields ();

      //## Operation: _CompareRegisters%1174305611
      //	Compare two register nodes, checking the pk fields. If there are same pks,
      //	it is same register
      //
      //	  @param ai_oExclusionPkFields, exclusion pk fields map.
      //	  @param ai_bSameRegister, true if it is same register, false if it is not
      //	the same register.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _CompareRegisters (fieldsMap_t ai_oExclusionPkFields, m4bool_t &ao_bSameRegister);

      //## Operation: _GenerateExclusionPKFields%1174305613
      //	This method generates a map with exclusion pk fields (name and value).
      //	  @param ai_sExclusionTableName exclusion table name.
      //	  @param ai_oExclusionNodeFields, exclusion node fields map.
      //	  @param ao_oExclusionPkFields, list of exclusion pk fields.
      //
      //	 @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _GenerateExclusionPKFields (const string &ai_sExclusionTableName, ClMDTDomTraverser ai_oExclusionNodeFields, fieldsMap_t &ao_oExclusionPkFields);

      //## Operation: _DumpExclusionFields%1174305612
      //	Dump exclusion fields data into merged xml file.
      //
      //	  @param ai_sExclusionTableName, exclusion table name.
      //	  @param ai_oExclusionFieldsMap, exclusion fields map.
      //
      //	  @return M4_SUCCESS if everything is ok, M4_ERROR if an error happens.
      m4return_t _DumpExclusionFields (const string &ai_sExclusionTableName, ClMDTDomTraverser ai_oExclusionFieldsMap);

    // Data Members for Class Attributes

      //## Attribute: m_sTableName%45F944020156
      //## begin ClMDTSAXMerger::m_sTableName%45F944020156.attr preserve=no  protected: string {UA} 
      string m_sTableName;
      //## end ClMDTSAXMerger::m_sTableName%45F944020156.attr

      //## Attribute: m_bIsMultiline%45FE7C04011D
      //## begin ClMDTSAXMerger::m_bIsMultiline%45FE7C04011D.attr preserve=no  protected: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsMultiline;
      //## end ClMDTSAXMerger::m_bIsMultiline%45FE7C04011D.attr

      XercesDOMParser m_oParser ;

      //## Attribute: m_oExclusionDOMData%45FE7F7D019F
      //## begin ClMDTSAXMerger::m_oExclusionDOMData%45FE7F7D019F.attr preserve=no  protected: DOMDocument {UA} 
	  xercesc::DOMDocument* m_poExclusionDOMData;
      //## end ClMDTSAXMerger::m_oExclusionDOMData%45FE7F7D019F.attr

      //## Attribute: m_poService%46012C540200
      //## begin ClMDTSAXMerger::m_poService%46012C540200.attr preserve=no  protected: IMDTService * {UA} NULL
      IMDTService *m_poService;
      //## end ClMDTSAXMerger::m_poService%46012C540200.attr

    // Data Members for Associations

      //## Association: M4MDTransfer::<unnamed>%45F80F5F00D7
      //## Role: ClMDTSAXMerger::m_poOutput%45F80F5F0210
      //## begin ClMDTSAXMerger::m_poOutput%45F80F5F0210.role preserve=no  protected: M4XMLStreamGenerator { -> RFHAN}
      M4XMLStreamGenerator *m_poOutput;
      //## end ClMDTSAXMerger::m_poOutput%45F80F5F0210.role

      //## Association: M4MDTransfer::<unnamed>%45F813580306
      //## Role: ClMDTSAXMerger::m_oOutStream%45F8135900E5
      //## begin ClMDTSAXMerger::m_oOutStream%45F8135900E5.role preserve=no  protected: ofstream { -> VHAN}
      ofstream m_oOutStream;
      //## end ClMDTSAXMerger::m_oOutStream%45F8135900E5.role

      //## Association: M4MDTransfer::<unnamed>%45FFF91E0274
      //## Role: ClMDTSAXMerger::m_oPKFieldsMap%45FFF91F017B
      //## begin ClMDTSAXMerger::m_oPKFieldsMap%45FFF91F017B.role preserve=no  protected: fieldsMap_t { -> VHAN}
      fieldsMap_t m_oPKFieldsMap;
      //## end ClMDTSAXMerger::m_oPKFieldsMap%45FFF91F017B.role

      //## Association: M4MDTransfer::<unnamed>%46013160031B
      //## Role: ClMDTSAXMerger::m_oDicLogicFieldsMap%460131610147
      //## begin ClMDTSAXMerger::m_oDicLogicFieldsMap%460131610147.role preserve=no  protected: MDTDOMTraverserMap_t { -> VHAN}
      MDTDOMTraverserMap_t m_oDicLogicFieldsMap;
      //## end ClMDTSAXMerger::m_oDicLogicFieldsMap%460131610147.role

    // Additional Protected Declarations
      //## begin ClMDTSAXMerger%45F7FD85028A.protected preserve=yes
      //## end ClMDTSAXMerger%45F7FD85028A.protected

  private:
    // Additional Private Declarations
      //## begin ClMDTSAXMerger%45F7FD85028A.private preserve=yes
      //## end ClMDTSAXMerger%45F7FD85028A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMDTSAXMerger%45F7FD85028A.implementation preserve=yes
      //## end ClMDTSAXMerger%45F7FD85028A.implementation

};

//## begin ClMDTSAXMerger%45F7FD85028A.postscript preserve=yes
//## end ClMDTSAXMerger%45F7FD85028A.postscript

// Class ClMDTSAXMerger 

//## begin module%45F8010E02FF.epilog preserve=yes
//## end module%45F8010E02FF.epilog


#endif
