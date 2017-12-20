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

#include "m4modkeywrap.hpp"

#include "xmlanalyzerfactory.hpp"
#include "xmlanalyzerinterface.hpp"
#include "license.hpp"
#include "encrypt.hpp"
#include "licenseproperties.hpp"

//-------------------------------------------------------------------------------
//EXPLICACIÓ RELACIÓ DE LA FALLA
//-------------------------------------------------------------------------------
//Escenario1 (Creación del mdb generico):
//1.- Ramdl creará el analizador de modulos.
//2.- El contenido del fichero se analiza (el analizador contendrá toda información).
//3.- Para cada objeto segurizable se pregunta (al analizador) si está en el paquete.
//	3.1.- Si SI esta, tenemos que encriptar el objeto.
//	3.2.- Si NO esta no pasa nada.
//4.- En este punto, todos los objetos segurizables deben haber sido encriptados.
//5.- Destruimos el analizador.

//Escenario2 (Creación del mdb para el cliente):
//1.- Ramdl creará el analizador de licencia.
//2.- El contenido del fichero SE DESENCRIPTA y se analiza -> Obtenemos el objeto licencia.
//3.- Para cada objeto encriptado se pregunta si hay permiso.
//	3.1.- Si SI hay permiso, tenemos que desencriptar.
//	3.2.- Si NO hay hay permiso, se deja encriptado.
//4.- En este punto, todos los objetos segurizables que el cliente tiene permiso
//se han desencriptado. Se han quedado encriptados los que no tiene permiso.
//5.- Destruimos el analizador.
//-------------------------------------------------------------------------------
//FIN EXPLICACIÓ RELACIÓ DE LA FALLA
//-------------------------------------------------------------------------------


//------------------------------------------------
// Variables globales
//------------------------------------------------

//Creamos un objeto estático que será la factoria de analizadores.
static	ClXmlAnalyzerFactory	g_oClXmlAnalyzerFactory;

//------------------------------------------------
// FIN Variables globales
//------------------------------------------------

//------------------------------------------------
// Creacion/Destruccion de Analizadores.
//------------------------------------------------
//Pasos:
//	1.- Crearemos el analizador que corresponda.
//	2.- Cuando finalicemos lo destruiremos.

SHORT WINAPI M4MODKEYWRAP_CreateAnalyzerModule( LPLONG ao_pIXmlAnalyzer )
{
	//Pedimos a la factoria un analizador de modulos.
	IXmlAnalyzer * pIXmlAnalizer = g_oClXmlAnalyzerFactory.CreateInstanceXmlAnalyzer(OBJECT_TYPE_MODULE);
	if (pIXmlAnalizer != NULL) {
		*ao_pIXmlAnalyzer = (LONG) pIXmlAnalizer;		
		return TRUE;
	}
	*ao_pIXmlAnalyzer = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_CreateAnalyzerLicense( LPLONG ao_pIXmlAnalyzer )
{
	//Pedimos a la factoria un analizador de licencias
	IXmlAnalyzer * pIXmlAnalizer = g_oClXmlAnalyzerFactory.CreateInstanceXmlAnalyzer(OBJECT_TYPE_LICENSE);
	if (pIXmlAnalizer != NULL) {
		*ao_pIXmlAnalyzer = (LONG) pIXmlAnalizer;
		return TRUE;
	}
	*ao_pIXmlAnalyzer = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_DestroyAnalyzer( LPLONG aio_pIXmlAnalyzer )
{
	//Destruimos el analizador.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) (*aio_pIXmlAnalyzer);
	if (pIXmlAnalizer != NULL) {
		g_oClXmlAnalyzerFactory.DestroyInstanceXmlAnalyzer(pIXmlAnalizer);
		*aio_pIXmlAnalyzer = (LONG) NULL;
		return TRUE;
	}
	*aio_pIXmlAnalyzer = (LONG) NULL;
	return FALSE;
}
//------------------------------------------------
// FIN Creacion/Destruccion de Analizadores.
//------------------------------------------------

//------------------------------------------------
// Encriptación/Desencriptacion.
//------------------------------------------------
//Pasos:
//	1.- Hay que crear un objeto encriptador.
//	2.- Se invoca a la funcion que establece la clave de encriptación
//	3.- Ya se pueden realizar operaciones de:
//		3.1.- Desencriptar la licencia para poderla analizar.
//			La operacion de encriptacion en principio no es necesaria.
//		3.2.- Encriptar objetos (items).
//		3.3.- Desencriptar objetos (items).
//		3.4.- Preguntar si un buffer (asociado al codigo LN4 o polaca) esta encriptado.
//	4.- Destruimos el objeto encriptador.

SHORT WINAPI M4MODKEYWRAP_CreateEncryption( LPLONG ao_pClEncryption )
{
	//Creamos un objeto para encriptar.
	ClEncryption * pClEncryption = new ClEncryption();
	if (pClEncryption != NULL) {
		*ao_pClEncryption = (LONG) pClEncryption;		
		return TRUE;
	}
	*ao_pClEncryption = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_SendKeyB( LONG ai_pClEncryption, LPCSTR ai_pBufferEncrypted, LONG ai_iSizeEncrypted, LPBSTR ao_pSignature, LONG ai_iSizeSignature )
{
	//Nos permite establecer la clave de encriptacion.

	//El buffer de entrada contiene una cadena encriptada con la ClaveA y dentro tendremos la ClaveB.
	//Nos dan el tamaño del buffer de entrada.
	//Se devuelve un buffer cuyo tamaño lo conoce el invocante.
	//El cuarto parametro es la longitud de la firma que espera el invocante.

	ClEncryption * pClEncryption = (ClEncryption *) ai_pClEncryption;
	if (pClEncryption != NULL) {

		m4pchar_t pBufferEncrypted = new m4char_t[ai_iSizeEncrypted + 1];
		if (pBufferEncrypted == NULL) {
			*ao_pSignature = (BSTR) NULL;
			return FALSE;
		}
		memcpy(pBufferEncrypted, ai_pBufferEncrypted, ai_iSizeEncrypted);
		pBufferEncrypted[ai_iSizeEncrypted] = NULL;

		//Reservamos para que nos dejen la firma que la enviaremos al invocante.
		m4int32_t iSizeSignature = (m4int32_t) ai_iSizeSignature;
		m4pchar_t pSignature =  new m4char_t [(m4int32_t) iSizeSignature + 1];
		if (pSignature == NULL) {
			delete [] pBufferEncrypted;
			*ao_pSignature = (BSTR) NULL;
			return FALSE;
		}
		pSignature[iSizeSignature] = NULL;
		if (pClEncryption->SendKeyB( pBufferEncrypted, ai_iSizeEncrypted, pSignature, iSizeSignature) == M4_SUCCESS) {
			//Tenemos que copiar la firma para que VB la verifique.
			//-- Determina la longitud de un null terminado.			---
			//-- m4uint32_t iSizeBuffer = SysStringByteLen(ai_pBuffer);	---
			*ao_pSignature = SysAllocStringByteLen((LPCSTR)pSignature, (UINT)iSizeSignature);
			delete [] pBufferEncrypted;
			delete [] pSignature;
			return TRUE;
		}
		delete [] pBufferEncrypted;
		delete [] pSignature;
	}
	*ao_pSignature = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_EncryptObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCode, LPBSTR ao_pCodeEncrypted )
{
	//Encripta un objeto segurizable
	ClEncryption * pClEncryption = (ClEncryption *) ai_pClEncryption;
	if ((pClEncryption != NULL) 
		&& (ai_pNS != NULL) 
		&& (ai_pItem != NULL)
		&& (ai_pCode != NULL))
	{
		m4int_t iText = 0;
		m4pchar_t pText = NULL;
		m4int_t iCode = strlen((m4pcchar_t) ai_pCode);
		m4pchar_t pCode = new m4char_t [iCode + 1];		
		if (pCode == NULL) {
			*ao_pCodeEncrypted = (BSTR) NULL;
			return FALSE;
		}
		memcpy(pCode, (m4pcchar_t) ai_pCode, iCode);
		pCode[iCode] = NULL;
		ClSecurizedObject oClSecurizedObject(NULL, (m4pcchar_t) ai_pNS, (m4pcchar_t)ai_pItem);
		if (pClEncryption->EncryptSecurizedObject(oClSecurizedObject, pCode, iCode, pText, iText) == M4_SUCCESS) {
			//Tendremos en las variables de salida el buffer desencriptado.
			//Necesitamos pasarlo a VB.
			if (pText != NULL) {
				*ao_pCodeEncrypted = SysAllocStringByteLen((LPCSTR)pText, (UINT)iText);
				delete [] pText;
				delete [] pCode;
				return TRUE;
			}
		}
		delete [] pCode;
	}
	*ao_pCodeEncrypted = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_DecryptObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCodeEncrypted, LPBSTR ao_pCode )
{
	//Encripta un objeto segurizable
	ClEncryption * pClEncryption = (ClEncryption *) ai_pClEncryption;
	if ((pClEncryption != NULL) 
		&& (ai_pNS != NULL) 
		&& (ai_pItem != NULL)
		&& (ai_pCodeEncrypted != NULL))
	{
		m4int_t iText = 0;
		m4pchar_t pText = NULL;		
		ClSecurizedObject oClSecurizedObject(NULL, (m4pcchar_t) ai_pNS, (m4pcchar_t)ai_pItem);
		if (pClEncryption->DecryptSecurizedObject(oClSecurizedObject, (m4pcchar_t) ai_pCodeEncrypted, strlen((m4pcchar_t) ai_pCodeEncrypted), pText, iText) == M4_SUCCESS) {
			//Tendremos en las variables de salida el buffer desencriptado.
			//Necesitamos pasarlo a VB.
			if (pText != NULL) {
				*ao_pCode = SysAllocStringByteLen((LPCSTR)pText, (UINT)iText);
				delete [] pText;
				return TRUE;
			}
		}		
	}
	*ao_pCode = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_IsEncryptedSecurizedObject( LONG ai_pClEncryption, LPCSTR ai_pNS, LPCSTR ai_pItem, LPCSTR ai_pCodeEncrypted, PSHORT ao_bIsEncrypted )
{
	ClEncryption * pClEncryption = (ClEncryption *) ai_pClEncryption;
	if (pClEncryption != NULL) {
		ClSecurizedObject oClSecurizedObject(NULL, (m4pcchar_t) ai_pNS, (m4pcchar_t)ai_pItem);
		if (pClEncryption->IsEncryptedSecurizedObject(oClSecurizedObject, (m4pcchar_t) ai_pCodeEncrypted, strlen((m4pcchar_t) ai_pCodeEncrypted)) == M4_TRUE) {
			*ao_bIsEncrypted = TRUE;
			return TRUE;
		}		
		else {
			*ao_bIsEncrypted = FALSE;
			return TRUE;
		}
	}	
	*ao_bIsEncrypted = FALSE;
	return FALSE;
}

M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_DestroyEncryption( LPLONG aio_pClEncryption )
{	
	ClEncryption * pClEncryption = (ClEncryption *) (*aio_pClEncryption);
	if (pClEncryption != NULL) {
		delete pClEncryption;
		*aio_pClEncryption = (LONG) NULL;
		return TRUE;
	}
	*aio_pClEncryption = (LONG) NULL;
	return FALSE;
}


//------------------------------------------------
// FIN Encriptación/Desencriptacion
//------------------------------------------------

//------------------------------------------------
// Summary
//------------------------------------------------

SHORT WINAPI M4MODKEYWRAP_CreatePropertiesBySummary( LPSTR ai_pBufferEncrypted, LPLONG ao_pClProperties )
{
	//El resumen de licencia se puede crear directamente con el buffer encriptado.

	ClModKeyProperties * pClProperties = new ClModKeyProperties();
	if (pClProperties != NULL) {
		m4return_t iRet = pClProperties->Init((m4pcchar_t) ai_pBufferEncrypted, strlen((m4pcchar_t) ai_pBufferEncrypted + 1));
		if (iRet == M4_SUCCESS) {
			*ao_pClProperties = (LONG) pClProperties;
			return TRUE;
		}
		delete pClProperties;
	}
	*ao_pClProperties = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_CreatePropertiesByLicense( LONG ai_pClLicense, LPLONG ao_pClProperties )
{
	//Creamos las propiedades en función de la licencia.	
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {
		ClModKeyProperties * pClProperties = NULL;		
		if (pClLicense->CreatePropertiesByLicense(pClProperties) == M4_SUCCESS) {
			*ao_pClProperties = (LONG) pClProperties;
			return TRUE;
		}
	}
	*ao_pClProperties = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetPropertyTransById( LONG ai_pClProperties, LPSTR ai_pPropertyId, LPBSTR ao_pPropertyTrans)
{
	//Encripta un objeto segurizable
	ClModKeyProperties * pClProperties = (ClModKeyProperties *) ai_pClProperties;
	if ((pClProperties != NULL) 
		&& (ai_pPropertyId != NULL))
	{
		m4pcchar_t pTrans = pClProperties->GetPropertyTransById((m4pcchar_t)ai_pPropertyId);
		if (pTrans != NULL) {
			//Necesitamos pasarlo a VB.
			*ao_pPropertyTrans = SysAllocStringByteLen((LPCSTR)pTrans, (UINT)strlen(pTrans));
			return TRUE;
		}
	}
	*ao_pPropertyTrans = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetPropertyValueById( LONG ai_pClProperties, LPSTR ai_pPropertyId, LPBSTR ao_pPropertyValue)
{
	//Encripta un objeto segurizable
	ClModKeyProperties * pClProperties = (ClModKeyProperties *) ai_pClProperties;
	if ((pClProperties != NULL) 
		&& (ai_pPropertyId != NULL))
	{
		m4pcchar_t pValue = pClProperties->GetPropertyValueById((m4pcchar_t)ai_pPropertyId);
		if (pValue != NULL) {
			//Necesitamos pasarlo a VB.
			*ao_pPropertyValue = SysAllocStringByteLen((LPCSTR)pValue, (UINT)strlen(pValue));
			return TRUE;
		}
	}
	*ao_pPropertyValue = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_DestroyProperties( LPLONG aio_pClProperties )
{
	ClModKeyProperties * pClProperties = (ClModKeyProperties *) (*aio_pClProperties);
	if (pClProperties != NULL) {
		delete pClProperties;
		*aio_pClProperties = (LONG) NULL;
		return TRUE;
	}
	*aio_pClProperties = (LONG) NULL;
	return FALSE;
}
//------------------------------------------------
// FIN Summary
//------------------------------------------------

//------------------------------------------------
// Analisis del xml (COMUN).
//------------------------------------------------
//Pasos:
//	1.- El analizador estará creado (MODULOS O LICENCIA).
//	2.- Necesitamos analizar el xml.
//	3.- Importante: Si falla el analisis el invocante deberia destruir y crear de nuevo
//		el analizador!.

M4_DECL_M4MODKEYWRAP SHORT WINAPI M4MODKEYWRAP_AnalyzeXmlFile( LONG ai_pIXmlAnalyzer, LPCSTR ai_pBuffer )
{
	//Importante:
	//Si el analisis falla deberíamos eliminar el analizador.

	//Analizamos el buffer que contiene la informacion de modulos.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
	if (pIXmlAnalizer != NULL) {
		//Si el analisis falla deberíamos eliminar el analizador.
		m4pcchar_t pBuffer = ai_pBuffer;
		if (pBuffer == NULL) {
			return FALSE;
		}
		if (pIXmlAnalizer->AnalyzeXmlFile(pBuffer) != M4_ERROR) {
			return TRUE;
		}
	}
	return FALSE;
}
//------------------------------------------------
// FIN Analisis del xml (COMUN).
//------------------------------------------------

//------------------------------------------------
// Informacion de modulos segurizables.
//------------------------------------------------
//Pasos:
//	1.- Tendremos analizado el xml y en el analizador estara la informacion de MODULOS.
//	2.- A partir del analizador podremos obtener los modulos segurizables.
//		Y a cada modulo le preguntaremos por sus objetos segurizables.

SHORT WINAPI M4MODKEYWRAP_IsSecurizedObject( LONG ai_pIXmlAnalyzer, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObject )
{
	//Despues de analizar el fichero de modulos-objetos podremos preguntar si un 
	//determinado objeto debe ser segurizable, es decir, ecncriptarse.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
	if (pIXmlAnalizer != NULL) {		
		m4int32_t iNumSecurizedModules = pIXmlAnalizer->GetNumSecurizedModules();
		for (m4int32_t i=0; i<iNumSecurizedModules; i++) {
			const ClSecurizedModule * pClSecurizedModule = pIXmlAnalizer->GetSecurizedModuleByIndex(i);
			if (pClSecurizedModule != NULL) {
				m4int32_t iNumSecurizedObjects = pClSecurizedModule->GetNumSecurizedObjects();
				for (m4int32_t j=0; j<iNumSecurizedModules; j++) {
					const ClSecurizedObject * pClSecurizedObject = pClSecurizedModule->GetSecurizedObjectByIndex(j);
					if (pClSecurizedObject != NULL) {
						ClSecurizedObject oCopy(NULL, (m4pcchar_t) ai_pClSecurizedObjectIdNodeStructure, (m4pcchar_t) ai_pClSecurizedObjectIdItem);
						if (pClSecurizedObject->IsEqualById(oCopy) == M4_TRUE) {
							*ao_bIsSecurizedObject = TRUE;							
							return TRUE;							
						}						
					}
				}

			}
		}
		*ao_bIsSecurizedObject = FALSE;
		return TRUE;
	}
	*ao_bIsSecurizedObject = FALSE;
	return FALSE;
}


SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedModules( LONG ai_pIXmlAnalyzer, LPLONG ao_pNumSecurizedModules )
{
	//Obtenemos cuantos modulos hay en el analizador y lo retornamos en el argumento por referencia.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
	if (pIXmlAnalizer != NULL) {		
		*ao_pNumSecurizedModules = (LONG) (pIXmlAnalizer->GetNumSecurizedModules());
		return TRUE;
	}
	*ao_pNumSecurizedModules = (LONG) 0;
	return FALSE;	
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedModuleByIndex( LONG ai_pIXmlAnalyzer, LONG ai_lIndex, LPLONG ao_pClSecurizedModule )
{
	//Obtenemos el modulo iesimo que hay en el analizador.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
	if (pIXmlAnalizer != NULL) {		
		const ClSecurizedModule * pClSecurizedModule = pIXmlAnalizer->GetSecurizedModuleByIndex((m4int32_t) ai_lIndex);
		if (pClSecurizedModule != NULL) {
			*ao_pClSecurizedModule = (LONG) pClSecurizedModule;
			return TRUE;
		}
	}
	*ao_pClSecurizedModule = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjects( LONG ai_pClSecurizedModule, LPLONG ao_pNumSecurizedObjects )
{
	//Obtenemos cuantos objetos hay para el modulo.
	ClSecurizedModule * pClSecurizedModule = (ClSecurizedModule *) ai_pClSecurizedModule;
	if (pClSecurizedModule != NULL) {		
		*ao_pNumSecurizedObjects = (LONG) (pClSecurizedModule->GetNumSecurizedObjects());
		return TRUE;
	}
	*ao_pNumSecurizedObjects = (LONG) 0;
	return FALSE;	
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectByIndex( LONG ai_pClSecurizedModule, LONG ai_lIndex, LPLONG ao_pClSecurizedObject )
{
	//Obtenemos el objeto iesimo de un determinado modulo.
	ClSecurizedModule * pClSecurizedModule = (ClSecurizedModule *) ai_pClSecurizedModule;
	if (pClSecurizedModule != NULL) {
		const ClSecurizedObject * pClSecurizedObject = pClSecurizedModule->GetSecurizedObjectByIndex((m4int32_t) ai_lIndex);
		if (pClSecurizedObject != NULL) {
			*ao_pClSecurizedObject = (LONG) pClSecurizedObject;
			return TRUE;
		}
	}
	*ao_pClSecurizedObject = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdModule( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdModule )
{
	//Devolvemos el identificador de un determinado objeto.
	ClSecurizedObject * pClSecurizedObject = (ClSecurizedObject *) ai_pClSecurizedObject;
	if (pClSecurizedObject != NULL) {
		const ClSecurizedModule * pClSecurizedModule = pClSecurizedObject->GetModule();
		if (pClSecurizedModule != NULL) {
			*ao_pClSecurizedObjectIdModule = SysAllocStringByteLen((LPCSTR) pClSecurizedModule->GetIdModule().c_str(), (UINT) pClSecurizedModule->GetIdModule().size());
			return TRUE;
		}
	}
	*ao_pClSecurizedObjectIdModule = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdNodeStructure( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdNodeStructure )
{
	//Devolvemos el identificador de un determinado objeto.
	ClSecurizedObject * pClSecurizedObject = (ClSecurizedObject *) ai_pClSecurizedObject;
	if (pClSecurizedObject != NULL) {
		*ao_pClSecurizedObjectIdNodeStructure = SysAllocStringByteLen((LPCSTR) pClSecurizedObject->GetIdNodeStructure().c_str(), (UINT) pClSecurizedObject->GetIdNodeStructure().size());
		return TRUE;
	}
	*ao_pClSecurizedObjectIdNodeStructure = (BSTR) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectIdItem( LONG ai_pClSecurizedObject, LPBSTR ao_pClSecurizedObjectIdItem )
{
	//Devolvemos el identificador de un determinado objeto.
	ClSecurizedObject * pClSecurizedObject = (ClSecurizedObject *) ai_pClSecurizedObject;
	if (pClSecurizedObject != NULL) {
		*ao_pClSecurizedObjectIdItem = SysAllocStringByteLen((LPCSTR) pClSecurizedObject->GetIdItem().c_str(), (UINT) pClSecurizedObject->GetIdItem().size());
		return TRUE;
	}
	*ao_pClSecurizedObjectIdItem = (BSTR) NULL;
	return FALSE;
}

//------------------------------------------------
// FIN Informacion de modulos segurizables.
//------------------------------------------------

//------------------------------------------------
// Informacion de licencia.
//------------------------------------------------
//Pasos:
//	1.- Pasamos un analizador de licencias.
//	2.- El buffer donde se encuentra la licencia encriptada.
//	3.- Obtenemos un handle de licencia..

//------------  Esta la comentamos pq no es necesaria!. -------------

//	SHORT WINAPI M4MODKEYWRAP_GetClLicense( LONG ai_pIXmlAnalyzer, LONG ai_pClEncryption, LPSTR ai_pBufferEncrypted, LPLONG ao_pClLicense )
//	{
//		//Obtenemos la licencia.
//		IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
//		if (pIXmlAnalizer != NULL) {		
//		ClEncryption * pClEncryption = (ClEncryption *) ai_pClEncryption;
//		if (pClEncryption != NULL) {
//			m4int_t iText = 0;
//			m4pchar_t pText = NULL;		
//			if (pClEncryption->Decrypt( (m4pcchar_t) ai_pBufferEncrypted, strlen((m4pcchar_t) ai_pBufferEncrypted), pText, iText) == M4_ERROR) {
//				*ao_pClLicense = (LONG) NULL;
//				return FALSE;
//			}
//			//Tendremos en las variables de salida el buffer desencriptado.
//				
//			//Analizamos el buffer de licencia. El buffer no es nulo.
//			if (pIXmlAnalizer->AnalyzeXmlFile(pText) == M4_ERROR) {
//				delete [] pText;					
//				*ao_pClLicense = (LONG) NULL;
//				return FALSE;
//			}
//			delete [] pText;
//		}
//		else {
//			*ao_pClLicense = (LONG) NULL;
//			return FALSE;
//		}
//		//-----------------------------------------------
//	
//		const ClLicense * pClLicense = pIXmlAnalizer->GetpClLicense();
//		if (pClLicense != NULL) {
//			*ao_pClLicense = (LONG) pClLicense;
//			return TRUE;
//		}
//	}
//	*ao_pClLicense = (LONG) NULL;
//	return FALSE;
//}

SHORT WINAPI M4MODKEYWRAP_GetClLicense( LONG ai_pIXmlAnalyzer, LPSTR ai_pBufferEncrypted, LPLONG ao_pClLicense )
{
	//Obtenemos la licencia.
	IXmlAnalyzer * pIXmlAnalizer = (IXmlAnalyzer *) ai_pIXmlAnalyzer;
	if (pIXmlAnalizer != NULL) {
		const ClLicense * pClLicense = pIXmlAnalizer->GetpClLicense( (m4pcchar_t) ai_pBufferEncrypted, strlen(ai_pBufferEncrypted) );
		if (pClLicense != NULL) {
			*ao_pClLicense = (LONG) pClLicense;
			return TRUE;
		}
	}
	*ao_pClLicense = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_IsSecurizedObjectNotAllowed( LONG ai_pClLicense, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObjectNotAllowed )
{
	//Dada la licencia, nos indica si un objeto es NOT ALLOWED.	
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {
		const ClSecurizedObject oCopy(NULL, (m4pcchar_t) ai_pClSecurizedObjectIdNodeStructure, (m4pcchar_t) ai_pClSecurizedObjectIdItem);
		if (pClLicense->IsSecurizedObjectNotAllowed(oCopy) == M4_TRUE) {
			*ao_bIsSecurizedObjectNotAllowed = TRUE;
			return TRUE;		
		}
		else {
			*ao_bIsSecurizedObjectNotAllowed = FALSE;
			return TRUE;		
		}
	}
	*ao_bIsSecurizedObjectNotAllowed = FALSE;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_IsSecurizedObjectAllowed( LONG ai_pClLicense, BSTR ai_pClSecurizedObjectIdNodeStructure, BSTR ai_pClSecurizedObjectIdItem, PSHORT ao_bIsSecurizedObjectAllowed )
{
	//Dada la licencia, nos indica si un objeto es ALLOWED.
	//En caso de estar permitido, este objeto acabará siendo desencriptado del repositorio.
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {
		const ClSecurizedObject oCopy(NULL, (m4pcchar_t) ai_pClSecurizedObjectIdNodeStructure, (m4pcchar_t) ai_pClSecurizedObjectIdItem);
		if (pClLicense->IsSecurizedObjectAllowed(oCopy) == M4_TRUE) {
			*ao_bIsSecurizedObjectAllowed = TRUE;
			return TRUE;		
		}
		else {
			*ao_bIsSecurizedObjectAllowed = FALSE;
			return TRUE;		
		}
	}
	*ao_bIsSecurizedObjectAllowed = FALSE;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjectsNotAllowed( LONG ai_pClLicense, LPLONG ao_pNumSecurizedObjectsNotAllowed )
{
	//Dada la licencia, obtenemos cuantos objetos NOT ALLOWED hay.
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {			
		*ao_pNumSecurizedObjectsNotAllowed = (LONG) (pClLicense->GetNumSecurizedObjectsNotAllowed());
		return TRUE;
	}
	*ao_pNumSecurizedObjectsNotAllowed = (LONG) 0;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectNotAllowedByIndex( LONG ai_pClLicense, LONG ai_lIndex, LPLONG ao_pClSecurizedObject )
{
	//Dada la licencia, obtenemos el iesimo objeto NOT ALLOWED.
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {	
		const ClSecurizedObject * pClSecurizedObject = pClLicense->GetSecurizedObjectNotAllowedByIndex((m4int32_t) ai_lIndex);
		if (pClSecurizedObject != NULL) {
			*ao_pClSecurizedObject = (LONG) pClSecurizedObject;
			return TRUE;
		}		
	}
	*ao_pClSecurizedObject = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetNumSecurizedObjectsAllowed( LONG ai_pClLicense, LPLONG ao_pNumSecurizedObjectsAllowed )
{
	//Dada la licencia, obtenemos cuantos objetos ALLOWED hay.
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {				
		*ao_pNumSecurizedObjectsAllowed = (LONG) (pClLicense->GetNumSecurizedObjectsAllowed());
		return TRUE;
	}
	*ao_pNumSecurizedObjectsAllowed = (LONG) NULL;
	return FALSE;
}

SHORT WINAPI M4MODKEYWRAP_GetSecurizedObjectAllowedByIndex( LONG ai_pClLicense, LONG ai_lIndex, LPLONG ao_pClSecurizedObject )
{
	//Dada la licencia, obtenemos el iesimo objeto NOT ALLOWED.
	ClLicense * pClLicense = (ClLicense *) ai_pClLicense;
	if (pClLicense != NULL) {	
		const ClSecurizedObject * pClSecurizedObject = pClLicense->GetSecurizedObjectAllowedByIndex((m4int32_t) ai_lIndex);
		if (pClSecurizedObject != NULL) {
			*ao_pClSecurizedObject = (LONG) pClSecurizedObject;
			return TRUE;
		}
	}
	*ao_pClSecurizedObject = (LONG) NULL;
	return FALSE;
}

//------------------------------------------------
// FIN Informacion de licencia.
//------------------------------------------------


