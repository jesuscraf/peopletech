//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4dmsxmldefines.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:9/8/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#ifndef M4_DMSXMLDEFINES_HPP
#define M4_DMSXMLDEFINES_HPP

//----------------------------------------------------------------------------//
//                 CONSTANTS FOR IMPORT/EXPORT								  //
//----------------------------------------------------------------------------//
#define M4_IMPORT_XML_PROCESS 1
#define M4_EXPORT_XML_PROCESS 2

// Preprocess and postprocess method return values.
#define M4_XML_EXECUTION_METHOD_OK	1
#define M4_XML_IGNORE_REGISTER		0
#define M4_XML_ABORT_PROCESS		-1

// Register status
#define M4_REGISTER_STATUS_DELETE	-1
#define M4_REGISTER_STATUS_ADD		0
#define M4_REGISTER_STATUS_UPDATE	1

#define M4_MAX_BUFFER_SIZE			256

#define M4_NODE_TYPE 1
#define M4_ITEM_TYPE 2
#define M4_NODE_AND_ITEM_TYPE 3
#define M4_KEY_SEP ','

#define M4_EQUAL				"="
#define M4_SPACE				" "
#define M4_CARRIAGE_RETURN		'\n'
#define M4_SINGLE_QUOTE			"'"
#define	M4_DOBLE_QUOTE			"\""
#define M4_CH_SINGLE_QUOTE		'\''
#define	M4_CH_DOBLE_QUOTE		'"'
#define	M4_CH_AMP				'&'
#define	M4_CH_LT				'<'
#define	M4_CH_GT				'>'
#define M4_XML_AMP				"&amp;"
#define M4_XML_GT				"&gt;"
#define M4_XML_LT				"&lt;"
#define M4_XML_QUOT				"&quot;"
#define M4_XML_APOS				"&apos;"


#define	M4_XML_OPEN_ELEMENT		"<"
#define M4_XML_CLOSE_ELEMENT	">"
#define M4_XML_OPEN_END_ELEMENT	"</"
#define M4_XML_END_ELEMENT		"/>"
#define M4_XML_OPEN_COMMENT		"<!-- "
#define M4_XML_CLOSE_COMMENT	" -->"

#define M4_XML_STRUCTURE_SEC	1	// XML Structure is sequence type 
#define M4_XML_STRUCTURE_OP		2	// XML Structure is option type 

//----------------------------------------------------------------------------//
//                 METHOD CHANNEL ERRORS									  //
//----------------------------------------------------------------------------//
#define M4_COPY_DTD_FILE_ERROR	-2
#define M4_EOF					1

//----------------------------------------------------------------------------//
//                 ELEMENT CONSISTENCY ERRORS
//----------------------------------------------------------------------------//
#define M4_CONSISTENCY_ELEMENT_ERROR -3

//----------------------------------------------------------------------------//
//                 METHOD NAMES FOR IMPORT/EXPORT							  //
//----------------------------------------------------------------------------//
#define M4O_M4OBJECT_XML_MAP				"DMS_M4OBJECT_XML_MAP"
#define NODE_M4OBJECT_XML_MAP				"M4OBJECT_XML_MAP"
#define METHOD_FILTERED_LOAD				"FILTERED_LOAD"
#define METHOD_UNLOAD_INFO					"UNLOAD_INFO"
#define METHOD_GET_XML_ELEMENT_MAP_INFO		"GET_XML_ELEMENT_MAP_INF"
#define METHOD_GET_XML_ATTRIBUTE_MAP_INF	"GET_XML_ATTRIBUTE_MAP_INF"
#define METHOD_GET_ROOT_ELEMENT_INFO		"GET_ROOT_ELEMENT_INFO"
#define METHOD_GET_BASE_ELEMENT_INFO		"GET_BASE_ELEMENT_INFO"
#define METHOD_GET_DTD_FILE_NAME			"GET_DTD_FILE_NAME"

#define METHOD_LOAD_ATTRIBUTES_OF_ELEMENT	"LOAD_ATTRIBUTES_OF_ELEMENT"
#define METHOD_LOAD_SUBELEMENTS_OF_ELEMENT	"LOAD_SUBELEMENTS_OF_ELEMENT"
#define METHOD_GET_NEXT_SUBELEMENT			"GET_NEXT_SUBELEMENT"
#define METHOD_GET_NEXT_ATTRIBUTE			"GET_NEXT_ATTRIBUTE"

#define METHOD_CHECK_ITEMS_SCOPE			"CHECK_ITEMS_SCOPE"
#define METHOD_CHECK_NODES_CONSISTENCY		"CHECK_NODES_CONSISTENCY"

//----------------------------------------------------------------------------//
//                 XML SCHEMA INFORMATION
//----------------------------------------------------------------------------//
#define M4_XML_SCHEMA_INSTANCE "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
#define M4_XML_SCHEMA_NO_NAMESPACE_SCHEMA_LOCATION  "xsi:noNamespaceSchemaLocation"
#define M4_XML_SCHEMA_LOCATION "xsi:schemaLocation"
#define M4_XML_SCHEMA_XMLNS "xmlns"
#define M4_XML_SCHEMA_XSI "xsi"

// Tipos de encoding XML
typedef	enum eUniXmlEncodingType_tag
{
	M4XmlNative,
	M4XmlANSI,
	M4XmlISO,
	M4XmlUtf8

} eUniXmlEncodingType_t;




#endif


