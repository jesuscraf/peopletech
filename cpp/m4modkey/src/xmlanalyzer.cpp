//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
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

#include "xmlanalyzer.hpp"
#include "encrypt.hpp"
#include "xmlcontainers.hpp"
#include "xmlutil.hpp"

#include "version.hpp"

#include "m4date.hpp"

//Para las claves de encriptacion.
#include "key.hpp"

#include "error.hpp"
#include "xmlstreamgenerator.hpp"
#include "m4unicode.hpp"


//----------------------------------------------
//Clase ClXmlSystemInitializer
//----------------------------------------------

//Creamos la variable statica para inicializar el sistema xml.
static ClXmlSystemInitializer g_oClXmlSystemInitializer;

//Se crea un objeto global para inicializar la librería de DOM.
//Estas dos operaciones solo deben hacerse una vez!!.
ClXmlSystemInitializer::ClXmlSystemInitializer( void )
{
	//Se inicializa el sistema Xml.
	XMLPlatformUtils::Initialize();
}

ClXmlSystemInitializer::~ClXmlSystemInitializer( )
{
	//Con esta llamada desmontamos el sistema Xml y no lo vamos a volver a utlizar.
	//-- XMLPlatformUtils::Terminate();
}

//----------------------------------------------
//FIN Clase ClXmlSystemInitializer
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlAnalyzer
//----------------------------------------------

ClXmlAnalyzer::ClXmlAnalyzer( void )
{
	m_ClXmlTagContainerRoot = NULL;	
}

ClXmlAnalyzer::~ClXmlAnalyzer( )
{
	if (m_ClXmlTagContainerRoot != NULL) {
		delete m_ClXmlTagContainerRoot;
		m_ClXmlTagContainerRoot = NULL;
	}
}

m4return_t ClXmlAnalyzer::AnalyzeXmlFile( const m4char_t * ai_pFileBuffer )
{
	//Si ya estaba inicializado daremos un error. Solo puede llamarse una vez para este objeto!.
	if ((ai_pFileBuffer == NULL) || (m_ClXmlTagContainerRoot != NULL)) {
		return M4_ERROR;
	}

	//Obtenemos en un objeto DOM la estructura del fichero XML donde viene la definición, segun la operación.
	
	//El sistema de Xml se habrá inicializado con:
	//XMLPlatformUtils::Initialize();

	//Instanciamos un parser.
	XercesDOMParser parser;
	parser.setValidationScheme(XercesDOMParser::Val_Never);

	//Creamos nuestro manejador de errores y lo instalamos.
    ModKeyErrorHandler errorHandler;
    parser.setErrorHandler(&errorHandler);

	parser.parse(MemBufInputSource( (const XMLByte *) ai_pFileBuffer, strlen(ai_pFileBuffer), "inputfile"));
	//parser.parse(ai_pFileName);
	if (errorHandler.HasErrors() == M4_TRUE) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_BAD_XML );		
		return M4_ERROR;
	}

	DOMElement *modkeyDOMRoot = parser.getDocument()->getDocumentElement();
	if (errorHandler.HasErrors() == M4_TRUE) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_BAD_XML );
		return M4_ERROR;
	}

	//Leeremos el xml de objectos y modulos.
	if (_ReadXmlFile( modkeyDOMRoot ) == M4_ERROR) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_BAD_XML );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClXmlAnalyzer::MergeXml( IXmlAnalyzer * ai_pIXmlAnalyzer, m4pchar_t & ao_pBufferMerged, m4int32_t & ao_iSizeMerged )
{
	return _MergeXml( ai_pIXmlAnalyzer, ao_pBufferMerged, ao_iSizeMerged );
}
//----------------------------------------------
//FIN Clase ClXmlAnalyzer
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlAnalyzerModule
//----------------------------------------------
ClXmlAnalyzerModule::ClXmlAnalyzerModule( void )
	:ClXmlAnalyzer()
{
	m_pVcSecurizedModule = new VcClSecurizedModule;	
}

ClXmlAnalyzerModule::~ClXmlAnalyzerModule( )
{	
	//Se eliminan tanto el vector como los modulos!. Si alguien ha hecho un Get
	//tiene que tener en cuenta que no puede liberar el analizador.
	if (m_pVcSecurizedModule != NULL) {
		for (itVcClSecurizedModule it = m_pVcSecurizedModule->begin(); it != m_pVcSecurizedModule->end(); it++) {
			delete (*it);
		}
		delete m_pVcSecurizedModule;
		m_pVcSecurizedModule = NULL;
	}
}

m4return_t ClXmlAnalyzerModule::_MergeXml( IXmlAnalyzer* ai_pIXmlAnalyzer, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged )
{
	//No dejamos que los modulos sepan hacer merge.
	return M4_ERROR;
}

m4return_t ClXmlAnalyzerModule::_ReadXmlFile( DOMElement* ai_pRoot )
{
	/* Ejemplo de Xml para objetos-modulo.
	<?xml version="1.0" encoding="UTF-8"?>
	<!--
	Copyright 1991-2006 by Meta4, Inc.
	Centro Europa Empresarial - Edf. Roma
	C/ Rozabella, 8
	28230 Las Rozas - Madrid
	Spain

	Private and Confidential
	The information contained in this document is the property of Meta4.
	It is for the exclusive use of designated employees
	and not for distribution without prior written authorization.
	-->

	<modkey fileversion="000.000.001">
		<objectsmodules>
			<objectmodule id="modulo1" desc="Sin permiso pero Desencriptado en cliente">
				<object ns="NsId1" item="ItmId1" />
				<object ns="NsId1" item="ItmId2" />
				<object ns="NsId2" item="ItmId1" />
			</objectmodule>
			<objectmodule id="modulo2" desc="Sin permiso pero Desencriptado en cliente">
				<object ns="NsId5" item="ItmId1" />
				<object ns="NsId5" item="ItmId2" />
				<object ns="NsId6" item="ItmId1" />
			</objectmodule>
			<objectmodule id="modulo3" desc="Con permiso y Desencriptado en cliente">
				<object ns="NsId1" item="ItmId1" />
				<object ns="NsId1" item="ItmId2" />
				<object ns="NsId1" item="ItmId3" />
			</objectmodule>
			<objectmodule id="modulo5" desc="Con permiso. Nuevo desarrollo. Desencriptado en origen M4">
				<object ns="NsId10" item="ItmId1" />
				<object ns="NsId10" item="ItmId2" />
				<object ns="NsId10" item="ItmId1" />
			</objectmodule>					
			<objectmodule id="modulo6" desc="Con permiso. Nuevo desarrollo. Encriptado en origen M4. Pero RAMDL desencriptara pq sí tiene permiso el cliente (ver licencia)">						
				<object ns="NsId20" item="ItmId1" />
				<object ns="NsId20" item="ItmId2" />
				<object ns="NsId20" item="ItmId1" />
			</objectmodule>
			<objectmodule id="modulo7" desc="Sin permiso. Nuevo desarrollo. Encriptado en origen M4">						
				<object ns="NsId21" item="ItmId1" />
				<object ns="NsId21" item="ItmId2" />
				<object ns="NsId21" item="ItmId1" />
			</objectmodule>
		</objectsmodules>
	</modkey>
	
	*/ //FIN Ejemplo de Xml para objetos-modulo.

	string sName("");

	//Vamos a recorrer los nodos xml.
	
	//Escenario:
	//1.- Obtenemos el nodo.
	//2.- Lo insertamos.
	//3.- Registramos sus atributos.
	//4.- Podremos consultar los atributos.

	//----------------------------	
	//Obtenemos el nodo raiz.
	if (ai_pRoot == NULL) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );		
		return M4_ERROR;
	}
	// UNICODE XML
	M4XMLToSTL((M4XMLCh*) ai_pRoot->getNodeName(), sName);
	if (sName.compare(XMLTAGLABEL_OBJMOD_ROOT) != 0) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
		return M4_ERROR;
	}

	m_ClXmlTagContainerRoot	= new ClXmlTagContainer(XMLTAGLABEL_OBJMOD_ROOT, NULL);

	if (_RegisterAttributes(m_ClXmlTagContainerRoot, ai_pRoot, XMLTAGLABEL_OBJMOD_ROOT_INDEX ) == M4_ERROR) {
		return M4_ERROR;
	}
	//----------------------------

	//-------------- OBTENIENDO VALOR DE ATRIBUTOS ROOT DE MODULO-OBJETOS  --------------
	const ClAttributeValue * pAttributeValue = NULL;

	pAttributeValue = m_ClXmlTagContainerRoot->GetAttributeValue( XMLTAGLABEL_OBJMOD_ATT_FILEVERSION, M4_FALSE );
	if (pAttributeValue == NULL) {
		//Debe haber valor.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING );		
		return M4_ERROR;
	}
	const string * pValueFileVersion = pAttributeValue->GetValue();

	//Vamos a validar la version del fichero de modulos objeto.
	if (pValueFileVersion != NULL) {
		if (ClVersion::ValidateFileVersionObjMod(*pValueFileVersion) == M4_FALSE) {
			//No es buena version.
			ClErrorManager::DumpError( M4MODKEY_ERROR_VALIDATE_VERSION );		
			return M4_ERROR;
		}
	}
	//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS ROOT DE MODULO-OBJETOS  --------------
	
	//----------------------------
	//Obtenemos un nodo con un determinado nombre.
	DOMNode* nModules;
	nModules = ModKeyDOMUtils::ModKeyFindChildNodeByName(ai_pRoot, XMLTAGLABEL_OBJMOD_OBJECTSMODULES.c_str());
	if (nModules == NULL) {
		//Tiene que existir la seccion.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
		return M4_ERROR;
	}
	ClXmlTagContainer * pClXmlTagContainerLevel1 = new ClXmlTagContainer(XMLTAGLABEL_OBJMOD_OBJECTSMODULES, m_ClXmlTagContainerRoot);

	if (_RegisterAttributes( pClXmlTagContainerLevel1, nModules, XMLTAGLABEL_OBJMOD_OBJECTSMODULES_INDEX ) == M4_ERROR) {
		return M4_ERROR;
	}
	//----------------------------

	DOMNodeList* childrenModules;
	DOMNode* nModule;	

	//Vamos a ir obteniendo los diferentes modulos.
    childrenModules = nModules->getChildNodes();
    for (m4uint32_t i=0; i<childrenModules->getLength(); i++)
    {
		ClXmlTagContainer * pClXmlTagContainerLevel2 = NULL;

        nModule = childrenModules->item(i);
        if (DOMNode::ELEMENT_NODE == nModule->getNodeType())
        {
			// UNICODE XML
			M4XMLToSTL((M4XMLCh*) nModule->getNodeName(), sName);

			//----------------------------
			//El nodo obligatoriamente debe ser el que toca!.
			if (sName.compare(XMLTAGLABEL_OBJMOD_OBJECTMODULE) != 0) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );				
				return M4_ERROR;
			}
			pClXmlTagContainerLevel2 = new ClXmlTagContainer(XMLTAGLABEL_OBJMOD_OBJECTMODULE, pClXmlTagContainerLevel1);

			if (_RegisterAttributes( pClXmlTagContainerLevel2, nModule, XMLTAGLABEL_OBJMOD_OBJECTMODULE_INDEX ) == M4_ERROR) {
				return M4_ERROR;
			}
			//----------------------------
			
			//------------------------------------------------------			
			//Aqui sabemos qué modulo es el especificado. Lo insertamos en el vector de modulos.
			pAttributeValue = pClXmlTagContainerLevel2->GetAttributeValue( XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_ID, M4_FALSE );
			if (pAttributeValue == NULL) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING );
				//Debe haber valor.
				return M4_ERROR;
			}
			const string * pValueId = pAttributeValue->GetValue();

			pAttributeValue = pClXmlTagContainerLevel2->GetAttributeValue( XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC, M4_FALSE );
			if (pAttributeValue == NULL) {
				//Debe haber valor.
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING );				
				return M4_ERROR;
			}
			const string * pValueDescription = pAttributeValue->GetValue();

			ClSecurizedModule * pSecurizedModule = new ClSecurizedModule(*pValueId, *pValueDescription);
			m_pVcSecurizedModule->push_back(pSecurizedModule);
			//------------------------------------------------------			

			//-----------------------------
			//El modulo puede tener o no hijos que serían los objetos.!!
			//-----------------------------

			//------------------------------------------------------			
			DOMNodeList* childrenModule;
			DOMNode* nObject;

			//Para cada modulo tenemos N objetos que se identifican por la estructura de nodo y el item!.
			childrenModule = nModule->getChildNodes();			
		    for (m4uint32_t i=0; i<childrenModule->getLength(); i++)
			{
				ClXmlTagContainer * pClXmlTagContainerLevel3 = NULL;

				nObject = childrenModule->item(i);
				if (DOMNode::ELEMENT_NODE == nObject->getNodeType())
				{
					// UNICODE XML
					M4XMLToSTL((M4XMLCh*) nObject->getNodeName(), sName);

					//----------------------------
					//El nodo obligatoriamente debe ser el que toca!.
					if (sName.compare(XMLTAGLABEL_OBJMOD_OBJECT) != 0){
						ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );						
						return M4_ERROR;
					}
					pClXmlTagContainerLevel3 = new ClXmlTagContainer(XMLTAGLABEL_OBJMOD_OBJECT, pClXmlTagContainerLevel2);

					if (_RegisterAttributes( pClXmlTagContainerLevel3, nObject, XMLTAGLABEL_OBJMOD_OBJECT_INDEX ) == M4_ERROR) {
						return M4_ERROR;
					}
					//----------------------------

					//-------------- OBTENIENDO VALOR DE ATRIBUTOS OBJETO --------------
					//En este punto tenemos los atributos para los objetos-modulo.

					pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_OBJMOD_OBJECT_ATT_NS, M4_FALSE );
					if (pAttributeValue == NULL) {
						//Debe haber valor.
						ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING );
						return M4_ERROR;
					}
					const string * pValueNs = pAttributeValue->GetValue();

					pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_OBJMOD_OBJECT_ATT_ITEM, M4_FALSE );
					if (pAttributeValue == NULL) {
						//Debe haber valor.
						ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING );						
						return M4_ERROR;
					}
					const string * pValueItem = pAttributeValue->GetValue();
					
					//Insertamos el objeto en el modulo!.
					ClSecurizedObject * pSecurizedObject = new ClSecurizedObject(pSecurizedModule, *pValueNs, *pValueItem);					
					pSecurizedModule->AddSecurizedObject(pSecurizedObject);
					//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS OBJETO --------------
				}
			}
			//------------------------------------------------------
        }
    }
	return M4_SUCCESS;
}

m4return_t ClXmlAnalyzerModule::_RegisterAttributes( ClXmlTagContainer* ai_pXmlTagContainerToAdd, DOMNode* ai_pNode, const m4int16_t ai_iLabelIndex )
{
	//El primer argumento es el contenedor donde vamos a añadir el atributo.

	m4return_t iRet = M4_ERROR;

	if ((ai_pNode == NULL) || (ai_pXmlTagContainerToAdd == NULL)) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );		
		return M4_ERROR;
	}

	//Obtenemos todos los atributos de un nodo, comprobando si son obligatorios o no.
	//Los añadimos en nuestro arbol.
	m4pchar_t pcBuffer = NULL;

	//En funcion del nodo tendremos una serie de atributos obligatorios y otros que no lo serán.
	switch (ai_iLabelIndex)
	{
	//---------------------------------------------------------------
	//INDICES RELACIONADOS CON OBJETOS-MODULO
	//---------------------------------------------------------------
	case XMLTAGLABEL_OBJMOD_ROOT_INDEX:

		//Tiene la version, que coincidirá con la version de generacion.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_OBJMOD_ATT_FILEVERSION.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );			
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_ATT_FILEVERSION, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_OBJMOD_OBJECTSMODULES_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_OBJMOD_OBJECTMODULE_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_ID.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );			
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_ID, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			//Con valor por defecto.
			iRet =ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC, XMLTAGLABEL_OBJMOD_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		
		break;

	case XMLTAGLABEL_OBJMOD_OBJECT_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_OBJMOD_OBJECT_ATT_NS.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_OBJECT_ATT_NS, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_OBJMOD_OBJECT_ATT_ITEM.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_OBJMOD_OBJECT_ATT_ITEM, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}
		
		break;

	//---------------------------------------------------------------
	//FIN INDICES RELACIONADOS CON OBJETOS-MODULO
	//---------------------------------------------------------------

	default:
		return iRet;
	}

	return M4_SUCCESS;
}

const ClLicense * ClXmlAnalyzerModule::GetpClLicense ( void ) const
{
	return NULL;
}

const ClLicense * ClXmlAnalyzerModule::GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted )
{
	return NULL;
}

m4int32_t ClXmlAnalyzerModule::GetNumSecurizedModules ( void ) const
{
	if (m_pVcSecurizedModule != NULL) {
		return m_pVcSecurizedModule->size();
	}
	return 0;
}

const ClSecurizedModule * ClXmlAnalyzerModule::GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const
{
	if ((ai_iIndex >= 0) && (ai_iIndex < GetNumSecurizedModules())) {
		return (*m_pVcSecurizedModule)[ai_iIndex];
	}
	return NULL;
}

void ClXmlAnalyzerModule::DumpInfo( FILE* ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}
	if (m_pVcSecurizedModule != NULL) {
		for (itVcClSecurizedModule it = m_pVcSecurizedModule->begin(); it != m_pVcSecurizedModule->end(); it++) {
			(*it)->DumpInfo(ai_pFile, ai_iLevel);
		}
	}
}

//----------------------------------------------
//FIN Clase ClXmlAnalyzerModule
//----------------------------------------------


//----------------------------------------------
//Clase ClXmlAnalyzer
//----------------------------------------------

ClXmlAnalyzerLicense::ClXmlAnalyzerLicense( void )
	:ClXmlAnalyzer()
{
	m_pClLicense = new ClLicense;
}

ClXmlAnalyzerLicense::~ClXmlAnalyzerLicense( )
{
	if (m_pClLicense != NULL) {
		delete m_pClLicense;
		m_pClLicense = NULL;
	}
}

m4return_t ClXmlAnalyzerLicense::_InitEncrypter( ClEncryption* ai_pClEncryption, m4pcchar_t ai_pKey )
{
	if (ai_pClEncryption == NULL) {
		return M4_ERROR;
	}

	//------------- IMPORTANTE --------------------------
	//Tenemos que seguir el protocolo.
	m4date_t dNow;
	ClActualDate (dNow);
	m4int32_t iSizeKeyA = sizeof(dNow) + strlen(ai_pKey);
	m4char_t * pKeyA = new m4char_t[iSizeKeyA + 1];
	if (pKeyA == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pKeyA, &dNow, sizeof(dNow));
	memcpy(pKeyA + sizeof(dNow), ai_pKey, strlen(ai_pKey));
	pKeyA[iSizeKeyA] = NULL;
	
	//----------------------------------------------------
	//Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------
	m4pchar_t pBufferEncryptedWithKeyA = pKeyA;
	m4int32_t iSizeEncryptedWithKeyA = iSizeKeyA;

	m4int32_t iLeft = 0;
	m4int32_t iRight = (iSizeEncryptedWithKeyA - 1);	
	while (iLeft <= iRight)
	{
		if ((iLeft % 2) == 0) {
			//Es caracter par.			
			m4char_t cTemp = *(pBufferEncryptedWithKeyA + iRight);
			*(pBufferEncryptedWithKeyA + iRight) = *(pBufferEncryptedWithKeyA + iLeft);
			*(pBufferEncryptedWithKeyA + iLeft) = cTemp;
		}
		iLeft++;
		iRight--;
	}

	//Restauramos para dar otra vuelta.
	pBufferEncryptedWithKeyA = pKeyA;

	for (m4int32_t i=0; i<iSizeEncryptedWithKeyA; i++)
	{		
		*pBufferEncryptedWithKeyA = ((*pBufferEncryptedWithKeyA) + M4MODKEY_ENCRYPT_KEYA);
		pBufferEncryptedWithKeyA++;
	}
	//----------------------------------------------------
	//FIN Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------

	//Emviamos la clave a utilizar!. Esperamos una fecha.
	m4char_t * pSignature = new m4char_t[sizeof(dNow) + 1];
	if (ai_pClEncryption->SendKeyB( pKeyA, iSizeKeyA, pSignature, sizeof(dNow) ) == M4_ERROR) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_SEND_KEY );
		return M4_ERROR;
	}

	//Verificamos que la fecha es correcta.
	if (memcmp(&dNow, pSignature, sizeof(dNow)) != 0) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_VERIFYING_KEY );
		return M4_ERROR;
	}

	//Si estamos aqui es pq el protocolo se ha validado. Desmontamos la paraeta.
	delete [] pKeyA;
	delete [] pSignature;
	pKeyA = NULL;
	pSignature = NULL;
	//------------- FIN IMPORTANTE --------------------------

	return M4_SUCCESS;
}

m4return_t ClXmlAnalyzerLicense::_MergeXml( IXmlAnalyzer* ai_pIXmlAnalyzer, m4pchar_t& ao_pBufferMerged, m4int32_t& ao_iSizeMerged )
{
	ao_iSizeMerged = 0;
	ao_pBufferMerged = NULL;

	string sMerged(M4MODKEY_EMPTY);
	sMerged.reserve(2048);	//Para evitar reallocs pq la licencia va a ser de unos 2k.

	// UNICODE XML
	sMerged.append(M4XMLHeader());
	sMerged.append(XMLTAGLABEL_FILE_COPYRIGHT);

	sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_ROOT); sMerged.append(" "); 
	sMerged.append(XMLTAGLABEL_LICENSE_ATT_FILEVERSION); sMerged.append("=");
	sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
	//Necesitamos escapar la cadena.
	string sScaped(ClVersion::GetFileVersionLicense());
	if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
		return M4_ERROR;
	}
	sMerged.append(sScaped);	
	sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");
	sMerged.append(">"); sMerged.append("\n");

	//Veamos cuantas licencias tenemos.
	if (m_pClLicense != NULL) {

		ClLicense * pLicense = m_pClLicense;

		//Cabecera de licencia.
		sMerged.append("\t");
		sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_LICENSE); sMerged.append(" ");

		sMerged.append(XMLTAGLABEL_LICENSE_LICENSE_ATT_LICENSEVERSION); sMerged.append("=");
		sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
		//Necesitamos escapar la cadena.
		sScaped.assign(pLicense->GetLicenseVersion());
		if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
			return M4_ERROR;
		}
		sMerged.append(sScaped);	
		sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

		sMerged.append(XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION); sMerged.append("=");
		sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(pLicense->GetExpiration()); sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

		sMerged.append(">"); sMerged.append("\n");

		//----- Datos de generacion. ------
		//La parte de generacion se calcula en tiempo de generacion!!!!. Siempre debe existir!.
		const ClGenerationInfo * pGenerationInfo = pLicense->GetGenerationInfo();
		if (pGenerationInfo != NULL) {
			sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_GENERATIONINFO); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_IDLICENSE);	sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); 
			//Necesitamos escapar la cadena.
			sScaped.assign(ClGenerationInfo::CalculateIdLicense());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONVERSION); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(ClVersion::GetFileVersionLicense());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);	
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_HOSTNAME); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(ClGenerationInfo::CalculateHostName());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONDATE); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(ClGenerationInfo::CalculateGenerationDate()); sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append("/>"); sMerged.append("\n");				
		}
		//----- FIN Datos de generacion. ------

		//----- Datos de cliente. ------
		const ClCustomerInfo * pCustomerInfo = pLicense->GetCustomerInfo();
		if (pCustomerInfo != NULL) {
			sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_ID); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(pCustomerInfo->GetIdCustomer());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_NAME); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(pCustomerInfo->GetCustomerName());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_COUNTRY); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(pCustomerInfo->GetCustomerCountry());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_GEODIV); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
			//Necesitamos escapar la cadena.
			sScaped.assign(pCustomerInfo->GetCustomerGeodiv());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped);
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC); sMerged.append("=");
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); 
			//Necesitamos escapar la cadena.
			sScaped.assign(pCustomerInfo->GetDescription());
			if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
				return M4_ERROR;
			}
			sMerged.append(sScaped); 			
			sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

			sMerged.append("/>"); sMerged.append("\n");
		}
		//----- FIN Datos de cliente. ------

		//----- Datos de Nps. ------			
		const ClNp * pNp = pLicense->GetNp();
		if (pNp != NULL) {
			sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_NPS); sMerged.append(">"); sMerged.append("\n");				
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_NP); sMerged.append(">"); sMerged.append("\n");
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SELECTS); sMerged.append(">"); sMerged.append("\n");
			
			m4int32_t nNp = pNp->GetNumNpItem();
			for (m4int32_t i=0; i<nNp; i++) {
				const ClNpItem * pNpItem = pNp->GetNpItemByIndex(i);

				if (pNpItem != NULL) {				
					sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
					sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SELECT); sMerged.append(" ");

					sMerged.append(XMLTAGLABEL_LICENSE_SELECT_ATT_DESC); sMerged.append("=");
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); 
					//Necesitamos escapar la cadena.
					sScaped.assign(pNpItem->GetDescription());
					if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
						return M4_ERROR;
					}
					sMerged.append(sScaped);
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

					sMerged.append(XMLTAGLABEL_LICENSE_SELECT_ATT_EXEC); sMerged.append("=");
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); 
					//Necesitamos escapar la cadena.
					sScaped.assign(pNpItem->GetSelect());
					if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
						return M4_ERROR;
					}
					sMerged.append(sScaped);
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

					sMerged.append(XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED); sMerged.append("=");
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(pNpItem->GetAuthorized()); sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

					sMerged.append(XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED); sMerged.append("=");
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(pNpItem->GetEffectiveAuthorized()); sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

					sMerged.append(XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT); sMerged.append("=");
					sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(pNpItem->GetLimit()); sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

					sMerged.append("/>"); sMerged.append("\n");
				}
			}

			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SELECTS); sMerged.append(">"); sMerged.append("\n");
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_NP); sMerged.append(">"); sMerged.append("\n");
			sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_NPS); sMerged.append(">"); sMerged.append("\n");
		}
		//----- FIN Datos de Nps. ------

		//----------------------------
		//Importante!!:
		//Ventas indica qué modulos el cliente SI tiene permiso. Pero hace falta los que son securizables
		//y que el cliente NO tiene permiso.
		//----------------------------

		sMerged.append("\t"); sMerged.append("\t");
		sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED); sMerged.append(">"); sMerged.append("\n");

		//----- Lista de modulos que NOT_ALLOWED. ------
		//Para saber los que no tiene permiso tengo que comprobar la lista de modulos securizables.
		if (ai_pIXmlAnalyzer != NULL) {
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED); sMerged.append(">"); sMerged.append("\n");

			m4int32_t nSecurizedModules = ai_pIXmlAnalyzer->GetNumSecurizedModules();
			m4int32_t i=0;
			for (i=0; i<nSecurizedModules; i++) {
				const ClSecurizedModule * pClSecurizedModule = ai_pIXmlAnalyzer->GetSecurizedModuleByIndex(i);
				if (pClSecurizedModule != NULL) {
					const string & sIdModule = pClSecurizedModule->GetIdModule();
					//Veamos si el modulo que está securizable, esta en la informacion de licencia en la lista de los que tiene permiso.

					if (pLicense->IsSecurizedModuleAllowed(*pClSecurizedModule) == M4_FALSE) {
						//Es un módulo segurizable pero el cliente no lo tiene, en su información de licencia, 
						//en la lista de permitidos. Por lo tanto el cliente NO tendrá permiso para el módulo.
						//Por lo tanto lo añado a la lista de NO permitidos.
						
						sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
						sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE); sMerged.append(" ");
						
						sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_ID); sMerged.append("=");							
						sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
						//Necesitamos escapar la cadena.
						sScaped.assign(pClSecurizedModule->GetIdModule());
						if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
							return M4_ERROR;
						}
						sMerged.append(sScaped);
						sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

						sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC); sMerged.append("=");							
						sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
						//Necesitamos escapar la cadena.
						sScaped.assign(pClSecurizedModule->GetDescription());
						if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
							return M4_ERROR;
						}
						sMerged.append(sScaped);
						sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");
						sMerged.append(">"); sMerged.append("\n");

						//Rellenamos la licencia con los objetos... (la informacion la tiene el xml de modulo-objeto).
						//------------ Lista de objetos de un modulo NOT ALLOWED ------------------
						m4int32_t nSecurizedObjects = pClSecurizedModule->GetNumSecurizedObjects();
						for (m4int32_t k=0; k<nSecurizedObjects; k++) {
							const ClSecurizedObject * pSecurizedObject = pClSecurizedModule->GetSecurizedObjectByIndex(k);
							if (pSecurizedObject != NULL) {

								sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");									
								sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT); sMerged.append(" ");

								sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS); sMerged.append("=");							
								sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
								//Necesitamos escapar la cadena.
								sScaped.assign(pSecurizedObject->GetIdNodeStructure());
								if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
									return M4_ERROR;
								}
								sMerged.append(sScaped);
								sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

								sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM); sMerged.append("=");							
								sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
								//Necesitamos escapar la cadena.
								sScaped.assign(pSecurizedObject->GetIdItem());
								if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
									return M4_ERROR;
								}
								sMerged.append(sScaped);
								sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

								sMerged.append("/>"); sMerged.append("\n");									
							}								
						}							
						//------------ FIN Lista de objetos de un modulo NOT ALLOWED ------------------

						sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
						sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE); sMerged.append(">"); ; sMerged.append("\n");
					}
				}
			}
			
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED); sMerged.append(">"); sMerged.append("\n");

			//----- FIN Lista de modulos que NOT_ALLOWED. ------

			//----- Lista de modulos que ALLOWED. ------
			//Han sido añadidos en comercial, es decir, ya venian en el fichero xml.
			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED); sMerged.append(">"); sMerged.append("\n");

			//---  Vamos a crear un mapa de objetos para no meter objetos repetidos!.---
			SClSecurizedObject oSetSecurizedObject;

			//Para seguir constuyendo la licencia es necesario conocer los allowed que hay en la informacion de licencia.
			m4int32_t nSecurizedModulesAllowed = pLicense->GetNumSecurizedModulesAllowed();				
			for (i=0; i<nSecurizedModulesAllowed; i++) {
				const ClSecurizedModule * pClSecurizedModule = pLicense->GetSecurizedModuleAllowedByIndex(i);
				if (pClSecurizedModule != NULL) {
					const string & sIdModule = pClSecurizedModule->GetIdModule();
					//En este punto, en la información de licencia se ha indicado un modulo para el que SI hay permiso,
					//pero vamos a comprobar si se trata efetivamente de un módulo segurizable.
					//Solo añadiré en la licencia si el modulo es segurizable.
					//La razon es pq es posible que lo hayan puesto como con permiso pero en realidad no sea segurizable,
					//por lo tanto no es necesario que se incluya en la licencia.

					m4bool_t bSecurizedAllowedModulePrinted = M4_FALSE;

					m4int32_t nSM = ai_pIXmlAnalyzer->GetNumSecurizedModules();
					for (m4int32_t j=0; j<nSM; j++) {
						const ClSecurizedModule * pSM = ai_pIXmlAnalyzer->GetSecurizedModuleByIndex(j);
						if (pSM != NULL) {
							const string & sIdModuleSM = pSM->GetIdModule();								

							if (pClSecurizedModule->IsEqualById(*pSM) == M4_TRUE) {							
								//Son el mismo modulo!.

								//Hay permiso y es segurizable, por lo tanto lo añadimos en la lista
								//de SI permitidos.

								//El cliente NO tiene permiso sobre el modulo securizable.

								//Solo pintamos una vez!.
								if (bSecurizedAllowedModulePrinted == M4_FALSE) {
									sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
									sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE); sMerged.append(" ");
									
									sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_ID); sMerged.append("=");							
									sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
									//Necesitamos escapar la cadena.
									sScaped.assign(pClSecurizedModule->GetIdModule());
									if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
										return M4_ERROR;
									}
									sMerged.append(sScaped);
									sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

									sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC); sMerged.append("=");							
									sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
									//Necesitamos escapar la cadena.
									sScaped.assign(pClSecurizedModule->GetDescription());
									if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
										return M4_ERROR;
									}
									sMerged.append(sScaped);
									sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");
									sMerged.append(">"); sMerged.append("\n");									
								}

								//Rellenamos la licencia con los objetos... (la informacion la tiene el xml de modulo-objeto).
								//------------ Lista de objetos de un modulo ALLOWED ------------------
								m4int32_t nSecurizedObjects = pSM->GetNumSecurizedObjects();
								for (m4int32_t k=0; k<nSecurizedObjects; k++) {
									const ClSecurizedObject * pSecurizedObject = pSM->GetSecurizedObjectByIndex(k);
									if (pSecurizedObject != NULL) {

										//Lo metemos en el set y comprobamos si ya existía.
										//Ojo con el operador de igualdad y menor de la clase del set!.											
										pair<itSClSecurizedObject, m4bool_t> pair = oSetSecurizedObject.insert(*pSecurizedObject);
										if (pair.second == M4_FALSE) {
											//No se ha insertado pq ya existía, por lo tanto no lo añadimos a la licencia.
											continue;
										}

										sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");									
										sMerged.append("<"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT); sMerged.append(" ");

										sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS); sMerged.append("=");							
										sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
										//Necesitamos escapar la cadena.
										sScaped.assign(pSecurizedObject->GetIdNodeStructure());
										if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
											return M4_ERROR;
										}
										sMerged.append(sScaped);
										sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

										sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM); sMerged.append("=");							
										sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE);
										//Necesitamos escapar la cadena.
										sScaped.assign(pSecurizedObject->GetIdItem());
										if (M4XMLStreamGenerator::EscapeXMLStream(sScaped) != M4_SUCCESS) {
											return M4_ERROR;
										}
										sMerged.append(sScaped);
										sMerged.append(XMLTAGLABEL_DOUBLE_QUOTE); sMerged.append(" ");

										sMerged.append("/>"); sMerged.append("\n");									
									}								
								}							
								//------------ FIN Lista de objetos de un modulo ALLOWED ------------------

								//No cerramos todavía pq puede haber otros modulos segurizables que cumplan la expresion regular!.
								//-- sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
								//-- sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE); sMerged.append(">"); ; sMerged.append("\n");

								//El módulo que venia en la informacion de licencia SI es segurizable y SI está
								//en la lista de permitidos.
								
								//Importante:
								//Puede ser que el módulo cumpla varias expresiones regulares, vamos a evitar
								//añadir todos los módulos que cumplan la expresion regular.

								bSecurizedAllowedModulePrinted = M4_TRUE;
							}
						}
					} // for (m4int32_t j=0; j<nSM; j++)

					//Si se pinto una vez, tenemos que cerrar.
					if (bSecurizedAllowedModulePrinted == M4_TRUE) {
						sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
						sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE); sMerged.append(">"); ; sMerged.append("\n");
					}
				}
			}
		
			//---  Borramos el set.---
			oSetSecurizedObject.clear();

			sMerged.append("\t"); sMerged.append("\t"); sMerged.append("\t");
			sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED); sMerged.append(">"); sMerged.append("\n");

			//----- FIN Lista de modulos que ALLOWED. ------

		} // if (ai_pXmlAnalyzer != NULL)
			
		//Cerramos securizacion.
		sMerged.append("\t"); sMerged.append("\t");
		sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_SECURIZED); sMerged.append(">"); sMerged.append("\n");

		//Cerramos licencia.
		sMerged.append("\t");
		sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_LICENSE); sMerged.append(">"); sMerged.append("\n");

	}	// if (m_pClLicense != NULL)

	//Cerramos fichero.
	sMerged.append("</"); sMerged.append(XMLTAGLABEL_LICENSE_ROOT); sMerged.append(">"); sMerged.append("\n");

	//Retornamos valores.
	ao_iSizeMerged = sMerged.size();
	if (ao_iSizeMerged <= 0) {
		ao_iSizeMerged = 0;
		return M4_ERROR;
	}

	ao_pBufferMerged = new m4char_t [ao_iSizeMerged + 1];
	if (ao_pBufferMerged == NULL) {
		ao_iSizeMerged = 0;
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(ao_pBufferMerged, sMerged.c_str(), ao_iSizeMerged);
	ao_pBufferMerged[ao_iSizeMerged] = NULL;

	return M4_SUCCESS;
}

m4return_t ClXmlAnalyzerLicense::_ReadXmlFile( DOMElement* ai_pRoot )
{
	/* Ejemplo de Xml para la licencia.

	<modkey fileversion="000.000.001">
		<license licenseversion="2" expiration="">
			<generationinfo idlicense="identificador unico" generationversion="000.000.001" hostname="" generationdate="" />
			<customerinfo id="IdMeta4" name="Meta4 S.A." country="España" geodiv="Madrid" desc="" />
			<nps>
				<np>
					<selects>
						<select desc="" exec="SELECT @ITEM=#COUNT(FIELD) FROM &amp;LOGIC_TABLE..." authorized="100"	effectiveauthorized="105" limit="110">
						</select>
						<select desc="" exec="SELECT @ITEM=#COUNT(FIELD) FROM &amp;LOGIC_TABLE..." authorized="1000" effectiveauthorized="1025" limit="1100">
						</select>
					</selects>
				</np>
			</nps>
			<securized>
				<securizednotallowed>
					<objectmodule id="modulo1" desc="Sin permiso pero Desencriptado en cliente" />
						<object ns="xxxxxxxx" item="xxxxxx" />
					<objectmodule id="modulo2" desc="Sin permiso pero Desencriptado en cliente" />
					<objectmodule id="*" desc="Esta linea nos da permiso absoluto" />					
				</securizednotallowed>
				<securizedallowed>
					<objectmodule id="modulo5" desc="Con permiso. Nuevo desarrollo. Encriptado en origen M4" />
					<objectmodule id="modulo6" desc="Con permiso. Nuevo desarrollo. Encriptado en origen M4" />
				</securizedallowed>
			</securized>
		</license>
	</modkey>

	*/ //FIN Ejemplo de Xml para la licencia.

	string sName("");

	//Vamos a recorrer los nodos xml.
	//----------------------------		
	//Obtenemos el nodo raiz.
	if (ai_pRoot == NULL) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );		
		return M4_ERROR;
	}
	// UNICODE XML
	M4XMLToSTL((M4XMLCh*) ai_pRoot->getNodeName(), sName);
	if (sName.compare(XMLTAGLABEL_LICENSE_ROOT) != 0) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
		return M4_ERROR;
	}

	m_ClXmlTagContainerRoot	= new ClXmlTagContainer(XMLTAGLABEL_LICENSE_ROOT, NULL);

	if (_RegisterAttributes(m_ClXmlTagContainerRoot, ai_pRoot, XMLTAGLABEL_LICENSE_ROOT_INDEX ) == M4_ERROR) {
		return M4_ERROR;
	}

	//-------------- OBTENIENDO VALOR DE ATRIBUTOS ROOT DE LICENCIA  --------------
	const ClAttributeValue * pAttributeValue = NULL;

	pAttributeValue = m_ClXmlTagContainerRoot->GetAttributeValue( XMLTAGLABEL_LICENSE_ATT_FILEVERSION, M4_FALSE );
	if (pAttributeValue == NULL) {
		//Debe haber valor.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
		return M4_ERROR;
	}
	const string * pValueFileVersion = pAttributeValue->GetValue();

	//Vamos a validar la version del fichero de modulos objeto.
	if (pValueFileVersion != NULL) {
		if (ClVersion::ValidateFileVersionLicense(*pValueFileVersion) == M4_FALSE) {
			//No es buena version.
			ClErrorManager::DumpError( M4MODKEY_ERROR_VALIDATE_VERSION );
			return M4_ERROR;
		}
	}
	//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS ROOT DE LICENCIA  --------------

	//----------------------------

	//----------------------------	
	//Obtenemos un nodo con un determinado nombre.
	DOMNode *nLicense = ModKeyDOMUtils::ModKeyFindChildNodeByName(ai_pRoot, XMLTAGLABEL_LICENSE_LICENSE.c_str());
	if (nLicense == NULL) {
		//Tiene que existir la seccion.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
		return M4_ERROR;
	}
	ClXmlTagContainer * pClXmlTagContainerLevel1 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_LICENSE, m_ClXmlTagContainerRoot);

	if (_RegisterAttributes( pClXmlTagContainerLevel1, nLicense, XMLTAGLABEL_LICENSE_LICENSE_INDEX ) == M4_ERROR) {
		return M4_ERROR;
	}
	//----------------------------

	//-------------- OBTENIENDO VALOR DE ATRIBUTOS LICENCIA  --------------

	ClGenerationInfo * pClGenerationInfo = NULL;
	ClCustomerInfo * pClCustomerInfo = NULL;	
	ClNp * pNp = NULL;

	//--- Propias de licencia.
	//--- FIN Propias de licencia.

	pAttributeValue = pClXmlTagContainerLevel1->GetAttributeValue( XMLTAGLABEL_LICENSE_LICENSE_ATT_LICENSEVERSION, M4_FALSE );
	if (pAttributeValue == NULL) {
		//Debe haber valor.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
		return M4_ERROR;
	}
	const string * pValueLicenseVersion = pAttributeValue->GetValue();

	if (pValueLicenseVersion != NULL) {
		m_pClLicense->_SetLicenseVersion(*pValueLicenseVersion);
	}

	pAttributeValue = pClXmlTagContainerLevel1->GetAttributeValue( XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION, M4_FALSE );
	if (pAttributeValue == NULL) {
		//Debe haber valor.
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
		return M4_ERROR;
	}
	const string * pValueExpiration = pAttributeValue->GetValue();

	if (pValueExpiration != NULL) {
		m_pClLicense->_SetExpiration(*pValueExpiration);
	}

	//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS LICENCIA  --------------

	//Vamos a ir obteniendo los diferentes nodos hijo que puede tener una licencia.	
	DOMNodeList *childrenLicense;
	DOMNode *nChildLicense;

	//Para los diferentes tipo de nodo hijo. Ademas esos nodos no pueden repetirse!.
	m4bool_t bExistsTagGenerationInfo = M4_FALSE;
	m4bool_t bExistsTagCustomerInfo = M4_FALSE;
	m4bool_t bExistsTagNps = M4_FALSE;
	m4bool_t bExistsTagSecurized = M4_FALSE;	

    childrenLicense = nLicense->getChildNodes();
    for (m4uint32_t i=0; i<childrenLicense->getLength(); i++)
    {
		ClXmlTagContainer * pClXmlTagContainerLevel1_1 = NULL;
		ClXmlTagContainer * pClXmlTagContainerLevel1_2 = NULL;
		ClXmlTagContainer * pClXmlTagContainerLevel1_3 = NULL;
		ClXmlTagContainer * pClXmlTagContainerLevel1_4 = NULL;

        nChildLicense = childrenLicense->item(i);
        if (DOMNode::ELEMENT_NODE == nChildLicense->getNodeType())
        {
			// UNICODE XML
			M4XMLToSTL((M4XMLCh*) nChildLicense->getNodeName(), sName);

			//---------- GENERATION INFO ------------------
			//Comprobamos si es el nodo de informacion de generacion.
			if (sName.compare(XMLTAGLABEL_LICENSE_GENERATIONINFO) == 0) {
				//Sí es el nodo que esperaba.

				//Solo permitimos un nodo de este tipo.
				if (bExistsTagGenerationInfo == M4_TRUE) {
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );					;
					return M4_ERROR;
				}
				bExistsTagGenerationInfo = M4_TRUE;

				//Se trata de un hijo asociado con la generacion de la licencia.
				pClXmlTagContainerLevel1_1 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_GENERATIONINFO, pClXmlTagContainerLevel1);

				if (_RegisterAttributes( pClXmlTagContainerLevel1_1, nChildLicense, XMLTAGLABEL_LICENSE_GENERATIONINFO_INDEX ) == M4_ERROR) {
					return M4_ERROR;
				}

				//-------------- OBTENIENDO VALOR DE ATRIBUTOS GENERACION  --------------

				//--- Propias de la informacion de generacion.

				pAttributeValue = pClXmlTagContainerLevel1_1->GetAttributeValue( XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_IDLICENSE, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueIdLicense = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_1->GetAttributeValue( XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONVERSION, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueGenerationVersion = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_1->GetAttributeValue( XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_HOSTNAME, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueHostName = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_1->GetAttributeValue( XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONDATE, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueGenerationDate = pAttributeValue->GetValue();

				if ((pValueIdLicense != NULL) && (pValueGenerationVersion != NULL)
					&& (pValueHostName != NULL) && ( pValueGenerationDate!= NULL))
				{
					pClGenerationInfo = new ClGenerationInfo(m_pClLicense, *pValueIdLicense, *pValueGenerationVersion, *pValueHostName, *pValueGenerationDate);
					m_pClLicense->_SetGenerationInfo( pClGenerationInfo );
				}

				//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS GENERACION  --------------

				continue;
			}
			//---------- FIN GENERATION INFO ------------------

			//---------- CUSTOMER INFO ------------------
			//Comprobamos si es el nodo de informacion de licencia.
			// UNICODE XML
			M4XMLToSTL((M4XMLCh*) nChildLicense->getNodeName(), sName);
			if (sName.compare(XMLTAGLABEL_LICENSE_CUSTOMERINFO) == 0){
				//Sí es el nodo que esperaba.

				//Solo permitimos un nodo de este tipo.
				if (bExistsTagCustomerInfo == M4_TRUE) {
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
					return M4_ERROR;
				}
				bExistsTagCustomerInfo = M4_TRUE;

				//Se trata de un hijo asociado con la informacion del cliente.
				pClXmlTagContainerLevel1_2 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_CUSTOMERINFO, pClXmlTagContainerLevel1);

				if (_RegisterAttributes( pClXmlTagContainerLevel1_2, nChildLicense, XMLTAGLABEL_LICENSE_CUSTOMERINFO_INDEX ) == M4_ERROR) {
					return M4_ERROR;
				}

				//-------------- OBTENIENDO VALOR DE ATRIBUTOS INFORMACION DEL CLIENTE  --------------

				//--- Propias de la informacion del cliente.
				pAttributeValue = pClXmlTagContainerLevel1_2->GetAttributeValue( XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_ID, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueCustomerId = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_2->GetAttributeValue( XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_NAME, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueCustomerName = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_2->GetAttributeValue( XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_COUNTRY, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueCustomerCountry = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_2->GetAttributeValue( XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_GEODIV, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueCustomerGeodiv = pAttributeValue->GetValue();

				pAttributeValue = pClXmlTagContainerLevel1_2->GetAttributeValue( XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC, M4_FALSE );
				if (pAttributeValue == NULL) {
					//Debe haber valor.
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
					return M4_ERROR;
				}
				const string * pValueCustomerDesc = pAttributeValue->GetValue();
				
				if ((pValueCustomerId != NULL) && (pValueCustomerName != NULL)
					&& (pValueCustomerCountry != NULL) && (pValueCustomerGeodiv != NULL)
					&& (pValueCustomerDesc != NULL)) {
					pClCustomerInfo = new ClCustomerInfo(*pValueCustomerId, *pValueCustomerName, *pValueCustomerCountry, *pValueCustomerGeodiv, *pValueCustomerDesc);
					m_pClLicense->_SetCustomerInfo( pClCustomerInfo );
				}

				//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS INFORMACION DEL CLIENTE  --------------

				continue;
			}
			//---------- FIN CUSTOMER INFO ------------------

			//---------- NPS ------------------
			//Cada Licencia puede tener una unica seccion Nps.
			//Comprobamos si es el nodo de Nps.
			// UNICODE XML
			M4XMLToSTL((M4XMLCh*) nChildLicense->getNodeName(), sName);
			if (sName.compare(XMLTAGLABEL_LICENSE_NPS) == 0) {
				//Sí es el nodo que esperaba.

				//Solo permitimos un nodo de este tipo.
				if (bExistsTagNps == M4_TRUE) {
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
					return M4_ERROR;
				}
				bExistsTagNps = M4_TRUE;

				DOMNode* nNps = nChildLicense;

				//Se trata de un hijo asociado con la informacion Nps.
				pClXmlTagContainerLevel1_3 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_NPS, pClXmlTagContainerLevel1);

				if (_RegisterAttributes( pClXmlTagContainerLevel1_3, nNps, XMLTAGLABEL_LICENSE_NPS_INDEX ) == M4_ERROR) {
					return M4_ERROR;
				}

				//---------- NP ------------------
				DOMNodeList* childrenNps;
				DOMNode* nChildNps;
				
				m4bool_t bExistsTagNp = M4_FALSE;

				childrenNps = nNps->getChildNodes();
				for (m4uint32_t i=0; i<childrenNps->getLength(); i++)
				{
					ClXmlTagContainer * pClXmlTagContainerLevel2 = NULL;

					nChildNps = childrenNps->item(i);
					if (DOMNode::ELEMENT_NODE == nChildNps->getNodeType())
					{
						// UNICODE XML
						M4XMLToSTL((M4XMLCh*) nChildNps->getNodeName(), sName);
						//----------------------------						
						//El nodo obligatoriamente debe ser el que toca!.
						if (sName.compare(XMLTAGLABEL_LICENSE_NP) != 0) {
							//Tiene que existir la seccion.
							ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
							return M4_ERROR;
						}

						//Solo permitimos un nodo de este tipo.
						if (bExistsTagNp == M4_TRUE) {
							ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
							return M4_ERROR;
						}
						bExistsTagNp = M4_TRUE;

						DOMNode* nNp = nChildNps;

						//Se trata de un hijo asociado con np.
						pClXmlTagContainerLevel2 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_NP, pClXmlTagContainerLevel1_3);

						if (_RegisterAttributes( pClXmlTagContainerLevel2, nNp, XMLTAGLABEL_LICENSE_NP_INDEX ) == M4_ERROR) {
							return M4_ERROR;
						}
						//----------------------------

						//-------------- OBTENIENDO VALOR DE ATRIBUTOS NP  --------------
						//Recordar que solo puede haber un nodo np!.
						pNp = new ClNp();
						m_pClLicense->_SetNp( pNp );
						//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS NP  --------------


						//---------- SELECTS ------------------
						DOMNodeList* childrenNp;
						DOMNode* nChildNp;

						ClXmlTagContainer * pClXmlTagContainerLevel3 = NULL;
						m4bool_t bExistsTagSelects = M4_FALSE;

						childrenNp = nNp->getChildNodes();
						for (m4uint32_t i=0; i<childrenNp->getLength(); i++)
						{
							nChildNp = childrenNp->item(i);
							if (DOMNode::ELEMENT_NODE == nChildNp->getNodeType())
							{
								// UNICODE XML
								M4XMLToSTL((M4XMLCh*) nChildNp->getNodeName(), sName);
								//----------------------------
								//El nodo obligatoriamente debe ser el que toca!.
								if (sName.compare(XMLTAGLABEL_LICENSE_SELECTS) != 0) {							
									//Tiene que existir la seccion.
									ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
									return M4_ERROR;
								}

								//Solo permitimos un nodo de este tipo.
								if (bExistsTagSelects == M4_TRUE) {
									ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
									return M4_ERROR;
								}
								bExistsTagSelects = M4_TRUE;

								DOMNode* nSelects = nChildNp;

								//Se trata de un hijo asociado con selects.
								pClXmlTagContainerLevel3 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SELECTS, pClXmlTagContainerLevel2);

								if (_RegisterAttributes( pClXmlTagContainerLevel3, nSelects, XMLTAGLABEL_LICENSE_SELECTS_INDEX ) == M4_ERROR) {
									return M4_ERROR;
								}
								//----------------------------

								//---------- SELECT ------------------
								//Ahora comprobamos cuantas select tenemos.
								DOMNodeList* childrenSelects;
								DOMNode* nChildSelects;

								ClXmlTagContainer * pClXmlTagContainerLevel4 = NULL;
								m4bool_t bExistsTagSelect = M4_FALSE;

								childrenSelects = nSelects->getChildNodes();
								for (m4uint32_t i=0; i<childrenSelects->getLength(); i++)
								{
									nChildSelects = childrenSelects->item(i);
									if (DOMNode::ELEMENT_NODE == nChildSelects->getNodeType())
									{
										// UNICODE XML
										M4XMLToSTL((M4XMLCh*) nChildSelects->getNodeName(), sName);
										//----------------------------
										//El nodo obligatoriamente debe ser el que toca!.
										if (sName.compare(XMLTAGLABEL_LICENSE_SELECT) != 0) {
											//Tiene que existir la seccion.
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
											return M4_ERROR;
										}
										bExistsTagSelect = M4_TRUE;

										DOMNode* nSelect = nChildSelects;

										//Se trata de un hijo asociado con selects.
										pClXmlTagContainerLevel4 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SELECT, pClXmlTagContainerLevel3);

										if (_RegisterAttributes( pClXmlTagContainerLevel4, nSelect, XMLTAGLABEL_LICENSE_SELECT_INDEX ) == M4_ERROR) {
											return M4_ERROR;
										}
										//----------------------------

										//-------------- OBTENIENDO VALOR DE ATRIBUTOS SELECT --------------

										//--- Propias de la select.
										const string * pValueDesc;
										const string sEmpty = M4MODKEY_EMPTY;

										pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SELECT_ATT_DESC, M4_FALSE );
										if (pAttributeValue == NULL) {
											//Optional.
											pValueDesc = &sEmpty;
										}
										else
										{
											pValueDesc = pAttributeValue->GetValue();
										}

										pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SELECT_ATT_EXEC, M4_FALSE );
										if (pAttributeValue == NULL) {
											//Debe haber valor.
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
											return M4_ERROR;
										}
										const string * pValueExec = pAttributeValue->GetValue();

										pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED, M4_FALSE );
										if (pAttributeValue == NULL) {
											//Debe haber valor.
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
											return M4_ERROR;
										}
										const string * pValueAuthorized = pAttributeValue->GetValue();

										pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED, M4_FALSE );
										if (pAttributeValue == NULL) {
											//Debe haber valor.
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
											return M4_ERROR;
										}
										const string * pValueEffectiveAuthorized = pAttributeValue->GetValue();

										pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT, M4_FALSE );
										if (pAttributeValue == NULL) {
											//Debe haber valor.
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
											return M4_ERROR;
										}
										const string * pValueLimit = pAttributeValue->GetValue();

										ClNpItem * pNpItem = NULL;
										if ((pValueDesc != NULL) 
											&& (pValueExec != NULL)
											&& (pValueAuthorized != NULL)
											&& (pValueEffectiveAuthorized != NULL)											
											&& (pValueLimit != NULL) ) {
											pNpItem = new ClNpItem(*pValueDesc, *pValueExec, *pValueAuthorized, *pValueEffectiveAuthorized, *pValueLimit);
										}
										else {
											ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
											return M4_ERROR;
										}
							
										//Ahora podemos añadirlo a la lista!.
										pNp->AddNpItem(pNpItem);
										
										//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS SELECT --------------										
									}
								}
								//---------- FIN SELECT ------------------
							}
						}
						//---------- FIN SELECTS ------------------

					}
				}
				//---------- FIN NP ------------------

				continue;
			}
			//---------- FIN NPS ------------------

			//---------- SECURIZED ------------------
			//Comprobamos si es el nodo de informacion de items securizados.
			// UNICODE XML
			M4XMLToSTL((M4XMLCh*) nChildLicense->getNodeName(), sName);
			if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED) == 0) {
				//Sí es el nodo que esperaba.

				//Solo permitimos un nodo de este tipo.
				if (bExistsTagSecurized == M4_TRUE) {
					ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
					return M4_ERROR;
				}
				bExistsTagSecurized = M4_TRUE;

				DOMNode* nSecurized = nChildLicense;

				//Se trata de un hijo asociado con la informacion del cliente.
				pClXmlTagContainerLevel1_4 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED, pClXmlTagContainerLevel1);

				if (_RegisterAttributes( pClXmlTagContainerLevel1_4, nSecurized, XMLTAGLABEL_LICENSE_SECURIZED_INDEX ) == M4_ERROR) {
					return M4_ERROR;
				}

				//SECURIZED puede tener varios tipos de hijos.

				DOMNodeList* childrenSecurized;				
				DOMNode* nChildSecurized;

				m4bool_t bExistsTagSecurizedNotAllowed = M4_FALSE;
				m4bool_t bExistsTagSecurizedAllowed = M4_FALSE;

				childrenSecurized = nSecurized->getChildNodes();
				for (m4uint32_t i=0; i<childrenSecurized->getLength(); i++)
				{
					ClXmlTagContainer * pClXmlTagContainerLevel2 = NULL;

					nChildSecurized = childrenSecurized->item(i);
					if (DOMNode::ELEMENT_NODE == nChildSecurized->getNodeType())
					{
						//No tiene pq existir una seccion de elementos antiguos securizados.
						//Continuamos con el siguiente hermano de SECURIZEDCONSTANT

						// UNICODE XML
						M4XMLToSTL((M4XMLCh*) nChildSecurized->getNodeName(), sName);

						//---------- SECURIZENOTALLOWED ------------------
						//Comprobamos si es el nodo de SecurizedNotAllowed.						
						if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED) == 0) 
						{
							//Solo permitimos un nodo de este tipo.
							if (bExistsTagSecurizedNotAllowed == M4_TRUE) {
								ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
								return M4_ERROR;
							}
							bExistsTagSecurizedNotAllowed = M4_TRUE;

							DOMNode* nSecurizedNotAllowed = nChildSecurized;

							//Se trata de un hijo asociado con np.
							pClXmlTagContainerLevel2 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED, pClXmlTagContainerLevel1_4);

							if (_RegisterAttributes( pClXmlTagContainerLevel2, nSecurizedNotAllowed, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_INDEX ) == M4_ERROR) {
								return M4_ERROR;
							}

							//-------------- OBTENIENDO VALOR DE ATRIBUTOS SECURIZEDNOTALLOWED  --------------
							//No hay
							//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS SECURIZEDNOTALLOWED  --------------

							//---------- MODULE ------------------
							DOMNodeList* childrenSecurizedNotAllowed;
							DOMNode* nChildSecurizedNotAllowed;

							ClXmlTagContainer * pClXmlTagContainerLevel3 = NULL;
							
							childrenSecurizedNotAllowed = nSecurizedNotAllowed->getChildNodes();
							for (m4uint32_t i=0; i<childrenSecurizedNotAllowed->getLength(); i++)
							{
								nChildSecurizedNotAllowed = childrenSecurizedNotAllowed->item(i);
								if (DOMNode::ELEMENT_NODE == nChildSecurizedNotAllowed->getNodeType())
								{
									// UNICODE XML
									M4XMLToSTL((M4XMLCh*) nChildSecurizedNotAllowed->getNodeName(), sName);

									//----------------------------
									//Comprobamos que el nodo es de modulo.
									if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE) != 0) {
										//Tiene que existir la seccion.
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
										return M4_ERROR;
									}

									DOMNode* nModule = nChildSecurizedNotAllowed;

									//Se trata de un hijo asociado con modulos.
									pClXmlTagContainerLevel3 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE, pClXmlTagContainerLevel2);

									if (_RegisterAttributes( pClXmlTagContainerLevel3, nModule, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_INDEX ) == M4_ERROR) {
										return M4_ERROR;
									}
									//----------------------------

									//-------------- OBTENIENDO VALOR DE ATRIBUTOS MODULO --------------
									//En este punto tenemos los atributos para los modulos Securizados viejos de los cuales el cliente NO tiene licencia.

									pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_ID, M4_FALSE );
									if (pAttributeValue == NULL) {
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
										//Debe haber valor.
										return M4_ERROR;
									}
									const string * pValueIdModule = pAttributeValue->GetValue();

									pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC, M4_FALSE );
									if (pAttributeValue == NULL) {
										//Debe haber valor.
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
										return M4_ERROR;
									}
									const string * pValueDescription = pAttributeValue->GetValue();

									//Insertamos el elemento securizado que no tenemos permiso!.
									ClSecurizedModule * pSecurizedModuleNotAllowed = new ClSecurizedModule(*pValueIdModule, *pValueDescription);
									m_pClLicense->_AddSecurizedModule( SECURIZED_NOT_ALLOWED, pSecurizedModuleNotAllowed );
									
									//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS MODULO --------------										


									//-------------- OBTENIENDO OBJETOS DEL MODULO NOT ALLOWED --------------									
									DOMNodeList* childrenModule;
									DOMNode* nObject;

									//Para cada modulo tenemos N objetos que se identifican por la estructura de nodo y el item!.
									childrenModule = nModule->getChildNodes();			
									for (m4uint32_t i=0; i<childrenModule->getLength(); i++)
									{
										ClXmlTagContainer * pClXmlTagContainerLevel4 = NULL;

										nObject = childrenModule->item(i);
										if (DOMNode::ELEMENT_NODE == nObject->getNodeType())
										{											
											// UNICODE XML
											M4XMLToSTL((M4XMLCh*) nObject->getNodeName(), sName);

											//----------------------------
											//El nodo obligatoriamente debe ser el que toca!.
											if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT) != 0) {
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
												return M4_ERROR;
											}
											pClXmlTagContainerLevel4 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT, pClXmlTagContainerLevel3);

											if (_RegisterAttributes( pClXmlTagContainerLevel4, nObject, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_INDEX ) == M4_ERROR) {
												return M4_ERROR;
											}
											//----------------------------

											//-------------- OBTENIENDO VALOR DE ATRIBUTOS OBJETO NOT ALLOWED --------------
											//En este punto tenemos los atributos para los objetos-modulo.

											pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS, M4_FALSE );
											if (pAttributeValue == NULL) {
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
												//Debe haber valor.
												return M4_ERROR;
											}
											const string * pValueNs = pAttributeValue->GetValue();

											pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM, M4_FALSE );
											if (pAttributeValue == NULL) {
												//Debe haber valor.
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
												return M4_ERROR;
											}
											const string * pValueItem = pAttributeValue->GetValue();
											
											//Insertamos el objeto en el modulo!.
											ClSecurizedObject * pSecurizedObject = new ClSecurizedObject(pSecurizedModuleNotAllowed, *pValueNs, *pValueItem);
											pSecurizedModuleNotAllowed->AddSecurizedObject(pSecurizedObject);
											//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS OBJETO NOT ALLOWED --------------
										}
									}
									//-------------- FIN OBTENIENDO OBJETOS DEL MODULO NOT ALLOWED --------------
								}
							}
							//---------- FIN MODULE ------------------

							continue;
						}
						//---------- FIN SECURIZEDNOTALLOWED ------------------

						//---------- SECURIZEDALLOWED ------------------
						//Comprobamos si es el nodo de SecurizedAllowed.
						// UNICODE XML
						M4XMLToSTL((M4XMLCh*) nChildSecurized->getNodeName(), sName);
						if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED) == 0) 
						{
							//Solo permitimos un nodo de este tipo.
							if (bExistsTagSecurizedAllowed == M4_TRUE) {
								ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_DUPLICATED );
								return M4_ERROR;
							}
							bExistsTagSecurizedAllowed = M4_TRUE;

							DOMNode* nSecurizedAllowed = nChildSecurized;

							//Se trata de un hijo asociado con np.
							pClXmlTagContainerLevel2 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED, pClXmlTagContainerLevel1_4);

							if (_RegisterAttributes( pClXmlTagContainerLevel2, nSecurizedAllowed, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_INDEX ) == M4_ERROR) {
								return M4_ERROR;
							}

							//-------------- OBTENIENDO VALOR DE ATRIBUTOS SECURIZEDALLOWED  --------------
							//Recordar que solo puede haber un nodo np!.
							//No hay
							//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS SECURIZEDALLOWED  --------------

							//---------- MODULE ------------------
							DOMNodeList* childrenSecurizedAllowed;
							DOMNode* nChildSecurizedAllowed;

							ClXmlTagContainer * pClXmlTagContainerLevel3 = NULL;
							
							childrenSecurizedAllowed = nSecurizedAllowed->getChildNodes();
							for (m4uint32_t i=0; i<childrenSecurizedAllowed->getLength(); i++)
							{
								nChildSecurizedAllowed = childrenSecurizedAllowed->item(i);
								if (DOMNode::ELEMENT_NODE == nChildSecurizedAllowed->getNodeType())
								{

									// UNICODE XML
									M4XMLToSTL((M4XMLCh*) nChildSecurizedAllowed->getNodeName(), sName);

									//----------------------------
									//Comprobamos que el nodo es de modulo.
									if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE) != 0) {
										//Tiene que existir la seccion.
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
										return M4_ERROR;
									}

									DOMNode* nModule = nChildSecurizedAllowed;

									//Se trata de un hijo asociado con modulos.
									pClXmlTagContainerLevel3 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE, pClXmlTagContainerLevel2);

									if (_RegisterAttributes( pClXmlTagContainerLevel3, nModule, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_INDEX ) == M4_ERROR) {
										return M4_ERROR;
									}
									//----------------------------

									//-------------- OBTENIENDO VALOR DE ATRIBUTOS MODULO --------------
									//En este punto tenemos los atributos para los modulos Securizados nuevos de los cuales el cliente SI tiene licencia.

									pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_ID, M4_FALSE );
									if (pAttributeValue == NULL) {
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
										//Debe haber valor.
										return M4_ERROR;
									}
									const string * pValueIdModule = pAttributeValue->GetValue();

									pAttributeValue = pClXmlTagContainerLevel3->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC, M4_FALSE );
									if (pAttributeValue == NULL) {
										//Debe haber valor.
										ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
										return M4_ERROR;
									}
									const string * pValueDescription = pAttributeValue->GetValue();

									//Insertamos el elemento securizado que no tenemos permiso!.
									ClSecurizedModule * pSecurizedModuleAllowed = new ClSecurizedModule(*pValueIdModule, *pValueDescription);
									m_pClLicense->_AddSecurizedModule( SECURIZED_ALLOWED, pSecurizedModuleAllowed );
									
									//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS MODULO --------------										


									//-------------- OBTENIENDO OBJETOS DEL MODULO ALLOWED --------------									
									DOMNodeList* childrenModule;
									DOMNode* nObject;

									//Para cada modulo tenemos N objetos que se identifican por la estructura de nodo y el item!.
									childrenModule = nModule->getChildNodes();			
									for (m4uint32_t i=0; i<childrenModule->getLength(); i++)
									{
										ClXmlTagContainer * pClXmlTagContainerLevel4 = NULL;

										nObject = childrenModule->item(i);
										if (DOMNode::ELEMENT_NODE == nObject->getNodeType())
										{
											// UNICODE XML
											M4XMLToSTL((M4XMLCh*) nObject->getNodeName(), sName);

											//----------------------------
											//El nodo obligatoriamente debe ser el que toca!.
											if (sName.compare(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT) != 0) {
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
												return M4_ERROR;
											}
											pClXmlTagContainerLevel4 = new ClXmlTagContainer(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT, pClXmlTagContainerLevel3);

											if (_RegisterAttributes( pClXmlTagContainerLevel4, nObject, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_INDEX ) == M4_ERROR) {
												return M4_ERROR;
											}
											//----------------------------

											//-------------- OBTENIENDO VALOR DE ATRIBUTOS OBJETO NOT ALLOWED --------------
											//En este punto tenemos los atributos para los objetos-modulo.

											pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS, M4_FALSE );
											if (pAttributeValue == NULL) {
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
												//Debe haber valor.
												return M4_ERROR;
											}
											const string * pValueNs = pAttributeValue->GetValue();

											pAttributeValue = pClXmlTagContainerLevel4->GetAttributeValue( XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM, M4_FALSE );
											if (pAttributeValue == NULL) {
												//Debe haber valor.
												ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
												return M4_ERROR;
											}
											const string * pValueItem = pAttributeValue->GetValue();
											
											//Insertamos el objeto en el modulo!.
											ClSecurizedObject * pSecurizedObject = new ClSecurizedObject(pSecurizedModuleAllowed, *pValueNs, *pValueItem);					
											pSecurizedModuleAllowed->AddSecurizedObject(pSecurizedObject);
											//-------------- FIN OBTENIENDO VALOR DE ATRIBUTOS OBJETO ALLOWED --------------
										}
									}
									//-------------- FIN OBTENIENDO OBJETOS DEL MODULO ALLOWED --------------
								}
							}
							//---------- FIN MODULE ------------------

							continue;
						}
						//---------- FIN SECURIZEDALLOWED ------------------

						//Si estamos aqui es porque el tag no es ningun hijo valido de SECURIZED.
						ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
						return M4_ERROR;
					}
				}

				//Hemos terminado con los hijos de securized
				
				continue;
			}
			//---------- FIN SECURIZED ------------------

			//Si estamos aqui es porque el tag no es ningun hijo valido de LICENSE.
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
			return M4_ERROR;
        }
    }

	//-------------------------------------------

	//Importante: Si estamos leyendo el fichero de info No existira la seccion de generacion, pero tenemos que crearla!.
	if (bExistsTagGenerationInfo == M4_FALSE) {
		ClGenerationInfo * pClGenerationInfo = new ClGenerationInfo(m_pClLicense);
		m_pClLicense->_SetGenerationInfo( pClGenerationInfo );
	}

	//Las secciones que son obligatorias
	if (bExistsTagCustomerInfo == M4_FALSE) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_TAG_MISSING );
			return M4_ERROR;
	}

	//-------------------------------------------
	//Validamos la informacion de la licencia.
	if (m_pClLicense->ValidateLicense() == M4_FALSE) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_VALIDATE_LICENSE );
		return M4_ERROR;
	}
	//-------------------------------------------

	return M4_SUCCESS;
}

m4return_t ClXmlAnalyzerLicense::_RegisterAttributes( ClXmlTagContainer* ai_pXmlTagContainerToAdd, DOMNode* ai_pNode, const m4int16_t ai_iLabelIndex )
{
	//El primer argumento es el contenedor donde vamos a añadir el atributo.
	m4return_t iRet = M4_ERROR;

	if ((ai_pNode == NULL) || (ai_pXmlTagContainerToAdd == NULL)) {
		ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
		return M4_ERROR;
	}

	//Obtenemos todos los atributos de un nodo, comprobando si son obligatorios o no.
	//Los añadimos en nuestro arbol.
	m4pchar_t pcBuffer = NULL;

	//En funcion del nodo tendremos una serie de atributos obligatorios y otros que no lo serán.
	switch (ai_iLabelIndex)
	{
	//---------------------------------------------------------------
	//INDICES RELACIONADOS CON LICENCIA
	//---------------------------------------------------------------
	case XMLTAGLABEL_LICENSE_ROOT_INDEX:

		//Tiene la version, que coincidirá con la version de generacion.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_ATT_FILEVERSION.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_ATT_FILEVERSION, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_LICENSE_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_LICENSE_ATT_LICENSEVERSION.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_LICENSE_ATT_LICENSEVERSION, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			//Con valor por defecto.
			iRet =ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION, XMLTAGLABEL_LICENSE_LICENSE_ATT_EXPIRATION_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_GENERATIONINFO_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_IDLICENSE.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_IDLICENSE, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONVERSION.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONVERSION, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_HOSTNAME.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_HOSTNAME, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONDATE.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_GENERATIONINFO_ATT_GENERATIONDATE, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_CUSTOMERINFO_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_ID.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_ID, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_NAME.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_NAME, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_COUNTRY.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_COUNTRY, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_GEODIV.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_GEODIV, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			//Con valor por defecto.
			iRet =ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC, XMLTAGLABEL_LICENSE_CUSTOMERINFO_ATT_DESC_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_NPS_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_NP_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_SELECTS_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_SELECT_INDEX:

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SELECT_ATT_DESC.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_DESC, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_DESC, ""));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SELECT_ATT_EXEC.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_EXEC, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED, XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_AUTHORIZED, XMLTAGLABEL_LICENSE_SELECT_ATT_EFFECTIVE_AUTHORIZED_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT, XMLTAGLABEL_LICENSE_SELECT_ATT_LIMIT_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_ID.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_ID, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			//Con valor por defecto.
			iRet =ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}
		
		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_NOT_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}
		
		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_INDEX:

		//No tiene atributos.
		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_ID.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_ID, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		//Opcional.
		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC.c_str());
		if (pcBuffer != NULL) {
			iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC, pcBuffer));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		else {
			//Con valor por defecto.
			iRet =ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_ATT_DESC_DEFAULT_VALUE));
			if (iRet == M4_ERROR) {
				ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
				if (pcBuffer != NULL) {
					delete [] pcBuffer;
					pcBuffer = NULL;
				}
				return M4_ERROR;
			}
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		break;

	case XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_INDEX:

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_NS, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}

		pcBuffer = NULL;
		pcBuffer = ModKeyDOMUtils::ModKeyGetAttVal(ai_pNode, XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM.c_str());
		if (pcBuffer == NULL) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_MANDATORY_ATTRIBUTE_MISSING_VALUE );
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		iRet = ai_pXmlTagContainerToAdd->AddAttributeValue(new ClAttributeValue(XMLTAGLABEL_LICENSE_SECURIZED_ALLOWED_OBJECTMODULE_OBJECT_ATT_ITEM, pcBuffer));
		if (iRet == M4_ERROR) {
			ClErrorManager::DumpError( XMLTAGLABEL_ERROR_INTERNAL_ADD_ATTRIBUTE );			
			if (pcBuffer != NULL) {
				delete [] pcBuffer;
				pcBuffer = NULL;
			}
			return M4_ERROR;
		}
		if (pcBuffer != NULL) {
			delete [] pcBuffer;
			pcBuffer = NULL;
		}
		
		break;

	//---------------------------------------------------------------
	//FIN INDICES RELACIONADOS CON LICENCIA
	//---------------------------------------------------------------

	default:
		return iRet;
	}

	return M4_SUCCESS;
}

const ClLicense * ClXmlAnalyzerLicense::GetpClLicense ( void ) const
{
	return m_pClLicense;
}

const ClLicense * ClXmlAnalyzerLicense::GetpClLicense ( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted )
{
	m4pchar_t pBuffer = NULL;	
	m4int_t iSize;

	//Desencripta el buffer, lo analiza y finalmente devuelve la licencia.
	if (ai_pBufferEncrypted == NULL) {
		return NULL;
	}

	//Creamos un encriptador temporal para poder desencriptar el buffer.
	ClEncryption oClEncryption;
	if (_InitEncrypter(&oClEncryption, M4MODKEY_ENCRYPT_KEY) == M4_ERROR) {
		return NULL;
	}

	//Si estamos aqui es pq ya tenemos encriptador válido.

	//Desencriptamos!.
	pBuffer = NULL;	
	iSize = 0;
	if (oClEncryption.Decrypt( ai_pBufferEncrypted, ai_iSizeEncrypted, pBuffer, iSize ) == M4_ERROR) {
		return NULL;
	}

	//Ya tenemos el buffer desencriptado, lo podemos analizar.
	if (AnalyzeXmlFile(pBuffer) == M4_ERROR) {
		if (pBuffer != NULL) {
			delete [] pBuffer;
		}
		return NULL;
	}

	//Ya no necesitamos el buffer desencriptado.
	if (pBuffer != NULL) {
		delete [] pBuffer;
		pBuffer = NULL;
	}

	//En este punto tenemos que tener una licencia.
	return GetpClLicense();
}

m4int32_t ClXmlAnalyzerLicense::GetNumSecurizedModules ( void ) const
{
	return 0;
}

const ClSecurizedModule * ClXmlAnalyzerLicense::GetSecurizedModuleByIndex ( m4int32_t ai_iIndex ) const
{
	//A la licencia no podemos preguntarle por indices de modulos pq hay varios
	//tipos, con o sin permiso.
	return NULL;
}

void ClXmlAnalyzerLicense::DumpInfo( FILE* ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}
	if (m_pClLicense != NULL) {
		m_pClLicense->DumpInfo(ai_pFile, ai_iLevel);
	}
}
//----------------------------------------------
//FIN Clase ClXmlAnalyzerLicense
//----------------------------------------------
