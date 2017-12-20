//## begin module%3A8A6ED9025F.cm preserve=no
//## end module%3A8A6ED9025F.cm

//## begin module%3A8A6ED9025F.cp preserve=no
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
//## end module%3A8A6ED9025F.cp

//## Module: m4dmsm4objecttoxml%3A8A6ED9025F; Package specification
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Source file: C:\m4server\m4dmsxml\inc\m4dmsm4objecttoxml.hpp

#ifndef m4dmsm4objecttoxml_h
#define m4dmsm4objecttoxml_h 1

//## begin module%3A8A6ED9025F.additionalIncludes preserve=no
//## end module%3A8A6ED9025F.additionalIncludes

//## begin module%3A8A6ED9025F.includes preserve=yes
struct stElementInfo;
//## end module%3A8A6ED9025F.includes

#include <m4dmsxmldefines.hpp>
#include "m4stl.hpp"
// m4date
#include <m4date.hpp>

class ClChannel;
class ClAccess;
class ClDMSXmlM4OMap;
typedef list< stElementInfo * > ElementAttributesList;
typedef list< stElementInfo * > NodeSubElementList;
struct stElementInfo;
class ClDMSChannelHandler;

//## begin module%3A8A6ED9025F.declarations preserve=no
//## end module%3A8A6ED9025F.declarations

//## begin module%3A8A6ED9025F.additionalDeclarations preserve=yes
//## end module%3A8A6ED9025F.additionalDeclarations


//## begin ClDMSM4ObjectToXml%3A8A561000FD.preface preserve=yes
//## end ClDMSM4ObjectToXml%3A8A561000FD.preface

//## Class: ClDMSM4ObjectToXml%3A8A561000FD
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A94F31902BD;string { -> }
//## Uses: <unnamed>%3AA7CC6903B9;M4ClTimeStamp { -> }
//## Uses: <unnamed>%3AAC94970349;ofstream { -> }

class ClDMSM4ObjectToXml 
{
  //## begin ClDMSM4ObjectToXml%3A8A561000FD.initialDeclarations preserve=yes
  //## end ClDMSM4ObjectToXml%3A8A561000FD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDMSM4ObjectToXml%982251392
      ClDMSM4ObjectToXml (const m4bool_t &ai_bDoValidation, const string &ai_sValidationFilePath, const m4bool_t ai_bUseSchema, const string &ai_sSchemaNamespace);

    //## Destructor (specified)
      //## Operation: ~ClDMSM4ObjectToXml%982251393
      ~ClDMSM4ObjectToXml ();


    //## Other Operations (specified)
      //## Operation: InitEnv%982251394
      m4return_t InitEnv (ClAccess *ai_poSource);

      //## Operation: InitializeXmlMapM4O%982251395
      m4return_t InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency);

      //## Operation: ReleaseEnv%982251397
      m4return_t ReleaseEnv ();

      //## Operation: StartExportProcess%982251399
      m4return_t StartExportProcess (const string &ai_sXMLFile, const m4bool_t ai_bDoPublic, eUniXmlEncodingType_t ai_eEncoding);

      //## Operation: ProcessElement%982251403
      m4return_t ProcessElement (const string &ai_sCurrentNodeName, const m4int32_t &ai_iInstanceNumber, const string &ai_sXmlElementName, const string &ai_sElementName, const m4int32_t &ai_iElementType, const m4int32_t &ai_iTabNumber, m4bool_t ai_bDoJustBlockNodeItems, m4bool_t ai_bIsNodeValue, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bIsComment, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding);

      //## Operation: ProcessNodeElement%984388355
      m4return_t ProcessNodeElement (const string &ai_sXmlElementName, const string &ai_sElementName, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const m4int32_t &ai_iTabNumber, m4bool_t ai_bXmlRootNode, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding);

      //## Operation: ProcessItemElement%984388356
      m4return_t ProcessItemElement (const string &ai_sXmlElementName, const string &ai_sElementName, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const m4int32_t &ai_iInstanceNumber, m4pchar_t ai_pcIndent, m4bool_t ai_bOnlyBlockNodeScopeItems, m4bool_t ai_bIsNodeValue, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bIsComment, eUniXmlEncodingType_t ai_eEncoding);
    // Additional Public Declarations
      //## begin ClDMSM4ObjectToXml%3A8A561000FD.public preserve=yes
      //## end ClDMSM4ObjectToXml%3A8A561000FD.public

  protected:
    // Additional Protected Declarations
      //## begin ClDMSM4ObjectToXml%3A8A561000FD.protected preserve=yes
      //## end ClDMSM4ObjectToXml%3A8A561000FD.protected

  private:

    //## Other Operations (specified)
      //## Operation: AddXmlHeader%982251400
      m4return_t AddXmlHeader (const string &ai_sXmlRootElementName, const m4bool_t ai_bDoPublic, eUniXmlEncodingType_t ai_eEncoding);

      //## Operation: ProcessRegister%1123661605
      m4return_t ProcessRegister (const m4int32_t &ai_iRegister, const m4int32_t &ai_iTabNumber, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const string &ai_sXmlElementName, const string &ai_sElementName, m4bool_t ai_bXmlRootNode, m4bool_t ai_bOnlyBlockNodeScopeItems, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding);

      //## Operation: LoadAttributesOfElement%982251404
      m4return_t LoadAttributesOfElement (const string &ai_sCurrentNodeName, const string &ai_sAttributeOf, string &ao_sListHandle);

      //## Operation: LoadSubElementsOfElement%982575342
      m4return_t LoadSubElementsOfElement (const string &ai_sCurrentNodeName, string &ao_sSubElemListHandle, m4uint32_t &ao_sXmlStructType);

      //## Operation: GetSubElementInfo%982575344
      m4return_t GetSubElementInfo (const string &ai_sSubElemListHandle, const m4bool_t &ai_bXmlInOrder, string &ao_sElementId, m4uint32_t &ao_iElementType, string &ao_sXmlElementName, m4bool_t & ao_bIsNodeValue, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod);

      //## Operation: GetAttributeInfo%982575348
      m4return_t GetAttributeInfo (const string &ai_sAttributeListHandle, string &ao_sElementId, string &ao_sXmlElementName, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment);

      //## Operation: BuildAttXmlData%982575341
      m4return_t BuildAttXmlData (ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const string &ai_sElementName, const string &ai_sXmlElementName, const m4uint32_t &ai_iElementType, m4pcchar_t ai_pcIndent, m4bool_t ai_bXmlRootNode, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bOnlyBlockNodeScopeItems, eUniXmlEncodingType_t ai_eEncoding, string &ao_sXmlAttData);

      //## Operation: BuildXMLSchemaInformation%1120050060
      m4return_t BuildXMLSchemaInformation (string &ao_sSchemaInformation);

      //## Operation: BuildWellFormedXmlData%982575343
      void BuildWellFormedXmlData (const string &ai_sXmlData, eUniXmlEncodingType_t ai_eEncoding, string &ao_sWellFormedXmlData);

      //## Operation: EncloseAttXmlData%985938330
      void EncloseAttXmlData (const string &ai_sXmlData, string &ao_sWellFormedXmlData);

      //## Operation: FindSubElementInList%988212319
      stElementInfo * FindSubElementInList (const m4uint32_t &ai_uiElementHandle);

      //## Operation: FindAttributeInList%988618089
      stElementInfo * FindAttributeInList (const m4uint32_t &ai_uiElementHandle);

      //## Operation: AddSubElementInfo%988212320
      void AddSubElementInfo (const string &ai_sNodeName, const m4uint32_t &ao_uiNodeHandle, const string &ai_sNodeKey, const m4uint32_t &ai_uiXmlStructType);

      //## Operation: AddAttributeInfo%988618088
      void AddAttributeInfo (const string &ai_sElementName, const m4uint32_t &ai_uiItemHandle, const string &ai_sElementKey);

      //## Operation: DeleteNodeSubElementList%988212321
      void DeleteNodeSubElementList ();

      //## Operation: DeleteNodeAttributesList%988212322
      void DeleteNodeAttributesList ();

    // Data Members for Class Attributes

      //## Attribute: m_sExportChannelName%3A8CF2CA0120
      //## begin ClDMSM4ObjectToXml::m_sExportChannelName%3A8CF2CA0120.attr preserve=no  private: string {UA} 
      string m_sExportChannelName;
      //## end ClDMSM4ObjectToXml::m_sExportChannelName%3A8CF2CA0120.attr

      //## Attribute: m_bDoValidation%3A8CFB87032F
      //## begin ClDMSM4ObjectToXml::m_bDoValidation%3A8CFB87032F.attr preserve=no  private: m4bool_t {UA} ai_bDoValidation
      m4bool_t m_bDoValidation;
      //## end ClDMSM4ObjectToXml::m_bDoValidation%3A8CFB87032F.attr

      //## Attribute: m_sXmlBaseElement%3A91355C02BD
      //## begin ClDMSM4ObjectToXml::m_sXmlBaseElement%3A91355C02BD.attr preserve=no  private: string {UA} 
      string m_sXmlBaseElement;
      //## end ClDMSM4ObjectToXml::m_sXmlBaseElement%3A91355C02BD.attr

      //## Attribute: m_sXmlRootElement%3A915B530010
      //## begin ClDMSM4ObjectToXml::m_sXmlRootElement%3A915B530010.attr preserve=no  private: string {UA} 
      string m_sXmlRootElement;
      //## end ClDMSM4ObjectToXml::m_sXmlRootElement%3A915B530010.attr

      //## Attribute: m_fXmlStreamData%3AAC92DA0245
      //## begin ClDMSM4ObjectToXml::m_fXmlStreamData%3AAC92DA0245.attr preserve=no  private: ofstream {UA} 
      ofstream m_fXmlStreamData;
      //## end ClDMSM4ObjectToXml::m_fXmlStreamData%3AAC92DA0245.attr

      //## Attribute: m_sXmlData%3AACB3FF03B8
      //## begin ClDMSM4ObjectToXml::m_sXmlData%3AACB3FF03B8.attr preserve=no  private: string {UA} 
      string m_sXmlData;
      //## end ClDMSM4ObjectToXml::m_sXmlData%3AACB3FF03B8.attr

      //## Attribute: m_bUseSchema%42C299A300F2
      //## begin ClDMSM4ObjectToXml::m_bUseSchema%42C299A300F2.attr preserve=no  private: m4bool_t {UA} ai_bUseSchema
      m4bool_t m_bUseSchema;
      //## end ClDMSM4ObjectToXml::m_bUseSchema%42C299A300F2.attr

      //## Attribute: m_sSchemaNamespace%42C299B3020C
      //## begin ClDMSM4ObjectToXml::m_sSchemaNamespace%42C299B3020C.attr preserve=no  private: string {UA} 
      string m_sSchemaNamespace;
      //## end ClDMSM4ObjectToXml::m_sSchemaNamespace%42C299B3020C.attr

      //## Attribute: m_sValidationFilePath%42C299BE02F7
      //## begin ClDMSM4ObjectToXml::m_sValidationFilePath%42C299BE02F7.attr preserve=no  private: string {UA} 
      string m_sValidationFilePath;
      //## end ClDMSM4ObjectToXml::m_sValidationFilePath%42C299BE02F7.attr

    // Data Members for Associations

      //## Association: M4DMSXml::<unnamed>%3A8A5C9D00CC
      //## Role: ClDMSM4ObjectToXml::m_poExportAccess%3A8A5C9E02AF
      //## begin ClDMSM4ObjectToXml::m_poExportAccess%3A8A5C9E02AF.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poExportAccess;
      //## end ClDMSM4ObjectToXml::m_poExportAccess%3A8A5C9E02AF.role

      //## Association: M4DMSXml::<unnamed>%3A8A5E040340
      //## Role: ClDMSM4ObjectToXml::m_poExportChannel%3A8A5E050270
      //## begin ClDMSM4ObjectToXml::m_poExportChannel%3A8A5E050270.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poExportChannel;
      //## end ClDMSM4ObjectToXml::m_poExportChannel%3A8A5E050270.role

      //## Association: M4DMSXml::<unnamed>%3A8A5E97017E
      //## Role: ClDMSM4ObjectToXml::m_poXmlMap%3A8A5E98018B
      //## begin ClDMSM4ObjectToXml::m_poXmlMap%3A8A5E98018B.role preserve=no  private: ClDMSXmlM4OMap { -> RFHAN}
      ClDMSXmlM4OMap *m_poXmlMap;
      //## end ClDMSM4ObjectToXml::m_poXmlMap%3A8A5E98018B.role

      //## Association: M4DMSXml::<unnamed>%3A8A6CB002C8
      //## Role: ClDMSM4ObjectToXml::m_poChannelHandler%3A8A6CB10068
      //## begin ClDMSM4ObjectToXml::m_poChannelHandler%3A8A6CB10068.role preserve=no  private: ClDMSChannelHandler { -> RFHAN}
      ClDMSChannelHandler *m_poChannelHandler;
      //## end ClDMSM4ObjectToXml::m_poChannelHandler%3A8A6CB10068.role

      //## Association: M4DMSXml::<unnamed>%3AE6E9BE03D3
      //## Role: ClDMSM4ObjectToXml::m_oNodeSubElementList%3AE6E9BF0243
      //## begin ClDMSM4ObjectToXml::m_oNodeSubElementList%3AE6E9BF0243.role preserve=no  private: NodeSubElementList { -> VFHAN}
      NodeSubElementList m_oNodeSubElementList;
      //## end ClDMSM4ObjectToXml::m_oNodeSubElementList%3AE6E9BF0243.role

      //## Association: M4DMSXml::<unnamed>%3AE80095035E
      //## Role: ClDMSM4ObjectToXml::m_oAttributesList%3AE8009700FE
      //## begin ClDMSM4ObjectToXml::m_oAttributesList%3AE8009700FE.role preserve=no  private: ElementAttributesList { -> VFHAN}
      ElementAttributesList m_oAttributesList;
      //## end ClDMSM4ObjectToXml::m_oAttributesList%3AE8009700FE.role

	  m4pcchar_t m_pccDateSeparator;

    // Additional Private Declarations
      //## begin ClDMSM4ObjectToXml%3A8A561000FD.private preserve=yes
	  m4bool_t		IgnoreNullOrEmptyValues(m4VariantType vItemValue, m4bool_t ai_bIgnoreEmptyValues);
	  //## end ClDMSM4ObjectToXml%3A8A561000FD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDMSM4ObjectToXml%3A8A561000FD.implementation preserve=yes
      //## end ClDMSM4ObjectToXml%3A8A561000FD.implementation

};

//## begin ClDMSM4ObjectToXml%3A8A561000FD.postscript preserve=yes
//## end ClDMSM4ObjectToXml%3A8A561000FD.postscript

//## begin stElementInfo%3AE6DB7E005C.preface preserve=yes
//## end stElementInfo%3AE6DB7E005C.preface

//## Class: stElementInfo%3AE6DB7E005C
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct stElementInfo 
{
  //## begin stElementInfo%3AE6DB7E005C.initialDeclarations preserve=yes
  //## end stElementInfo%3AE6DB7E005C.initialDeclarations

    //## Constructors (specified)
      //## Operation: stElementInfo%988212316
      stElementInfo (const string &ai_sElementName, const m4uint32_t &ai_uiElementHandle, const string &ai_sElementKey, const m4uint32_t &ai_uiXmlStructType);


    //## Other Operations (specified)
      //## Operation: GetElementInfo%988212318
      void GetElementInfo (string &ao_sNodeKey, m4uint32_t &ao_XmlStructType);

    // Data Members for Class Attributes

      //## Attribute: m_sElementName%3AE6E3CE0002
      //## begin stElementInfo::m_sElementName%3AE6E3CE0002.attr preserve=no  public: string {UA} ai_sElementName
      string m_sElementName;
      //## end stElementInfo::m_sElementName%3AE6E3CE0002.attr

	  m4uint32_t m_uiElementHandle;

      //## Attribute: m_sElementKey%3AE6E3D400F1
      //## begin stElementInfo::m_sElementKey%3AE6E3D400F1.attr preserve=no  public: string {UA} ai_sElementKey
      string m_sElementKey;
      //## end stElementInfo::m_sElementKey%3AE6E3D400F1.attr

      //## Attribute: m_uiXmlStructType%3AE6F2EB01E6
      //## begin stElementInfo::m_uiXmlStructType%3AE6F2EB01E6.attr preserve=no  public: m4uint32_t {UA} ai_uiXmlStructType
      m4uint32_t m_uiXmlStructType;
      //## end stElementInfo::m_uiXmlStructType%3AE6F2EB01E6.attr

  public:
    // Additional Public Declarations
      //## begin stElementInfo%3AE6DB7E005C.public preserve=yes
      //## end stElementInfo%3AE6DB7E005C.public

  protected:
    // Additional Protected Declarations
      //## begin stElementInfo%3AE6DB7E005C.protected preserve=yes
      //## end stElementInfo%3AE6DB7E005C.protected

  private:
    // Additional Private Declarations
      //## begin stElementInfo%3AE6DB7E005C.private preserve=yes
      //## end stElementInfo%3AE6DB7E005C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin stElementInfo%3AE6DB7E005C.implementation preserve=yes
      //## end stElementInfo%3AE6DB7E005C.implementation

};

//## begin stElementInfo%3AE6DB7E005C.postscript preserve=yes
//## end stElementInfo%3AE6DB7E005C.postscript

//## begin NodeSubElementList%3AE6E804020A.preface preserve=yes
//## end NodeSubElementList%3AE6E804020A.preface

//## Class: NodeSubElementList%3AE6E804020A; Instantiated Class
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3AE706EF000D;stElementInfo { -> F}

typedef list< stElementInfo * > NodeSubElementList;

//## begin NodeSubElementList%3AE6E804020A.postscript preserve=yes
//## end NodeSubElementList%3AE6E804020A.postscript

//## begin ElementAttributesList%3AE8001A03D9.preface preserve=yes
//## end ElementAttributesList%3AE8001A03D9.preface

//## Class: ElementAttributesList%3AE8001A03D9; Instantiated Class
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3FC5EDFC02FC;stElementInfo { -> }

typedef list< stElementInfo * > ElementAttributesList;

//## begin ElementAttributesList%3AE8001A03D9.postscript preserve=yes
//## end ElementAttributesList%3AE8001A03D9.postscript

// Class ClDMSM4ObjectToXml 

// Class stElementInfo 

//## begin module%3A8A6ED9025F.epilog preserve=yes
//## end module%3A8A6ED9025F.epilog


#endif
