//	=============================================================================
//	
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              M4DMSXML.cpp
//	 Project:           M4DMSXML
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

#include "m4dmssaxhandlers.hpp"
#include "m4dmsm4objecttoxml.hpp"
#include "m4dmsxmlexception.hpp"
#include "m4dmsxml.hpp"
#include "m4dmsxmlerr.hpp"
#include <access.hpp>
#include <execontx.hpp>
#include "m4unicode.hpp"
#include "m4objglb.hpp"
#include "chlog.hpp"


// SAX Parser.
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/validators/schema/SchemaValidator.hpp>

// Xalan
#include <xalanc/XalanTransformer/m4xsltransformation.hpp>


#ifdef _WINDOWS
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, void** lpReserved)
{
	return TRUE;
}
#endif // _WINDOWS

/****************************************************************************
*	Name:			LoadXMLData
*	Description:	This function imports XML data into the current channel.
*	Parameters:
*  	Input:			0 (Input) -> Map type.
*					1 (Input) -> XML file name.
*					2 (Input) -> Do or not validation.
*					3 (Input) -> First register for the XML process.
*					4 (Input) -> Last register for the XML process.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t LoadXMLData(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t					iResult = M4_SUCCESS;
	string						sXMLFile, sElementBaseName;
	m4bool_t					bDoValidation = M4_TRUE;
	m4int32_t					iMapType, iFirstRegister, iLastRegister;
	m4int32_t					iChannelHandle = -1;
	m4bool_t					bUseSchemaXML = M4_FALSE;
	m4bool_t					bElementConsistency = M4_TRUE;
	m4bool_t					bItemValidation = M4_FALSE;
	
	// Check parameters number.
	if (ai_iLongArg != 5)
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get access to the channel executing in this moment.
	ClAccess * poAccess = ((ClVMRunContext *) ai_pvContext)->m_pAccess;
	if (poAccess == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poAccess#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get map type.
	// Default mappings: null value for this argument.
	m4bool_t bDefaultMappings = M4_FALSE;
	iMapType = -1;
	if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iMapType = (m4int32_t) ai_pvArg[0].Data.DoubleData;
	}
	else
	{
		// If map_type is NullValue(), it means default mappings.
		// In other case, it is an error.
		if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NULL)
		{
			bDefaultMappings = M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get XML file name.
	if ((ai_pvArg[1].Type != M4CL_CPP_TYPE_NULL) && (ai_pvArg[1].Data.PointerChar != NULL))
	{
		sXMLFile = (m4pchar_t) ai_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get validation flag. If true, do validation.
	if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bDoValidation = (ai_pvArg[2].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		// If validation argument is NullValue(), it means there is no validation.
		// In other case, it is an error.
		if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NULL)
		{
			bDoValidation = M4_FALSE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get the first register to begin the parsing process.
	// If null value or < 0, first register 0 (the first element).
	if (ai_pvArg[3].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iFirstRegister = (m4int32_t) ai_pvArg[3].Data.DoubleData;
	}
	else
	{
		iFirstRegister = 0;
	}

	// Get the last register to finish the parsing process.
	// If nullvalue or < 0 means that we want to parse all xml file.
	if (ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iLastRegister = (m4int32_t) ai_pvArg[4].Data.DoubleData;
	}
	else
	{
		iLastRegister = -1;
	}

	// If the first or the last register is < 0 means that we want to
	// parse all the xml file, all registers.
	if (iFirstRegister < 0)
	{
		iFirstRegister = 0;
	}

	// Check first register to parse is not greater than the last register.
	if ((iLastRegister != -1) && (iFirstRegister > iLastRegister))
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	// Start import XML process.
	iResult = StartImportProcess(poAccess, iMapType, sXMLFile, bDoValidation, iFirstRegister, iLastRegister, bDefaultMappings, iChannelHandle, bUseSchemaXML, bElementConsistency, bItemValidation);
	if (iResult != M4_SUCCESS)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;

	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			ExportXMLData
*	Description:	This function exports XML data from a channel into 
*					a given .
*	Parameters:
*  	Input:			0 (Input) -> Map type.
*					1 (Input) -> XML file name.
*					2 (Input) -> Do or not validation.
*					3 (Input) -> DTD file name.
*					4 (Input-optional) -> Public/System tag.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t ExportXMLData(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t					iRet = M4_SUCCESS;
	m4int32_t					iMapType;
	string						sXMLFile, sDTDPathFile;
	m4bool_t					bValidation;
	eUniXmlEncodingType_t		eEncoding = M4XmlNative;

	// Check parameters number.
	if (ai_iLongArg != 4 && ai_iLongArg != 5)
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get access to the channel executing in this moment.
	ClAccess * poAccess = ((ClVMRunContext *) ai_pvContext)->m_pAccess;
	if (poAccess == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poAccess#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get map type.
	// Default mappings: null value for this argument.
	m4bool_t bDefaultMappings = M4_FALSE;
	iMapType = -1;
	if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iMapType = (m4int32_t) ai_pvArg[0].Data.DoubleData;
	}
	else
	{
		// If map_type is NullValue(), it means default mappings.
		// In other case, it is an error.
		if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NULL)
		{
			bDefaultMappings = M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get XML file name (where data is going to be written the channel info).
	if ((ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR) && (ai_pvArg[1].Data.PointerChar != NULL))
	{
		sXMLFile = (m4pchar_t) ai_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get validation flag. If true, do validation.
	if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bValidation = (ai_pvArg[2].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		// If validation argument is NullValue(), it means there is no validation.
		// In other case, it is an error.
		if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NULL)
		{
			bValidation = M4_FALSE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get path for DTD file. This is only needed when validation is specified (1).
	sDTDPathFile = "";
	if (bValidation)
	{
		// fix bug 0090174
		if (ai_pvArg[3].Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			sDTDPathFile = (m4pchar_t) ai_pvArg[3].Data.PointerChar;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Include support for public tag
	// fix bug 0090163
	m4bool_t bDoPublic = M4_FALSE;
	if (ai_iLongArg > 4)
	{
		if (ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER)
		{
			bDoPublic = (ai_pvArg[4].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Check default mappings and validation. It is not possible to specified
	// validation and default mappings (maptype is null).
	if ((bDefaultMappings == M4_TRUE) && (bValidation == M4_TRUE))
	{
		SETCODEF(M4_DMS_PARTIAL_LOAD_ERROR, ERRORLOG, "#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}
	
	// No channel handle.
	m4int32_t iChannelHandle = -1;

	// Do not use Schema XML and no Schema namespaces.
	m4bool_t bUseSchemaXML = M4_FALSE;
	string sSchemaNamespace = "";

	// Begin to read the channel info and write it to the
	// XML file.
	iRet = StartExportProcess(poAccess, iMapType, sXMLFile, sDTDPathFile, bValidation, bDoPublic, bDefaultMappings, iChannelHandle, bUseSchemaXML, sSchemaNamespace, eEncoding);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_START_EXPORT_PROCESS_ERROR, ERRORLOG, "#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			LoadXMLDataExtended
*	Description:	This function imports XML data into the current channel,
*					same as LoadXMLData, but it provides all new funcitonalities:
*					- channel handle for DMS_M4OBJECT_XML_MAP
*					- use schema XML for validation.
*	Parameters:
*  	Input:			0 (Input) -> Map type.
*					1 (Input) -> XML file name.
*					2 (Input) -> Do or not validation.
*					3 (Input) -> First register for the XML process.
*					4 (Input) -> Last register for the XML process.
*					5 (Input) -> DMS_M4OBJECT_XML_MAP channel handle.
*					6 (Input) -> use Schema XML for validation.
*					7 (Input) -> check items consistency [Optional].
*					8 (Input) -> validate items value [Optional].
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*  Additional Information:
*
****************************************************************************/
m4return_t LoadXMLDataExtended(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t					iResult = M4_SUCCESS;
	string						sXMLFile;
	m4bool_t					bDoValidation = M4_TRUE;
	m4bool_t					bUseSchemaXML = M4_TRUE;
	m4int32_t					iMapType, iFirstRegister, iLastRegister, iChannelHandle;
	m4bool_t					bElementConsistency = M4_TRUE;
	m4bool_t					bItemValidation = M4_FALSE;
	
	// Check parameters number.
	if (ai_iLongArg < 7 || ai_iLongArg > 9)
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get access to the channel executing in this moment.
	ClAccess * poAccess = ((ClVMRunContext *) ai_pvContext)->m_pAccess;
	if (poAccess == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poAccess#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get map type.
	// Default mappings: null value for this argument.
	m4bool_t bDefaultMappings = M4_FALSE;
	iMapType = -1;
	if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iMapType = (m4int32_t) ai_pvArg[0].Data.DoubleData;
	}
	else
	{
		// If map_type is NullValue(), it means default mappings.
		// In other case, it is an error.
		if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NULL)
		{
			bDefaultMappings = M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get XML file name.
	if ((ai_pvArg[1].Type != M4CL_CPP_TYPE_NULL) && (ai_pvArg[1].Data.PointerChar != NULL))
	{
		sXMLFile = (m4pchar_t) ai_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get validation flag. If true, do validation.
	if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bDoValidation = (ai_pvArg[2].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		// If validation argument is NullValue(), it means there is no validation.
		// In other case, it is an error.
		if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NULL)
		{
			bDoValidation = M4_FALSE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get the first register to begin the parsing process.
	// If null value or < 0, first register 0 (the first element).
	if (ai_pvArg[3].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iFirstRegister = (m4int32_t) ai_pvArg[3].Data.DoubleData;
	}
	else
	{
		iFirstRegister = 0;
	}

	// Get the last register to finish the parsing process.
	// If nullvalue or < 0 means that we want to parse all xml file.
	if (ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iLastRegister = (m4int32_t) ai_pvArg[4].Data.DoubleData;
	}
	else
	{
		iLastRegister = -1;
	}

	// Get channel handle.
	if (ai_pvArg[5].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iChannelHandle = (m4int32_t) ai_pvArg[5].Data.DoubleData;
	}
	else
	{
		iChannelHandle = -1;
	}

	// Get use Schema XML to validate with an XML Schema.
	if (ai_pvArg[6].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bUseSchemaXML = (ai_pvArg[6].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Este argumento especifica si el usuario desea procesar con error
	// los elementos XML que no existan en el M4O (nodos o items).
	if (ai_iLongArg > 7)
	{
		if (ai_pvArg[7].Type == M4CL_CPP_TYPE_NUMBER)
		{
			bElementConsistency = (ai_pvArg[7].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Este argumento especifica si el usuario desea validar el valor de los items.
	if (ai_iLongArg > 8)
	{
		if (ai_pvArg[8].Type == M4CL_CPP_TYPE_NUMBER)
		{
			bItemValidation = (ai_pvArg[8].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Some checkings:
	// If the first or the last register is < 0 means that we want to
	// parse all the xml file, all registers.
	if (iFirstRegister < 0)
	{
		iFirstRegister = 0;
	}

	// Check first register to parse is not greater than the last register.
	if ((iLastRegister != -1) && (iFirstRegister > iLastRegister))
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	// Start import XML process.
	iResult = StartImportProcess(poAccess, iMapType, sXMLFile, bDoValidation, iFirstRegister, iLastRegister, bDefaultMappings, iChannelHandle, bUseSchemaXML, bElementConsistency, bItemValidation);
	if (iResult != M4_SUCCESS)
	{
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}
	
	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;
	return M4_SUCCESS;
}

/****************************************************************************
*	Name:			ExportXMLDataExtended
*	Description:	This function exports XML data from a channel into 
*					a given, same as ExportXMLData, but it provides all 
*					functionalities.
*					- channel handle for DMS_M4OBJECT_XML_MAP
*					- use schema XML for validation.
*	Parameters:
*  	Input:			0 (Input) -> Map type.
*					1 (Input) -> XML file name.
*					2 (Input) -> Use or not validation.
*					3 (Input) -> DTD file name.
*					4 (Input) -> Public/System tag.
*					5 (Input) -> DMS_M4OBJECT_XML_MAP channel handle.
*					6 (Input) -> use Schema XML for validation.
*					7 (Input) -> XML Schema namespace.
*
* 	Output:
*
*	Return:			0	No error.	
*					> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t ExportXMLDataExtended(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t					iRet = M4_SUCCESS;
	m4int32_t					iMapType;
	string						sXMLFile, sDTDPathFile, sSchemaNamespace;
	m4bool_t					bValidation = M4_FALSE;
	m4bool_t					bUseSchemaXML = M4_FALSE;
	eUniXmlEncodingType_t		eEncoding = M4XmlNative;

	// Check parameters number.
	if (ai_iLongArg < 8 || ai_iLongArg > 9)
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get access to the channel executing in this moment.
	ClAccess * poAccess = ((ClVMRunContext *) ai_pvContext)->m_pAccess;
	if (poAccess == NULL)
	{
		SETCODEF(M4_DMS_POINTER_NULL, ERRORLOG,"#*s1*#poAccess#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get map type.
	// Default mappings: null value for this argument.
	m4bool_t bDefaultMappings = M4_FALSE;
	iMapType = -1;
	if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iMapType = (m4int32_t) ai_pvArg[0].Data.DoubleData;
	}
	else
	{
		// If map_type is NullValue(), it means default mappings.
		// In other case, it is an error.
		if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NULL)
		{
			bDefaultMappings = M4_TRUE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get XML file name (where data is going to be written the channel info).
	if ((ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR) && (ai_pvArg[1].Data.PointerChar != NULL))
	{
		sXMLFile = (m4pchar_t) ai_pvArg[1].Data.PointerChar;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}
	
	// Get validation flag. If true, do validation.
	if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bValidation = (ai_pvArg[2].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		// If validation argument is NullValue(), it means there is no validation.
		// In other case, it is an error.
		if (ai_pvArg[2].Type == M4CL_CPP_TYPE_NULL)
		{
			bValidation = M4_FALSE;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}
	
	// Get path for DTD file. This is only needed when validation is specified (1).
	sDTDPathFile = "";
	if (bValidation)
	{
		if (ai_pvArg[3].Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			sDTDPathFile = (m4pchar_t) ai_pvArg[3].Data.PointerChar;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Get System/Public tag.
	m4bool_t bDoPublic = M4_FALSE;
	if (ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bDoPublic = (ai_pvArg[4].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get channel handle.
	m4int32_t iChannelHandle = 0;
	if (ai_pvArg[5].Type == M4CL_CPP_TYPE_NUMBER)
	{
		iChannelHandle = (m4int32_t) ai_pvArg[5].Data.DoubleData;
	}
	else
	{
		// If Nullvalue(), no channel handle.
		if (ai_pvArg[5].Type == M4CL_CPP_TYPE_NULL)
		{
			iChannelHandle = -1;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Get use Schema XML to validate with an XML Schema.
	if (ai_pvArg[6].Type == M4CL_CPP_TYPE_NUMBER)
	{
		bUseSchemaXML = (ai_pvArg[6].Data.DoubleData == 0) ? M4_FALSE : M4_TRUE;
	}
	else
	{
		SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_ERROR;
	}

	// Get Schema namespace. Just necessary if we use XML Schemas to validate.
	if (bUseSchemaXML == M4_TRUE)
	{
		if (ai_pvArg[7].Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			sSchemaNamespace = (m4pchar_t) ai_pvArg[7].Data.PointerChar;
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG,"#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Este argumento especifica el encoding en el que se quiere volcar el fichero XML.
	if (ai_iLongArg > 8)
	{
		if (ai_pvArg[8].Type == M4CL_CPP_TYPE_NUMBER)
		{
			if (ai_pvArg[8].Data.DoubleData == M4XmlNative)
			{
				eEncoding = M4XmlNative;
			}
			else if (ai_pvArg[8].Data.DoubleData == M4XmlANSI)
			{
				eEncoding = M4XmlANSI;
			}
			else if (ai_pvArg[8].Data.DoubleData == M4XmlISO)
			{
				eEncoding = M4XmlISO;
			}
			else if (ai_pvArg[8].Data.DoubleData == M4XmlUtf8)
			{
				eEncoding = M4XmlUtf8;
			}
			else
			{
				eEncoding = M4XmlNative;
			}
		}
		else
		{
			SETCODEF(M4_DMS_BAD_ARG, ERRORLOG, "#*s1*#");
			ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
			ai_pvReturn.Data.DoubleData = M4_ERROR;
			return M4_ERROR;
		}
	}

	// Some checkings.
	// Check default mappings and validation. It is not possible to specified
	// validation and default mappings (maptype is null).
	if ((bDefaultMappings == M4_TRUE) && (bValidation == M4_TRUE))
	{
		SETCODEF(M4_DMS_PARTIAL_LOAD_ERROR, ERRORLOG, "#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	// Begin to read the channel info and write it to the XML file.
	iRet = StartExportProcess(poAccess, iMapType, sXMLFile, sDTDPathFile, bValidation, bDoPublic, bDefaultMappings, iChannelHandle, bUseSchemaXML, sSchemaNamespace, eEncoding);
	if (iRet != M4_SUCCESS)
	{		
		SETCODEF(M4_DMS_START_EXPORT_PROCESS_ERROR, ERRORLOG, "#*s1*#");
		ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
		ai_pvReturn.Data.DoubleData = M4_ERROR;
		return M4_SUCCESS;
	}

	ai_pvReturn.Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvReturn.Data.DoubleData = M4_SUCCESS;	
	return M4_SUCCESS;
}


/****************************************************************************
*	Name:			XSLTransformation
*	Description:	Method that applies an XSL transformation over a XML file, resulting in another XML file 
*	Parameters:
*  	Input:			0 (Input) -> The input XML file.
*					1 (Input) -> The output XML file.
*					2 (Input) -> The input XSL file.
*					3 (Input) -> A params string like "p1::v1@@p2::v2@@p3::v3@@".
*
* 	Output:
*
*	Return:			 0	No error.	
*					-1  Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t XSLTransformation(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{

	int			iTransform = -1 ;
	m4pchar_t	pcParameters = NULL ;
	m4pcchar_t	pccError = NULL ;


	ai_pvReturn.SetDouble( M4_ERROR ) ;

	// Comprobaciones.
	// Verificar número de parámetros de entrada.
	CHECK_CHLOG_ERRORF( ai_iLongArg != 4, M4_SUCCESS, M4JDMS_ERROR_XSL_BAD_NUMBER_ARGUMENTS, "XSLTransformation" << ai_iLongArg << 4 ) ;

	// Fichero de entrada.
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JDMS_ERROR_XSL_BAD_ARGUMENT_TYPE, "Input file" << "XSLTransformation" << M4ClCppType( ai_pvArg[ 0 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 0 ].Data.PointerChar == NULL || *( ai_pvArg[ 0 ].Data.PointerChar ) == '\0' , M4_SUCCESS, M4JDMS_ERROR_XSL_NULL_ARGUMENT, "Input file" << "XSLTransformation" ) ;

	// Fichero de salida.
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JDMS_ERROR_XSL_BAD_ARGUMENT_TYPE, "Output file" << "XSLTransformation" << M4ClCppType( ai_pvArg[ 1 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 1 ].Data.PointerChar == NULL || *( ai_pvArg[ 1 ].Data.PointerChar ) == '\0' , M4_SUCCESS, M4JDMS_ERROR_XSL_NULL_ARGUMENT, "Output file" << "XSLTransformation" ) ;

	// Hoja XSL.
	CHECK_CHLOG_ERRORF( ai_pvArg[ 2 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JDMS_ERROR_XSL_BAD_ARGUMENT_TYPE, "XSL sheet" << "XSLTransformation" << M4ClCppType( ai_pvArg[ 2 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;
	CHECK_CHLOG_ERRORF( ai_pvArg[ 2 ].Data.PointerChar == NULL || *( ai_pvArg[ 2 ].Data.PointerChar ) == '\0' , M4_SUCCESS, M4JDMS_ERROR_XSL_NULL_ARGUMENT, "XSL sheet" << "XSLTransformation" ) ;

	// Parámetros.
	CHECK_CHLOG_ERRORF( ai_pvArg[ 3 ].Type != M4CL_CPP_TYPE_NULL && ai_pvArg[ 3 ].Type != M4CL_CPP_TYPE_STRING_VAR, M4_SUCCESS, M4JDMS_ERROR_XSL_BAD_ARGUMENT_TYPE, "Parameters" << "XSLTransformation" << M4ClCppType( ai_pvArg[ 3 ].Type ) << M4ClCppType( M4CL_CPP_TYPE_STRING_VAR ) ) ;

	if( ai_pvArg[ 3 ].Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		pcParameters = ai_pvArg[ 3 ].Data.PointerChar ;
	}

	// Perform transformation
	iTransform = M4XSLTransformation( ai_pvArg[ 0 ].Data.PointerChar, ai_pvArg[ 2 ].Data.PointerChar, ai_pvArg[ 1 ].Data.PointerChar, pcParameters, pccError ) ;

	if( iTransform == 0 )
	{
		ai_pvReturn.SetDouble( M4_SUCCESS ) ;
	}
	else
	{
		DUMP_CHLOG_ERRORF( M4JDMS_ERROR_XSL_TRANSFORMATION_ERROR, pccError ) ;
	}

	if( pccError != NULL )
	{
		delete( pccError ) ;
		pccError = NULL ;
	}

	return( M4_SUCCESS ) ;
}


/****************************************************************************
*	Name:			StartImportProcess
*	Description:	Do XML import process.
*	Parameters:
*	Return:			0	No error.	
*					> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t StartImportProcess(ClAccess * ai_poAccess, m4int32_t ai_iMapType, const string &ai_sXmlFile, m4bool_t ai_bDoValidation, m4int32_t ai_iFirstRegister, m4int32_t ai_iLastRegister, m4bool_t ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bUseSchemaXML, m4bool_t ai_bElementConsistency, m4bool_t ai_bItemValidation)
{
	m4pchar_t			pcException = NULL ;
	ClDMSSAXHandlers	oHandler(ai_sXmlFile, ai_iFirstRegister, ai_iLastRegister);

	// Save the channel access.
	m4return_t iResult = oHandler.Init(ai_poAccess);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_INIT_DATA_ERROR, ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	iResult = oHandler.InitializeXmlMapM4O(ai_iMapType, ai_bDoValidation, ai_sXmlFile, ai_bDefaultMappings, ai_iChannelHandle, ai_bElementConsistency, ai_bItemValidation);
	if (iResult != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_LOAD_XML_MAP_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}

	// Get root element info.
	iResult = oHandler.GetRootElementInfo();
	if (iResult != M4_SUCCESS)
	{
		// Release xml - m4o map channel.
		oHandler.ReleaseXmlMapM4O();

		SETCODEF(M4_DMS_GET_ROOT_ELEMENT_INFO_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}

	// Get base element info.
	iResult = oHandler.GetBaseElementInfo();
	if (iResult != M4_SUCCESS)
	{
		// Release xml - m4o map channel.
		oHandler.ReleaseXmlMapM4O();

		SETCODEF(M4_DMS_GET_BASE_ELEMENT_INFO_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}

	// Initialize the XML system.
	try
	{
		XMLPlatformUtils::Initialize();
	}
	
	catch (const XMLException& toCatch)
	{
		// Release xml - m4o map channel.
		oHandler.ReleaseXmlMapM4O();

		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)toCatch.getMessage() );
		SETCODEF(M4_DMS_XML_SAX_INIT_ERROR, ERRORLOG,"#*s1*#%s#", pcException);
		delete [] pcException ;
		return M4_ERROR;
	} 
	
	// Creates SAX parser.
	SAXParser parser;

	// Set parser features.
	SAXParser::ValSchemes valScheme;
	m4bool_t bSchemaFullChecking = M4_FALSE;
	m4bool_t bDoNamespaces = M4_FALSE;
	m4bool_t bDoSchema = M4_FALSE;
	if (ai_bDoValidation == M4_TRUE)
	{
		valScheme = SAXParser::Val_Always;
		bSchemaFullChecking = M4_TRUE;

		// Validation with a XML Schema or DTD.
		if (ai_bUseSchemaXML == M4_TRUE)
		{
			// Validation with a XML Schema.
			bDoNamespaces = M4_TRUE;
			bDoSchema = M4_TRUE;
		}
		else
		{
			// Validation with a DTD.
			bDoNamespaces = M4_FALSE;
			bDoSchema = M4_FALSE;
		}
	}
	else
	{
		// No validation.
		valScheme = SAXParser::Val_Never;
		bSchemaFullChecking = M4_FALSE;
		bDoNamespaces = M4_FALSE;
		bDoSchema = M4_FALSE;
	}

	bSchemaFullChecking = M4_FALSE;
	parser.setValidationScheme(valScheme);
	parser.setDoNamespaces(bDoNamespaces);
	parser.setDoSchema(bDoSchema);
	parser.setValidationSchemaFullChecking(bSchemaFullChecking);
	
	// This flag indicates whether we bail out on XML error.
    // or not. M4_FALSE = we continue parsing on XML errors.
	parser.setExitOnFirstFatalError(M4_FALSE);
	
	parser.setDocumentHandler(&oHandler);
	parser.setErrorHandler(&oHandler);
	
	try 
	{
		parser.parse(ai_sXmlFile.c_str());

		/* Bug 0125617
		Hay que devolver error si lo ha habido
		*/
		iResult = oHandler.GetError() ;
	}
	catch (const XMLException& toCatch)
	{
		// Release xml - m4o map channel.
		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)toCatch.getMessage() );
		SETCODEF(M4_DMS_XML_PARSE_ERROR, ERRORLOG,"#*s1*#%s#", pcException);
		delete [] pcException ;
		iResult = M4_ERROR;
	}
    catch (const ClDMSXmlException& toCatch)
    {
		// Release xml - m4o map channel.
		SETCODEF(M4_DMS_XML_PARSE_END, DEBUGINFOLOG,"#*s1*#%d#%d#", toCatch.m_uiCurrentRegister, toCatch.m_eCode);

		// Bug 0144798.
		iResult = M4_SUCCESS;
    }

	// Release xml - m4o map channel.
	oHandler.ReleaseXmlMapM4O();

	return iResult;
}

/****************************************************************************
*	Name:			StartExportProcess
*	Description:	Do XML export process.
*	Parameters:
*	Return:			0	No error.	
*					> 0 Error.
*
*	Additional Information:
*
****************************************************************************/
m4return_t StartExportProcess(ClAccess * ai_poAccess, m4int32_t ai_iMapType, const string &ai_sXmlFile, const string &ai_sDTDPathFile, m4bool_t ai_bValidation, m4bool_t ai_bDoPublic, m4bool_t ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bUseSchemaXML, const string & ai_sSchemaNamespace, eUniXmlEncodingType_t ai_eEncoding)
{
	m4bool_t bElementConsistency = M4_TRUE;
	
	ClDMSM4ObjectToXml oM4OToXMLBuilder(ai_bValidation, ai_sDTDPathFile, ai_bUseSchemaXML, ai_sSchemaNamespace);

	// Save the channel access.
	m4return_t iRet = oM4OToXMLBuilder.InitEnv(ai_poAccess);
	if (iRet != M4_SUCCESS)
	{
		SETCODEF(M4_DMS_INIT_DATA_ERROR, ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	// Load XML - M4O channel data.
	// We do not need to copy DTD file, so doValidation = 0 and XML file = "".
	iRet = oM4OToXMLBuilder.InitializeXmlMapM4O(ai_iMapType, M4_FALSE, "", ai_bDefaultMappings, ai_iChannelHandle, bElementConsistency);
	if (iRet != M4_SUCCESS)
	{
		oM4OToXMLBuilder.ReleaseEnv();
		SETCODEF(M4_DMS_LOAD_XML_MAP_M4O_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}

	// Begin to read the channel info and write it to the XML file.
	iRet = oM4OToXMLBuilder.StartExportProcess(ai_sXmlFile, ai_bDoPublic, ai_eEncoding);
	if (iRet != M4_SUCCESS)
	{
		oM4OToXMLBuilder.ReleaseEnv();
		SETCODEF(M4_DMS_START_EXPORT_PROCESS_ERROR, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}

	// Release channels (XML - M4O Map channel and export channel, if needed).
	oM4OToXMLBuilder.ReleaseEnv();

	return M4_SUCCESS;
}

