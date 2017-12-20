//## begin module%4090B5A60394.cm preserve=no
//## end module%4090B5A60394.cm

//## begin module%4090B5A60394.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%4090B5A60394.cp

//## Module: m4mdtsaxrichtransformer%4090B5A60394; Package body
//## Subsystem: M4MDTransfer::src%3A3E5D2D0159
//## Source file: C:\m4server\m4mdtransfer\src\m4mdtsaxrichtransformer.cpp

//## begin module%4090B5A60394.additionalIncludes preserve=no
//## end module%4090B5A60394.additionalIncludes

//## begin module%4090B5A60394.includes preserve=yes
#include "m4mdtsaxrichtransformer.hpp"
#include "m4mdtutils.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"
#include "m4xmlbase.hpp"
#include "m4unicode.hpp"

#include "xercesc/dom/DOMElement.hpp"

//## end module%4090B5A60394.additionalDeclarations



// Class ClMDTSAXRichTransformer 


ClMDTSAXRichTransformer::ClMDTSAXRichTransformer ()
  //## begin ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.hasinit preserve=no
      : m_bIsMultiline(M4_FALSE), m_poOutput(NULL)
  //## end ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.hasinit
  //## begin ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.initialization preserve=yes
  //## end ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.initialization
{
  //## begin ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.body preserve=yes
	m_sRegisterPks.reserve(1024) ;
	m_sFieldElements.reserve(4096) ;
  //## end ClMDTSAXRichTransformer::ClMDTSAXRichTransformer%1083226487.body
}


ClMDTSAXRichTransformer::~ClMDTSAXRichTransformer ()
{
  //## begin ClMDTSAXRichTransformer::~ClMDTSAXRichTransformer%1083226488.body preserve=yes
	// Delete lookups.
	DeleteLookUpMap();

	if (m_poOutput != NULL)
	{
		delete m_poOutput;
	}

	m_oOutStream.close();
  //## end ClMDTSAXRichTransformer::~ClMDTSAXRichTransformer%1083226488.body
}



//## Other Operations (implementation)
m4return_t ClMDTSAXRichTransformer::Init (IMDTService* ai_poService, const string &ai_sRichXMLFile)
{
  //## begin ClMDTSAXRichTransformer::Init%1083226489.body preserve=yes
	M4MDT_RETURN_ERROR2(ai_poService == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_poService", "ClMDTSAXRichTransformer::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2(ai_sRichXMLFile.empty() == M4_TRUE, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_sRichXMLFile", "ClMDTSAXRichTransformer::Init", "%s#%s" ) ;
	
	m_poService = ai_poService;
	m_sRichXMLFile = ai_sRichXMLFile;
	
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	M4Open( m_oOutStream, ai_sRichXMLFile.c_str(), M4UniWriteBinary, eEncoding ) ;

	M4MDT_RETURN_ERROR1(m_oOutStream.is_open() == 0 , M4_ERROR, M4MDTERROR_TR_OPENFILE, ai_sRichXMLFile.c_str(), "%s");
	
	// UNICODE XML
	m_poOutput = new M4Utf8XMLStreamGenerator(m_oOutStream);
	M4MDT_RETURN_ERROR1(m_poOutput == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(M4XMLStreamGenerator), "%d");
	
	// Methods registration: start. No registration for end.
	RegisterElementProc("object-data",(ElementProcStart)&ClMDTSAXRichTransformer::procObjectData);
	RegisterElementProc("param-value",(ElementProcStart)&ClMDTSAXRichTransformer::procParamValue);
	RegisterElementProc("table",(ElementProcStart)&ClMDTSAXRichTransformer::procTable);
	RegisterElementProc("field",(ElementProcStart)&ClMDTSAXRichTransformer::procField);
	
	// Methods registration: end
	RegisterElementEndProc("object-data",(ElementProcEnd)&ClMDTSAXRichTransformer::procEndComplexElement);
	RegisterElementEndProc("param-value",(ElementProcEnd)&ClMDTSAXRichTransformer::procEndSimpleElement);
	RegisterElementEndProc("table",(ElementProcEnd)&ClMDTSAXRichTransformer::procEndComplexElement);
	RegisterElementEndProc("register",(ElementProcEnd)&ClMDTSAXRichTransformer::procEndRegister);
	RegisterElementEndProc("field",(ElementProcEnd)&ClMDTSAXRichTransformer::procEndSimpleElement);
	
	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::Init%1083226489.body
}

void ClMDTSAXRichTransformer::startDocument ()
{
  //## begin ClMDTSAXRichTransformer::startDocument%1083226490.body preserve=yes
	// UNICODE XML
  	m_poOutput->MakeXMLDeclaration();
  //## end ClMDTSAXRichTransformer::startDocument%1083226490.body
}

void ClMDTSAXRichTransformer::endDocument ()
{
  //## begin ClMDTSAXRichTransformer::endDocument%1083226491.body preserve=yes
	m_oOutStream.close();
  //## end ClMDTSAXRichTransformer::endDocument%1083226491.body
}

void ClMDTSAXRichTransformer::characters (const XMLCh* const chars, const XMLSize_t length)
{
  //## begin ClMDTSAXRichTransformer::characters%1083226494.body preserve=yes
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
	string sElementValue = M4MD_XML_CDATA_START;
	sElementValue.append(pcText);
	sElementValue.append(M4MD_XML_CDATA_END);
	
	// Add element value.
	m_sFieldElements.append(">\n");
	m_sFieldElements.append(sElementValue);
  //## end ClMDTSAXRichTransformer::characters%1083226494.body
}

m4void_t ClMDTSAXRichTransformer::procObjectData (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXRichTransformer::procObjectData%1083233064.body preserve=yes
	// Get XML element name.
	ClCharHolder pcXmlElementName(name);

	// Write a new element into the rich XML file.
	m4return_t iErrorCode = m_poOutput->MakeElement(pcXmlElementName);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procObjectData", "MakeElement", pcXmlElementName, "%s#%s - Element name: <%s>");
		SetError(M4_ERROR);
		return;
	}

	// Get name attribute.
	ClCharHolder pcObjectDataName;
	iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, attributes, pcObjectDataName);
	if(iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_NAME_ATTRIBUTE, "object-data", "%s#%s" ) ;
		SetError(M4_ERROR);
		return;
	}

	// Write all "object-data" attributes.
	iErrorCode = WriteElementAttributes(attributes);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procObjectData", "WriteElementAttributes", "%s#%s");
		SetError(M4_ERROR);
		return;
	}
	
	// Add "caption" attribute.
	iErrorCode = m_poOutput->AddEscapedElementAttribute(M4MDT_CAPTION_ATTRIBUTE, (m4pcchar_t) pcObjectDataName);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR4(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procObjectData", "AddEscapedElementAttribute", M4MDT_CAPTION_ATTRIBUTE, pcObjectDataName, "%s#%s - Attibute name: <%s> - Attribute value <%s>");
		SetError(M4_ERROR);
		return;
	}
	
	// End element (>).
	m_poOutput->EndElement();
  //## end ClMDTSAXRichTransformer::procObjectData%1083233064.body
}

m4void_t ClMDTSAXRichTransformer::procParamValue (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXRichTransformer::procParamValue%1083233065.body preserve=yes
  	// Get XML element name.
	ClCharHolder pcXmlElementName(name);

	// Write a new element into the rich XML file.
	m4return_t iErrorCode = m_poOutput->MakeElement(pcXmlElementName);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procParamValue", "MakeElement", pcXmlElementName, "%s#%s - Element name: <%s>");
		SetError(M4_ERROR);
		return;
	}

	// Write all "param-value" attributes.
	iErrorCode = WriteElementAttributes(attributes);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procParamValue", "WriteElementAttributes", "%s#%s");
		SetError(M4_ERROR);
		return;
	}
  //## end ClMDTSAXRichTransformer::procParamValue%1083233065.body
}

m4void_t ClMDTSAXRichTransformer::procTable (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXRichTransformer::procTable%1083233066.body preserve=yes
	// Get XML element name.
	ClCharHolder pcElementName(name);

	// Write a new element into the rich XML file.
	m4return_t iErrorCode = m_poOutput->MakeElement(pcElementName);
	M4MDT_VOID_ERROR3(iErrorCode != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procTable", "MakeElement", pcElementName, "%s#%s - Element name: <%s>");

	// Go through all its attributes and check if it has an attribute called "value".
	// If it has an attribute "value", add a new attribute "caption" with the translated 
	// value.
	// Get name attribute.
	ClCharHolder pcTableName;
	iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, attributes, pcTableName);
	if(iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_NAME_ATTRIBUTE, "table", "%s#%s" ) ;
		SetError(M4_ERROR);
		return;
	}

	// Write all "table" attributes.
	iErrorCode = WriteElementAttributes(attributes);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procTable", "WriteElementAttributes", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	// Get table caption.
	ClCharHolder pcTableCaption;
	iErrorCode = LoadDictionaryInfo(pcTableName, pcTableCaption);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procTable", "LoadDictionaryInfo", pcTableName, "%s#%s - Element name: <%s>");
		SetError(M4_ERROR);
		return;
	}

	// If no table caption found, take table name.
	if (pcTableCaption == NULL)
	{
		pcTableCaption.Set( pcTableName ) ;
	}

	// Add new attribute caption.
	iErrorCode = m_poOutput->AddEscapedElementAttribute(M4MDT_CAPTION_ATTRIBUTE, (m4pcchar_t)pcTableCaption);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR4(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procTable", "AddEscapedElementAttribute", M4MDT_CAPTION_ATTRIBUTE, pcTableCaption, "%s#%s - Attibute name: <%s> - Attribute value <%s>");
		SetError(M4_ERROR);
		return;
	}
	
	// End element (>).
	m_poOutput->EndElement();
  //## end ClMDTSAXRichTransformer::procTable%1083233066.body
}

m4void_t ClMDTSAXRichTransformer::procField (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTSAXRichTransformer::procField%1083233068.body preserve=yes
	// Reset multiline flag.
	m_bIsMultiline = M4_FALSE;

	// Get XML element name.
	ClCharHolder pcXmlElementName(name);
	m_sFieldElements.append("\n<");
	m_sFieldElements.append(pcXmlElementName);
	
	// Get number of attributes.
	m4uint32_t uiLen = attributes.getLength();

	// Go through all its attributes and check if it has an attribute called "value".
	// If it has an attribute "value", add a new attribute "caption" with the translated 
	// value.
	m4return_t iErrorCode = M4_SUCCESS;
	ClCharHolder pcFieldName, pcFieldValue;
	ClCharHolder pcIsNull, pcIsPrintable, pcIsMultiline;
	m4bool_t bIsNull = M4_FALSE, bIsPrintable = M4_FALSE, bIsMultiline = M4_FALSE;
	for (m4uint32_t uiIndex = 0; uiIndex < uiLen; uiIndex++)
	{
		ClCharHolder pcAttributeName(attributes.getName(uiIndex));
		ClCharHolder pcAttributeValue(attributes.getValue(uiIndex));
		
		m_sFieldElements.append(" ");
		m_sFieldElements.append(pcAttributeName);
		m_sFieldElements.append("=\"");

		// Escape attribute values.
		iErrorCode = M4XMLStreamGenerator::AppendScapedString(pcAttributeValue, m_sFieldElements);
		if (iErrorCode != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procField", "AppendScapedString", pcAttributeValue, "%s#%s - Field value <%s>");
			SetError(M4_ERROR);
			return;
		}

		m_sFieldElements.append("\"");
	}

	// Get "name" attribute.
	iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, attributes, pcFieldName);
	if(iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_NAME_ATTRIBUTE, "field", "%s#%s");
		SetError(M4_ERROR);
		return;
	}

	// Get "is-null" attribute. This attribute is optional, only sent if the field has
	// null value. In this case, "value" attribute will not be sent.
	// This is why we do not check returned value.
	iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChIsNull, attributes, pcIsNull);
	
	// Check if the field value is null.
	if (iErrorCode == M4_SUCCESS && strcmpi(pcIsNull, "true") == 0)
	{
		bIsNull = M4_TRUE;
	}
	
	// Get "value" attribute only if is-null="true". Same as "is-null" attribute: it can
	// be sent or not. This is why we do not check returned value.
	if (bIsNull == M4_FALSE)
	{
		// Get "multiline" attribute. This attribute is optional, only sent if the field has
		// multiline values. This is why we do not check returned value.
		iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChMultiline, attributes, pcIsMultiline);
		if (iErrorCode == M4_SUCCESS && strcmpi(pcIsMultiline, "true") == 0)
		{
			m_bIsMultiline = M4_TRUE;
		}
		else
		{
			// Get "value" attribute.
			iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChValue, attributes, pcFieldValue);
			if(iErrorCode != M4_SUCCESS)
			{
				M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_VALUE_ATTRIBUTE, "field", "%s#%s");
				SetError(M4_ERROR);
				return;
			}

			// Check if this field is a lookup field. In this case, we will save the lookup name
			// instead of the lookup value.
			ClCharHolder pcLookUpCaption;
			
			iErrorCode= GetLookUpCaption((m4pchar_t)pcFieldName, (m4pchar_t) pcFieldValue, pcLookUpCaption);
			if (iErrorCode == M4_SUCCESS && pcLookUpCaption != NULL)
			{
				pcFieldValue.Set( pcLookUpCaption ) ;
			}
		}
	}
	
	// Get "is-printable" attribute.
	iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChIsPrintable, attributes, pcIsPrintable);
	if (iErrorCode == M4_SUCCESS && strcmpi(pcIsPrintable, "true") == 0)
	{
		bIsPrintable = M4_TRUE;
	}
	
	// Get caption attibute value for the current field. If no caption retrieved, 
	// we will set caption value to "value" attribute value.
	ClCharHolder pcFieldCaption;

	iErrorCode = GetFieldCaption((m4pchar_t)pcFieldName, pcFieldCaption);
	if (iErrorCode == M4_ERROR || pcFieldCaption == NULL )
	{
		pcFieldCaption.Set( pcFieldName ) ;
	}

	// Append field element. If the field value is null we will enclose field value without
	// quotes and "NULL" literal as value.
	m_sFieldElements.append(" ");
	m_sFieldElements.append(M4MDT_CAPTION_ATTRIBUTE);
	m_sFieldElements.append("=\"");

	// Escape caption value.
	iErrorCode = M4XMLStreamGenerator::AppendScapedString(pcFieldCaption, m_sFieldElements);
	if (iErrorCode != M4_SUCCESS)
	{
		M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procField", "AppendScapedString", pcFieldCaption, "%s#%s - Field value <%s>");
		SetError(M4_ERROR);
		return;
	}

	m_sFieldElements.append("=");

	if (bIsNull == M4_TRUE)
	{
		m_sFieldElements.append(M4MDT_XML_NULL_VALUE);
		m_sFieldElements.append("\"");
	}
	else
	{
		m_sFieldElements.append(M4MDT_XML_QUOTE);

		// Escape field value.
		iErrorCode = M4XMLStreamGenerator::AppendScapedString(pcFieldValue, m_sFieldElements);
		if (iErrorCode != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procField", "AppendScapedString", pcFieldValue, "%s#%s - Field value <%s>");
			SetError(M4_ERROR);
			return;
		}

		m_sFieldElements.append(M4MDT_XML_QUOTE);
		m_sFieldElements.append("\"");
	}

	// If this field is printable, add it to register pks.
	if (bIsPrintable == M4_TRUE)
	{
		if (m_sRegisterPks.empty() == M4_TRUE)
		{
			m_sRegisterPks = M4MDT_CAPTION_ATTRIBUTE;
			m_sRegisterPks.append("=\"");
		}
		else
		{
			m_sRegisterPks.append(M4MDT_PKS_SEPARATOR);
		}

		// Escape caption value.
		iErrorCode = M4XMLStreamGenerator::AppendScapedString(pcFieldCaption, m_sRegisterPks);
		if (iErrorCode != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procField", "AppendScapedString", pcFieldCaption, "%s#%s - Field value <%s>");
			SetError(M4_ERROR);
			return;
		}

		m_sRegisterPks.append("=");

		// Check if field value is null. If it is null, add "NULL" instead of its value.
		if (bIsNull == M4_TRUE)
		{
			m_sRegisterPks.append(M4MDT_XML_NULL_VALUE);
		}
		else
		{
			m_sRegisterPks.append(M4MDT_XML_QUOTE);

			// Escape field value.
			iErrorCode = M4XMLStreamGenerator::AppendScapedString(pcFieldValue, m_sRegisterPks);
			if (iErrorCode != M4_SUCCESS)
			{
				M4MDT_DUMP_ERROR3(M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procField", "AppendScapedString", pcFieldValue, "%s#%s - Field value <%s>");
				SetError(M4_ERROR);
				return;
			}

			m_sRegisterPks.append(M4MDT_XML_QUOTE);
		}
	}
  //## end ClMDTSAXRichTransformer::procField%1083233068.body
}

m4void_t ClMDTSAXRichTransformer::procEndComplexElement (const XMLCh * const name)
{
  //## begin ClMDTSAXRichTransformer::procEndComplexElement%1083569618.body preserve=yes
	ClCharHolder pcXmlElementName(name);
	
	// Close element (</element_name>). Complex element (has childs).
	m4return_t iErrorCode = m_poOutput->CloseElement(pcXmlElementName);
	M4MDT_CHECK_ERROR3(iErrorCode != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procEndComplexElement", "CloseElement", pcXmlElementName, "%s#%s - Element name: <%s>" );
  //## end ClMDTSAXRichTransformer::procEndComplexElement%1083569618.body
}

m4void_t ClMDTSAXRichTransformer::procEndSimpleElement (const XMLCh * const name)
{
  //## begin ClMDTSAXRichTransformer::procEndSimpleElement%1083569619.body preserve=yes
	ClCharHolder pcXmlElementName(name);
	
	// Simple element (no childs). A simple XML element can have value or not. If it has value
	// (characters event previously called) close it with </name>. If it has not value,
	// close it with />.
	m4return_t iErrorCode = M4_SUCCESS;
	if (m_bIsMultiline == M4_TRUE)
	{
		// </name>.
		if (strcmpi( pcXmlElementName,"field") == 0)
		{
			m_sFieldElements.append("</");
			m_sFieldElements.append(pcXmlElementName);
			m_sFieldElements.append(">");
		}
		else
		{
			iErrorCode = m_poOutput->CloseElement(pcXmlElementName);
			M4MDT_CHECK_ERROR3(iErrorCode != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::procEndSimpleElement", "CloseElement", pcXmlElementName, "%s#%s - Element name: <%s>" );
		}
	}	
	else
	{
		// />.
		if (strcmpi( pcXmlElementName, "field") == 0)
		{
			m_sFieldElements.append("/>");
		}
		else
		{
			m_poOutput->CloseSingleClosedElement();
		}
	}	
  //## end ClMDTSAXRichTransformer::procEndSimpleElement%1083569619.body
}

m4void_t ClMDTSAXRichTransformer::procEndRegister (const XMLCh * const name)
{
  //## begin ClMDTSAXRichTransformer::procEndRegister%1084265436.body preserve=yes
	ClCharHolder pcXmlElementName(name);
	int			iLength = -1 ;
	m4pchar_t	pccElementName = NULL ;
	m4pchar_t	pccRegisterPks = NULL ;
	m4pchar_t	pccFieldElements = NULL ;
	
	// Complete register pks.
	m_sRegisterPks.append("\"");

	// Bug 0128780. Add carriage return.
	m_sFieldElements.append("\n");

	// UNICODE XML
	iLength = -1 ;
	pccElementName = M4CppToUtf8( pcXmlElementName, iLength ) ;
	iLength = m_sRegisterPks.size() ;
	pccRegisterPks = M4CppToUtf8( m_sRegisterPks.c_str(), iLength ) ;
	iLength = m_sFieldElements.size() ;
	pccFieldElements = M4CppToUtf8( m_sFieldElements.c_str(), iLength ) ;

	// Build register element with its pks and its field child elements.

	//Open register element with PK
	m_oOutStream << "<";
	m_oOutStream << pccElementName;
	m_oOutStream << " ";
	m_oOutStream << pccRegisterPks;
	m_oOutStream << ">";
	
	//Adding fields
	m_oOutStream << pccFieldElements;

	//Close register element
	m_oOutStream << "</";
	m_oOutStream << pccElementName;	
	m_oOutStream << ">";
	
	//Adding comment with PK
	m_oOutStream << " <!--";
	m_oOutStream << pccRegisterPks;
	m_oOutStream << "-->\n";

	delete [] pccElementName ;
	delete [] pccRegisterPks ;
	delete [] pccFieldElements ;

	// Reset field elements and register pks.
	m_sRegisterPks = "";
	m_sFieldElements = "";
  //## end ClMDTSAXRichTransformer::procEndRegister%1084265436.body
}

m4return_t ClMDTSAXRichTransformer::LoadDictionaryInfo (m4pcchar_t ai_pccTableName, ClCharHolder& ao_rpcTableCaption)
{
  //## begin ClMDTSAXRichTransformer::LoadDictionaryInfo%1086078628.body preserve=yes
	m4return_t iRes = M4_ERROR;
	xercesc::DOMDocument *poDoc = NULL;

	ao_rpcTableCaption.SetNull();

	iRes = m_poService->GetIDictionary()->GetDOMTable(ai_pccTableName, poDoc);
	M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS || poDoc == NULL, M4_ERROR, M4MDTERROR_TR_TABLE_DEF, ai_pccTableName, "%s");

	// Load logic-field nodes.
	iRes = GenerateTableFields(poDoc, ao_rpcTableCaption);
	M4MDT_RETURN_ERROR2(iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::LoadDictionaryInfo", "GenerateTableFields", "%s#%s");

	// Load lookups.
	iRes = GenerateLookUps(poDoc);
	M4MDT_RETURN_ERROR2(iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::LoadDictionaryInfo", "GenerateLookUps", "%s#%s");

	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::LoadDictionaryInfo%1086078628.body
}

m4return_t ClMDTSAXRichTransformer::GenerateTableFields (const xercesc::DOMDocument *ai_pcoDOMDocument, ClCharHolder& ao_rpcTableCaption)
{
  //## begin ClMDTSAXRichTransformer::GenerateTableFields%1083233071.body preserve=yes
	m4return_t iErrorCode = M4_ERROR;

	ao_rpcTableCaption.SetNull();

	// Get the logical fields node
	DOMNode *poScan = NULL;

	if (ai_pcoDOMDocument != NULL)
	{
		poScan = ai_pcoDOMDocument->getDocumentElement() ;
	}

	// Go down
	if (poScan != NULL)
	{
		// Get table caption ("caption" attribute from "logic-object" node).
		iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChCaption, poScan, ao_rpcTableCaption);
		M4MDT_RETURN_ERROR2(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_CAPTION_ATTRIBUTE, "logic-object", "%s#%s");

		poScan = poScan->getFirstChild();
		{
			while (XMLString::compareString(poScan->getNodeName(), IMDTService::sm_sXMLChLogicFields) != 0)
			{
				poScan = poScan->getNextSibling();
				if (poScan == NULL)
				{
					break;
				}
			}
		}
	}

	// Save logic-field nodes, indexed by name.
	iErrorCode = m_oLogicFields.Init(IMDTService::sm_sXMLChLogicField, IMDTService::sm_sXMLChName, poScan);
	M4MDT_RETURN_ERROR2(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::GenerateTableFields", "m_oLogicFields.Init", "%s#%s");

	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::GenerateTableFields%1083233071.body
}

m4return_t ClMDTSAXRichTransformer::GenerateLookUps (const xercesc::DOMDocument *ai_pcoDOMDocument)
{
  //## begin ClMDTSAXRichTransformer::GenerateLookUps%1086078627.body preserve=yes
	m4return_t iErrorCode = M4_ERROR;

	// Get the logical fields node
	DOMNode *poScan = NULL;

	if (ai_pcoDOMDocument != NULL)
	{
		poScan = ai_pcoDOMDocument->getDocumentElement()->getFirstChild() ;
	}

	// Go down
	if (poScan != NULL)
	{
		while (XMLString::compareString(poScan->getNodeName(), IMDTService::sm_sXMLChLookups) != 0)
		{
			poScan = poScan->getNextSibling();
			if (poScan == NULL)
			{
				break;
			}
		}
	}

	DOMNode *poLookUp = poScan->getFirstChild();

	// Iterate scanning for lookup.
	for (; poLookUp != NULL; poLookUp = poLookUp->getNextSibling())
	{
		// Checks the lookup node name.
		if (XMLString::compareString(poLookUp->getNodeName(), IMDTService::sm_sXMLChLookup) != 0)
		{
			continue;
		}

		// Check attributes.
		DOMNamedNodeMap *poMap = poLookUp->getAttributes();
		if (poMap == NULL)
		{
			continue;
		}
	
		// Get look up name attribute.
		ClCharHolder pcLookUpName;
		iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poLookUp, pcLookUpName);
		M4MDT_RETURN_ERROR2(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_NAME_ATTRIBUTE, "lookup", "%s#%s");

		// First of all, check if there has been already inserted the same lookup.
		m4bool_t bExist = CheckLookUp((m4pchar_t)pcLookUpName);
		
		if (bExist == M4_FALSE)
		{	
			// Create DOMTraverser object.
			ClMDTDomTraverser * poDOMTraverser = new ClMDTDomTraverser();
			M4MDT_RETURN_ERROR1(poDOMTraverser == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(ClMDTDomTraverser), "%d");
			
			// Save "lookup-register" node, indexed by "value".
			iErrorCode = poDOMTraverser->Init(IMDTService::sm_sXMLChLookupRegister, IMDTService::sm_sXMLChValue, poLookUp);
			M4MDT_RETURN_ERROR2(iErrorCode != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::GenerateLookUps", "poDOMTraverser->Init", "%s#%s");
			
			// Save the lookup node.
			pair<MDTDOMTraverserMap_t::iterator, bool> mapRes;
			MDTDOMTraverserMap_t::value_type mapEntry((m4pchar_t)pcLookUpName, poDOMTraverser);
			mapRes = m_oLookUpMap.insert(mapEntry);
			M4MDT_RETURN_ERROR2(mapRes.second == M4_FALSE, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::GenerateLookUps", "m_oLookUpMap.insert", "%s#%s");
		}
	}
	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::GenerateLookUps%1086078627.body
}

m4return_t ClMDTSAXRichTransformer::GetFieldCaption (const string &ai_sFieldName, ClCharHolder& ao_rpcFieldCaption)
{
  //## begin ClMDTSAXRichTransformer::GetFieldCaption%1083569616.body preserve=yes
	// Scan all <logic-field> nodes.
	m4return_t iErrorCode = M4_ERROR;
	
	ao_rpcFieldCaption.SetNull();

	// Find logic-field with the given name. Calculated fields do not have logic field.
	// This is why we do not dump an error in this function.
	DOMNode *poLogicFieldNode = NULL;
	iErrorCode = m_oLogicFields.FindChild(ai_sFieldName, poLogicFieldNode);
	
	// Gets "caption" attribute.
	if (iErrorCode == M4_SUCCESS)
	{
		if (poLogicFieldNode != NULL)
		{
			iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChCaption, poLogicFieldNode, ao_rpcFieldCaption);
			if (iErrorCode != M4_SUCCESS)
			{
				M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_CAPTION_ATTRIBUTE, "logic-field", "%s#%s");
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::GetFieldCaption%1083569616.body
}

m4return_t ClMDTSAXRichTransformer::GetLookUpCaption (const string &ai_sLookUpName, const string &ai_sLookUpValue, ClCharHolder& ao_rpcLookUpCaption)
{
  //## begin ClMDTSAXRichTransformer::GetLookUpCaption%1086078629.body preserve=yes
	m4return_t iErrorCode = M4_ERROR;
				
	ao_rpcLookUpCaption.SetNull();

	// Find "lookup" node in lookup map.
	MDTDOMTraverserMap_t::iterator it;
	it = m_oLookUpMap.find(ai_sLookUpName);
	if (it != m_oLookUpMap.end())
	{
		ClMDTDomTraverser * poLookUpNode = (*it).second;
		if (poLookUpNode != NULL)
		{
			DOMNode *poLookUpRegisterNode = NULL;
			iErrorCode = poLookUpNode->FindChild(ai_sLookUpValue, poLookUpRegisterNode);
			if (iErrorCode == M4_SUCCESS)
			{	
				// Get caption from lookup-register.
				if (poLookUpRegisterNode != NULL)
				{
					// Get "caption" attribute.
					iErrorCode = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChCaption, poLookUpRegisterNode, ao_rpcLookUpCaption);
					if (iErrorCode != M4_SUCCESS)
					{
						M4MDT_DUMP_ERROR2(M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_XML_DATA, M4MDT_CAPTION_ATTRIBUTE, "lookup-register", "%s#%s");
						return M4_ERROR;
					}
				}
			}
		}
	}

	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::GetLookUpCaption%1086078629.body
}

m4bool_t ClMDTSAXRichTransformer::CheckLookUp (const string &ai_sLookUpName)
{
  //## begin ClMDTSAXRichTransformer::CheckLookUp%1086165948.body preserve=yes
	m4bool_t bFound = M4_FALSE;
	MDTDOMTraverserMap_t::iterator it;
	it = m_oLookUpMap.find(ai_sLookUpName);
	if (it != m_oLookUpMap.end())
	{
		bFound = M4_TRUE;
	}
	return bFound;
  //## end ClMDTSAXRichTransformer::CheckLookUp%1086165948.body
}

m4return_t ClMDTSAXRichTransformer::WriteElementAttributes (const AttributeList &ai_roAttributes)
{
  //## begin ClMDTSAXRichTransformer::WriteElementAttributes%1085474121.body preserve=yes
	// Get number of attributes.
	m4uint32_t uiLen = ai_roAttributes.getLength();

	// Go through all its attributes and write them.
	for (m4uint32_t uiIndex = 0; uiIndex < uiLen; uiIndex++)
	{
		ClCharHolder pcAttributeName(ai_roAttributes.getName(uiIndex));
		ClCharHolder pcAttributeValue(ai_roAttributes.getValue(uiIndex));

		// Add attributes to element.
		m4return_t iErrorCode = m_poOutput->AddEscapedElementAttribute(pcAttributeName, pcAttributeValue);
		M4MDT_CHECK_ERROR4(iErrorCode != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTSAXRichTransformer::WriteElementAttributes", "AddEscapedElementAttribute", pcAttributeName, pcAttributeValue, "%s#%s - Attibute name: <%s> - Attribute value <%s>");
	}
	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::WriteElementAttributes%1085474121.body
}

m4return_t ClMDTSAXRichTransformer::DeleteLookUpMap ()
{
  //## begin ClMDTSAXRichTransformer::DeleteLookUpMap%1086165947.body preserve=yes
	MDTDOMTraverserMap_t::iterator it;
	ClMDTDomTraverser * oDomTraverser = NULL;
	it = m_oLookUpMap.begin();
	while (it != m_oLookUpMap.end())
	{
		oDomTraverser = (*it).second;
		if (oDomTraverser != NULL)
		{
			delete oDomTraverser;
		}
		it++;
	}
	m_oLookUpMap.clear();
	return M4_SUCCESS;
  //## end ClMDTSAXRichTransformer::DeleteLookUpMap%1086165947.body
}

// Additional Declarations
  //## begin ClMDTSAXRichTransformer%4090B35402C7.declarations preserve=yes
  //## end ClMDTSAXRichTransformer%4090B35402C7.declarations

//## begin module%4090B5A60394.epilog preserve=yes
//## end module%4090B5A60394.epilog
