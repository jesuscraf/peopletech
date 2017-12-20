//## begin module%3A8A6F4A023A.cm preserve=no
//## end module%3A8A6F4A023A.cm

//## begin module%3A8A6F4A023A.cp preserve=no
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
//## end module%3A8A6F4A023A.cp

//## Module: m4dmsm4objecttoxml%3A8A6F4A023A; Package body
//## Subsystem: M4DMSXml::src%3A3E5D2D0159
//## Source file: C:\m4server\m4dmsxml\src\m4dmsm4objecttoxml.cpp

//## begin module%3A8A6F4A023A.additionalIncludes preserve=no
//## end module%3A8A6F4A023A.additionalIncludes

//## begin module%3A8A6F4A023A.includes preserve=yes
// access
#include <access.hpp>
// channel
#include <channel.hpp>
// dm
#include <dm.hpp>
// m4dmsxmlexception
#include <m4dmschannelhandler.hpp>
// m4dmsxmlm4omap
#include <m4dmsxmlm4omap.hpp>
// Errors
#include <m4dmsxmlerr.hpp>

//## end module%3A8A6F4A023A.includes

// m4date
#include <m4date.hpp>
// m4dmsm4objecttoxml
#include <m4dmsm4objecttoxml.hpp>
#include "m4unicode.hpp"
#include "m4regimpl.hpp"

extern int	M4IsUnicode(void);

//## begin module%3A8A6F4A023A.declarations preserve=no
//## end module%3A8A6F4A023A.declarations

//## begin module%3A8A6F4A023A.additionalDeclarations preserve=yes
#define MAX_SIZE_NUMBER 2048
//## end module%3A8A6F4A023A.additionalDeclarations


// Class ClDMSM4ObjectToXml 
















ClDMSM4ObjectToXml::ClDMSM4ObjectToXml (const m4bool_t &ai_bDoValidation, const string &ai_sValidationFilePath, const m4bool_t ai_bUseSchema, const string &ai_sSchemaNamespace)
  //## begin ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.hasinit preserve=no
      : m_bDoValidation(ai_bDoValidation), m_bUseSchema(ai_bUseSchema)
  //## end ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.hasinit
  //## begin ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.initialization preserve=yes
  //## end ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.initialization
{
  //## begin ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.body preserve=yes
	m_sXmlData = "";
	m_sSchemaNamespace = ai_sSchemaNamespace;
	m_sValidationFilePath = ai_sValidationFilePath;
	m_pccDateSeparator = "/";
  //## end ClDMSM4ObjectToXml::ClDMSM4ObjectToXml%982251392.body
}


ClDMSM4ObjectToXml::~ClDMSM4ObjectToXml ()
{
  //## begin ClDMSM4ObjectToXml::~ClDMSM4ObjectToXml%982251393.body preserve=yes
	if (m_poXmlMap != NULL)
		delete m_poXmlMap;

	if (m_poChannelHandler != NULL)
		delete m_poChannelHandler;

	// Delete node info lists (subelements list and attributes list).
	DeleteNodeSubElementList();
	DeleteNodeAttributesList();
  //## end ClDMSM4ObjectToXml::~ClDMSM4ObjectToXml%982251393.body
}



//## Other Operations (implementation)
m4return_t ClDMSM4ObjectToXml::InitEnv (ClAccess *ai_poSource)
{
  //## begin ClDMSM4ObjectToXml::InitEnv%982251394.body preserve=yes
	if (ai_poSource == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#ai_poSource#");
		return M4_ERROR;
	}

	// Get the channel associated to that access.
	ClChannel * poChannel = ai_poSource->GetpChannel();
	if (poChannel == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poChannel#");
		return M4_ERROR;
	}
	
	// Get the channel manager associated to that channel.
	ClChannelManager * poChannelManager = poChannel->GetpChannelManager();
	if (poChannelManager == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poChannelManager#");
		return M4_ERROR;
	}

	/* Bug 0125594
	Se lee el separador de fechas
	*/
	ClVMBaseEnv*	poEnvironment = poChannelManager->GetEnvironment() ;

	if( poEnvironment != NULL )
	{
		m_pccDateSeparator = poEnvironment->GetDateSeparator() ;
	}

	// Por si acaso es nulo
	if( m_pccDateSeparator == NULL )
	{
		m_pccDateSeparator = "/" ;
	}

	// Create xml - m4o map object.
	// Create channel handler.
	m_poChannelHandler = new ClDMSChannelHandler(poChannelManager);
	m_poXmlMap = new ClDMSXmlM4OMap(m_poChannelHandler);

	// Get the channel associated to that access.
	m_poExportAccess = ai_poSource;
	m_poExportChannel = poChannel;

	m4pcchar_t pcNameChannel = ai_poSource->GetpChannel()->GetpChannelDef()->Id();
	if (pcNameChannel == NULL)
	{
		SETCODEF(M4_DMS_GET_NAME_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	
	// Save export channel name.
	m_sExportChannelName = pcNameChannel;
	
	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::InitEnv%982251394.body
}

m4return_t ClDMSM4ObjectToXml::InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency)
{
  //## begin ClDMSM4ObjectToXml::InitializeXmlMapM4O%982251395.body preserve=yes
	// Create and load XML - M4O Map data.
	return m_poXmlMap->InitializeXmlMapM4O(ai_iMapType, ai_bDoValidation, ai_sXMLFilePath, m_sExportChannelName, ai_bDefaultMappings, ai_iChannelHandle, ai_bElementConsistency);
  //## end ClDMSM4ObjectToXml::InitializeXmlMapM4O%982251395.body
}

m4return_t ClDMSM4ObjectToXml::ReleaseEnv ()
{
  //## begin ClDMSM4ObjectToXml::ReleaseEnv%982251397.body preserve=yes
	// Destroy XML - M4O Map channel.
	return m_poXmlMap->ReleaseXmlMapM4O();
  //## end ClDMSM4ObjectToXml::ReleaseEnv%982251397.body
}

m4return_t ClDMSM4ObjectToXml::AddXmlHeader (const string &ai_sXmlRootElementName, const m4bool_t ai_bDoPublic, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::AddXmlHeader%982251400.body preserve=yes
	m4return_t		iRet;
	string			sXmlVersionInfo;

	switch (ai_eEncoding)
	{
	case M4XmlUtf8:

		sXmlVersionInfo = M4Utf8Header();
		break;

	case M4XmlANSI:
		
		sXmlVersionInfo = M4ANSIHeader();
		break;

	case M4XmlISO:

		sXmlVersionInfo = M4ISOHeader();
		break;

	case M4XmlNative:
	default:

		sXmlVersionInfo = M4XMLHeader();
		break;

	}

	// Enhacement 0103615.
	// string sComment = "<!-- created by Meta4 -->";

	// fix bug 0090163
	m4pcchar_t pcScope = (ai_bDoPublic == M4_TRUE) ? " PUBLIC " : " SYSTEM ";

	// Reference to DTD file in XML file is only necessary if
	// validation has been requested.
	if (m_bDoValidation == M4_TRUE && m_bUseSchema == M4_FALSE)
	{
		string sDtdFileName, sDtdInfo;
		sDtdInfo = "";
		
		// Get DTD file name.
		iRet = m_poXmlMap->GetDtdFileName(sDtdFileName);
		if (iRet != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		
		// Build dtd info.
		if (m_sValidationFilePath != "")
			sDtdInfo = "<!DOCTYPE " + ai_sXmlRootElementName + pcScope + "\"" + m_sValidationFilePath + "/" + sDtdFileName + "\"" + ">";
		else
			sDtdInfo = "<!DOCTYPE " + ai_sXmlRootElementName + pcScope + "\"" + sDtdFileName + "\"" + ">";
		
		// Write data.
        // fix bug 0182695
		m_fXmlStreamData << sXmlVersionInfo;
		m_fXmlStreamData << sDtdInfo  << endl;
		m_fXmlStreamData << flush;
	}
	else
	{
		// Write data.
        // fix bug 0182695
		m_fXmlStreamData << sXmlVersionInfo;
		m_fXmlStreamData << flush;
	}

	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::AddXmlHeader%982251400.body
}

m4return_t ClDMSM4ObjectToXml::StartExportProcess (const string &ai_sXMLFile, const m4bool_t ai_bDoPublic, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::StartExportProcess%982251399.body preserve=yes

	m4return_t iRet = M4_SUCCESS;
	
	// Prepare to write to XML file.
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	M4Open( m_fXmlStreamData, ai_sXMLFile.c_str(), M4UniWrite, eEncoding ) ;

	if (!m_fXmlStreamData)
	{
		SETCODEF(M4_DMS_OPEN_FILE_ERROR, ERRORLOG, "#*s1*#%s#", ai_sXMLFile.c_str());
		return M4_ERROR;
	}

	// Get base element name.
	string sXmlBaseElementName = "";
	string sDummy;
	iRet = m_poXmlMap->GetBaseElementInfo(sXmlBaseElementName, sDummy);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_BASE_ELEMENT_INFO_ERROR, ERRORLOG, "#*s1*#");
		m_fXmlStreamData.close();
		return M4_ERROR;
	}
	
	// Save XML base element name.
	m_sXmlBaseElement = sXmlBaseElementName;

	// Get XML root element info.
	string sRootNodeId;
	iRet = m_poXmlMap->GetRootElementInfo(m_sXmlRootElement, sRootNodeId);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ROOT_ELEMENT_INFO_ERROR, ERRORLOG, "#*s1*#");
		m_fXmlStreamData.close();
		return M4_ERROR;
	}

	if (m_sXmlRootElement.empty() == M4_TRUE)
	{
		m_sXmlRootElement = m_sExportChannelName;
		sRootNodeId = "";
	}
	
	// Add XML header.
	iRet = AddXmlHeader(m_sXmlRootElement, ai_bDoPublic, ai_eEncoding);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_DTD_FILE_NAME, ERRORLOG,"#*s1*#");
		m_fXmlStreamData.close();
		return M4_ERROR;
	}

	// Process channel elements.
	m4uint32_t	uiInstanceNumber = 0, uiTabNumber = 0;
	m4bool_t	bDoJustBlockNodeItems = M4_FALSE;
	string		sIdPreprocessMethod = "";
	string		sIdPostprocessMethod = "";
	iRet = ProcessElement(sRootNodeId, uiInstanceNumber, m_sXmlRootElement, sRootNodeId, M4_NODE_TYPE, uiTabNumber, bDoJustBlockNodeItems, M4_FALSE, M4_FALSE, M4_FALSE, sIdPreprocessMethod, sIdPostprocessMethod, ai_eEncoding);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PROCESS_ELEM_ERROR, ERRORLOG,"#*s1*#%s#", sRootNodeId.c_str());
		m_fXmlStreamData.close();
		M4RemoveFile(ai_sXMLFile.c_str());
		return M4_ERROR;
	}
	
	// Close XML stream.
	m_fXmlStreamData.flush();
	m_fXmlStreamData.close();

	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::StartExportProcess%982251399.body
}

m4return_t ClDMSM4ObjectToXml::ProcessElement (const string &ai_sCurrentNodeName, const m4int32_t &ai_iInstanceNumber, const string &ai_sXmlElementName, const string &ai_sElementName, const m4int32_t &ai_iElementType, const m4int32_t &ai_iTabNumber, m4bool_t ai_bDoJustBlockNodeItems, m4bool_t ai_bIsNodeValue, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bIsComment, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::ProcessElement%982251403.body preserve=yes
	m4return_t iRet;
	m4int32_t iTab ;
	ClNode * poCurrentNode = NULL;

	// Obtain number of tabs for this element.
	m4pchar_t pcIndent = new m4char_t[ai_iTabNumber + 1];
	for (iTab = 0; iTab < ai_iTabNumber; iTab++)
	{
		pcIndent[iTab] = '\t';
	}
	pcIndent[iTab] = '\0';
	
	// Check if XML root node exits as a root node in the export
	// channel. If it exists, process it as a node element. If it 
	// doesn't exist, just get XML root node name and write it to
	// the XML file.
	m4bool_t bXmlRootNodeExist = M4_TRUE;
	if (!strcmp(ai_sXmlElementName.c_str(), m_sXmlRootElement.c_str()) &&
		(ai_sElementName == ""))
	{
		// Root node does not exist.

		// We use XML Schema to validate, build XML Schema validation information.
		string sSchemaInformation;
		if ((m_bDoValidation == M4_TRUE) && (m_bUseSchema == M4_TRUE))
		{
			iRet = BuildXMLSchemaInformation(sSchemaInformation);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_BUILD_XML_SCHEMA_INFO, ERRORLOG,"#*s1*#");
				delete pcIndent;
				pcIndent = NULL;
				return M4_ERROR;
			}
		}

		// Write XML root element in XML file.
		string sXmlRootNodeData;
		sXmlRootNodeData = M4_XML_OPEN_ELEMENT + ai_sXmlElementName + sSchemaInformation + M4_XML_CLOSE_ELEMENT;
		m_fXmlStreamData << pcIndent << sXmlRootNodeData << flush;

		bXmlRootNodeExist = M4_FALSE;
	}

	// Check if the element exits in the export channel. If it does not
	// exist, it must be the case of XML root node not existing in the expor channel.
	if (bXmlRootNodeExist == M4_TRUE)
	{
		// Get the current node.
		poCurrentNode = &(m_poExportAccess->Node[ai_sCurrentNodeName.c_str()]);
		if (poCurrentNode == NULL)
		{
			SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG,"#*s1*#%s#", ai_sCurrentNodeName.c_str());
			delete pcIndent;
			pcIndent = NULL;
			return M4_ERROR;
		}
	}
	
	// Check if the element is a node or an item.
	if (ai_iElementType == M4_NODE_TYPE)
	{
		iRet = ProcessNodeElement(ai_sXmlElementName, ai_sElementName, poCurrentNode, ai_sCurrentNodeName, ai_iTabNumber, bXmlRootNodeExist, ai_sIdPreprocessMethod, ai_sIdPostprocessMethod, ai_eEncoding);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_PROCESS_ELEM_ERROR, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
			delete pcIndent;
			pcIndent = NULL;
			return M4_ERROR;
		}
	}
	else	// M4_ITEM.
	{
		iRet = ProcessItemElement(ai_sXmlElementName, ai_sElementName, poCurrentNode, ai_sCurrentNodeName, ai_iInstanceNumber, pcIndent, ai_bDoJustBlockNodeItems, ai_bIsNodeValue, ai_bIgnoreEmptyValues, ai_bIsComment, ai_eEncoding);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_PROCESS_ELEM_ERROR, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
			delete pcIndent;
			pcIndent = NULL;
			return M4_ERROR;
		}
	}

	delete pcIndent;
	pcIndent = NULL;
		
	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::ProcessElement%982251403.body
}

m4return_t ClDMSM4ObjectToXml::ProcessNodeElement (const string &ai_sXmlElementName, const string &ai_sElementName, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const m4int32_t &ai_iTabNumber, m4bool_t ai_bXmlRootNode, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::ProcessNodeElement%984388355.body preserve=yes
	m4return_t	iRet = M4_SUCCESS;
	m4int32_t	iInstanceNumber = 1;
	m4int32_t	iItemsScopeRegister = 0;
	m4int32_t	iItemsScopeBlockNode = 0;
	m4bool_t	bOnlyBlockNodeScopeItems = M4_FALSE;
	m4return_t	iPreprocessMethodRet = M4_XML_EXECUTION_METHOD_OK;
	m4return_t	iPostprocessMethodRet = M4_XML_EXECUTION_METHOD_OK;

	// Get number of instances for that node, just if it is not 
	// the root node (because this node has just one register).
	if (ai_sXmlElementName != m_sXmlRootElement)
	{
		// Recuperar los items de ámbito registro del nodo y los items de ámbito bloque / nodo.
		iRet = m_poXmlMap->CheckItemsScope(ai_sCurrentNodeName, iItemsScopeRegister, iItemsScopeBlockNode);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_CHECK_ITEMS_SCOPE, ERRORLOG,"#*s1*#%s#", ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}
		
		// Bug 0111599: we can have a node without items, but with child nodes 
		// (iItemsScopeRegister = 0 and iItemsScopeBlockNode = 0).
		// In this case, we could have "n" registers in the parent node and we need 
		// to process them all.
		if ((iItemsScopeRegister != 0) || ((iItemsScopeRegister == 0) && (iItemsScopeBlockNode == 0)))
		{
			// Bug 0115326.
			iInstanceNumber = ai_poCurrentNode->RecordSet.Current.Count();
		}
	}

	// Aunque no se tengan registros en el nodo, se procesa para volcar los items de ámbito nodo.
	if ((iInstanceNumber == 0) && (iItemsScopeBlockNode != 0))
	{
		bOnlyBlockNodeScopeItems = M4_TRUE;
		iInstanceNumber = 1;
	}
	
	// Go through all registers.
	for (m4int32_t iInstance = 0; iInstance < iInstanceNumber; iInstance++)
	{
		if (ai_poCurrentNode != NULL)
		{
			ai_poCurrentNode->RecordSet.Current.MoveTo(iInstance);
			
			// Bug 0264153. Call preprocess method (only valid for node type elements).
			iRet = m_poChannelHandler->ExecuteMethod(ai_poCurrentNode, ai_sCurrentNodeName, ai_sIdPreprocessMethod, M4_EXPORT_XML_PROCESS, iPreprocessMethodRet);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_EXECUTE_PREPROCESS_METHOD_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sIdPreprocessMethod.c_str(), ai_sCurrentNodeName.c_str());
				return M4_ERROR;
			}
			
			// Check returned value.
			// 1: everything is ok.
			// 0: ignore this element.
			// -1: abort export process.
			if (iPreprocessMethodRet == M4_XML_IGNORE_REGISTER || iPreprocessMethodRet == M4_XML_ABORT_PROCESS)
			{
				// Call postprocess method.
				iRet = m_poChannelHandler->ExecuteMethod(ai_poCurrentNode, ai_sCurrentNodeName, ai_sIdPostprocessMethod, M4_EXPORT_XML_PROCESS, iPostprocessMethodRet);
				if (iRet != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_EXECUTE_POSTPROCESS_METHOD_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sIdPostprocessMethod.c_str(), ai_sCurrentNodeName.c_str());
					return M4_ERROR;
				}
				
				// Check postprocess method returned value.
				// 1: everything is ok.
				// -1: abort export process.
				if (iPostprocessMethodRet == M4_XML_ABORT_PROCESS)
				{
					SETCODEF(M4_DMS_XML_ABORT_PROCESS, ERRORLOG,"#*s1*#");
					return M4_ERROR;
				}
				
				// Check preprocess method return value.
				if (iPreprocessMethodRet == M4_XML_IGNORE_REGISTER)
				{
					continue;
				}
				else if (iPreprocessMethodRet == M4_XML_ABORT_PROCESS)
				{
					SETCODEF(M4_DMS_XML_ABORT_PROCESS, ERRORLOG,"#*s1*#");
					return M4_ERROR;
				}
			}
		}

		iRet = ProcessRegister(iInstance, ai_iTabNumber, ai_poCurrentNode, ai_sCurrentNodeName, ai_sXmlElementName, ai_sElementName, ai_bXmlRootNode, bOnlyBlockNodeScopeItems, ai_sIdPreprocessMethod, ai_sIdPostprocessMethod, ai_eEncoding);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_PROCESS_REGISTER_ERROR, ERRORLOG,"#*s1*#%s#%d#", ai_sCurrentNodeName.c_str(), iInstanceNumber);
			return M4_ERROR;
		}
		
		if (ai_poCurrentNode != NULL)
		{			
			// Bug 0264153. Call postprocess method.
			// 1: everything is ok.
			// -1: abort export process.
			iRet = m_poChannelHandler->ExecuteMethod(ai_poCurrentNode, ai_sCurrentNodeName, ai_sIdPostprocessMethod, M4_EXPORT_XML_PROCESS, iPostprocessMethodRet);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_EXECUTE_POSTPROCESS_METHOD_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sIdPostprocessMethod.c_str(), ai_sCurrentNodeName.c_str());
				return M4_ERROR;
			}
			
			if (iPostprocessMethodRet == M4_XML_ABORT_PROCESS)
			{
				SETCODEF(M4_DMS_XML_ABORT_PROCESS, ERRORLOG,"#*s1*#");
				return M4_ERROR;
			}
		}
	}
	
	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::ProcessNodeElement%984388355.body
}

m4return_t ClDMSM4ObjectToXml::ProcessItemElement (const string &ai_sXmlElementName, const string &ai_sElementName, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const m4int32_t &ai_iInstanceNumber, m4pchar_t ai_pcIndent, m4bool_t ai_bOnlyBlockNodeScopeItems, m4bool_t ai_bIsNodeValue, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bIsComment, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::ProcessItemElement%984388356.body preserve=yes
	m4return_t		iRet;
	m4VariantType	vItemValue;
	string			sItemValue;
	string			sCommentValue;
	m4bool_t		bIsWritable = M4_FALSE;
	m4int32_t		iYear, iMonth, iDay, iHour, iMin, iSec;
	string			sXmlAttData = "";
	m4uint8_t		uiItemScope = 0;
	
	// Get item scope.
	if (m_poChannelHandler->GetItemScope(ai_poCurrentNode, ai_sElementName, ai_sCurrentNodeName, uiItemScope) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_SCOPE_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sCurrentNodeName.c_str());
		return M4_ERROR;
	}

	// We will parse just items with node scope if (ai_bOnlyBlockNodeScopeItems = true).
	if ((ai_bOnlyBlockNodeScopeItems == M4_TRUE) && (uiItemScope == M4CL_ITEM_SCOPE_REGISTER))
	{
		return M4_SUCCESS;
	}

	// Move to instance.
	ai_poCurrentNode->RecordSet.Current.MoveTo(ai_iInstanceNumber);
	
	// Obtain and build attribute xml data.
	// Here, it is not important if root node exist or not because this element
	// could not be a node, it is an item.
	m4bool_t bXmlRootNode = M4_FALSE;
	iRet = BuildAttXmlData(ai_poCurrentNode, ai_sCurrentNodeName, ai_sElementName, ai_sXmlElementName, M4_ITEM_TYPE, ai_pcIndent, bXmlRootNode, ai_bIgnoreEmptyValues, ai_bOnlyBlockNodeScopeItems, ai_eEncoding, sXmlAttData);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_BUILD_ATT_VALUE, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
		return M4_ERROR;
	}
	
	// Get item value.
	iRet = ai_poCurrentNode->RecordSet.Current.Item[ai_sElementName.c_str()].Value.Get(vItemValue);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sCurrentNodeName.c_str());
		return M4_ERROR;
	}

	// Bug 0144482. Verificar si el usuario quiere tratar los valores nulos o vacíos
	// del item.
	if (IgnoreNullOrEmptyValues(vItemValue, ai_bIgnoreEmptyValues) == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	// Check if this item has value != NULL. In this case, it could be written.
	m4char_t pBuffer[MAX_SIZE_NUMBER];
	M4ClTimeStamp dateValue;
	M4ClString sDateValue;
	string sItemAuxValue;
	m4double_t dItemValue;
	m4uint8_t uiItemScale, uiItemM4Type;
	m4uint32_t uiItemPrec;

	switch (vItemValue.Type)
	{
	case M4CL_CPP_TYPE_STRING_VAR:
	case M4CL_CPP_TYPE_STRING:
		if (vItemValue.Data.PointerChar != NULL)
		{
			sItemAuxValue = vItemValue.Data.PointerChar;
			
			// Check for special xml characters and change them for the corresponding
			// representation in xml.
			BuildWellFormedXmlData(sItemAuxValue, ai_eEncoding, sItemValue);
			bIsWritable = M4_TRUE;
		}
		break;
	case M4CL_CPP_TYPE_NUMBER:
		// Get item scale to find out how to format the item value.
		if (m_poChannelHandler->GetItemPrecScale(ai_poCurrentNode, ai_sElementName, ai_sCurrentNodeName, uiItemPrec, uiItemScale) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_ITEM_PREC_SCALE_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}

		dItemValue = vItemValue.Data.DoubleData;
		sprintf(pBuffer, "%.*f", uiItemScale, dItemValue);
		sItemValue = pBuffer;
		bIsWritable = M4_TRUE;
		break;
	case M4CL_CPP_TYPE_DATE:
		// Get item m4type to find out how to format a date value.
		if (m_poChannelHandler->GetItemM4Type(ai_poCurrentNode, ai_sElementName, ai_sCurrentNodeName, uiItemM4Type) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_ITEM_M4TYPE_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}

		dateValue.set(vItemValue.Data.DoubleData);
		dateValue.get(iYear, iMonth, iDay, iHour, iMin, iSec);

		/* Bug 0125594
		Se tiene en cuenta el separador de fechas
		*/
		if (uiItemM4Type == M4CL_M4_TYPE_TIMESTAMP)
		{
			sprintf(pBuffer, "%.4d%s%.2d%s%.2d %.2d:%.2d:%.2d", iYear, m_pccDateSeparator, iMonth, m_pccDateSeparator, iDay, iHour, iMin, iSec);
		}
		else if(uiItemM4Type == M4CL_M4_TYPE_HOUR)
		{
			sprintf(pBuffer, "%.2d:%.2d:%.2d", iHour, iMin, iSec);
		}
		else
		{
			sprintf(pBuffer, "%.4d%s%.2d%s%.2d", iYear, m_pccDateSeparator, iMonth, m_pccDateSeparator, iDay);
		}

		sItemValue =  pBuffer;
		bIsWritable = M4_TRUE;
		break;
	}
	
	// Save XML item value if its writtable or if it is not writtable,
	// at least, has attributes.
	if (bIsWritable)
	{
		// Build XML string (attributes + item value).
		if (ai_bIsComment == M4_TRUE)
		{
			if (vItemValue.Type == M4CL_CPP_TYPE_STRING_VAR || vItemValue.Type == M4CL_CPP_TYPE_STRING)
			{
				// Comments: no escaped characters.
				sCommentValue = sItemAuxValue;
			}
			else
			{
				sCommentValue = sItemValue;
			}

			m_sXmlData = m_sXmlData + M4_CARRIAGE_RETURN + ai_pcIndent + M4_XML_OPEN_COMMENT + sCommentValue + M4_XML_CLOSE_COMMENT;
		}
		// Bug 0144242. Permitir tratar un item como el valor de un nodo.
		else if (ai_bIsNodeValue == M4_FALSE)
		{
			m_sXmlData = m_sXmlData + sXmlAttData + sItemValue + M4_XML_OPEN_END_ELEMENT + ai_sXmlElementName + M4_XML_CLOSE_ELEMENT;
		}
		else
		{
			m_sXmlData = m_sXmlData + sItemValue;
		}
	}
	else
	{
		// Check if this item has attributes. If it has, write it with its attributes.
		// If it hasn't got attributes and it is not writtable (it has not value),
		// write an empty element.
		m_sXmlData = m_sXmlData + sXmlAttData + M4_XML_OPEN_END_ELEMENT + ai_sXmlElementName + M4_XML_CLOSE_ELEMENT;
	}

	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::ProcessItemElement%984388356.body
}

m4return_t ClDMSM4ObjectToXml::ProcessRegister (const m4int32_t &ai_iRegister, const m4int32_t &ai_iTabNumber, ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const string &ai_sXmlElementName, const string &ai_sElementName, m4bool_t ai_bXmlRootNode, m4bool_t ai_bOnlyBlockNodeScopeItems, const string & ai_sIdPreprocessMethod, const string & ai_sIdPostprocessMethod, eUniXmlEncodingType_t ai_eEncoding)
{
  //## begin ClDMSM4ObjectToXml::ProcessRegister%1123661605.body preserve=yes
	m4return_t	iRet = M4_SUCCESS;
	m4return_t	iSubElemRet = M4_SUCCESS;
	string		sXmlAttData, sSubElemListHandle;
	string		sSubElementName, sXmlSubElementName, sCurrentNode;
	m4uint32_t	uiXmlStructType = 0;
	m4uint32_t	uiSubElementType = 0;
	m4uint32_t	uiSubElementNum = 0;
	m4bool_t	bXmlInOrder = M4_FALSE;	// M4_XML_STRUCTURE_OP
	m4bool_t	bIsNodeValue = M4_FALSE;
	m4bool_t	bIgnoreEmptyValues = M4_FALSE;
	m4bool_t	bIsComment = M4_FALSE;
	m4bool_t	bIsEof = M4_FALSE;
	string		sIdPreprocessMethod;
	string		sIdPostprocessMethod;
	m4uint32_t	uiNodeHandle = 0;

	// Obtain number of tabs for this element.
	m4int32_t iTab = 0;
	m4pchar_t pcIndent = new m4char_t[ai_iTabNumber + 1];
	for (iTab = 0; iTab < ai_iTabNumber; iTab++)
	{
		pcIndent[iTab] = '\t';
	}
	pcIndent[iTab] = '\0';
	
	// Root node exists: build root node data.
	if (ai_bXmlRootNode == M4_TRUE)
	{
		// Obtain and build attribute xml data.
		sXmlAttData = "";
		iRet = BuildAttXmlData(ai_poCurrentNode, ai_sCurrentNodeName, ai_sElementName, ai_sXmlElementName, M4_NODE_TYPE, pcIndent, ai_bXmlRootNode, M4_FALSE, ai_bOnlyBlockNodeScopeItems, ai_eEncoding, sXmlAttData);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_BUILD_ATT_VALUE, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
			delete pcIndent;
			pcIndent = NULL;
			return M4_ERROR;
		}
	}

	// If root element, print it.
	if (!strcmp(ai_sXmlElementName.c_str(), m_sXmlRootElement.c_str()))
	{
		if (ai_bXmlRootNode)
		{
			m_fXmlStreamData << sXmlAttData << flush;
		}
	}
	else
	{
		// Save attribute xml data.
		m_sXmlData = m_sXmlData + sXmlAttData;
	}
	
	// Bug 0273192. Obtener el handle del nodo.
	if (ai_bXmlRootNode == M4_TRUE)
	{
		if (m_poChannelHandler->GetNodeHandle(ai_poCurrentNode, ai_sCurrentNodeName, uiNodeHandle) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_NODE_HANDLE_ERROR, ERRORLOG,"#*s1*#%s#", ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}
	}
	
	// Check if this node element is in the node list.
	// If this node is not in the list, load subelement node info
	// and save them. If it is in the list, get the corresponding information.	
	stElementInfo * infoTemp = NULL;
	infoTemp = FindSubElementInList(uiNodeHandle);
	if (infoTemp == NULL)
	{
		// Get subelements of this element (it could be another nodes or items).
		iRet = LoadSubElementsOfElement(ai_sCurrentNodeName, sSubElemListHandle, uiXmlStructType);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_LOAD_ELEM_SUBELEMENT, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
			delete pcIndent;
			pcIndent = NULL;
			return M4_ERROR;
		}
		
		// Save it in node subelement info list.
		AddSubElementInfo(ai_sElementName, uiNodeHandle, sSubElemListHandle, uiXmlStructType);
	}
	else
	{
		// Get list handle for this node and xml structure type.
		infoTemp->GetElementInfo(sSubElemListHandle, uiXmlStructType);
	}
	
	// Check XML order.
	if (uiXmlStructType == M4_XML_STRUCTURE_SEC)
	{
		bXmlInOrder = M4_TRUE;
	}
	
	// Get subelement info.
	while (bIsEof != M4_TRUE)
	{
		iSubElemRet = GetSubElementInfo(sSubElemListHandle, bXmlInOrder, sSubElementName, uiSubElementType, sXmlSubElementName, bIsNodeValue, bIgnoreEmptyValues, bIsComment, sIdPreprocessMethod, sIdPostprocessMethod);
		if (iSubElemRet != M4_SUCCESS)
		{
			// Check if this is an error or there is no more
			// subelements.
			if (iSubElemRet == M4_EOF)
			{
				bIsEof = M4_TRUE;
				
				// There is no more subelements (items or nodes) for this element. Close it.
				// If the element hasn't got subelements, just end element without a carriage return. In
				// other case, close element as always.
				if (uiSubElementNum)
				{
					m_sXmlData = m_sXmlData + M4_CARRIAGE_RETURN + pcIndent + M4_XML_OPEN_END_ELEMENT + ai_sXmlElementName + M4_XML_CLOSE_ELEMENT;
				}
				else
				{
					m_sXmlData = m_sXmlData + M4_XML_OPEN_END_ELEMENT + ai_sXmlElementName + M4_XML_CLOSE_ELEMENT;
				}
				
				// Check if this is the xml base element. In case, write all xml data to xml file.
				m4bool_t bFlushData = M4_FALSE;
				if (m_sXmlBaseElement.empty() == M4_TRUE)
				{
					bFlushData = M4_TRUE;
				}
				else
				{
					if ((!strcmp(ai_sXmlElementName.c_str(), m_sXmlBaseElement.c_str()) || (!strcmp(ai_sXmlElementName.c_str(), m_sXmlRootElement.c_str()))))
					{
						bFlushData = M4_TRUE;
					}
				}
				
				if (bFlushData == M4_TRUE)
				{
					// Print XML data to XML file.
					m_fXmlStreamData << m_sXmlData << flush;
					
					// Initialize XML data again (for each base element).
					m_sXmlData = "";
				}
			}
			else
			{
				SETCODEF(M4_DMS_GET_SUBELEM_ERROR, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
				delete pcIndent;
				pcIndent = NULL;
				return M4_ERROR;
			}
		}
		else
		{
			// Increase subelement number.
			uiSubElementNum++;
			
			sCurrentNode = ai_sCurrentNodeName;
			if (uiSubElementType == M4_NODE_TYPE)
			{
				sCurrentNode = sSubElementName;
			}
			
			// Obtain subelements of this element and process them.
			iRet = ProcessElement(sCurrentNode, ai_iRegister, sXmlSubElementName, sSubElementName, uiSubElementType, ai_iTabNumber + 1, ai_bOnlyBlockNodeScopeItems, bIsNodeValue, bIgnoreEmptyValues, bIsComment, sIdPreprocessMethod, sIdPostprocessMethod, ai_eEncoding);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_PROCESS_ELEM_ERROR, ERRORLOG,"#*s1*#%s#", sSubElementName.c_str());
				delete pcIndent;
				pcIndent = NULL;
				return M4_ERROR;
			}
		}
	}	// while

	delete pcIndent;
	pcIndent = NULL;

	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::ProcessRegister%1123661605.body
}

m4return_t ClDMSM4ObjectToXml::LoadAttributesOfElement (const string &ai_sCurrentNodeName, const string &ai_sAttributeOf, string &ao_sListHandle)
{
  //## begin ClDMSM4ObjectToXml::LoadAttributesOfElement%982251404.body preserve=yes
	return m_poXmlMap->LoadAttributesOfElement(ai_sCurrentNodeName, ai_sAttributeOf, ao_sListHandle);
  //## end ClDMSM4ObjectToXml::LoadAttributesOfElement%982251404.body
}

m4return_t ClDMSM4ObjectToXml::LoadSubElementsOfElement (const string &ai_sCurrentNodeName, string &ao_sSubElemListHandle, m4uint32_t &ao_sXmlStructType)
{
  //## begin ClDMSM4ObjectToXml::LoadSubElementsOfElement%982575342.body preserve=yes
	return m_poXmlMap->LoadSubElementsOfElement(ai_sCurrentNodeName, ao_sSubElemListHandle, ao_sXmlStructType);
  //## end ClDMSM4ObjectToXml::LoadSubElementsOfElement%982575342.body
}

m4return_t ClDMSM4ObjectToXml::GetSubElementInfo (const string &ai_sSubElemListHandle, const m4bool_t &ai_bXmlInOrder, string &ao_sElementId, m4uint32_t &ao_iElementType, string &ao_sXmlElementName, m4bool_t & ao_bIsNodeValue, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod)
{
  //## begin ClDMSM4ObjectToXml::GetSubElementInfo%982575344.body preserve=yes
	return m_poXmlMap->GetSubElementInfo(ai_sSubElemListHandle, ai_bXmlInOrder, ao_sElementId, ao_iElementType, ao_sXmlElementName, ao_bIsNodeValue, ao_bIgnoreEmptyValues, ao_bIsComment, ao_sIdPreprocessMethod, ao_sIdPostprocessMethod);
  //## end ClDMSM4ObjectToXml::GetSubElementInfo%982575344.body
}

m4return_t ClDMSM4ObjectToXml::GetAttributeInfo (const string &ai_sAttributeListHandle, string &ao_sElementId, string &ao_sXmlElementName, m4bool_t & ao_bIgnoreEmptyValues, m4bool_t & ao_bIsComment)
{
  //## begin ClDMSM4ObjectToXml::GetAttributeInfo%982575348.body preserve=yes
	return m_poXmlMap->GetAttributeInfo(ai_sAttributeListHandle, ao_sElementId, ao_sXmlElementName, ao_bIgnoreEmptyValues, ao_bIsComment);
  //## end ClDMSM4ObjectToXml::GetAttributeInfo%982575348.body
}

m4return_t ClDMSM4ObjectToXml::BuildAttXmlData (ClNode *ai_poCurrentNode, const string &ai_sCurrentNodeName, const string &ai_sElementName, const string &ai_sXmlElementName, const m4uint32_t &ai_iElementType, m4pcchar_t ai_pcIndent, m4bool_t ai_bXmlRootNode, m4bool_t ai_bIgnoreEmptyValues, m4bool_t ai_bOnlyBlockNodeScopeItems, eUniXmlEncodingType_t ai_eEncoding, string &ao_sXmlAttData)
{
  //## begin ClDMSM4ObjectToXml::BuildAttXmlData%982575341.body preserve=yes
	m4return_t		iRet = M4_SUCCESS;
	m4return_t		iAttRet = M4_SUCCESS;
	m4bool_t		bIsWritable = M4_FALSE;
	string			sItemName, sItemXmlName, sItemValue, sAttListHandle;
	string			sItemAuxValue;
	string			sWellFormedItemValue;
	m4VariantType	vItemValue;
	m4bool_t		bIsEof = M4_FALSE;
	m4char_t		pBuffer[MAX_SIZE_NUMBER];
	M4ClTimeStamp	dateValue;
	M4ClString		sDateValue;
	m4double_t		dItemValue;	
	m4int32_t		iYear, iMonth, iDay, iHour, iMin, iSec;
	m4uint32_t		uiItemPrec;
	m4uint8_t		uiItemScale, uiItemM4Type;
	m4bool_t		bIgnoreEmptyValues = M4_FALSE;
	m4bool_t		bIsComment = M4_FALSE;
	m4uint8_t		uiItemScope = 0;
	string			sComments;
	string			sCommentValue;
	m4uint32_t		uiHandle = 0;

	// Initialize XML data. Carriage return is only necessary if this element
	// is the xml root node, and it does not exist in the channel.
	string sSchemaInformation;
	if ((ai_sXmlElementName == m_sXmlRootElement) && (ai_bXmlRootNode == M4_TRUE))
	{
		// If we use XML Schemas to validate, we need to print Schema information 
		// within root XML element (root attributes).
		if ((m_bDoValidation == M4_TRUE) && (m_bUseSchema == M4_TRUE))
		{
			iRet = BuildXMLSchemaInformation(sSchemaInformation);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_BUILD_XML_SCHEMA_INFO, ERRORLOG,"#*s1*#");
				return M4_ERROR;
			}
		}
		ao_sXmlAttData = ao_sXmlAttData + ai_pcIndent + M4_XML_OPEN_ELEMENT + ai_sXmlElementName + sSchemaInformation;
	}
	else
	{
		ao_sXmlAttData = ao_sXmlAttData + M4_CARRIAGE_RETURN + ai_pcIndent + M4_XML_OPEN_ELEMENT + ai_sXmlElementName;
	}

	// Bug 0273192. Obtener el handle del elemento.
	if (ai_iElementType == M4_ITEM_TYPE)
	{
		if (m_poChannelHandler->GetItemHandle(ai_poCurrentNode, ai_sElementName, ai_sCurrentNodeName, uiHandle) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_ITEM_HANDLE_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}
	}
	else
	{
		if (m_poChannelHandler->GetNodeHandle(ai_poCurrentNode, ai_sCurrentNodeName, uiHandle) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_NODE_HANDLE_ERROR, ERRORLOG,"#*s1*#%s#", ai_sCurrentNodeName.c_str());
			return M4_ERROR;
		}
	}

	// Check if this item element is in the attributes node list.
	// If it is not in the list, load attributes item info
	// and save them. If it is in the list, get the corresponding information.
	stElementInfo * infoTemp = NULL;
	infoTemp = FindAttributeInList(uiHandle);
	if (infoTemp == NULL)
	{
		// Get element attributes.
		iRet = LoadAttributesOfElement(ai_sCurrentNodeName, ai_sXmlElementName, sAttListHandle);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_LOAD_ELEMENT_ATT, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
			return M4_ERROR;
		}

		// Save it in node subelement info list.
		AddAttributeInfo(ai_sElementName, uiHandle, sAttListHandle);
	}
	else
	{
		m4uint32_t uiXmlStrucType = 0;
		infoTemp->GetElementInfo(sAttListHandle, uiXmlStrucType);
	}

	while (bIsEof != M4_TRUE)
	{
		bIsWritable = M4_FALSE;

		// Obtain all element attributes.
		iAttRet = GetAttributeInfo(sAttListHandle, sItemName, sItemXmlName, bIgnoreEmptyValues, bIsComment);
		if (iAttRet != M4_SUCCESS)
		{
			// Check if the attribute list is finished or 
			// it is an error.
			if (iAttRet == M4_EOF)
			{
				bIsEof = M4_TRUE;
			}
			else
			{
				SETCODEF(M4_DMS_GET_ELEMENT_ATT, ERRORLOG,"#*s1*#%s#", sItemName.c_str());
				return M4_ERROR;
			}
		}
		else
		{
			// Get item scope.
			if (m_poChannelHandler->GetItemScope(ai_poCurrentNode, sItemName, ai_sCurrentNodeName, uiItemScope) != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_GET_ITEM_SCOPE_ERROR, ERRORLOG,"#*s1*#%s#%s#", sItemName.c_str(), ai_sCurrentNodeName.c_str());
				return M4_ERROR;
			}
			
			// We will parse just items with node scope if (ai_bOnlyBlockNodeScopeItems = true).
			if ((ai_bOnlyBlockNodeScopeItems == M4_TRUE) && (uiItemScope == M4CL_ITEM_SCOPE_REGISTER))
			{
				continue;
			}

			if (ai_poCurrentNode->RecordSet.Current.Item[sItemName.c_str()].Value.Get(vItemValue) != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_GET_ITEM_ERROR, ERRORLOG,"#*s1*#%s#%s#", sItemName.c_str(), ai_sCurrentNodeName.c_str());
				return M4_ERROR;
			}

			// Bug 0144482. Verificar si el usuario quiere tratar los valores nulos o vacíos
			// del item.
			if (IgnoreNullOrEmptyValues(vItemValue, bIgnoreEmptyValues) == M4_TRUE)
			{
				continue;
			}
			
			switch (vItemValue.Type)
			{
			case M4CL_CPP_TYPE_STRING_VAR:
			case M4CL_CPP_TYPE_STRING:
				if (vItemValue.Data.PointerChar != NULL)
				{
					sItemAuxValue = vItemValue.Data.PointerChar;
			
					// Check for special xml characters and change them for the corresponding
					// representation in xml.
					BuildWellFormedXmlData(sItemAuxValue, ai_eEncoding, sItemValue);
					bIsWritable = M4_TRUE;
				}
				break;
			case M4CL_CPP_TYPE_NUMBER:
				// Get item scale to find out how to format the item value.
				if (m_poChannelHandler->GetItemPrecScale(ai_poCurrentNode, sItemName, ai_sCurrentNodeName, uiItemPrec, uiItemScale) != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_GET_ITEM_PREC_SCALE_ERROR, ERRORLOG,"#*s1*#%s#%s#", sItemName.c_str(), ai_sCurrentNodeName.c_str());
					return M4_ERROR;
				}

				dItemValue = vItemValue.Data.DoubleData;
				sprintf(pBuffer, "%.*f", uiItemScale, dItemValue);
				sItemValue = pBuffer;
				bIsWritable = M4_TRUE;
				break;
			case M4CL_CPP_TYPE_DATE:
				// Get item m4type to find out how to format a date value.
				if (m_poChannelHandler->GetItemM4Type(ai_poCurrentNode, sItemName, ai_sCurrentNodeName, uiItemM4Type) != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_GET_ITEM_M4TYPE_ERROR, ERRORLOG,"#*s1*#%s#%s#", sItemName.c_str(), ai_sCurrentNodeName.c_str());
					return M4_ERROR;
				}

				dateValue.set(vItemValue.Data.DoubleData);
				dateValue.get(iYear, iMonth, iDay, iHour, iMin, iSec);

				/* Bug 0125594
				Se tiene en cuenta el separador de fechas
				*/
				if (uiItemM4Type == M4CL_M4_TYPE_TIMESTAMP)
				{
					sprintf(pBuffer, "%.4d%s%.2d%s%.2d %.2d:%.2d:%.2d", iYear, m_pccDateSeparator, iMonth, m_pccDateSeparator, iDay, iHour, iMin, iSec);
				}
				else if(uiItemM4Type == M4CL_M4_TYPE_HOUR)
				{
					sprintf(pBuffer, "%.2d:%.2d:%.2d", iHour, iMin, iSec);
				}
				else
				{
					sprintf(pBuffer, "%.4d%s%.2d%s%.2d", iYear, m_pccDateSeparator, iMonth, m_pccDateSeparator, iDay);
				}

				sItemValue = pBuffer;
				bIsWritable = M4_TRUE;
				break;
			}

			// Build xml string data.
			// Check if attribute value has characters like single_quote (')
			// or double quote (").
			// If the attribute value has single quote inside, enclose it with
			// doble quote and if it has doble quote, enclose it with single quote.
			// If the item/attribute element has not got value (it is not writtable),
			// write an empty element.
			if (bIsWritable == M4_TRUE)
			{
				EncloseAttXmlData(sItemValue, sWellFormedItemValue);
				
				if (bIsComment == M4_TRUE)
				{
					if (vItemValue.Type == M4CL_CPP_TYPE_STRING_VAR || vItemValue.Type == M4CL_CPP_TYPE_STRING)
					{
						// Comments: no escaped characters.
						sCommentValue = sItemAuxValue;
					}
					else
					{
						sCommentValue = sItemValue;
					}
					sComments = sComments + M4_CARRIAGE_RETURN + ai_pcIndent + M4_XML_OPEN_COMMENT + sCommentValue + M4_XML_CLOSE_COMMENT;
				}
				else
				{
					ao_sXmlAttData = ao_sXmlAttData + M4_SPACE + sItemXmlName + M4_EQUAL + sWellFormedItemValue;
				}
			}
			else
			{
				ao_sXmlAttData = ao_sXmlAttData + M4_SPACE + sItemXmlName + M4_EQUAL + M4_DOBLE_QUOTE + M4_DOBLE_QUOTE;
			}
		}	// if (iAttRet != M4_SUCCESS)
	}	// while

	// Finish XML attribute data, M4_XML_CLOSE_ELEMENT (>). If the 
	// element is the XML root node, print it to file.
	// In case of item, we do not need carriage return because afterwards 
	// we want to add its value (if it is writable).
	ao_sXmlAttData = sComments + ao_sXmlAttData + M4_XML_CLOSE_ELEMENT;
	
	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::BuildAttXmlData%982575341.body
}

m4return_t ClDMSM4ObjectToXml::BuildXMLSchemaInformation (string &ao_sSchemaInformation)
{
  //## begin ClDMSM4ObjectToXml::BuildXMLSchemaInformation%1120050060.body preserve=yes
	// Get validation file name.
	string sValidationFileName;
	if (m_poXmlMap->GetDtdFileName(sValidationFileName) != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	// Build XML Schema information, necessary for validations.
	ao_sSchemaInformation = "";
	ao_sSchemaInformation.append(" ");
	if (m_sSchemaNamespace.empty() == M4_TRUE)
	{
		// XML Schema without namespaces.
		// xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation=".\listatelefonos.xsd"
		ao_sSchemaInformation.append(M4_XML_SCHEMA_INSTANCE);
		ao_sSchemaInformation.append(" ");
		ao_sSchemaInformation.append(M4_XML_SCHEMA_NO_NAMESPACE_SCHEMA_LOCATION);
		ao_sSchemaInformation.append(" =");
		ao_sSchemaInformation.append("\"" + m_sValidationFilePath + "/" + sValidationFileName + "\"");
	}
	else
	{
		// XML Schema with namespaces.
		// xmlns="http://www.meta4.com/schema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.meta4.com/schemas ./ListaTelefonos_ns.xsd"
		ao_sSchemaInformation.append(M4_XML_SCHEMA_XMLNS);
		ao_sSchemaInformation.append("=\"" + m_sSchemaNamespace + "\"");
		ao_sSchemaInformation.append(" ");
		ao_sSchemaInformation.append(M4_XML_SCHEMA_INSTANCE);
		ao_sSchemaInformation.append(" ");
		ao_sSchemaInformation.append(M4_XML_SCHEMA_LOCATION);
		ao_sSchemaInformation.append(" ");
		ao_sSchemaInformation.append("=\"" + m_sSchemaNamespace + " " + m_sValidationFilePath + "/" + sValidationFileName + "\"");
	}
	
	return M4_SUCCESS;
  //## end ClDMSM4ObjectToXml::BuildXMLSchemaInformation%1120050060.body
}

void ClDMSM4ObjectToXml::BuildWellFormedXmlData (const string &ai_sXmlData, eUniXmlEncodingType_t ai_eEncoding, string &ao_sWellFormedXmlData)
{
  //## begin ClDMSM4ObjectToXml::BuildWellFormedXmlData%982575343.body preserve=yes

	// UNICODE XML
	m4char_t	c = '\0' ;
	size_t		i = -1 ;
	int			iLength = -1 ;
	m4pcchar_t	pccInString = NULL ;
	m4pchar_t	pcTranslatedText = NULL;
	m4pcchar_t	pccScaped = NULL ;
	
	// Convertir el valor del elemento al encoding especificado.
	switch (ai_eEncoding)
	{
	case M4XmlUtf8:

		pcTranslatedText = M4CppToUtf8(ai_sXmlData.c_str(), iLength);
		break;

	case M4XmlANSI:
	case M4XmlISO:

		pcTranslatedText = M4CppToANSI(ai_sXmlData.c_str(), iLength);
		break;

	case M4XmlNative:
	default:

		if (M4IsUnicode() == 1)
		{
			pcTranslatedText = M4CppToUtf8(ai_sXmlData.c_str(), iLength);
		}
		else
		{
			pcTranslatedText = M4CppToANSI(ai_sXmlData.c_str(), iLength);
		}
		break;
	}

	/* Bug 0142804
	No se estaba limpiando la cadena y de ahí que salieran los valores de atributos concatenados
	*/
	ao_sWellFormedXmlData = "";
	ao_sWellFormedXmlData.reserve( iLength * 1.5 ) ;

	for( i = 0 ; i < iLength; i++ )
	{
		c = pcTranslatedText[ i ] ;

		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			ao_sWellFormedXmlData.append( pccScaped ) ;
		}
		else
		{
			ao_sWellFormedXmlData.append( &c, 1 ) ;
		}
	}

	delete[] pcTranslatedText;

  //## end ClDMSM4ObjectToXml::BuildWellFormedXmlData%982575343.body
}

void ClDMSM4ObjectToXml::EncloseAttXmlData (const string &ai_sXmlData, string &ao_sWellFormedXmlData)
{
  //## begin ClDMSM4ObjectToXml::EncloseAttXmlData%985938330.body preserve=yes
	m4bool_t bIsFound = M4_FALSE;
	m4pcchar_t pDest = strchr(ai_sXmlData.c_str(), M4_CH_SINGLE_QUOTE);
	if (pDest != NULL)
	{
		ao_sWellFormedXmlData = M4_DOBLE_QUOTE + ai_sXmlData + M4_DOBLE_QUOTE;
		bIsFound = M4_TRUE;
	}
	else
	{
		pDest = strchr(ai_sXmlData.c_str(), M4_CH_DOBLE_QUOTE);
		if (pDest != NULL)
		{
			ao_sWellFormedXmlData = M4_SINGLE_QUOTE + ai_sXmlData + M4_SINGLE_QUOTE;
			bIsFound = M4_TRUE;
		}
	}
	
	// Check if we have found any special characters. If not, write XML
	// data with doble quote.
	if (bIsFound == M4_FALSE)
		ao_sWellFormedXmlData = M4_DOBLE_QUOTE + ai_sXmlData + M4_DOBLE_QUOTE;
  //## end ClDMSM4ObjectToXml::EncloseAttXmlData%985938330.body
}

stElementInfo * ClDMSM4ObjectToXml::FindSubElementInList (const m4uint32_t &ai_uiElementHandle)
{
  //## begin ClDMSM4ObjectToXml::FindSubElementInList%988212319.body preserve=yes
	NodeSubElementList::iterator	it;
	stElementInfo *					pNodeInfo = NULL;
	m4bool_t						bFound = M4_FALSE;

	for (it = m_oNodeSubElementList.begin(); it != m_oNodeSubElementList.end(); it++)
	{
		if (((*it)->m_uiElementHandle) == ai_uiElementHandle)
		{
			bFound = M4_TRUE;
			pNodeInfo = (*it);
			break;
		}
	}

	return pNodeInfo;
  //## end ClDMSM4ObjectToXml::FindSubElementInList%988212319.body
}

stElementInfo * ClDMSM4ObjectToXml::FindAttributeInList (const m4uint32_t &ai_uiElementHandle)
{
  //## begin ClDMSM4ObjectToXml::FindAttributeInList%988618089.body preserve=yes
	ElementAttributesList::iterator		it;
	stElementInfo *						pAttInfo = NULL;
	m4bool_t							bFound = M4_FALSE;

	for (it = m_oAttributesList.begin(); it != m_oAttributesList.end(); it++)
	{
		if (((*it)->m_uiElementHandle) == ai_uiElementHandle)
		{
			bFound = M4_TRUE;
			pAttInfo = (*it);
			break;
		}
	}

	return pAttInfo;
  //## end ClDMSM4ObjectToXml::FindAttributeInList%988618089.body
}

void ClDMSM4ObjectToXml::AddSubElementInfo (const string &ai_sNodeName, const m4uint32_t &ao_uiNodeHandle, const string &ai_sNodeKey, const m4uint32_t &ai_uiXmlStructType)
{
  //## begin ClDMSM4ObjectToXml::AddSubElementInfo%988212320.body preserve=yes
	// Insert node subelement info (node name, list handle and xml structure
	// type).
	stElementInfo * stNodeSubElement = new stElementInfo(ai_sNodeName, ao_uiNodeHandle, ai_sNodeKey, ai_uiXmlStructType);
	m_oNodeSubElementList.push_back(stNodeSubElement);
  //## end ClDMSM4ObjectToXml::AddSubElementInfo%988212320.body
}

void ClDMSM4ObjectToXml::AddAttributeInfo (const string &ai_sElementName, const m4uint32_t &ai_uiElementHandle, const string &ai_sElementKey)
{
  //## begin ClDMSM4ObjectToXml::AddAttributeInfo%988618088.body preserve=yes
	// Insert node subelement info (node name, list handle and xml structure
	// type).
	stElementInfo * stAttElement = new stElementInfo(ai_sElementName, ai_uiElementHandle, ai_sElementKey, 0);
	m_oAttributesList.push_back(stAttElement);
  //## end ClDMSM4ObjectToXml::AddAttributeInfo%988618088.body
}

void ClDMSM4ObjectToXml::DeleteNodeSubElementList ()
{
  //## begin ClDMSM4ObjectToXml::DeleteNodeSubElementList%988212321.body preserve=yes
	NodeSubElementList::iterator it;

	it = m_oNodeSubElementList.begin();
	while (it != m_oNodeSubElementList.end())
	{
		delete (*it);
		it++;
	}

	m_oNodeSubElementList.clear();
  //## end ClDMSM4ObjectToXml::DeleteNodeSubElementList%988212321.body
}

void ClDMSM4ObjectToXml::DeleteNodeAttributesList ()
{
  //## begin ClDMSM4ObjectToXml::DeleteNodeAttributesList%988212322.body preserve=yes
	ElementAttributesList::iterator it;

	it = m_oAttributesList.begin();
	while (it != m_oAttributesList.end())
	{
		delete (*it);
		it++;
	}

	m_oAttributesList.clear();
  //## end ClDMSM4ObjectToXml::DeleteNodeAttributesList%988212322.body
}

// Additional Declarations
  //## begin ClDMSM4ObjectToXml%3A8A561000FD.declarations preserve=yes

m4bool_t ClDMSM4ObjectToXml::IgnoreNullOrEmptyValues(m4VariantType vItemValue, m4bool_t ai_bIgnoreEmptyValues)
{
	// El valor del item es nulo. Si se ha especificado que se quieren ignorar los valores
	// vacios (ai_bIgnoreEmptyValues = true), no se deben volcar estos items.
	// Se ignoran para el tipo de item cadena, los valores nulos o vacios.
	// Para el tipo número, los valores nulos.
	// Para el tipo fecha, los valores nulos y las fechas cero.
	if (ai_bIgnoreEmptyValues == M4_TRUE)
	{
		if (vItemValue.Type == M4CL_CPP_TYPE_NULL)
		{	
			return M4_TRUE;
		}
		else
		{
			// Puede ser que el valor del item no sea nulo, pero esté igualmente vacio.
			if ((vItemValue.Type == M4CL_CPP_TYPE_STRING_VAR || vItemValue.Type == M4CL_CPP_TYPE_STRING))
			{
				if (vItemValue.Data.PointerChar == "" || *(vItemValue.Data.PointerChar) == '\0')
				{
					return M4_TRUE;
				}
			}
			
			if (vItemValue.Type == M4CL_CPP_TYPE_DATE && vItemValue.Data.DoubleData == 0)
			{
				return M4_TRUE;
			}
		}
	}

	return M4_FALSE;
}
  //## end ClDMSM4ObjectToXml%3A8A561000FD.declarations

// Class stElementInfo 




stElementInfo::stElementInfo (const string &ai_sElementName, const m4uint32_t &ai_uiElementHandle, const string &ai_sElementKey, const m4uint32_t &ai_uiXmlStructType)
  //## begin stElementInfo::stElementInfo%988212316.hasinit preserve=no
      : m_sElementName(ai_sElementName), m_uiElementHandle(ai_uiElementHandle), m_sElementKey(ai_sElementKey), m_uiXmlStructType(ai_uiXmlStructType)
  //## end stElementInfo::stElementInfo%988212316.hasinit
  //## begin stElementInfo::stElementInfo%988212316.initialization preserve=yes
  //## end stElementInfo::stElementInfo%988212316.initialization
{
  //## begin stElementInfo::stElementInfo%988212316.body preserve=yes
  //## end stElementInfo::stElementInfo%988212316.body
}



//## Other Operations (implementation)
void stElementInfo::GetElementInfo (string &ao_sNodeKey, m4uint32_t &ao_XmlStructType)
{
  //## begin stElementInfo::GetElementInfo%988212318.body preserve=yes
	ao_sNodeKey = m_sElementKey;
	ao_XmlStructType = m_uiXmlStructType;
  //## end stElementInfo::GetElementInfo%988212318.body
}

// Additional Declarations
  //## begin stElementInfo%3AE6DB7E005C.declarations preserve=yes
  //## end stElementInfo%3AE6DB7E005C.declarations

//## begin module%3A8A6F4A023A.epilog preserve=yes
//## end module%3A8A6F4A023A.epilog
