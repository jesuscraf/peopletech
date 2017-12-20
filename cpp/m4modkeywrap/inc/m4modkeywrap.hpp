//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkeywrap (VB)
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================


#ifndef	_M4MODKEYWRAP_M4MODKEYWRAP_HPP_
#define _M4MODKEYWRAP_M4MODKEYWRAP_HPP_

#include "m4modkeywrap_dll.hpp"

//Tipos de windows.
#include "wtypes.h"

//Importante:
//Las funciones devuelven SHORT (2 bytes) y su analogo en VB es Integer. Ambos
//ocupan 2 bytes.
//el tipo de datos BOOL ocupa 4 bytes, por lo tanto no es compatible con el tipo
//de datos integer de VB.
//------------------------------------------------
// Analizadores.
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_CreateAnalyzerModule( LPLONG ao_pIXmlAnalyzer );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_CreateAnalyzerLicense( LPLONG ao_pIXmlAnalyzer );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_DestroyAnalyzer( LPLONG aio_pIXmlAnalyzer );
//------------------------------------------------
// FIN Analizadores.
//------------------------------------------------

//------------------------------------------------
// Encriptación/Desencriptacion.
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_CreateEncryption( LPLONG ao_pClEncryption );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_SendKeyB( LONG ai_pClEncryption, LPCSTR ai_pBufferEncrypted, LONG ai_iSizeEncrypted, LPBSTR ao_pSignature, LONG ai_iSizeSignature );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_EncryptObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCode, LPBSTR ao_pCodeEncrypted );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_DecryptObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCodeEncrypted, LPBSTR ao_pCode );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_IsEncryptedSecurizedObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCodeEncrypted, PSHORT ao_bIsEncrypted );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_DestroyEncryption( LONG ai_pClEncryption );
//------------------------------------------------
// FIN Encriptación/Desencriptacion.
//------------------------------------------------

//------------------------------------------------
// Summary
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_CreatePropertiesBySummary( LPSTR ai_pBufferEncrypted, LPLONG ao_pClProperties );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_CreatePropertiesByLicense( LONG ai_pClLicense, LPLONG ao_pClProperties );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetPropertyTransById( LONG ai_pClProperties, LPSTR ai_pPropertyId, LPBSTR ao_pPropertyTrans);
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetPropertyValueById( LONG ai_pClProperties, LPSTR ai_pPropertyId, LPBSTR ao_pPropertyValue);

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_DestroyProperties( LPLONG aio_pClProperties );
//------------------------------------------------
// FIN Summary
//------------------------------------------------

//------------------------------------------------
// Analisis del xml (COMUN).
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_AnalyzeXmlFile( LONG ai_pIXmlAnalyzer, BSTR ai_pBuffer );
//------------------------------------------------
// FIN Analisis del xml (COMUN).
//------------------------------------------------

//------------------------------------------------
// Informacion de modulos segurizables.
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_IsSecurizedObject( LONG ai_pIXmlAnalyzer, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObject );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedModules( LONG ai_pIXmlAnalyzer, LPLONG ao_pNumSecurizedModules );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedModuleByIndex( LONG ai_pIXmlAnalyzer, LONG ai_lIndex, LPLONG ao_pClSecurizedModule );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjects( LONG ai_pClSecurizedModule, LPLONG ao_pNumSecurizedObjects );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectByIndex( LONG ai_pClSecurizedModule, LONG ai_lIndex, LPLONG ao_pClSecurizedObject );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdModule( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdModule );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdNodeStructure( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdNodeStructure );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdItem( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdItem );
//------------------------------------------------
// FIN Informacion de modulos segurizables.
//------------------------------------------------

//------------------------------------------------
// Informacion de licencia.
//------------------------------------------------
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetClLicense( LONG ai_pIXmlAnalyzer, LPSTR ai_pBufferEncrypted, LPLONG ao_pClLicense );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_IsSecurizedObjectNotAllowed( LONG ai_pClLicense, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObjectNotAllowed );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_IsSecurizedObjectAllowed( LONG ai_pClLicense, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObjectAllowed );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjectsNotAllowed( LONG ai_pClLicense, LPLONG ao_pNumSecurizedObjectsNotAllowed );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectNotAllowedByIndex( LONG ai_pClLicense, LONG ai_lIndex, LPLONG ao_pClSecurizedObject );

extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjectsAllowed( LONG ai_pClLicense, LPLONG ao_pNumSecurizedObjectsAllowed );
extern "C" M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectAllowedByIndex( LONG ai_pClLicense, LONG ai_lIndex, LPLONG ao_pClSecurizedObject );

//------------------------------------------------
// FIN Informacion de licencia.
//------------------------------------------------

#endif


