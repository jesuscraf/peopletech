// NamePropMap.h: interface for the CNamePropMap class.
//
//////////////////////////////////////////////////////////////////////

// STL includes
#include "m4stl.hpp"

#if !defined(_CNAMEPROPMAP_)
#define _CNAMEPROPMAP_


class CNamePropMap  
{
public:
	// element class for maps
	class CPropElm
	{
	public:
		BSTR bstr;
		int iCode;
		CPropElm(BSTR ai_bstr, int ai_code)
			: bstr(ai_bstr), iCode(ai_code) {}
	};

	class CPropKeyChar
	{
	public:
		char* key;
		int len;

		CPropKeyChar(char* ai_key) 
			: key(ai_key) 
		{
			len = strlen(ai_key);
		}
		
		bool operator < (const CPropKeyChar &p) const
		{
			if (len != p.len)
				return len < p.len;
			return strcmp(key,p.key) < 0;
		}
	};

	class CPropKeyBSTR
	{
	public:
		BSTR key;
		int len;

		CPropKeyBSTR(BSTR ai_key) 
			: key(ai_key) 
		{
			len = wcslen(ai_key);
		}
		
		bool operator < (const CPropKeyBSTR &p) const
		{
			if (len != p.len)
				return len < p.len;
			return wcscmp(key,p.key) < 0;
		}
	};

	// definitions
	typedef map<CPropKeyChar, CPropElm, less<CPropKeyChar> > CharToElmMap;
	typedef CharToElmMap::iterator itCharToElm;

	typedef map<CPropKeyBSTR, CPropElm, less<CPropKeyBSTR> > BSTRToElmMap;
	typedef BSTRToElmMap::iterator itBSTRToElm;
	
	typedef vector<BSTR> BSTRVector;

	CNamePropMap();
	~CNamePropMap();

	// inquire methods
	BSTR GetBSTRFromCode (int iCode);
	
	int GetCodeFromBSTR(BSTR ai_bstrPropName);
	int GetCodeFromChar(char* ai_bstrPropName);

	BSTR GetBSTRFromBSTR(BSTR ai_bstrPropName);
	BSTR GetBSTRFromChar(char* ai_bstrPropName);

	// internal members only
protected:
	CharToElmMap m_oCharMap;
	BSTRToElmMap m_oBSTRMap;
	BSTRVector m_oBSTRVector;

	// to add
	CPropElm addElm(BSTR ai_bstrPropName);
	CPropElm addElm(char* ai_pcPropName);
	CPropElm addElm(BSTR ai_bstrKey, char* ai_pcKey);
	int m_iNumElm;

	// clone funcitions
	BSTR CNamePropMap::CloneBSTR(const BSTR& ai_wcStr);
	char * CloneChar(const char* ai_pcChar);
};

#endif // !defined(_CNAMEPROPMAP_)
