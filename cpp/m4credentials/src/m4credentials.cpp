//## begin module%3959D0DA003A.cm preserve=no
//## end module%3959D0DA003A.cm

//## begin module%3959D0DA003A.cp preserve=no
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
//## end module%3959D0DA003A.cp

//## Module: m4credentials%3959D0DA003A; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: F:\integration\m4credentials\src\m4credentials.cpp

//## begin module%3959D0DA003A.additionalIncludes preserve=no
//## end module%3959D0DA003A.additionalIncludes

//## begin module%3959D0DA003A.includes preserve=yes
//Body Include
#include "stringutils.hpp"
//## end module%3959D0DA003A.includes

// m4string
#include <m4string.hpp>
// m4credentials
#include <m4credentials.hpp>
//## begin module%3959D0DA003A.declarations preserve=no
//## end module%3959D0DA003A.declarations

//## begin module%3959D0DA003A.additionalDeclarations preserve=yes
//## end module%3959D0DA003A.additionalDeclarations


// Class M4Credentials 



//## begin M4Credentials::M4CredTokenSeparator%396C7F3500FD.attr preserve=no  public: static m4char_t {VAC} 1
const m4char_t  M4Credentials::M4CredTokenSeparator = 1;
//## end M4Credentials::M4CredTokenSeparator%396C7F3500FD.attr

//## begin M4Credentials::M4CredEndSeparator%396C7F800187.attr preserve=no  public: static m4char_t {UAC} 2
const m4char_t  M4Credentials::M4CredEndSeparator = 2;
//## end M4Credentials::M4CredEndSeparator%396C7F800187.attr



M4Credentials::M4Credentials ()
  //## begin M4Credentials::M4Credentials%962187518.hasinit preserve=no
  //## end M4Credentials::M4Credentials%962187518.hasinit
  //## begin M4Credentials::M4Credentials%962187518.initialization preserve=yes
  //## end M4Credentials::M4Credentials%962187518.initialization
{
  //## begin M4Credentials::M4Credentials%962187518.body preserve=yes
  //## end M4Credentials::M4Credentials%962187518.body
}

M4Credentials::M4Credentials (const m4string_t &ai_strLogin, const TRoleInfoList &ai_oRoleList, const m4string_t &ai_strSessionID)
  //## begin M4Credentials::M4Credentials%962187519.hasinit preserve=no
  //## end M4Credentials::M4Credentials%962187519.hasinit
  //## begin M4Credentials::M4Credentials%962187519.initialization preserve=yes
  : m_strLogin(ai_strLogin), m_strSessionID(ai_strSessionID)
  //## end M4Credentials::M4Credentials%962187519.initialization
{
  //## begin M4Credentials::M4Credentials%962187519.body preserve=yes
	TRoleInfoListIterator	oIterator;
	for (
			oIterator =  ai_oRoleList.begin();
			oIterator != ai_oRoleList.end();
			oIterator++
		)
			{
				m_oRoleInfoList.push_front(*oIterator);

			}
  //## end M4Credentials::M4Credentials%962187519.body
}

M4Credentials::M4Credentials (const M4Credentials &ai_oCredentials)
  //## begin M4Credentials::M4Credentials%962266646.hasinit preserve=no
  //## end M4Credentials::M4Credentials%962266646.hasinit
  //## begin M4Credentials::M4Credentials%962266646.initialization preserve=yes
  :	m_strLogin(ai_oCredentials.m_strLogin), m_strSessionID(ai_oCredentials.m_strSessionID)
  //## end M4Credentials::M4Credentials%962266646.initialization
{
  //## begin M4Credentials::M4Credentials%962266646.body preserve=yes
	TRoleInfoListIterator	oIterator;
	for (
			oIterator =  ai_oCredentials.m_oRoleInfoList.begin();
			oIterator != ai_oCredentials.m_oRoleInfoList.end();
			oIterator++
		)
			{
				m_oRoleInfoList.push_front(*oIterator);

			}
  //## end M4Credentials::M4Credentials%962266646.body
}


M4Credentials::~M4Credentials ()
{
  //## begin M4Credentials::~M4Credentials%962187530.body preserve=yes
	m_oRoleInfoList.clear();
  //## end M4Credentials::~M4Credentials%962187530.body
}



//## Other Operations (implementation)
m4return_t M4Credentials::Serialize (m4string_t &ao_strBuffer) const
{
  //## begin M4Credentials::Serialize%962187520.body preserve=yes
	ao_strBuffer  += m_strLogin;
	ao_strBuffer  += M4CredTokenSeparator;
	ao_strBuffer  += m_strSessionID;
	ao_strBuffer  += M4CredTokenSeparator;
	TRoleInfoListIterator	oIterator;
	for (
			oIterator =  m_oRoleInfoList.begin();
			oIterator != m_oRoleInfoList.end();
			oIterator++
		)
			{
				ao_strBuffer  += (*oIterator).m_strRoleID;
				ao_strBuffer  += M4CredTokenSeparator;
				ao_strBuffer  += (*oIterator).m_strRSMID;
				ao_strBuffer  += M4CredEndSeparator;
			}

	return M4_SUCCESS;
  //## end M4Credentials::Serialize%962187520.body
}

M4Credentials * M4Credentials::Deserialize (const m4string_t &ai_strBuffer)
{
  //## begin M4Credentials::Deserialize%962187521.body preserve=yes
	m4string_t				strToken;
	m4string_t::size_type	iStartPos = 0;

	m4return_t				iRet;
	m4string_t				strLogin;

	m4string_t::size_type	iTotalLength;
	iTotalLength = ai_strBuffer.find(M4CredEndSeparator);

	iRet = TStringUtils::GetToken(ai_strBuffer, iStartPos, iTotalLength, M4CredTokenSeparator, strLogin);
	if (M4_SUCCESS != iRet)
		return 0;

	m4string_t				strSessionID;
	iRet = TStringUtils::GetToken(ai_strBuffer, iStartPos, iTotalLength, M4CredTokenSeparator, strSessionID);
	if (M4_SUCCESS != iRet)
		return 0;

	TRoleInfoList	oRoleInfoList;
	do
	{
		m4string_t				strRoleID;
		iRet = TStringUtils::GetToken(ai_strBuffer, iStartPos, iTotalLength, M4CredTokenSeparator, strRoleID);
		if (M4_SUCCESS != iRet)
			return 0;

		m4string_t				strRSMID;
		iRet = TStringUtils::GetToken(ai_strBuffer, iStartPos, iTotalLength, M4CredTokenSeparator, strRSMID);
	}
	while (M4_SUCCESS == iRet); 

	M4Credentials		*poCredentials = new M4Credentials(strLogin, oRoleInfoList, strSessionID);
	return poCredentials;
  //## end M4Credentials::Deserialize%962187521.body
}

m4bool_t M4Credentials::IsUserInRole (const m4string_t &ai_strRoleID) const
{
  //## begin M4Credentials::IsUserInRole%963823177.body preserve=yes
	TRoleInfoListIterator	oIterator;
	for (
			oIterator =  m_oRoleInfoList.begin();
			oIterator != m_oRoleInfoList.end();
			oIterator++
		)
			{
				if (ai_strRoleID == (*oIterator).m_strRoleID)
					return M4_TRUE;
			}

	return M4_FALSE;
  //## end M4Credentials::IsUserInRole%963823177.body
}

m4bool_t M4Credentials::IsUserInRSM (const m4string_t &ai_strRSMID) const
{
  //## begin M4Credentials::IsUserInRSM%963823178.body preserve=yes
	TRoleInfoListIterator	oIterator;
	for (
			oIterator =  m_oRoleInfoList.begin();
			oIterator != m_oRoleInfoList.end();
			oIterator++
		)
			{
				if (ai_strRSMID == (*oIterator).m_strRSMID)
					return M4_TRUE;
			}

	return M4_FALSE;
  //## end M4Credentials::IsUserInRSM%963823178.body
}

m4uint64_t M4Credentials::GetSessId ()
{
  //## begin M4Credentials::GetSessId%985334637.body preserve=yes
	return m_lSessId ;
  //## end M4Credentials::GetSessId%985334637.body
}

void M4Credentials::SetSessId (m4uint64_t ai_lSessId)
{
  //## begin M4Credentials::SetSessId%985334638.body preserve=yes
	m_lSessId = ai_lSessId ;
  //## end M4Credentials::SetSessId%985334638.body
}

// Additional Declarations
  //## begin M4Credentials%3959C68E0328.declarations preserve=yes
  //## end M4Credentials%3959C68E0328.declarations

// Class ClRoleInfo 



ClRoleInfo::ClRoleInfo (const m4string_t &ai_strRoleID, const m4string_t &ai_strRSMID)
  //## begin ClRoleInfo::ClRoleInfo%962187522.hasinit preserve=no
  //## end ClRoleInfo::ClRoleInfo%962187522.hasinit
  //## begin ClRoleInfo::ClRoleInfo%962187522.initialization preserve=yes
  : m_strRoleID(ai_strRoleID), m_strRSMID(ai_strRSMID)
  //## end ClRoleInfo::ClRoleInfo%962187522.initialization
{
  //## begin ClRoleInfo::ClRoleInfo%962187522.body preserve=yes
  //## end ClRoleInfo::ClRoleInfo%962187522.body
}

ClRoleInfo::ClRoleInfo (const ClRoleInfo &ai_oRoleInfo)
  //## begin ClRoleInfo::ClRoleInfo%962187528.hasinit preserve=no
  //## end ClRoleInfo::ClRoleInfo%962187528.hasinit
  //## begin ClRoleInfo::ClRoleInfo%962187528.initialization preserve=yes
  : m_strRoleID(ai_oRoleInfo.m_strRoleID), m_strRSMID(ai_oRoleInfo.m_strRSMID)
  //## end ClRoleInfo::ClRoleInfo%962187528.initialization
{
  //## begin ClRoleInfo::ClRoleInfo%962187528.body preserve=yes
  //## end ClRoleInfo::ClRoleInfo%962187528.body
}

ClRoleInfo::ClRoleInfo ()
  //## begin ClRoleInfo::ClRoleInfo%962187523.hasinit preserve=no
  //## end ClRoleInfo::ClRoleInfo%962187523.hasinit
  //## begin ClRoleInfo::ClRoleInfo%962187523.initialization preserve=yes
  : m_strRoleID(""), m_strRSMID("")
  //## end ClRoleInfo::ClRoleInfo%962187523.initialization
{
  //## begin ClRoleInfo::ClRoleInfo%962187523.body preserve=yes
  //## end ClRoleInfo::ClRoleInfo%962187523.body
}


ClRoleInfo::~ClRoleInfo ()
{
  //## begin ClRoleInfo::~ClRoleInfo%962187524.body preserve=yes
  //## end ClRoleInfo::~ClRoleInfo%962187524.body
}



//## Other Operations (implementation)
ClRoleInfo & ClRoleInfo::operator = (const ClRoleInfo &ai_oRHSRoleInfo)
{
  //## begin ClRoleInfo::operator=%962187529.body preserve=yes
	return *this;
  //## end ClRoleInfo::operator=%962187529.body
}

// Additional Declarations
  //## begin ClRoleInfo%3959C6BA0368.declarations preserve=yes
  //## end ClRoleInfo%3959C6BA0368.declarations

//## begin module%3959D0DA003A.epilog preserve=yes
//## end module%3959D0DA003A.epilog
