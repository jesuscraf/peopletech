//## begin module%396C9BA3029D.cm preserve=no
//## end module%396C9BA3029D.cm

//## begin module%396C9BA3029D.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%396C9BA3029D.cp

//## Module: StringUtils%396C9BA3029D; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: D:\eduardoma\m4credentials\src\stringutils.cpp

//## begin module%396C9BA3029D.additionalIncludes preserve=no
//## end module%396C9BA3029D.additionalIncludes

//## begin module%396C9BA3029D.includes preserve=yes
#include "m4cryptc.hpp"
#include "stringutils.hpp"
#include "m4stl.hpp"
//## end module%396C9BA3029D.includes

// StringUtils
#include <stringutils.hpp>
// m4string
#include <m4string.hpp>
//## begin module%396C9BA3029D.declarations preserve=no
//## end module%396C9BA3029D.declarations

//## begin module%396C9BA3029D.additionalDeclarations preserve=yes
//## end module%396C9BA3029D.additionalDeclarations


// Class Utility TStringUtils 


//## Other Operations (implementation)
m4return_t TStringUtils::GetToken (const m4string_t &ai_strRawString, m4string_t::size_type &aio_iStartPos, const m4string_t::size_type &ai_iEndPos, m4char_t ai_cSeparator, m4string_t &ao_strToken)
{
  //## begin TStringUtils::GetToken%963485957.body preserve=yes
	m4string_t::size_type	iOffset;

	iOffset = ai_strRawString.find(ai_cSeparator, aio_iStartPos);
	if (ai_iEndPos < iOffset)
		return M4_ERROR;

	//
	// Note: The next two code lines must not be used because it is not
	//		compile on Unix (the problem is to use "npos"). So, the other
	//		next two lines are used. In those lines check if the
	//		"ai_cSeparator" was not found in the string "ai_strRawString"
	//		since the position "aio_iStartPos"
	//
	// if (ai_strRawString.npos == iOffset)
	//    iOffset = ai_iEndPos;

	if (iOffset <= aio_iStartPos || iOffset >= ai_strRawString.length())
		iOffset = ai_iEndPos;

	ao_strToken.assign(ai_strRawString, aio_iStartPos, iOffset-aio_iStartPos);
	aio_iStartPos = iOffset + 1; 

	return M4_SUCCESS;
  //## end TStringUtils::GetToken%963485957.body
}

m4return_t TStringUtils::Cipher (const m4string_t &ai_strRawString, m4string_t &ao_strCipheredString, const m4string_t &ai_strKey, m4uint_t ai_iPasses)
{
  //## begin TStringUtils::Cipher%963485958.body preserve=yes	
	m4return_t	iRet = M4_SUCCESS;
	size_t		iLength = 0;
	m4int_t		iTotalLength = 0;
	m4pchar_t	pszCipheredBuffer = NULL;
	m4pchar_t	pszRawBuffer = NULL;
	
	// Inicializaciones.
	ao_strCipheredString = "";

	if (ai_iPasses == 0)
	{
		return M4_SUCCESS;
	}

	iLength = ai_strRawString.size();
	pszRawBuffer = new m4char_t[iLength + 1];
	ai_strRawString.copy(pszRawBuffer, iLength);
	pszRawBuffer[iLength] = '\0';
	
	for (m4uint_t i = 0; i < ai_iPasses; i++)
	{
		iRet = EncryptTwoWay(pszRawBuffer, (m4int_t) strlen(pszRawBuffer), ai_strKey.c_str(), (m4int_t) ai_strKey.size(), pszCipheredBuffer, iTotalLength);
		if (iRet != M4_SUCCESS)
		{
			delete pszRawBuffer;
			return iRet;
		}

		delete pszRawBuffer;
		pszRawBuffer = NULL;

		pszRawBuffer = new m4char_t[iTotalLength + 1];
		memcpy(pszRawBuffer, pszCipheredBuffer, iTotalLength + 1);

		delete pszCipheredBuffer;
		pszCipheredBuffer = NULL;
	}

	ao_strCipheredString.assign(pszRawBuffer, iTotalLength + 1);

	delete pszRawBuffer;
	
	return M4_SUCCESS;
  //## end TStringUtils::Cipher%963485958.body
}

m4return_t TStringUtils::Decipher (const m4string_t &ai_strRawString, m4string_t &ao_strDecipheredString, const m4string_t &ai_strKey, m4uint_t ai_iPasses)
{
  //## begin TStringUtils::Decipher%963485959.body preserve=yes
	m4return_t	iRet = M4_SUCCESS;
	size_t		iLength = 0;
	m4int_t		iTotalLength = 0;
	m4pchar_t	pszCipheredBuffer = NULL;
	m4pchar_t	pszRawBuffer = NULL;
	
	// Inicializaciones.
	ao_strDecipheredString = "";

	if (ai_iPasses == 0)
	{
		return M4_SUCCESS;
	}

	iLength = ai_strRawString.size();
	pszCipheredBuffer = new m4char_t[iLength + 1];
	ai_strRawString.copy(pszCipheredBuffer, iLength);
	pszCipheredBuffer[iLength] = '\0';
	
	for (m4uint_t i = 0; i < ai_iPasses; i++)
	{
		iRet = DecryptTwoWay(pszCipheredBuffer, (m4int_t) strlen(pszCipheredBuffer), ai_strKey.c_str(), (m4int_t) ai_strKey.size(),	pszRawBuffer, iTotalLength);

		if (iRet != M4_SUCCESS)
		{
			delete pszCipheredBuffer;
			return iRet;
		}

		delete pszCipheredBuffer;
		pszCipheredBuffer = NULL;

		pszCipheredBuffer = new m4char_t[iTotalLength + 1];
		memcpy(pszCipheredBuffer, pszRawBuffer, iTotalLength + 1);

		delete pszRawBuffer;
		pszRawBuffer = NULL;
	}

	ao_strDecipheredString.assign(pszCipheredBuffer, iTotalLength + 1);

	delete pszCipheredBuffer;
	
	return M4_SUCCESS;
  //## end TStringUtils::Decipher%963485959.body
}

m4return_t TStringUtils::GetDigest (const m4string_t &ai_strRawString, m4string_t &ao_strDigest)
{
  //## begin TStringUtils::GetDigest%963485960.body preserve=yes
	m4char_t		strBuffer [20]; //Tamaño suficiente para un entero 

	m4string_t::const_iterator	oIterator;
	m4uint_t					iCheckSum = 0,
								iPosition = 0;
	for (
			oIterator = ai_strRawString.begin();
			oIterator!= ai_strRawString.end();
			oIterator++
		)
		{
			iPosition++;
			iCheckSum += iPosition * (m4uint_t)*oIterator;
		}

	sprintf ( strBuffer , "%d" , iCheckSum ) ;;
	ao_strDigest = strBuffer;

	return M4_SUCCESS;
  //## end TStringUtils::GetDigest%963485960.body
}

m4return_t TStringUtils::CheckDigest (const m4string_t &ai_strRawString, const m4string_t &ai_strDigest)
{
  //## begin TStringUtils::CheckDigest%963485961.body preserve=yes
	m4string_t		strDigest;
	m4return_t		iChecksumResult;
	iChecksumResult = GetDigest(ai_strRawString, strDigest); 
	if (ai_strDigest != strDigest)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end TStringUtils::CheckDigest%963485961.body
}

//## begin module%396C9BA3029D.epilog preserve=yes
//## end module%396C9BA3029D.epilog
