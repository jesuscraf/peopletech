//## begin module%3A3E57A701A6.cm preserve=no
//## end module%3A3E57A701A6.cm

//## begin module%3A3E57A701A6.cp preserve=no
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
//## end module%3A3E57A701A6.cp

//## Module: m4dmssaxhandlers%3A3E57A701A6; Package body
//## Subsystem: M4DMSXml::src%3A3E5D2D0159
//## Source file: C:\m4server-xerces2.5.0\m4dmsxml\src\m4dmssaxhandlers.cpp

//## begin module%3A3E57A701A6.additionalIncludes preserve=no
//## end module%3A3E57A701A6.additionalIncludes

//## begin module%3A3E57A701A6.includes preserve=yes
// node
#include <node_x.hpp>
// access
#include <access.hpp>
// channel
#include <channel.hpp>
// dm
#include <dm.hpp>
// m4dmsxmlexception
#include <m4dmsxmlexception.hpp>
// m4dmsxmlm4omap
#include <m4dmsxmlm4omap.hpp>
// m4dmschannelhandler
#include <m4dmschannelhandler.hpp>
// m4date
#include <m4date.hpp>
// Errors
#include "m4dmsxmlerr.hpp"
// Defines
#include "m4dmsxmldefines.hpp"
// ClNodeDef
#include "nodedef.hpp"
//## end module%3A3E57A701A6.includes

// SAXHandlerBase
#include <xercesc/sax/HandlerBase.hpp>
// m4dmssaxhandlers
#include <m4dmssaxhandlers.hpp>
// unicode
#include "m4unicode.hpp"
//m4objglb
#include "m4objglb.hpp"

//## begin module%3A3E57A701A6.declarations preserve=no
//## end module%3A3E57A701A6.declarations

//## begin module%3A3E57A701A6.additionalDeclarations preserve=yes
//## end module%3A3E57A701A6.additionalDeclarations


// Class ClDMSSAXHandlers 






















ClDMSSAXHandlers::ClDMSSAXHandlers (const string &ai_sXMLFile, const m4int32_t &ai_iFirstRegister, const m4int32_t &ai_iLastRegister)
  //## begin ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.hasinit preserve=no
      : m_sXMLFile(ai_sXMLFile), m_bIsOk(M4_TRUE), m_iError(M4_SUCCESS), m_iFirstRegister(ai_iFirstRegister), m_iLastRegister(ai_iLastRegister), m_iCurrentRegister(-1), m_bInException(M4_FALSE), m_bRootXMLElementAlreadyParsed(M4_FALSE)
  //## end ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.hasinit
  //## begin ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.initialization preserve=yes
  //## end ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.initialization
{
  //## begin ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.body preserve=yes
	
	m_poCurrentNode = NULL;
	m_poImportChannel = NULL;
	m_poImportAccess = NULL;
	m_poXmlMap = NULL;
	m_poChannelHandler = NULL;
	m_sRecoveryElement = "";
	m_bElementConsistency = M4_TRUE;
	m_bItemValidation = M4_FALSE;
  //## end ClDMSSAXHandlers::ClDMSSAXHandlers%977216642.body
}


ClDMSSAXHandlers::~ClDMSSAXHandlers ()
{
  //## begin ClDMSSAXHandlers::~ClDMSSAXHandlers%977216643.body preserve=yes

	if (m_poXmlMap != NULL)
	{
		delete m_poXmlMap;
		m_poXmlMap = NULL;
	}

	if (m_poChannelHandler != NULL)
	{
		delete m_poChannelHandler;
		m_poChannelHandler = NULL;
	}

	// Delete the channel element list.
	DeleteChannelElementList();

	// Delete key base element list.
	DeleteKeyBaseElementList();
  //## end ClDMSSAXHandlers::~ClDMSSAXHandlers%977216643.body
}


//## Other Operations (implementation)
m4return_t ClDMSSAXHandlers::Init (ClAccess *ai_poSource)
{
  //## begin ClDMSSAXHandlers::Init%977216644.body preserve=yes

	if (ai_poSource == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#ai_poSource#");
		return M4_ERROR;
	}

	m_poImportAccess = ai_poSource;
	
	// Get the channel associated to that access.
	m_poImportChannel = m_poImportAccess->GetpChannel();
	if (m_poImportChannel == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG, "#*s1*#m_poImportChannel#");
		return M4_ERROR;
	}
	
	m4pcchar_t pcNameChannel = ai_poSource->GetpChannel()->GetpChannelDef()->Id();
	if (pcNameChannel == NULL)
	{
		SETCODEF(M4_DMS_GET_NAME_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	
	m_sImportChannelName = pcNameChannel;

	// Get the channel manager associated to that channel.
	ClChannelManager * poChannelManager =  m_poImportChannel->GetpChannelManager();
	if (poChannelManager == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poChannelManager#");
		return M4_ERROR;
	}

	// Create channel handler.
	m_poChannelHandler = new ClDMSChannelHandler(poChannelManager);
	if (m_poChannelHandler == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#m_poChannelHandler#");
		return M4_ERROR;
	}

	// Create xml - m4o map object.
	m_poXmlMap = new ClDMSXmlM4OMap(m_poChannelHandler);
	if (m_poXmlMap == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#m_poXmlMap#");
		return M4_ERROR;
	}

	return M4_SUCCESS;

  //## end ClDMSSAXHandlers::Init%977216644.body
}

void ClDMSSAXHandlers::characters(const XMLCh* const chars, const XMLSize_t length)
{
  //## begin ClDMSSAXHandlers::characters%977216647.body preserve=yes

	// Check if they are valid characters.
	m4bool_t	bValidChars = M4_TRUE;
	m4bool_t	bContinue = M4_FALSE;
	m4uint8_t	uiItemM4Type = M4CL_M4_TYPE_NULL;

	// UNICODE XML
	m4pchar_t pText = M4XMLToCpp( (M4XMLCh*)chars ) ;

	if (pText == NULL)			
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#");
		SetError();
		return;
	}
	
	bValidChars = CheckCharacters(pText);
	if (bValidChars == M4_TRUE)
	{
		if (m_iCurrentRegister < 0)
		{
			bContinue = M4_TRUE;
		}
		else if (m_iCurrentRegister >= m_iFirstRegister)
		{
			bContinue = M4_TRUE;
		}
	}
	else
	{
		bContinue = M4_FALSE;
	}

	// Continue only when valid characters and if we are in the first
	// (or next) register to be parsed.
	if (bContinue == M4_TRUE)
	{
		// Check if an error has happened. If there is an error, we have to 
		// wait until we recieve an startElement event for the base element.
		// This means that we are not going to parse until we recieve the 
		// base element specified by the user.
		if (CheckParserStatus() == M4_TRUE)
		{
			// Get the last element of the list.
			stChannelElementInfo *	infoTemp = NULL;
			string					sXmlItemName, sCurrentNodeName, sItemName;
			string					sItemValue, sLastItemValue;
			m4VariantType			vItemValue;
			ClNode *				poParentNode = NULL;
			
			if (m_oChannelElementList.empty() == M4_FALSE)
			{
				infoTemp = m_oChannelElementList.back();

				sXmlItemName = infoTemp->m_sXmlElementName;
				m4uint8_t iCppType = infoTemp->m_uiCppType;
				sItemName = infoTemp->m_sElementName;

				// There is a special case in which we recieve several 
				// characters events for the same element. This is the
				// case of entities defining special XML characters (&,
				// <, >). Extract the last value and join it to the new one.
				sLastItemValue = infoTemp->m_sElementValue;
				if (sLastItemValue != "")
				{
					string sAux = pText;
					sItemValue = sLastItemValue + sAux;
				}
				else
				{
					sItemValue = pText;
				}

				// Save item value into the element list.
				infoTemp->m_sElementValue = sItemValue;

				// Check if this item is one of the base element keys. 
				// If this is the case, save its value inside base element
				// keys list.
				SetKeyBaseElementValue(sItemName, sItemValue);

				// Get parent node name.
				// Bug 0270862
				poParentNode = infoTemp->m_poParent;
				if (poParentNode != NULL)
				{
					sCurrentNodeName = poParentNode->GetpNodeDef()->Id();
				}

				// Tipo meta4.
				if (m_poChannelHandler->GetItemM4Type(m_poCurrentNode, sItemName, sCurrentNodeName, uiItemM4Type) != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_GET_ITEM_M4TYPE_ERROR, ERRORLOG, "#*s1*#%s#%s#", sItemName.c_str(), sCurrentNodeName.c_str());
					SetError();
					delete[] pText;
					return;
				}

				// Save the given value. Item node will be current node and
				// the item name will be the last element introduces in the list.
				if (iCppType == M4CL_CPP_TYPE_NUMBER)
				{
					if (ValidateNumber(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, sItemValue, vItemValue) != M4_SUCCESS)
					{
						SetError();
						delete[] pText;
						return;
					}
				}
				else if (iCppType == M4CL_CPP_TYPE_STRING)
				{
					if (uiItemM4Type == M4CL_M4_TYPE_VARIANT)
					{
						if (ValidateVariant(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, sItemValue, vItemValue) != M4_SUCCESS)
						{
							SetError();
							delete[] pText;
							return;
						}
					}
					else
					{
						if (ValidateString(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, uiItemM4Type, sItemValue, vItemValue) != M4_SUCCESS)
						{
							SetError();
							delete[] pText;
							return;
						}
					}
				}
				else if (iCppType == M4CL_CPP_TYPE_DATE)
				{
					if (ValidateDate(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, uiItemM4Type, sItemValue, vItemValue) != M4_SUCCESS)
					{
						SetError();
						delete[] pText;
						return;
					}
				}
				else
				{
					SETCODEF(M4_DMS_ITEM_TYPE_ERROR, ERRORLOG, "#*s1*#%s#", sItemName.c_str());
					SetError();
					delete[] pText;
					return;
				}

				// Set the item value.
				if (m_poCurrentNode->RecordSet.Current.Item[sItemName.c_str()].Value.Set(vItemValue) != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_SET_ITEM_ERROR, ERRORLOG, "#*s1*#%s#", sItemName.c_str());
					SetError();
					delete[] pText;
					return;
				}
			}
		} // checkParserStatus
	}
	delete [] pText;

  //## end ClDMSSAXHandlers::characters%977216647.body
}


void ClDMSSAXHandlers::endElement (const XMLCh* const name)
{
  //## begin ClDMSSAXHandlers::endElement%977216648.body preserve=yes
	m4return_t		iRet = M4_SUCCESS;
	m4return_t		iPostprocessMethodRet = M4_XML_EXECUTION_METHOD_OK;
	string 			sPostprocessMethodId = "";
	string 			sElementName = "";

	// Only when we start to parse our first register.
	if (m_iCurrentRegister < 0 || m_iCurrentRegister >= m_iFirstRegister)
	{
		// UNICODE XML
		string sXmlElementName;
		M4XMLToSTL( (M4XMLCh*)name, sXmlElementName );

		// Delete the given element. It is not necessary to find that element
		// in the list. It will be always the last element saved in the list.
		// Check there is somethig in the list.
		if (m_oChannelElementList.empty() == M4_FALSE)
		{
			stChannelElementInfo *	infoTemp = NULL;
			m4bool_t				bDeleteRegister = M4_FALSE;
			m4return_t				iRet = M4_SUCCESS;

			// Get the last element of the list.
			infoTemp = m_oChannelElementList.back();
			sPostprocessMethodId = infoTemp->m_sPostprocessMethodId;
			sElementName = infoTemp->m_sElementName;

			// Check that the element to be deleted has the same name than
			// the recieved one.
			if (infoTemp->m_sXmlElementName == sXmlElementName)
			{
				// Delete that element from the list.
				m_oChannelElementList.pop_back();

				// Verificar el estado del registro.
				if (infoTemp->m_iStatus == M4_REGISTER_STATUS_DELETE)
				{
					iRet = m_poCurrentNode->RecordSet.Current.Delete();
					if (iRet != M4_SUCCESS)
					{
						SETCODEF(M4_DMS_DELETE_REGISTER_ERROR, ERRORLOG, "#*s1*#%s#", sElementName.c_str());
						return;
					}
				}

				// Check postprocess method returned value.
				// 1: everything is ok.
				// 0: ignore this element.	
				// -1: abort export process.
				// Call postprocess method.
				iRet = m_poChannelHandler->ExecuteMethod(m_poCurrentNode, sXmlElementName, sPostprocessMethodId, M4_IMPORT_XML_PROCESS, iPostprocessMethodRet);
				if (iRet != M4_SUCCESS)
				{
					SETCODEF(M4_DMS_EXECUTE_POSTPROCESS_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", sPostprocessMethodId.c_str(), sElementName.c_str());
					return;
				}

				if (iPostprocessMethodRet == M4_XML_ABORT_PROCESS)
				{
					SETCODEF(M4_DMS_XML_ABORT_PROCESS, ERRORLOG, "#*s1*#");

					// Throw exception.
					// When we throw our exception, the SAX parser catches it.
					// As it is not an exception known by the parser, the parser
					// throws the exception again.
					// Save that we are throwing our own exception.
					m_bInException = M4_TRUE;
					ThrowDMSXMLException(EndElement, m_iCurrentRegister);
				}
				else if (iPostprocessMethodRet == M4_XML_IGNORE_REGISTER)
				{
					iRet = m_poCurrentNode->RecordSet.Current.Destroy();
					if (iRet != M4_SUCCESS)
					{
						SETCODEF(M4_DMS_DELETE_REGISTER_ERROR, ERRORLOG, "#*s1*#%s#", sElementName.c_str());
						return;
					}
				}

				// If last element is a node, we have to change the current
				// node with the parent node of that element/node.
				// If last element is node and item type, we have to change
				// to the parent element of the last element introduced.
				if (infoTemp->m_uiElementType == M4_NODE_TYPE || infoTemp->m_uiElementType == M4_NODE_AND_ITEM_TYPE)
				{
					// Borrar el registro añadido. Esto se hace porque el nodo puede
					// tener items de ámbito registro o bloque, pero no tienen por qué
					// venir en el fichero XML.
					iRet = CheckDeleteRegister(infoTemp, bDeleteRegister);
					if (iRet == M4_SUCCESS && bDeleteRegister == M4_TRUE)
					{
						m_poCurrentNode->RecordSet.Current.Destroy();
					}

					// Actualizar el current node.
					m_poCurrentNode = infoTemp->m_poParent;

					// Borrar la lista de items del nodo.
					infoTemp->m_oItemsVector.clear();
				}
				else
				{
					// Guardar el item procesado para el nodo actual.
					AddProcessedItem(sXmlElementName);
				}

				// Delete last element.
				delete infoTemp;
			}
		}

		// Delete the base element block when there has been an error.
		// Only when endElement event for base element is recieved.
		if (IsOk() == M4_FALSE)
		{
			if (stricmp(sXmlElementName.c_str(), m_sXmlBaseElement.c_str()) == 0)
			{
				DeleteBaseElementBlock();
			}

			// Resetear errores.
			if (stricmp(sXmlElementName.c_str(), m_sRecoveryElement.c_str()) == 0)
			{
				m_bIsOk = M4_TRUE;
			}
		}
		
		// When we finish every base element block, release base element
		// list to start with another one with the new base element block.
		if (!stricmp(sXmlElementName.c_str(), m_sXmlBaseElement.c_str()))
		{
			ResetKeyBaseElementList();
		}

		// Find out if this is the last register to be parsed. In this case,
		// throw an exception to stop the parser.
		// By default, if not specify by the user, last register -1
		// and this means that we have to parse all registers.
		// The register is always referred to the base element block.
		if (m_iLastRegister >= 0 && 
			m_iCurrentRegister >= m_iLastRegister &&
			!stricmp(sXmlElementName.c_str(), m_sXmlBaseElement.c_str()))
		{
			// Throw exception.
			// When we throw our exception, the SAX parser catches it.
			// As it is not an exception known by the parser, the parser
			// throws the exception again.
			// Save that we are throwing our own exception.
			m_bInException = M4_TRUE;
			ThrowDMSXMLException(EndElement, m_iCurrentRegister);
		}
	}
  //## end ClDMSSAXHandlers::endElement%977216648.body
}

void ClDMSSAXHandlers::startElement (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClDMSSAXHandlers::startElement%977216649.body preserve=yes

	unsigned int	uiLen = 0;
	m4int32_t		iItemValue = 0;
	m4int8_t		iStatus = M4_REGISTER_STATUS_ADD;
	string			sXmlItemName, sItemValue;
	string			sPreprocessMethodId = "";
	string			sPostprocessMethodId = "";
	
	// UNICODE XML
	string sXmlElementName;
	M4XMLToSTL( (M4XMLCh*)name, sXmlElementName );

	// Bug 0264549. Permitir añadir, modificar y borrar registros.
	uiLen = attributes.getLength();
	if (uiLen > 0)
	{
		// UNICODE XML
		M4XMLToSTL( (M4XMLCh*)attributes.getName(0), sXmlItemName );

		if (sXmlItemName.compare("___systemrecordstatus___") == 0)
		{
			M4XMLToSTL( (M4XMLCh*)attributes.getValue((XMLSize_t)0), sItemValue );
			
			iItemValue = atoi(sItemValue.c_str());
			if (iItemValue == M4_REGISTER_STATUS_DELETE)
			{
				iStatus = M4_REGISTER_STATUS_DELETE;
			}
			else if (iItemValue == M4_REGISTER_STATUS_UPDATE)
			{
				iStatus = M4_REGISTER_STATUS_UPDATE;
			}
		}
	}

	// Find out if this is the element to start the process. It could be
	// the first element of the XML file or you could leave the "n" first
	// registers and parse the rest.
	// By default (if not specify by the user), the first register is 1.
	// The register is always referred to the base element block.
	if (m_sXmlBaseElement.empty() == M4_TRUE)
	{
		m_iCurrentRegister = 0;
	}
	else
	{
		if (!stricmp(sXmlElementName.c_str(), m_sXmlBaseElement.c_str()))
		{
			m_iCurrentRegister++;
		}
	}

	// m_bParseElement indicates if we have to parse the element.
	//		-	If it is the root node and if it exits in the import channel
	//			(m_sRootNodeId != ""), parse element. This is independent 
	//			of the current register.
	//		-	For any other element, parse it if it is the first register 
	//			to be parsed.
	// m_bRootXMLElementAlreadyParsed show if root XML element have been already parsed
	// or not. This is a new control due to default mappings. With defualt mappings, we
	// set root xml element to channel name. It could be the case where we have a data channel
	// with a root node with the name of the channel. In this case, we will process this node
	// like a root node and this is not a correct behaviour.
	m4bool_t bParseElement = M4_TRUE;

	// Bug 0142333.
	// Primero quiero ver si he tratado el nodo root XML. Esto es necesario por en el caso de 
	// mapeos por defecto, puede venir un nodo root XML "STD_PERSON" (en mapeos por defecto, 
	// siempre es el nombre del canal) y un nodo hijo que se llama igual (STD_PERSON, 
	// nodo root del canal). Para evitar posicionarse dos veces, una para el canal y 
	// otra para el nodo raíz del XML, se detecta si ya hemos parseado un elemento XML 
	// que sea el nodo root XML.
	if (m_bRootXMLElementAlreadyParsed == M4_FALSE)
	{
		if (m_sRootNodeId.empty() == M4_TRUE)
		{
			bParseElement = M4_FALSE;
		}
		m_bRootXMLElementAlreadyParsed = M4_TRUE;
	}
	else
	{
		if (m_iCurrentRegister >= 0 && m_iCurrentRegister < m_iFirstRegister)
		{
			bParseElement = M4_FALSE;
		}
	}

	// Verificar si este elemento tiene que ser parseado.
	if (bParseElement == M4_TRUE)
	{
		// Check if an error has happened. If there is an error, we have to 
		// wait until we recieve an startElement event for the base element.
		// This means that we are not going to parse until we recieve the 
		// base element specified by the user.
		if (CheckParserStatus() == M4_TRUE)
		{
			m4return_t			iRet = M4_ERROR;
			m4uint32_t			uiElementType = 0;
			m4uint8_t			uiCppType = M4CL_CPP_TYPE_NULL;
			string				sNodeName, sElementName, sAttributeOf;
			ClNode *			poParentNode = NULL;
			
			// Get parent node name.
			sNodeName = "";
			if (m_poCurrentNode != NULL)
			{
				sNodeName = m_poCurrentNode->GetpNodeDef()->Id();
			}

			iRet = GetXmlElementMapInfo(sXmlElementName, sNodeName, uiElementType, sElementName, sPreprocessMethodId, sPostprocessMethodId);
			if (iRet != M4_SUCCESS)
			{
				return;
			}
			
			// Check if the element has been found as a node or as an item.
			// If not, the element is wrong, it does not exist in the channel.
			if (uiElementType != M4_NODE_TYPE && uiElementType != M4_ITEM_TYPE && uiElementType != M4_NODE_AND_ITEM_TYPE)
			{
				SETCODEF(M4_DMS_FIND_NODE_ERROR, ERRORLOG,"#*s1*#%s%#", sXmlElementName.c_str());
				SetError();
				return;
			}
			
			// Save the current node as the parent node.
			poParentNode = m_poCurrentNode;
			if (uiElementType == M4_NODE_TYPE)
			{
				// Process node element.
				iRet = ProcessNodeElement(sElementName, sXmlElementName, iStatus, sPreprocessMethodId, sPostprocessMethodId);
			}
			else if (uiElementType == M4_ITEM_TYPE)
			{
				// Process item element.
				iRet = ProcessItemElement(sElementName, uiCppType);
			}
			else if (uiElementType == M4_NODE_AND_ITEM_TYPE)
			{
				// Process node and item element.
				iRet = ProcessNodeAndItemElement(sElementName, sXmlElementName, poParentNode, iStatus, sPreprocessMethodId, sPostprocessMethodId, uiCppType);
			}

			// Verificar si se ha podido procesar el elemento.
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_PROCESS_ELEMENT_ERROR, ERRORLOG,"#*s1*#%s#%d#", sXmlElementName.c_str(), uiElementType);
				m_poCurrentNode = poParentNode;
				SetError();
				return;
			}

			// Process element attributes.
			iRet = ProcessElementAttributes(sXmlElementName, attributes);
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_PROCESS_ATTRIBUTES_ERROR, ERRORLOG,"#*s1*#%s#", sXmlElementName.c_str());
				m_poCurrentNode = poParentNode;
				SetError();
				return;
			}
			
			// In all cases (node or item element), push the new element in the channel element list.
			stChannelElementInfo * elementInfo = new stChannelElementInfo(sXmlElementName, sElementName, uiElementType, poParentNode, uiCppType, iStatus, sPreprocessMethodId, sPostprocessMethodId);
			m_oChannelElementList.push_back(elementInfo);

			// Guardar los atributos (items) procesados para el nodo actual.
			AddProcessedAttributes(sXmlElementName, attributes);
		}
	}
	
  //## end ClDMSSAXHandlers::startElement%977216649.body
}

void ClDMSSAXHandlers::warning (const SAXParseException& exception)
{
  //## begin ClDMSSAXHandlers::warning%978973600.body preserve=yes
	if (m_iCurrentRegister >= m_iFirstRegister)
	{
		DumpMessage("Warning", exception);
	}
  //## end ClDMSSAXHandlers::warning%978973600.body
}

void ClDMSSAXHandlers::error (const SAXParseException& exception)
{
  //## begin ClDMSSAXHandlers::error%978973601.body preserve=yes
	if (m_iCurrentRegister >= m_iFirstRegister)
	{
		SetError();
		DumpMessage("Error", exception);
	}
  //## end ClDMSSAXHandlers::error%978973601.body
}

void ClDMSSAXHandlers::fatalError (const SAXParseException& exception)
{
  //## begin ClDMSSAXHandlers::fatalError%978973602.body preserve=yes
	// When we throw our own exceptions, the SAX parser catches it
	// and re-throws it again before dumping a fatal error.
	// The only reason to throw our exceptions is to stop the parsing
	// process (for example, when we only want to parse several 
	// registers, not all the xml file). In this case, we don't want to dump an error message
	// of fatal error.
	if (!m_bInException)
	{
		SetError();
		DumpMessage("Fatal Error", exception);
	}
  //## end ClDMSSAXHandlers::fatalError%978973602.body
}

void ClDMSSAXHandlers::resetErrors ()
{
  //## begin ClDMSSAXHandlers::resetErrors%978973603.body preserve=yes
  //## end ClDMSSAXHandlers::resetErrors%978973603.body
}

void ClDMSSAXHandlers::DumpMessage (m4pcchar_t ai_pcMessage, const SAXParseException& exception)
{
  //## begin ClDMSSAXHandlers::DumpMessage%978973604.body preserve=yes
	
	// Get key base element info.
	string sKeyInfo;
	GetKeyBaseElementInfo(sKeyInfo);

	// Error parsing.
	BL_BEGIN(ERRORLOG, M4_DMS_XML_DUMP_MESSAGE);

	// UNICODE XML
	m4pchar_t	pcSystemId = M4XMLToCpp( (M4XMLCh*)exception.getPublicId() );
	BL_OBJ << ai_pcMessage << pcSystemId << (m4int32_t)exception.getLineNumber() << (m4int32_t)exception.getColumnNumber() << sKeyInfo;
	delete [] pcSystemId ;
	BL_END;
  //## end ClDMSSAXHandlers::DumpMessage%978973604.body
}

void ClDMSSAXHandlers::SetError (m4return_t ai_iError)
{
  //## begin ClDMSSAXHandlers::SetError%977421357.body preserve=yes	
	string		sKeyInfo;

	// Establecer a false la variable de ok.
	m_bIsOk = M4_FALSE;
	m_iError = ai_iError;
		
	// Obtener la información del elemento que se está procesando.
	GetKeyBaseElementInfo(sKeyInfo);

	// Dump message.
	if (sKeyInfo.empty() == M4_FALSE)
	{
		BL_BEGIN(ERRORLOG, M4_DMS_IMPORT_XML_ERROR);
		BL_OBJ << sKeyInfo;
		BL_END;
	}
	else
	{
		BL_BEGIN(ERRORLOG, M4_DMS_IMPORT_XML_PROCESS_ERROR);
		BL_END;
	}
	
  //## end ClDMSSAXHandlers::SetError%977421357.body
}

m4return_t ClDMSSAXHandlers::ProcessNodeElement (const string &ai_sElementName, const string &ai_sXmlElementName, m4int8_t ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId)
{
  //## begin ClDMSSAXHandlers::ProcessNodeElement%1094119909.body preserve=yes
	m4return_t		iRet = M4_SUCCESS;
	m4return_t		iPreprocessMethodRet = M4_XML_EXECUTION_METHOD_OK;
	m4return_t		iPostprocessMethodRet = M4_XML_EXECUTION_METHOD_OK;

	// Get the node.
	ClNode * poNode = &(m_poImportAccess->Node[ai_sElementName.c_str()]);
	if (poNode == NULL)
	{
		SETCODEF(M4_DMS_CREATE_NODE_ERROR, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
		return M4_ERROR;
	}
		
	// Move to that node and to the first register.
	poNode->RecordSet.Current.MoveToEOF();
	
	// Bug 0264560. Call preprocess method item associated to node element.
	iRet = m_poChannelHandler->ExecuteMethod(poNode, ai_sElementName, ai_sPreprocessMethodId, M4_IMPORT_XML_PROCESS, iPreprocessMethodRet);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_PREPROCESS_METHOD_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sPreprocessMethodId.c_str(), ai_sElementName.c_str());
		return M4_ERROR;
	}
	
	// Check returned value.
	// 1: everything is ok.
	// -1: abort export process.
	if (iPreprocessMethodRet == M4_XML_ABORT_PROCESS)
	{
		SETCODEF(M4_DMS_XML_ABORT_PROCESS, ERRORLOG,"#*s1*#");

		// Call postprocess method.
		iRet = m_poChannelHandler->ExecuteMethod(poNode, ai_sElementName, ai_sPostprocessMethodId, M4_IMPORT_XML_PROCESS, iPostprocessMethodRet);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_EXECUTE_POSTPROCESS_METHOD_ERROR, ERRORLOG,"#*s1*#%s#%s#", ai_sPostprocessMethodId.c_str(), ai_sElementName.c_str());
		}

		// Throw exception.
		// When we throw our exception, the SAX parser catches it.
		// As it is not an exception known by the parser, the parser
		// throws the exception again.
		// Save that we are throwing our own exception.
		m_bInException = M4_TRUE;
		ThrowDMSXMLException(StartElement, m_iCurrentRegister);
	}

	// There are special cases for nodes. Before adding a register to the node, we have to 
	// check if there is at least one item of register scope.
	// If all items are node scope, we do not have to add a register to the node.
	m4int32_t iItemsScopeRegister = 0;
	m4int32_t iItemsScopeBlockNode = 0;
	iRet = m_poXmlMap->CheckItemsScope(ai_sElementName, iItemsScopeRegister, iItemsScopeBlockNode);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_CHECK_ITEMS_SCOPE, ERRORLOG,"#*s1*#%s#", ai_sElementName.c_str());
		return M4_ERROR;
	}

	if ((iItemsScopeRegister != 0) || ((iItemsScopeRegister == 0) && (iItemsScopeBlockNode == 0)))
	{
		// Add a new register to that node.
		iRet = poNode->RecordSet.Current.Add();
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_ADD_REGISTER_ERROR, ERRORLOG, "#*s1*#%s#", ai_sElementName.c_str());
			return M4_ERROR;
		}

		if (ai_iStatus == M4_REGISTER_STATUS_UPDATE || ai_iStatus == M4_REGISTER_STATUS_DELETE)
		{
			iRet = poNode->RecordSet.Current.CheckPoint();
			if (iRet != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_CHECK_POINT_REGISTER_ERROR, ERRORLOG, "#*s1*#%s#", ai_sElementName.c_str());
				return M4_ERROR;
			}
		}
	}
				
	// Save current node.
	m_poCurrentNode = poNode;

	return M4_SUCCESS;
  //## end ClDMSSAXHandlers::ProcessNodeElement%1094119909.body
}

m4return_t ClDMSSAXHandlers::ProcessItemElement (const string &ai_sElementName, m4uint8_t &ao_uiCppType)
{
  //## begin ClDMSSAXHandlers::ProcessItemElement%1094119910.body preserve=yes
	// Get cpp item type. The parent node is the last element
	// introduced in the list.
	ao_uiCppType = M4CL_CPP_TYPE_NULL;
	stChannelElementInfo *	infoTemp;
	string					sParentName;

	// Verificar que la lista no está vacía.
	if (m_oChannelElementList.empty() == M4_TRUE)
	{
		return M4_ERROR;
	}

	// Get parent node name from the element list.
	infoTemp = m_oChannelElementList.back();
	sParentName = infoTemp->m_sElementName;
	
	// Obtener cpp type.
	ao_uiCppType = m_poChannelHandler->GetItemCppType(m_poCurrentNode, ai_sElementName, sParentName);
	
	return M4_SUCCESS;
  //## end ClDMSSAXHandlers::ProcessItemElement%1094119910.body
}

m4return_t ClDMSSAXHandlers::ProcessNodeAndItemElement (const string &ai_sElementName, const string &ai_sXmlElementName, ClNode *ai_poParentNode, m4int8_t ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId, m4uint8_t &ao_uiCppType)
{
  //## begin ClDMSSAXHandlers::ProcessNodeAndItemElement%1094119911.body preserve=yes
	// There are some "special" elements that matches a node element and an item within this node.
	// In this cases, we will first process the element as a node elements and then, we will
	// process an item within the node with same name.
	ao_uiCppType = M4CL_CPP_TYPE_NULL;
	m4return_t iErrorCode = M4_SUCCESS;

	iErrorCode = ProcessNodeElement(ai_sElementName, ai_sXmlElementName, ai_iStatus, ai_sPreprocessMethodId, ai_sPostprocessMethodId);
	if (iErrorCode != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Get cpp item type. The parent node is the last element
	// introduced in the list.
	// Get parent node name from the element list.
	string sParentName = ai_sElementName;
	ao_uiCppType = m_poChannelHandler->GetItemCppType(m_poCurrentNode, ai_sElementName, sParentName);

	return M4_SUCCESS;
  //## end ClDMSSAXHandlers::ProcessNodeAndItemElement%1094119911.body
}

m4return_t ClDMSSAXHandlers::ProcessElementAttributes (const string &ai_sXmlElementName, AttributeList& attributes)
{
  //## begin ClDMSSAXHandlers::ProcessElementAttributes%1094119912.body preserve=yes
	// Process all element attributes.
	unsigned int uiLen = attributes.getLength();
	for (unsigned int uiIndex = 0; uiIndex < uiLen; uiIndex++)
	{
		m4return_t		iErrorCode = M4_SUCCESS;
		m4uint8_t		uiCppType = M4CL_CPP_TYPE_NULL;
		m4uint8_t		uiItemM4Type = M4CL_M4_TYPE_NULL;
		string			sXmlItemName, sItemName, sItemValue;
		string			sCurrentNodeName, sFormatDate;
		m4bool_t		bItemTypeFound = M4_FALSE; 
		m4double_t		dDate = 0;
		m4int32_t		iPos1 = 0;
		m4int32_t		iPos2 = 0;
		m4VariantType	vItemValue;
		M4ClTimeStamp	dateConvert;
		
		// UNICODE XML
		M4XMLToSTL( (M4XMLCh*)attributes.getName(uiIndex), sXmlItemName );
		M4XMLToSTL( (M4XMLCh*)attributes.getValue(uiIndex), sItemValue );
		
		// Bug 0264549. Permitir añadir, modificar y borrar registros.
		if ((uiIndex == 0) && (sXmlItemName.compare("___systemrecordstatus___") == 0))
		{
			continue;
		}

		// Save this item only in case that we have a not null value for it.
		if (sItemValue.empty() == M4_FALSE)
		{
			// Get current node id.
			if (m_poCurrentNode != NULL)
			{
				sCurrentNodeName = m_poCurrentNode->GetpNodeDef()->Id();
			}
			else
			{
				SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#m_poCurrentNode#");
				return M4_ERROR;
			}
			
			// If we have not found the attribute mapings, check it is not XML Schema 
			// attributes.
			// Skip schema attributes xmls, xmlns:xsi, xsi:.
			iPos1 = sXmlItemName.find(M4_XML_SCHEMA_XMLNS);
			iPos2 = sXmlItemName.find(M4_XML_SCHEMA_XSI);
			if (iPos1 == 0 || iPos2 == 0)
			{
				// Bug 0283673. No se parseaban los atributos que vienen despues de xmls, xmlns:xsi, xsi:.
				continue;
			}
				
			// Get XML map info.
			iErrorCode = GetXmlAttributeMapInfo(sXmlItemName, ai_sXmlElementName, sCurrentNodeName, sItemName);
			if (iErrorCode != M4_SUCCESS)
			{
				if (iErrorCode == M4_CONSISTENCY_ELEMENT_ERROR)
				{
					continue;
				}
				else
				{
					return M4_ERROR;
				}
			}
			
			// Check if this item is one of the base element keys. 
			// If this is the case, save its value inside base element
			// keys list.
			SetKeyBaseElementValue(sItemName, sItemValue);
			
			// Obtener el tipo meta4 del item.
			if (m_poChannelHandler->GetItemM4Type(m_poCurrentNode, sItemName, sCurrentNodeName, uiItemM4Type) != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_GET_ITEM_M4TYPE_ERROR, ERRORLOG, "#*s1*#%s#%s#", sItemName.c_str(), sCurrentNodeName.c_str());
				return M4_ERROR;
			}
			
			// Miriam || uiItemM4Type == M4CL_M4_TYPE_NUM_VARIANT || uiItemM4Type == M4CL_M4_TYPE_UNI_VARIANT
			uiCppType = m_poChannelHandler->GetItemCppType(m_poCurrentNode, sItemName, sCurrentNodeName);
			if (uiCppType == M4CL_CPP_TYPE_NUMBER)
			{
				if (ValidateNumber(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, sItemValue, vItemValue) != M4_SUCCESS)
				{
					return M4_ERROR;
				}
			}
			else if (uiCppType == M4CL_CPP_TYPE_STRING)
			{
				if (uiItemM4Type == M4CL_M4_TYPE_VARIANT)
				{
					if (ValidateVariant(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, sItemValue, vItemValue) != M4_SUCCESS)
					{
						return M4_ERROR;
					}
				}
				else
				{
					if (ValidateString(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, uiItemM4Type, sItemValue, vItemValue) != M4_SUCCESS)
					{
						return M4_ERROR;
					}
				}
			}
			else if (uiCppType == M4CL_CPP_TYPE_DATE)
			{
				if (ValidateDate(m_poCurrentNode, sItemName, sXmlItemName, sCurrentNodeName, uiItemM4Type, sItemValue, vItemValue) != M4_SUCCESS)
				{
					return M4_ERROR;
				}
			}
			else
			{
				SETCODEF(M4_DMS_ITEM_TYPE_ERROR, ERRORLOG,"#*s1*#%s#", sItemName.c_str());
				return M4_ERROR;
			}
			
			// Save the item value.
			if (m_poCurrentNode->RecordSet.Current.Item[sItemName.c_str()].Value.Set(vItemValue) != M4_SUCCESS)
			{
				SETCODEF(M4_DMS_SET_ITEM_ERROR, ERRORLOG,"#*s1*#%s#", sItemName.c_str());
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
  //## end ClDMSSAXHandlers::ProcessElementAttributes%1094119912.body
}

m4bool_t ClDMSSAXHandlers::CheckParserStatus ()
{
  //## begin ClDMSSAXHandlers::CheckParserStatus%978515139.body preserve=yes

	// Check if an internal error has happened.
	if (IsOk() == M4_FALSE)
	{
		return M4_FALSE;	// Stop parsing.
		
	}
	else
	{
		return M4_TRUE; // Continue parsing.
	}

  //## end ClDMSSAXHandlers::CheckParserStatus%978515139.body
}

void ClDMSSAXHandlers::DeleteBaseElementBlock ()
{
  //## begin ClDMSSAXHandlers::DeleteBaseElementBlock%978515140.body preserve=yes

	// If there has been an error, we have to delete the last
	// register added for the base element.
	ClNode * poNode = &(m_poImportAccess->Node[m_sBaseElementId.c_str()]);
	if (poNode != NULL)
	{
		if (poNode->RecordSet.Current.Index != M4DM_EOF_INDEX)
		{
			// Delete register.
			poNode->RecordSet.Current.Destroy();
		}
	}
  //## end ClDMSSAXHandlers::DeleteBaseElementBlock%978515140.body
}

void ClDMSSAXHandlers::DeleteChannelElementList ()
{
  //## begin ClDMSSAXHandlers::DeleteChannelElementList%980239734.body preserve=yes

	// Delete the channel element list.
	ElementHandlerList::iterator iterador;
	iterador = m_oChannelElementList.begin();

	while (iterador != m_oChannelElementList.end())
	{
		(*iterador)->m_oItemsVector.clear();
		delete (* iterador);
		iterador ++;
	}

	m_oChannelElementList.clear();
  //## end ClDMSSAXHandlers::DeleteChannelElementList%980239734.body
}

m4return_t ClDMSSAXHandlers::InitializeXmlMapM4O (const m4uint32_t &ai_iMapType, const m4bool_t &ai_bDoValidation, const string &ai_sXMLFilePath, const m4bool_t &ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bElementConsistency, m4bool_t ai_bItemValidation)
{
  //## begin ClDMSSAXHandlers::InitializeXmlMapM4O%979558714.body preserve=yes

	// Import channel name.
	string sImportChannelName;
	m4pcchar_t pcNameChannel = m_poImportChannel->GetpChannelDef()->Id();
	if (pcNameChannel == NULL)
	{
		SETCODEF(M4_DMS_GET_NAME_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	sImportChannelName = pcNameChannel;

	m_bElementConsistency = ai_bElementConsistency;
	m_bItemValidation = ai_bItemValidation;

	return m_poXmlMap->InitializeXmlMapM4O(ai_iMapType, ai_bDoValidation, ai_sXMLFilePath, sImportChannelName, ai_bDefaultMappings, ai_iChannelHandle, ai_bElementConsistency);

  //## end ClDMSSAXHandlers::InitializeXmlMapM4O%979558714.body
}

m4return_t ClDMSSAXHandlers::ReleaseXmlMapM4O ()
{
  //## begin ClDMSSAXHandlers::ReleaseXmlMapM4O%979749961.body preserve=yes
	// Release XML - M4O Map channel.
	return m_poXmlMap->ReleaseXmlMapM4O();
  //## end ClDMSSAXHandlers::ReleaseXmlMapM4O%979749961.body
}

m4return_t ClDMSSAXHandlers::GetRootElementInfo ()
{
  //## begin ClDMSSAXHandlers::GetRootElementInfo%980239726.body preserve=yes
	string sXmlRootElementName, sRootNodeId;
	m4return_t iRet;

	iRet = m_poXmlMap->GetRootElementInfo(sXmlRootElementName, sRootNodeId);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (sXmlRootElementName == "")
	{
		// If there is no xml name for the root element, 
		// this is an error.
		// return M4_ERROR;
		// Default mappings: there is no need to have XML root node rigth now.
		// If there is no xml root element, we will set channel name.
		m_sRootNodeXmlName = m_sImportChannelName;
		
		// There is no root node within channel.
		m_sRootNodeId = "";
	}
	else
	{
		m_sRootNodeXmlName = sXmlRootElementName;
		m_sRootNodeId = sRootNodeId;
	}

	return M4_SUCCESS;
  //## end ClDMSSAXHandlers::GetRootElementInfo%980239726.body
}

m4return_t ClDMSSAXHandlers::GetBaseElementInfo ()
{
  //## begin ClDMSSAXHandlers::GetBaseElementInfo%980239727.body preserve=yes
	m4return_t	iRet = M4_SUCCESS;
	string		sElementBaseKeyList = "";
	string		sPreprocessMethodId = "";
	string		sPostprocessMethodId = "";

	// Obtain base element xml name and the list of items to identify
	// that base element (in case of error, for example). This items 
	// are separated by comas.
	iRet = m_poXmlMap->GetBaseElementInfo(m_sXmlBaseElement, sElementBaseKeyList);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// El elemento XML a partir del cuál debemos recuperarnos tras un error.
	// Por defecto, es el elemento base.
	m_sRecoveryElement = m_sXmlBaseElement;

	// Base element and key base element ids must have a value if we have partial load (the
	// user specifies a number of registers to be parsed).
	// If there is no base element, it means default mappings. the user has not specified
	// mappings for these m4object. In this case, we have not got base element => so, we cannot
	// have parcial loads.
	if ((m_sXmlBaseElement.empty() == M4_TRUE) && (m_iFirstRegister != 0 || m_iLastRegister != -1))
	{
		SETCODEF(M4_DMS_PARTIAL_LOAD_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	
	if (m_sXmlBaseElement.empty() == M4_FALSE && sElementBaseKeyList.empty() == M4_FALSE)
	{	
		// Get xml map info for root node.
		m4uint32_t uiElementType;
		iRet = m_poXmlMap->GetXmlElementMapInfo(m_sXmlBaseElement, m_sRootNodeId, uiElementType, m_sBaseElementId, sPreprocessMethodId, sPostprocessMethodId);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_XML_MAP_INFO, ERRORLOG,"#*s1*#%s#", m_sXmlBaseElement.c_str());
			return M4_ERROR;
		}
		
		// Parse key base element ids and insert them in the list.
		m4int32_t iPos = 0;
		string sFirstKeyName;
		
		m4pcchar_t pDest = strchr(sElementBaseKeyList.c_str(), M4_KEY_SEP);
		iPos = pDest - sElementBaseKeyList.c_str();
		
		if (pDest != NULL)
		{
			m4pchar_t pKey = new m4char_t[iPos + 1];
			strncpy(pKey, sElementBaseKeyList.c_str(), iPos);
			pKey[iPos] = '\0';
			sFirstKeyName = pKey;
			delete pKey;
		}
		else // just one key
		{
			sFirstKeyName = sElementBaseKeyList;
		}
		
		// Insert the first key base element. If there is no M4_KEY_SEP,
		// there is only one element.
		stBaseElementKeyInfo * stKeyInfo = new stBaseElementKeyInfo(sFirstKeyName);
		m_oBaseElementKeyList.push_back(stKeyInfo);
		
		if (pDest)
		{
			pDest++;
		}
		
		while (pDest != NULL)
		{
			m4pcchar_t pAux = pDest;
			
			// Look for more key elements.
			pDest = strchr(pAux, M4_KEY_SEP);
			if (pDest != NULL)
			{
				pDest++;
				iPos = pDest - pAux - 1;
				m4pchar_t pKey = new m4char_t[iPos + 1];
				if (pKey != NULL)
				{
					strncpy(pKey, pAux, iPos);
					pKey[iPos] = '\0';
					
					string sKeyName(pKey);
					stBaseElementKeyInfo * stKeyInfo = new stBaseElementKeyInfo(sKeyName);
					m_oBaseElementKeyList.push_back(stKeyInfo);
					delete pKey;
				}
			}
			else
			{
				string sKeyName = pAux;
				stBaseElementKeyInfo * stKeyInfo = new stBaseElementKeyInfo(sKeyName);
				m_oBaseElementKeyList.push_back(stKeyInfo);
			}
		}
	}
	
	return M4_SUCCESS;	
  //## end ClDMSSAXHandlers::GetBaseElementInfo%980239727.body
}

void ClDMSSAXHandlers::SetKeyBaseElementValue (const string &ai_sKeyName, const string &ai_sKeyValue)
{
  //## begin ClDMSSAXHandlers::SetKeyBaseElementValue%980239732.body preserve=yes
	BaseElementKeyList::iterator it;
	m4bool_t bFound = M4_FALSE;

	for (it = m_oBaseElementKeyList.begin(); it != m_oBaseElementKeyList.end(); it++)
	{
		if (!stricmp(((*it)->m_sName).c_str(), ai_sKeyName.c_str()))
		{
			bFound = M4_TRUE;
			break;
		}
	}

	if (bFound == M4_TRUE)
	{
		(*it)->SetValue(ai_sKeyValue);
	}

  //## end ClDMSSAXHandlers::SetKeyBaseElementValue%980239732.body
}

void ClDMSSAXHandlers::GetKeyBaseElementInfo (string &ao_sKeyInfo)
{
  //## begin ClDMSSAXHandlers::GetKeyBaseElementInfo%980239736.body preserve=yes
	// Get key base element info.
	ao_sKeyInfo = "";
	BaseElementKeyList::iterator it;
	it = m_oBaseElementKeyList.begin();
	while (it != m_oBaseElementKeyList.end())
	{
		ao_sKeyInfo = ao_sKeyInfo + (*it)->m_sName + ": " + (*it)->m_sValue + " ";
		it++;
	}

  //## end ClDMSSAXHandlers::GetKeyBaseElementInfo%980239736.body
}

void ClDMSSAXHandlers::DeleteKeyBaseElementList ()
{
  //## begin ClDMSSAXHandlers::DeleteKeyBaseElementList%980239733.body preserve=yes
	BaseElementKeyList::iterator it;

	it = m_oBaseElementKeyList.begin();
	while (it != m_oBaseElementKeyList.end())
	{
		delete (*it);
		it++;
	}

	m_oBaseElementKeyList.clear();

  //## end ClDMSSAXHandlers::DeleteKeyBaseElementList%980239733.body
}

void ClDMSSAXHandlers::ResetKeyBaseElementList ()
{
  //## begin ClDMSSAXHandlers::ResetKeyBaseElementList%980239735.body preserve=yes
	// Only, reset key values, not delete the list.
	BaseElementKeyList::iterator it;

	it = m_oBaseElementKeyList.begin();
	while (it != m_oBaseElementKeyList.end())
	{
		(*it)->SetValue("");
		it++;
	}

  //## end ClDMSSAXHandlers::ResetKeyBaseElementList%980239735.body
}

m4return_t ClDMSSAXHandlers::ConvertStringToDouble (const string &ai_sItemValue, m4double_t & ao_dDoubleValue)
{
  	m4return_t	iRet = M4_SUCCESS;
	m4pchar_t	pStopStr = NULL;
	m4double_t	dItemValue = 0;

	dItemValue = strtod(ai_sItemValue.c_str(), &pStopStr);
	if (*pStopStr != '\0')
	{
		iRet = M4_ERROR;
	}
	else
	{
		ao_dDoubleValue = atof(ai_sItemValue.c_str());
	}

	return iRet;
}

m4return_t ClDMSSAXHandlers::ConvertStringToDate(const string &ai_sDateValue, m4uint8_t ai_uiItemM4Type, m4double_t & ao_dDateValue)
{
	M4ClTimeStamp	oTimeStamp;
	string			sTimeStamp;
	m4return_t		iRet = M4_SUCCESS;

	// Check M4 type and format date to timestamp.
	if (ai_uiItemM4Type == M4CL_M4_TYPE_TIMESTAMP)
	{
		sTimeStamp = ai_sDateValue;
	}
	else if (ai_uiItemM4Type == M4CL_M4_TYPE_DATE)
	{
		sTimeStamp = ai_sDateValue + " 00:00:00";
	}
	else if (ai_uiItemM4Type == M4CL_M4_TYPE_HOUR)
	{
		sTimeStamp = "1970/09/25 " + ai_sDateValue;
	}

	iRet = oTimeStamp.set(sTimeStamp.c_str());
	oTimeStamp.get(ao_dDateValue);

	return iRet;
}

m4return_t ClDMSSAXHandlers::ValidateString(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, m4uint8_t ai_uiM4Type, const string &ai_sValue, m4VariantType &ao_vValue)
{
	m4return_t		iRet = M4_SUCCESS;
	m4uint32_t		uiItemPrec = 0;
	m4uint8_t		uiItemScale = 0;
	m4int_t			iByteLength = 0;
	m4int_t			iCharLength = 0;

	ao_vValue.SetNull();
	
	if (m_bItemValidation == M4_FALSE)
	{
		ao_vValue.SetString((char*)ai_sValue.c_str(), M4_TRUE);
		return M4_SUCCESS;
	}

	iRet = m_poChannelHandler->GetItemPrecScale(ai_poNode, ai_sElementName, ai_sParentNodeName, uiItemPrec, uiItemScale);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_PREC_SCALE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sParentNodeName.c_str());
		return M4_ERROR;
	}
	
	iByteLength = -1;
	iCharLength = M4StrLen(ai_sValue.c_str(), iByteLength);
	if (uiItemPrec >= 0 && iCharLength > uiItemPrec)
	{
		SETCODEF(M4_DMS_BAD_STRING_PRECISION_ERROR, ERRORLOG,"#*s1*#%s#%d#", ai_sElementName.c_str(), uiItemPrec);
		return M4_ERROR;
	}

	ao_vValue.SetString((char*)ai_sValue.c_str(), M4_TRUE);

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::ValidateDate(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, m4uint8_t ai_uiM4Type, const string &ai_sValue, m4VariantType &ao_vValue)
{
	m4return_t		iRet = M4_SUCCESS;
	m4double_t		dValue = 0;
	m4uint32_t		uiItemPrec = 0;
	m4uint8_t		uiItemScale = 0;
	m4double_t		dDecimalPart = 0;
	m4double_t		dIntegerPart = 0;

	ao_vValue.SetNull();

	if (ConvertStringToDate(ai_sValue, ai_uiM4Type, dValue) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_CONVERT_ITEM_VALUE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sXmlElementName.c_str(), ai_sValue.c_str());
		return M4_ERROR;
	}

	if (m_bItemValidation == M4_FALSE)
	{
		ao_vValue.SetDate(dValue);
		return M4_SUCCESS;
	}

	if (dValue < M4CL_MINUS_INFINITE_IN_JULIAN || dValue > M4CL_PLUS_INFINITE_IN_JULIAN)
	{
		SETCODEF(M4_DMS_OUT_OF_RANGE_DATE_ERROR, ERRORLOG, "#*s1*#%s#%s#%s#", ai_sXmlElementName.c_str(), M4CL_MINUS_INFINITE_DATE, M4CL_PLUS_INFINITE_DATE);
		return M4_ERROR;
	}
	
	iRet = m_poChannelHandler->GetItemPrecScale(ai_poNode, ai_sElementName, ai_sParentNodeName, uiItemPrec, uiItemScale);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_PREC_SCALE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sParentNodeName.c_str());
		return M4_ERROR;
	}
	
	if (uiItemPrec == 10)
	{
		dDecimalPart = modf(dValue, &dIntegerPart);
		if (fabs(dDecimalPart) > M4_LN4_MATH_PRECISION) 
		{
			SETCODEF(M4_DMS_BAD_SHORT_DATE_ERROR, ERRORLOG, "#*s1*#%s#", ai_sXmlElementName.c_str());
			return M4_ERROR;
		}
	}
	
	ao_vValue.SetDate(dValue);

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::ValidateNumber(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, const string &ai_sValue, m4VariantType &ao_vValue)
{
	m4return_t	iRet = M4_SUCCESS;
	m4uint32_t	uiItemPrec = 0;
	m4uint8_t	uiItemScale = 0;
	m4double_t	dValue = 0;
	m4double_t  dAux = 0;
	m4pchar_t	pBuffer = NULL;
	
	ao_vValue.SetNull();

	if (ConvertStringToDouble(ai_sValue, dValue) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_CONVERT_ITEM_VALUE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sXmlElementName.c_str(), ai_sValue.c_str());
		return M4_ERROR;
	}
	
	if (m_bItemValidation == M4_FALSE)
	{
		ao_vValue.SetDouble(dValue);
		return M4_SUCCESS;
	}

	iRet = m_poChannelHandler->GetItemPrecScale(ai_poNode, ai_sElementName, ai_sParentNodeName, uiItemPrec, uiItemScale);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_PREC_SCALE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sElementName.c_str(), ai_sParentNodeName.c_str());
		return M4_ERROR;
	}
	
	if (fabs(dValue) > pow(10.0, (m4int_t)(uiItemPrec - uiItemScale)))
	{
		SETCODEF(M4_DMS_BAD_NUMBER_PRECISION_ERROR, ERRORLOG, "#*s1*#%s#%d#", ai_sElementName.c_str(), (uiItemPrec - uiItemScale));
		return M4_ERROR;
	}

	pBuffer = new m4char_t[M4_MAX_BUFFER_SIZE + 1];
	sprintf(pBuffer, "%.*f", uiItemScale, dValue);
	dAux = atof(pBuffer);
	
	delete pBuffer;
	pBuffer = NULL;
	
	if (fabs(dAux - dValue) > M4_LN4_MATH_PRECISION)
	{
		SETCODEF(M4_DMS_BAD_NUMBER_SCALE_ERROR, ERRORLOG, "#*s1*#%s#%d#", ai_sElementName.c_str(), uiItemScale);
		return M4_ERROR;
	}

	ao_vValue.SetDouble(dValue);

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::ValidateVariant(ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sXmlElementName, const string &ai_sParentNodeName, const string &ai_sValue, m4VariantType &ao_vValue)
{
	m4uint32_t		uiAuxItemHandle = 0;
	string			sAuxItemId = "";
	m4VariantType	vAuxItemValue;
	m4double_t		dValue = 0;
	m4uint8_t		uiCppType = 0;

	ao_vValue.SetString((char*)ai_sValue.c_str(), M4_TRUE);

	// El tipo meta4 del item variant se obtiene a partir del valor del item auxiliar asociado.
	if (m_poChannelHandler->GetItemAux(ai_poNode, ai_sElementName, ai_sParentNodeName, uiAuxItemHandle, sAuxItemId) != M4_SUCCESS)
	{
		// Si el item no tiene definido el item auxiliar, simplemente no realizamos la validación.
		return M4_SUCCESS;
	}
	
	// if (ai_poNode->RecordSet.Current.Item[uiAuxItemIndex].Value.Get(vAuxItemValue) != M4_SUCCESS)
	if (ai_poNode->RecordSet.Current.Item[(ClHandle)uiAuxItemHandle].Value.Get(vAuxItemValue) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_ITEM_ERROR, ERRORLOG, "#*s1*#%s#%s#", sAuxItemId.c_str(), ai_sParentNodeName.c_str());
		return M4_ERROR;
	}
	
	if (vAuxItemValue.Type != M4CL_CPP_TYPE_NUMBER)
	{
		// Si el item auxiliar, no tiene valor, no se puede realizar la validación pero no damos error.
		return M4_SUCCESS;
	}

	// Obtener el tipo cpp del item a partir del tipo meta4.
	uiCppType = ConvertM4TypeToCppType(vAuxItemValue.Data.DoubleData);
	if (uiCppType == M4CL_CPP_TYPE_NUMBER)
	{
		// Comprobar que el valor es realmente de tipo número. Las comprobaciones de precisión y escala no se pueden
		// hacer porque el item auxiliar no tiene estos datos definidos.
		if (ConvertStringToDouble(ai_sValue, dValue) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_CONVERT_ITEM_VALUE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sXmlElementName.c_str(), ai_sValue.c_str());
			return M4_ERROR;
		}
		ao_vValue.SetDouble(dValue);
	}
	else if (uiCppType == M4CL_CPP_TYPE_STRING)
	{
		// No se hace ninguna comprobación adicional porque no tenemos precisión en el item auxiliar.
	}
	else if (uiCppType == M4CL_CPP_TYPE_DATE)
	{
		// Comprobar que el valor es realmente de tipo fecha. Las comprobaciones de precisión y escala no se pueden
		// hacer porque el item auxiliar no tiene estos datos definidos.
		if (ConvertStringToDate(ai_sValue, vAuxItemValue.Data.DoubleData, dValue) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_CONVERT_ITEM_VALUE_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sXmlElementName.c_str(), ai_sValue.c_str());
			return M4_ERROR;
		}
		ao_vValue.SetDate(dValue);
	}
	
	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClDMSSAXHandlers%3A3DFC4E01D2.declarations preserve=yes
m4return_t ClDMSSAXHandlers::GetXmlElementMapInfo(const string &ai_sXmlElementKey, const string &ai_sCurrentNode, m4uint32_t &ao_uiElementType, string &ao_sElementId, string & ao_sIdPreprocessMethod, string & ao_sIdPostprocessMethod)
{
	m4return_t iRet = M4_ERROR;

	iRet = m_poXmlMap->GetXmlElementMapInfo(ai_sXmlElementKey, ai_sCurrentNode, ao_uiElementType, ao_sElementId, ao_sIdPreprocessMethod, ao_sIdPostprocessMethod);
	if (iRet != M4_SUCCESS)
	{
		// Verificar si se ha establecido la consistencia de elementos.
		if (m_bElementConsistency == M4_TRUE)
		{
			// Establecer error.
			SetError();
		
			// Establecer el elemento a partir del cuál el proceso de importación se recupera.
			m_sRecoveryElement = m_sXmlBaseElement;

			SETCODEF(M4_DMS_GET_XML_MAP_INFO, ERRORLOG,"#*s1*#%s#", ai_sXmlElementKey.c_str());
		}
		else
		{
			// Establecer la variable de error.
			m_bIsOk = M4_FALSE;

			// Establecer el elemento a partir del cuál el proceso de importación se recupera.
			m_sRecoveryElement = ai_sXmlElementKey;

			SETCODEF(M4_DMS_GET_XML_MAP_INFO, WARNINGLOG,"#*s1*#%s#", ai_sXmlElementKey.c_str());
		}
			
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::GetXmlAttributeMapInfo(const string &ai_sXmlElementKey, const string &ai_sAttributeOf, const string &ai_sCurrentNode, string &ao_sElementId)
{
	m4return_t iRet = M4_ERROR;

	iRet = m_poXmlMap->GetXmlAttributeMapInfo(ai_sXmlElementKey, ai_sAttributeOf, ai_sCurrentNode, ao_sElementId);
	if (iRet != M4_SUCCESS)
	{
		// Verificar si se ha establecido la consistencia de elementos.
		if (m_bElementConsistency == M4_TRUE)
		{
			SETCODEF(M4_DMS_GET_XML_MAP_INFO, ERRORLOG,"#*s1*#%s#", ai_sXmlElementKey.c_str());
			return M4_ERROR;
		}
		else
		{
			SETCODEF(M4_DMS_GET_XML_MAP_INFO, WARNINGLOG,"#*s1*#%s#", ai_sXmlElementKey.c_str());
			return M4_CONSISTENCY_ELEMENT_ERROR;
		}
	}

	return M4_SUCCESS;
}


m4return_t ClDMSSAXHandlers::AddProcessedItem(const string & ai_sXMLElementName)
{
	m4return_t				iRet = M4_SUCCESS;
	m4uint32_t				uiElementType = 0;
	string					sNodeName;
	string					sItemName;
	string					sPreprocessMethodId = "";
	string					sPostprocessMethodId = "";
	stChannelElementInfo *	oNodeInfo = NULL;
	ClXMLStringVector		oItemsVector;
	
	// Verificar que la lista de los elementos que se han ido procesando no está vacía.
	if (m_oChannelElementList.empty() == M4_TRUE)
	{
		return M4_ERROR;
	}
	
	// Obtener el nombre del nodo padre.
	if (m_poCurrentNode != NULL)
	{
		sNodeName = m_poCurrentNode->GetpNodeDef()->Id();
	}
	
	// Obtener el nombrel item equivalente al nombre del elemento XML.
	iRet = GetXmlElementMapInfo(ai_sXMLElementName, sNodeName, uiElementType, sItemName, sPreprocessMethodId, sPostprocessMethodId);
	if (iRet != M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	// Obtener la información del último elemento procesado.
	oNodeInfo = m_oChannelElementList.back();

	// Actualizar la lista de items procesados para el nodo actual.
	oItemsVector = oNodeInfo->m_oItemsVector;
	oItemsVector.push_back(sItemName);
	oNodeInfo->m_oItemsVector = oItemsVector;

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::AddProcessedAttributes(const string & ai_sXmlElementName, AttributeList& attributes)
{
	// Process all element attributes.
	unsigned int			uiLen = attributes.getLength();
	string					sAttributeName;
	string					sNodeName;
	string					sItemName;
	m4int32_t				iPos1 = 0;
	m4int32_t				iPos2 = 0;
	stChannelElementInfo *	oNodeInfo = NULL;
	ClXMLStringVector		oItemsVector;
	m4return_t				iErrorCode = M4_SUCCESS;

	// Verificar que la lista de los elementos que se han procesado no está vacía.
	if (m_oChannelElementList.empty() == M4_TRUE)
	{
		return M4_ERROR;
	}

	for (unsigned int uiIndex = 0; uiIndex < uiLen; uiIndex++)
	{
		// UNICODE XML
		M4XMLToSTL((M4XMLCh*)attributes.getName(uiIndex), sAttributeName);

		// Bug 0264549. Permitir añadir, modificar y borrar registros.
		if ((uiIndex == 0) && (sAttributeName.compare("___systemrecordstatus___") == 0))
		{
			continue;
		}

		// No se tratan los atributos proios de los schemas xml: xmls, xmlns:xsi, xsi:.
		iPos1 = sAttributeName.find(M4_XML_SCHEMA_XMLNS);
		iPos2 = sAttributeName.find(M4_XML_SCHEMA_XSI);
		if (iPos1 == 0 || iPos2 == 0)
		{
			continue;
		}

		// AddProcessedItem(sAttributeName);
		// Obtener el nombre del nodo padre.
		if (m_poCurrentNode != NULL)
		{
			sNodeName = m_poCurrentNode->GetpNodeDef()->Id();
		}
		
		// Obtener la información de mapeo del atributo.
		iErrorCode = GetXmlAttributeMapInfo(sAttributeName, ai_sXmlElementName, sNodeName, sItemName);
		if (iErrorCode != M4_SUCCESS)
		{
			return M4_ERROR;
		}

		// Obtener la información del último elemento procesado.
		oNodeInfo = m_oChannelElementList.back();

		// Actualizar la lista de items procesados para el nodo actual.
		oItemsVector = oNodeInfo->m_oItemsVector;
		oItemsVector.push_back(sItemName);
		oNodeInfo->m_oItemsVector = oItemsVector;
	}

	return M4_SUCCESS;
}

m4return_t ClDMSSAXHandlers::CheckDeleteRegister(stChannelElementInfo * ai_poNodeInfo, m4bool_t & ao_bDeleteRegister)
{
	m4int32_t			iItemNum = 0;
	m4int32_t			i = 0;
	string				sItemName;
	string				sNodeName;
	m4uint8_t			uiItemScope = 0;
	ClXMLStringVector	oItemsVector;
	m4bool_t			bFound = M4_FALSE;

	// Inicialización.
	ao_bDeleteRegister = M4_FALSE;

	// Verificar que estamos procesando un elemento de tipo nodo.
	if (ai_poNodeInfo->m_uiElementType == M4_ITEM_TYPE)
	{
		return M4_SUCCESS;
	}

	// Obtener el nombre del nodo que se está procesando.
	sNodeName = ai_poNodeInfo->m_sElementName;

	// Obtener la lista de items procesado para este nodo.
	oItemsVector = ai_poNodeInfo->m_oItemsVector;

	// Si no se han procesado items para el nodo, no se debe borrar el registro.
	iItemNum = oItemsVector.size();
	if (iItemNum == 0)
	{
		ao_bDeleteRegister = M4_FALSE;
		return M4_SUCCESS;
	}

	// Recorrer la lista de items procesados del nodo y verificar su ámbito.
	for (i = 0; i < iItemNum; i++) 
	{
		sItemName = oItemsVector[i];

		// Obtener scope del item.
		if (m_poChannelHandler->GetItemScope(m_poCurrentNode, sItemName, sNodeName, uiItemScope) != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_GET_ITEM_SCOPE_ERROR, ERRORLOG,"#*s1*#%s#%s#", sItemName.c_str(), sNodeName.c_str());
			return M4_ERROR;
		}

		// Establecer si se han procesado items de ámbito registro o bloque para el nodo actual.
		if (uiItemScope == M4CL_ITEM_SCOPE_REGISTER)
		{
			bFound = M4_TRUE;
			break;
		}	
	}

	// Si hay al menos un item procesado de ámbito registro o bloque, el registro del nodo
	// no debe borrarse. Si todos los items procesados del nodo son de ámbito nodo, 
	// el registro del nodo se debe borrar.
	if (bFound == M4_TRUE)
	{
		ao_bDeleteRegister = M4_FALSE;
	}
	else
	{
		ao_bDeleteRegister = M4_TRUE;
	}

	return M4_SUCCESS;
}

m4bool_t ClDMSSAXHandlers::CheckCharacters(m4char_t * ai_pcText)
{
	m4bool_t	bValidCharacters = M4_FALSE;

	if (ai_pcText == NULL)
	{
		return M4_FALSE;
	}

	while (*ai_pcText != '\0')
	{
		if (isspace(*ai_pcText) == 0)
		{
			bValidCharacters = M4_TRUE;
			break;
		} 
		ai_pcText++;
	}
	
	return bValidCharacters;
}

  //## end ClDMSSAXHandlers%3A3DFC4E01D2.declarations

// Class stChannelElementInfo 







stChannelElementInfo::stChannelElementInfo (const string &ai_sXmlElementName, const string &ai_sElementName, const m4uint32_t &ai_ElementType, ClNode *  ai_poParent, const m4uint8_t &ai_uiCppType, const m4int8_t &ai_iStatus, const string &ai_sPreprocessMethodId, const string &ai_sPostprocessMethodId)
  //## begin stChannelElementInfo::stChannelElementInfo%977216650.hasinit preserve=no
      : m_sXmlElementName(ai_sXmlElementName), m_uiElementType(ai_ElementType), m_poParent(ai_poParent), m_uiCppType(ai_uiCppType), m_sElementName(ai_sElementName), m_sElementValue(""), m_iStatus(ai_iStatus), m_sPreprocessMethodId(ai_sPreprocessMethodId), m_sPostprocessMethodId(ai_sPostprocessMethodId)
  //## end stChannelElementInfo::stChannelElementInfo%977216650.hasinit
  //## begin stChannelElementInfo::stChannelElementInfo%977216650.initialization preserve=yes
  //## end stChannelElementInfo::stChannelElementInfo%977216650.initialization
{
  //## begin stChannelElementInfo::stChannelElementInfo%977216650.body preserve=yes
	m_oItemsVector.reserve(16);
  //## end stChannelElementInfo::stChannelElementInfo%977216650.body
}


// Additional Declarations
  //## begin stChannelElementInfo%3A3E49F501EB.declarations preserve=yes
  //## end stChannelElementInfo%3A3E49F501EB.declarations

// Class stBaseElementKeyInfo 



stBaseElementKeyInfo::stBaseElementKeyInfo (const string &ai_sName)
  //## begin stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.hasinit preserve=no
      : m_sName(ai_sName), m_sValue("")
  //## end stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.hasinit
  //## begin stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.initialization preserve=yes
  //## end stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.initialization
{
  //## begin stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.body preserve=yes
  //## end stBaseElementKeyInfo::stBaseElementKeyInfo%980239728.body
}


// Additional Declarations
  //## begin stBaseElementKeyInfo%3A6DAE830164.declarations preserve=yes
  //## end stBaseElementKeyInfo%3A6DAE830164.declarations

//## begin module%3A3E57A701A6.epilog preserve=yes
//## end module%3A3E57A701A6.epilog
