//## begin module%3A66B5A7025F.cm preserve=no
//## end module%3A66B5A7025F.cm

//## begin module%3A66B5A7025F.cp preserve=no
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
//## end module%3A66B5A7025F.cp

//## Module: m4dmsxmlm4omap%3A66B5A7025F; Package body
//## Subsystem: M4DMSXml::src%3A3E5D2D0159
//## Source file: C:\m4server\m4dmsxml\src\m4dmsxmlm4omap.cpp

//## begin module%3A66B5A7025F.additionalIncludes preserve=no
//## end module%3A66B5A7025F.additionalIncludes

//## begin module%3A66B5A7025F.includes preserve=yes
// channel
#include <channel.hpp>
// access
#include <access.hpp>
//executor
#include <executor.hpp>
// dm
#include <dm.hpp>
//file_finder
#include <file_finder.hpp>
// m4dmschannelhandler
#include <m4dmschannelhandler.hpp>
// Defines
#include "m4dmsxmldefines.hpp"
// Errors
#include "m4dmsxmlerr.hpp"
//## end module%3A66B5A7025F.includes

// node_x
#include <node_x.hpp>
// m4dmsxmlm4omap
#include <m4dmsxmlm4omap.hpp>


//## begin module%3A66B5A7025F.declarations preserve=no
//## end module%3A66B5A7025F.declarations

//## begin module%3A66B5A7025F.additionalDeclarations preserve=yes
//## end module%3A66B5A7025F.additionalDeclarations


// Class ClDMSXmlM4OMap 







ClDMSXmlM4OMap::ClDMSXmlM4OMap (ClDMSChannelHandler *ai_poChannelHandler)
  //## begin ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.hasinit preserve=no
      : m_bLoadByChannelHandle(M4_FALSE)
  //## end ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.hasinit
  //## begin ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.initialization preserve=yes
  //## end ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.initialization
{
  //## begin ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.body preserve=yes
	m_poXmlMapChannel = NULL;
	m_poXmlMapAccess = NULL;
	m_poChannelHandler = ai_poChannelHandler;
	m_bElementConsistency = M4_TRUE;
  //## end ClDMSXmlM4OMap::ClDMSXmlM4OMap%979749964.body
}


ClDMSXmlM4OMap::~ClDMSXmlM4OMap ()
{
  //## begin ClDMSXmlM4OMap::~ClDMSXmlM4OMap%979749965.body preserve=yes
  //## end ClDMSXmlM4OMap::~ClDMSXmlM4OMap%979749965.body
}



//## Other Operations (implementation)
m4return_t ClDMSXmlM4OMap::InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const string &ai_sChannelName, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency)
{
  //## begin ClDMSXmlM4OMap::InitializeXmlMapM4O%979749966.body preserve=yes
	m4return_t		iRet = M4_SUCCESS;
	m4VariantType	vM4ObjectId, vMapType, vDoValidation;
	m4VariantType	vXmlFilePath, vRes;
	ClFileName oCompleteFileName, oFilePath;	
	ClFile oFile(ai_sXMLFilePath.c_str());

	// Establecer la consistencia de elementos.
	m_bElementConsistency = ai_bElementConsistency;

	if (ai_iChannelHandle > 0)
	{
		// Get M4Object-XML Map channel (DMS_M4OBJECT_XML_MAP) from its channel handle.
		// This means that this channel has been already loaded and we want to use this
		// instance instead of loading a new one.
		iRet = m_poChannelHandler->GetChannelByHandle(ai_iChannelHandle, m_poXmlMapChannel, m_poXmlMapAccess);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_LOAD_CHANNEL_ERROR, ERRORLOG, "#*s1*#%s#", M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}

		// Move to first register (should have just one).
		ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
		if (poNode == NULL)
		{
			SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
		poNode->RecordSet.Current.MoveTo(0);

		m_bLoadByChannelHandle = M4_TRUE;

		// Bug 0113729.
		// Verify mapped nodes consistency. When you specify a channel handler, you usually 
		// used dynamic mappins (user takes care of defining desired m4o mappings, node mappings
		// and items mappings). When you used node mappings method, it automatically fills child
		// nodes for this mapped node, but this does not mean that user have mapped all its child 
		// nodes. This is the reason, we need to verify mapped nodes consistency.
		iRet = CheckNodesConsistency();
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_CHECK_NODES_CONSISTENCY_ERROR, ERRORLOG, "#*s1*#");
			return M4_ERROR;
		}
	}
	else
	{
		// Load M4Object-XML Map channel (DMS_M4OBJECT_XML_MAP).
		iRet = m_poChannelHandler->LoadChannel(M4O_M4OBJECT_XML_MAP, m_poXmlMapChannel, m_poXmlMapAccess);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_LOAD_CHANNEL_ERROR, ERRORLOG, "#*s1*#%s#", M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
		
		// Get root node.
		ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
		if (poNode == NULL)
		{
			SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
				
		// Execute filtered load method (LOAD_FILTERED).
		// Push method parameters.
		vM4ObjectId.Type = M4CL_CPP_TYPE_STRING_VAR;
		vM4ObjectId.Data.PointerChar = (m4pchar_t) ai_sChannelName.c_str();
		
		// Map type.
		// Check default mappings: if ai_bDefaultMappings is true, this means we have no mappings for the
		// channel. In this case, we will load default mappings calling method SET_DEFAULT_MAPPINGS.
		m4int32_t iMapType = ai_iMapType;
		if (ai_bDefaultMappings == M4_TRUE)
		{
			vMapType.Type = M4CL_CPP_TYPE_NULL;
		}
		else
		{
			vMapType.Type = M4CL_CPP_TYPE_NUMBER;
			vMapType.Data.DoubleData = iMapType;
		}
		
		// Do validation.
		vDoValidation.Type = M4CL_CPP_TYPE_NUMBER;
		vDoValidation.Data.DoubleData = ai_bDoValidation;
		
		// Extract XML file path (we have path + name), only when the 
		// path is not null.
		if (ai_sXMLFilePath == "")
		{
			// XML file path.
			vXmlFilePath.Type = M4CL_CPP_TYPE_STRING_VAR;
			vXmlFilePath.Data.PointerChar = (m4pchar_t) ai_sXMLFilePath.c_str();
		}
		else
		{
			oCompleteFileName = oFile.GetFileName();
			oCompleteFileName.GetDir(oFilePath);
			
			// XML file path.
			vXmlFilePath.Type = M4CL_CPP_TYPE_STRING_VAR;
			vXmlFilePath.Data.PointerChar = (char *)(const char *) oFilePath;
		}
		
		if (poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].StackPush(vM4ObjectId) != M4_SUCCESS ||
			poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].StackPush(vMapType) != M4_SUCCESS ||
			poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].StackPush(vDoValidation) != M4_SUCCESS ||
			poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].StackPush(vXmlFilePath) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_FILTERED_LOAD, M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
		
		if (poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].Call() != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_FILTERED_LOAD, M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
		
		// Get executing method result.
		if (poNode->RecordSet.Current.Item[METHOD_FILTERED_LOAD].StackPop(vRes) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_FILTERED_LOAD, M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
		
		// Check error type.
		if (vRes.Data.DoubleData != M4_SUCCESS)
		{
			if (vRes.Data.DoubleData == M4_COPY_DTD_FILE_ERROR)
			{
				SETCODEF(M4_DMS_COPY_DTD_FILE_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_FILTERED_LOAD, M4O_M4OBJECT_XML_MAP);
			}
			else 
			{
				SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_FILTERED_LOAD, M4O_M4OBJECT_XML_MAP);
			}
			
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::InitializeXmlMapM4O%979749966.body
}

m4return_t ClDMSXmlM4OMap::ReleaseXmlMapM4O ()
{
  //## begin ClDMSXmlM4OMap::ReleaseXmlMapM4O%979749967.body preserve=yes
	
	// Unload M4O - XML map channel info loaded.
	m4return_t iRet = M4_SUCCESS;
	m4VariantType	vRes;

	// Execute unload method (UNLOAD_INFO).
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	if (poNode->RecordSet.Current.Item[METHOD_UNLOAD_INFO].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_UNLOAD_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_UNLOAD_INFO].StackPop(vRes) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_UNLOAD_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check error type.
	if (vRes.Data.DoubleData != M4_SUCCESS)
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_UNLOAD_INFO, M4O_M4OBJECT_XML_MAP);
	
	// Release XML - M4O Map channel.
	if (m_bLoadByChannelHandle == M4_FALSE)
	{
		iRet = m_poChannelHandler->UnLoadChannel(m_poXmlMapAccess, m_poXmlMapChannel);
	}

	return iRet;
  //## end ClDMSXmlM4OMap::ReleaseXmlMapM4O%979749967.body
}

m4return_t ClDMSXmlM4OMap::GetXmlElementMapInfo (const string &ai_sXmlElementKey, const string &ai_sCurrentNode, m4uint32_t &ao_uiElementType, string &ao_sElementId, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod)
{
  //## begin ClDMSXmlM4OMap::GetXmlElementMapInfo%979749968.body preserve=yes
	m4VariantType	vXmlElementKey, vCurrentNodeId;
	m4VariantType	vElementType, vElementId, vRes;
	m4VariantType	vIdPreprocessMethod, vIdPostprocessMethod;

	// Execute ln4 method (GET_XML_ELEMENT_MAP_INF) to obtain the 
	// element type (node or item) and element id for the xml element.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// XML key.
	vXmlElementKey.Type = M4CL_CPP_TYPE_STRING_VAR;
	vXmlElementKey.Data.PointerChar = (m4pchar_t) ai_sXmlElementKey.c_str();

	// Current node.
	vCurrentNodeId.Type = M4CL_CPP_TYPE_STRING_VAR;
	vCurrentNodeId.Data.PointerChar = (m4pchar_t) ai_sCurrentNode.c_str();

	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vXmlElementKey) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vCurrentNodeId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vElementType) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vElementId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vIdPreprocessMethod) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPush(vIdPostprocessMethod) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPop (vElementType) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPop (vElementId) != M4_SUCCESS ||	
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPop (vIdPreprocessMethod) != M4_SUCCESS ||	
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPop (vIdPostprocessMethod) != M4_SUCCESS ||	
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ELEMENT_MAP_INFO].StackPop (vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		if (m_bElementConsistency == M4_TRUE)
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		}
		else
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, WARNINGLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		}
		return M4_ERROR;
	}

	ao_sElementId = "";
	ao_sIdPreprocessMethod = "";
	ao_sIdPostprocessMethod = "";
	
	ao_uiElementType = (m4int32_t) vElementType.Data.DoubleData;
	
	if ((vElementId.Type != M4CL_CPP_TYPE_NULL) && (vElementId.Data.PointerChar != NULL))
	{
		ao_sElementId = vElementId.Data.PointerChar;
	}
	
	// Bug 0264560. Métodos preprocesamiento y postprocesamiento del elemento.
	if ((vIdPreprocessMethod.Type != M4CL_CPP_TYPE_NULL) && (vIdPreprocessMethod.Data.PointerChar != NULL))
	{
		ao_sIdPreprocessMethod = vIdPreprocessMethod.Data.PointerChar;
	}
	
	if ((vIdPostprocessMethod.Type != M4CL_CPP_TYPE_NULL) && (vIdPostprocessMethod.Data.PointerChar != NULL))
	{
		ao_sIdPostprocessMethod = vIdPostprocessMethod.Data.PointerChar;
	}

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetXmlElementMapInfo%979749968.body
}

m4return_t ClDMSXmlM4OMap::GetXmlAttributeMapInfo (const string &ai_sXmlElementKey, const string &ai_sAttributeOf, const string &ai_sCurrentNode, string &ao_sElementId)
{
  //## begin ClDMSXmlM4OMap::GetXmlAttributeMapInfo%979749969.body preserve=yes
	m4VariantType	vXmlElementKey, vAttributeOf, vCurrentNodeId;
	m4VariantType	vElementId, vRes;

	// Execute ln4 method (GET_XML_ATTRIBUTE_MAP_INF) to obtain the 
	// id item associated to xml element.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// XML key.
	vXmlElementKey.Type = M4CL_CPP_TYPE_STRING_VAR;
	vXmlElementKey.Data.PointerChar = (m4pchar_t) ai_sXmlElementKey.c_str();

	// Attribute of (XML element which belongs to).
	vAttributeOf.Type = M4CL_CPP_TYPE_STRING_VAR;
	vAttributeOf.Data.PointerChar = (m4pchar_t) ai_sAttributeOf.c_str();

	// Current node.
	vCurrentNodeId.Type = M4CL_CPP_TYPE_STRING_VAR;
	vCurrentNodeId.Data.PointerChar = (m4pchar_t) ai_sCurrentNode.c_str();

	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPush(vXmlElementKey) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPush(vAttributeOf) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPush(vCurrentNodeId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPush(vElementId) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ATTRIBUTE_MAP_INF, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ATTRIBUTE_MAP_INF, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPop(vElementId) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_XML_ATTRIBUTE_MAP_INF].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ATTRIBUTE_MAP_INF, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		if (m_bElementConsistency == M4_TRUE)
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ATTRIBUTE_MAP_INF, M4O_M4OBJECT_XML_MAP);
		}
		else
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, WARNINGLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ATTRIBUTE_MAP_INF, M4O_M4OBJECT_XML_MAP);
		}
		
		return M4_ERROR;
	}

	if ((vElementId.Type != M4CL_CPP_TYPE_NULL) && (vElementId.Data.PointerChar != NULL))
	{
		ao_sElementId = vElementId.Data.PointerChar;
	}
	else
	{
		ao_sElementId = "";
	}

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetXmlAttributeMapInfo%979749969.body
}

m4return_t ClDMSXmlM4OMap::GetRootElementInfo (string &ao_sXmlRootElementName, string &ao_sRootNodeId)
{
  //## begin ClDMSXmlM4OMap::GetRootElementInfo%980239724.body preserve=yes
	m4VariantType	vXmlRootElementName, vRootNodeId, vRes;

	// Execute ln4 method (GET_ROOT_ELEMENT_INFO) to obtain the 
	// root element information. This is xml element name and node id 
	// (it can be null).
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	if (poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].StackPush(vXmlRootElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].StackPush(vRootNodeId) != M4_SUCCESS) 
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1#%s#%s#", METHOD_GET_ROOT_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_ROOT_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].StackPop(vXmlRootElementName) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].StackPop(vRootNodeId) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_ROOT_ELEMENT_INFO].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_ROOT_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_XML_ELEMENT_MAP_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	ao_sXmlRootElementName = "";
	ao_sRootNodeId = "";
	if ((vXmlRootElementName.Type != M4CL_CPP_TYPE_NULL) && (vXmlRootElementName.Data.PointerChar != NULL))
		ao_sXmlRootElementName = vXmlRootElementName.Data.PointerChar;
	
	if ((vRootNodeId.Type != M4CL_CPP_TYPE_NULL) && (vRootNodeId.Data.PointerChar != NULL))
		ao_sRootNodeId = vRootNodeId.Data.PointerChar;

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetRootElementInfo%980239724.body
}

m4return_t ClDMSXmlM4OMap::GetBaseElementInfo (string &ao_sXmlBaseElementName, string &ao_sBaseElementKey)
{
  //## begin ClDMSXmlM4OMap::GetBaseElementInfo%980239725.body preserve=yes
	m4VariantType	vXmlBaseElementName, vBaseElementKey, vRes;

	// Execute ln4 method (GET_BASE_ELEMENT_INFO) to obtain the 
	// base element information. This is xml base element name and 
	// id list for key base element.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	if (poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].StackPush(vXmlBaseElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].StackPush(vBaseElementKey) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_BASE_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_BASE_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].StackPop(vXmlBaseElementName) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].StackPop(vBaseElementKey) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_BASE_ELEMENT_INFO].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_BASE_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_BASE_ELEMENT_INFO, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	ao_sXmlBaseElementName = "";
	ao_sBaseElementKey = "";
	if ((vXmlBaseElementName.Type != M4CL_CPP_TYPE_NULL) && (vXmlBaseElementName.Data.PointerChar != NULL))
		ao_sXmlBaseElementName = vXmlBaseElementName.Data.PointerChar;

	if ((vBaseElementKey.Type != M4CL_CPP_TYPE_NULL) && (vBaseElementKey.Data.PointerChar != NULL))
		ao_sBaseElementKey = vBaseElementKey.Data.PointerChar;

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetBaseElementInfo%980239725.body
}

m4return_t ClDMSXmlM4OMap::GetDtdFileName (string &ao_sDtdFileName)
{
  //## begin ClDMSXmlM4OMap::GetDtdFileName%982251402.body preserve=yes
	m4VariantType	vDtdFileName;

	// Execute ln4 method (GET_ROOT_ELEMENT) to obtain the 
	// xml root element name.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	if (poNode->RecordSet.Current.Item[METHOD_GET_DTD_FILE_NAME].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_DTD_FILE_NAME, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_DTD_FILE_NAME].StackPop(vDtdFileName) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_DTD_FILE_NAME, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	ao_sDtdFileName = "";
	if ((vDtdFileName.Type != M4CL_CPP_TYPE_NULL) && (vDtdFileName.Data.PointerChar != NULL))
		ao_sDtdFileName = vDtdFileName.Data.PointerChar;

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetDtdFileName%982251402.body
}

m4return_t ClDMSXmlM4OMap::LoadAttributesOfElement (const string &ai_sCurrentNodeName, const string &ai_sAttributeOf, string &ao_sListHandle)
{
  //## begin ClDMSXmlM4OMap::LoadAttributesOfElement%982575345.body reserve=yes
	m4VariantType	vCurrentNodeName, vAttributeOf, vAttListHandle, vRes;

	// Execute ln4 method (LOAD_ATTRIBUTES_OF_ELEMENT) to load 
	// attributes information.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// Current node id.
	vCurrentNodeName.Type = M4CL_CPP_TYPE_STRING_VAR;
	vCurrentNodeName.Data.PointerChar = (m4pchar_t) ai_sCurrentNodeName.c_str();

	// Current node id.
	vAttributeOf.Type = M4CL_CPP_TYPE_STRING_VAR;
	vAttributeOf.Data.PointerChar = (m4pchar_t) ai_sAttributeOf.c_str();

	if (poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].StackPush(vCurrentNodeName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].StackPush(vAttributeOf) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].StackPush(vAttListHandle) != M4_SUCCESS) 
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_ATTRIBUTES_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_ATTRIBUTES_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].StackPop(vAttListHandle) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_ATTRIBUTES_OF_ELEMENT].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_ATTRIBUTES_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_ATTRIBUTES_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	ao_sListHandle = "";
	if ((vAttListHandle.Type != M4CL_CPP_TYPE_NULL) && (vAttListHandle.Data.PointerChar != NULL))
		ao_sListHandle = vAttListHandle.Data.PointerChar;

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::LoadAttributesOfElement%982575345.body
}

m4return_t ClDMSXmlM4OMap::LoadSubElementsOfElement (const string &ai_sCurrentNodeName, string &ao_sSubElemListHandle, m4uint32_t &ao_sXmlStructType)
{
  //## begin ClDMSXmlM4OMap::LoadSubElementsOfElement%982575346.body preserve=yes
	m4VariantType	vCurrentNodeName, vSubElemListHandle, vXmlStructType, vRes;

	// Execute ln4 method (LOAD_SUBELEMENTS_OF_ELEMENT) to load 
	// subelements info.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// Current node id.
	// Root element does not exist. Find child nodes.
	if (ai_sCurrentNodeName == "")
	{
		vCurrentNodeName.Type = M4CL_CPP_TYPE_NULL;
		vCurrentNodeName.Data.PointerChar = (m4pchar_t) ai_sCurrentNodeName.c_str();
	}
	else
	{
		vCurrentNodeName.Type = M4CL_CPP_TYPE_STRING_VAR;
		vCurrentNodeName.Data.PointerChar = (m4pchar_t) ai_sCurrentNodeName.c_str();
	}

	if (poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPush(vCurrentNodeName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPush(vSubElemListHandle) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPush(vXmlStructType) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_SUBELEMENTS_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_SUBELEMENTS_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPop(vSubElemListHandle) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPop(vXmlStructType) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_LOAD_SUBELEMENTS_OF_ELEMENT].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_SUBELEMENTS_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_LOAD_SUBELEMENTS_OF_ELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	ao_sSubElemListHandle = "";
	ao_sXmlStructType = 0;
	if ((vSubElemListHandle.Type != M4CL_CPP_TYPE_NULL) && (vSubElemListHandle.Data.PointerChar != NULL))
		ao_sSubElemListHandle = vSubElemListHandle.Data.PointerChar;

	if (vXmlStructType.Type != M4CL_CPP_TYPE_NULL)
		ao_sXmlStructType = (m4uint32_t) vXmlStructType.Data.DoubleData;

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::LoadSubElementsOfElement%982575346.body
}

m4return_t ClDMSXmlM4OMap::GetSubElementInfo (const string &ai_sSubElemListHandle, const m4bool_t &ai_bXmlInOrder, string &ao_sElementId, m4uint32_t &ao_iElementType, string &ao_sXmlElementName, m4bool_t & ao_bIsNodeValue, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod)
{
  //## begin ClDMSXmlM4OMap::GetSubElementInfo%982575347.body preserve=yes
	m4VariantType	vSubElemListHandle, vXmlInOrder, vElementId, vElementType;
	m4VariantType	vXmlElementName, vIsNodeValue, vIgnoreEmptyValues, vRes;
	m4VariantType	vIsComment;
	m4VariantType	vIdPreprocessMethod, vIdPostprocessMethod;

	// Execute ln4 method (GET_NEXT_SUBELEMENT) to load 
	// subelements info.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// SubElement list handle.
	vSubElemListHandle.Type = M4CL_CPP_TYPE_STRING_VAR;
	vSubElemListHandle.Data.PointerChar = (m4pchar_t) ai_sSubElemListHandle.c_str();

	vXmlInOrder.Type = M4CL_CPP_TYPE_NUMBER;
	vXmlInOrder.Data.DoubleData = ai_bXmlInOrder;

	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vSubElemListHandle) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vXmlInOrder) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vElementId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vElementType) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vXmlElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vIsNodeValue) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vIgnoreEmptyValues) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vIsComment) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vIdPreprocessMethod) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPush(vIdPostprocessMethod) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_SUBELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_SUBELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vElementId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vElementType) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vXmlElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vIsNodeValue) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vIgnoreEmptyValues) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vIsComment) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vIdPreprocessMethod) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vIdPostprocessMethod) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_SUBELEMENT].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_SUBELEMENT, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		if (vRes.Data.DoubleData == M4_EOF)
		{
			return M4_EOF;
		}
		else
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_SUBELEMENT, M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
	}

	ao_sElementId = "";
	ao_iElementType = 0;
	ao_sXmlElementName = "";
	ao_bIsNodeValue = M4_FALSE;
	ao_bIgnoreEmptyValues = M4_FALSE;
	ao_bIsComment = M4_FALSE;
	ao_sIdPreprocessMethod = "";
	ao_sIdPostprocessMethod = "";
	if ((vElementId.Type != M4CL_CPP_TYPE_NULL) && (vElementId.Data.PointerChar != NULL))
	{
		ao_sElementId = vElementId.Data.PointerChar;
	}

	if (vElementType.Type != M4CL_CPP_TYPE_NULL)
	{
		ao_iElementType = (m4uint32_t) vElementType.Data.DoubleData;
	}

	if ((vXmlElementName.Type != M4CL_CPP_TYPE_NULL) && (vXmlElementName.Data.PointerChar != NULL))
	{
		ao_sXmlElementName = vXmlElementName.Data.PointerChar;
	}

	// Bug 0144242. Indicar si este elemento debe ser tratado como el valor del
	// nodo padre.
	if (vIsNodeValue.Type != M4CL_CPP_TYPE_NULL && vIsNodeValue.Data.DoubleData == 1)
	{
		ao_bIsNodeValue = M4_TRUE;
	}

	// Bug 0144482. Indicar si se deben volcar valores vacios.
	if (vIgnoreEmptyValues.Type != M4CL_CPP_TYPE_NULL && vIgnoreEmptyValues.Data.DoubleData == 1)
	{
		ao_bIgnoreEmptyValues = M4_TRUE;
	}

	// Bug 0258811. Indicar si el item es un comentario XML.
	if (vIsComment.Type != M4CL_CPP_TYPE_NULL && vIsComment.Data.DoubleData == 1)
	{
		ao_bIsComment = M4_TRUE;
	}

	// Bug 0264153. Métodos preprocesamiento y postprocesamiento del elemento.
	if ((vIdPreprocessMethod.Type != M4CL_CPP_TYPE_NULL) && (vIdPreprocessMethod.Data.PointerChar != NULL))
	{
		ao_sIdPreprocessMethod = vIdPreprocessMethod.Data.PointerChar;
	}

	if ((vIdPostprocessMethod.Type != M4CL_CPP_TYPE_NULL) && (vIdPostprocessMethod.Data.PointerChar != NULL))
	{
		ao_sIdPostprocessMethod = vIdPostprocessMethod.Data.PointerChar;
	}

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetSubElementInfo%982575347.body
}

m4return_t ClDMSXmlM4OMap::GetAttributeInfo (const string &ai_sAttributeListHandle, string &ao_sElementId, string &ao_sXmlElemenName, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment)
{
  //## begin ClDMSXmlM4OMap::GetAttributeInfo%982575349.body preserve=yes
	m4VariantType	vAttListHandle, vElementId, vXmlElementName ,vRes;
	m4VariantType	vIgnoreEmptyValues;
	m4VariantType	vIsComment;

	// Execute ln4 method (GET_NEXT_ATTRIBUTE) to load 
	// attributes info.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// Attribute list handle.
	vAttListHandle.Type = M4CL_CPP_TYPE_STRING_VAR;
	vAttListHandle.Data.PointerChar = (m4pchar_t) ai_sAttributeListHandle.c_str();

	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPush(vAttListHandle) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPush(vElementId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPush(vXmlElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPush(vIgnoreEmptyValues) != M4_SUCCESS || 
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPush(vIsComment) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_ATTRIBUTE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_ATTRIBUTE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPop(vElementId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPop(vXmlElementName) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPop(vIgnoreEmptyValues) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPop(vIsComment) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_GET_NEXT_ATTRIBUTE].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_ATTRIBUTE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		if (vRes.Data.DoubleData == M4_EOF)
		{
			return M4_EOF;
		}	
		else
		{
			SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_GET_NEXT_ATTRIBUTE, M4O_M4OBJECT_XML_MAP);
			return M4_ERROR;
		}
	}

	ao_sElementId = "";
	ao_sXmlElemenName = "";
	ao_bIgnoreEmptyValues = M4_FALSE;
	ao_bIsComment = M4_FALSE;
	if ((vElementId.Type != M4CL_CPP_TYPE_NULL) && (vElementId.Data.PointerChar != NULL))
	{
		ao_sElementId = vElementId.Data.PointerChar;
	}

	if ((vXmlElementName.Type != M4CL_CPP_TYPE_NULL) && (vXmlElementName.Data.PointerChar != NULL))
	{
		ao_sXmlElemenName = vXmlElementName.Data.PointerChar;
	}

	// Bug 0144482. Indicar si se deben volcar valores vacios.
	if (vIgnoreEmptyValues.Type != M4CL_CPP_TYPE_NULL && vIgnoreEmptyValues.Data.DoubleData == 1)
	{
		ao_bIgnoreEmptyValues = M4_TRUE;
	}

	// Bug 0258811. Indicar si el elemento es un comentario XML.
	if (vIsComment.Type != M4CL_CPP_TYPE_NULL && vIsComment.Data.DoubleData == 1)
	{
		ao_bIsComment = M4_TRUE;
	}

	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::GetAttributeInfo%982575349.body
}

m4return_t ClDMSXmlM4OMap::CheckItemsScope (const string &ai_sCurrentNodeId, m4int32_t &aio_iItemsScopeRegister, m4int32_t &aio_iItemsScopeBlockNode)
{
  //## begin ClDMSXmlM4OMap::CheckItemsScope%1109161577.body preserve=yes
	m4VariantType	vNodeId, vItemsScopeRegister, vItemsScopeBlockNode, vRes;

	// Execute ln4 method (CHECK_ITEMS_SCOPE).
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Push method parameters.
	// Attribute list handle.
	vNodeId.Type = M4CL_CPP_TYPE_STRING_VAR;
	vNodeId.Data.PointerChar = (m4pchar_t) ai_sCurrentNodeId.c_str();
	if (poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPush(vNodeId) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPush(vItemsScopeRegister) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPush(vItemsScopeBlockNode) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_ITEMS_SCOPE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if (poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_ITEMS_SCOPE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPop(vItemsScopeRegister) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPop(vItemsScopeBlockNode) != M4_SUCCESS ||
		poNode->RecordSet.Current.Item[METHOD_CHECK_ITEMS_SCOPE].StackPop(vRes) != M4_SUCCESS)
	{	
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_ITEMS_SCOPE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check result.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_ITEMS_SCOPE, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	if ((vItemsScopeRegister.Type != M4CL_CPP_TYPE_NULL) && (vItemsScopeBlockNode.Type != M4CL_CPP_TYPE_NULL))
	{
		aio_iItemsScopeRegister = vItemsScopeRegister.Data.DoubleData;
		aio_iItemsScopeBlockNode = vItemsScopeBlockNode.Data.DoubleData;
		return M4_SUCCESS;
	}
	else
	{
		return M4_ERROR;
	}
  //## end ClDMSXmlM4OMap::CheckItemsScope%1109161577.body
}

m4return_t ClDMSXmlM4OMap::CheckNodesConsistency ()
{
  //## begin ClDMSXmlM4OMap::CheckNodesConsistency%1140186372.body preserve=yes
	m4VariantType	vRes;

	// Execute consistency nodes method.
	ClNode * poNode = &(m_poXmlMapAccess->Node[NODE_M4OBJECT_XML_MAP]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG, "#*s1*#%s#%s#", M4O_M4OBJECT_XML_MAP, NODE_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	if (poNode->RecordSet.Current.Item[METHOD_CHECK_NODES_CONSISTENCY].Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_NODES_CONSISTENCY, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	// Get executing method result.
	if (poNode->RecordSet.Current.Item[METHOD_CHECK_NODES_CONSISTENCY].StackPop(vRes) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_NODES_CONSISTENCY, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}

	// Check error type.
	if (vRes.Data.DoubleData != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", METHOD_CHECK_NODES_CONSISTENCY, M4O_M4OBJECT_XML_MAP);
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
  //## end ClDMSXmlM4OMap::CheckNodesConsistency%1140186372.body
}

// Additional Declarations
  //## begin ClDMSXmlM4OMap%3A66AF770301.declarations preserve=yes
  //## end ClDMSXmlM4OMap%3A66AF770301.declarations

//## begin module%3A66B5A7025F.epilog preserve=yes
//## end module%3A66B5A7025F.epilog
