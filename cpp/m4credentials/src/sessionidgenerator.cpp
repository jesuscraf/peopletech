//## begin module%3965B2650162.cm preserve=no
//## end module%3965B2650162.cm

//## begin module%3965B2650162.cp preserve=no
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
//## end module%3965B2650162.cp

//## Module: SessionIDGenerator%3965B2650162; Subprogram body
//## Subsystem: M4Credentials::src%3959D1180378
//## Source file: F:\integration\m4credentials\src\sessionidgenerator.cpp

//## begin module%3965B2650162.additionalIncludes preserve=no
//## end module%3965B2650162.additionalIncludes

//## begin module%3965B2650162.includes preserve=yes
//## end module%3965B2650162.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// SessionIDGenerator
#include <sessionidgenerator.hpp>
// clarraylibres
#include <clarraylibres.hpp>
//## begin module%3965B2650162.declarations preserve=no
//## end module%3965B2650162.declarations

//## begin module%3965B2650162.additionalDeclarations preserve=yes
//## end module%3965B2650162.additionalDeclarations


// Class ISessionIDGenerator 

//## begin ISessionIDGenerator::M4DateExpired%396B4051031A.attr preserve=no  public: static m4return_t {VAC} -2
const m4return_t  ISessionIDGenerator::M4DateExpired = -2;
//## end ISessionIDGenerator::M4DateExpired%396B4051031A.attr


// Class ClDefaultSessionIDGenerator 

//## begin ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator%39606E180244.role preserve=no  public: static ClDefaultSessionIDGenerator { -> 1VHANC}
const ClDefaultSessionIDGenerator  ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator;
//## end ClDefaultSessionIDGenerator::M4DefaultSessionIDGenerator%39606E180244.role


ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator ()
  //## begin ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.hasinit preserve=no
  //## end ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.hasinit
  //## begin ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.initialization preserve=yes
  //## end ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.initialization
{
  //## begin ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.body preserve=yes
  //## end ClDefaultSessionIDGenerator::ClDefaultSessionIDGenerator%962621267.body
}



//## Other Operations (implementation)
m4return_t ClDefaultSessionIDGenerator::CreateSessionID (const m4string_t &ai_strLoginName, const m4uint_t &ai_oSecondsFromNow, m4string_t &ao_strSessionID) const
{
  //## begin ClDefaultSessionIDGenerator::CreateSessionID%962621268.body preserve=yes
	if (ai_strLoginName.empty())
		return M4_ERROR;

	ao_strSessionID = ai_strLoginName;

	return M4_SUCCESS;
  //## end ClDefaultSessionIDGenerator::CreateSessionID%962621268.body
}

m4return_t ClDefaultSessionIDGenerator::ExtractVariables (const m4string_t &ai_strSessionID, m4string_t &ao_strUsername, m4string_t &ao_strServerName, m4string_t &ao_strServerSessionID, m4string_t &ao_strServerHost, m4string_t &ao_iServerBasePort) const
{
  //## begin ClDefaultSessionIDGenerator::ExtractVariables%962621269.body preserve=yes
	if (ai_strSessionID.empty())
		return M4_ERROR;

	ao_strUsername = ai_strSessionID;

	return M4_SUCCESS;
  //## end ClDefaultSessionIDGenerator::ExtractVariables%962621269.body
}

//## begin module%3965B2650162.epilog preserve=yes
//## end module%3965B2650162.epilog
