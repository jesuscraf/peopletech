//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           OLEExec.dll
// File:             misc.cpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "m4oleexec_dll.hpp"

extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_Execute				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_Property			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_SetProp				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_GetProp				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t Initialise				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t Release					(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t ReleaseCol				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t GetObjectFromCollection	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t GetObjectFromProperty	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t GetObjectFromMethod		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_PROPERTY_NAME		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_EXECUTE_NAME		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_PROPERTY_ID			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_EXECUTE_ID			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t INITIALISE_LIC			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t InitialiceCom			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t ExecuteCom				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t PropertyCom				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t ReleaseCom				(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t GetPropertyCom			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t PutPropertyCom			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t PasteSpecialText		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t GetObjectInstance		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t DumpRangeToNode			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4OLEEXEC m4return_t OLE_ExecRetrying		(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);

m4bool_t restoreClipboard(HANDLE ai_hMem);
m4return_t CheckServerAlive(IDispatch* ai_iDsp);


