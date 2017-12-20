//## begin module%3A66B52E0160.cm preserve=no
//## end module%3A66B52E0160.cm

//## begin module%3A66B52E0160.cp preserve=no
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
//## end module%3A66B52E0160.cp

//## Module: m4dmsxmlm4omap%3A66B52E0160; Package specification
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Source file: C:\m4server\m4dmsxml\inc\m4dmsxmlm4omap.hpp

#ifndef m4dmsxmlm4omap_h
#define m4dmsxmlm4omap_h 1

//## begin module%3A66B52E0160.additionalIncludes preserve=no
//## end module%3A66B52E0160.additionalIncludes

//## begin module%3A66B52E0160.includes preserve=yes
#include <m4types.hpp>
//## end module%3A66B52E0160.includes

#include "m4stl.hpp"
// node_x
#include <node_x.hpp>

class ClChannel;
class ClAccess;
class ClDMSChannelHandler;

//## begin module%3A66B52E0160.declarations preserve=no
//## end module%3A66B52E0160.declarations

//## begin module%3A66B52E0160.additionalDeclarations preserve=yes
//## end module%3A66B52E0160.additionalDeclarations


//## begin ClDMSXmlM4OMap%3A66AF770301.preface preserve=yes
//## end ClDMSXmlM4OMap%3A66AF770301.preface

//## Class: ClDMSXmlM4OMap%3A66AF770301
//## Category: M4DMSXml%3A3DFB720294
//## Subsystem: M4DMSXml::inc%3A3E5D200088
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3A66CF7900D3;string { -> }
//## Uses: <unnamed>%3A66D3CD0285;ClNode { -> }
//## Uses: <unnamed>%3A6737BF000F;ClFileName { -> F}

class ClDMSXmlM4OMap 
{
  //## begin ClDMSXmlM4OMap%3A66AF770301.initialDeclarations preserve=yes
  //## end ClDMSXmlM4OMap%3A66AF770301.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDMSXmlM4OMap%979749964
      ClDMSXmlM4OMap (ClDMSChannelHandler *ai_poChannelHandler);

    //## Destructor (specified)
      //## Operation: ~ClDMSXmlM4OMap%979749965
      ~ClDMSXmlM4OMap ();


    //## Other Operations (specified)
      //## Operation: InitializeXmlMapM4O%979749966
      m4return_t InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const string &ai_sChannelName, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency);

      //## Operation: ReleaseXmlMapM4O%979749967
      m4return_t ReleaseXmlMapM4O ();

      //## Operation: GetXmlElementMapInfo%979749968
      m4return_t GetXmlElementMapInfo (const string &ai_sXmlElementKey, const string &ai_sCurrentNode, m4uint32_t &ao_uiElementType, string &ao_sElementId, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod);

	  //## Operation: GetXmlAttributeMapInfo%979749969
      m4return_t GetXmlAttributeMapInfo (const string &ai_sXmlElementKey, const string &ai_sAttributeOf, const string &ai_sCurrentNode, string &ao_sElementId);

      //## Operation: GetRootElementInfo%980239724
      m4return_t GetRootElementInfo (string &ao_sXmlRootElementName, string &ao_sRootNodeId);

      //## Operation: GetBaseElementInfo%980239725
      m4return_t GetBaseElementInfo (string &ao_sXmlBaseElementName, string &ao_sBaseElementKey);

      //## Operation: GetDtdFileName%982251402
      m4return_t GetDtdFileName (string &ao_sDtdFileName);

      //## Operation: LoadAttributesOfElement%982575345
      m4return_t LoadAttributesOfElement (const string &ai_sCurrentNodeName, const string &ai_sAttributeOf, string &ao_sListHandle);

      //## Operation: LoadSubElementsOfElement%982575346
      m4return_t LoadSubElementsOfElement (const string &ai_sCurrentNodeName, string &ao_sSubElemListHandle, m4uint32_t &ao_sXmlStructType);

      //## Operation: GetSubElementInfo%982575347
	  m4return_t GetSubElementInfo (const string &ai_sSubElemListHandle, const m4bool_t &ai_bXmlInOrder, string &ao_sElementId, m4uint32_t &ao_iElementType, string &ao_sXmlElementName, m4bool_t & ao_bIsNodeValue, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment,string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod);
      
      //## Operation: GetAttributeInfo%982575349
      m4return_t GetAttributeInfo (const string &ai_sAttributeListHandle, string &ao_sElementId, string &ao_sXmlElemenName, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment);

      //## Operation: CheckItemsScope%1109161577
      //	Check if all items scope within a given node is register or there are items
      //	with node or block scope.
      m4return_t CheckItemsScope (const string &ai_sCurrentNodeId, m4int32_t &aio_iItemsScopeRegister, m4int32_t &aio_iItemsScopeBlockNode);

      //## Operation: CheckNodesConsistency%1140186372
      m4return_t CheckNodesConsistency ();

    // Additional Public Declarations
      //## begin ClDMSXmlM4OMap%3A66AF770301.public preserve=yes
      //## end ClDMSXmlM4OMap%3A66AF770301.public

  protected:
    // Additional Protected Declarations
      //## begin ClDMSXmlM4OMap%3A66AF770301.protected preserve=yes
      //## end ClDMSXmlM4OMap%3A66AF770301.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bLoadByChannelHandle%4295AB3E0249
      //## begin ClDMSXmlM4OMap::m_bLoadByChannelHandle%4295AB3E0249.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bLoadByChannelHandle;
      //## end ClDMSXmlM4OMap::m_bLoadByChannelHandle%4295AB3E0249.attr

    // Data Members for Associations

      //## Association: M4DMSXml::<unnamed>%3A66D5A0031C
      //## Role: ClDMSXmlM4OMap::m_poXmlMapChannel%3A66D5A102E2
      //## begin ClDMSXmlM4OMap::m_poXmlMapChannel%3A66D5A102E2.role preserve=no  private: ClChannel { -> RFHAN}
      ClChannel *m_poXmlMapChannel;
      //## end ClDMSXmlM4OMap::m_poXmlMapChannel%3A66D5A102E2.role

      //## Association: M4DMSXml::<unnamed>%3A66D66E0192
      //## Role: ClDMSXmlM4OMap::m_poXmlMapAccess%3A66D66F0035
      //## begin ClDMSXmlM4OMap::m_poXmlMapAccess%3A66D66F0035.role preserve=no  private: ClAccess { -> RFHAN}
      ClAccess *m_poXmlMapAccess;
      //## end ClDMSXmlM4OMap::m_poXmlMapAccess%3A66D66F0035.role

      //## Association: M4DMSXml::<unnamed>%3A9A3A710299
      //## Role: ClDMSXmlM4OMap::m_poChannelHandler%3A9A3A720042
      //## begin ClDMSXmlM4OMap::m_poChannelHandler%3A9A3A720042.role preserve=no  private: ClDMSChannelHandler { -> RFHAN}
      ClDMSChannelHandler *m_poChannelHandler;
      //## end ClDMSXmlM4OMap::m_poChannelHandler%3A9A3A720042.role

    // Additional Private Declarations
      //## begin ClDMSXmlM4OMap%3A66AF770301.private preserve=yes
	  m4bool_t m_bElementConsistency;
      //## end ClDMSXmlM4OMap%3A66AF770301.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDMSXmlM4OMap%3A66AF770301.implementation preserve=yes
      //## end ClDMSXmlM4OMap%3A66AF770301.implementation

};

//## begin ClDMSXmlM4OMap%3A66AF770301.postscript preserve=yes
//## end ClDMSXmlM4OMap%3A66AF770301.postscript

// Class ClDMSXmlM4OMap 

//## begin module%3A66B52E0160.epilog preserve=yes
//## end module%3A66B52E0160.epilog


#endif
