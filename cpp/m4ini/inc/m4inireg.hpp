//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini
// File:                m4inireg.hpp   
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


#ifndef __MYREGKEY_HPP__
#define __MYREGKEY_HPP__


#include <windows.h>
#include "m4ini.hpp"

#define		M4_INI_REGISTRY_MAX_CHAR	1024



//##ModelId=396B329703C4
class M4_DECL_M4INI CMyRegKey
{

protected:
	
	//##ModelId=396B32980010
	VARIANT		m_varValue;
	//##ModelId=396B32980004
	HKEY		m_hKey;
	//##ModelId=396B329703E3
	HKEY		m_openhKey;
	//##ModelId=396B329703E2
	char		m_strSubKeyAnsi[M4_INI_REGISTRY_MAX_CHAR + 1];

	m4pchar_t	m_pcSubKeyCpp;

	//##ModelId=396B329703D8
	BOOL		m_bMain;
	//##ModelId=396B329703CF
	BOOL		m_bSub;
	//##ModelId=396B329703CE
	BOOL		m_bOpen;

	//TRUE si estámos accediendo a la rama HKEY_LOCAL_MACHINE
	BOOL		m_bLocalMachine ;
	//TRUE si accedemos al api de la m4ini
	//FALSE si accedemos al api del registro de windows
	BOOL		m_bAccessBym4ini;

	//##ModelId=396B3298009A
	BOOL CharCompare(LPCTSTR cFirst, LPCTSTR cSecond);

	//##ModelId=396B329800A4
	DWORD DeleteKeyRec(HKEY hStartKey,LPTSTR pKeyName);

public:
	//##ModelId=396B32980091
	BOOL OpenKeyEx(M4RegKeyMode openMode);
	//##ModelId=396B32980090
	BOOL CreateKey();
	//##ModelId=396B32980087
	BOOL DeleteValue(LPSTR strValueName);
	//##ModelId=396B3298007E
	BOOL DeleteKey(LPSTR strKeyName);
	//##ModelId=396B32980072
	BOOL SetValueLong(LPSTR lpstrValueName, long lValue);
	//##ModelId=396B32980068
	BOOL SetValueString(LPSTR lpstrValueName, LPSTR lpstrValue);
	//##ModelId=396B3298005E
	BOOL GetValue(LPTSTR ValueName, VARIANT *ValueSet);
	//##ModelId=396B32980054
	BOOL GetValueName(long nIndex, LPTSTR strName);
	//##ModelId=396B3298004D
	long GetNumberOfValues();
	//##ModelId=396B3298004A
	BOOL GetSubKeyName(long nIndex, LPSTR strName);
	//##ModelId=396B32980040
	void GetSubKey(LPSTR strSubKey);
	//##ModelId=396B32980037
	void GetMainKey(LPTSTR strMainKey);
	//##ModelId=396B32980036
	BOOL IsValidPosition();
	//##ModelId=396B3298002E
	long GetNumberOfKeys();
	//##ModelId=396B3298002D
	void CloseKey();
	//##ModelId=396B32980024
	void SetSubKey(LPCTSTR strSubKey);
	//##ModelId=396B32980022
	void SetMainKey(LPCTSTR strMainKey);
	//##ModelId=396B32980019
	 CMyRegKey();
	~CMyRegKey();
	//##ModelId=396B32980018
	BOOL OpenKey();

private:
//	void ConvertBSTR2char(BSTR bstrIn, char * charOut);

};




#endif
