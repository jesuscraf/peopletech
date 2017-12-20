//## begin module%45F80154012C.cm preserve=no
//## end module%45F80154012C.cm

//## begin module%45F80154012C.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
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
//## end module%45F80154012C.cp

//## Module: m4mdtsaxmerger%45F80154012C; Package body
//## Subsystem: M4MDTransfer::src%3A3E5D2D0159
//## Source file: C:\m4server\m4mdtransfer\src\m4mdtsaxmerger.cpp

//## begin module%45F80154012C.additionalIncludes preserve=no
//## end module%45F80154012C.additionalIncludes

//## begin module%45F80154012C.includes preserve=yes
#include "m4mdtsaxmerger.hpp"
#include "m4mdtres.hpp"
#include "m4imdtservice.hpp"
#include "m4unicode.hpp"
#include <m4xmlbase.hpp>

#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"

//## end module%45F80154012C.includes

class M4XMLStreamGenerator;

//## begin module%45F80154012C.declarations preserve=no
//## end module%45F80154012C.declarations

//## begin module%45F80154012C.additionalDeclarations preserve=yes
//## end module%45F80154012C.additionalDeclarations


// Class ClMDTSAXMerger 









ClMDTSAXMerger::ClMDTSAXMerger ()
  //## begin ClMDTSAXMerger::ClMDTSAXMerger%1173884848.hasinit preserve=no
      : m_bIsMultiline(M4_FALSE), m_poService(NULL), m_poOutput(NULL)
  //## end ClMDTSAXMerger::ClMDTSAXMerger%1173884848.hasinit
  //## begin ClMDTSAXMerger::ClMDTSAXMerger%1173884848.initialization preserve=yes
  //## end ClMDTSAXMerger::ClMDTSAXMerger%1173884848.initialization
{
  //## begin ClMDTSAXMerger::ClMDTSAXMerger%1173884848.body preserve=yes
  //## end ClMDTSAXMerger::ClMDTSAXMerger%1173884848.body
}


ClMDTSAXMerger::~ClMDTSAXMerger ()
{
  //## begin ClMDTSAXMerger::~ClMDTSAXMerger%1173884849.body preserve=yes
	// Delete dictionary "logical-fields" table data.
	MDTDOMTraverserMap_t::iterator it;
	ClMDTDomTraverser * poDomTraverser = NULL;
	it = m_oDicLogicFieldsMap.begin();
	while (it != m_oDicLogicFieldsMap.end())
	{
		poDomTraverser = (*it).second;
		if (poDomTraverser != NULL)
		{
			delete poDomTraverser;
		}
		it++;
	}
	m_oDicLogicFieldsMap.clear();

	if (m_poOutput != NULL)
	{
		delete m_poOutput;
	}

	m_oOutStream.close();

	m_oPKFieldsMap.clear();
  //## end ClMDTSAXMerger::~ClMDTSAXMerger%1173884849.body
}



//## Other Operations (implementation)
m4return_t ClMDTSAXMerger::Init (IMDTService* ai_poService, const string &ai_sXMLExclusionFileName, const string &ai_sXMLMergedFileName)
{
  //## begin ClMDTSAXMerger::Init%1173884850.body preserve=yes
	m4return_t		iResult = M4_SUCCESS;
	FILE			* pfFile = NULL;

	m_poService = ai_poService;

	// Check if the exclusion file exists.
	M4MDT_RETURN_ERROR2(ai_sXMLExclusionFileName.empty() == M4_TRUE, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, ai_sXMLExclusionFileName.c_str(), strerror(errno), "%s#%s");

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen(ai_sXMLExclusionFileName.c_str(), M4UniRead, eEncoding);
	M4MDT_RETURN_ERROR2(pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, ai_sXMLExclusionFileName.c_str(), strerror(errno), "%s#%s");
	fclose(pfFile);

	// UNICODE FILE
	eEncoding = M4UniANSI ;
	M4Open( m_oOutStream, ai_sXMLMergedFileName.c_str(), M4UniWriteBinary, eEncoding ) ;
	M4MDT_RETURN_ERROR1(m_oOutStream.is_open() == 0, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, ai_sXMLMergedFileName.c_str(), "%s");

	m_poOutput = new M4Utf8XMLStreamGenerator(m_oOutStream);
	M4MDT_RETURN_ERROR1(m_poOutput == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(M4XMLStreamGenerator), "%d");

	// Methods registration: start. No registration for end.
	RegisterElementProc("object-data",(ElementProcStart)&ClMDTSAXMerger::_ProcObjectData);
	RegisterElementProc("param-value",(ElementProcStart)&ClMDTSAXMerger::_ProcParamValue);
	RegisterElementProc("table",(ElementProcStart)&ClMDTSAXMerger::_ProcTable);
	RegisterElementProc("register",(ElementProcStart)&ClMDTSAXMerger::_ProcRegister);
	RegisterElementProc("field",(ElementProcStart)&ClMDTSAXMerger::_ProcField);
	
	// Methods registration: end
	RegisterElementEndProc("object-data",(ElementProcEnd)&ClMDTSAXMerger::_ProcEndComplexElement);
	RegisterElementEndProc("param-value",(ElementProcEnd)&ClMDTSAXMerger::_ProcEndSimpleElement);
	RegisterElementEndProc("table",(ElementProcEnd)&ClMDTSAXMerger::_ProcEndComplexElement);
	RegisterElementEndProc("register",(ElementProcEnd)&ClMDTSAXMerger::_ProcEndRegister);
	RegisterElementEndProc("field",(ElementProcEnd)&ClMDTSAXMerger::_ProcEndSimpleElement);

	// Load exclusion XML file.
	iResult = _LoadExclusionData(ai_sXMLExclusionFileName);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::Init", "_LoadExclusionData", "%s#%s");
	
	return M4_SUCCESS;
  //## end ClMDTSAXMerger::Init%1173884850.body
}

void ClMDTSAXMerger::startDocument ()
{
  //## begin ClMDTSAXMerger::startDocument%1173884851.body preserve=yes
	m_poOutput->MakeXMLDeclaration();
  //## end ClMDTSAXMerger::startDocument%1173884851.body
}

void ClMDTSAXMerger::endDocument ()
{
  //## begin ClMDTSAXMerger::endDocument%1173884852.body preserve=yes
	m_oOutStream.close();
  //## end ClMDTSAXMerger::endDocument%1173884852.body
}

void ClMDTSAXMerger::characters (const XMLCh* const chars, const XMLSize_t length)
{
  //## begin ClMDTSAXMerger::characters%1173884853.body preserve=yes
	m4return_t	iResult = M4_SUCCESS;
	string		sElementValue;

	// Check multiline attribute set for multiline fields.
	if (m_bIsMultiline == M4_FALSE)
	{
		return;
	}

	// Multiline field value always begin with a prefix "\n#STARTVALUE#\n" and end with 
	// the suffix "\n#ENDVALUE#\n".
	ClCharHolder pcText(chars);
	if (strnicmp(pcText, M4MDT_XML_CDATA_PREFIX, M4MDT_XML_CDATA_PREFIX_LENGTH) != 0)
	{
		return;
	}
	
	// First of all, close element to add the element value.
	sElementValue = M4MD_XML_CDATA_START;
	sElementValue.append(pcText);
	sElementValue.append(M4MD_XML_CDATA_END);

	iResult = m_poOutput->AddElementValue(sElementValue.c_str());
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR5(iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::characters", "AddElementValue", "field", sElementValue.c_str(), "%s#%s - Element name: <%s> - Element value <%s>");
		SetError(M4_ERROR);
	}
  //## end ClMDTSAXMerger::characters%1173884853.body
}

m4void_t ClMDTSAXMerger::_ProcObjectData (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXMerger::_ProcObjectData%1173884857.body preserve=yes
	m4return_t	iResult = M4_SUCCESS;
	m4bool_t	bCloseElement = M4_FALSE;

	iResult = _DumpElement(name, attributes, bCloseElement);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcObjectData", "_DumpElement", "%s#%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTSAXMerger::_ProcObjectData%1173884857.body
}

m4void_t ClMDTSAXMerger::_ProcParamValue (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXMerger::_ProcParamValue%1174575090.body preserve=yes
	m4return_t	iResult = M4_SUCCESS;
	m4bool_t	bCloseElement = M4_FALSE;

	iResult = _DumpElement(name, attributes, bCloseElement);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcParamValue", "_DumpElement", "%s#%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTSAXMerger::_ProcParamValue%1174575090.body
}

m4void_t ClMDTSAXMerger::_ProcTable (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXMerger::_ProcTable%1173884859.body preserve=yes
	m4return_t		iResult = M4_SUCCESS;
	m4bool_t		bCloseElement = M4_FALSE;

	// Dump element.
	iResult = _DumpElement(name, attributes, bCloseElement);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcTable", "_DumpElement", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	// Go through all its attributes and check if it has an attribute called "value".
	// If it has an attribute "value", add a new attribute "caption" with the translated 
	// value.
	// Get name attribute.
	iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, attributes, m_sTableName);
	if(iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, IMDTService::sm_sXMLChName, "table", "%s#%s" ) ;
		SetError(M4_ERROR);
		return;
	}
  //## end ClMDTSAXMerger::_ProcTable%1173884859.body
}

m4void_t ClMDTSAXMerger::_ProcRegister (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXMerger::_ProcRegister%1174575089.body preserve=yes
	m4return_t		iResult = M4_SUCCESS;
	m4bool_t		bCloseElement = M4_FALSE;

	// Dump element.
	iResult = _DumpElement(name, attributes, bCloseElement);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcRegister", "_DumpElement", "%s#%s");
		SetError(M4_ERROR);
		return;
	}
  //## end ClMDTSAXMerger::_ProcRegister%1174575089.body
}

m4void_t ClMDTSAXMerger::_ProcField (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXMerger::_ProcField%1173884860.body preserve=yes
	m4return_t		iResult = M4_SUCCESS;
	string			sIsMultiline;
	string			sFieldName;
	string			sFieldValue;
	string			sIsNull;
	m4bool_t		bIsNull = M4_FALSE;
	m4bool_t		bIsPK = M4_FALSE;
	m4bool_t		bCloseElement = M4_FALSE;

	// Dump element.
	iResult = _DumpElement(name, attributes, bCloseElement);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcField", "_DumpElement", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	// Get "name" attribute.
	iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, attributes, sFieldName);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "name", "field", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	// Get "is-null" attribute. This attribute is optional, only sent if the field has
	// null value. In this case, "value" attribute will not be sent.
	// This is why we do not check returned value.
	iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChIsNull, attributes, sIsNull);
	
	// Check if the field value is null.
	if (iResult == M4_SUCCESS && sIsNull.compare("true") == 0)
	{
		bIsNull = M4_TRUE;
	}
	
	// Get "value" attribute only if is-null="true".
	if (bIsNull == M4_FALSE)
	{
		// Get "multiline" attribute. This attribute is optional, only sent if the field has
		// multiline values.
		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChMultiline, attributes, sIsMultiline);	
		if ((iResult == M4_SUCCESS) && (sIsMultiline.compare("true") == 0))
		{
			m_bIsMultiline = M4_TRUE;
		}
		else
		{
			// Get "value" attribute.
			iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChValue, attributes, sFieldValue);
			if (iResult != M4_SUCCESS)
			{
				M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "value", "field", "%s#%s");
				SetError(M4_ERROR);
				return;
			}
		}
	}

	// Check and save PK fields.
	iResult = _CheckIsPKField(m_sTableName, sFieldName, bIsPK);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcField", "_CheckPKField", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	if (bIsPK == M4_TRUE)
	{
		m_oPKFieldsMap.insert(fieldsMap_t::value_type(sFieldName, sFieldValue));
	}
  //## end ClMDTSAXMerger::_ProcField%1173884860.body
}

m4void_t ClMDTSAXMerger::_ProcEndComplexElement (const XMLCh * const name)
{
  //## begin ClMDTSAXMerger::_ProcEndComplexElement%1173884854.body preserve=yes
	ClCharHolder	pcXmlElementName(name);
	
	// Close element (>).
	m4return_t iResult = m_poOutput->CloseElement(pcXmlElementName);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcEndComplexElement", "CloseElement", pcXmlElementName, "%s#%s - Element name: <%s>" );
		SetError(M4_ERROR);
		return;
	}
  //## end ClMDTSAXMerger::_ProcEndComplexElement%1173884854.body
}

m4void_t ClMDTSAXMerger::_ProcEndSimpleElement (const XMLCh * const name)
{
  //## begin ClMDTSAXMerger::_ProcEndSimpleElement%1173884855.body preserve=yes
	ClCharHolder	pcXmlElementName(name);
	m4return_t		iResult = M4_SUCCESS;

	// </name>.
	iResult = m_poOutput->CloseElement(pcXmlElementName);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcEndSimpleElement", "CloseElement", pcXmlElementName, "%s#%s - Element name: <%s>" );
		SetError(M4_ERROR);
		return;
	}
  //## end ClMDTSAXMerger::_ProcEndSimpleElement%1173884855.body
}

m4void_t ClMDTSAXMerger::_ProcEndRegister (const XMLCh * const name)
{
  //## begin ClMDTSAXMerger::_ProcEndRegister%1173884856.body preserve=yes
	m4return_t		iResult = M4_SUCCESS;
	ClCharHolder	pcXmlElementName(name);

	// Check if there are exclusion fields to be added in this register.
	// We have to look for a table with same pks in the exclusion XML file.
	iResult = _ProcessExclusionFields();
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcEndRegister", "_ProcessExclusionFields", pcXmlElementName, "%s#%s - Element name: <%s>" );
		SetError(M4_ERROR);
		return;
	}

	// Close element
	iResult = m_poOutput->CloseElement(pcXmlElementName);
	if (iResult != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcEndRegister", "CloseElement", pcXmlElementName, "%s#%s - Element name: <%s>" );
		SetError(M4_ERROR);
		return;
	}

	// Clear PK field list.
	m_oPKFieldsMap.clear();
  //## end ClMDTSAXMerger::_ProcEndRegister%1173884856.body
}

m4return_t ClMDTSAXMerger::_LoadExclusionData (const string &ai_sXMLExclusionFileName)
{
  //## begin ClMDTSAXMerger::_LoadExclusionData%1174305607.body preserve=yes
	m4return_t		iResult = M4_ERROR;
	InputSource		*poSource = NULL;
	
	iResult = _GetXMLExclusionData(ai_sXMLExclusionFileName.c_str(), poSource);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadExclusionData", "_GetXMLExclusionData", "%s#%s");

	// Load DOM Document from exclusion XML file.
	m_oParser.parse(*poSource);
	m_poExclusionDOMData = m_oParser.getDocument();

	// Release input source.
	iResult = _ReleaseXMLExclusionData(poSource);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadExclusionData", "_ReleaseXMLExclusionData", "%s#%s");

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_LoadExclusionData%1174305607.body
}

m4return_t ClMDTSAXMerger::_LoadDictionaryData (const string &ai_sTableName, const string &ai_sFieldName, DOMNode* &ao_rpoLogicFieldNode)
{
  //## begin ClMDTSAXMerger::_LoadDictionaryData%1174403379.body preserve=yes
	m4return_t						iResult = M4_SUCCESS;
	m4uint32_t						iLogicalFieldNum = 0;
	string							sLogicFieldName;
	m4bool_t						bNodeFound = M4_FALSE;
	xercesc::DOMDocument			*poDoc = NULL;
	DOMNode							*poScan = NULL;
	DOMNode							*poLogicalField = NULL;
	MDTDOMTraverserMap_t::iterator	it;
	ClMDTDomTraverser *				poLogicalFieldsMap = NULL;


	ao_rpoLogicFieldNode = NULL;

	// Look table in cache table map.
	it = m_oDicLogicFieldsMap.find(ai_sTableName);
	if (it != m_oDicLogicFieldsMap.end())
	{
		poLogicalFieldsMap = (*it).second;
		if (poLogicalFieldsMap !=  NULL)
		{
			iResult = poLogicalFieldsMap->FindChild(ai_sFieldName, poLogicalField);
			if (iResult == M4_SUCCESS)
			{	
				ao_rpoLogicFieldNode = poLogicalField;
				return M4_SUCCESS;
			}
		}
	}

	iResult = m_poService->GetIDictionary()->GetDOMTable(ai_sTableName.c_str(), poDoc);
	M4MDT_RETURN_ERROR1(iResult != M4_SUCCESS || poDoc == NULL, M4_ERROR, M4MDTERROR_TR_TABLE_DEF, ai_sTableName.c_str(), "%s");
	
	// Get root node.
	poScan = poDoc->getDocumentElement() ;
	M4MDT_RETURN_ERROR2(poScan == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "poScan::null", "%s#%s");
	
	poScan = poScan->getFirstChild();
	M4MDT_RETURN_ERROR2(poScan == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "poScan::null", "%s#%s");

	while (XMLString::compareString(poScan->getNodeName(), IMDTService::sm_sXMLChLogicFields) != 0)
	{
		poScan = poScan->getNextSibling();
		if (poScan == NULL)
		{
			break;
		}
	}

	M4MDT_RETURN_ERROR2(poScan == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "poScan::null", "%s#%s");
	
	// Get "logical-field" node.
	poLogicalField = poScan->getFirstChild();
	M4MDT_RETURN_ERROR2(poLogicalField == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "oLogicalField::null", "%s#%s");

	// Iterate scanning for pk's.
	for (; poLogicalField != NULL; poLogicalField = poLogicalField->getNextSibling())
	{
		// Check the field name.
		if (XMLString::compareString(poLogicalField->getNodeName(), IMDTService::sm_sXMLChLogicField) != 0)
		{
			continue;
		}

		// Check pk-position for logic field.
		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poLogicalField, sLogicFieldName);
		M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "name", iLogicalFieldNum, "logic-field", ai_sTableName.c_str(), "%s#%s#%s#%s" ) ;

		if (ai_sFieldName.compare(sLogicFieldName) == 0)
		{
			bNodeFound = M4_TRUE;
			break;
		}

		iLogicalFieldNum++;
	}

	if (bNodeFound == M4_TRUE)
	{
		// Create DOMTraverser object.
		ClMDTDomTraverser * poDOMTraverser = new ClMDTDomTraverser();
		M4MDT_RETURN_ERROR1(poDOMTraverser == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(ClMDTDomTraverser), "%d");
		
		// Save "logic-field" node, indexed by "value".
		iResult = poDOMTraverser->Init(IMDTService::sm_sXMLChLogicField, IMDTService::sm_sXMLChName, poScan);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "poDOMTraverser->Init", "%s#%s");
		
		// Save table data (all "logic-field" nodes).
		pair<MDTDOMTraverserMap_t::iterator, bool> mapRes;
		MDTDOMTraverserMap_t::value_type mapEntry(ai_sTableName.c_str(), poDOMTraverser);
		mapRes = m_oDicLogicFieldsMap.insert(mapEntry);
		M4MDT_RETURN_ERROR2(mapRes.second == M4_FALSE, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_LoadDictionaryData", "m_oDicLogicFieldsMap.insert", "%s#%s");
		
		ao_rpoLogicFieldNode = poLogicalField;

		return M4_SUCCESS;
	}
	else
	{
		return M4_SUCCESS;
	}
  //## end ClMDTSAXMerger::_LoadDictionaryData%1174403379.body
}

m4return_t ClMDTSAXMerger::_GetXMLExclusionData (const string &ai_sExclusionXMLFileName, InputSource* &ao_rpoSource)
{
  //## begin ClMDTSAXMerger::_GetXMLExclusionData%1174305608.body preserve=yes
	m4return_t	iResult = M4_SUCCESS;
	m4pchar_t	pcException = NULL;
	XMLCh		* wsFile = NULL;
	FILE		* pfFile = NULL;

	ao_rpoSource = NULL;

	// UNICODE XML
	wsFile = (XMLCh*)M4CppToXML(ai_sExclusionXMLFileName.c_str());

    try
    {
		ao_rpoSource = new LocalFileInputSource(wsFile);
		M4MDT_CHECK_ERROR1(ao_rpoSource == NULL, M4MDTERROR_NO_MEMORY, sizeof(LocalFileInputSource), "%d");
    }
    catch (const XMLException &e)
    {
		// UNICODE XML
		pcException = M4XMLToCpp((M4XMLCh*)e.getMessage());
		M4MDT_DUMP_ERROR1(M4MDTERROR_EXCEPTION, pcException, "%s");
		delete [] pcException;

		ao_rpoSource = NULL;
    }

	delete [] wsFile;

	if (ao_rpoSource == NULL)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_GetXMLExclusionData%1174305608.body
}

m4return_t ClMDTSAXMerger::_ReleaseXMLExclusionData (InputSource* &ao_rpoSource)
{
  //## begin ClMDTSAXMerger::_ReleaseXMLExclusionData%1174305609.body preserve=yes
	M4MDT_RETURN_ERROR2(ao_rpoSource == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_rpoSource", "ClMDTSAXMerger::_ReleaseXMLExclusionData", "%s#%s");

	delete ao_rpoSource;
	ao_rpoSource = NULL;

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_ReleaseXMLExclusionData%1174305609.body
}

m4return_t ClMDTSAXMerger::_CheckIsPKField (const string &ai_sTableName, const string &ai_sFieldName, m4bool_t &ao_bIsPk)
{
  //## begin ClMDTSAXMerger::_CheckIsPKField%1174296497.body preserve=yes
	m4return_t	iResult = M4_SUCCESS;
	string		sPkPosition;
	m4int32_t	iPkPosition = 0;
	DOMNode		*poLogicalFieldNode = NULL;
	
	ao_bIsPk = M4_FALSE;

	// Load dictionary data.
	iResult = _LoadDictionaryData(ai_sTableName, ai_sFieldName, poLogicalFieldNode);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_CheckIsPKField", "_LoadDictionaryData", "%s#%s");

	// Check logical-field node.
	if (poLogicalFieldNode == NULL)
	{
		// Field not found in dictionary....for example, calculated field (not pk).
		return M4_SUCCESS;
	}

	// Get pk-position for logical field.
	iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChPkPosition, poLogicalFieldNode, sPkPosition);
	M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "pk-position", ai_sFieldName.c_str(), "logic-field", ai_sTableName.c_str(), "%s#%s#%s#%s" ) ;

	iPkPosition = atoi(sPkPosition.c_str());
	if (iPkPosition != 0)
	{
		ao_bIsPk = M4_TRUE;
	}

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_CheckIsPKField%1174296497.body
}

m4return_t ClMDTSAXMerger::_DumpElement (const XMLCh* const name, const AttributeList &ai_roAttributes, m4bool_t ai_bCloseElement)
{
  //## begin ClMDTSAXMerger::_DumpElement%1174296501.body preserve=yes
	// Get XML element name.
	ClCharHolder pcXmlElementName(name);

	// Write a new element into the rich XML file.
	m4return_t iResult = m_poOutput->MakeElement(pcXmlElementName);
	M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpElement", "MakeElement", pcXmlElementName, "%s#%s - Element name: <%s>");
	
	// Write all "param-value" attributes.
	iResult = _DumpElementAttributes(ai_roAttributes);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpElement", "_DumpElementAttributes", "%s#%s");
	
	if (ai_bCloseElement == M4_TRUE)
	{
		// End element (/>)
		m_poOutput->CloseSingleClosedElement();
	}
	else
	{
		// End element (>)
		m_poOutput->EndElement();
	}
	
	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_DumpElement%1174296501.body
}

m4return_t ClMDTSAXMerger::_DumpElementAttributes (const AttributeList &ai_roAttributes)
{
  //## begin ClMDTSAXMerger::_DumpElementAttributes%1173884861.body preserve=yes
	// Get number of attributes.
	m4uint32_t uiLen = ai_roAttributes.getLength();

	// Go through all its attributes and write them.
	for (m4uint32_t uiIndex = 0; uiIndex < uiLen; uiIndex++)
	{
		ClCharHolder pcAttributeName(ai_roAttributes.getName(uiIndex));
		ClCharHolder pcAttributeValue(ai_roAttributes.getValue(uiIndex));

		// Add attributes to element.
		m4return_t iResult = m_poOutput->AddEscapedElementAttribute(pcAttributeName, pcAttributeValue);
		M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::_DumpElementAttributes", "AddEscapedElementAttribute", pcAttributeName, pcAttributeValue, "%s#%s - Attibute name: <%s> - Attribute value <%s>");
	}
	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_DumpElementAttributes%1173884861.body
}

m4return_t ClMDTSAXMerger::_ProcessExclusionFields ()
{
  //## begin ClMDTSAXMerger::_ProcessExclusionFields%1174305606.body preserve=yes
	m4return_t						iResult = M4_SUCCESS;
	int								iElement = 0;
	unsigned int					i = 0;
	short							iType = 0;
	m4bool_t						bSameRegister = M4_FALSE;
	string							sName;
	string							sExclusionTableName;
	DOMNode							*poNode= NULL;
	DOMNode							*poTableNode = NULL;
	DOMNode							*poRegisterNode = NULL;
	ClMDTDomTraverser				oTablesNodeMap;
	ClMDTDomTraverser::iterator		itTable;
	ClMDTDomTraverser				oExclusionNodeFieldsMap;
	ClMDTDomTraverser::iterator		itField;
	fieldsMap_t						oExclusionPkFieldsMap;

	// Look for exclusion fields for the current table and current register (PK fields).
	poNode = m_poExclusionDOMData->getDocumentElement();
	M4MDT_RETURN_ERROR2(poNode == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "oNode:null", "%s#%s");

	// Build map with all tables (indexed by name).
	iResult = oTablesNodeMap.Init(IMDTService::sm_sXMLChTable, IMDTService::sm_sXMLChName, poNode);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "oTables.Init", "%s#%s");

	// Look for current table.
	itTable = oTablesNodeMap.find(m_sTableName);
	if (itTable != oTablesNodeMap.end())
	{
		// Table found.
		poTableNode = (*itTable).second;
		M4MDT_RETURN_ERROR2(poTableNode == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "oTableNode:null", "%s#%s");
		
		// Get exclusion table name.
		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poTableNode, sExclusionTableName);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "name", "table", "%s#%s");
				
		while (iElement != -1)
		{
			// Get through each register.
			iElement = M4MDT_DOMFindChildNode(IMDTService::sm_sXMLChRegister, iElement + 1, poTableNode, poRegisterNode);	
			M4MDT_RETURN_ERROR2(poRegisterNode == NULL && iElement != -1, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "oRegisterNode:null", "%s#%s");	

			if (iElement != -1)
			{
				// Build map with all fields for that register (indexed by name).
				iResult = oExclusionNodeFieldsMap.Init(IMDTService::sm_sXMLChField, IMDTService::sm_sXMLChName, poRegisterNode);
				M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "oFieldsMap.Init", "%s#%s");
			
				// Generate pk fields map (name and value).
				iResult = _GenerateExclusionPKFields(sExclusionTableName, oExclusionNodeFieldsMap, oExclusionPkFieldsMap);
				M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "_GetExclusionPkFields", "%s#%s");

				// Compare pks with current field pks.
				iResult = _CompareRegisters(oExclusionPkFieldsMap, bSameRegister);
				M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "_CompareRegisters", "%s#%s");
				
				if (bSameRegister == M4_TRUE)
				{
					// Dump exclusion fields.
					iResult = _DumpExclusionFields(sExclusionTableName, oExclusionNodeFieldsMap);
					M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_ProcessExclusionFields", "_DumpExclusionFields", "%s#%s");
				}
			}
			oExclusionPkFieldsMap.clear();
			oExclusionNodeFieldsMap.clear();
		}
	}
	
	// Clear maps.
	oTablesNodeMap.clear();
	oExclusionNodeFieldsMap.clear();

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_ProcessExclusionFields%1174305606.body
}

m4return_t ClMDTSAXMerger::_CompareRegisters (fieldsMap_t ai_oExclusionPkFields, m4bool_t &ao_bSameRegister)
{
  //## begin ClMDTSAXMerger::_CompareRegisters%1174305611.body preserve=yes
	m4return_t						iResult = M4_SUCCESS;
	string							sPkFieldName;
	string							sPkFieldValue;
	string							sExclusionFieldName;
	string							sExclusionFieldValue;
	fieldsMap_t::iterator			itPKField;
	fieldsMap_t::iterator			itExclusionPKField;
	
	ao_bSameRegister = M4_FALSE;

	itPKField = m_oPKFieldsMap.begin();
	while (itPKField != m_oPKFieldsMap.end())
	{
		sPkFieldName = (*itPKField).first;
		sPkFieldValue = (*itPKField).second;
		
		// Look for this pk field within exclusion fields map.
		itExclusionPKField = ai_oExclusionPkFields.find(sPkFieldName);
		if (itExclusionPKField != ai_oExclusionPkFields.end())
		{
			sExclusionFieldValue = (*itExclusionPKField).second;	
			if (sExclusionFieldValue.compare(sPkFieldValue) != 0)
			{
				// Register pk not found. It is not the same register, not same pks.
				ao_bSameRegister = M4_FALSE;
				return M4_SUCCESS;
			}
			else
			{
				ao_bSameRegister = M4_TRUE;
			}
		}
		else
		{
			// Register pk not found. It is not the same register, not same pks.
			ao_bSameRegister = M4_FALSE;
			return M4_SUCCESS;
		}

		itPKField++;
	}

	
	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_CompareRegisters%1174305611.body
}

m4return_t ClMDTSAXMerger::_GenerateExclusionPKFields (const string &ai_sExclusionTableName, ClMDTDomTraverser ai_oExclusionNodeFields, fieldsMap_t &ao_oExclusionPkFields)
{
  //## begin ClMDTSAXMerger::_GenerateExclusionPKFields%1174305613.body preserve=yes
	m4return_t					iResult = M4_SUCCESS;
	m4bool_t					bIsPK = M4_FALSE;
	string						sExclusionFieldName;
	string						sExclusionFieldValue;
	DOMNode						*poFieldNode = NULL;
	ClMDTDomTraverser::iterator	itExclusionNodeField;
	
	itExclusionNodeField = ai_oExclusionNodeFields.begin();
	while (itExclusionNodeField != ai_oExclusionNodeFields.end())
	{
		poFieldNode = (*itExclusionNodeField).second;
		M4MDT_RETURN_ERROR2(poFieldNode == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_GenerateExclusionPKFields", "oFieldNode.Init", "%s#%s");
		
		// Get "name" attribute.
		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poFieldNode, sExclusionFieldName);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "name", "field", "%s#%s");
	
		// Check and save PK fields.
		iResult = _CheckIsPKField(ai_sExclusionTableName, sExclusionFieldName, bIsPK);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_GenerateExclusionPKFields", "_CheckPKField", "%s#%s");
		
		if (bIsPK == M4_TRUE)
		{
			// Get "value" attribute.
			iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChValue, poFieldNode, sExclusionFieldValue);
			M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "value", "field", "%s#%s");
	
			ao_oExclusionPkFields.insert(fieldsMap_t::value_type(sExclusionFieldName, sExclusionFieldValue));
		}
		itExclusionNodeField++;
	}

	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_GenerateExclusionPKFields%1174305613.body
}

m4return_t ClMDTSAXMerger::_DumpExclusionFields (const string &ai_sExclusionTableName, ClMDTDomTraverser ai_oExclusionFieldsMap)
{
  //## begin ClMDTSAXMerger::_DumpExclusionFields%1174305612.body preserve=yes
	m4return_t							iResult = M4_SUCCESS;
	m4uint32_t							i = 0;
	m4uint32_t							iLength = 0;
	string								sExclusionFieldName;
	m4bool_t							bIsPK = M4_FALSE;
	m4bool_t							bIsMultiline = M4_FALSE;
	DOMNode								*poFieldNode = NULL;
	DOMNode								*poFieldNodeValue = NULL;
	DOMNode								*poFieldAttNode = NULL;
	DOMNamedNodeMap						*poFieldAttNodeMap = NULL;
	ClMDTDomTraverser::iterator			itExclusionField;
	
	// Get through all exclusion fields and dump all those that are not pks.
	itExclusionField = ai_oExclusionFieldsMap.begin();
	while (itExclusionField != ai_oExclusionFieldsMap.end())
	{
		// Field node.
		poFieldNode = (*itExclusionField).second;

		// Check if this field is a PK field. If it is a PK field, we do not need to dump
		// because they have been already included.
		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poFieldNode, sExclusionFieldName);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_EXCLUSION_XML_DATA, "name", "field", "%s#%s");
	
		// Check and save PK fields.
		iResult = _CheckIsPKField(ai_sExclusionTableName, sExclusionFieldName, bIsPK);
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "_CheckPKField", "%s#%s");
		
		if (bIsPK == M4_TRUE)
		{
			itExclusionField++;
			continue;
		}

		// Make field element.		
		ClCharHolder pcFieldName(poFieldNode->getNodeName());
		m_poOutput->MakeElement(pcFieldName);

		// Add field attributes.
		poFieldAttNodeMap = poFieldNode->getAttributes();
		if (poFieldAttNodeMap != NULL)
		{
			iLength = poFieldAttNodeMap->getLength();
			for (i = 0; i < iLength; i++)
			{
				poFieldAttNode = poFieldAttNodeMap->item(i);
				if (poFieldAttNode != NULL)
				{
					ClCharHolder pcAttributeFieldName(poFieldAttNode->getNodeName());
					ClCharHolder pcAttributeFieldValue(poFieldAttNode->getNodeValue());
					
					iResult = m_poOutput->AddEscapedElementAttribute(pcAttributeFieldName, pcAttributeFieldValue);
					M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "AddEscapedElementAttribute", pcAttributeFieldName, pcAttributeFieldValue, "%s#%s - Attibute name: <%s> - Attribute value <%s>");
				}
			}
		}
		
		// End element (>)
		m_poOutput->EndElement();

		// Add element value.
		poFieldNodeValue = poFieldNode->getFirstChild();
		if (poFieldNodeValue != 0)
		{
			while (poFieldNodeValue != 0)
			{
				poFieldNodeValue = poFieldNodeValue->getNextSibling();
					
				ClCharHolder pcFieldValue(poFieldNodeValue->getNodeValue());
				if (strnicmp(pcFieldValue, M4MDT_XML_CDATA_PREFIX, M4MDT_XML_CDATA_PREFIX_LENGTH) != 0)
				{
					continue;
				}
				
				iResult = m_poOutput->AddElementValue(M4MD_XML_CDATA_START);
				M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "AddElementValue", pcFieldName, M4MD_XML_CDATA_START, "%s#%s - Element name: <%s> - Element value <%s>");
				
				iResult = m_poOutput->AddElementValue(pcFieldValue);
				M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "AddElementValue", pcFieldName, pcFieldValue, "%s#%s - Element name: <%s> - Element value <%s>");
					
				iResult = m_poOutput->AddElementValue(M4MD_XML_CDATA_END);
				M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "AddElementValue", pcFieldName, M4MD_XML_CDATA_END, "%s#%s - Element name: <%s> - Element value <%s>");
			}
		}

		// Close element.
		iResult = m_poOutput->CloseElement(pcFieldName);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXMerger::_DumpExclusionFields", "CloseElement", pcFieldName, "%s#%s - Element name: <%s>");

		itExclusionField++;
	}
	
	return M4_SUCCESS;
  //## end ClMDTSAXMerger::_DumpExclusionFields%1174305612.body
}

// Additional Declarations
  //## begin ClMDTSAXMerger%45F7FD85028A.declarations preserve=yes
  //## end ClMDTSAXMerger%45F7FD85028A.declarations

//## begin module%45F80154012C.epilog preserve=yes
//## end module%45F80154012C.epilog
