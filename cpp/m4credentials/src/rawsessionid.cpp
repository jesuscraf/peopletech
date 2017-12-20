//## begin module%39633C20039A.cm preserve=no
//## end module%39633C20039A.cm

//## begin module%39633C20039A.cp preserve=no
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
//## end module%39633C20039A.cp

//## Module: RawSessionID%39633C20039A; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: F:\integration\m4credentials\src\rawsessionid.cpp

//## begin module%39633C20039A.additionalIncludes preserve=no
//## end module%39633C20039A.additionalIncludes

//## begin module%39633C20039A.includes preserve=yes
#include <cldates.hpp>
//BODY INCLUDE
#include "stringutils.hpp"
#include "m4stl.hpp"
#include <clarraylibres.hpp>
//## end module%39633C20039A.includes

// RawSessionID
#include <rawsessionid.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>
//## begin module%39633C20039A.declarations preserve=no
//## end module%39633C20039A.declarations

//## begin module%39633C20039A.additionalDeclarations preserve=yes
//## end module%39633C20039A.additionalDeclarations


// Class ClRawSessionIDGenerator 

//## begin ClRawSessionIDGenerator::M4SessionIDFieldSeparator%396DF2A8034D.attr preserve=no  protected: static m4char_t {VAC} 3
const m4char_t  ClRawSessionIDGenerator::M4SessionIDFieldSeparator = 3;
//## end ClRawSessionIDGenerator::M4SessionIDFieldSeparator%396DF2A8034D.attr

//## begin ClRawSessionIDGenerator::M4SessionIDEndSeparator%396DF2D60335.attr preserve=no  protected: static m4char_t {UAC} 4
const m4char_t  ClRawSessionIDGenerator::M4SessionIDEndSeparator = 4;
//## end ClRawSessionIDGenerator::M4SessionIDEndSeparator%396DF2D60335.attr

ClRawSessionIDGenerator::ClRawSessionIDGenerator ()
  //## begin ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.hasinit preserve=no
  //## end ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.hasinit
  //## begin ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.initialization preserve=yes
  //## end ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.initialization
{
  //## begin ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.body preserve=yes
  //## end ClRawSessionIDGenerator::ClRawSessionIDGenerator%962964029.body
}



//## Other Operations (implementation)
m4return_t ClRawSessionIDGenerator::CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const
{
  //## begin ClRawSessionIDGenerator::CreateSessionID%962964030.body preserve=yes
	m4string_t		strSessionID;
	m4string_t		strHost ;
	m4string_t		strBasePort ;
	m4char_t		pszStringDate[128];
	m4char_t		pszSessId[10];
	m4pchar_t		pExpirationDate = 0;
	//m4uint32_t		iBasePort ;


	if (0 == ai_oSecondsFromNow)
	{
		pExpirationDate = M4CL_PLUS_INFINITE_DATE;
	}
	else
	{
		m4uint32_t uiDays, uiSeconds;
		ClActualDate( uiDays, uiSeconds) ;
		uiSeconds += ai_oSecondsFromNow;

		m4date_t	oExpirationDate;
		ClLongsToDate( uiDays, uiSeconds, oExpirationDate ) ;

		ClDateToString( oExpirationDate, pszStringDate, 128 ) ;
		pExpirationDate = pszStringDate;
	}

	strSessionID = ai_strLoginName;
	strSessionID.append(1, M4SessionIDFieldSeparator);
	
	//Campo para el nombre del server que crea el id de sesion
	strSessionID.append( ClIdGenerator::GetInstanceName() );
	strSessionID.append(1, M4SessionIDFieldSeparator);


	//id generado por el server que crea la credencial
	sprintf ( pszSessId , "%d" , (m4uint32_t) ClIdGenerator::GetNewId () ) ;
	m4string_t strId = pszSessId ;
	strSessionID.append( strId );
	strSessionID.append(1, M4SessionIDFieldSeparator);

	//Host del servidor
	ClIdGenerator::GetServerKey (strHost , strBasePort) ;
	strSessionID.append( strHost );
	strSessionID.append(1, M4SessionIDFieldSeparator);

	//Puerto base del servidor
	strSessionID.append( strBasePort);
	strSessionID.append(1, M4SessionIDFieldSeparator);

	strSessionID.append(pExpirationDate);

	m4string_t		strDigest;
	if (M4_SUCCESS != TStringUtils::GetDigest(strSessionID, strDigest))
		return M4_ERROR;

	strSessionID.append(1, M4SessionIDFieldSeparator);
	strSessionID.append(strDigest);
	strSessionID.append(1, M4SessionIDEndSeparator);
	
	ao_strSessionID = strSessionID;

	return M4_SUCCESS;
  //## end ClRawSessionIDGenerator::CreateSessionID%962964030.body
}

m4return_t ClRawSessionIDGenerator::ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const
{
  //## begin ClRawSessionIDGenerator::ExtractVariables%962964031.body preserve=yes
	m4string_t::size_type	iTotalLength, iStartPos;

	iStartPos = 0;
	iTotalLength = ai_strSessionID.find(M4SessionIDEndSeparator);

	if ( iTotalLength > strlen ( ai_strSessionID.c_str () ) )
	{
		return M4_ERROR ;
	}

	m4return_t		iResult;
	m4string_t		strUsername;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strUsername);
	if (M4_SUCCESS != iResult)
		return iResult;

	m4string_t strServerInstanceName ;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strServerInstanceName);
	if (M4_SUCCESS != iResult)
		return iResult;

	m4string_t strServerSessionId  ;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strServerSessionId );
	if (M4_SUCCESS != iResult)
		return iResult;

	m4string_t strHost  ;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strHost );
	if (M4_SUCCESS != iResult)
		return iResult;

	m4string_t strBasePort  ;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strBasePort);
	if (M4_SUCCESS != iResult)
		return iResult;

	m4string_t		strExpirationToken;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDFieldSeparator, strExpirationToken);
	if (M4_SUCCESS != iResult)
		return iResult;
	
	m4date_t	oCurrentDate, oExpirationDate;
	ClStringToDate( strExpirationToken.c_str(), oExpirationDate) ;
	ClActualDate( oCurrentDate ) ;
	if (oCurrentDate > oExpirationDate)
	{
		return M4DateExpired;
	}

	m4string_t		strSessionID = ai_strSessionID;
	strSessionID.erase(iStartPos-1);

	m4string_t		strDigest;
	iResult = TStringUtils::GetToken(ai_strSessionID, iStartPos, iTotalLength, M4SessionIDEndSeparator, strDigest);
	if (M4_SUCCESS != iResult)
		return iResult;

	iResult = TStringUtils::CheckDigest(strSessionID, strDigest);
	if (M4_SUCCESS != iResult)
		return iResult;

	ao_strUsername.swap(strUsername );
	ao_strServerName.swap (strServerInstanceName );
	ao_strServerSessionID.swap ( strServerSessionId   ) ;
	ao_strServerHost.swap ( strHost ) ;
	ao_iServerBasePort.swap ( strBasePort   ) ;
	return M4_SUCCESS;
  //## end ClRawSessionIDGenerator::ExtractVariables%962964031.body
}

// Additional Declarations
  //## begin ClRawSessionIDGenerator%39633B7B02DE.declarations preserve=yes
  //## end ClRawSessionIDGenerator%39633B7B02DE.declarations

//## begin module%39633C20039A.epilog preserve=yes
//## end module%39633C20039A.epilog
