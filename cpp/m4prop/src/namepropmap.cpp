// NamePropMap.cpp: implementation of the CNamePropMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "namepropmap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNamePropMap::CNamePropMap() :
	m_iNumElm(0)
{
}

CNamePropMap::~CNamePropMap()
{
	// frees map keys
	{
		itBSTRToElm it = m_oBSTRMap.begin();
		while (it != m_oBSTRMap.end())
		{
			if ((*it).first.key != NULL)
			{
				::SysFreeString((*it).first.key);
			}
			
			++it;
		}
	}

	{
		itCharToElm it = m_oCharMap.begin();
		while (it != m_oCharMap.end())
		{
			if ((*it).first.key != NULL)
			{
				delete (*it).first.key;
			}

			++it;
		}
	}
}

int CNamePropMap::GetCodeFromBSTR(BSTR ai_strPropName)
{
	itBSTRToElm it = m_oBSTRMap.find(ai_strPropName);

	if (it != m_oBSTRMap.end())
		return (*it).second.iCode;

	return addElm(ai_strPropName).iCode;
}

int CNamePropMap::GetCodeFromChar(char* ai_strPropName)
{
	itCharToElm it = m_oCharMap.find(ai_strPropName);

	if (it != m_oCharMap.end())
		return (*it).second.iCode;

	return addElm(ai_strPropName).iCode;
}

BSTR CNamePropMap::GetBSTRFromBSTR(BSTR ai_strPropName)
{
	itBSTRToElm it = m_oBSTRMap.find(ai_strPropName);

	if (it != m_oBSTRMap.end())
		return (*it).second.bstr;

	// adds the key
	return addElm(ai_strPropName).bstr;
}

BSTR CNamePropMap::GetBSTRFromChar(char* ai_strPropName)
{
	itCharToElm it = m_oCharMap.find(ai_strPropName);

	if (it != m_oCharMap.end())
		return (*it).second.bstr;

	return addElm(ai_strPropName).bstr;
}

BSTR CNamePropMap::GetBSTRFromCode(int iCode)
{
	// no recreamos el BSTR asi que mucho ojito con borrarlo

	if (iCode < 0 || iCode >= m_iNumElm)
		return NULL;

	return m_oBSTRVector[iCode];
}

CNamePropMap::CPropElm CNamePropMap::addElm(BSTR ai_bstrPropName)
{
	// generates a new string
	char* pcKey = M4VBToCpp(ai_bstrPropName);
	BSTR bstrKey = CloneBSTR(ai_bstrPropName);

	// creates a new element
	return addElm(bstrKey, pcKey);
}

CNamePropMap::CPropElm CNamePropMap::addElm(char* ai_pcPropName)
{

	// generates a new string
	char* pcKey = CloneChar(ai_pcPropName);
	BSTR bstrKey = M4CppToVB(ai_pcPropName);

	// creates a new element
	return addElm(bstrKey, pcKey);
}

CNamePropMap::CPropElm CNamePropMap::addElm(BSTR ai_bstrKey, char* ai_pcKey)
{
	CPropElm oElm(ai_bstrKey,m_iNumElm++);

	// updates maps and vectors
	m_oBSTRMap.insert(BSTRToElmMap::value_type(ai_bstrKey,oElm));
	m_oCharMap.insert(CharToElmMap::value_type(ai_pcKey,oElm));
	m_oBSTRVector.push_back(ai_bstrKey);

	return oElm;
}

BSTR CNamePropMap::CloneBSTR(const BSTR& ai_wcStr)
{
	return ::SysAllocString(ai_wcStr);
}

char * CNamePropMap::CloneChar(const char* ai_pcChar)
{
	char* pcNew = new char[strlen(ai_pcChar)+1];
	strcpy(pcNew,ai_pcChar);
	return pcNew;
}


