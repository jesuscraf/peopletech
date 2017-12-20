//## begin module%39DA02A303B0.cm preserve=no
//## end module%39DA02A303B0.cm

//## begin module%39DA02A303B0.cp preserve=no
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
//## end module%39DA02A303B0.cp

//## Module: xmlrequest%39DA02A303B0; Package body
//## Subsystem: ssccsubsystems::src%38D7880B0333
//## Source file: F:\integration\m4comsrv\src\xmlrequest.cpp

//## begin module%39DA02A303B0.additionalIncludes preserve=no
//## end module%39DA02A303B0.additionalIncludes

//## begin module%39DA02A303B0.includes preserve=yes
//## end module%39DA02A303B0.includes

// clbasedictionary
#include <clbasedictionary.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// xmlrequest
#include <xmlrequest.hpp>
//## begin module%39DA02A303B0.declarations preserve=no
//## end module%39DA02A303B0.declarations

//## begin module%39DA02A303B0.additionalDeclarations preserve=yes
//## end module%39DA02A303B0.additionalDeclarations


// Class ClXMLRequest 


ClXMLRequest::ClXMLRequest (m4pchar_t ai_pBuffer)
  //## begin ClXMLRequest::ClXMLRequest%970572583.hasinit preserve=no
  //## end ClXMLRequest::ClXMLRequest%970572583.hasinit
  //## begin ClXMLRequest::ClXMLRequest%970572583.initialization preserve=yes
  //## end ClXMLRequest::ClXMLRequest%970572583.initialization
{
  //## begin ClXMLRequest::ClXMLRequest%970572583.body preserve=yes
	m_pBuffer = ai_pBuffer ;
  //## end ClXMLRequest::ClXMLRequest%970572583.body
}


ClXMLRequest::~ClXMLRequest ()
{
  //## begin ClXMLRequest::~ClXMLRequest%970572584.body preserve=yes
	if ( m_pBuffer  )
		delete m_pBuffer ;
  //## end ClXMLRequest::~ClXMLRequest%970572584.body
}



//## Other Operations (implementation)
m4bool_t ClXMLRequest::CanDestroy ()
{
  //## begin ClXMLRequest::CanDestroy%970572585.body preserve=yes
	return M4_TRUE ;
  //## end ClXMLRequest::CanDestroy%970572585.body
}

m4bool_t ClXMLRequest::CanRecycle ()
{
  //## begin ClXMLRequest::CanRecycle%970572586.body preserve=yes
	return M4_TRUE ;
  //## end ClXMLRequest::CanRecycle%970572586.body
}

m4return_t ClXMLRequest::Destroy ()
{
  //## begin ClXMLRequest::Destroy%970572587.body preserve=yes
	delete this ;

	return M4_SUCCESS ;	
  //## end ClXMLRequest::Destroy%970572587.body
}

m4return_t ClXMLRequest::Recycle ()
{
  //## begin ClXMLRequest::Recycle%970572588.body preserve=yes
	if ( m_pBuffer  )
	{
		delete m_pBuffer ;
		m_pBuffer = NULL ;
	}

	m_lType = 0 ;

	m_lId = 0 ;

	return M4_SUCCESS ;
  //## end ClXMLRequest::Recycle%970572588.body
}

// Additional Declarations
  //## begin ClXMLRequest%39D9C37B0126.declarations preserve=yes
  //## end ClXMLRequest%39D9C37B0126.declarations

//## begin module%39DA02A303B0.epilog preserve=yes
//## end module%39DA02A303B0.epilog
