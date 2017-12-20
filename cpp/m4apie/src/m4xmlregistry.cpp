// m4xmlregistry.cpp : Implementation of CM4XmlRegistry
#include "stdafx.h"
#include "m4apie.h"
#include "m4xmlregistry.h"
#include "afxinet.h"
#include "m4xmlreg.hpp"
#include "m4unicode.hpp"
#include "gstring.h"

/////////////////////////////////////////////////////////////////////////////
// CM4XmlRegistry

STDMETHODIMP CM4XmlRegistry::get_Handle(short * retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = m_iHandle;

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::put_Handle(short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal < 0)
	{
		m_bUseGlobalInstance = TRUE;
		m_iHandle = UNDEFINDED_REGISTRY_HANDLE;
	}
	else
	{
		m_bUseGlobalInstance = FALSE;
		m_iHandle = newVal;
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::get_UseGlobalInstance(VARIANT_BOOL * retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = m_bUseGlobalInstance ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::put_UseGlobalInstance(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (newVal == VARIANT_FALSE)
	{
		m_bUseGlobalInstance = FALSE;
	}
	else
	{
		m_bUseGlobalInstance = TRUE;
	}
	m_iHandle = UNDEFINDED_REGISTRY_HANDLE;

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::LoadMachineRegistry( VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == FALSE) // Sólo aplicable a instance de fichero
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (reg->LoadMachine() == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::Load( BSTR ai_sFile, VARIANT_BOOL ai_bIsXml, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == FALSE) // Sólo aplicable a instancia de fichero
	{
		gstring		pszFile(ai_sFile);
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (reg->Load(pszFile, ai_bIsXml == VARIANT_FALSE ? M4_FALSE : M4_TRUE) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::GetValue(BSTR ai_sKey, BSTR ai_sValue, BSTR *ao_sData, VARIANT_BOOL * retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m4return_t	iReturn = M4_ERROR;
	m4pchar_t	pcData = NULL;
	gstring		pszKey(ai_sKey);
	gstring		pszValue(ai_sValue);
	M4XmlRegistryFile *reg = NULL;

	*retVal = VARIANT_FALSE;

	// Lo suyo sería que M4XmlRegistryFile y M4XmlRegistry tuvieran interfaz común
	// pero no nos metemos ahora con dichos cambios para minimizar cambios en la m4ini
	if (m_bUseGlobalInstance == FALSE)
	{
		reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);
	}

	if (m_bUseGlobalInstance == TRUE || reg != NULL)
	{
		if (*ao_sData != NULL)
		{
			SysFreeString(*ao_sData);
			*ao_sData = NULL;
		}

		if (m_bUseGlobalInstance)
		{
			iReturn = M4XmlRegistry::Instance()->GetValue(pszKey, pszValue, pcData);
		}
		else
		{
			iReturn = reg->GetValue(pszKey, pszValue, pcData);
		}

		if ( iReturn == M4_SUCCESS )
		{
			*ao_sData = M4CppToVB(pcData);
			delete[] pcData;

			*retVal = VARIANT_TRUE;
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::SetValue( BSTR ai_sKey,  BSTR ai_sValue,  BSTR ai_sData, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	gstring		pszKey(ai_sKey);
	gstring		pszValue(ai_sValue);
	gstring		pszData(ai_sData);

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == TRUE)
	{
		if (M4XmlRegistry::Instance()->SetValue(pszKey, pszValue, pszData) == M4_SUCCESS)
		{
			*retVal = VARIANT_TRUE;
		}
	}
	else
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (reg->SetValue(pszKey, pszValue, pszData) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

HRESULT CM4XmlRegistry::_ElementNames( BOOL ai_ReadValues, BSTR ai_sKey, SAFEARRAY ** ao_psaElements, VARIANT_BOOL * retVal )
{
	m4return_t		iResult = M4_ERROR;
	HRESULT			hResult = S_OK;
	size_t			i = 0;
	size_t			iLength = 0;
	m4pchar_t		pccElementName = NULL;
	BSTR			*psElements = NULL;
	gstring			pszKey(ai_sKey);
	list<m4pchar_t>	oValues;

	list<m4pchar_t>::iterator	itIterator;

	M4XmlRegistryFile *reg = NULL;

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == FALSE)
	{
		reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);
	}

	if (m_bUseGlobalInstance == TRUE || reg != NULL)
	{
		if (*ao_psaElements != NULL)
		{
			SafeArrayDestroy(*ao_psaElements);
			*ao_psaElements = NULL;
		}

		if (ai_ReadValues == TRUE)
		{
			if (m_bUseGlobalInstance == TRUE)
			{
				iResult = M4XmlRegistry::Instance()->GetValueNames(pszKey, oValues);
			}
			else
			{
				iResult = reg->GetValueNames(pszKey, oValues);
			}
		}
		else
		{
			if (m_bUseGlobalInstance == TRUE)
			{
				iResult = M4XmlRegistry::Instance()->GetSubKeyNames(pszKey, oValues);
			}
			else
			{
				iResult = reg->GetSubKeyNames(pszKey, oValues);
			}
		}

		if (iResult != M4_SUCCESS)
		{
			return S_OK;
		}

		iLength = oValues.size();

		*ao_psaElements = SafeArrayCreateVector(VT_BSTR, 0, iLength);

		if (*ao_psaElements == NULL)
		{
			return S_OK;
		}

		hResult = SafeArrayAccessData(*ao_psaElements, (void**)&psElements);

		if (hResult != S_OK)
		{
			SafeArrayDestroy(*ao_psaElements);
			*ao_psaElements = NULL;
			return S_OK;
		}

		i = 0;
		itIterator = oValues.begin();

		while (itIterator != oValues.end())
		{
			pccElementName = *itIterator;
			psElements[i++] = M4CppToVB(pccElementName);
			delete[] pccElementName;
			itIterator++;
		}
		oValues.clear();

		hResult = SafeArrayUnaccessData(*ao_psaElements);

		if (hResult != S_OK)
		{
			SafeArrayDestroy(*ao_psaElements);
			*ao_psaElements = NULL;
			return S_OK;
		}

		*retVal = VARIANT_TRUE;
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::GetValueNames( BSTR ai_sKey, SAFEARRAY **ao_psaValues, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return _ElementNames(M4_TRUE, ai_sKey, ao_psaValues, retVal);
}

STDMETHODIMP CM4XmlRegistry::GetSubKeyNames( BSTR ai_sKey, SAFEARRAY **ao_psaKeys, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return _ElementNames(M4_FALSE, ai_sKey, ao_psaKeys, retVal);
}

STDMETHODIMP CM4XmlRegistry::DeleteValue( BSTR ai_sKey,  BSTR ai_sValue, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	gstring		pszKey(ai_sKey);
	gstring		pszValue(ai_sValue);

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == TRUE)
	{
		if (M4XmlRegistry::Instance()->DeleteValue(pszKey, pszValue) == M4_SUCCESS)
		{
			*retVal = VARIANT_TRUE;
		}
	}
	else
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);
		if (reg != NULL)
		{
			if (reg->DeleteValue(pszKey, pszValue) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::DeleteKey( BSTR ai_sKey, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	gstring		pszKey(ai_sKey);

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == TRUE)
	{
		if (M4XmlRegistry::Instance()->DeleteKey(pszKey) == M4_SUCCESS)
		{
			*retVal = VARIANT_TRUE;
		}
	}
	else
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (reg->DeleteKey(pszKey) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::Merge( short ai_iDestinationHandle, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == FALSE) // Sólo aplicable a instancia de fichero
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);
		M4XmlRegistryFile *regDestionation = M4XmlRegistryFile::GetRegistryFile(ai_iDestinationHandle);

		if (reg != NULL && regDestionation != NULL)
		{
			if (reg->Merge(*regDestionation) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
		else
		{
			*retVal = VARIANT_FALSE;
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::Save( BSTR ai_sFile, VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	gstring		pszFile(ai_sFile);

	*retVal = VARIANT_FALSE;

	if (m_bUseGlobalInstance == TRUE)
	{
		if (M4XmlRegistry::Instance()->Save(pszFile) == M4_SUCCESS)
		{
			*retVal = VARIANT_TRUE;
		}
	}
	else
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (reg->Save(pszFile) == M4_SUCCESS)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CM4XmlRegistry::UnLoad( VARIANT_BOOL * retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retVal = VARIANT_FALSE;
	if (m_bUseGlobalInstance == TRUE)
	{
		M4XmlRegistry::FreeInstance();
		*retVal = VARIANT_TRUE;
	}
	else
	{
		M4XmlRegistryFile *reg = M4XmlRegistryFile::GetRegistryFile(m_iHandle);

		if (reg != NULL)
		{
			if (M4XmlRegistryFile::FreeRegistryFile(m_iHandle) == M4_TRUE)
			{
				*retVal = VARIANT_TRUE;
			}
		}
	}

	return S_OK;
}

