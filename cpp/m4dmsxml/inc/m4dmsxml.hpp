//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4dmsxml.hpp
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
//	=========================================================
//	=====================

#ifndef dmsxmlimport_h
#define dmsxmlimport_h 1

#include "m4dmsxml_dll.hpp"
#include "m4dmsxmldefines.hpp"
#include "m4var.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"



extern "C" M4_DECL_M4DMSXML m4return_t LoadXMLData			(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DMSXML m4return_t ExportXMLData		(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DMSXML m4return_t LoadXMLDataExtended	(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4DMSXML m4return_t ExportXMLDataExtended(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

extern "C" M4_DECL_M4DMSXML m4return_t XSLTransformation	(m4VariantType * ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

m4return_t StartImportProcess(ClAccess * ai_poAccess, m4int32_t ai_iMapType, const string &ai_sXmlFile, m4bool_t ai_bDoValidation, m4int32_t ai_iFirstRegister, m4int32_t ai_iLastRegister, m4bool_t ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bUseSchemaXML, m4bool_t ai_bElementConsistency, m4bool_t ai_bItemValidation);
m4return_t StartExportProcess(ClAccess * ai_poAccess, m4int32_t ai_iMapType, const string &ai_sXmlFile, const string &ai_sDTDPathFile, m4bool_t ai_bValidation, m4bool_t ai_bDoPublic, m4bool_t ai_bDefaultMappings, m4int32_t ai_iChannelHandle, m4bool_t ai_bUseSchemaXML, const string & ai_sSchemaNamespace, eUniXmlEncodingType_t ai_eEncoding);

#endif
