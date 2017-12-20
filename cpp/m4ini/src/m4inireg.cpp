
//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini
// File:                m4inireg.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-03-2008
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module defines the interface to access registry information 
//
//
//==============================================================================

#include "definitions.h"
#include "m4inireg.hpp"
#include "m4xmlreg.hpp"
#include "m4unicode.hpp"


//Implementada en m4xmlreg
void	_ClearList( list<m4pchar_t> &ao_rlSubKeys );

// Constructor

//##ModelId=396B32980019
CMyRegKey::CMyRegKey()
{
	m_bMain = FALSE;
	m_bSub = FALSE;
	m_bOpen = FALSE;
	m_hKey = NULL;
	m_openhKey = NULL;
	*m_strSubKeyAnsi = '\0';
	m_pcSubKeyCpp = NULL;

	m_bAccessBym4ini = FALSE;
	m_bLocalMachine = FALSE;
	
}

CMyRegKey::~CMyRegKey()
{
	if (m_pcSubKeyCpp != NULL)
	{
		delete[] m_pcSubKeyCpp ;
		m_pcSubKeyCpp = NULL ;
	}
}

//##ModelId=396B32980022
void CMyRegKey::SetMainKey(LPCTSTR ai_sMainKey)
{

	m_bMain = TRUE;
	m_bOpen = FALSE;
	m_bLocalMachine = FALSE;

	if (CharCompare("HKEY_CLASSES_ROOT", ai_sMainKey) || CharCompare("HKCR", ai_sMainKey))
	{
		m_hKey = HKEY_CLASSES_ROOT;	
		return;
	}

	if (CharCompare("HKEY_CURRENT_USER", ai_sMainKey) || CharCompare("HKCU", ai_sMainKey))
	{
		m_hKey = HKEY_CURRENT_USER;
		return;
	}

	if (CharCompare("HKEY_LOCAL_MACHINE", ai_sMainKey) || CharCompare("HKLM", ai_sMainKey))
	{
		m_bLocalMachine = TRUE;
		m_hKey = HKEY_LOCAL_MACHINE;
		return;
	}

	if (CharCompare("HKEY_USERS", ai_sMainKey) || CharCompare("HKU", ai_sMainKey))
	{
		m_hKey = HKEY_USERS;
		return;
	}
	
	if (CharCompare("HKEY_PERFORMANCE_DATA", ai_sMainKey) || CharCompare("HKPD", ai_sMainKey))
	{
		m_hKey = HKEY_PERFORMANCE_DATA;
		return;
	}

	if (CharCompare("HKEY_DYN_DATA", ai_sMainKey) || CharCompare("HKDD", ai_sMainKey))
	{
		m_hKey = HKEY_DYN_DATA;
		return;
	}

	if (CharCompare("HKEY_CURRENT_CONFIG", ai_sMainKey) || CharCompare("HKCC", ai_sMainKey))
	{
		m_hKey = HKEY_CURRENT_CONFIG;
		return;
	}

	m_bOpen = FALSE;
	m_bMain = FALSE;
}

//##ModelId=396B32980037
void CMyRegKey::GetMainKey(LPTSTR ao_sMainKey)
{

	if (m_bMain)
	{
		if (m_hKey == HKEY_CLASSES_ROOT )
		{
			strcpy(ao_sMainKey, "HKEY_CLASSES_ROOT");
			return;
		}

		if (m_hKey == HKEY_CURRENT_CONFIG )
		{
			strcpy(ao_sMainKey, "HKEY_CURRENT_CONFIG");
			return;
		}

		if (m_hKey == HKEY_DYN_DATA )
		{
			strcpy(ao_sMainKey, "HKEY_DYN_DATA");
			return;
		}

		if (m_hKey == HKEY_PERFORMANCE_DATA )
		{
			strcpy(ao_sMainKey, "HKEY_PERFORMANCE_DATA");
			return;
		}

		if (m_hKey == HKEY_CURRENT_USER )
		{
			strcpy(ao_sMainKey, "HKEY_CURRENT_USER");
			return;
		}

		if (m_hKey == HKEY_LOCAL_MACHINE )
		{
			strcpy(ao_sMainKey, "HKEY_LOCAL_MACHINE");
			return;
		}

		if (m_hKey == HKEY_USERS )
		{
			strcpy(ao_sMainKey, "HKEY_USERS");
			return;
		}
	}
	
	// Si no es ninguna de estas:
	strcpy(ao_sMainKey, "NOT_DEFINED");
}

//##ModelId=396B32980024
void CMyRegKey::SetSubKey(LPCTSTR ai_sSubKey)
{

	int iLength = -1; 
	size_t iSubKeyLength = -1 ;
	m4pchar_t	pcSubKey = NULL;
	LPCTSTR sSubKey = "SOFTWARE\\Meta4"; 

	strcpy( m_strSubKeyAnsi, ai_sSubKey );
	
	
	m_bSub = TRUE;
	m_bOpen = FALSE;

	
	//Si la subclave que nos pasan contiene SOFTWARE\Meta4 que corresponde
	// con la rama del registro de meta4, accedemos al registro a través de la m4ini
	iSubKeyLength = strlen(sSubKey) ;

	if ( strnicmp (sSubKey, m_strSubKeyAnsi, iSubKeyLength ) == 0 && m_bLocalMachine == TRUE)
	{
		m_bAccessBym4ini = TRUE;

		pcSubKey = M4ANSIToCpp(m_strSubKeyAnsi, iLength);
		
		if( m_pcSubKeyCpp != NULL )
		{
			delete[] m_pcSubKeyCpp ;
			m_pcSubKeyCpp = NULL ;
		}

		m_pcSubKeyCpp = new m4char_t[iLength + 1] ;

		strcpy( m_pcSubKeyCpp, pcSubKey );

		delete[] pcSubKey;
	}
	
}

//##ModelId=396B32980040
void CMyRegKey::GetSubKey(LPSTR ao_sSubKey)
{

	if (m_bSub)
	{
		strcpy(ao_sSubKey, m_strSubKeyAnsi);
	}
	else
	{
		strcpy(ao_sSubKey, "NOT_DEFINED");
	}

}

//##ModelId=396B32980018
BOOL CMyRegKey::OpenKey()
{
	// Abrimos la key posicionada
	if (m_bMain && m_bSub)
	{
		//En modo m4ini
		if (m_bAccessBym4ini)
		{
			m_bOpen = TRUE;
			return TRUE;
		}

		//Acceso por api de windows al registro
		if (RegOpenKeyEx(m_hKey, m_strSubKeyAnsi,0L, KEY_ALL_ACCESS , &m_openhKey) == ERROR_SUCCESS) 
		{
			m_bOpen = TRUE;
			return TRUE;
		}
	}
	
	m_bOpen = FALSE;
	return FALSE;
}

//##ModelId=396B32980091
BOOL CMyRegKey::OpenKeyEx(M4RegKeyMode ao_iOpenMode)
{
	// Abrimos la key posicionada
	if (m_bMain && m_bSub)
	{
		//En modo m4ini
		if (m_bAccessBym4ini)
		{
			m_bOpen = TRUE;
			return TRUE;
		}

		//Acceso por api de windows al registro
		if (RegOpenKeyEx(m_hKey, m_strSubKeyAnsi,0L, KEY_ALL_ACCESS * (ao_iOpenMode == ALL_ACCESS) + KEY_READ * (ao_iOpenMode == READ_ACCESS) + KEY_WRITE * (ao_iOpenMode == WRITE_ACCESS),  &m_openhKey) == ERROR_SUCCESS) 
		{
			m_bOpen = TRUE;
			return TRUE;
		}
	}

	m_bOpen = FALSE;
	return FALSE;
	
}

//##ModelId=396B32980090
BOOL CMyRegKey::CreateKey()
{
	// Abrimos la key posicionada
	if (m_bMain && m_bSub)
	{
		//En modo m4ini hacemos un setvalue a ""
		if (m_bAccessBym4ini)
		{
			m_bOpen = TRUE;

			//No hacemos nada.
			return TRUE;
		}

		//Acceso por api de windows al registro
		if (RegCreateKeyEx(m_hKey, m_strSubKeyAnsi,0L, NULL, REG_OPTION_NON_VOLATILE , KEY_ALL_ACCESS, NULL, &m_openhKey, NULL) == ERROR_SUCCESS) 
		{
			
			m_bOpen = TRUE;
			return TRUE;
		}
	}

	m_bOpen = FALSE;
	return FALSE;
}


//##ModelId=396B3298002D
void CMyRegKey::CloseKey()
{

	if (!m_bAccessBym4ini)
	{
		if (RegCloseKey(m_openhKey) != ERROR_SUCCESS)
		{
			return;
		}
	}

	m_bSub = FALSE;
	m_hKey = NULL;
	m_openhKey = NULL;
	m_bOpen = FALSE;
	strcpy(m_strSubKeyAnsi, "");
	//strcpy(m_pcSubKeyCpp, "");

	m_bLocalMachine = FALSE; 
	m_bAccessBym4ini = FALSE;
}

//##ModelId=396B3298002E
long CMyRegKey::GetNumberOfKeys()
{
	DWORD dwIndex;
	unsigned long ntemp, ntemp2, t1, t2;
	char ctemp[250], ctemp2[250];
	FILETIME ftemp;
	long lResult;
	m4return_t	iResult = M4_ERROR ;
	
	list<m4pchar_t> rlSubKeys;


	if (m_bMain && m_bSub && m_bOpen)
	{
		dwIndex = 0;

		if (m_bAccessBym4ini)
		{
			iResult  = M4XmlRegistry::Instance()->GetSubKeyNames(m_pcSubKeyCpp, rlSubKeys) ;

			if( iResult != M4_ERROR )
			{
				dwIndex = (DWORD) rlSubKeys.size();
				_ClearList(rlSubKeys);

				return (dwIndex);
			}
		}
		else
		{
			//Acceso por api de windows al registro
			lResult = 1000;
			t1 = ntemp = sizeof(ctemp);
			t2 = ntemp2 = sizeof(ctemp2);
			
			while ((lResult = RegEnumKeyEx(m_openhKey, dwIndex, &ctemp[0], &ntemp, NULL, &ctemp2[0], &ntemp2, &ftemp))== ERROR_SUCCESS)
			{
				ntemp = t1;
				ntemp2 = t2;
				dwIndex ++;
			}	

			return dwIndex;
		}
	}

	return -1;

}

//##ModelId=396B3298004A
BOOL CMyRegKey::GetSubKeyName(long ai_nIndex, LPSTR ao_sName)
{

	unsigned long ntemp ;
	unsigned long ntemp2;
	char		ctemp[250] ;
	char		ctemp2[250];
	FILETIME	ftemp;
	long		lResult;
	DWORD		dwIndex = 0;
	list<m4pchar_t> rlSubKeys ;
	m4pchar_t	pcKeyName = NULL ;
	list<m4pchar_t>::iterator	itIterator ;
	m4pchar_t sName ;
	LPTSTR		sNameANSI = NULL;
	m4return_t	iResult = M4_ERROR ;
	BOOL bFound = FALSE; 
	int iLength = -1 ;

	strcpy(ao_sName, "NOT_DEFINED");

	if (m_bMain && m_bSub && m_bOpen)
	{
		if (m_bAccessBym4ini)
		{
			dwIndex = 0 ;
			iResult = M4XmlRegistry::Instance()->GetSubKeyNames(m_pcSubKeyCpp, rlSubKeys) ;

			if( iResult != M4_ERROR )
			{
				// Se itera por los elementos 
				itIterator = rlSubKeys.begin() ;

				while( itIterator != rlSubKeys.end() )
				{
					if ( dwIndex == ai_nIndex)
					{

						sName = *itIterator ;
						sNameANSI = M4CppToANSI (sName, iLength);
						strcpy(ao_sName, sNameANSI) ;

						delete[] sNameANSI ;

						bFound = TRUE;
						break;
					}
					itIterator++ ;
					dwIndex++ ;
				}

				_ClearList(rlSubKeys);
			}

		}
		else
		{

			lResult = 0; // Inicializamos con ERROR_SUCCESS
			ntemp = sizeof(ctemp);
			ntemp2 = sizeof(ctemp2);
			
			lResult = RegEnumKeyEx(m_openhKey, ai_nIndex, &ctemp[0], &ntemp, NULL, &ctemp2[0], &ntemp2, &ftemp);
			if (lResult == ERROR_SUCCESS)
			{
				strcpy(ao_sName, ctemp);
				bFound = TRUE;
			}
		}
	}

	return bFound;
}


//##ModelId=396B3298004D
long CMyRegKey::GetNumberOfValues()
{

	DWORD	dwIndex;
	DWORD	dwType;
	DWORD	cbValueName ;
	DWORD	t1;
	char	ValueName[M4_INI_REGISTRY_MAX_CHAR + 1 ];
	long	lResult = 0;
	m4return_t	iResult = M4_ERROR ;
	list<m4pchar_t>		rlValues;

	if (m_bMain && m_bSub && m_bOpen)
	{
		dwIndex = 0;

		if (m_bAccessBym4ini)
		{
			iResult = M4XmlRegistry::Instance()->GetValueNames(m_pcSubKeyCpp, rlValues) ;
			
			if( iResult != M4_ERROR )
			{
				dwIndex = (DWORD) rlValues.size();
				_ClearList(rlValues);

				return (dwIndex);
			}
		}
		else
		{
			//Acceso por api de windows al registro
			lResult = 1000;
			t1 = cbValueName = sizeof(ValueName);
					
			while ((lResult = RegEnumValue(m_openhKey, dwIndex, ValueName, &cbValueName, NULL, &dwType, NULL, NULL))== ERROR_SUCCESS)
			{
				cbValueName = t1;
				dwIndex++;
			}	

			return dwIndex;
		}
	}
	
	return -1;

}

//##ModelId=396B32980054
BOOL CMyRegKey::GetValueName(long ai_nIndex, LPTSTR ao_sName)
{
	
	DWORD	cbValueName = 0;
	char	ValueName[M4_INI_REGISTRY_MAX_CHAR + 1];
	long	lResult = 0;
	DWORD	dwType = 0;
	m4pchar_t	sName = NULL;
	LPTSTR		sNameANSI = NULL;
	DWORD		dwIndex = 0;
	m4return_t	iResult = M4_ERROR ;
	int iLength = -1 ;
	list<m4pchar_t>				rlValues ;
	list<m4pchar_t>::iterator	itIterator ;
	BOOL bFound = FALSE; 

	strcpy(ao_sName, "NOT_DEFINED");

	if (m_bMain && m_bSub && m_bOpen)
	{
		if (m_bAccessBym4ini)
		{
			iResult = M4XmlRegistry::Instance()->GetValueNames(m_pcSubKeyCpp, rlValues) ;
			
			if( iResult != M4_ERROR )
			{
				// Se itera por los elementos limpiando
				itIterator = rlValues.begin() ;

				while( itIterator != rlValues.end() )
				{
					if ( dwIndex == ai_nIndex)
					{
						sName = *itIterator ;
						sNameANSI = M4CppToANSI (sName, iLength);
						strcpy(ao_sName, sNameANSI) ;

						delete[] sNameANSI ;

						bFound = TRUE;
						break;
					}
					
					itIterator++ ;
					dwIndex++;
				}
			}
		}
		else
		{
			//Acceso por api de windows al registro
			lResult = 0; // Iniciamos con ERROR_SUCCESS
			cbValueName = sizeof(ValueName);
					
			lResult = RegEnumValue(m_openhKey, ai_nIndex, ValueName, &cbValueName, NULL, &dwType, NULL, NULL);
			if (lResult  == ERROR_SUCCESS)
			{
				strcpy(ao_sName, ValueName);
				bFound = TRUE;
			}
		}
	}

	return bFound;
	
}

//##ModelId=396B3298005E
BOOL CMyRegKey::GetValue(LPTSTR ai_sValueName, VARIANT *ao_vValue)
{

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pcValueName = NULL ;
	m4pchar_t	pcData = NULL ;
	BSTR		cData = NULL;
	int iLength = -1 ;

	if (m_bMain && m_bSub && m_bOpen)
	{
		if (m_bAccessBym4ini)
		{	
			iLength = -1; 
			pcValueName = M4ANSIToCpp(ai_sValueName, iLength);

			//Leemos a través de la m4ini							
			iResult = M4XmlRegistry::Instance()->GetValue( m_pcSubKeyCpp, pcValueName, pcData ) ;

			delete [] pcValueName ;
			
			if( iResult != M4_ERROR )
			{
				cData = M4CppToVB( pcData ) ;

				ao_vValue->bstrVal =  cData ;
				ao_vValue->vt = VT_BSTR;

				delete [] pcData ;
			}

			return TRUE;
		}

		//Acceso por api de windows al registro
		//Leemos del registro
		DWORD	ValueType, DataBufferSize;
		BYTE	DataBuffer[512];
		DWORD	tempValue;
		long	lResult;
		
		DataBufferSize = 512;
		lResult = RegQueryValueEx(m_openhKey, ai_sValueName, NULL, &ValueType, DataBuffer, &DataBufferSize);
		if ( lResult == ERROR_SUCCESS)
		{
			switch (ValueType)
			{
				case REG_DWORD:
				//case REG_DWORD_LITTLE_ENDIAN:
				
					tempValue = DataBuffer[3];
					tempValue <<= 8;
					tempValue += DataBuffer[2];
					tempValue <<= 8;
					tempValue += DataBuffer[1];
					tempValue <<= 8;
					tempValue += DataBuffer[0];
					ao_vValue->lVal = tempValue;
					ao_vValue->vt = VT_I4;
					return TRUE;
					break;

				case REG_SZ:
					
					
					cData = M4CppToVB( (char*) DataBuffer ) ;

					ao_vValue->bstrVal = cData ;	
					ao_vValue->vt = VT_BSTR ;

					return TRUE;
					break;

				case REG_DWORD_BIG_ENDIAN:
				case REG_BINARY:
					return TRUE;
					break;

			}

		}
		else
			VariantClear(ao_vValue);
		return S_OK;
	}
	return FALSE;
}

//##ModelId=396B32980072
BOOL CMyRegKey::SetValueLong(LPSTR ai_sValueName, long ai_lValue)
{
	m4return_t	iResult = M4_ERROR ;
	char charValue[250 + 1];
	m4pchar_t pcValue = NULL ;
	m4pchar_t pcValueName = NULL ;
	int iLength = -1; 
	
	if (m_bMain && m_bSub && m_bOpen)
	{
		if (m_bAccessBym4ini)
		{	
			sprintf (charValue, "%d", ai_lValue) ;

			iLength = -1; 
			pcValueName = M4ANSIToCpp(ai_sValueName, iLength);
			
			iLength = -1; 
			pcValue = M4ANSIToCpp(charValue, iLength);

			//Escribimos a través de la m4ini
			iResult = M4XmlRegistry::Instance()->SetValue( m_pcSubKeyCpp, pcValueName, pcValue ) ;
			
			delete[] pcValueName;
			delete[] pcValue;

			if( iResult != M4_ERROR )
			{
				//Grabamos el fichero.
				if (M4XmlRegistry::Instance()->Save(NULL)!= M4_ERROR)
				{
					return TRUE; 
				}
			}
		}
		else
		{
			//Acceso por api de windows al registro
			if (RegSetValueEx(m_openhKey, ai_sValueName, NULL, REG_DWORD, (const unsigned char*) &ai_lValue, sizeof(long)) == ERROR_SUCCESS)
			{
				return TRUE;
			}
		}
	}
	return FALSE;

}


//##ModelId=396B32980068
BOOL CMyRegKey::SetValueString(LPSTR ai_sValueName, LPSTR ai_sValue)
{
	
	long lStrLong = -1;
	m4return_t	iResult = M4_ERROR ;
	m4pchar_t pcValue = NULL ;
	m4pchar_t pcValueName = NULL ;
	int iLength = -1; 
		
	if (m_bMain && m_bSub && m_bOpen)
	{
		if (m_bAccessBym4ini)
		{	
			iLength = -1; 
			pcValueName = M4ANSIToCpp(ai_sValueName, iLength);
			iLength = -1; 
			pcValue = M4ANSIToCpp(ai_sValue, iLength);

			//Escribimos a través de la m4ini
			iResult = M4XmlRegistry::Instance()->SetValue( m_pcSubKeyCpp, pcValueName, pcValue ) ;
			
			delete[] pcValueName;
			delete[] pcValue;

			if( iResult != M4_ERROR )
			{
				//Grabamos el fichero.
				if (M4XmlRegistry::Instance()->Save(NULL)!= M4_ERROR)
				{
					return TRUE; 
				}
			}
		}
		else
		{
			//Acceso por api de windows al registro
			lStrLong = 0;
			//Contamos el número de caracteres
			while(ai_sValue[lStrLong++] !=0){}
			if (RegSetValueEx(m_openhKey, ai_sValueName, NULL, REG_SZ, (const unsigned char *)ai_sValue, lStrLong) == ERROR_SUCCESS)
			{
				return TRUE;
			}
		}
	}

	return FALSE;

}

//##ModelId=396B32980087
BOOL CMyRegKey::DeleteValue(LPSTR ai_sValueName)
{
	m4return_t	iResult = M4_ERROR ;
	m4pchar_t pcValueName = NULL ;
	int iLength = -1; 

	//En modo m4ini
	if (m_bAccessBym4ini)
	{
		iLength = -1; 
		pcValueName = M4ANSIToCpp(ai_sValueName, iLength);

		//Borramos a través de la m4ini
		iResult = M4XmlRegistry::Instance()->DeleteValue( m_pcSubKeyCpp, pcValueName ) ;
		
		delete[] pcValueName;

		if( iResult != M4_ERROR )
		{
			//Grabamos el fichero.
			if (M4XmlRegistry::Instance()->Save(NULL)!= M4_ERROR)
			{
				return TRUE; 
			}
		}
	}

	//Acceso por api de windows al registro
	// no implementadas
	if (RegDeleteValue(m_openhKey, ai_sValueName) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	return FALSE;
}

//##ModelId=396B3298007E
BOOL CMyRegKey::DeleteKey(LPSTR ai_sKeyName)
{

	m4pchar_t pcKeyName = NULL;
	int iLength = -1 ; 

	char	pcKey[M4_INI_REGISTRY_MAX_CHAR + 1];

	//En modo m4ini
	if (m_bAccessBym4ini)
	{
		iLength = -1; 
		pcKeyName = M4ANSIToCpp(ai_sKeyName, iLength);

		sprintf( pcKey, "%s%s", m_pcSubKeyCpp, pcKeyName ) ;
		
		M4XmlRegistry::Instance()->DeleteKey( pcKey) ;

		delete [] pcKeyName ;

		//Grabamos el fichero.
		if (M4XmlRegistry::Instance()->Save(NULL)!= M4_ERROR)
		{
			return TRUE; 
		}
	}

	if (DeleteKeyRec(m_openhKey, ai_sKeyName) == ERROR_SUCCESS)
	{
		// Por si acaso
		CloseKey();
		OpenKey();
		return TRUE;
	}

	CloseKey();
	OpenKey();
	return FALSE;

}


//##ModelId=396B329800A4
DWORD CMyRegKey::DeleteKeyRec(HKEY ai_hStartKey, LPTSTR ai_sKeyName)
{

	DWORD dwRtn, dwSubKeyLength;
	LPTSTR pSubKey = NULL;
	TCHAR	szSubKey[M4_INI_REGISTRY_MAX_CHAR + 1];
	HKEY	hKey;

	//En modo m4ini
	if (m_bAccessBym4ini)
	{
		return ERROR_SUCCESS ;
	}

	//Acceso por api de windows al registro
	if (ai_sKeyName && lstrlen(ai_sKeyName))
	{
		if ((dwRtn=RegOpenKeyEx(ai_hStartKey, ai_sKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hKey)) == ERROR_SUCCESS)
		{
			while(dwRtn == ERROR_SUCCESS)
			{
				dwSubKeyLength = M4_INI_REGISTRY_MAX_CHAR + 1;
				dwRtn = RegEnumKeyEx(
					hKey,
					0,
					szSubKey,
					&dwSubKeyLength,
					NULL,
					NULL,
					NULL,
					NULL
					);
				if (dwRtn == ERROR_NO_MORE_ITEMS)
				{
					dwRtn = RegDeleteKey(ai_hStartKey, ai_sKeyName);
					break;
				}
				else if(dwRtn == ERROR_SUCCESS)
					dwRtn = DeleteKeyRec(hKey, szSubKey);
			}
			RegCloseKey(hKey);
		}
	}
	else
		dwRtn = ERROR_BADKEY;
	return dwRtn;
	

}


//##ModelId=396B32980036
BOOL CMyRegKey::IsValidPosition()
{

	return (m_bMain && m_bSub && m_bOpen);

}

//##ModelId=396B3298009A
BOOL CMyRegKey::CharCompare(LPCTSTR ai_sFirst, LPCTSTR ai_sSecond)
{
	long	Contador;

	Contador = 0;
	while (ai_sFirst[Contador] == ai_sSecond[Contador])
	{
		// Incrementamos y Si hemos llegado al final, nos vamos
		if (ai_sFirst[Contador++] == 0)
			return TRUE;
	}

	return FALSE;

}