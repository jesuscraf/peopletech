//## begin module%3A8A70060262.cm preserve=no
//## end module%3A8A70060262.cm

//## begin module%3A8A70060262.cp preserve=no
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
//## end module%3A8A70060262.cp

//## Module: m4dmschannelhandler%3A8A70060262; Package body
//## Subsystem: M4DMSXml::src%3A3E5D2D0159
//## Source file: C:\m4server\m4dmsxml\src\m4dmschannelhandler.cpp

//## begin module%3A8A70060262.additionalIncludes preserve=no
//## end module%3A8A70060262.additionalIncludes

//## begin module%3A8A70060262.includes preserve=yes
// dm
#include <dm.hpp>
// Errors
#include <m4dmsxmlerr.hpp>
//## end module%3A8A70060262.includes

// node_x
#include <node_x.hpp>
// access
#include <access.hpp>
// channel
#include <channel.hpp>
// m4dmschannelhandler
#include <m4dmschannelhandler.hpp>
#include "m4dmsxmldefines.hpp"

struct stChannelElementInfo;

//## begin module%3A8A70060262.declarations preserve=no
//## end module%3A8A70060262.declarations

//## begin module%3A8A70060262.additionalDeclarations preserve=yes
//## end module%3A8A70060262.additionalDeclarations


// Class ClDMSChannelHandler 





ClDMSChannelHandler::ClDMSChannelHandler (ClChannelManager *ai_poChannelManager)
  //## begin ClDMSChannelHandler::ClDMSChannelHandler%982251389.hasinit preserve=no
  //## end ClDMSChannelHandler::ClDMSChannelHandler%982251389.hasinit
  //## begin ClDMSChannelHandler::ClDMSChannelHandler%982251389.initialization preserve=yes
  //## end ClDMSChannelHandler::ClDMSChannelHandler%982251389.initialization
{
  //## begin ClDMSChannelHandler::ClDMSChannelHandler%982251389.body preserve=yes
	m_poChannelManager = ai_poChannelManager;
  //## end ClDMSChannelHandler::ClDMSChannelHandler%982251389.body
}


ClDMSChannelHandler::~ClDMSChannelHandler ()
{
  //## begin ClDMSChannelHandler::~ClDMSChannelHandler%982251398.body preserve=yes
  //## end ClDMSChannelHandler::~ClDMSChannelHandler%982251398.body
}



//## Other Operations (implementation)
m4return_t ClDMSChannelHandler::LoadChannel (const string &ai_sNameChannel, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess)
{
  //## begin ClDMSChannelHandler::LoadChannel%982251390.body preserve=yes
	m4return_t iRet;

	m_poChannelManager->CreateChannel(ao_poChannel);
	if (ao_poChannel == NULL)
	{
		SETCODEF(M4_DMS_CREATE_CHANNEL_ERROR, ERRORLOG, "#*s1*#%s#", ai_sNameChannel.c_str());
		ao_poChannel = NULL;
		return M4_ERROR;
	}

	// ao_poChannel->Role_ID.Set(M4CL_SEC_SUPER_USER);

	iRet = ao_poChannel->BuildFromId(ai_sNameChannel.c_str());
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_BUILD_CHANNEL_ERROR, ERRORLOG, "#*s1*#%s#", ai_sNameChannel.c_str());
		ao_poChannel->Destroy();
		ao_poChannel = NULL;
		return M4_ERROR;
	}

	ao_poAccess = ao_poChannel->CreateAccess();
	if (ao_poAccess == NULL)
	{
		SETCODEF(M4_DMS_CREATE_ACCESS_ERROR, ERRORLOG, "#*s1*#%s", ai_sNameChannel.c_str());
		ao_poChannel->Destroy();
		ao_poChannel = NULL;
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::LoadChannel%982251390.body
}

m4return_t ClDMSChannelHandler::UnLoadChannel (ClAccess *&ao_poAccess, ClChannel *&ao_poChannel)
{
  //## begin ClDMSChannelHandler::UnLoadChannel%982251391.body preserve=yes
	m4return_t	iRet;
	m4bool_t	bError = M4_FALSE;
	
	string sChannelName;
	iRet = GetChannelName(ao_poChannel, sChannelName);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_CHANNEL_NAME_ERROR, ERRORLOG, "#*s1*#");
	}
	
	// Destroy access to channel.
	if (ao_poAccess)
	{
		iRet = ao_poAccess->Destroy();
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_DESTROY_ACCESS_ERROR, ERRORLOG, "#*s1*#%s#", sChannelName.c_str());
			bError = M4_TRUE;
		}
		ao_poAccess = NULL;
	}

	if (ao_poChannel)
	{
		// Destroy channel.
		iRet = ao_poChannel->Destroy();
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_DMS_DESTROY_CHANNEL_ERROR, ERRORLOG, "#*s1*#%s#", sChannelName.c_str());
			bError = M4_TRUE;
		}	
		ao_poChannel = NULL;
	}

	if (bError == M4_TRUE)
		return M4_ERROR;	
	
	return M4_SUCCESS;
  //## end ClDMSChannelHandler::UnLoadChannel%982251391.body
}

m4return_t ClDMSChannelHandler::GetBuiltChannel (const string &ai_sChannelName, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess)
{
  //## begin ClDMSChannelHandler::GetBuiltChannel%984388359.body preserve=yes
	
	// Run through the list of built channels to find the given one.
	ClChannelDirectory *	pChanDir = NULL;
	ClChannel *				pChanAux = NULL;
	m4pcchar_t				pcAuxChanId = NULL;
	m4uint32_t				iNumChan, i;

	pChanDir = &(m_poChannelManager->Channel);
	iNumChan = pChanDir->Count();

	ao_poChannel = NULL;
	for (i = 0; (i < iNumChan) && (!ao_poChannel); i++)
	{
		pChanAux = (*pChanDir)[i];

		if (pChanAux->IsBuilt())
		{
			pcAuxChanId = pChanAux->GetpChannelDef()->Id();
		}
		else
		{
			pcAuxChanId = NULL;
		}

		if ((pcAuxChanId) && (stricmp (pcAuxChanId, ai_sChannelName.c_str()) == 0))
		{
			// Given channel found.
			ao_poChannel = pChanAux;
		}
	}

	if (!ao_poChannel)
	{
		// There is not the given channel built.
		return M4_ERROR;
	}

	ao_poAccess = ao_poChannel->CreateAccess();
	if (ao_poAccess == NULL)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetBuiltChannel%984388359.body
}

m4return_t ClDMSChannelHandler::GetNodeHandle (ClNode *ai_poNode, const string &ai_sNodeName, m4uint32_t &ao_uiNodeHandle)
{
	m4uint32_t uiNodeHandle = M4CL_M4_TYPE_NULL;
	
	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	ao_uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sNodeName.c_str());
	if (ao_uiNodeHandle == M4CL_M4_TYPE_NULL)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClDMSChannelHandler::GetItemHandle (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiItemHandle)
{
	m4uint32_t uiNodeHandle = M4CL_M4_TYPE_NULL;
	m4uint32_t uiItemHandle = M4CL_M4_TYPE_NULL;

	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentNodeName.c_str());
	if (uiNodeHandle == M4CL_M4_TYPE_NULL)
	{
		return M4_ERROR;
	}
				
	// Get item handle.
	ao_uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sElementName.c_str());
	if (ao_uiItemHandle == M4CL_M4_TYPE_NULL)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClDMSChannelHandler::GetItemPrecScale (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiItemPrec, m4uint8_t &ao_uiItemScale)
{
  //## begin ClDMSChannelHandler::GetItemPrecScale%990436214.body preserve=yes
	// Get parent node name from the element list.
	m4uint32_t uiNodeHandle, uiItemHandle;
	
	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentNodeName.c_str());
	if (uiNodeHandle != 0)
	{
		uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sElementName.c_str());
		if (uiItemHandle != 0)
		{	
			// Find out item precision.
			ao_uiItemPrec = poAccess->GetpCMCR()->GetItemPrecision(uiItemHandle);

			// Find out item scale.
			ao_uiItemScale = poAccess->GetpCMCR()->GetItemScale(uiItemHandle);
		}
		else
			return M4_ERROR;
	}
	else
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetItemPrecScale%990436214.body
}

m4uint8_t ClDMSChannelHandler::GetItemCppType (ClNode *ai_poNode, const string &ai_sItemName, const string &ai_sParentName)
{
  //## begin ClDMSChannelHandler::GetItemCppType%990436215.body preserve=yes
	m4uint32_t uiNodeHandle, uiItemHandle;
	m4uint8_t uiCppItemType = M4CL_CPP_TYPE_NULL;
	
	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentName.c_str());
	if (uiNodeHandle != 0)
	{
		uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sItemName.c_str());
		if (uiItemHandle != 0)
		{	
			// Find out cpp item type.
			uiCppItemType = poAccess->GetpCMCR()->GetItemCppType(uiItemHandle);
		}
	}

	return uiCppItemType;
  //## end ClDMSChannelHandler::GetItemCppType%990436215.body
}

m4return_t ClDMSChannelHandler::GetItemM4Type (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint8_t &ao_uiItemM4Type)
{
  //## begin ClDMSChannelHandler::GetItemM4Type%990436216.body preserve=yes
	m4uint32_t uiNodeHandle = 0;
	m4uint32_t uiItemHandle = M4CL_M4_TYPE_NULL;

	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentNodeName.c_str());
	if (uiNodeHandle == 0)
	{
		return M4_ERROR;
	}
				
	// Get item handle.
	uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sElementName.c_str());
	if (uiItemHandle == 0)
	{
		return M4_ERROR;
	}
	
	// Get item scale.
	ao_uiItemM4Type = poAccess->GetpCMCR()->GetItemM4Type(uiItemHandle);

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetItemM4Type%990436216.body
}

m4return_t ClDMSChannelHandler::GetItemAux (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint32_t &ao_uiAuxItemHandle, string& ao_sAuxItemId)
{
 	m4uint32_t uiNodeHandle = 0;
	m4uint32_t uiItemHandle = M4CL_M4_TYPE_NULL;
	m4pcchar_t	pccId = NULL;

	ao_uiAuxItemHandle = 0;
	ao_sAuxItemId = "";

	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentNodeName.c_str());
	if (uiNodeHandle == 0)
	{
		return M4_ERROR;
	}
				
	// Get item handle.
	uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sElementName.c_str());
	if (uiItemHandle == 0)
	{
		return M4_ERROR;
	}
	
	// Get aux item.
	ao_uiAuxItemHandle = poAccess->GetpCMCR()->GetItemAuxItem(uiItemHandle);
	if (ao_uiAuxItemHandle == 0)
	{
		return M4_ERROR;
	}

	ao_sAuxItemId = poAccess->GetpCMCR()->GetItemId(ao_uiAuxItemHandle);
	
	return M4_SUCCESS;
}


m4return_t ClDMSChannelHandler::GetChannelByHandle (m4int32_t ai_iChannelHandle, ClChannel * &ao_poChannel, ClAccess * &ao_poAccess)
{
  //## begin ClDMSChannelHandler::GetChannelByHandle%1116582877.body preserve=yes
	ao_poChannel = m_poChannelManager->GetChannelByHandle(ai_iChannelHandle);
	if (ao_poChannel == NULL)
	{
		SETCODEF(M4_DMS_GET_CHANNEL_BY_HANDLE_ERROR, ERRORLOG, "#*s1*#%d#", ai_iChannelHandle);
		ao_poChannel = NULL;
		return M4_ERROR;
	}

	// Get channel name.
	string sChannelName;
	m4return_t iRet = GetChannelName(ao_poChannel, sChannelName);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_GET_CHANNEL_NAME_ERROR, ERRORLOG, "#*s1*#");
	}
	
	ao_poAccess = ao_poChannel->CreateAccess();
	if (ao_poAccess == NULL)
	{
		SETCODEF(M4_DMS_CREATE_ACCESS_ERROR, ERRORLOG, "#*s1*#%s", sChannelName.c_str());
		ao_poChannel->Destroy();
		ao_poChannel = NULL;
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetChannelByHandle%1116582877.body
}

m4return_t ClDMSChannelHandler::GetChannelName (ClChannel *  ai_poChannel, string &aio_sChannelName)
{
  //## begin ClDMSChannelHandler::GetChannelName%1116582878.body preserve=yes
	m4pcchar_t pcNameChannel;
	if (ai_poChannel != NULL)
	{
		pcNameChannel = ai_poChannel->GetpChannelDef()->Id();
	}
	else
	{
		pcNameChannel = NULL;
		return M4_ERROR;
	}

	aio_sChannelName = pcNameChannel;
	
	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetChannelName%1116582878.body
}

m4return_t ClDMSChannelHandler::GetItemScope (ClNode *ai_poNode, const string &ai_sElementName, const string &ai_sParentNodeName, m4uint8_t &ao_uiItemScope)
{
  //## begin ClDMSChannelHandler::GetItemScope%1123661606.body preserve=yes
	m4uint8_t uiItemM4Type = 0;
	m4uint32_t uiNodeHandle, uiItemHandle;

	// Get access channel from node.
	ClAccess * poAccess = ai_poNode->GetpAccess();

	// Get node handle.
	uiNodeHandle = poAccess->GetpCMCR()->GetChannelNodeHandleById(ai_sParentNodeName.c_str());
	if (uiNodeHandle == 0)
	{
		return M4_ERROR;
	}
				
	// Get item handle.
	uiItemHandle = poAccess->GetpCMCR()->GetNodeItemHandleById(uiNodeHandle, ai_sElementName.c_str());
	if (uiItemHandle == 0)
	{
		return M4_ERROR;
	}
	
	// Get item scale.
	ao_uiItemScope = poAccess->GetpCMCR()->GetItemScope(uiItemHandle);

	return M4_SUCCESS;
  //## end ClDMSChannelHandler::GetItemScope%1123661606.body
}

m4return_t ClDMSChannelHandler::ExecuteMethod(ClNode * ai_poNode, const string & ai_sNodeName, const string & ai_sMethodName, m4int32_t ai_ArgValue, m4return_t & ao_iResult)
{
	ClNode *		poNode = NULL;
	m4VariantType	vArg;
	m4VariantType	vRes;
	ClItem	*		poItem = NULL;
	m4uint16_t		iNumArgs = 0;

	// Inicializaciones
	ao_iResult = M4_XML_EXECUTION_METHOD_OK;

	if (ai_sMethodName.empty() == M4_TRUE)
	{
		return M4_SUCCESS;
	}

	if (ai_poNode == NULL)
	{
		return M4_ERROR;
	}

	m4pcchar_t pcNameChannel = ai_poNode->GetpAccess()->GetpChannel()->GetpChannelDef()->Id();
	if (pcNameChannel == NULL)
	{
		SETCODEF(M4_DMS_GET_NAME_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	
	poItem = &(ai_poNode->RecordSet.Current.Item[ai_sMethodName.c_str()]);
	if (poItem->GetIndex() == M4DM_EOF_INDEX)
	{
		SETCODEF(M4_CH_DM_ITEM_NOT_FOUND, ERRORLOG,"#*s1*#%s#%s#%s#%s#", ai_sMethodName.c_str(), ai_sNodeName.c_str(), pcNameChannel, "ExecuteMethod");
		return M4_ERROR;
	}
	
	// Verificar que el método tiene un solo argumento.
	iNumArgs = poItem->ItemDef.NumberOfArguments();
	if (iNumArgs != 1)
	{
		SETCODEF(M4_CH_DM_WRONG_ITEM_DEFINITION, ERRORLOG,"#*s1*#%s#%s#%s#%d#%d#", ai_sMethodName.c_str(), ai_sNodeName.c_str(), pcNameChannel, 1, iNumArgs);
		return M4_ERROR;
	}

	vArg.Type = M4CL_CPP_TYPE_NUMBER;
	vArg.Data.DoubleData = ai_ArgValue;
	if (poItem->StackPush(vArg) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sMethodName.c_str(), pcNameChannel);
		return M4_ERROR;
	}

	if (poItem->Call() != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_EXECUTE_METHOD_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sMethodName.c_str(), pcNameChannel);
		return M4_ERROR;
	}
	
	if (poItem->StackPop(vRes) != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_POP_STACK_ERROR, ERRORLOG, "#*s1*#%s#%s#", ai_sMethodName.c_str(), pcNameChannel);
		return M4_ERROR;
	}
	
	ao_iResult = vRes.Data.DoubleData;

	return M4_SUCCESS;
}
// Additional Declarations
  //## begin ClDMSChannelHandler%3A8A690E007C.declarations preserve=yes
  //## end ClDMSChannelHandler%3A8A690E007C.declarations

//## begin module%3A8A70060262.epilog preserve=yes
//## end module%3A8A70060262.epilog
