// M4Prop.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f M4Propps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "m4prop.h"
#include "m4prop_i.c"
#include "definitions_i.c"
#include "cproperty.h"
#include "propcol.h"
#include "csingleprop.h"
#include "cpropcol.h"
#include "cchildcol.h"
#include "cpropbag.h"
#include "convertpropbag.h"
#include "containedobjects.h"
#include "eocollection.h"
#include "mdobject.h"
#include "mdcollection.h"
#include "propdata.h"
#include "cdefprop.h"
#include "cpropcoldef.h"
#include "cchildcoldef.h"
#include "cpropbagdef.h"
#include "dataproperties.h"
#include "traductprop.h"
#include "persistobject.h"
#include "dgenericobject.h"
#include "menuitem.h"
#include "menupoppup.h"
#include "oblparser.h"
#include "m4unicode.hpp"



//##. Begin Adri y Juanma.
//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
//	de propiedades y las sustituimos por un código
#include "namepropmap.h"

// la variable globar que hace de resolutor de claves
CNamePropMap g_NamePropMap;
// End


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_CProperty, CCProperty)
	OBJECT_ENTRY(CLSID_PropCol, CPropCol)
	OBJECT_ENTRY(CLSID_CSingleProp, CCSingleProp)
	OBJECT_ENTRY(CLSID_CPropCol, CCPropCol)
	OBJECT_ENTRY(CLSID_CPropBag, CCPropBag)
	OBJECT_ENTRY(CLSID_CChildCol, CCChildCol)
	OBJECT_ENTRY(CLSID_ConvertPropBag, CConvertPropBag)
	OBJECT_ENTRY(CLSID_ContainedObjects, CContainedObjects)
	OBJECT_ENTRY(CLSID_EOCollection, CEOCollection)
	OBJECT_ENTRY(CLSID_MDObject, CMDObject)
	OBJECT_ENTRY(CLSID_MDCollection, CMDCollection)
	OBJECT_ENTRY(CLSID_CDefProp, CCDefProp)
	OBJECT_ENTRY(CLSID_CPropColDef, CCPropColDef)
	OBJECT_ENTRY(CLSID_CPropBagDef, CCPropBagDef)
	OBJECT_ENTRY(CLSID_CChildColDef, CCChildColDef)
	OBJECT_ENTRY(CLSID_DataProperties, CDataProperties)
	OBJECT_ENTRY(CLSID_TraductProp, CTraductProp)
	OBJECT_ENTRY(CLSID_PersistObject, CPersistObject)
	OBJECT_ENTRY(CLSID_DGenericObject, CDGenericObject)
	OBJECT_ENTRY(CLSID_MenuItem, CMenuItem)
	OBJECT_ENTRY(CLSID_MenuPopPup, CMenuPopPup)
	OBJECT_ENTRY(CLSID_OBLParser, COBLParser)
END_OBJECT_MAP()

struct BlockProp
{
	DefProp * pBlock;
	BlockProp * pNext;
};

DefProp *			p = NULL;
DefProp **			pP = NULL;
DefProp *			gpFreeBlock = NULL;
BlockProp *			gpMainBlocks = NULL;
long				gnBlockNumber = 0;
_FE_PROP_ENTRY **	gpPropHash = NULL;
_FE_PROP_ENTRY *	gpPropMap = NULL;
long				gPropNumber = 0;
DataPropMap **		gDataPropMapHash = NULL;
long				gnIdPropMapFree = 0;
DataPropMap *		gPropMapFirst = NULL;


// Predefiniciones
long InsertNewPropInPropMap(char * cName, long nIndex);

DefProp * GetFreeBlock()
{
	if (gpFreeBlock == NULL)
	{
		// Creamos un nuevo bloque de propiedades
		long nSize = sizeof(DefProp)*BLOCK_DEFPROP_SIZE;
		DefProp * pTemp = (DefProp*) malloc(nSize);
		memset(pTemp, 0, nSize);
		// Los enlazamos
		for (long i = 0; i< BLOCK_DEFPROP_SIZE-1; i++)
		{
			pTemp[i].pNext = & pTemp[i+1];
		}
		gpFreeBlock = pTemp;
		BlockProp * pNewBlock = (BlockProp*) malloc(sizeof(BlockProp));
		memset(pNewBlock, 0, sizeof(BlockProp));
		if (gpMainBlocks)
		{
			BlockProp * pBP = gpMainBlocks;
			while(pBP->pNext) pBP = pBP->pNext;
			pBP->pNext = pNewBlock;
			pNewBlock->pBlock = pTemp;
		}
		else
		{
			gpMainBlocks = pNewBlock;
			gpMainBlocks->pBlock = pTemp;
		}
		
	}

	DefProp * pReturn = gpFreeBlock;
	gpFreeBlock = gpFreeBlock->pNext;
	gnBlockNumber++;
	return pReturn;
}

UINT HashKey(char * Key)
{
	UINT nHash = 0;
	while (*Key)
	{
		nHash = (nHash<<5) + nHash + *Key++;
	}
	return nHash;
}

const char * GetNameFromPropId(long nId)
{
	long nProp = 0;
	_FE_PROP_ENTRY * fpe = gpPropMap;

	while(fpe && (fpe->nIndex != nId))
	{
		fpe = fpe->pNextSeq;
	}
	if (fpe)
	{
		return fpe->pstrProperty;
	}
	return NULL;
}

long GetIdFromPropName(char * pstrName)
{
	UINT nHash = HashKey(pstrName) % HASH_SIZE;

	_FE_PROP_ENTRY * p;
	if (gpPropHash)
	{
		p = gpPropHash[nHash];
		while(p && strcmp(p->pstrProperty, pstrName) !=0)
		{
			p = p->pNextHash;
		}
	}
	if (p)
	{
		return p->nIndex;
	}
	else
	{
		// Lo añadimos a la tabla de Propiedades
		return InsertNewPropInPropMap(pstrName, -1);
	}
}


void FreeAllBlocks()
{
	// Ya deberíamos tener todos los bloques como Free
	BlockProp * pBP = gpMainBlocks;
	while(pBP)
	{
		BYTE* bytes = (BYTE*) pBP->pBlock;
		delete[] bytes;
		pBP = pBP->pNext;
		free(gpMainBlocks);
		gpMainBlocks = pBP; 
	}
	gpMainBlocks = NULL;
	gpFreeBlock = NULL;
	
	// Liberamos también la DataPropMap
	while(gPropMapFirst)
	{
		DataPropMap * p = gPropMapFirst;
		gPropMapFirst = gPropMapFirst->pNextSeq;
		free(p->strValue);
		p->strValue = NULL;
		free(p);
		p = NULL;
	}
	free(gDataPropMapHash);
	gnIdPropMapFree = 0;
	gDataPropMapHash = NULL;
}

void FreeBlock(DefProp * pBlock)
{
	switch (pBlock->Type)
	{
		case FE_STRING:
		case FE_IMAGE:
		case FE_ALIGNTYPE:	
		case FE_LN4_CODEARGS:
		case FE_STRING_NOTDEFINED:
			free(pBlock->Value.pstrValue);
			free(pBlock->DefValue.pstrValue);
			break;
		case FE_TRADUCTPROP:
		case FE_DATAPROP:
		case FE_FONT:	
			RELEASE((pBlock->Value.pdispValue));
			RELEASE((pBlock->DefValue.pdispValue));
			break;
		default:
			break;
	}
	free(pBlock->Key);
	memset(pBlock, 0, sizeof(DefProp));
	pBlock->pNext = gpFreeBlock;
	gpFreeBlock = pBlock;
	gnBlockNumber--; 
	if (gnBlockNumber == 0)
	{
		FreeAllBlocks();
	}
}

long InsertNewPropInPropMap(char * cName, long nIndex)
{
	UINT nHash = 0;
	_FE_PROP_ENTRY * fpe2 = NULL;
	_FE_PROP_ENTRY * fpe = NULL;
	
	// Creamos un nuevo puntero
	fpe = (_FE_PROP_ENTRY*)malloc(sizeof(_FE_PROP_ENTRY));
	memset(fpe, 0, sizeof(_FE_PROP_ENTRY));
	if (nIndex == -1)
	{
		// Se asigna uno nuevo
		nIndex = gPropNumber;
	}
	fpe->nIndex = nIndex;
	fpe->pstrProperty = (char*)malloc(strlen(cName)+1);
	strcpy(fpe->pstrProperty, cName);

	if (gpPropMap)
	{
		fpe2 = gpPropMap;
		while(fpe2->pNextSeq)
		{
			fpe2 = fpe2->pNextSeq;
		}
		fpe2->pNextSeq = fpe;
		fpe->pNextSeq = NULL;
	}
	else
	{
		gpPropMap = fpe;
	}

	nHash = HashKey((char*)fpe->pstrProperty)%HASH_SIZE;
	if (nHash >= 0)
	{
		if (gpPropHash[nHash] == NULL)
		{
			gpPropHash[nHash] = fpe;
			fpe->pNextHash = NULL;
		}
		else
		{
			_FE_PROP_ENTRY * pe = gpPropHash[nHash];
			while(pe->pNextHash)
			{
				pe = pe->pNextHash;
			}
			pe->pNextHash = fpe;
			fpe->pNextHash = NULL;
		}
	}
	return gPropNumber++;
}

void InitMapPropHash()
{
	gPropNumber = 0;
	
	free(gpPropHash);
	gpPropHash = (_FE_PROP_ENTRY**)malloc(HASH_SIZE*sizeof(_FE_PROP_ENTRY*));
	memset(gpPropHash, 0, HASH_SIZE*sizeof(_FE_PROP_ENTRY*));
	
	while(PropMap[gPropNumber].pstrProperty)
	{
		InsertNewPropInPropMap(PropMap[gPropNumber].pstrProperty, PropMap[gPropNumber].nIndex);
	}
}

void FreeMapPropHash()
{
	free(gpPropHash);
	gpPropHash = NULL;
	// a eliminar el mapa dinámico
	_FE_PROP_ENTRY * fpe = NULL;
	if (gpPropMap)
	{
		while(gpPropMap)
		{
			free(gpPropMap->pstrProperty);
			fpe = gpPropMap;
			gpPropMap = gpPropMap->pNextSeq;
			free(fpe);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	long j = 0;
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		InitMapPropHash();
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		FreeMapPropHash();
		ATLASSERT(gnBlockNumber == 0);
		_Module.Term();
	}
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}

IDataProperties * GetDataProperties(ICPropBagDef * pPropBagDef)
{

	// A partir de una PropertyBagDef obtenemos su propiedad de datos y
	// devolvemos su puntero con un incremento de referencia

	if (pPropBagDef)
	{
		CComVariant vReturn;
		HRESULT hr = pPropBagDef->Property(M4CComBSTR("DataProps"), &vReturn);
		if (hr == S_OK)
		{
			if (vReturn.vt == VT_DISPATCH)
			{
				IDataProperties * pData = NULL;
				hr = (vReturn.pdispVal)->QueryInterface(IID_IDataProperties, (void**)&pData);
				if (hr == S_OK)
				{
					return pData;
				}
			}
		}
	}
	return NULL;
}

void AddPBDef2PC(IPropCol * pPropCol, ICPropBagDef * pPropBag)
{
	if (pPropCol && pPropBag)
	{
		// Iteramos por todas las propiedades de la property Bag
		BSTR bstrClassName = NULL;
		BSTR bstrAlias = NULL;
		pPropBag->get_ClassName(&bstrClassName);
		pPropBag->get_Alias(&bstrAlias);
		pPropCol->AddProperty(M4CComBSTR("Classname"), CComVariant(bstrClassName), 0, FE_STRING, CComVariant(""));
		pPropCol->AddProperty(M4CComBSTR("Alias"), CComVariant(bstrAlias), -1, FE_STRING, CComVariant(""));
		
		RELEASE_BSTR(bstrClassName);
		RELEASE_BSTR(bstrAlias);
		
		ICPropColDef * pPropColDef = NULL;
		HRESULT hr = pPropBag->get_PropColDef(&pPropColDef);
		if (hr == S_OK)
		{
			// Marchando una de enumerador
			IEnumVARIANT	* pEnum = NULL;
			IUnknown		* pUnk = NULL;
			hr = pPropColDef->get__NewEnum(&pUnk);
			if (hr == S_OK)
			{
				hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				if (hr == S_OK)
				{
					ICDefProp * pDefProp = NULL;
					unsigned long num = 0;
					CComVariant vIn;
					pEnum->Reset();
					pEnum->Next(1,&vIn,&num);
					while ((vIn.vt==VT_DISPATCH)&&(num==1))
					{
						hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_ICDefProp, (void**)&pDefProp);
						if (hr == S_OK)
						{	
							AddDefProp2PC(pDefProp, pPropCol);
							RELEASE(pDefProp);
						}
						pEnum->Next(1,&vIn,&num);
					}
					RELEASE(pEnum);
				}
				RELEASE(pUnk);
			}
			RELEASE(pPropColDef);
		}
	}
}

void InformExec(IContainedObjects * pContObj, ICChildColDef * pChildColDef)
{
	IEnumVARIANT	* pEnum;
	IUnknown		* pUnk;
	HRESULT hr = E_INVALIDARG;
	IDesignObject	* pObject;

	if (pContObj && pChildColDef)
	{
		hr = pContObj->get__NewEnum(&pUnk);
		if (hr == S_OK)
		{
			hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			if (hr == S_OK)
			{
				unsigned long num = 0;
				CComVariant vIn;
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);
				while ((vIn.vt==VT_DISPATCH)&&(num==1))
				{
					hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_IDesignObject, (void**)&pObject);
					if (hr == S_OK)
					{
						short nExec = 0;
						BSTR bstrKey = NULL;
						pObject->get_Key(&bstrKey);
						pChildColDef->get_Exec(CComVariant(bstrKey), (long*)&nExec);
						pObject->put_ExecOrder(&nExec);
						RELEASE(pObject);
						RELEASE_BSTR(bstrKey);
					}
					pEnum->Next(1,&vIn,&num);
				}
				RELEASE(pEnum);
			}
			RELEASE(pUnk);
		}
	}
}


void AddDefProp2PC(ICDefProp * pDefProp, IPropCol * pPropCol)
{
	/*With tmpDefProp
        If .Type = FE_DATAPROP Then
            Set tmpData2 = .Value
            Set tmpData = tmpData2.Clone
            Set tmpData2 = Nothing
            pcDest.AddProperty .Name, tmpData, True, .Type, .defValue
        ElseIf .Type = FE_FONT Then
            Set tmpfont2 = .Value
            Set tmpFont = tmpfont2.Clone
            Set tmpfont2 = Nothing
            pcDest.AddProperty .Name, tmpFont, True, .Type, .defValue
        ElseIf .Type = FE_TRADUCTPROP Then
            Set tmptraduct2 = .Value
            Set tmptraduct = tmptraduct2.Clone
            Set tmptraduct2 = Nothing
            pcDest.AddProperty .Name, tmptraduct, True, .Type, .defValue
        Else
            pcDest.AddProperty .Name, .Value, True, .Type, .defValue
        End If
    End With*/

	if (pDefProp && pPropCol)
	{
		IDataProperties * pDataProp = NULL;
		IDataProperties * pDataProp2 = NULL;
		IM4FontHolder * pFontProp = NULL;
		IM4FontHolder * pFontProp2 = NULL;
		ITraductProp * pTrProp = NULL;
		ITraductProp * pTrProp2 = NULL;
		M4PropertyType Type = FE_NOTDEFINED;
		BSTR bstrName = NULL;
		CComVariant vValue, vDefValue, vCopyValue, vCopyDefValue;
		pDefProp->get_Type(&Type);
		pDefProp->get_Value(&vValue);
		pDefProp->get_DefValue(&vDefValue);
		pDefProp->get_Name(&bstrName);
		switch (Type)
		{
			case FE_DATAPROP:
				// Primero Tratamos con el Value
				((IDispatch*)(vValue.pdispVal))->QueryInterface(IID_IDataProperties, (void**)&pDataProp);
				if (pDataProp)
				{
					// Sacamos una copia
					pDataProp->Clone(&pDataProp2);
					if (pDataProp2)
					{
						IDispatch * pDisp;
						pDataProp2->QueryInterface(IID_IDispatch, (void**)&pDisp);
						if (pDisp)
						{
							vCopyValue = pDisp;
							RELEASE(pDisp);
						}
						RELEASE(pDataProp2);
					}
					RELEASE(pDataProp);
				}
				// Para el caso de las propiedades de datos, no tienen DefValue conocido.
				pPropCol->AddProperty(bstrName, vCopyValue, -1, Type, vDefValue);
				break;
			case FE_FONT:
				// Primero Tratamos con el Value
				((IDispatch*)(vValue.pdispVal))->QueryInterface(IID_IM4FontHolder, (void**)&pFontProp);
				if (pFontProp)
				{
					// Sacamos una copia
					pFontProp->Clone(&pFontProp2);
					if (pFontProp2)
					{
						IDispatch * pDisp;
						pFontProp2->QueryInterface(IID_IDispatch, (void**)&pDisp);
						if (pDisp)
						{
							vCopyValue = pDisp;
							RELEASE(pDisp);
						}
						RELEASE(pFontProp2);
					}
					RELEASE(pFontProp);
				}
				// A por el DefValue
				((IDispatch*)(vValue.pdispVal))->QueryInterface(IID_IM4FontHolder, (void**)&pFontProp);
				if (pFontProp)
				{
					// Sacamos una copia
					pFontProp->Clone(&pFontProp2);
					if (pFontProp2)
					{
						IDispatch * pDisp;
						pFontProp2->QueryInterface(IID_IDispatch, (void**)&pDisp);
						if (pDisp)
						{
							vCopyDefValue = pDisp;
							RELEASE(pDisp);
						}
						RELEASE(pFontProp2);
					}
					RELEASE(pFontProp);
				}
				pPropCol->AddProperty(bstrName, vCopyValue, -1, Type, vCopyDefValue);
				break;
			case FE_TRADUCTPROP:
				((IDispatch*)(vValue.pdispVal))->QueryInterface(IID_ITraductProp, (void**)&pTrProp);
				if (pTrProp)
				{
					// Sacamos una copia
					pTrProp->Clone(&pTrProp2);
					if (pTrProp2)
					{
						IDispatch * pDisp;
						pTrProp2->QueryInterface(IID_IDispatch, (void**)&pDisp);
						if (pDisp)
						{
							vCopyValue = pDisp;
							RELEASE(pDisp);
						}
						RELEASE(pTrProp2);
					}
					RELEASE(pTrProp);
				}
				pPropCol->AddProperty(bstrName, vCopyValue, -1, Type, vDefValue);
				break;
			default:
				pPropCol->AddProperty(bstrName, vValue, -1, Type, vDefValue);
				break;
		}
		RELEASE_BSTR(bstrName);
	}
	
}

void CopyPBD2PBDProp(ICPropBagDef * pbDest,ICPropBagDef * pbSource)
{

	CCPropBagDef * pCDest = static_cast<CCPropBagDef*>(pbDest);
	CCPropBagDef * pCSource = static_cast<CCPropBagDef*>(pbSource);
	
	if (pCDest && pCSource)
	{
		if (strcmp(pCDest->m_strClassName, pCSource->m_strClassName) == 0)
		{
			// Lo primero es un reset del destino
			pCDest->Reset();
			// Copiar el Alias
			pCDest->ChangeAlias(pCSource->m_strAlias);
			
			DefProp * pDefPropSource = pCSource->m_PropCol->m_pNodeHead;
			DefProp * pDefPropDest = NULL;
			while (pDefPropSource)
			{
				pDefPropDest = pCDest->m_PropCol->GetDefProp(pDefPropSource->Key);
				if (pDefPropDest == NULL)
				{
					// Le añadimos un elemento a la PBDest con los valores adecuados
					if (pDefPropSource->Type == FE_STRING_NOTDEFINED)
					{
						pCDest->AddProperty(M4CComBSTR(pDefPropSource->Key), M4CComVariant(pDefPropSource->Value.pstrValue), CComVariant(), FE_STRING_NOTDEFINED);
					}
					else if (pDefPropSource->Type == FE_NUMBER_NOTDEFINED)
					{
						pCDest->AddProperty(M4CComBSTR(pDefPropSource->Key), CComVariant(pDefPropSource->Value.lValue), CComVariant(), FE_NUMBER_NOTDEFINED);
					}
					else if (pDefPropSource->Type == FE_BOOLEAN_NOTDEFINED)
					{
						if (pDefPropSource->Value.boolValue)
						{
							pCDest->AddProperty(M4CComBSTR(pDefPropSource->Key), CComVariant((bool)-1), CComVariant(), FE_BOOLEAN_NOTDEFINED);
						}
						else
						{
							pCDest->AddProperty(M4CComBSTR(pDefPropSource->Key), CComVariant((bool)0), CComVariant(), FE_BOOLEAN_NOTDEFINED);
						}
					}
				}
				else
				{
					//Podemos hacer la copia
					CopyDefProperty(pDefPropDest, pDefPropSource);
				}
				pDefPropSource = pDefPropSource->pNextSeq;
			}
		}
	}
	
}

void CopyProp2PBD(ICPropBagDef * pbDest, ICDefProp * pDefProp)
{

	ICDefProp * pDestProp = NULL;
	
	if (pbDest)
	{
		BSTR bstrName = NULL;
		HRESULT hr = pDefProp->get_Name(&bstrName);
		if (hr == S_OK)
		{
			CComVariant vValue;
			M4PropertyType nType;
			ICPropColDef * pPropColDef = NULL;
			hr = pbDest->get_PropColDef(&pPropColDef);
			if (hr == S_OK)
			{
				// La tenemos en el Destino
				hr = pPropColDef->get_Item(CComVariant(bstrName), &pDestProp);
				if (hr == S_OK)
				{
					CopyProperty(pDestProp, pDefProp);
					RELEASE(pDestProp);
				}
				else
				{
					// No la tenemos en el destino
					CComVariant vDefValue;
					pDefProp->get_Value(&vValue);
					pDefProp->get_DefValue(&vDefValue);
					pDefProp->get_Type(&nType);
					pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
				}
				RELEASE(pPropColDef);
			}
		}
		RELEASE_BSTR(bstrName);
	}
}

void CopyDefProperty(DefProp * pDest, DefProp * pSource)
{

	IM4FontHolder * pFont = NULL;
	IDataProperties * pData = NULL;
	ITraductProp * pTraduct = NULL;
	
	if ((pDest->Type == pSource->Type) && (pDest->nIdProperty == pSource->nIdProperty))
	{
		switch(pSource->Type)
		{
			case FE_FONT:
				RELEASE((pDest->Value.pdispValue));
				pFont = static_cast<IM4FontHolder*>(pSource->Value.pdispValue);
				if (pFont)
					pFont->Clone((IM4FontHolder**)&pDest->Value.pdispValue);
				break;
			case FE_DATAPROP:
				// Sacamos copia
				RELEASE((pDest->Value.pdispValue));
				pData = static_cast<IDataProperties*>(pSource->Value.pdispValue);
				if (pData)
					pData->Clone((IDataProperties**)&pDest->Value.pdispValue);
				break;
			case FE_TRADUCTPROP:
				RELEASE((pDest->Value.pdispValue));
				pTraduct = static_cast<ITraductProp*>(pSource->Value.pdispValue);
				if (pTraduct)
					pTraduct->Clone((ITraductProp**)&pDest->Value.pdispValue);
				break;
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
				pDest->Value.lValue = pSource->Value.lValue;
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_LN4_CODEARGS:
				free(pDest->Value.pstrValue);
				pDest->Value.pstrValue = NULL;
				if (pSource->Value.pstrValue)
				{
					pDest->Value.pstrValue = (char*)malloc(strlen(pSource->Value.pstrValue)+1);
					strcpy(pDest->Value.pstrValue, pSource->Value.pstrValue);
				}
				break;
			case FE_BOOLEAN:
				pDest->Value.boolValue = pSource->Value.boolValue;
				break;
			default:
				break;
		}
	}			
}

void CopyProperty(ICDefProp * pDestProp, ICDefProp * pDefProp)
{


/*    ' Miramos en función del tipaje
    With pbDest.PropColDef.Item(tmpProp.Name)
        If .Type = FE_BOOLEAN Then
            .Value = CBool(tmpProp.Value)
        ElseIf .Type = FE_NUMBER Or .Type = FE_COLOR Then
            .Value = CLng(tmpProp.Value)
        Else
            .Value = tmpProp.Value
        End If
    End With
	
	*/

	// Se resetean las propiedades a sus valores por defecto
	CCDefProp * pTemp = static_cast<CCDefProp*>(pDestProp);
	DefProp * pDest = pTemp->m_pData;
	pTemp = static_cast<CCDefProp*>(pDefProp);
	DefProp * pSource = pTemp->m_pData;

	IM4FontHolder * pFont = NULL;
	IDataProperties * pData = NULL;
	ITraductProp * pTraduct = NULL;
	
	switch(pSource->Type)
	{

		case FE_FONT:
			pFont = static_cast<IM4FontHolder*>(pDest->Value.pdispValue);
			pFont->Release();
			pFont = static_cast<IM4FontHolder*>(pSource->Value.pdispValue);
			pFont->Clone((IM4FontHolder**)&pDest->Value.pdispValue);
			break;
		case FE_DATAPROP:
			// Sacamos copia
			pData = static_cast<IDataProperties*>(pDest->Value.pdispValue);
			pData->Release();
			pData = static_cast<IDataProperties*>(pSource->Value.pdispValue);
			pData->Clone((IDataProperties**)&pDest->Value.pdispValue);
			break;
		case FE_TRADUCTPROP:
			pTraduct = static_cast<ITraductProp*>(pDest->Value.pdispValue);
			pTraduct->Release();
			pTraduct = static_cast<ITraductProp*>(pSource->Value.pdispValue);
			pTraduct->Clone((ITraductProp**)&pDest->Value.pdispValue);
			break;
		case FE_NUMBER:
		case FE_COLOR:
		case FE_CONSTRUCTIF:
		case FE_GRANTSTYPE:
			pDest->Value.lValue = pSource->Value.lValue;
			break;
		case FE_STRING:
		case FE_DATE:
		case FE_IMAGE:
		case FE_ALIGNTYPE:
		case FE_LN4_CODEARGS:
			free(pDest->Value.pstrValue);
			pDest->Value.pstrValue = NULL;
			if (pSource->Value.pstrValue)
			{
				pDest->Value.pstrValue = (char*)malloc(strlen(pSource->Value.pstrValue)+1);
				strcpy(pDest->Value.pstrValue, pSource->Value.pstrValue);
			}
			break;
		case FE_BOOLEAN:
			pDest->Value.boolValue = pSource->Value.boolValue;
			break;
		default:
			break;
	}			

	/*M4PropertyType nType;
	HRESULT hr = S_OK;
	if (pDestProp && pDefProp)
	{
		CComVariant vDestValue, vSourceValue;
		pDestProp->get_Type(&nType);
		pDefProp->get_Value(&vSourceValue);
		switch (nType)
		{
			case FE_NUMBER:
			case FE_COLOR:
				// Se copia al VT_I4
				vDestValue.vt = VT_I4;
				vDestValue.lVal = 0;
				if (vSourceValue.vt == VT_UI1)
				{
					vDestValue.lVal = (long) vSourceValue.bVal;
				}
				else if (vSourceValue.vt == VT_I2)
				{
					vDestValue.lVal = (long) vSourceValue.iVal;
				}
				else if (vSourceValue.vt == VT_I4)
				{
					vDestValue.lVal = (long) vSourceValue.lVal;
				}
				break;
			case FE_STRING:
			case FE_DATE:
				vDestValue = L"";
				if (vSourceValue.vt == VT_BSTR)
				{
					vDestValue = vSourceValue.bstrVal;
				}
				break;
			case FE_BOOLEAN:
				vDestValue.vt = VT_BOOL;
				vDestValue.boolVal = 0;
				if (vSourceValue.vt == VT_BOOL)
				{
					vDestValue.boolVal = vSourceValue.boolVal;
				}
				else if (vSourceValue.vt == VT_BSTR)
				{
					if (_wcsicmp(vSourceValue.bstrVal, L"true") == 0)
					{
						vDestValue.boolVal = -1;
					}
				}
				else if (vSourceValue.vt == VT_UI1)
				{
					if (vSourceValue.iVal != 0)
					{
						vDestValue.boolVal = -1;
					}
				}
				else if (vSourceValue.vt == VT_I2)
				{
					if (vSourceValue.bVal != 0)
					{
						vDestValue.boolVal = -1;
					}
				}
				else if (vSourceValue.vt == VT_I4)
				{
					if (vSourceValue.lVal != 0)
					{
						vDestValue.boolVal = -1;
					}
				}

				break;
			case FE_TRADUCTPROP:
				if (vSourceValue.vt == VT_DISPATCH)
				{
					ITraductProp * pTraduct;
					IDispatch * pDisp = vSourceValue.pdispVal;
					hr = pDisp->QueryInterface(IID_ITraductProp, (void**)&pTraduct);
					if (hr == S_OK)
					{
						vDestValue = vSourceValue;
						RELEASE(pTraduct);
					}
				}
				break;
			case FE_FONT:
				if (vSourceValue.vt == VT_DISPATCH)
				{
					IM4FontHolder * pFont;
					IDispatch * pDisp = vSourceValue.pdispVal;
					hr = pDisp->QueryInterface(IID_IM4FontHolder, (void**)&pFont);
					if (hr == S_OK)
					{
						vDestValue = vSourceValue;
						RELEASE(pFont);
					}
				}
				break;
			case FE_DATAPROP:
				if (vSourceValue.vt == VT_DISPATCH)
				{
					IDataProperties * pData;
					IDispatch * pDisp = vSourceValue.pdispVal;
					hr = pDisp->QueryInterface(IID_IDataProperties, (void**)&pData);
					if (hr == S_OK)
					{
						vDestValue = vSourceValue;
						RELEASE(pData);
					}
				}
				break;
			default:
				vDestValue = vSourceValue;
				break;
		}
		pDestProp->put_Value(vDestValue);
	}*/

}


ICChildColDef * GetChildColDef(ICPropBagDef * pPropBagDef)
{
	ICChildColDef * pChild = NULL;
	if (pPropBagDef)
	{
		pPropBagDef->get_ChildColDef(&pChild);
	}
	return pChild;
}

HRESULT  GetPBDefFromPC(ICPropBagDef * pPBDest, IPropCol * pPropCol)
{
/*   Dim tmpDefProp As CDefProp
    Dim tmpSP As CProperty
    
    For Each tmpDefProp In pbdDest.PropColDef
        On Error Resume Next
        Set tmpSP = pcSource.Item(tmpDefProp.Name)
        tmpDefProp.Value = tmpSP.Value
        Set tmpSP = Nothing
        On Error GoTo 0
    Next tmpDefProp*/

	HRESULT hr = E_FAIL;

	if (pPBDest && pPropCol)
	{
		ICPropColDef * pPropColDef = NULL;
		hr = pPBDest->get_PropColDef(&pPropColDef);
		if (hr == S_OK)
		{
			// Marchando una de enumerador
			IEnumVARIANT	* pEnum = NULL;
			IUnknown		* pUnk = NULL;
			hr = pPropColDef->get__NewEnum(&pUnk);
			if (hr == S_OK)
			{
				hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				if (hr == S_OK)
				{
					ICDefProp * pDefProp = NULL;
					unsigned long num = 0;
					CComVariant vIn;
					pEnum->Reset();
					pEnum->Next(1,&vIn,&num);
					while ((vIn.vt==VT_DISPATCH)&&(num==1))
					{
						hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_ICDefProp, (void**)&pDefProp);
						if (hr == S_OK)
						{	
							// Tenemos la DefProp
							BSTR bstrName = NULL;
							ICProperty * pProperty = NULL;
							pDefProp->get_Name(&bstrName);
							hr = pPropCol->get_Item(CComVariant(bstrName), &pProperty);
							::SysFreeString(bstrName);
							if (hr == S_OK)
							{
								CComVariant vValue;
								pProperty->get_Value(&vValue);
								pDefProp->put_Value(vValue);
								RELEASE(pProperty);
							}

							RELEASE(pDefProp);
						}
						pEnum->Next(1,&vIn,&num);
					}
					RELEASE(pEnum);
				}
				RELEASE(pUnk);
			}
			RELEASE(pPropColDef);
		}
	}

	return hr;


}

HRESULT ApplyInheritance(IDesignObject * pDesignObject, IDataProperties * pNewData)
{
	HRESULT hr = E_INVALIDARG;
	IDataProperties * pNewData2;
	IDataProperties * pParentData;

	if (pDesignObject && pNewData)
	{
		hr = pNewData->Clone(&pNewData2);
		if (hr == S_OK)
		{
			hr = pDesignObject->get_DataProps(&pParentData);
			if (hr == S_OK)
			{
				// Pillamos los containedObjects
				IContainedObjects * pContained;
				hr = pDesignObject->get_ContainedObjects(&pContained);
				if (hr == S_OK)
				{
					// Marchando una de enumerador
					IEnumVARIANT	* pEnum = NULL;
					IUnknown		* pUnk = NULL;
					hr = pContained->get__NewEnum(&pUnk);
					if (hr == S_OK)
					{
						hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
						if (hr == S_OK)
						{
							
							unsigned long num = 0;
							CComVariant vIn;
							IDesignObject * pChildObject = NULL;
							pEnum->Reset();
							pEnum->Next(1,&vIn,&num);
							while ((vIn.vt==VT_DISPATCH)&&(num==1))
							{
								hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_IDesignObject, (void**)&pChildObject);
								if (hr == S_OK)
								{	
									ProcessInheritance(pChildObject, pParentData, pNewData);
									RELEASE(pChildObject);
								}
								pEnum->Next(1,&vIn,&num);
							}
							RELEASE(pEnum);
						}
						RELEASE(pUnk);
					}
					RELEASE(pContained);
				}
				CopyDataProps(pParentData, pNewData2);
				RELEASE(pParentData);
			}
			RELEASE(pNewData2);
		}
	}

	return hr;
}

void CopyDataProps(IDataProperties * pDest, IDataProperties * pSource)
{

	CDataProperties * pSourceData = static_cast<CDataProperties*> (pSource);
	CDataProperties * pDestData = static_cast<CDataProperties*> (pDest);

	if (pSourceData && pDest)
	{
		pDestData->m_nGrants = pSourceData->m_nGrants;
		pDestData->m_nStartPos = pSourceData->m_nStartPos;
		pDestData->m_nIdPresentation = pSourceData->m_nIdPresentation;
		pDestData->m_nmIdChannel = pSourceData->m_nmIdChannel;
		pDestData->m_nmIdChannelKey = pSourceData->m_nmIdChannelKey;
		pDestData->m_nmIdChannelAlias = pSourceData->m_nmIdChannelAlias;
		pDestData->m_nmIdNode = pSourceData->m_nmIdNode;
		pDestData->m_nmIdItem = pSourceData->m_nmIdItem;
		pDestData->m_nmIdAccessAlias = pSourceData->m_nmIdAccessAlias;
	}

}

HRESULT GetDefValueFromDefProp(DefProp * pDefProp, VARIANT * pValue)
{
	USES_CONVERSION;
	if (pDefProp && pValue)
	{
		::VariantClear(pValue);
		switch (pDefProp->Type)
		{
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
				pValue->vt = VT_I4;
				pValue->lVal = pDefProp->DefValue.lValue;
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_LN4_CODEARGS:
				pValue->vt = VT_BSTR;
				if (pDefProp->DefValue.pstrValue)
				{
					pValue->bstrVal = M4CppToVB(pDefProp->DefValue.pstrValue);
				}
				else
				{
					pValue->bstrVal = ::SysAllocString(L"");
				}

				break;
			case FE_TRADUCTPROP:
			case FE_FONT:
			case FE_DATAPROP:
				if (pDefProp->DefValue.pdispValue)
				{
					pValue->vt = VT_DISPATCH;
					pValue->pdispVal = pDefProp->DefValue.pdispValue;
					(pValue->pdispVal)->AddRef();
				}
				else
				{
					pValue->vt = VT_EMPTY;
					pValue->pdispVal = NULL;
				}
				break;
			case FE_BOOLEAN:
				pValue->vt = VT_BOOL;
				if (pDefProp->DefValue.boolValue)
					pValue->boolVal = -1;
				else
					pValue->boolVal = 0;
				break;
		}
		
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT PutDefValueInDefProp(DefProp * pDefProp, VARIANT * pValue)
{
	USES_CONVERSION;
	if (pDefProp && pValue)
	{
		switch (pDefProp->Type)
		{
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
				if (pValue->vt == VT_I4)
				{
					pDefProp->DefValue.lValue = pValue->lVal;
				}
				else if (pValue->vt = VT_I2)
				{
					pDefProp->DefValue.lValue = pValue->iVal;
				}
				else if (pValue->vt = VT_UI1)
				{
					pDefProp->DefValue.lValue = pValue->bVal;
				}
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_LN4_CODEARGS:
				if (pValue->vt == VT_BSTR)
				{
					long nLength = ::SysStringLen(pValue->bstrVal);
					free(pDefProp->DefValue.pstrValue);
					if (nLength)
					{
						pDefProp->DefValue.pstrValue = M4VBToCpp(pValue->bstrVal);
					}
					else
					{
						pDefProp->DefValue.pstrValue = NULL;
					}
				}
				break;
			case FE_TRADUCTPROP:
			case FE_FONT:
			case FE_DATAPROP:
				if (pValue->vt == VT_DISPATCH)
				{
					RELEASE((pDefProp->DefValue.pdispValue));
					pDefProp->DefValue.pdispValue = pValue->pdispVal;
					if (pDefProp->DefValue.pdispValue)
					{
						(pDefProp->DefValue.pdispValue)->AddRef();
					}
				}			
				break;
			case FE_BOOLEAN:
				if (pValue->vt = VT_BOOL)
				{
					if (pValue->boolVal)
						pDefProp->DefValue.boolValue = -1;
					else
						pDefProp->DefValue.boolValue = 0;
				}
				break;
		}
		
		return S_OK;
	}
	return E_INVALIDARG;
}


HRESULT GetValueFromDefProp(DefProp * pDefProp, VARIANT * pValue)
{
	USES_CONVERSION;
	if (pDefProp && pValue)
	{
		::VariantClear(pValue);
		switch (pDefProp->Type)
		{
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
			case FE_NUMBER_NOTDEFINED:
				pValue->vt = VT_I4;
				pValue->lVal = pDefProp->Value.lValue;
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_STRING_NOTDEFINED:
			case FE_LN4_CODEARGS:
				pValue->vt = VT_BSTR;
				if (pDefProp->Value.pstrValue)
				{
					pValue->bstrVal = M4CppToVB(pDefProp->Value.pstrValue);
				}
				else
				{
					pValue->bstrVal = ::SysAllocString(L"");
				}
				break;
			case FE_TRADUCTPROP:
			case FE_FONT:
			case FE_DATAPROP:
				if (pDefProp->Value.pdispValue)
				{
					pValue->vt = VT_DISPATCH;
					pValue->pdispVal = pDefProp->Value.pdispValue;
					(pValue->pdispVal)->AddRef();
				}
				else
				{
					pValue->vt = VT_EMPTY;
					pValue->pdispVal = NULL;
				}
				break;
			case FE_BOOLEAN:
			case FE_BOOLEAN_NOTDEFINED:
				pValue->vt = VT_BOOL;
				if (pDefProp->Value.boolValue)
					pValue->boolVal = -1;
				else
					pValue->boolVal = 0;
				break;
		}
		
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT PutValueInDefProp(DefProp * pDefProp, VARIANT * pValue)
{
	USES_CONVERSION;
	if (pDefProp && pValue)
	{
		//::VariantClear(pValue);
		switch (pDefProp->Type)
		{
			case FE_NUMBER:
			case FE_COLOR:
			case FE_CONSTRUCTIF:
			case FE_GRANTSTYPE:
				if (pValue->vt == VT_I4)
				{
					pDefProp->Value.lValue = pValue->lVal;
				}
				else if (pValue->vt = VT_I2)
				{
					pDefProp->Value.lValue = pValue->iVal;
				}
				else if (pValue->vt = VT_UI1)
				{
					pDefProp->Value.lValue = pValue->bVal;
				}
				else
					return E_INVALIDARG;
				break;
			case FE_STRING:
			case FE_DATE:
			case FE_IMAGE:
			case FE_ALIGNTYPE:
			case FE_LN4_CODEARGS:
				if (pValue->vt == VT_BSTR)
				{
					long nLength = ::SysStringLen(pValue->bstrVal);
					free(pDefProp->Value.pstrValue);
					if (nLength)
					{
						pDefProp->Value.pstrValue = M4VBToCpp(pValue->bstrVal);
					}
					else
					{
						pDefProp->Value.pstrValue = NULL;
					}
				}
				else
					return E_INVALIDARG;
				break;
			case FE_TRADUCTPROP:
			case FE_FONT:
			case FE_DATAPROP:
				if (pValue->vt == VT_DISPATCH)
				{
					RELEASE((pDefProp->Value.pdispValue));
					pDefProp->Value.pdispValue = pValue->pdispVal;
					if (pDefProp->Value.pdispValue)
					{
						(pDefProp->Value.pdispValue)->AddRef();
					}
				}					
				else
					return E_INVALIDARG;
				break;
			case FE_BOOLEAN:
				if (pValue->vt = VT_BOOL)
				{
					if (pValue->boolVal)
						pDefProp->Value.boolValue = -1;
					else
						pDefProp->Value.boolValue = 0;
				}
				else
					return E_INVALIDARG;
				break;
			default:
				if (pValue->vt == VT_BSTR)
				{
					long nLength = ::SysStringLen(pValue->bstrVal);
					free(pDefProp->Value.pstrValue);
					if (nLength)
					{
						pDefProp->Value.pstrValue = M4VBToCpp(pValue->bstrVal);
					}
					else
					{
						pDefProp->Value.pstrValue = NULL;
					}
					// Lo damos de alta también como Key
					pDefProp->Type = FE_STRING_NOTDEFINED;
				}
				else if (pValue->vt == VT_BOOL)
				{
					pDefProp->Value.boolValue = pValue->boolVal;
					pDefProp->Type = FE_BOOLEAN_NOTDEFINED;
				}
				else if (pValue->vt == VT_I4)
				{
					pDefProp->Value.lValue = pValue->lVal;
					pDefProp->Type = FE_NUMBER_NOTDEFINED;
				}
				else if (pValue->vt = VT_I2)
				{
					pDefProp->Value.lValue = pValue->iVal;
					pDefProp->Type = FE_NUMBER_NOTDEFINED;
				}
				else if (pValue->vt = VT_UI1)
				{
					pDefProp->Value.lValue = pValue->bVal;
					pDefProp->Type = FE_NUMBER_NOTDEFINED;
				}
				break;
		}
		return S_OK;
	}
	return E_INVALIDARG;
}


void ProcessInheritance(IDesignObject * pChildObject, IDataProperties * pParentData, IDataProperties * pNewData)
{
	if (pChildObject && pParentData)
	{
		IDataProperties * pChildData = NULL;
		IDataProperties * pNewData3 = NULL;
		CDataProperties * pNew = NULL;
		CDataProperties * pCChildData = NULL;
		CDataProperties * pCParentData = static_cast<CDataProperties*> (pParentData);
		ICPropBagDef * pPBDef = NULL;

		if (pChildObject->get_PropBagDef(&pPBDef) == S_OK)
		{

			pChildData = GetDataProperties(pPBDef);
			RELEASE(pPBDef);
			if (pChildData)
			{
				pCChildData = static_cast<CDataProperties*> (pChildData);
				if (pCChildData->m_nmIdChannel == pCParentData->m_nmIdChannel)
				{
					
					pNewData->Clone(&pNewData3);
					pNew = static_cast<CDataProperties*> (pNewData3);
					if (pCChildData->m_nmIdNode != pCParentData->m_nmIdNode)
					{
						// Es distinto
						pNew->m_nmIdNode = pCChildData->m_nmIdNode;
					}
					if (pCChildData->m_nmIdItem != pCParentData->m_nmIdItem)
					{
						// Es distinto
						pNew->m_nmIdItem = pCChildData->m_nmIdItem;
					}
					if (pCChildData->m_nmIdAccessAlias != pCParentData->m_nmIdAccessAlias)
					{
						// Es distinto
						pNew->m_nmIdAccessAlias = pCChildData->m_nmIdAccessAlias;
					}
					if (pCChildData->m_nmIdChannelAlias != pCParentData->m_nmIdChannelAlias)
					{
						// Es distinto
						pNew->m_nmIdChannelAlias = pCChildData->m_nmIdChannelAlias;
					}
					if (pCChildData->m_nmIdChannelKey != pCParentData->m_nmIdChannelKey)
					{
						// Es distinto
						pNew->m_nmIdChannelKey = pCChildData->m_nmIdChannelKey;
					}
					if (pCChildData->m_nGrants != pCParentData->m_nGrants)
					{
						// Es distinto
						pNew->m_nGrants = pCChildData->m_nGrants;
					}
					if (pCChildData->m_nIdPresentation != pCParentData->m_nIdPresentation)
					{
						// Es distinto
						pNew->m_nIdPresentation = pCChildData->m_nIdPresentation;
					}
					if (pCChildData->m_nStartPos != pCParentData->m_nStartPos)
					{
						// Es distinto
						pNew->m_nStartPos = pCChildData->m_nStartPos;
					}
					ApplyInheritance(pChildObject, pNewData3);
					RELEASE(pNewData3);
				}
				RELEASE(pChildData);
			}
		}
	}
}

BOOL DefPropEqual(DefProp * pDefProp)
{
	switch (pDefProp->Type)
	{
		// Numéricos
		case FE_NUMBER:
		case FE_COLOR:
		case FE_CONSTRUCTIF:
		case FE_GRANTSTYPE:
			return (pDefProp->Value.lValue == pDefProp->DefValue.lValue);
			break;
		// String
		case FE_STRING:
		case FE_DATE:
		case FE_IMAGE:
		case FE_ALIGNTYPE:
		case FE_LN4_CODEARGS:
			if (pDefProp->Value.pstrValue && pDefProp->DefValue.pstrValue)
			{
				// Si ambos punteros son válidos
				// Devolvemos la comparación del String
				return (strcmp(pDefProp->Value.pstrValue, pDefProp->DefValue.pstrValue) == 0);
			}
			else
			{
				// devolvemos si ambos son NULL
				return (pDefProp->Value.pstrValue == pDefProp->DefValue.pstrValue);
			}
			break;
		// Punteros IDispatch
		case FE_TRADUCTPROP:
		case FE_FONT:
		case FE_DATAPROP:
			// Ya contemplados
			break;
		// Booleanos
		case FE_BOOLEAN:
			return (pDefProp->Value.boolValue == pDefProp->DefValue.boolValue);
			break;
	}
	return 0;
}

void GetUniqueRef(char ** pNewID)
{
	*pNewID = (char*) malloc(9 * sizeof(char));

	USES_CONVERSION;
	// Generamos una Key
	GUID newGuid;
	CoCreateGuid(&newGuid);
	BSTR Result = ::SysAllocStringLen(NULL, 38);
	StringFromGUID2(newGuid, Result, 39);
	char* pTemp = M4VBToCpp(Result);
	::SysFreeString(Result);
 	strcpy((char *)*pNewID, "_");
	strncat((char *)*pNewID, pTemp+2, 7);
	delete pTemp;
}

void GetUniqueKey(char ** pNewID)
{
	*pNewID = (char*) malloc(9 * sizeof(char));

	USES_CONVERSION;
	// Generamos una Key
	GUID newGuid;
	CoCreateGuid(&newGuid);
	BSTR Result = ::SysAllocStringLen(NULL, 38);
	StringFromGUID2(newGuid, Result, 39);
	char* pTemp = M4VBToCpp(Result);
	::SysFreeString(Result);
 	strcpy((char *)*pNewID, "M");
	strncat((char *)*pNewID, pTemp+2, 7);
	delete pTemp;
}


void ApplyInh(CDataProperties * pParent, CDataProperties * pChild)
{

	if (pParent && pChild)
	{
		if (pChild->m_nmIdChannel == -1)
			pChild->m_nmIdChannel = pParent->m_nmIdChannel;
		if (pChild->m_nmIdNode == -1)
			pChild->m_nmIdNode = pParent->m_nmIdNode;
		if (pChild->m_nmIdItem == -1)
			pChild->m_nmIdItem = pParent->m_nmIdItem;
		if (pChild->m_nmIdAccessAlias == -1)
			pChild->m_nmIdAccessAlias = pParent->m_nmIdAccessAlias;
		if (pChild->m_nmIdChannelKey == -1)
			pChild->m_nmIdChannelKey = pParent->m_nmIdChannelKey;
		if (pChild->m_nmIdChannelAlias == -1)
			pChild->m_nmIdChannelAlias = pParent->m_nmIdChannelAlias;
		if (pChild->m_nIdPresentation == -1)
			pChild->m_nIdPresentation = pParent->m_nIdPresentation;
		if (pChild->m_nGrants == 31)
			pChild->m_nGrants = pParent->m_nGrants;
		if (pChild->m_nStartPos == -6)
			pChild->m_nStartPos = pParent->m_nStartPos;
	}
}


long GetIdFromBSTR(BSTR bstrName)
{
	USES_CONVERSION;
	
	char * cName = NULL;
	if (bstrName)
	{
		cName = M4VBToCpp(bstrName);
		long result = GetIdFromChar(cName);
		delete cName;
		return result;
	}
	else
	{
		return -1;
	}
}

long GetIdFromChar(char * cName)
{
	
	if ((cName == NULL) || strcmp(cName, "") == 0) return -1;
	// Primero se busca, si no existe lo creamos
	
	int nHash = HashKey(cName) % HASH_SIZE;

	if (gDataPropMapHash == NULL)
	{
		// Creamos tabla Hash
		gDataPropMapHash = (DataPropMap**) malloc(HASH_SIZE * sizeof(DataPropMap*));
		memset(gDataPropMapHash, 0, HASH_SIZE * sizeof(DataPropMap*));
	}
	DataPropMap * p;
	p = gDataPropMapHash[nHash];
	while(p && strcmp(p->strValue, cName) !=0)
	{
		p = p->pNextHash;
	}
	if (p)
	{
		return p->nId;
	}
	
	// Si estamos aquí es que no lo hemos encontrado
	// Habrá que crearlo
	p = (DataPropMap*)malloc(sizeof(DataPropMap));
	memset(p, 0, sizeof(DataPropMap));

	DataPropMap * p2 = gPropMapFirst;

	if (p2 == NULL)
		gPropMapFirst = p;
	else
	{
		while(p2->pNextSeq)
		{
			p2 = p2->pNextSeq;
		}
		p2->pNextSeq = p;
	}
	p->nId = gnIdPropMapFree++;   // Directamente le incrementamos uno
	long nLength = strlen(cName) +1;
	p->strValue = (char*) malloc(nLength);
	strcpy(p->strValue, cName);
	// Lo añadimos a la tabla hash
	// nHash ya está calculado
	p2 = gDataPropMapHash[nHash];
	if (p2)
	{
		while(p2->pNextHash)
		{
			p2 = p2->pNextHash;
		}
		p2->pNextHash = p;
		p->pNextHash = NULL;
	}
	else
	{
		gDataPropMapHash[nHash] = p;
		p->pNextHash = NULL;
	}

	if (p)
		return p->nId;

	return -1;
}

BSTR GetBSTRFromId(long nId)
{
	USES_CONVERSION;
	if (gPropMapFirst == NULL) return NULL;

	DataPropMap * p = gPropMapFirst;

	while(p && (p->nId != nId))
	{
		p = p->pNextSeq;
	}

	if (p)
	{
		return M4CppToVB(p->strValue);
	}
	else
	{
		return NULL;
	}
}

char * GetcharFromId(long nId)
{
	USES_CONVERSION;
	if (gPropMapFirst == NULL) return NULL;

	DataPropMap * p = gPropMapFirst;

	while(p && (p->nId != nId))
	{
		p = p->pNextSeq;
	}

	if (p)
	{
		return p->strValue;
	}
	else
		return NULL;
}

long AddProp2Map(char * cName)
{
	_FE_PROP_ENTRY * fpe = NULL;
	_FE_PROP_ENTRY * fpe2 = NULL;
	
	fpe = (_FE_PROP_ENTRY*)malloc(sizeof(_FE_PROP_ENTRY));
	fpe->nIndex = gPropNumber;
	fpe->pstrProperty = (char*)malloc(strlen(cName));
	
	if (gpPropMap)
	{
		fpe2 = gpPropMap;
		while(fpe2->pNextSeq)
		{
			fpe2 = fpe2->pNextSeq;
		}
		fpe2->pNextSeq = fpe;
		fpe->pNextSeq = NULL;
	}
	else
		gpPropMap = fpe;

	int nHash = HashKey((char*)fpe->pstrProperty)%HASH_SIZE;
	if (nHash >= 0)
	{
		if (gpPropHash[nHash] == NULL)
		{
			gpPropHash[nHash] = fpe;
			fpe->pNextHash = NULL;
		}
		else 
		{
			_FE_PROP_ENTRY * pe = gpPropHash[nHash];
			while(pe->pNextHash)
			{
				pe = pe->pNextHash;
			}
			pe->pNextHash = fpe;
			fpe->pNextHash = NULL;
		}
	}
	gPropNumber++;
	return fpe->nIndex;
}
