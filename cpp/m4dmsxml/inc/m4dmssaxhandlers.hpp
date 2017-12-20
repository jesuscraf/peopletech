//## begin module%3A3E58540028.cm preserve=no
//## end module%3A3E58540028.cm

//## begin module%3A3E58540028.cp preserve=no
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
//## end module%3A3E58540028.cp

//## Module: m4dmssaxhandlers%3A3E58540028; Package specification
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Source file: C:\m4server-xerces2.5.0\m4dmsxml\inc\m4dmssaxhandlers.hpp

#ifndef m4dmssaxhandlers_h
#define m4dmssaxhandlers_h 1

//## begin module%3A3E58540028.additionalIncludes preserve=no
//## end module%3A3E58540028.additionalIncludes

//## begin module%3A3E58540028.includes preserve=yes
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <m4types.hpp>
//## end module%3A3E58540028.includes

#include "m4stl.hpp"
// node_x
#include <node_x.hpp>
// SAXHandlerBase
#include <xercesc/sax/HandlerBase.hpp>

class ClAccess;
class ClChannel;
class ClChannelManager;
class ClDMSXmlM4OMap;
class ClDMSXmlException;
class ClDMSChannelHandler;

//## begin module%3A3E58540028.declarations preserve=no
//## end module%3A3E58540028.declarations

//## begin module%3A3E58540028.additionalDeclarations preserve=yes
XERCES_CPP_NAMESPACE_USE
//## end module%3A3E58540028.additionalDeclarations


//## begin stChannelElementInfo%3A3E49F501EB.preface preserve=yes
//## end stChannelElementInfo%3A3E49F501EB.preface

//## Class: stChannelElementInfo%3A3E49F501EB
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A3F7DD1023C;string { -> }
//## Uses: <unnamed>%3A3F7DD5006B;ClNode { -> }

typedef vector< string > ClXMLStringVector;

struct stChannelElementInfo 
{
  //## begin stChannelElementInfo%3A3E49F501EB.initialDeclarations preserve=yes
  //## end stChannelElementInfo%3A3E49F501EB.initialDeclarations

    //## Constructors (specified)
      //## Operation: stChannelElementInfo%977216650
      stChannelElementInfo (const string &ai_sXmlElementName, const string &ai_sElementName, const m4uint32_t &ai_ElementType, ClNode *  ai_poParent, const m4uint8_t &ai_uiCppType, const m4int8_t &ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId);

    // Data Members for Class Attributes

      //## Attribute: m_sXmlElementName%3A3E4B7D03CF
      //## begin stChannelElementInfo::m_sXmlElementName%3A3E4B7D03CF.attr preserve=no  public: string {UA} ai_sXmlElementName
      string m_sXmlElementName;
      //## end stChannelElementInfo::m_sXmlElementName%3A3E4B7D03CF.attr

      //## Attribute: m_uiElementType%3A3E4C1A0154
      //## begin stChannelElementInfo::m_uiElementType%3A3E4C1A0154.attr preserve=no  public: m4uint32_t {UA} ai_ElementType
      m4uint32_t m_uiElementType;
      //## end stChannelElementInfo::m_uiElementType%3A3E4C1A0154.attr

      //## Attribute: m_poParent%3A3E4C46032E
      //## begin stChannelElementInfo::m_poParent%3A3E4C46032E.attr preserve=no  public: ClNode * {UA} ai_poParent
      ClNode *m_poParent;
      //## end stChannelElementInfo::m_poParent%3A3E4C46032E.attr

      //## Attribute: m_uiCppType%3A5C2E960355
      //## begin stChannelElementInfo::m_uiCppType%3A5C2E960355.attr preserve=no  public: m4uint8_t {UA} ai_uiCppType
      m4uint8_t m_uiCppType;
      //## end stChannelElementInfo::m_uiCppType%3A5C2E960355.attr

      //## Attribute: m_sElementName%3A6D60840308
      //## begin stChannelElementInfo::m_sElementName%3A6D60840308.attr preserve=no  public: string {UA} ai_sElementName
      string m_sElementName;
      //## end stChannelElementInfo::m_sElementName%3A6D60840308.attr

      //## Attribute: m_sElementValue%3AC468E102EC
      //## begin stChannelElementInfo::m_sElementValue%3AC468E102EC.attr preserve=no  public: string {UA} ""
      string m_sElementValue;
      //## end stChannelElementInfo::m_sElementValue%3AC468E102EC.attr

	  // Record status.
	  m4int8_t m_iStatus;

	  // Preprocess method.
	  string m_sPreprocessMethodId;

	  // Postprocess method.
	  string m_sPostprocessMethodId;

  public:
    // Additional Public Declarations
      //## begin stChannelElementInfo%3A3E49F501EB.public preserve=yes
	  ClXMLStringVector m_oItemsVector;
	  //## end stChannelElementInfo%3A3E49F501EB.public

  protected:
    // Additional Protected Declarations
      //## begin stChannelElementInfo%3A3E49F501EB.protected preserve=yes
      //## end stChannelElementInfo%3A3E49F501EB.protected

  private:
    // Additional Private Declarations
      //## begin stChannelElementInfo%3A3E49F501EB.private preserve=yes
      //## end stChannelElementInfo%3A3E49F501EB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin stChannelElementInfo%3A3E49F501EB.implementation preserve=yes
      //## end stChannelElementInfo%3A3E49F501EB.implementation

};

//## begin stChannelElementInfo%3A3E49F501EB.postscript preserve=yes
//## end stChannelElementInfo%3A3E49F501EB.postscript

//## begin ElementHandlerList%3A3E43FA02C6.preface preserve=yes
//## end ElementHandlerList%3A3E43FA02C6.preface

//## Class: ElementHandlerList%3A3E43FA02C6; Instantiated Class
//	This list saves the info for the elements that are being parsed.
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A3E4C7103B2;stChannelElementInfo { -> }

typedef list< stChannelElementInfo * > ElementHandlerList;

//## begin ElementHandlerList%3A3E43FA02C6.postscript preserve=yes
//## end ElementHandlerList%3A3E43FA02C6.postscript

//## begin stBaseElementKeyInfo%3A6DAE830164.preface preserve=yes
//## end stBaseElementKeyInfo%3A6DAE830164.preface

//## Class: stBaseElementKeyInfo%3A6DAE830164
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct stBaseElementKeyInfo 
{
  //## begin stBaseElementKeyInfo%3A6DAE830164.initialDeclarations preserve=yes
  //## end stBaseElementKeyInfo%3A6DAE830164.initialDeclarations

    //## Constructors (specified)
      //## Operation: stBaseElementKeyInfo%980239728
      stBaseElementKeyInfo (const string &ai_sName);


    //## Other Operations (specified)
      //## Operation: SetValue%980239729
      void SetValue (string ai_sValue);

    // Data Members for Class Attributes

      //## Attribute: m_sName%3A6DAED10026
      //## begin stBaseElementKeyInfo::m_sName%3A6DAED10026.attr preserve=no  public: string {UA} ai_sName
      string m_sName;
      //## end stBaseElementKeyInfo::m_sName%3A6DAED10026.attr

      //## Attribute: m_sValue%3A6DAEC90255
      //## begin stBaseElementKeyInfo::m_sValue%3A6DAEC90255.attr preserve=no  public: string {UA} ""
      string m_sValue;
      //## end stBaseElementKeyInfo::m_sValue%3A6DAEC90255.attr

  public:
    // Additional Public Declarations
      //## begin stBaseElementKeyInfo%3A6DAE830164.public preserve=yes
      //## end stBaseElementKeyInfo%3A6DAE830164.public

  protected:
    // Additional Protected Declarations
      //## begin stBaseElementKeyInfo%3A6DAE830164.protected preserve=yes
      //## end stBaseElementKeyInfo%3A6DAE830164.protected

  private:
    // Additional Private Declarations
      //## begin stBaseElementKeyInfo%3A6DAE830164.private preserve=yes
      //## end stBaseElementKeyInfo%3A6DAE830164.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin stBaseElementKeyInfo%3A6DAE830164.implementation preserve=yes
      //## end stBaseElementKeyInfo%3A6DAE830164.implementation

};

//## begin stBaseElementKeyInfo%3A6DAE830164.postscript preserve=yes
//## end stBaseElementKeyInfo%3A6DAE830164.postscript

//## begin BaseElementKeyList%3A6DB2450118.preface preserve=yes
//## end BaseElementKeyList%3A6DB2450118.preface

//## Class: BaseElementKeyList%3A6DB2450118; Instantiated Class
//	This list saves the name/value of the base element keys. This is, the
//	identifiers for the base element to be parsed.
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A6DB4C40058;stBaseElementKeyInfo { -> }

typedef list< stBaseElementKeyInfo * > BaseElementKeyList;

//## begin BaseElementKeyList%3A6DB2450118.postscript preserve=yes
//## end BaseElementKeyList%3A6DB2450118.postscript

//## begin ClDMSSAXHandlers%3A3DFC4E01D2.preface preserve=yes
//## end ClDMSSAXHandlers%3A3DFC4E01D2.preface

//## Class: ClDMSSAXHandlers%3A3DFC4E01D2
//	This class allow us to parse a XML file through differents events send by
//	SAX parser. THis event are:
//
//	** startElement: new XML element arrived.
//
//	** endElement: end XML element.
//
//	** characters: value for XML element.
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A3F74B60277;string { -> }
//## Uses: <unnamed>%3A558D480046;ClDMSXmlException { -> F}
//## Uses: <unnamed>%3A5C24B2020A;M4ClTimeStamp { -> F}
//## Uses: <unnamed>%3AE6829B003E;M4XMLCh { -> }

class ClDMSSAXHandlers : public HandlerBase  //## Inherits: <unnamed>%3AE689E2014B
{
  //## begin ClDMSSAXHandlers%3A3DFC4E01D2.initialDeclarations preserve=yes
  //## end ClDMSSAXHandlers%3A3DFC4E01D2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDMSSAXHandlers%977216642
      ClDMSSAXHandlers (const string &ai_sXMLFile, const m4int32_t &ai_iFirstRegister, const m4int32_t &ai_iLastRegister);

    //## Destructor (specified)
      //## Operation: ~ClDMSSAXHandlers%977216643
      ~ClDMSSAXHandlers ();


    //## Other Operations (specified)
      //## Operation: Init%977216644
      m4return_t Init (ClAccess *ai_poSource);

      //## Operation: characters%977216647
      void characters(const XMLCh* const chars, const XMLSize_t length);
	  
      //## Operation: endElement%977216648
      void endElement (const XMLCh* const name);

      //## Operation: startElement%977216649
      void startElement (const XMLCh* const name, AttributeList& attributes);

      //## Operation: warning%978973600
      void warning (const SAXParseException& exception);

      //## Operation: error%978973601
      void error (const SAXParseException& exception);

      //## Operation: fatalError%978973602
      void fatalError (const SAXParseException& exception);

      //## Operation: resetErrors%978973603
      void resetErrors ();

      //## Operation: DumpMessage%978973604
      void DumpMessage (m4pcchar_t ai_pcMessage, const SAXParseException& exception);

      //## Operation: IsOk%977421355
      m4bool_t IsOk () const;

      //## Operation: GetError%977421356
      m4return_t GetError () const;

      //## Operation: SetError%977421357
      void SetError (m4return_t ai_iError = M4_ERROR);

      //## Operation: CheckParserStatus%978515139
      m4bool_t CheckParserStatus ();

      //## Operation: DeleteBaseElementBlock%978515140
      void DeleteBaseElementBlock ();

      //## Operation: DeleteChannelElementList%980239734
      void DeleteChannelElementList ();

      //## Operation: InitializeXmlMapM4O%979558714
      m4return_t InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency, m4bool_t ai_bItemValidation);

      //## Operation: ReleaseXmlMapM4O%979749961
      m4return_t ReleaseXmlMapM4O ();

      //## Operation: GetRootElementInfo%980239726
      m4return_t GetRootElementInfo ();

      //## Operation: GetBaseElementInfo%980239727
      m4return_t GetBaseElementInfo ();

      //## Operation: SetKeyBaseElementValue%980239732
      void SetKeyBaseElementValue (const string &ai_sKeyName, const string &ai_sKeyValue);

      //## Operation: GetKeyBaseElementInfo%980239736
      void GetKeyBaseElementInfo (string &ao_sKeyInfo);

      //## Operation: DeleteKeyBaseElementList%980239733
      void DeleteKeyBaseElementList ();

      //## Operation: ResetKeyBaseElementList%980239735
      void ResetKeyBaseElementList ();

	  m4return_t ValidateString(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, m4uint8_t ai_uiM4Type, const string &ai_sValue, m4VariantType &ao_vValue);

	  m4return_t ValidateDate(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, m4uint8_t ai_uiM4Type, const string &ai_sValue, m4VariantType &ao_vValue);

	  m4return_t ValidateNumber(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, const string &ai_sValue, m4VariantType &ao_vValue);

	  m4return_t ValidateVariant(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, const string &ai_sValue, m4VariantType &ao_vValue);

      m4return_t ConvertStringToDouble(const string &ai_sItemValue, m4double_t & ao_dDoubleValue);

      m4return_t ConvertStringToDate(const string &ai_sDateValue, m4uint8_t ai_uiItemM4Type, m4double_t & ao_dDateValue);
		
	  m4bool_t CheckCharacters(m4char_t * ai_pcText);

    // Additional Public Declarations
      //## begin ClDMSSAXHandlers%3A3DFC4E01D2.public preserve=yes
      //## end ClDMSSAXHandlers%3A3DFC4E01D2.public

  protected:

    //## Other Operations (specified)
      //## Operation: ProcessNodeElement%1094119909
      m4return_t ProcessNodeElement (const string &ai_sElementName, const string &ai_sXmlElementName, m4int8_t ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId);

      //## Operation: ProcessItemElement%1094119910
      m4return_t ProcessItemElement (const string &ai_sElementName, m4uint8_t &ao_uiCppType);

      //## Operation: ProcessNodeAndItemElement%1094119911
      m4return_t ProcessNodeAndItemElement (const string &ai_sElementName, const string &ai_sXmlElementName, ClNode *ai_poParentNode, m4int8_t ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId, m4uint8_t &ao_uiCppType);

      //## Operation: ProcessElementAttributes%1094119912
      m4return_t ProcessElementAttributes (const string &ai_sXmlElementName, AttributeList& attributes);

    // Additional Protected Declarations
      //## begin ClDMSSAXHandlers%3A3DFC4E01D2.protected preserve=yes
	  m4return_t GetXmlElementMapInfo(const string &ai_sXmlElementKey, const string &ai_sCurrentNode, m4uint32_t &ao_uiElementType, string &ao_sElementId, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod);
	  m4return_t GetXmlAttributeMapInfo(const string &ai_sXmlElementKey, const string &ai_sAttributeOf, const string &ai_sCurrentNode, string &ao_sElementId);
	  m4return_t CheckDeleteRegister(stChannelElementInfo * ai_poNodeInfo, m4bool_t & ao_bDeleteRegister);
	  m4return_t AddProcessedItem(const string & ai_sXMLElementName);
	  m4return_t AddProcessedAttributes(const string & ai_sXmlElementName, AttributeList& attributes);
      //## end ClDMSSAXHandlers%3A3DFC4E01D2.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_sXMLFile%3A3DFEC5008F
      //## begin ClDMSSAXHandlers::m_sXMLFile%3A3DFEC5008F.attr preserve=no  private:  string {UA} ai_sXMLFile
       string m_sXMLFile;
      //## end ClDMSSAXHandlers::m_sXMLFile%3A3DFEC5008F.attr

      //## Attribute: m_bIsOk%3A4241AD000F
      //## begin ClDMSSAXHandlers::m_bIsOk%3A4241AD000F.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bIsOk;
      //## end ClDMSSAXHandlers::m_bIsOk%3A4241AD000F.attr

      //## Attribute: m_iError%3A42427D0339
      //## begin ClDMSSAXHandlers::m_iError%3A42427D0339.attr preserve=no  private: m4return_t {UA} M4_SUCCESS
      m4return_t m_iError;
      //## end ClDMSSAXHandlers::m_iError%3A42427D0339.attr

      //## Attribute: m_sXmlBaseElement%3A431CA302B0
      //## begin ClDMSSAXHandlers::m_sXmlBaseElement%3A431CA302B0.attr preserve=no  private: string {UA} 
      string m_sXmlBaseElement;
      //## end ClDMSSAXHandlers::m_sXmlBaseElement%3A431CA302B0.attr

      //## Attribute: m_iFirstRegister%3A5462220209
      //## begin ClDMSSAXHandlers::m_iFirstRegister%3A5462220209.attr preserve=no  private: m4int32_t {UA} ai_iFirstRegister
      m4int32_t m_iFirstRegister;
      //## end ClDMSSAXHandlers::m_iFirstRegister%3A5462220209.attr

      //## Attribute: m_iLastRegister%3A5462540279
      //## begin ClDMSSAXHandlers::m_iLastRegister%3A5462540279.attr preserve=no  private: m4int32_t {UA} ai_iLastRegister
      m4int32_t m_iLastRegister;
      //## end ClDMSSAXHandlers::m_iLastRegister%3A5462540279.attr

      //## Attribute: m_iCurrentRegister%3A5489AF0353
      //## begin ClDMSSAXHandlers::m_iCurrentRegister%3A5489AF0353.attr preserve=no  private: m4int32_t {UA} -1
      m4int32_t m_iCurrentRegister;
      //## end ClDMSSAXHandlers::m_iCurrentRegister%3A5489AF0353.attr

      //## Attribute: m_bInException%3A5A018502CC
      //## begin ClDMSSAXHandlers::m_bInException%3A5A018502CC.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bInException;
      //## end ClDMSSAXHandlers::m_bInException%3A5A018502CC.attr

      //## Attribute: m_sRootNodeXmlName%3A76A40E037E
      //## begin ClDMSSAXHandlers::m_sRootNodeXmlName%3A76A40E037E.attr preserve=no  private: string {UA} 
      string m_sRootNodeXmlName;
      //## end ClDMSSAXHandlers::m_sRootNodeXmlName%3A76A40E037E.attr

      //## Attribute: m_sRootNodeId%3A76A42A0003
      //## begin ClDMSSAXHandlers::m_sRootNodeId%3A76A42A0003.attr preserve=no  private: string {UA} 
      string m_sRootNodeId;
      //## end ClDMSSAXHandlers::m_sRootNodeId%3A76A42A0003.attr

      //## Attribute: m_sBaseElementId%3A9F9A38010E
      //## begin ClDMSSAXHandlers::m_sBaseElementId%3A9F9A38010E.attr preserve=no  private: string {UA} 
      string m_sBaseElementId;
      //## end ClDMSSAXHandlers::m_sBaseElementId%3A9F9A38010E.attr

      //## Attribute: m_bRootXMLElementAlreadyParsed%422598B403C3
      //## begin ClDMSSAXHandlers::m_bRootXMLElementAlreadyParsed%422598B403C3.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bRootXMLElementAlreadyParsed;
      //## end ClDMSSAXHandlers::m_bRootXMLElementAlreadyParsed%422598B403C3.attr

      //## Attribute: m_sImportChannelName%422598EC024D
      //## begin ClDMSSAXHandlers::m_sImportChannelName%422598EC024D.attr preserve=no  private: string {UA} 
      string m_sImportChannelName;
      //## end ClDMSSAXHandlers::m_sImportChannelName%422598EC024D.attr
		
    // Data Members for Associations

      //## Association: M4DMSXml::<unnamed>%3A3E558F0390
      //## Role: ClDMSSAXHandlers::m_poCurrentNode%3A3E559101A8
      //## begin ClDMSSAXHandlers::m_poCurrentNode%3A3E559101A8.role preserve=no  private: ClNode { -> RFHAN}
      ClNode *m_poCurrentNode;
      //## end ClDMSSAXHandlers::m_poCurrentNode%3A3E559101A8.role

      //## Association: M4DMSXml::<unnamed>%3A3F36B502E5
      //## Role: ClDMSSAXHandlers::m_poImportAccess%3A3F36B70021
      //## begin ClDMSSAXHandlers::m_poImportAccess%3A3F36B70021.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poImportAccess;
      //## end ClDMSSAXHandlers::m_poImportAccess%3A3F36B70021.role

      //## Association: M4DMSXml::<unnamed>%3A3F8C140291
      //## Role: ClDMSSAXHandlers::m_oChannelElementList%3A3F8C150170
      //## begin ClDMSSAXHandlers::m_oChannelElementList%3A3F8C150170.role preserve=no  private: ElementHandlerList { -> VHAN}
      ElementHandlerList m_oChannelElementList;
      //## end ClDMSSAXHandlers::m_oChannelElementList%3A3F8C150170.role

      //## Association: M4DMSXml::<unnamed>%3A6709B10110
      //## Role: ClDMSSAXHandlers::m_poXmlMap%3A6709B20017
      //## begin ClDMSSAXHandlers::m_poXmlMap%3A6709B20017.role preserve=no  private: ClDMSXmlM4OMap { -> RFHAN}
      ClDMSXmlM4OMap *m_poXmlMap;
      //## end ClDMSSAXHandlers::m_poXmlMap%3A6709B20017.role

      //## Association: M4DMSXml::<unnamed>%3A6DB51E03B5
      //## Role: ClDMSSAXHandlers::m_oBaseElementKeyList%3A6DB51F01C1
      //## begin ClDMSSAXHandlers::m_oBaseElementKeyList%3A6DB51F01C1.role preserve=no  private: BaseElementKeyList { -> VHAN}
      BaseElementKeyList m_oBaseElementKeyList;
      //## end ClDMSSAXHandlers::m_oBaseElementKeyList%3A6DB51F01C1.role

      //## Association: M4DMSXml::<unnamed>%3A8A5D09035D
      //## Role: ClDMSSAXHandlers::m_poImportChannel%3A8A5D0B0084
      //## begin ClDMSSAXHandlers::m_poImportChannel%3A8A5D0B0084.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poImportChannel;
      //## end ClDMSSAXHandlers::m_poImportChannel%3A8A5D0B0084.role

      //## Association: M4DMSXml::<unnamed>%3A9A420A014B
      //## Role: ClDMSSAXHandlers::m_poChannelHandler%3A9A420A0386
      //## begin ClDMSSAXHandlers::m_poChannelHandler%3A9A420A0386.role preserve=no  private: ClDMSChannelHandler { -> RFHAN}
      ClDMSChannelHandler *m_poChannelHandler;
      //## end ClDMSSAXHandlers::m_poChannelHandler%3A9A420A0386.role

      //## Association: M4DMSXml::<unnamed>%3A9A449403C7
      //## Role: ClDMSSAXHandlers::m_poChannelManager%3A9A4495021A
      //## begin ClDMSSAXHandlers::m_poChannelManager%3A9A4495021A.role preserve=no  private: ClChannelManager { -> RFHAN}
      ClChannelManager *m_poChannelManager;
      //## end ClDMSSAXHandlers::m_poChannelManager%3A9A4495021A.role

    // Additional Private Declarations
      //## begin ClDMSSAXHandlers%3A3DFC4E01D2.private preserve=yes
	  string			m_sRecoveryElement;
	  m4bool_t			m_bElementConsistency;

	  // Item validation.
	  m4bool_t			m_bItemValidation;

      //## end ClDMSSAXHandlers%3A3DFC4E01D2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDMSSAXHandlers%3A3DFC4E01D2.implementation preserve=yes
      //## end ClDMSSAXHandlers%3A3DFC4E01D2.implementation

};

//## begin ClDMSSAXHandlers%3A3DFC4E01D2.postscript preserve=yes
//## end ClDMSSAXHandlers%3A3DFC4E01D2.postscript

// Class stChannelElementInfo 

// Class stBaseElementKeyInfo 


//## Other Operations (inline)
inline void stBaseElementKeyInfo::SetValue (string ai_sValue)
{
  //## begin stBaseElementKeyInfo::SetValue%980239729.body preserve=yes
	m_sValue = ai_sValue;
  //## end stBaseElementKeyInfo::SetValue%980239729.body
}

// Class ClDMSSAXHandlers 


//## Other Operations (inline)
inline m4bool_t ClDMSSAXHandlers::IsOk () const
{
  //## begin ClDMSSAXHandlers::IsOk%977421355.body preserve=yes
	return m_bIsOk;
  //## end ClDMSSAXHandlers::IsOk%977421355.body
}

inline m4return_t ClDMSSAXHandlers::GetError () const
{
  //## begin ClDMSSAXHandlers::GetError%977421356.body preserve=yes
	return m_iError;
  //## end ClDMSSAXHandlers::GetError%977421356.body
}

//## begin module%3A3E58540028.epilog preserve=yes
//## end module%3A3E58540028.epilog


#endif
