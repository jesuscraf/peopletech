// M4RegKey.cpp : Implementation of CM4RegKey
#include "stdafx.h"
#include "m4apie.h"
#include "m4inireg.hpp"
#include "m4regkey.h"



/////////////////////////////////////////////////////////////////////////////
// CM4RegKey


//##ModelId=396B329901B4
STDMETHODIMP CM4RegKey::CloseKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	m_rgKey.CloseKey();
	return S_OK;

}

//##ModelId=396B329901BF
STDMETHODIMP CM4RegKey::get_GetNumberOfKeys(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	*pVal = m_rgKey.GetNumberOfKeys();
	return S_OK;

}

//##ModelId=396B329901C8
STDMETHODIMP CM4RegKey::get_GetNumberOfValues(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	*pVal = m_rgKey.GetNumberOfValues();

	return S_OK;
}

//##ModelId=396B329901D3
STDMETHODIMP CM4RegKey::get_KeyName(long nIndex, BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here
	char lpstrPrueba[500];

	m_rgKey.GetSubKeyName(nIndex, lpstrPrueba);
	*pVal = ::SysAllocString(A2OLE(lpstrPrueba));

	return S_OK;
}

//##ModelId=396B32990204
STDMETHODIMP CM4RegKey::get_MainKey(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CComBSTR p;
	//BSTR bstrNewVal;
	char cTemp[JFM_MAX_CHAR];
	
	m_rgKey.GetMainKey(cTemp);
	
	p.Empty();
	p.Append(cTemp);
	
	*pVal = SysAllocString((BSTR)p);
	
	p.Empty();

	return S_OK;
}

//##ModelId=396B329901F1
STDMETHODIMP CM4RegKey::put_MainKey(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	// TODO: Add your implementation code here
	char * cTemp;
	
	// Liberamos memoria
	SysFreeString(m_bstrMainKey);
	// Asignamos la BSTR de origen
	if (newVal)
	{
		m_bstrMainKey = SysAllocString(newVal);
	}
	else
	{
		m_bstrMainKey = SysAllocString(L"");
	}
	
	// Convertimos la BSTR a un array de Char
	cTemp = OLE2A(m_bstrMainKey);
	//ConvertBSTR2char(m_bstrMainKey, cTemp);

	m_rgKey.SetMainKey(cTemp);

	return S_OK;
}

//##ModelId=396B32990218
STDMETHODIMP CM4RegKey::get_SubKey(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CComBSTR p;
	char cTemp[JFM_MAX_CHAR];
	
	m_rgKey.GetSubKey(cTemp);

	p.Empty();
	p.Append(cTemp);
	
	// En General esto se cumplirá
	*pVal = SysAllocString((BSTR)p);
	p.Empty();

	return S_OK;
}

//##ModelId=396B3299020E
STDMETHODIMP CM4RegKey::put_SubKey(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here
	char * cTemp;

	SysFreeString(m_bstrSubKey);
	

	if (newVal)
	{
		m_bstrSubKey = SysAllocString(newVal);
	}
	else
	{
		m_bstrSubKey = ::SysAllocString(L"");
	}

	// Convertimos la BSTR a un array de Char
	cTemp = OLE2A(m_bstrSubKey);
	//ConvertBSTR2char(m_bstrSubKey, cTemp);

	// Y la colocamos en su sitio
	m_rgKey.SetSubKey(cTemp);

	return S_OK;
}

//##ModelId=396B32990223
STDMETHODIMP CM4RegKey::get_ValidPosition(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	*pVal = m_rgKey.IsValidPosition();

	return S_OK;
}

//##ModelId=396B3299024A
STDMETHODIMP CM4RegKey::get_Value(BSTR bstrValueName, VARIANT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here
	//char * cTemp;

	//ConvertBSTR2char(bstrValueName, cTemp);
	if (m_rgKey.GetValue(OLE2A(bstrValueName), pVal) == TRUE)
	{	
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}

}

//##ModelId=396B32990236
STDMETHODIMP CM4RegKey::put_Value(BSTR bstrValueName, VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here

	char * cTemp;
	
	//ConvertBSTR2char(bstrValueName, cTemp);

	if (bstrValueName)
	{
		cTemp = OLE2A(bstrValueName);
	}
	else
	{
		cTemp = OLE2A(L"");
	}

	if (newVal.vt == VT_I4 || newVal.vt == VT_I2) // Caso Long
	{
		long Value;
		Value = (long)newVal.lVal;
		if (m_rgKey.SetValueLong(cTemp, Value) == TRUE)
		{
			return S_OK;
		}
	}
	else if (newVal.vt == VT_BSTR)
	{
		char * pstrValue;
		
		if (newVal.bstrVal)
		{
			pstrValue = OLE2A(newVal.bstrVal);
			
		}
		else
		{
			pstrValue = OLE2A(L"");
		}
		//ConvertBSTR2char(newVal.bstrVal, strValue);
		
		if (m_rgKey.SetValueString(cTemp, pstrValue) == TRUE)
		{
			return S_OK;
		}
	}

	return S_FALSE;

}

//##ModelId=396B3299025E
STDMETHODIMP CM4RegKey::get_ValueName(long nIndex, BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CComBSTR p;
	char cTemp[JFM_MAX_CHAR];
	
	
	p.Empty();
	// Obtenemos la subclave
	m_rgKey.GetValueName(nIndex, cTemp);
	
	p.Append(cTemp);
	// La pasamos y devolvemos (aunque no esté definida)

	*pVal = SysAllocString((BSTR)p);
	return S_OK;

}

//##ModelId=396B32990272
STDMETHODIMP CM4RegKey::OpenKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	m_rgKey.OpenKey();

	return S_OK;
}

//##ModelId=396B32990196
STDMETHODIMP CM4RegKey::CreateKey()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	m_rgKey.CreateKey();

	return S_OK;
}

//////////////////////////////////////////////////////


/*void CM4RegKey::ConvertBSTR2char(BSTR bstrIn, char * charOut)
{
	long	Contador;

	Contador = 0;
	while ((bstrIn[Contador] != 0) && (Contador < JFM_MAX_CHAR -1))
	{
		charOut[Contador] = (char)bstrIn[Contador];
		Contador +=1;
	}
	// Le finalizamos con cero
	charOut[Contador]=0;

}*/


//##ModelId=396B3299027D
STDMETHODIMP CM4RegKey::DeleteKey(BSTR KeyName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here

	//char chartemp[JFM_MAX_CHAR];
	
	//ConvertBSTR2char(KeyName, chartemp);
	
	if (m_rgKey.DeleteKey(OLE2A(KeyName)) == TRUE)
	{
		return S_OK;
	}
	return S_FALSE;
}

//##ModelId=396B32990274
STDMETHODIMP CM4RegKey::DeleteValue(BSTR ValueName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	// TODO: Add your implementation code here

	//char chartemp[JFM_MAX_CHAR];
	
	//ConvertBSTR2char(KeyName, chartemp);
	
	if (m_rgKey.DeleteValue(OLE2A(ValueName)) == TRUE)
	{
		return S_OK;
	}
	return S_FALSE;
}


//##ModelId=396B32990288
STDMETHODIMP CM4RegKey::OpenKeyEx(M4RegKeyMode openMode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_rgKey.OpenKeyEx(openMode);

	return S_OK;
}
