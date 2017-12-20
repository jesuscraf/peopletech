// XMLPersist.cpp: implementation of the CXMLPersist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4prop.h"
#include "cpropcoldef.h"
#include "dataproperties.h"
#include "xmlpersist.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLPersist::CXMLPersist()
{
	m_pDocument = NULL;
}

CXMLPersist::~CXMLPersist()
{
	RELEASE(m_pDocument);
}

HRESULT CXMLPersist::SetAttribute(MSXML::IXMLDOMNode *pNode, BSTR AttName, VARIANT vValue)
{
	MSXML::IXMLDOMElement* pEle = NULL;
    HRESULT hr;

    hr = pNode->QueryInterface(MSXML::IID_IXMLDOMElement, (void**)&pEle);
	if (hr == S_OK)
	{
		pEle->setAttribute(AttName, vValue);
		RELEASE(pEle);
	}
    return hr;
}

HRESULT CXMLPersist::GetNewChild(MSXML::IXMLDOMNode **ppNode)
{
	if (m_pDocument)
	{
		return m_pDocument->createNode(CComVariant((long) MSXML::NODE_ELEMENT), M4CComBSTR("Node"), NULL, ppNode);
	}
	return E_FAIL;
}

HRESULT CXMLPersist::SavePBDef2File(ICPropBagDef *pPBDef, char *sFileName)
{
	HRESULT hr = E_FAIL;

	if (pPBDef)
	{
		RELEASE(m_pDocument);

		if (CoCreateInstance(MSXML::CLSID_DOMDocument, NULL, CLSCTX_SERVER, MSXML::IID_IXMLDOMDocument, (void**)&m_pDocument) == S_OK)
		{
			MSXML::IXMLDOMNode * pRoot = NULL;
			if (GetNewChild(&pRoot) == S_OK)
			{
				ProcessRec(pPBDef, pRoot, NULL);
				m_pDocument->appendChild(pRoot, NULL);
			}
			m_pDocument->save(M4CComVariant(sFileName));
			RELEASE(m_pDocument);
			hr = S_OK;
		}
	}
	
	return hr;
}

HRESULT CXMLPersist::ProcessRec(ICPropBagDef *pPBDef, MSXML::IXMLDOMNode *pNode, CDataProperties * pData)
{

	BSTR bsClassName = NULL;
	BSTR bsAlias = NULL;

	pPBDef->get_ClassName(&bsClassName);
	pPBDef->get_Alias(&bsAlias);

	// Añadimos ClassName y Alias
	SetAttribute(pNode, M4CComBSTR("Classname"), CComVariant(bsClassName));
	SetAttribute(pNode, M4CComBSTR("Alias"), CComVariant(bsAlias));

	ICChildColDef * pChildCol = NULL;

	// Añadimos nuestras propiedades
	AddPropsToNode(pNode, pPBDef, pData);
	
	// Nos apuntamos nuestro acceso a datos
	CDataProperties * pMyData = NULL;
	CComVariant vData;
	HRESULT hr = pPBDef->Property(M4CComBSTR("DataProps"), &vData);
	if (hr == S_OK)
	{
		if (vData.vt == VT_DISPATCH)
		{
			IDataProperties * pIMyData = NULL;
			vData.pdispVal->QueryInterface(IID_IDataProperties, (void**)&pIMyData);
			pMyData = static_cast<CDataProperties*>(pIMyData);
			pIMyData->Release();
			pIMyData = NULL;
		}
	}


	// Iteramos por nuestros hijos
	long nCount = 0;
	pPBDef->get_ChildColDef(&pChildCol);
	
	pChildCol->get_Count(&nCount);
	for (long i = 0 ; i< nCount; i++)
	{
		ICPropBagDef * pChild = NULL;
		MSXML::IXMLDOMNode * pChildNode = NULL;
		
		pChildCol->get_Item(CComVariant(i+1), &pChild);
		if (GetNewChild(&pChildNode) == S_OK)
		{
			ProcessRec(pChild, pChildNode, pMyData);
			pNode->appendChild(pChildNode, NULL);
			pChildNode->Release();
		}
		pChild->Release();
	}
	
	pChildCol->Release();
	SysFreeString(bsClassName);
	SysFreeString(bsAlias);
	
	return S_OK;
}

HRESULT CXMLPersist::GetPBDef2String(ICPropBagDef *pPBDef, BSTR *bsOut)
{
	HRESULT hr = E_FAIL;

	if (pPBDef)
	{
		RELEASE(m_pDocument);

		if (CoCreateInstance(MSXML::CLSID_DOMDocument, NULL, CLSCTX_SERVER, MSXML::IID_IXMLDOMDocument, (void**)&m_pDocument) == S_OK)
		{
			MSXML::IXMLDOMNode * pRoot = NULL;
			if (GetNewChild(&pRoot) == S_OK)
			{
				ProcessRec(pPBDef, pRoot, NULL);
				m_pDocument->appendChild(pRoot, NULL);
				pRoot->Release();
			}
			m_pDocument->get_xml(bsOut);
			RELEASE(m_pDocument);
			hr = S_OK;
		}
	}
	
	return hr;
}


void CXMLPersist::AddPropsToNode(MSXML::IXMLDOMNode *pNode, ICPropBagDef *pPBDef, CDataProperties *pData)
{

	ICPropColDef * pPCD = NULL;
	CCPropColDef * pCPCD = NULL;
	DefProp * pDefProp = NULL;


	pPBDef->get_PropColDef(&pPCD);

	pCPCD = static_cast<CCPropColDef*>(pPCD);
	
	pDefProp = pCPCD->m_pNodeHead;

	while(pDefProp)
	{
		PersistDefPropInXML(pDefProp, pNode, pData);
		pDefProp = pDefProp->pNextSeq;
	};

	pPCD->Release();
}

void CXMLPersist::PersistDefPropInXML(DefProp * pDefProp, MSXML::IXMLDOMNode *pNode, CDataProperties *pData)
{
	CDataProperties * pChildData = NULL;
	IDataProperties * pIDP = NULL;	

	switch(pDefProp->Type)
	{
		case FE_DATAPROP:
			// Tenemos una propiedad de Datos
			pDefProp->Value.pdispValue->QueryInterface(IID_IDataProperties, (void**)&pIDP);
			pChildData = static_cast<CDataProperties*>(pIDP);
			PersistDataProp(pNode, pChildData, pData);
			pIDP->Release();
			break;
		case FE_NUMBER:
		case FE_CONSTRUCTIF:
		case FE_COLOR:
		case FE_NUMBER_NOTDEFINED:
			// Probamos la persistencia de una propiedad tipo numérico
			if (pDefProp->Value.lValue != pDefProp->DefValue.lValue)
			{
				BSTR bsName = ::GetBSTRFromId(pDefProp->nIdProperty);
				SetAttribute(pNode, bsName, CComVariant(pDefProp->Value.lValue));
				::SysFreeString(bsName);
			}
			break;
		default:
			// Nothing
			break;
	}
}

void CXMLPersist::PersistDataProp(MSXML::IXMLDOMNode *pNode, CDataProperties *pActualData, CDataProperties *pInheritData)
{
	char * pstrIdNode = NULL;
	char * pstrIdItem = NULL;
	char * pstrIdChannel = NULL;
	char * pstrIdChannelAlias = NULL;
	char * pstrIdChannelKey = NULL;
	char * pstrIdAccessAlias = NULL;
	long nGrants = 31;
	long nIdPresentation = -1;
	long nStartPos = -6;


	if ((pInheritData != NULL) && (pActualData != NULL))
	{

		// primero comprobamos si se está heredando o no
		if (pActualData->m_nmIdChannel == pInheritData->m_nmIdChannel)
		{
			// El canal se está heredando, no hacemos nada
			if (pActualData->m_nmIdNode != pInheritData->m_nmIdNode)
			{
				// Ha cambiado el Nodo
				pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
			}
			if (pActualData->m_nmIdItem != pInheritData->m_nmIdItem)
			{
				// Ha cambiado el Item
				pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
			}
		}
		else
		{
			pstrIdChannel = GetcharFromId(pActualData->m_nmIdChannel);
			pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
			pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
		}

		if (pActualData->m_nmIdChannelAlias != pInheritData->m_nmIdChannelAlias)
		{
			// Ha cambiado el ChannelAlias
			pstrIdChannelAlias = GetcharFromId(pActualData->m_nmIdChannelAlias);
		}
		if (pActualData->m_nmIdChannelKey != pInheritData->m_nmIdChannelKey)
		{
			// Ha cambiado el ChannelKey
			pstrIdChannelKey = GetcharFromId(pActualData->m_nmIdChannelKey);
		}
		if (pActualData->m_nmIdAccessAlias != pInheritData->m_nmIdAccessAlias)
		{
			// Ha cambiado el AccessAlias
			pstrIdAccessAlias = GetcharFromId(pActualData->m_nmIdAccessAlias);
		}
		if (pActualData->m_nGrants != pInheritData->m_nGrants)
		{
			// Ha cambiado el Grants
			nGrants = pActualData->m_nGrants;
		}
		if (pActualData->m_nIdPresentation != pInheritData->m_nIdPresentation)
		{
			// Ha cambiado el Grants
			nIdPresentation = pActualData->m_nIdPresentation;
		}
		if (pActualData->m_nStartPos != pInheritData->m_nStartPos)
		{
			// Ha cambiado el StartPos
			nStartPos = pActualData->m_nStartPos;
		}
	}

	// Si no estamos heredando, quizá es el primer elemento
	if ((pInheritData == NULL) && (pActualData))
	{
		pstrIdChannel = GetcharFromId(pActualData->m_nmIdChannel);
		pstrIdNode = GetcharFromId(pActualData->m_nmIdNode);
		pstrIdItem = GetcharFromId(pActualData->m_nmIdItem);
		pstrIdChannelAlias = GetcharFromId(pActualData->m_nmIdChannelAlias);
		pstrIdChannelKey = GetcharFromId(pActualData->m_nmIdChannelKey);
		pstrIdAccessAlias = GetcharFromId(pActualData->m_nmIdAccessAlias);
		nGrants = pActualData->m_nGrants;
		nIdPresentation = pActualData->m_nIdPresentation;
		nStartPos = pActualData->m_nStartPos;
	}
	
	// Hala, a persistir !
	if ((pstrIdChannel) && (strcmp(pstrIdChannel, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Idchannel"), M4CComVariant(pstrIdChannel));
	}

	if ((pstrIdNode) && (strcmp(pstrIdNode, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Idnode"), M4CComVariant(pstrIdNode));
	}

	if ((pstrIdItem) && (strcmp(pstrIdItem, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Iditem"), M4CComVariant(pstrIdItem));
	}

	if ((pstrIdChannelKey) && (strcmp(pstrIdChannelKey, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Idchannelkey"), M4CComVariant(pstrIdChannelKey));
	}

	if ((pstrIdChannelAlias) && (strcmp(pstrIdChannelAlias, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Idchannelalias"), M4CComVariant(pstrIdChannelAlias));
	}

	if ((pstrIdAccessAlias) && (strcmp(pstrIdAccessAlias, "") !=0))
	{
		SetAttribute(pNode, M4CComBSTR("Idaccessalias"), M4CComVariant(pstrIdAccessAlias));
	}

	if (nGrants != 31)
	{
		SetAttribute(pNode, M4CComBSTR("Grants"), CComVariant(nGrants));
	}

	if (nIdPresentation != -1)
	{
		SetAttribute(pNode, M4CComBSTR("Idpresentation"), CComVariant(nIdPresentation));
	}

	if (nStartPos != -6)
	{
		SetAttribute(pNode, M4CComBSTR("Startpos"), CComVariant(nStartPos));
	}
}
