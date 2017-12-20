// CPropBagDef.cpp : Implementation of CCPropBagDef
#include "stdafx.h"
#include "m4prop.h"
#include "propdata.h"
#include "cpropcoldef.h"
#include "cchildcoldef.h"
#include "cpropbagdef.h"
#include "cdefprop.h"
#include "dataproperties.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// CCPropBagDef

void ApplyInh(CDataProperties * pParent, CDataProperties * pChild);

DefProp * CCPropBagDef::GetDefProp(long nId)
{
	return m_PropCol->GetDefProp(nId);
}


STDMETHODIMP CCPropBagDef::get_Alias(BSTR * pVal)
{
	
	USES_CONVERSION;
	
	if (pVal)
	{
		if (m_strAlias)
		{
			*pVal = M4CppToVB(m_strAlias);
		}
		else
		{
			*pVal = ::SysAllocString(L"");
		}
		return S_OK;
	}
	return E_POINTER;

}

STDMETHODIMP CCPropBagDef::put_Alias(BSTR newVal)
{
	USES_CONVERSION;

	char* pTemp = M4VBToCpp(newVal);
	ChangeAlias(pTemp);
	delete pTemp;
	return S_OK;
}

STDMETHODIMP CCPropBagDef::get_ClassName(BSTR * pVal)
{
	USES_CONVERSION;
	
	if (pVal)
	{
		if (m_strClassName)
		{
			*pVal = M4CppToVB(m_strClassName);
		}
		else
		{
			*pVal = ::SysAllocString(L"");
		}
		return S_OK;
	}
	return E_POINTER;
}

STDMETHODIMP CCPropBagDef::put_ClassName(BSTR newVal)
{
	USES_CONVERSION;

	char* pTemp = M4VBToCpp(newVal);
	ChangeClassName(pTemp);
	delete pTemp;
	return S_OK;	
}

STDMETHODIMP CCPropBagDef::AddChild(ICPropBagDef ** pbdChild, VARIANT vIndex)
{
	USES_CONVERSION;
	
	VARIANT v;
	HRESULT hr;

	CCPropBagDef * pCPBD = static_cast<CCPropBagDef*>(*pbdChild);
	
	VariantInit(&v);
	// Lo introducimos con la Key del elemento en cuestión
	hr = m_ChildCol->Add(*pbdChild, M4CComVariant(pCPBD->m_strKey));
	CDataProperties * pDataParent = NULL;
	CDataProperties * pDataChild = NULL;
	DefProp * pDef = m_PropCol->GetDefProp("DataProps");
	if (pDef)
	{
		DefProp * pDef2 = pCPBD->m_PropCol->GetDefProp("DataProps");	

		if (pDef2)
		{
			pDataParent = (CDataProperties*) pDef->Value.pdispValue;
			pDataChild = (CDataProperties*) pDef2->Value.pdispValue;
				
			ApplyInh(pDataParent, pDataChild);
		}
	}
	
	/*else
	{
		// Creamos información de error
		ICreateErrorInfo * pCreateErrorInfo;
		CreateErrorInfo(&pCreateErrorInfo);
		wchar_t tmpBuff[255];
				
		if (*pbdChild == NULL)
		{
			wcscpy(tmpBuff, L"The Child is NULL");

			pCreateErrorInfo->SetDescription(tmpBuff);
			pCreateErrorInfo->SetGUID(IID_ICPropBagDef);
			pCreateErrorInfo->SetSource(L"Property Bags Defs");

			IErrorInfo* pErrorInfo;
			pCreateErrorInfo->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo);
			SetErrorInfo(0, pErrorInfo);
			RELEASE(pErrorInfo);
			RELEASE(pCreateErrorInfo);
		}
		else
		{
			wcscpy(tmpBuff, L"Duplicate Object: ");
			wcscat(tmpBuff, v.bstrVal);
			wcscat(tmpBuff, L" In ");
			wcscat(tmpBuff, M4CComBSTR(m_strAlias));

			pCreateErrorInfo->SetDescription(tmpBuff);
			pCreateErrorInfo->SetGUID(IID_ICPropBagDef);
			pCreateErrorInfo->SetSource(L"Property Bags Defs");

			IErrorInfo* pErrorInfo;
			pCreateErrorInfo->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo);
			SetErrorInfo(0, pErrorInfo);
			RELEASE(pErrorInfo);
			RELEASE(pCreateErrorInfo);
		}
	}*/
	VariantClear(&v);
	return S_OK;
}

STDMETHODIMP CCPropBagDef::AddProperty(BSTR bstrName, VARIANT vValue, VARIANT vDefValue, M4PropertyType sType)
{
	USES_CONVERSION;
	DefProp *pDefProp = NULL;
	char* pTemp;
	
	if (wcscmp(L"Classname", bstrName) == 0)
	{
		pTemp = M4VBToCpp(vValue.bstrVal);
		ChangeClassName(pTemp);
		delete pTemp;
		return S_OK;
	}
	
	if (wcscmp(L"Alias", bstrName) == 0)
	{
		pTemp = M4VBToCpp(vValue.bstrVal);
		ChangeAlias(pTemp);
		delete pTemp;
		return S_OK;
	}

	char* pName = M4VBToCpp(bstrName);
	long nId = GetIdFromPropName(pName);
	
	if (nId == -1)
	{
		// Hay que añadir el nombre de propiedad al mapa
		nId = AddProp2Map(pName);
	}
	
	// return S_OK;
	pDefProp = m_PropCol->GetDefProp(pName);
				
	if (pDefProp)
	{
		PutValueInDefProp(pDefProp, &vValue);
		delete pName;
		return S_OK;
	}

	// Creamos un nuevo DefProp
	pDefProp = GetFreeBlock();
	
	// Rellenamos la DefProp
	pDefProp->Type = sType;
	PutValueInDefProp(pDefProp, &vValue);
	PutDefValueInDefProp(pDefProp, &vDefValue);
	pDefProp->nIdProperty = nId;

	 if (m_PropCol->AddDefProp(pDefProp, pName) == S_OK)
	 {
		delete pName;
		return S_OK;
	 }
	 delete pName;
	 FreeBlock(pDefProp);
	 return E_INVALIDARG;
}

STDMETHODIMP CCPropBagDef::get_PropColDef(ICPropColDef ** pVal)
{
	*pVal = m_PropCol;
	m_PropCol->AddRef();
	return S_OK;

}

STDMETHODIMP CCPropBagDef::putref_ChildColDef(ICChildColDef * pVal)
{
	if (pVal)
	{
		RELEASE(m_ChildCol);
		m_ChildCol = pVal;
		m_ChildCol->AddRef();
		return S_OK;
	}
	return E_POINTER;
}


STDMETHODIMP CCPropBagDef::put_ChildColDef(ICChildColDef * pVal)
{
	if (pVal)
	{
		RELEASE(m_ChildCol);
		m_ChildCol = pVal;
		m_ChildCol->AddRef();
		return S_OK;
	}
	return E_POINTER;
}

STDMETHODIMP CCPropBagDef::get_ChildColDef(ICChildColDef ** pVal)
{
	*pVal = m_ChildCol;
	m_ChildCol->AddRef();
	return S_OK;

}

// Property
// Le pasamos el valor actual de la property
// Nos encargamos de leerlo directamente de la colección
// en lugar de que el cliente se encarge de hacerlo por sí solo.

STDMETHODIMP CCPropBagDef::Property(BSTR PropertyName, VARIANT * vReturn)
{
	
	ICDefProp *pProp = NULL;

	HRESULT hr = m_PropCol->get_Item(CComVariant(PropertyName), &pProp);
	if (SUCCEEDED(hr))
	{
		// No necesito guardar el objeto para nada
		HRESULT hr = pProp->get_Value(vReturn);
		RELEASE(pProp);
		return hr;
	}
	else
	{
		// Creamos información de error
		ICreateErrorInfo * pCreateErrorInfo;
		CreateErrorInfo(&pCreateErrorInfo);
		wchar_t tmpBuff[255];
				
		wcscpy(tmpBuff, L"This Property Doesn't Exist!:   ");
		wcscat(tmpBuff, PropertyName);

		pCreateErrorInfo->SetDescription(tmpBuff);
		pCreateErrorInfo->SetGUID(IID_ICPropBagDef);
		pCreateErrorInfo->SetSource(L"Property Bags Defs");

		IErrorInfo* pErrorInfo;
		pCreateErrorInfo->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo);
		SetErrorInfo(0, pErrorInfo);
		RELEASE(pErrorInfo);
		RELEASE(pCreateErrorInfo);
	}
	return hr;
}

STDMETHODIMP CCPropBagDef::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ICPropBagDef,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CCPropBagDef::get_ChildPBDefCountRec(long * pVal)
{
	USES_CONVERSION;
	
	long nCount = 0;
	long nTemp = 0;
	*pVal = 0;	
	
	// Primero recorremos las PBD hijas
	HRESULT hr = m_ChildCol->get_Count(&nCount);
	if (nCount > 0)
	{
		// Las recorremos en sentido inverso
		for(long tmpCount = nCount; tmpCount > 0; tmpCount--)
		{
			ICPropBagDef * pInnerPBD = NULL;
			hr = m_ChildCol->get_Item(CComVariant(tmpCount), &pInnerPBD);
			if (pInnerPBD)
			{
				nTemp = 0;
				hr = pInnerPBD->get_ChildPBDefCountRec(&nTemp);
				*pVal += nTemp +1;
				RELEASE(pInnerPBD);
			}
		}
	}

	return hr;
}

DefProp * CCPropBagDef::GetDefProp(const char *key)
{
	long nL = strlen(key);
	char * pTemp = (char*)malloc(nL+1);
	strcpy(pTemp, key);
	DefProp * pRet = m_PropCol->GetDefProp(pTemp);
	free(pTemp);
	return pRet;
}

void CCPropBagDef::ChangeAlias(char *strAlias)
{
	if (strAlias)
	{
		long nLength = strlen(strAlias) + 1;

		if (nLength > m_nAliasLength)
		{
			m_strAlias = (char*) realloc(m_strAlias, nLength);
			m_nAliasLength = (short) nLength;
		}
		strcpy(m_strAlias, strAlias);
	}
	else
	{
		free(m_strAlias);
		m_strAlias = NULL;
	}
}

void CCPropBagDef::ChangeClassName(char *strClassName)
{
	if (strClassName)
	{
		long nLength = strlen(strClassName) + 1;

		if (nLength > m_nClassNameLength)
		{
			m_strClassName = (char*) realloc(m_strClassName, nLength);
			m_nClassNameLength = (short) nLength;
		}
		strcpy(m_strClassName, strClassName);
	}
	else
	{
		free(m_strClassName);
		m_strClassName = NULL;
	}
}


STDMETHODIMP CCPropBagDef::get_Key(BSTR *pVal)
{
	USES_CONVERSION;
	
	if (pVal)
	{
		if (m_strKey)
		{
			*pVal = M4CppToVB(m_strKey);
		}
		else
		{
			*pVal = ::SysAllocString(L"");
		}
		return S_OK;
	}
	return E_POINTER;
}


STDMETHODIMP CCPropBagDef::Reset()
{
	// Nos encargamos de resetear las propiedades a sus valores por defecto
	m_PropCol->Reset();
	return S_OK;
}

STDMETHODIMP CCPropBagDef::ReplaceChild(ICPropBagDef *pOld, ICPropBagDef *pNew)
{
	HRESULT hr = E_POINTER;
	
	if (pOld && pNew)
	{
		// Eliminamos el antiguo
		CCPropBagDef * pPBD = (CCPropBagDef*)pOld;
		hr = m_ChildCol->Remove(M4CComVariant(pPBD->m_strKey));
		if (hr == S_OK)
		{
			hr = AddChild(&pNew, CComVariant(""));
		}
	}
	return hr;
}



STDMETHODIMP CCPropBagDef::SolveTranslation()
{

	HRESULT			hr = S_OK ;
	bool			bFound = false ;
	long			i = 0 ;
	long			j = 0 ;
	long			k = 0 ;
	long			iLength = 0 ;
	DefProp*		pDefProp = NULL ;
	DefProp*		pChildDefProp = NULL ;
	ITraductProp*	pTraduct = NULL ;
	ICPropBagDef*	pChildPBD = NULL;
	CCPropBagDef*	pChildCPBD = NULL ;
	CCPropColDef*	pChildPCD = NULL ;

	
	// Se recorren las propiedades buscando objetos traducibles
	for( i = 0 ; i < m_PropCol->m_nCount ; i++ )
	{
		pDefProp = m_PropCol->GetDefProp( i + 1 ) ;

		if( pDefProp == NULL || pDefProp->Type != FE_TRADUCTPROP )
		{
			continue ;
		}

		pTraduct = static_cast<ITraductProp*> (pDefProp->Value.pdispValue) ;

		if( pTraduct == NULL )
		{
			continue ;
		}

		// Recorremos los hijos buscando las traducciones
		hr = m_ChildCol->get_Count( &iLength ) ;

		if( hr != S_OK )
		{
			return( hr ) ;
		}

		bFound = false ;

		for( j = 1 ; j <= iLength && bFound == false ; j++ )
		{
			hr = m_ChildCol->get_Item( CComVariant(j), &pChildPBD ) ;

			if( pChildPBD != NULL )
			{
				pChildCPBD = static_cast<CCPropBagDef*>(pChildPBD) ;

				if( strcmpi( pChildCPBD->m_strClassName, "TraductProp" ) == 0 )
				{
					pChildPCD = pChildCPBD->m_PropCol ;

					// Se busca la propiedad Name y se comprueba que sea el nombre de la propiedad traducida
					for( k = 1 ; k <= pChildPCD->m_nCount && bFound == false ; k++ )
					{
						pChildDefProp = pChildPCD->GetDefProp( k ) ;

						if(	pChildDefProp == NULL || pChildDefProp->Type != FE_STRING )
						{
							continue ;
						}

						if( strcmpi( pChildDefProp->Key, "Name" ) == 0 )
						{
							if( strcmpi( pDefProp->Key, pChildDefProp->Value.pstrValue ) == 0 )
							{
								// Lo hemos encontrado
								bFound = true ;
							}
							else
							{
								break ;
							}
						}
					}

					// Si se han encontrado las traducciones se sustituyen
					if( bFound == true )
					{
						pTraduct->put_Identifier( M4CComBSTR( pChildCPBD->m_strAlias ) ) ;

						for( k = 1 ; k <= pChildPCD->m_nCount ; k++ )
						{
							pChildDefProp = pChildPCD->GetDefProp( k ) ;

							if(	pChildDefProp == NULL || ( pChildDefProp->Type != FE_STRING && pChildDefProp->Type != FE_STRING_NOTDEFINED ) )
							{
								continue ;
							}

							if( strncmp( pChildDefProp->Key, "Value_", 6 ) == 0 )
							{
								pTraduct->put_ValueByLng( M4CComVariant( pChildDefProp->Key + 6 ), M4CComBSTR( pChildDefProp->Value.pstrValue ) ) ;
							}
						}

						// Se borran los idiomas
						m_ChildCol->Remove( CComVariant(j) ) ;
					}
				}

				RELEASE( pChildPBD ) ;
			}
		}
	}


	// Recursión por los hijos
	hr = m_ChildCol->get_Count( &iLength ) ;

	if( hr != S_OK )
	{
		return( hr ) ;
	}

	for( j = 1 ; j <= iLength ; j++ )
	{
		hr = m_ChildCol->get_Item( CComVariant(j), &pChildPBD ) ;

		if( pChildPBD != NULL )
		{
			pChildCPBD = static_cast<CCPropBagDef*>(pChildPBD) ;

			hr = pChildPBD->SolveTranslation() ;
			RELEASE( pChildPBD ) ;

			if( hr != S_OK )
			{
				return( hr ) ;
			}
		}
	}

	return S_OK;
}
