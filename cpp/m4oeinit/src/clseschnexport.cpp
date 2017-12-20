//==============================================================================
//
// (c) Copyright 2002 Meta4 Spain S.A
// All rights reserved.
//
// Module:           
// File:            clseschnexport.cpp
// Project:         Mind3x
// Author:         
// Date:            
// Language:         C++
//
// Definition:
//   This module defines the code for import/export the session channel
//	 from/to a ClSessionContext object
//
//==============================================================================


#include "clseschnexport.hpp"
#include "m4trace.hpp"
#include "chan_knl.hpp"
#include "nod_data.hpp"
#include "itemsimp.hpp"
#include "access.hpp"
#include "cllgadap.hpp"


m4return_t ClSesChnExport::Export(const ClChannel* ai_poSesChn, ClSessionContext &ao_sesCtx)
{
	// Check Session Channel.
	if (ai_poSesChn == NULL)
	{
		m4Trace(cout << "SessionChannel == NULL" << endl);
		return M4_ERROR;
	}

    // Get the channel data.
    ClChannel_Data *poChannel_Data = ai_poSesChn->GetpChannel_Data();

	// Get node.
	int i=0;
	ClNodeDirectory_Data *poNode = &(poChannel_Data->Node);
	ClNode_Data *poNodeData = poNode->Get("ROOT_SESSION");
	if (NULL == poNodeData)
	{
		m4Trace(cout << "Error getting ROOT_SESSION node..." << endl);
		return M4_ERROR;
	}

	// items.
	const char *pcId = NULL;
	const char *pcValue = NULL;
	char bufferTmp[1024];
	
	ClNoCallableItem item(M4CL_ITEM_SCOPE_NODE);
	item.Attach(poNodeData);
    item.Begin();


    while (item.GetIndex() != M4DM_EOF_INDEX) 
	{
		if ((item.ItemDef.Scope() == M4CL_ITEM_SCOPE_NODE) 
			&& (item.ItemDef.Type() != M4CL_ITEM_TYPE_METHOD))
		{
			// Gets item ID.
			pcId = item.ItemDef.Id();
			if (NULL != pcId) 
			{ 
				// Gets item value.
				m4VariantType oValue;
				item.Value.Get(oValue);
				pcValue = (m4pchar_t)oValue;
				switch (item.ItemDef.CppType()) 
				{
					case M4CL_CPP_TYPE_NUMBER:
						sprintf (bufferTmp, "%f", (m4double_t)oValue);
						pcValue = bufferTmp;
						break;
					case M4CL_CPP_TYPE_STRING:
						pcValue = (m4pchar_t)oValue;
						break;
					case M4CL_CPP_TYPE_DATE:
						ClDateToString((m4double_t)oValue, bufferTmp, M4_SECOND_END + 1);
						pcValue = bufferTmp;
						break;
					default:
						pcValue = "Unsupported type";
						break;
				}
			}

			if ((NULL != pcId) && (NULL != pcValue))
			{
				m4Trace (cout << "   <" <<  pcId << "> : <" << pcValue << ">" << endl);
				ao_sesCtx.SetAtribute(pcId, pcValue);
			}
		}

        item.Next ();
    }

	return M4_SUCCESS;
}


m4return_t ClSesChnExport::Import(ClChannel* aio_poSesChn, ClSessionContext &ai_sesCtx)
{
	// saves ID_APP_ROLE and ID_ORGANIZATION
	m4pcchar_t pcID_APP_ROLE = NULL;
	m4pcchar_t pcID_ORGANIZATION = NULL;

	m4return_t retCode = M4_ERROR;


	// Check Session Channel.
	if (aio_poSesChn == NULL)
	{
		m4Trace(cout << "Import SessionChannel: SessionChannel == NULL" << endl);
		return M4_ERROR;
	}

	ClAccess *poAccess = aio_poSesChn->CreateAccess();
	if (NULL == poAccess)	//Creamos un acceso por cada canal
	{
		m4Trace(cout << "Import SessionChannel: Error creating Access." << endl);
		return M4_ERROR;
	}

	ClNode *poNode=&(poAccess->Node["ROOT_SESSION"]);
	if (NULL == poNode )
	{
		m4Trace(cout << "Import SessionChannel: Error creating Node." << endl);
		return M4_ERROR;
	}

	// foreach element in context, set its item.
	m4Trace(cout << "Setting current SessionChannel from a ClSessionContext object." << endl);
	m4pcchar_t pcKey = NULL;
	m4pcchar_t pcValue = NULL;
	
	SessionIterator it = ai_sesCtx.GetIterator();
	while ((pcKey = it.GetKey()) != NULL)
	{
		// Checks if value is != NULL
		pcValue = it.GetValue();
		if (NULL == pcValue) continue;

		// Check for special items.
		if (!strcmp(pcKey, "ID_APP_ROLE"))
		{
			m4Trace(cout << "Setting " << pcKey << " : " << pcValue << endl);

			m4VariantType oArgs[2];
			oArgs[0].Type = M4CL_CPP_TYPE_STRING_VAR;
			oArgs[0].Data.PointerChar = (char*)pcValue;

			oArgs[1].Type = M4CL_CPP_TYPE_NUMBER;
			oArgs[1].Data.DoubleData = 0;

			m4pcchar_t pcM4Object = "ROOT_SESSION";
			m4pcchar_t pcM4Method = "_CHECK_APP_ROLE";

			ClAccess *poAccess = poNode->GetpAccess();

			retCode = M4ExecuteItem(NULL, NULL, pcM4Object, pcM4Method, oArgs, 2, NULL, 0, NULL, &poAccess);
			if(retCode != M4_SUCCESS)
			{
				m4Trace(cout << "   Error executing method " << pcM4Object << "::" << pcM4Method << ". RetCode = " << retCode << endl);

				// get default ID_APP_ROLE.
				pcValue = (m4pcchar_t)poNode->RecordSet.Current.Item[pcKey].Value.Get();
				m4Trace(cout << "   New Role is = " << pcValue << endl);
			}

			// saves ID_APP_ROLE for future references.
			pcID_APP_ROLE = pcValue;
		}
		else if (!strcmp(pcKey, "ID_APP_USER"))
		{
			m4Trace(cout << "Skipping set \"" << pcKey << "\"." << endl);
		}
		else if (!strcmp(pcKey, "ID_RSM"))
		{
			m4Trace(cout << "Skipping set \"" << pcKey << "\"." << endl);
		}
		else if (!strcmp(pcKey, "ID_ORGANIZATION"))
		{
			// saves ID_ORGANIZATION for future references.
			m4Trace(cout << "Saving \"" << pcKey << "\"." << endl);
			pcID_ORGANIZATION = pcValue;
		}
		else
		{
			// any other item.
			m4Trace(cout << "   " << pcKey << " : " << pcValue << endl);
			retCode = SetItem(poNode, pcKey, pcValue);
			if (retCode != M4_SUCCESS)
			{
				m4Trace(cout << "   Error setting \"" << pcKey << "\" to \"" << pcValue << "\"." << endl);
			}
		}
				
		it.Next();
	}

	// set ID_ORGANIZATION.
	if ((NULL!=pcID_ORGANIZATION) && (NULL!=pcID_APP_ROLE))
	{

		m4Trace(cout << "Setting " << "ID_ORGANIZATION" << " : " << pcID_ORGANIZATION << endl);

		m4VariantType inputArgs[4];
		m4VariantType outputArgs[1];

		inputArgs[0].Type = M4CL_CPP_TYPE_STRING_VAR;
		inputArgs[0].Data.PointerChar = (char*)pcID_APP_ROLE;

		inputArgs[1].Type = M4CL_CPP_TYPE_STRING_VAR;
		inputArgs[1].Data.PointerChar = (char*)pcID_ORGANIZATION;

		inputArgs[2].Type = M4CL_CPP_TYPE_NUMBER;
		inputArgs[2].Data.DoubleData = 2;

		inputArgs[3].Type = M4CL_CPP_TYPE_NULL;
		inputArgs[3].Data.DoubleData = 0;

		m4pcchar_t pcM4Object = "ROOT_SESSION";
		m4pcchar_t pcM4Method = "CHECK_ORG_ROLE";

		ClAccess *poAccess = poNode->GetpAccess();

		retCode = M4ExecuteItem(NULL, NULL, pcM4Object, pcM4Method, inputArgs, 4, outputArgs, 1, NULL, &poAccess);
		if(retCode != M4_SUCCESS)
		{
			m4Trace(cout << "   Error executing method " << pcM4Object << "::" << pcM4Method << ". RetCode = " << retCode << endl);
		}
		else
		{
			retCode = SetItem(poNode, "ID_ORGANIZATION", pcID_ORGANIZATION);
			if (retCode != M4_SUCCESS)
			{
				m4Trace(cout << "   Error setting \"" << "ID_ORGANIZATION" << "\" to \"" << pcID_ORGANIZATION << "\"." << endl);
			}
		}
	}
	
	// destroy access.
	retCode = poAccess->Destroy();
	if (M4_SUCCESS != retCode)
	{
		m4Trace(cout << "Import SessionChannel: Error Destroying access." << endl);
	}

	return retCode;
}


m4return_t ClSesChnExport::SetItem(ClNode* ai_poNode, m4pcchar_t ai_pcItemId, m4pcchar_t ai_pcItemValue)
{
	// items.
	const char *pcId = NULL;
	const char *pcValue = NULL;
	double dValue;
	m4return_t retCode = M4_ERROR;

	// Check input arguments.
	if ((NULL == ai_poNode) || (NULL == ai_pcItemId) || (NULL == ai_pcItemValue))
	{
		return M4_ERROR;
	}
	
	ClItem &item = ai_poNode->RecordSet.Current.Item[ai_pcItemId];
	if( item.GetIndex() == M4DM_EOF_INDEX )
	{
		m4Trace(cout << "SetItem SessionChannel: Item not found: " << ai_pcItemId << endl);
		return M4_ERROR;
	}

	m4VariantType oValue;

	switch (item.ItemDef.CppType()) 
	{
		case M4CL_CPP_TYPE_NUMBER:
			dValue = atof(ai_pcItemValue);
			oValue = m4VariantType(dValue);
			oValue.Type = M4CL_CPP_TYPE_NUMBER;
			retCode = item.Value.Set(oValue);
			break;
		case M4CL_CPP_TYPE_STRING:
			oValue = m4VariantType((m4pchar_t)ai_pcItemValue);
			oValue.Type = M4CL_CPP_TYPE_STRING_VAR;
			retCode = item.Value.Set(oValue);
			break;
		case M4CL_CPP_TYPE_DATE:
			ClStringToDate(ai_pcItemValue, dValue);
			oValue = m4VariantType(dValue);
			oValue.Type = M4CL_CPP_TYPE_DATE;
			retCode = item.Value.Set(oValue);
			break;
		default:
			m4Trace(cout << "Unsupported type." << endl);
			retCode = M4_ERROR;
			break;
	}

	return retCode;
}

